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

void LobWizard::connect()
{
    cout << "Connect" << endl;
}
void LobWizard::accept()
{
        string fileStr;
        string charStr;
	string binStr;
	string loadStr;
	string userStr;
	string passStr;
	string dbStr;
	string sqlStr;
	
	fileStr = this->filenameEdit->text().toStdString();
	charStr = this->charsetEdit->text().toStdString();
	
	if ( this->BinaryCheckBox->isChecked() )
	{
	   binStr = "B" ;
	}   
	else
	{
	   binStr = "C" ;
	}
	if ( this->UploadRadioButton->isChecked() )
	{
	   loadStr = "U";
	}   
	if ( this->DownloadRadioButton->isChecked() )
	{
	   loadStr = "D";
	}   
	userStr =  this->userEdit->text().toStdString();
	passStr =  this->passEdit->text().toStdString();
	dbStr   =  this->connectionEdit->text().toStdString();
        sqlStr  =  this->SqlTextEdit->toPlainText().toStdString();
	cout << "Filename " <<  fileStr           << endl;
	cout << "Charset "  <<  charStr           << endl;
	cout << "Load "     <<  loadStr << binStr << endl;
	cout << "User "     <<  userStr  << endl;
	cout << "Pass "     <<  passStr  << endl;
	cout << "Db "       <<  dbStr << endl;
	cout << "SQL "      <<  sqlStr << endl;
	QDialog::accept();
}
