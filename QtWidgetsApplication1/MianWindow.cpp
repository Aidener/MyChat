#include "MainWindow.h"
#include "MyListItem.h"
#include"MyListItemRight.h"
#include"qmessagebox.h"
#include<qdebug.h>
#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),map()
    , ui(new Ui::MianWindowClass()),group(this)
{
    ui->setupUi(this);
    ChatMember::IntiNamePool();
    group.addAction(ui->serverAction);
    group.addAction(ui->customerActionk);
    socket = nullptr;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newconnect()));
    ui->customerActionk->setChecked(true);
}

void MainWindow::send() {
    QString str = ui->textEdit->toPlainText();
    if (!str.isEmpty()) {
        MyListItemRight* i = new MyListItemRight(ui->textEdit);
        i->setLabel(me);
        i->setText(str);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(QSize(0, i->size().height()));
        item->setFlags(Qt::ItemFlags(126));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, i);
    }
    if (map.size() > 1) {
        if (ui->serverAction->isChecked()) {
            str = "[" + me + "]" + str;
            sendAll("127.0.0.1", str);
        }
    }
    else if (socket != nullptr && socket->isOpen()) {
            socket->write(str.toUtf8());
    }
    else {
        QMessageBox::critical(this, "发送失败", "网络存在问题，消息已保存但未发送");
    }
}



MainWindow::~MainWindow()
{
    if (ui->customerActionk->isChecked())
        tcpdisconnect();
    delete ui;
}

void MainWindow::error(QAbstractSocket::SocketError e)
{
    {
        QMessageBox::critical(this, "连接断开", "网络问题或是对方已终止对话");
        return;
    }
}

void MainWindow::statusinfoc()
{
    QMessageBox::about(this, "连接信息", "已连接");

}

void MainWindow::statusinfodc()
{
    QMessageBox::about(this, "连接信息", "已断开连接");
}

void MainWindow::statusinfodc(const QString& ip)
{
    ChatMember* m = map.value(ip);
    map.remove(ip);
    if (m->socket != nullptr) {
        if (m->socket->isValid())
            m->socket->disconnectFromHost();
        m->socket->deleteLater();
    }
    delete m;
}

void MainWindow::statusinfo(QAbstractSocket::SocketState socketState)
{
    QMessageBox::about(this, "连接信息", "状态改变"+ socketState);
}

void MainWindow::newconnect()
{
    QTcpSocket* socket = server->nextPendingConnection();
    if (map.contains(socket->localAddress().toString() + QString::number(socket->peerPort()))){
        socket->write(map[socket->localAddress().toString() + QString::number(socket->peerPort())]->name.toUtf8());
        return;
    }
    ChatMember* m = ChatMember::quest(this, socket);
    if (m != nullptr) {
        map.insert(socket->localAddress().toString()+ QString::number(socket->peerPort()), m);
        socket->write(m->name.toUtf8());
        connect(socket, SIGNAL(readyRead()), m, SLOT(recieve()));
        connect(m, SIGNAL(readyRead(const QString&)), this, SLOT(recieve(const QString&)));
        //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(error(QAbstractSocket::SocketError)));
        connect(socket, SIGNAL(disconnected()), m, SLOT(statusinfodc()));
        connect(m, SIGNAL(disconnected(const QString&)), this, SLOT(statusinfodc(const QString&)));
        QMessageBox::information(this, "连接信息", m->name + "加入群聊");
    }
}

void MainWindow::clearmap()
{
    if (map.isEmpty())
        return;
    auto i= map.constBegin();
    while (i!= map.constEnd()) {
        statusinfodc(i.key());
    }
}

void MainWindow::sendAll(QString host,QString message)
{
    if (message.isNull()) {
        return;
    }
    else {
        auto i = map.constBegin();
        while (i!=map.constEnd())
        {
            if (i.key() == "127.0.0.1" || i.key() == host) {       //不转发给自己
                ++i;
                continue;
            }
            i.value()->socket->write(message.toUtf8());
            ++i;
        }
    }
}

