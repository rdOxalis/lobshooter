//  ************************************************
//  lobshooter
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
//  20.04.2009 Char Set stuff, GUI
// ************************************************

//  ************************************************
//
//    <lobshooter - loads and gets lobs (Large Objects) into and out of an Oracle Database> 
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

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <time.h>
using namespace std;

#include <cstdint>   // Oracle's occiControl.h uses uint8_t without including it
#include <occi.h>
#include "CharLob.hpp"
#include "BinLob.hpp"
#include "Log.hpp"
//#include "LobWizard.h"

using namespace oracle::occi;

static string const VERSION("0.2.2");
static string vLogFile("lobshooter.log");
static string vCharSet("UTF8");

int multifile = 0;

void Version(){
 cout << "lobshooter." << VERSION << endl;
}

void Usage(char* vProg){
 cout << "Usage: " << vProg << " [-v] user pass db DC|UC|DB|UB SqlString filename[,file2,...] [options]" << endl;
 cout << "                  " << " DC:Download Clob     UC:Upload Clob" << endl;
 cout << "                  " << " DB:Download Blob     UB:Upload Blob" << endl;
 cout << "Options: " << endl;
 cout << "-l  logfile_name     Assign Log File Name (default /tmp/lobshooter.log) " << endl;
 cout << "-c  CharSet          Assign CharSet for CharLobs (default UTF8) " << endl;
 cout << "-v                   Show version number " << endl;
 }

int main(int argc, char *argv[])
{
  
  if ( argc == 1 ) {
    Usage(argv[0]);
    return (-1);
  }

/*  if ( strcmp(argv[1],"-gui") == 0 ){
	QApplication app(argc, argv);
    LobWizard *wiz = new LobWizard;
    wiz->show();
	wiz->setWindowState(Qt::WindowActive);
	wiz->raise();
	
    return app.exec();

  }
*/
  	
  if ( ( argc == 2) && (( (strcmp(argv[1],"-v") == 0) || (strcmp(argv[1],"--version") == 0) ))  ) {
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
	cout << "Logfile set to " <<  vLogFile << endl;
      }
      if(!strcmp(option,"-c")){
        // Assign different CharSet to char lob
        i++;
        if ( argc < (i+1) ) {
          Usage(argv[0]);
          return (-1);
        }
        vCharSet.assign(argv[i]);
	cout << "Charset set to " <<  vCharSet << endl;
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
	CL->WriteLogFile("Connection failed, invalid login?");
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
	BL->WriteLogFile("Connection failed, invalid login?");
	return (-9);
    }
  }

  // Der Rueckgabewert der Transfermethoden wird nach aussen durchgereicht,
  // damit aufrufende Skripte Fehler ueberhaupt bemerken koennen.
  int rc = 0;
  // Upload of CharData
  if (strcmp(argv[4],"UC") == 0){
    rc = CL->UploadClobData();
  }
  // Download of CharData
  if (strcmp(argv[4],"DC") == 0){
    rc = CL->DownloadClobData();
  }
  // Upload of BinData
  if (strcmp(argv[4],"UB") == 0){
    rc = BL->UploadBlobData();
  }
  // Download of BinData
  if (strcmp(argv[4],"DB") == 0){
    rc = BL->DownloadBlobData();
  }
  return rc;
}
