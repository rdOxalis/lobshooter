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
 * oraload user pass db DC|UC|DB|UB SqlString filename [options]
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
 * 
 *  Options
 *  - -l logfile_name = path to the file where oraload should put the logging infos.
 *  - -v show version number
 *  - -m multiple file load (experimental)
 *
 * \section Download
 * Download from 
 * - http://sourceforge.net/projects/oraload/
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
 * You need cmake for the build process. Try "apt-cache search cmake" and then install from your Debian-Repositories 
 * or get it from cmake.org
 * 
 * Set your Oracle Home Environment variable 
 * 
 * - for a standard xe install this would be /usr/lib/oracle/xe/app/oracle/product/10.2.0/server
 * - for an InstantClient install this would be sth like /path/to/InstantClient_top_level_dir
 *
 * The build-process:
 * - CD to the source directory and type 
 * - cmake .  (that is a dot here)
 * - make all
 * - make install (as root). 
 *
 * On runtime, set your library path like this:
 * 
 * - LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORACLE_HOME/lib:$ORACLE_HOME/ctx/lib
 * - LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib:/usr/lib
 * - export LD_LIBRARY_PATH
 * 
 * \subsection Examples
 * - oraload hr hr xe UB "select blob_field from blob_table where pk_field = 4711 for update" music.mp3
 *
 * - note that you always have to lock the row with "for update", if you upload something. You do not 
 *   need that when downloading.
 * 
 * Example with EasyConnect (e.g. if you have InstantClient and no TNSNAMES.ora)
 * - oraload hr hr //lynx:1521/xe DC "select blob_field from blob_table where pk_field = 4711" music.mp3 ./log.txt
 *
 * \subsection Using Using the library
 * You can use the library to implement in your own programs. A good starting point is the test-directory
 * with the unit test sources. For a better understanding of classes and methods continue reading this documentation 
 *
 * 
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

static string const VERSION("0.1.2");

static OCIEnv        *envhp;
static OCIError      *errhp;
static OCIServer     *srvhp;
static OCISvcCtx     *svchp;
static OCISession    *authp = (OCISession *) 0;
static OCIStmt       *stmthp;
static sword         rc;
static string        vLogFile("/tmp/oraload.log");

int multifile = 0;

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

// FIXME log class needed
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

// FIXME Err-Class needed
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

void Version(){
 cout << "oraload." << VERSION << endl;
}

void Usage(char* vProg){
 cout << "Usage: " << vProg << " user pass db DC|UC|DB|UB SqlString filename [options]" << endl;
 cout << "                  " << " DC:Download Clob     UC:Upload Clob" << endl;
 cout << "                  " << " DB:Download Blob     UB:Upload Blob" << endl;
 cout << "Options: " << endl;
 cout << "-l logfile_name      Assign Log File Name (default /tmp/oraload.log) " << endl;
 cout << "-v                   Show version number " << endl;
 cout << "-m                   MultiFile Load (experimental) " << endl;
 }

int main(int argc, char *argv[])
{
  if ( argc == 2 and ( strcmp(argv[1],"-v") == 0 or strcmp(argv[1],"--version") == 0 )  ) {
    Version();
    return (1);
  }

  if ( argc < 7 ) {
    Usage(argv[0]);
    return (-1);
  }

  char* option;
  if(argc>7) {
    for(int i=7;i<argc;i++) {
      option=argv[i];
      if(!strcmp(option,"-l")){
        // Assign Log to different location
        i++;
        if ( argc < (i+1) ) {
          Usage(argv[0]);
          return (-1);
        }
        vLogFile.assign(argv[i]);
      }
      else if(!strcmp(option,"-h")) {
        Usage(argv[0]);
        return (1);
      }
      else if(!strcmp(option,"-m")) {
        // Multi File Load
        multifile = 1;
      }
      else {
        Usage(argv[0]);
        return (1);
      }
    }
  }

  FlushLogFile();
  WriteLogFile(argv[0]);
  
  //CharLob *CL = new CharLob();
  //BinLob  *BL = new BinLob();
  CharLob *CL;
  BinLob  *BL;

  if ( (strcmp(argv[4],"UC") == 0) or (strcmp(argv[4],"DC") == 0) ){
    CL = new CharLob(argv[1],argv[2],argv[3]);
    CL->connect();
    CL->setFilename(argv[6]);
    CL->setSqlLocator(argv[5]);
  }
  if ( (strcmp(argv[4],"UB") == 0) or (strcmp(argv[4],"DB") == 0) ){
    BL = new BinLob(argv[1],argv[2],argv[3]);
    BL->connect();
    BL->setFilename(argv[6]);
    BL->setSqlLocator(argv[5]);
  }

  // Upload of CharData
  if (strcmp(argv[4],"UC") == 0){
    CL->UploadClobData();
  }
  // Download of CharData
  if (strcmp(argv[4],"DC") == 0){
    CL->DownloadClobData();
  }
  // Upload of BinData
  if (strcmp(argv[4],"UB") == 0){
    BL->UploadBlobData();
  }
  // Download of BinData
  if (strcmp(argv[4],"DB") == 0){
    BL->DownloadBlobData();
  }


  return 0;

}
