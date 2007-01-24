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
      return(-1);
    }
    conn->terminateStatement (stmt);
    return(0);
  }
}

int BasicDML::InsertRow(void)
{
  this->InsertRow(this->sqlStmt);
}

/**
 * displaying rows from table
 * FIXME
 */
string BasicDML::displayRows (int selectCount,int bindCount)
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
    }
    stmt->closeResultSet (rset);
    conn->terminateStatement (stmt);
    return(ret);
  }
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

