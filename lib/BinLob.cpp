#include "BinLob.hpp"

BinLob::BinLob ()
{
}

BinLob::BinLob (char* user , char* pass , char* db, char* cfile)
{
  /**
   * default values of username & password
   */
   username = user;
   password = pass;
   url = db;
   filename = cfile;
}

BinLob::BinLob (char* user , char* pass , char* db)
{
  /**
   * default values of username & password
   */
   username = user;
   password = pass;
   url = db;
}

BinLob::~BinLob ()
{
   cout << "Destroy";
}

int BinLob::DownloadBlobData(void){
  if (this->getConnectorType() == "Oracle"){
    unsigned int bufsize=10000;
    unsigned char* buffer = new unsigned char[bufsize + 1];
    memset(buffer,0,bufsize);
    ofstream ofFile;
      ofFile.open((const char*)filename.c_str(),ios_base::binary);
    
    if (!ofFile)
      {
        cout << "Could not open file for writing." << endl;
        delete[] buffer;
        return(-1);
      }
    try{
        Statement *stmt = conn->createStatement(sqlLocator);
        ResultSet *rset = stmt->executeQuery();
        cout << "Got ResultSet." << endl;
        while(rset->next())
        {
          Blob blob = rset->getBlob(1);
          cout << blob.length() << endl;
          Stream *strm=blob.getStream(1,0);
          int bytesRead = strm->readBuffer((char*)buffer,200);
          while (bytesRead>0)
          {
            for (int i=0;i<bytesRead;++i)
            {
              ofFile << buffer[i];
            }
            bytesRead = strm->readBuffer((char*)buffer,200);
          }
          blob.closeStream(strm);
          ofFile.close();
          cout << "Getting the Blob - Success" << endl;
        }
        free(buffer);

    }
    catch (SQLException e){
      cout << e.getMessage();
      free(buffer);
      return(-3);
    }
    return(0);
  }
}

int BinLob::UploadBlobData(void){
  if (this->getConnectorType() == "Oracle"){
    // Uses stream here
    cout << "Populating the Blob" << endl;

    ifstream countFile;
    countFile.open((const char*)filename.c_str(),ios_base::binary|ios_base::in);
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

    //unsigned int bufsize=sizeof(char);
    //char* buffer = new char[bufsize];
    unsigned int bufsize=i-2;
    char* buffer = new char[bufsize];
    ifstream inFile;
    inFile.open((const char*)filename.c_str(),ios_base::binary|ios_base::in);

    if (!inFile)
    {
      cout << "file not found\n";
      delete[] buffer;
      return(-1);
    }
    unsigned int size;
    try{
      std::string sqlStat = sqlLocator;
      Statement *stmt = conn->createStatement(sqlStat);
      ResultSet *rset = stmt->executeQuery();
      cout << "Got ResultSet." << endl;
      while(rset->next())
      {
        Blob blob = rset->getBlob(1);
        Stream *strm=blob.getStream();
        memset(buffer,0,bufsize);
        inFile.read(buffer,bufsize);
        strm->writeBuffer(buffer,bufsize);
        strcpy(buffer,"");
        size=strlen(buffer);
        strm->writeLastBuffer(buffer,size);
        blob.closeStream(strm);
        inFile.close();
        delete[] buffer;
        cout << "Populating the Blob - Success" << endl;
      }
      stmt->setAutoCommit(TRUE);
      stmt->executeUpdate();
      stmt->closeResultSet(rset);
    }
    catch(SQLException e){
      cout <<e.getMessage();
      return (-2);
    }
    return(0);
  }
}


