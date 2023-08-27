#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <qtcpsocket.h>
#include <qtcpserver.h>
#include"ChatMember.h"
#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class MianWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MianWindowClass *ui;
    QActionGroup group;
    QTcpSocket *socket;
    QTcpServer* server;
    QMap<QString,ChatMember*> map;
    QString me;
    void clearmap();
private slots:
    void send();
    void sendAll(QString host,QString message = NULL);
    void tcpconnect();
    void tcpdisconnect();
    void serverSelected();
    void customeSelected();
    void recieve();
    void recieve(const QString&);
    void error(QAbstractSocket::SocketError e);
    void statusinfoc();
    void statusinfodc();
    void statusinfodc(const QString&);
    void statusinfo(QAbstractSocket::SocketState socketState);
    void newconnect();
};
