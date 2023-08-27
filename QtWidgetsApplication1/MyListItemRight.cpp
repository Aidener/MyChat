#include "MyListItemRight.h"
#include<qdebug.h>

MyListItemRight::MyListItemRight(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}
void MyListItemRight::setLabel(QString str)
{
	ui.label->setText(str);
}

void MyListItemRight::setText(QString str)
{
	ui.textBrowser->setText(str);
    QTextDocument* doc = ui.textBrowser->document();
    QTextEdit* textEdit = ui.textBrowser;
    //textEdit内的document和边框的距离
    QMargins margins = textEdit->contentsMargins();
    doc->adjustSize();
    textEdit->adjustSize();
    qDebug() << doc->size().height() << doc->size().width();
    
    textEdit->setFixedWidth(doc->size().width() + margins.left() + margins.right());
    qDebug() << textEdit->height() << textEdit->width();
    this->resize(QSize(this->size().width(), this->size().height()+ doc->size().height() + margins.top() + margins.bottom()-17));
}

MyListItemRight::~MyListItemRight()
{}
