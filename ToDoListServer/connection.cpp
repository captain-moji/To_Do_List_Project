#include "connection.h"

Connection::Connection() {}

Connection::~Connection() {
    if ( socket != nullptr )
        delete socket;
}

void Connection::setIp(QString i)
{
    ip = i;
}

QString Connection::getIp() const
{
    return ip;
}

void Connection::setPort(int p)
{
    port = p;
}

int Connection::getPort() const
{
    return port;
}

void Connection::setMessage(QString m)
{
    message = m;
}

QString Connection::getMessage() const
{
    return message;
}

void Connection::server_newConnection()
{
    socket = new QTcpSocket();
    socket = server->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &Connection::socket_readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &Connection::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &Connection::socket_disConnected);
}

void Connection::connectSlot() {
    if (!server)
    {
        server = new QTcpServer();
        server->listen(QHostAddress::Any, port);


        if (server->isListening()) {
            connect(server, &QTcpServer::newConnection, this, &Connection::server_newConnection);
        }

    }
}

void Connection::socket_readyRead() {
    message = socket->readAll();
}

void Connection::socket_bytesWritten() {

}

void Connection::socket_disConnected() {

}

void Connection::sendSlot() {
    if (socket)
    {
        socket->write(message.toUtf8());
    }
}

void Connection::disconnectSlot() {  
    if (server)
    {
        if ( socket != nullptr )
            delete socket;
        delete server;
        socket = nullptr;
        server = nullptr;
    }
}
