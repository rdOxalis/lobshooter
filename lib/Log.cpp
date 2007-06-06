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

#include "Log.hpp"


string Log::GetTime()
{
  time_t t ;

  t = time(NULL);  // aktuelle Zeit in t

  // mit tm struct weiter arbeiten
  tm *   tpointer = localtime (&t); // zeiger auf tm struct

  char DS[20];
  strcpy(DS , "");
  size_t strsize = sizeof(DS);
  strsize = strftime(DS, strsize, "%d.%m.%Y %H:%M:%S", tpointer);
  return(string(DS));
}


void Log::FlushLogFile(void)
{
  std::ofstream out(vLogFile.c_str(), std::ios_base::trunc);
  out.close(); 
}

int Log::WriteLogFile(string pLog)
{
  std::ofstream out(vLogFile.c_str(), std::ios_base::app);
  if ( !out ) {
    cout << "Unable to open Log File";
    return (EXIT_FAILURE);
  }

  out << GetTime() << " :: " << pLog.c_str() << "\n";
  out.close();
  // Written? Sure?
  if (! out) {
    return (EXIT_FAILURE);
  }
  return(0);
}

  
void Log::setLogFile(string f)
{
   this->vLogFile = f;
}
 

