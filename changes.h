#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets>

#include "ui_changes.h"

class changes : public QDialog
{
	Q_OBJECT

private:
	Ui::Dialog* ui2;
public:
	changes(QWidget* parent);
	~changes() {
		delete ui2;
	};
};