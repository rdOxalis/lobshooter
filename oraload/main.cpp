//  ************************************************
//  oraload
//  -----------
//  Upload and Download of Lobs to an Oracle Server
//
//  Author: Ralf Duenkelmann
//  Date :  01.06.2004
//
//  CHANGES
//  11.11.2004 R.Duenkelmann
//             - making use of OCCI
//             - CLOB management
//  08.12.2004 removed old OCI functions
//  23.01.2007 usage
//  06.06.2007 Logging
//  07.06.2007 Multifile-Support (experimental)
// ************************************************

//  ************************************************
//
//    <oraload - loads and gets lobs (Large Objects) into and out of an Oracle Database> 
//    Copyright (C) <2004-2007> <Ralf Duenkelmann>
//
//    This library is free software; you can redistribute it and/or modify it under the terms of the 
//    GNU Lesser General Public License as published by the Free Software Foundation; 
//    either version 2.1 of the License, or (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful, 
//    but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
//    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

//    You should have received a copy of the GNU Lesser General Public License along with this library; 
//    if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
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
 * oraload [-v] user pass db DC|UC|DB|UB SqlString [-m|-ml] filename[,file2,...] [options]
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
 *               in case you use option -ml, filename holds the list of files to be processed
 * 
 *  Options
 *  - -l logfile_name => path to the file where oraload should put the logging infos.
 *  - -v show version number
 *  - -m filename [,file2,...] multifile support
 *  - -ml filename => in this case filename holds a simple list of files to be processed
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
 * - oraload hr hr //lynx:1521/xe DC "select blob_field from blob_table where pk_field = 4711" music.mp3 -l ./log.txt
 *
 * \subsection Using Using the library
 * You can use the library to implement in your own programs. A good starting point is the test-directory
 * with the unit test sources. For a better understanding of classes and methods continue reading this documentation 
 *
 */

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <time.h>
using namespace std;

#include <occi.h>
#include "CharLob.hpp"
#include "BinLob.hpp"
#include "Log.hpp"
#include "Lua.hpp"

using namespace oracle::occi;

static string const VERSION("0.2");
static string vLogFile("oraload.log");
static string vCharSet("UTF8");

int multifile = 0;

void Version(){
 cout << "oraload." << VERSION << endl;
}

void Usage(char* vProg){
 cout << "Usage: " << vProg << " oraload [-v] user pass db DC|UC|DB|UB SqlString [-m|-ml] filename[,file2,...] [options]" << endl;
 cout << "                  " << " DC:Download Clob     UC:Upload Clob" << endl;
 cout << "                  " << " DB:Download Blob     UB:Upload Blob" << endl;
 cout << "Options: " << endl;
 cout << "-l  logfile_name     Assign Log File Name (default /tmp/oraload.log) " << endl;
 cout << "-c  CharSet          Assign CharSet for CharLobs (default UTF8) " << endl;
 cout << "-v                   Show version number " << endl;
 cout << "-m  filename [,file2,...] multifile support" << endl;
 cout << "-ml filename => in this case filename holds a simple list of files to be processed" << endl;
 }

int main(int argc, char *argv[])
{
  	
  if ( ( argc == 2) && (( (strcmp(argv[1],"-v") == 0) || (strcmp(argv[1],"--version") == 0) ))  ) {
    Version();
    return (1);
  }

  if ( argc < 7 ) {
    Usage(argv[0]);
    return (-1);
  }

  if ((strcmp(argv[6],"-m") == 0 || strcmp(argv[6],"-ml") == 0))  {
  	// Multifile Support
  	multifile = 1;
    Lua LuaInterpretor;
    LuaInterpretor.init();
    int rc = LuaInterpretor.do_file("/usr/local/lib/multifile.lua");
    LuaInterpretor.close();
    cout << "RC" << rc << endl;;
    return (0);
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
      if(!strcmp(option,"-c")){
        // Assign different CharSet to char lob
        i++;
        if ( argc < (i+1) ) {
          Usage(argv[0]);
          return (-1);
        }
        vCharSet.assign(argv[i]);
      }
      else if(!strcmp(option,"-h")) {
        Usage(argv[0]);
        return (1);
      }
      else {
        Usage(argv[0]);
        return (1);
      }
    }
  }

    
  CharLob *CL;
  BinLob  *BL;

  if ( (strcmp(argv[4],"UC") == 0) || (strcmp(argv[4],"DC") == 0) ){
    CL = new CharLob(argv[1],argv[2],argv[3]);
    CL->setCharSet(vCharSet);
  	CL->setLogFile(vLogFile);
    CL->FlushLogFile();
    CL->WriteLogFile(argv[0]);
    if ( CL->connect() == 0 ){
      CL->setFilename(argv[6]);
      CL->setSqlLocator(argv[5]);
	}
	else{  // couldn't connect, returning -9
		return (-9);
	}
	
  }
  if ( (strcmp(argv[4],"UB") == 0) || (strcmp(argv[4],"DB") == 0) ){

    BL = new BinLob(argv[1],argv[2],argv[3]);
  	BL->setLogFile(vLogFile);
    BL->FlushLogFile();
    BL->WriteLogFile(argv[0]);
    if ( BL->connect() == 0 ) {
       BL->setFilename(argv[6]);
       BL->setSqlLocator(argv[5]);
    }
	else{  // couldn't connect, returning -9
		return (-9);
	}
	
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
