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
//
//    You should have received a copy of the GNU Lesser General Public License along with this library; 
//    if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
#include "Log.hpp"

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
class Conn:public Log
{
  protected:
  string ConnectorType;
  string username;
  string password;
  string url;
  oracle::occi::Connection *conn;   // The connection

  public:
  Conn ();
  Conn (string pConnectorType);
  ~Conn ();
  void setConnectorType (string c);
  string getConnectorType (void);

  void setUsername (string u);
  void setPassword (string p);
  void setUrl (string u);
  int connect(void);
};
#endif
