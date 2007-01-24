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


