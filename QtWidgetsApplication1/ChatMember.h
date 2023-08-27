#pragma once

#include <QObject>
#include<qtcpsocket.h>
#include <qhostaddress.h>

//Q_DECLARE_METATYPE(QString)

struct Nameinfo {
	bool use;
	QString name;
};

class ChatMember  : public QObject
{
	Q_OBJECT

public:
	static ChatMember* quest(QObject* parent,QTcpSocket *s);
	static const QString& search(int);
	static void IntiNamePool();
	QTcpSocket* socket;
	const QString name;
	~ChatMember();
private:
	static QList<Nameinfo> namepool;
	int num;
	ChatMember(QObject* parent, QTcpSocket* s, QString name, int num);
signals:
	void readyRead(const QString&);
	void disconnected(const QString&);
private slots:
	void recieve();
	void statusinfodc();
};
