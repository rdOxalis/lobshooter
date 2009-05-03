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
#include <QtGui>
#include "LobWizard.h"
#include <string>

LobWizard::LobWizard(QWidget *parent)
    : QWizard(parent)
{
    setupUi(this);
    this->logfile = "LobWizard.log";
    this->Log::setLogFile(logfile);
    
    // Test only!
    this->userEdit->setText("ftxpress");
    this->passEdit->setText("ftxpress");
    this->connectionEdit->setText("//pEL5:1522/panorpa");
    this->filenameEdit->setText("test.txt");
    this->SqlTextEdit->setText("select text from test_lob where id = 1");
    this->charsetEdit->setText("WE8ISO8859P15");
    //connect(toolButton, SIGNAL(clicked()), this, SLOT(open()));
}

void LobWizard::on_openButton_clicked()
{
    this->open();
}

void LobWizard::open()
{
    this->filenameEdit->setText( QFileDialog::getOpenFileName(this,tr("Choose File Location"),"/",tr("Alle *")) );
}

void LobWizard::lw_connect()
{
    this->BasicDML::setUsername(this->userEdit->text().toStdString());
    this->BasicDML::setPassword(this->passEdit->text().toStdString());
    this->BasicDML::setUrl(this->connectionEdit->text().toStdString());
    if ( this->BasicDML::connect() == 0 ) 
    {
	    this->Log::WriteLogFile("Connected");
	    cout << "Connected" << endl;
	    this->setSQLStmt("select name from v$database");
	    string databaseSql = "Connected to ";
	    databaseSql.append(this->getString(0));
       this->connectedLabel->setText(databaseSql.c_str());
	    this->next();
    }
    else
    {  
	    this->Log::WriteLogFile("Connection failed, invalid login?");
	    cout << "Connection failed" << endl;
    }
}
void LobWizard::accept()
{
   string fileStr;
   string charStr;
	string binStr;
	string loadStr;
	string sqlStr;
	BinLob *BL;
	CharLob *CL;
	
	fileStr = this->filenameEdit->text().toStdString();
	charStr = this->charsetEdit->text().toStdString();
        sqlStr  =  this->SqlTextEdit->toPlainText().toStdString();
	
	if ( this->BinaryCheckBox->isChecked() )
	{
	   binStr = "B" ;
	   BL = new BinLob(this->conn, this->env);
	   BL->setLogFile(this->logfile);
	   BL->WriteLogFile("BinLob");
	   BL->setFilename(fileStr);
	   BL->setSqlLocator(sqlStr);
	}   
	else
	{
	   binStr = "C" ;
	   CL = new CharLob(this->conn, this->env);
	   CL->setLogFile(this->logfile);
	   CL->WriteLogFile("CharLob");
	   CL->setFilename(fileStr);
	   CL->setCharSet(charStr);
	   CL->setSqlLocator(sqlStr);
	}
	if ( this->UploadRadioButton->isChecked() )
	{
	   loadStr = "U";
	}   
	if ( this->DownloadRadioButton->isChecked() )
	{
	   loadStr = "D";
	}   
   loadStr.append(binStr);
	if (loadStr == "UC")
	  CL->UploadClobData();
	if (loadStr == "DC")
	  CL->DownloadClobData();
	if (loadStr == "UB")
	  BL->UploadBlobData();
	if (loadStr == "DB")
	  BL->DownloadBlobData();
        
	//cout << "Filename " <<  fileStr           << endl;
	//cout << "Charset "  <<  charStr           << endl;
	//cout << "Load "     <<  loadStr           << endl;
	//cout << "SQL "      <<  sqlStr            << endl;
        QDialog::accept();
	
}
void LobWizard::next()
{
  cout << "Page " << this->currentId();
  QWizard::next();
}
void LobWizard::on_connectButton_clicked()
{
	this->lw_connect();
}
