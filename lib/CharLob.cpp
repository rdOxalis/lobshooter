#include "CharLob.hpp"

CharLob::CharLob (char* user , char* pass , char* db, char* cfile)
{
  /**
   * default values of username & password
   */
   username = user;
   password = pass;
   url = db;
   filename = cfile;
}

CharLob::CharLob (char* user , char* pass , char* db)
{
  /**
   * default values of username & password
   */
   username = user;
   password = pass;
   url = db;
}

CharLob::~CharLob ()
{
   cout << "Destroy";
}

int CharLob::DownloadClobData(void){
  if (this->getConnectorType() == "Oracle"){
    unsigned int bufsize=10000;
    char* buffer = new char[bufsize + 1];
    
    ofstream ofFile;
      ofFile.open((const char*)filename.c_str(),ios_base::out);
    
    if (!ofFile)
      {
        cout << "Could not open file for writing." << endl;
        delete[] buffer;
        return(-1);
      }
    try{
        Statement *stmt = conn->createStatement(sqlLocator);
        ResultSet *rset = stmt->executeQuery();
        cout << "Got ResultSet" << endl;
        while(rset->next())
        {
          Clob clob = rset->getClob(1);
          Stream *strm=clob.getStream();
          
          //strm->readBuffer(buffer,clob.length());
          int bytesRead = strm->readBuffer((char*)buffer,200);
          while (bytesRead>0)
          {
            for (int i=0;i<bytesRead;++i)
            {
              ofFile << buffer[i];
            }
            bytesRead = strm->readBuffer((char*)buffer,200);
          }
          
          clob.closeStream(strm);
          ofFile << buffer;
          ofFile.close();
          cout << "Getting the Clob - Success" << endl;
        }
    }
    catch (SQLException e){
      cout << e.getMessage() << endl;
      delete[] buffer;
      return(-3);
    }
    return(0);
  }
}

int CharLob::UploadClobData(void){
  if (this->getConnectorType() == "Oracle"){
    cout << "Populating the Clob" << endl;
    
    ifstream countFile;
    countFile.open((const char*)filename.c_str(),ios_base::in);
    if (!countFile)
    {
      cout << "file not found\n";
      //delete[] buffer;
      return(-1);
    }
    // determine length of file
    int i=0;
    while(countFile)
    {
      i++;
      countFile.ignore();
    }
    countFile.close();
    
    
    ifstream inFile;
    inFile.open((const char*)filename.c_str(),ios_base::in);
    // abcdefg[eof][aktuelle pos]  , daher -2
    unsigned int bufsize=i-2;//sizeof(char);
    char* buffer = new char[bufsize];
        
    unsigned int size;
    try{
      std::string sqlStat = sqlLocator;
      Statement *stmt = conn->createStatement(sqlStat);
      ResultSet *rset = stmt->executeQuery();
      cout << "Got ResultSet." << endl;
      while(rset->next())
      {
        Clob clob = rset->getClob(1);
        Stream *strm=clob.getStream();
        //memset (buffer, NULL, bufsize);
        memset (buffer, 0, bufsize);
        inFile.read(buffer,bufsize);
        strm->writeBuffer(buffer,bufsize);
        strcpy(buffer,"");
        size=strlen(buffer);
        strm->writeLastBuffer(buffer,size);
        clob.closeStream(strm);
        inFile.close();
        delete[] buffer;
      }
      cout << "Populating the Clob - Success" << endl;
      stmt->setAutoCommit(TRUE);
      stmt->executeUpdate();
      stmt->closeResultSet(rset);
      
    }
    catch(SQLException e){
      cout <<e.getMessage();
      delete[] buffer;
      return (-2);
    }
    return(0);
  }
}

