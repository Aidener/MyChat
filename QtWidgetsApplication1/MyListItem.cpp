#include "MyListItem.h"

MyListItem::MyListItem(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void MyListItem::setLabel(QString str)
{
	ui.label->setText(str);
}

void MyListItem::setText(QString str)
{
	ui.textBrowser->setText(str);
    QTextDocument* doc = ui.textBrowser->document();
    QTextEdit* textEdit = ui.textBrowser;
    //textEdit内的document和边框的距离
    QMargins margins = textEdit->contentsMargins();
    doc->adjustSize();
    textEdit->adjustSize();
    textEdit->setFixedWidth(doc->size().width() + margins.left() + margins.right());
    this->resize(QSize(this->size().width(), this->size().height() + doc->size().height() + margins.top() + margins.bottom() - 17));
}

MyListItem::~MyListItem()
{}
