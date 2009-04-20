#include <QtGui>

#include "LobWizard.h"

LobWizard::LobWizard(QWidget *parent)
    : QWizard(parent)
{
    setupUi(this);
	//connect(toolButton, SIGNAL(clicked()), this, SLOT(open()));
}

void LobWizard::on_toolButton_clicked()
{
    this->open();
}

void LobWizard::open()
{
    this->lineEdit->setText( QFileDialog::getOpenFileName(this,tr("Choose File Location"),"/",tr("Alle *")) );
}
