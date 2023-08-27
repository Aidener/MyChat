#pragma once

#include <QWidget>
#include "ui_MyListItem.h"

class MyListItem : public QWidget
{
	Q_OBJECT

public:
	MyListItem(QWidget *parent = nullptr);
	void setLabel(QString str);
	void setText(QString str);
	~MyListItem();

private:
	Ui::MyListItemClass ui;
};
