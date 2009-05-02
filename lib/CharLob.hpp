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

///  ************************************************
///  CharLob
///  -----------
///  Upload and Download of Char Lobs 
///  to an Oracle Server
///
///  Author: Ralf Duenkelmann
///  Date :  01.06.2004
///
/// ************************************************

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <cstdio>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
#include "Lob.hpp"
#ifndef CHAR_LOB_HPP
#define CHAR_LOB_HPP

/*!  CharLob
  
  -----------
  
  Upload and Download of Character Lobs 
  to an Oracle Server

  Author: Ralf Duenkelmann
  
  Date :  01.06.2004
*/
class CharLob:public Lob
{
  private:
	string char_set; 
	
  public:
  /*!  Def Constructor
  */
  CharLob ();
  /*!  Constructor
   * 
   * @param user     name of the database user
   * @param pass     password of the database user
   * @param db       The database alias from tnsnames
   * @param cfile    filename to up- or download (full path) 
  */
  CharLob (char* user , char* pass , char* db, char* cfile);
  /*!  Constructor
   * 
   * @param user     name of the database user
   * @param pass     password of the database user
   * @param db       The database alias from tnsnames
  */
  CharLob (char* user , char* pass , char* db);
  /*!  Def Constructor
  */
  /*!  Constructor
   * 
   * @param connection     name of a formerly built connection
   * @param connection     the corresponding environment
  */
  CharLob (oracle::occi::Connection* p_connection, oracle::occi::Environment* p_env);
  ~CharLob ();
  /*!  DownloadBlob
   * Download it
   * @return 0 ok , -1 file open problem , -3 Oracle Error  
   *  
  */  
  int DownloadClobData(void);
  /*!  UploadClob
   * Upload it
   * @return 0 ok , -1 file open problem , -3 Oracle Error  
   *  
  */
  int UploadClobData(void);
  void setCharSet(string pCharSet);
  string getCharSet();

};//end of class
#endif
