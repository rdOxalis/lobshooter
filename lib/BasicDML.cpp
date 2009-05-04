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

#include"BasicDML.hpp"
BasicDML::BasicDML ()
{
}

BasicDML::BasicDML (string user, string pass, string db)
{
   username = user;
   password = pass;
   url = db;
}

int BasicDML::InsertRow(string pSql){
  if (this->getConnectorType() == "Oracle"){
    string sqlStmt = pSql;
    Statement *stmt = conn->createStatement (sqlStmt);
    try{
      stmt->executeUpdate ();
      cout << "InsertRow - Success" << endl;
    }
    catch(SQLException ex)
    {
      cout<<"Exception thrown for InsertRow"<<endl;
      cout<<"Error number: "<<  ex.getErrorCode() << endl;
      cout<<ex.getMessage() << endl;
      this->WriteLogFile("Exception thrown for InsertRow");
      this->WriteLogFile(ex.getMessage());
      return(-1);
    }
    conn->terminateStatement (stmt);
    return(0);
  }
  return(0);
}

int BasicDML::InsertRow(void)
{
  this->InsertRow(this->sqlStmt);
  return(0);
}

string BasicDML::getString(unsigned int bindCount)
{
    string ret;
    stmt = conn->createStatement (this->sqlStmt);
    for (unsigned int i=0;i<bindCount;i++)
    {
      try{
      if ( bind_type[i] == "int" )
        stmt->setInt(i+1 , atoi(bind[i].c_str()));
      if ( bind_type[i] == "string" )
        stmt->setString(i+1 , bind[i]);
      }catch(SQLException ex)
      {
       cout<<"Exception thrown when binding"<<endl;
       cout<<"Error number: "<<  ex.getErrorCode() << endl;
       cout<<ex.getMessage() << endl;
       this->WriteLogFile("Exception thrown when binding");
       this->WriteLogFile(ex.getMessage());
       return ("");
      }  
    }
    ResultSet *rset = stmt->executeQuery ();
    try{
	    // just get the first line >(since this function is only useful for single value queries
	    rset->next();
	    string erg;
	    erg = rset->getString(1);
	    ret = erg;
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
     this->WriteLogFile("Exception thrown for displayRows");
     this->WriteLogFile(ex.getMessage());
     return ("");
    }
    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);
    return(ret);
}

string BasicDML::getStringList(unsigned int bindCount)
{
    string ret;
    stmt = conn->createStatement (this->sqlStmt);
    for (unsigned int i=0;i<bindCount;i++)
    {
      try{
      if ( bind_type[i] == "int" )
        stmt->setInt(i+1 , atoi(bind[i].c_str()));
      if ( bind_type[i] == "string" )
        stmt->setString(i+1 , bind[i]);
      }catch(SQLException ex)
      {
       cout<<"Exception thrown when binding"<<endl;
       cout<<"Error number: "<<  ex.getErrorCode() << endl;
       cout<<ex.getMessage() << endl;
       this->WriteLogFile("Exception thrown when binding");
       this->WriteLogFile(ex.getMessage());
       return ("");
      }  
    }
    ResultSet *rset = stmt->executeQuery ();
    try{
	    // just get the first line >(since this function is only useful for single value queries
	    while (rset->next())
		{
	      string erg;
	      erg = rset->getString(1);
		  if ( ret == "" )
	        ret = erg;
		  else{
		    ret = ret.append(",");
			ret = ret.append(erg);
		  }	
		} 
		
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
     this->WriteLogFile("Exception thrown for displayRows");
     this->WriteLogFile(ex.getMessage());
     return ("");
    }
    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);
    return(ret);
}

int BasicDML::getInt(unsigned int bindCount)
{
    int ret; 
    stmt = conn->createStatement (sqlStmt);
    for (unsigned int i=0;i<bindCount;i++)
    {
      try{
      if ( bind_type[i] == "int" )
        stmt->setInt(i+1 , atoi(bind[i].c_str()));
      if ( bind_type[i] == "string" )
        stmt->setString(i+1 , bind[i]);
      }catch(SQLException ex)
      {
       cout<<"Exception thrown when binding"<<endl;
       cout<<"Error number: "<<  ex.getErrorCode() << endl;
       cout<<ex.getMessage() << endl;
       this->WriteLogFile("Exception thrown when binding");
       this->WriteLogFile(ex.getMessage());
       return (-3);
      }  
    }
    ResultSet *rset = stmt->executeQuery ();
    try{
	    // just get the first line >(since this function is only useful for single value queries
	    rset->next();
	    int erg;
	    erg = rset->getNumber(1);
	    ret = erg;
    }catch(SQLException ex)
    {
     cout<<ex.getMessage() << endl;
     this->WriteLogFile("Exception thrown for displayRows");
     this->WriteLogFile(ex.getMessage());
     return (-1);
    }
    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);
    return(ret);
}

/**
 * displaying rows from table
 * FIXME
 */
string BasicDML::displayRows (int selectCount,unsigned int bindCount)
{
  if (this->getConnectorType() == "Oracle"){
    string ret; 
    stmt = conn->createStatement (sqlStmt);
    for (unsigned int i=0;i<bindCount;i++)
    {
      try{
      if ( bind_type[i] == "int" )
        stmt->setInt(i+1 , atoi(bind[i].c_str()));
      if ( bind_type[i] == "string" )
        stmt->setString(i+1 , bind[i]);
      }catch(SQLException ex)
      {
       cout<<"Exception thrown when binding"<<endl;
       cout<<"Error number: "<<  ex.getErrorCode() << endl;
       cout<<ex.getMessage() << endl;
       this->WriteLogFile("Exception thrown when binding");
       this->WriteLogFile(ex.getMessage());
       return ("");
      }  
    }
    ResultSet *rset = stmt->executeQuery ();
    try{
    cout.precision(7);
    // FIXME Speicherzugriffsfehler bei next()
    while (rset->next ())
    {
      string erg[255];
      for (int i=0;i<selectCount;i++)
      {
        erg[i] = rset->getString(i+1);
        cout <<  erg[i] << '|';
        ret = (ret.append(erg[i])).append("|");
      }
    }
    }catch(SQLException ex)
    {
     cout<<"Exception thrown for displayRows"<<endl;
     cout<<"Error number: "<<  ex.getErrorCode() << endl;
     cout<<ex.getMessage() << endl;
     this->WriteLogFile("Exception thrown for displayRows");
     this->WriteLogFile(ex.getMessage());
     return ("");
    }
    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);
    return(ret);
  }
  return("");
}


void BasicDML::setSQLStmt (string s)
{
  sqlStmt = s;
}

void BasicDML::setBind(int pos, string datatype , string wert)
{
  if (datatype == "int")
    bind_type[pos]= "int";
  if (datatype == "string")
    bind_type[pos]= "string";
  bind[pos] = wert;
}

void BasicDML::Rollback(void)
{
  this->setSQLStmt("rollback");
  InsertRow();
}  

void BasicDML::Commit(void)
{
  this->setSQLStmt("commit");
  InsertRow();
}  

