//  ************************************************
//  oraload
//  -----------
//  Upload and Download of Lobs to an Oracle Server
//
//  Author: Ralf Dnkelmann
//  Date :  01.06.2004
//
//  CHANGES
//  11.11.2004 R.Dnkelmann
//             - making use of OCCI
//             - CLOB management
//  08.12.2004 removed old OCI functions
//  23.01.2007 usage
// ************************************************

/*! \mainpage LobShooter for Oracle (oraload)
 *
 * \section intro_sec Introduction
 *
 * LobShooter for Oracle (oraload) is a utility to easily up- and download Lob-Files 
 * to and from an Oracle database. You are able to directly load binary (blob) and char data with it.
 * oraload comes with a library (libloadutil) and the main program.
 * For the moment oraload is a command line utility, that takes parameters like this
 *
 * oraload user pass db DC|UC|DB|UB SqlString filename [logfile]
 *
 *
 *  , where 
 *
 *  - user = database user
 *  - pass = database password
 *  - db   = database tnsname
 *  - DC   = Download a char lob
 *  - UC   = Upload a char lob
 *  - DB   = Download a bin lob
 *  - UB   = Upload a bin lob
 *  - SqlString = the sql which sais where to put the lob (or where to get it from)
 *  - filename = the filename for input or output (the file that you want to make a database lob from)
 *  - logfile = path to the file where oraload should put the logging infos.
 *
 * \section install_sec Installation
 * Prerequisites:
 * 
 * You need a working Oracle-Install, i.e
 *
 * - Oracle Server 
 * - Oracle Client (you may compile the sources, of course you need a server somewhere to get it to work)
 * - Oracle XE
 * - Oracle Instant Client
 *
 * You need cmake for the build process. Try "apt-cache search cmake" and then install from your Debian-Repositories. 
 *
 * The build-process:
 * - CD to the source directory and type 
 * - cmake .  (that is a dot here)
 * - make all
 * - make install (as root). 
 *
 * \subsection Examples
 * oraload hr hr xe UB "select blob_field from blob_table where pk_field = 4711 for update" music.mp3
 *
 * - note that you always have to lock the row with "for update", if you upload something. You do not 
 *   need that when downloading.
 *
 * \subsection Using Using the library
 * You can use the library to implement in your own programs. A good starting point is the test-directory
 * with the unit test sources. For a better understanding of classes and methods continue reading this documentation 
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
using namespace std;

// rd 111104
#include <occi.h>
#include "CharLob.hpp"
#include "BinLob.hpp"

using namespace oracle::occi;


static OCIEnv        *envhp;
static OCIError      *errhp;
static OCIServer     *srvhp;
static OCISvcCtx     *svchp;
static OCISession    *authp = (OCISession *) 0;
static OCIStmt       *stmthp;
static sword         rc;
static string        vLogFile("/tmp/oraload.log");

//  ************************************************
//  function GetTime:
/*!
    gets local time.
    \return calculated time as String Object (dd.mm.yyyy)
*/
// ************************************************
string GetTime()
{
  time_t t ;

  t = time(NULL);  // aktuelle Zeit in t

  // mit tm struct weiter arbeiten
  tm *   tpointer = localtime (&t); // zeiger auf tm struct

  char DS[20];
  strcpy(DS , "");
  size_t strsize = sizeof(DS);
  strsize = strftime(DS, strsize, "%d.%m.%Y %H:%M:%S", tpointer);
  return(string(DS));
}

void FlushLogFile()
{
  std::ofstream out(vLogFile.c_str(), std::ios_base::trunc);
  out.close();
}

int WriteLogFile(string pLog)
{
  std::ofstream out(vLogFile.c_str(), std::ios_base::app);
  if ( !out ) {
    perror("Outputdatei kann nicht zum Schreiben geöffnet werden!");
    return (EXIT_FAILURE);
  }

  out << GetTime() << " :: " << pLog.c_str() << "\n";
  out.close();
  // Written? Sure?
  if (! out) {
    return (EXIT_FAILURE);
  }
  return(0);
}


void CheckErr(OCIError* errhp, sword status, string mark)
{
  text errbuf[2048];
  sb4 errcode = 0;
  string s;
  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
 	s.assign("Error - OCI_SUCCESS_WITH_INFO at ");
	s += mark.c_str();
	s += "\n";
    WriteLogFile(s);
    break;
  case OCI_NEED_DATA:
 	s.assign("Error - OCI_NEED_DATA at ");
	s += mark.c_str();
	s += "\n";
    WriteLogFile(s);
    break;
  case OCI_NO_DATA:
 	s.assign("Error - OCI_NODATA at ");
	s += mark.c_str();
	s += "\n";
    WriteLogFile(s);
    break;
  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
 	s.assign((char*)errbuf);
	s += "                       at ";
	s += mark.c_str();
    WriteLogFile(s);

    break;
  case OCI_INVALID_HANDLE:
 	s.assign("Error - OCI_INVALID_HANDLE at ");
	s += mark.c_str();
    WriteLogFile(s);
    break;
  case OCI_STILL_EXECUTING:
 	s.assign("Error - OCI_STILL_EXECUTE at ");
	s += mark.c_str();
    WriteLogFile(s);
    break;
  case OCI_CONTINUE:
 	s.assign("Error - OCI_CONTINUE at ");
	s += mark.c_str();
    WriteLogFile(s);
    break;
  default:
    break;
  }
}


void Usage(char* vProg){
 cout << "Usage: " << vProg << " user pass db DC|UC|DB|UB SqlString filename [logfile]" << endl;
 cout << "                  " << " DC:Download Clob     UC:Upload Clob" << endl;
 cout << "                  " << " DB:Download Blob     UB:Upload Blob" << endl;
}

int main(int argc, char *argv[])
{
  cout << argc;
  if ( argc < 7 or argc > 8 ) {
    Usage(argv[0]);
    return (-1);
  }
  if ( argc == 8 ) {
    vLogFile.assign(argv[7]);
  }

  FlushLogFile();
  WriteLogFile(argv[0]);

  int rc;

  // Upload of CharData
  if (strcmp(argv[4],"UC") == 0
      ){
    CharLob CL(argv[1],argv[2],argv[3]);
    CL.connect();
    CL.setFilename(argv[6]);
    CL.setSqlLocator(argv[5]);
    string sqlLoc(argv[5]);
    CL.UploadClobData();
  }

  // Download of CharData
  if (strcmp(argv[4],"DC") == 0
      ){
    CharLob CL(argv[1],argv[2],argv[3]);
    CL.connect();
    CL.setFilename(argv[6]);
    CL.setSqlLocator(argv[5]);
    string sqlLoc(argv[5]);
    CL.DownloadClobData();
  }

  // Upload of BinData
  if (strcmp(argv[4],"UB") == 0
      ){
    BinLob BL(argv[1],argv[2],argv[3]);
    BL.connect();
    BL.setFilename(argv[6]);
    BL.setSqlLocator(argv[5]);
    string sqlLoc(argv[5]);
    BL.UploadBlobData();
  }

  // Download of BinData
  if (strcmp(argv[4],"DB") == 0
      ){
    BinLob BL(argv[1],argv[2],argv[3]);
    BL.connect();
    BL.setFilename(argv[6]);
    BL.setSqlLocator(argv[5]);
    BL.DownloadBlobData();
  }


  return 0;

}
