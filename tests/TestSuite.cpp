#include <cppunit/extensions/HelperMacros.h>
#include "../../oraload/lib/Connection.hpp"
#include "../../oraload/lib/BasicDML.hpp"
#include "../../oraload/lib/CharLob.hpp"
#include "../../oraload/lib/BinLob.hpp"
//#include "Misc/ParReader.hpp"
//#include "Misc/BaseError.hpp"

class TestSuite : public CppUnit::TestFixture
{ 
  CPPUNIT_TEST_SUITE( TestSuite );
  CPPUNIT_TEST( runConnTest );
  //CPPUNIT_TEST( runBasicDMLTest );
  //CPPUNIT_TEST( runCharLobTest_Upload );
  CPPUNIT_TEST( runBinLobTest_Upload );
  //CPPUNIT_TEST( runMiscTest );
  CPPUNIT_TEST_SUITE_END();
  
public: 
void setUp()
{
}
void tearDown()
{
}
// Class Conn
void runConnTest(void) 
{
  Conn c;
  c.setUsername("hr");
  c.setPassword("hr");
  c.setUrl("xe");
  cout << "Mark 10" << endl;
  CPPUNIT_ASSERT( c.connect() == 0 );
  c.setPassword("sthwrong");
  cout << "Mark 20" << endl;
  CPPUNIT_ASSERT( c.connect() < 0);
  cout << "MArk 25" << endl;

}
// Class BasicDML
void runBasicDMLTest(void) 
{
  string user = "hr";
  string passwd = "hr";
  string db = "xe";
  cout << "Mark 30" << endl;
  
  BasicDML S(user,passwd,db);
  S.connect();
  S.setSQLStmt("select to_number(1),'Eins',2.345,to_char(to_date('29122004','DDMMYYYY'),'DD-MON-YY') from dual");
  // this means: 4 fields to display, 0 binds
  cout << "Mark 40" << endl;
  string erg = S.displayRows(4,0);
  cout << "Mark 45" << endl;
  CPPUNIT_ASSERT( erg == "1|Eins|2.345|29-DEC-04|");
  cout << "Mark 50" << endl;
  S.setSQLStmt("select to_number(1),'Eins',2.345,to_char(to_date('29122004','DDMMYYYY'),'DD-MON-YY') from dual where 'eins' = :v2");
  // this means: position 0 is first bind par position, string is the datatype and eins is the value
  S.setBind(0,"string","eins");
  cout << "Mark 60" << endl;
  
  // this means: 4 fields to display, 1 binds
  erg = S.displayRows(4,1);
  CPPUNIT_ASSERT( erg == "1|Eins|2.345|29-DEC-04|");
  cout << "Mark 70" << endl;
  
  S.setSQLStmt("select to_number(1),'Eins',2.345,to_char(to_date('29122004','DDMMYYYY'),'DD-MON-YY') from dual where 'eins' = :v1 and 1=:v2") ;
  S.setBind(0,"string","eins");
  S.setBind(1,"int","1");
  cout << "Mark 80" << endl;
  // this means: 4 fields to display, 1 binds
  erg =S.displayRows(4,2);
  CPPUNIT_ASSERT( erg == "1|Eins|2.345|29-DEC-04|");
  
  // Creating a table
  S.setSQLStmt("drop table TestSuite");
  S.InsertRow();
  S.setSQLStmt("create table TestSuite (id number, text varchar2(30))");
  S.InsertRow();
  
  // inserting that table
  S.setSQLStmt("insert into TestSuite values(1,'Eins')");
  S.InsertRow();
  S.setSQLStmt("insert into TestSuite values(2,'Zwei')");
  S.InsertRow();

  // selecting that table
  S.setSQLStmt("select id,text from TestSuite") ;
  // this means: 2 fields to display, 0 binds
  erg = S.displayRows(2,0);
  CPPUNIT_ASSERT( erg == "1|Eins|2|Zwei|");
  
  // Rollbacking
  S.setSQLStmt("rollback");
  S.InsertRow();
  S.setSQLStmt("select id,text from TestSuite") ;
  // this means: 2 fields to display, 0 binds
  erg = S.displayRows(2,0);
  CPPUNIT_ASSERT( erg == "");
       
}

// Class CharLob
void runCharLobTest_Upload(void) 
{
  // Upload of CharData
  CharLob CL("hr","hr","xe");
  CL.connect();
  CL.setSQLStmt("drop table TestSuiteCL");
  CL.InsertRow();
  CL.setSQLStmt("create table TestSuiteCL (id number, flob clob)");
  CL.InsertRow();
  CL.setSQLStmt("insert into TestSuiteCL values(1,empty_clob())");
  CL.InsertRow();
  CL.Commit();

  CL.setFilename("TestSuiteCL.clob");
  string Loc("select flob from TestSuiteCL where id = 1 for update");
  CL.setSqlLocator(Loc);
  CL.UploadClobData();
  
  CL.setSQLStmt("select id,dbms_lob.getlength(flob) from TestSuiteCL") ;
  // this means: 2 fields to display, 0 binds
  string erg = CL.displayRows(2,0);
  // 729 plus 1 char (FIXME?)
  CPPUNIT_ASSERT( erg == "1|729|");    

  CL.setSQLStmt("insert into TestSuiteCL values(2,empty_clob())");
  CL.InsertRow();
  CL.Commit();
  CL.setFilename("TestSuiteCL.clob.2");
  Loc = "select flob from TestSuiteCL where id = 2 for update";
  CL.setSqlLocator(Loc);
  CL.UploadClobData();
  CL.setSQLStmt("select id,dbms_lob.getlength(flob) from TestSuiteCL where id = 2") ;
  erg = CL.displayRows(2,0);
  CPPUNIT_ASSERT( erg == "2|39455|");    

  CL.setFilename("TestSuiteCL.clob.2.OUT");
  Loc = "select flob from TestSuiteCL where id = 2";
  CL.setSqlLocator(Loc);
  CL.DownloadClobData();
}

// Class BinLob
void runBinLobTest_Upload(void) 
{
  // Upload of CharData
  BinLob BL("hr","hr","xe");
  BL.connect();
  BL.setSQLStmt("drop table TestSuiteBL");
  BL.InsertRow();
  BL.setSQLStmt("create table TestSuiteBL (id number, flob blob)");
  BL.InsertRow();

  BL.setSQLStmt("insert into TestSuiteBL values(1,empty_blob())");
  BL.InsertRow();
  BL.Commit();
  BL.setFilename("TestSuiteBL.blob.1.pdf");
  string Loc("select flob from TestSuiteBL where id = 1 for update");
  BL.setSqlLocator(Loc);
  BL.UploadBlobData();
  BL.setSQLStmt("select id,dbms_lob.getlength(flob) from TestSuiteBL") ;
  string erg = BL.displayRows(2,0); 
  CPPUNIT_ASSERT( erg == "1|9192|");    

  BL.setSQLStmt("insert into TestSuiteBL values(2,empty_blob())");
  BL.InsertRow();
  BL.Commit();
  BL.setFilename("TestSuiteBL.blob.2.pdf");
  Loc="select flob from TestSuiteBL where id = 2 for update";
  BL.setSqlLocator(Loc);
  BL.UploadBlobData();
  BL.setSQLStmt("select id,dbms_lob.getlength(flob) from TestSuiteBL where id = 2") ;
  //erg = BL.displayRows(2,0); 
  //CPPUNIT_ASSERT( erg == "2|159925|");    

  BL.setFilename("TestSuiteBL.blob.2.pdf.OUT");
  Loc="select flob from TestSuiteBL where id = 2";
  BL.setSqlLocator(Loc);
  BL.DownloadBlobData();
 
}

/*
void runMiscTest(void)
{
  cout << endl << "TESTING MISC" << endl;
  ParReader PR("Misc/test.par");
  BaseError e("ErrorMessage" , -4711 , "AdditionalMessage");
  e.ShowErrorMessage();
}
*/
};
// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestSuite );
