#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <occi.h>
using namespace oracle::occi;
using namespace std;

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
class Conn
{
  protected:
  string ConnectorType;
  string username;
  string password;
  string url;
  Connection *conn;   // The connection

  public:
  Conn ();
  Conn (string pConnectorType);
  ~Conn ();
  void setConnectorType (string c);
  string getConnectorType (void);

  void setUsername (string u);
  void setPassword (string p);
  void setUrl (string u);
  int connect(void);
};
#endif
