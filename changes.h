#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "ui_changes.h"

class Dialog : public QDialog
{
	Q_OBJECT

private:

	Ui::Dialog* ui2;

public:

	//changes(QWidget* parent);
	Dialog(QWidget* parent = Q_NULLPTR) : QDialog(parent), ui2(new Ui::Dialog)
	{
		ui2->setupUi(this);
	}

	~Dialog() 
	{
		delete ui2;
	};

	int getTyp() { return ui2->spinBox->value(); };
	QString getLogin() { return ui2->log->text(); };
	QString getPswd() { return ui2->hes->text(); };
	QString getName() { return ui2->men->text(); };
	QString getSurname() { return ui2->pri->text(); };
	QString getAdress() { return ui2->adr->text(); };
	QString getMinute() { return ui2->min->text(); };
	QString getPosition() { return ui2->poz->text(); };
};