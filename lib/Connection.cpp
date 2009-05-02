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
      //Environment *env = Environment::createEnvironment (Environment::DEFAULT);
      env = Environment::createEnvironment (Environment::DEFAULT);
      conn = env->createConnection (username, password, url);
    }
    catch(SQLException e){
      this->WriteLogFile(e.getMessage());	
      cout << e.getMessage() << endl;  
      //FIXME Produces *** glibc detected *** free(): invalid pointer: 0x0806be30 ***
      // Fixes with MALLOC_CHECK_=0;export MALLOC_CHECK on the cmdline
      return(-1);
    }
    return(0);
  }
  return(0);
}
  
 

