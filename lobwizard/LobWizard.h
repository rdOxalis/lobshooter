#ifndef LOBWIZARD_H
#define LOBWIZARD_H
#include <QApplication>
#include <QWizard>

#include "ui_LobWizard.h"

class LobWizard : public QWizard, public Ui::LobWizard
{
    Q_OBJECT
	
public:
	LobWizard(QWidget *parent = 0);

private slots:
	void open();
	void on_toolButton_clicked();										
};


#endif

