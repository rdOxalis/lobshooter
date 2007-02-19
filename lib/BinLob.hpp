//  ************************************************
//  oraload
//  -----------
//  Upload and Download of Binary Lobs
//  to an Oracle Server
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
#ifndef BIN_LOB_HPP
#define BIN_LOB_HPP
class  BinLob:public Lob
{
  public:
  BinLob ();
  BinLob (char* user , char* pass , char* db, char* cfile);
  BinLob (char* user , char* pass , char* db);
  ~BinLob();
  int DownloadBlobData(void);
  int UploadBlobData(void);
};//end of class
#endif
