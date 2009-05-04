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
#ifndef LOBWIZARD_H
#define LOBWIZARD_H
#include <QApplication>
#include <QWizard>

#include "ui_LobWizard.h"
#include "CharLob.hpp"
#include "BinLob.hpp"
#include "Log.hpp"
#include "BasicDML.hpp"
#include "Connection.hpp"

class LobWizard : public QWizard, public Ui::LobWizard, public BasicDML 
{
    Q_OBJECT
	
public:
	LobWizard(QWidget *parent = 0);
	string logfile;
    //Versuch, validatePage zu überladen, soll angeblich bei Click auf Next-Button ausgelöst werden, 
	//dies ist aber nicht so
	//bool validatePage();

protected slots:
    //auch diese Funktion aus QWizard soll angeblich bei Drücken des Next-Buttons ziehen,
	//ist aber auch nicht so, daher keine Chance, das Verhalten des Next-Buttons zu ändern?
	// anders als der Virtual Slot "accept()", scheinen diese Dokumentationen falsch zu sein.
    //void next();
	
private slots:
	void open();
	void on_openButton_clicked();
	void on_connectButton_clicked();
	void lw_connect();
	//overloaded virtual slot
	void accept();	
	QStringList users();
	//QStringList tables(string user);
	//QStringList columns(string user , string table); 

	
};

#endif

