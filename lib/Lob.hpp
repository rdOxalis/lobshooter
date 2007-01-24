#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
#include "BasicDML.hpp"

#ifndef LOB_HPP
#define LOB_HPP
class Lob:public BasicDML
{
  protected:
  string filename;
  string sqlLocator;  // The Locator string

  public:
  void setFilename (string f);
    // for inserting a LOB sqlLocator MUST be an select string
    // Dont forget the locking via "for update" - clause!!
  void setSqlLocator (string sl);
  int InitLob(string pField , string pTable , string pWhere);
};
#endif
