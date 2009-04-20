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
#include "Connection.hpp"

#ifndef BASIC__DML_HPP
#define BASIC__DML_HPP
/*!  BasicDML
  
  -----------
  
  Basic DML methods
  
  Author: Ralf Duenkelmann
  
  Date :  01.06.2004
*/
class BasicDML:public Conn
{
  private:
  Statement *stmt;  // Statement
  string sqlStmt;   // the sql itself with or without binds
  string bind[255]; // the bind values
  string bind_type[255]; // the bind datatypes

  public:
  
  /*!  Def Constructor
  */
  BasicDML ();

  /*!  Constructor
   * @param user     username of Database
   * @param pass     password 
   * @param db       the database alias
  */
  BasicDML (string user, string pass, string db);
  
  /*!  displayRows
   * 
   * execute and display the result of a select 
   * 
   * @param selectCount     number of select attributes
   * @param bindCount       number of binds in select
   * @return                the result as string 
  */
  string displayRows (int selectCount,unsigned int bindCount);

  /*!  setSQLStmt
   * 
   * set the SQL to be executed 
   * 
   * @param s               the SQL
  */
  void setSQLStmt (string s);

  /*!  setBind
   * 
   * set a Bind  
   * 
   * @param pos               the position in SQL 
   * @param datatype          the datatype of the bind, valid are "string" and "int" at the moment
   * @param wert              the actual value of the bind
  */
  void setBind(int pos, string datatype , string wert) ;
  
  /*!  InsertRow
   * 
   * use InsertRow for updates, inserts etc, which do not return values 
   * 
   * @param pSql               the SQL 
  */
  int InsertRow(string pSql);
  
  /*!  InsertRow
   * 
   * use InsertRow for updates, inserts etc, which do not return values 
   * 
  */
  int InsertRow(void);
  
  /*!  Commit
  */
  void Commit(void);
  
  /*!  Rollback
  */
  void Rollback(void);
};

#endif
