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
		{false,"����" }, {false, "�ܲ�"}, {false, "С��"}, {false, "���"}, {false, "�����"}, {false, "������"}, {false, "��"}, {false, "����"},
		{false,"��ʩ"},{false,"���Ѿ�"},{false,"����"} ,{false,"����"},{false,"����"},{false,"����"},{false,"槼�"} ,{false,"����"},
		{false,"������"},{false,"�ŷ�"},{false,"��Τ"} ,{false,"���"},{false,"�ݼ�"},{false,"����"},{false,"����"} ,{false,"������"},
		{false,"�ݼ̹�"},{false,"������"},{false,"��ҧ��"} ,{false,"Ѧ�ʹ�"},{false,"����"},{false,"���"},{false,"���ڱ�"} ,{false,"��ľ��"},
		{false,"�Ϲ����"},{false,"�ܲ�"},{false,"���Ƿ�"} ,{false,"��ȥ��"},{false,"����"},{false,"���ļ�"},{false,"����ĸ"} ,{false,"�̹�"},
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
