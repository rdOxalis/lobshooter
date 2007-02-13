//  ************************************************
//  oraload
//  -----------
//  Upload and Download of Lobs to an Oracle Server
//
//  Author: Ralf D?nkelmann
//  Date :  01.06.2004
//
//  CHANGES
//  11.11.2004 R.Dnkelmann
//             this file :
//               - making use of OCCI
//               - CLOB management
// ************************************************

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
#include "Lob.hpp"
#ifndef CHAR_LOB_HPP
#define CHAR_LOB_HPP
class CharLob:public Lob
{

  public:
  CharLob (char* user , char* pass , char* db, char* cfile);
  CharLob (char* user , char* pass , char* db);
  ~CharLob ();
  int DownloadClobData(void);
  int UploadClobData(void);

};//end of class
#endif
