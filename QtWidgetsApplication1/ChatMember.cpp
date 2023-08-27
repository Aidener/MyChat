#include "ChatMember.h"
#include<qrandom.h>
#include"MainWindow.h"
#pragma execution_character_set("utf-8")

QList<Nameinfo> ChatMember::namepool = QList<Nameinfo>();

ChatMember::ChatMember(QObject *parent, QTcpSocket* s, QString name,int num)
	: QObject(parent),socket(s),name(name),num(num)
{}

void ChatMember::statusinfodc()
{
	emit disconnected(socket->localAddress().toString()+QString::number(socket->peerPort()));
}

const QString&  ChatMember::search(int p)
{
	return namepool[p].name;
}

void ChatMember::IntiNamePool()
{
	Nameinfo name[40] = {
		{false,"韩信" }, {false, "曹操"}, {false, "小乔"}, {false, "孙策"}, {false, "诸葛亮"}, {false, "孙尚香"}, {false, "马超"}, {false, "黄忠"},
		{false,"西施"},{false,"王昭君"},{false,"貂蝉"} ,{false,"关羽"},{false,"吕布"},{false,"杨玉环"},{false,"妲己"} ,{false,"赵云"},
		{false,"武则天"},{false,"张飞"},{false,"典韦"} ,{false,"李白"},{false,"虞姬"},{false,"项羽"},{false,"岳飞"} ,{false,"李清照"},
		{false,"戚继光"},{false,"辛弃疾"},{false,"程咬金"} ,{false,"薛仁贵"},{false,"唐婉"},{false,"褒姒"},{false,"杨宗保"} ,{false,"花木兰"},
		{false,"上官婉儿"},{false,"曹操"},{false,"周亚夫"} ,{false,"霍去病"},{false,"白起"},{false,"蔡文姬"},{false,"西王母"} ,{false,"盘古"},
	};
	for (int i = 0; i < 40; i++) {
		namepool.append(name[i]);
	}
}

ChatMember* ChatMember::quest(QObject* parent, QTcpSocket* s) {
	int r = QRandomGenerator::global()->bounded(39);
	bool k = false;
	for (int i = 0; i < 40; i++) {
		if (namepool[r].use)
			r = (r + 1) % 40;
		else
		{
			k = true;
			break;
		}
	}
	if (k) {
		ChatMember* n = new ChatMember(parent, s, namepool[r].name,r);
		namepool[r].use = true;
		return n;
	}
	else
	{
		return nullptr;
	}
}

void ChatMember::recieve() {
	emit readyRead(socket->localAddress().toString()+ QString::number(socket->peerPort()));
}

ChatMember::~ChatMember()
{
	namepool[num].use = false;
}
