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

#include "Lob.hpp"
int Lob::InitLob(string pField , string pTable , string pWhere){
  if (this->getConnectorType() == "Oracle"){
    string sqlStmt = "update " + pTable + " set " + pField + " = empty_blob() where " + pWhere;
    Statement *stmt = conn->createStatement (sqlStmt);
    try{
      stmt->executeUpdate ();
      cout << "InitLob - Success" << endl;
    }
    catch(SQLException ex)
    {
      cout<<"Exception thrown for InitLob"<<endl;
      cout<<"Error number: "<<  ex.getErrorCode() << endl;
      cout<<ex.getMessage() << endl;
      return(-1);
    }
    conn->terminateStatement (stmt);
    return(0);
  }
  return(0);
} // InitLob

void Lob::setFilename (string f)
{
  filename = f;
}

void Lob::setSqlLocator (string sl)
{
  // for inserting a CLOB sqlLocator MUST be an insert string and
  // the first bind must be the CLOB.
  // Dont forget the locking via "for update" - clause!!
  sqlLocator = sl;
}