void MainWindow::tcpconnect()
{
    if (ui->customerActionk->isChecked()) {
        QString ipstr = ui->serverEdit->text();
        QHostAddress ip = QHostAddress(ipstr);
        ipstr=ip.toString();
        if (ip.isNull()) {
            QMessageBox::critical(this, "ip地址错误", "ip地址为空地址");
            return;
        }
        QString port = ui->portEdit->text();
        if (port.toInt() > 65535 || port.toInt() < 1025) {
            QMessageBox::critical(this, "端口错误", "端口大于65535或者为保留端口");
            return;
        }
        if (socket != nullptr) {
            if (socket->isOpen())
                tcpdisconnect();
        }
        socket = new QTcpSocket(this);
        socket->connectToHost(QHostAddress(ip), port.toInt());
        socket->waitForReadyRead();
        if (socket->isReadable()) {
            me = QString::fromUtf8(socket->readAll());
            connect(socket, SIGNAL(readyRead()), this, SLOT(recieve()));
            connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(error(QAbstractSocket::SocketError)));
            connect(socket, SIGNAL(connected()), this, SLOT(statusinfoc()));
            statusinfoc();
            connect(socket, SIGNAL(disconnected()), this, SLOT(statusinfodc()));
            connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(statusinfo(QAbstractSocket::SocketState)));
        }
        else
        {
            QMessageBox::about(this, "连接失败", "没有可用身份");
            tcpdisconnect();
        }
    }
    else
    {
        QString port = ui->portEdit->text();
        if (port.toInt() > 65535 || port.toInt() < 1025) {
            QMessageBox::critical(this, "端口错误", "端口大于65535或者为保留端口");
            return;
        }
        if (socket != nullptr)
            tcpdisconnect();
        server->listen(QHostAddress::Any,port.toInt());
        server->setMaxPendingConnections(40);                    //最大连接数，由昵称池多少决定
        clearmap();
        socket = server->nextPendingConnection();
        ChatMember* m = ChatMember::quest(this, socket);        //给自己取名字
        me = m->name;
        map.insert("127.0.0.1", m);
        QMessageBox::information(this, "服务器消息", "正在监听");
    }
}

void MainWindow::tcpdisconnect()
{
    if (socket != NULL) {
        socket->abort();
        delete socket;
        socket = nullptr;
    }
}

void MainWindow::serverSelected()
{
    ui->serverEdit->setEnabled(false);
}

void MainWindow::customeSelected()
{
    ui->serverEdit->setEnabled(true);
}

void MainWindow::recieve()
{
    QString str=QString::fromUtf8(socket->readAll());
    QRegExp exp("\\[[A-Za-z0-9\u4e00-\u9fa5]*\\]", Qt::CaseSensitive, QRegExp::RegExp);
    bool t = exp.isValid();
    exp.indexIn(str);
    QString name = exp.cap(0);
    str.remove(name);
    if (!str.isEmpty()) {
        MyListItem* i = new MyListItem(ui->textEdit);
        name.remove("[");
        name.remove("]");
        i->setLabel(name);
        i->setText(str);
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(QSize(0, i->size().height()));
        item->setFlags(Qt::ItemFlags(126));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, i);
    }
}

void MainWindow::recieve(const QString& ip)
{
    ChatMember* m = map.value(ip);
    QString str = QString::fromUtf8(m->socket->readAll());
    
    if (!str.isEmpty()) {
        MyListItem* i = new MyListItem(ui->textEdit);
        i->setLabel(m->name);
        i->setText(str);
        QListWidgetItem* item = new QListWidgetItem();
        qDebug() << i->size().height() << endl;
        item->setSizeHint(QSize(0, i->size().height()));
        item->setFlags(Qt::ItemFlags(126));
        ui->listWidget->addItem(item);
        ui->listWidget->setItemWidget(item, i);
        sendAll(ip,("[" + m->name + "]" + str));
    }
}
