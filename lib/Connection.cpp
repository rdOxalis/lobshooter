#include "Connection.hpp"

Conn::Conn ()
{
  this->setConnectorType("Oracle");
}
 
Conn::Conn (string pConnectorType)
{
  this->setConnectorType(pConnectorType);
}
Conn::~Conn ()
{
}
void Conn::setConnectorType (string c)
{
  ConnectorType = c;
}
string Conn::getConnectorType (void)
{
  return(ConnectorType);
}

void Conn::setUsername(string u){
   username = u;
}

void Conn::setPassword (string p)
{
  password = p;
}

void Conn::setUrl (string u)
{
  url = u;
}

int Conn::connect(void)
{
  if (this->getConnectorType() == "Oracle"){
    try{
      Environment *env = Environment::createEnvironment (Environment::DEFAULT);
      conn = env->createConnection (username, password, url);
    }
    catch(SQLException e){
      cout << e.getMessage() << endl;  
      //FIXME Produces *** glibc detected *** free(): invalid pointer: 0x0806be30 ***
      // Fixes with MALLOC_CHECK_=0;export MALLOC_CHECK on the cmdline
      return(-1);
    }
    return(0);
  }
}
  
 

