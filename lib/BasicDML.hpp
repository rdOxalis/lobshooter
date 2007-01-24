#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
#include "Connection.hpp"

#ifndef BASIC__DML_HPP
#define BASIC__DML_HPP
class BasicDML:public Conn
{
  private:
  Statement *stmt;  // Statement
  string sqlStmt;   // the sql itself with or without binds
  string bind[255]; // the bind values
  string bind_type[255]; // the bind datatypes

  public:
  BasicDML ();
  BasicDML (string user, string pass, string db);
  string displayRows (int selectCount,int bindCount);
  void setSQLStmt (string s);
  void setBind(int pos, string datatype , string wert) ;
  int InsertRow(string pSql);
  int InsertRow(void);
  void Commit(void);
  void Rollback(void);
};

#endif
