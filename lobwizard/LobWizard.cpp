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
    this->Log::setLogFile("LobWizard.log");
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
    this->Conn::setUsername(this->userEdit->text().toStdString());
    this->Conn::setPassword(this->passEdit->text().toStdString());
    this->Conn::setUrl(this->connectionEdit->text().toStdString());
    if ( this->Conn::connect() == 0 ) 
    {
	cout << "Connected" << endl;
    }
    else
    {  
	//c->WriteLogFile("Connection failed, invalid login?");
	cout << "Connection failed, invalid login?" << endl;
    }
}
void LobWizard::accept()
{
        string fileStr;
        string charStr;
	string binStr;
	string loadStr;
	string sqlStr;
	
	fileStr = this->filenameEdit->text().toStdString();
	charStr = this->charsetEdit->text().toStdString();
	
	if ( this->BinaryCheckBox->isChecked() )
	{
	   binStr = "B" ;
	   BinLob *BL;
	   BL = new BinLob(this->conn);
	   BL->setLogFile("BLob.log");
	   BL->FlushLogFile();
	   BL->WriteLogFile("BinLob");
	   BL->setFilename(fileStr);
	   BL->setSqlLocator(sqlStr);
	   BL->DownloadBlobData();
	}   
	else
	{
	   binStr = "C" ;
	   CharLob *CL;
	   CL = new CharLob(this->conn);
	   CL->setLogFile("CLob.log");
	   CL->FlushLogFile();
	   CL->WriteLogFile("CharLob");
	   CL->setFilename(fileStr);
	   CL->setCharSet(charStr);
	   CL->setSqlLocator(sqlStr);
	   CL->DownloadClobData();
	}
	if ( this->UploadRadioButton->isChecked() )
	{
	   loadStr = "U";
	}   
	if ( this->DownloadRadioButton->isChecked() )
	{
	   loadStr = "D";
	}   
        sqlStr  =  this->SqlTextEdit->toPlainText().toStdString();
	cout << "Filename " <<  fileStr           << endl;
	cout << "Charset "  <<  charStr           << endl;
	cout << "Load "     <<  loadStr << binStr << endl;
	cout << "SQL "      <<  sqlStr << endl;
        QDialog::accept();
	
}
void LobWizard::on_connectButton_clicked()
{
	this->lw_connect();
}
