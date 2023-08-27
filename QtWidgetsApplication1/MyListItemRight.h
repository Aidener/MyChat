#pragma once

#include <QWidget>
#include "ui_MyListItemRight.h"

class MyListItemRight : public QWidget
{
	Q_OBJECT

public:
	MyListItemRight(QWidget *parent = nullptr);
	void setLabel(QString str);
	void setText(QString str);
	~MyListItemRight();
private:
	Ui::MyListItemRightClass ui;
};
