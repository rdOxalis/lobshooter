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
#include <cstring>
#include <occi.h>
using namespace oracle::occi;
using namespace std;

#ifndef LOG_HPP
#define LOG_HPP
class Log
{
  protected:
  
  public:
  string        vLogFile;  
  string GetTime();
  
  void setLogFile(string);
  void FlushLogFile();
  int WriteLogFile(string);
};
#endif
