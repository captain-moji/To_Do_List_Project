#include "connection.h"

Connection::Connection() {}

Connection::~Connection() {
    if (socket != nullptr)
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

void Connection::connectSlot() {
    socket = new QTcpSocket();
    socket->connectToHost(ip, port);

    connect(socket, &QTcpSocket::connected, this, &Connection::socket_connected);
    connect(socket, &QTcpSocket::readyRead, this, &Connection::socket_readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &Connection::socket_bytesWritten);
    connect(socket, &QTcpSocket::disconnected, this, &Connection::socket_disconnected);
}


void Connection::socket_connected() {

}

void Connection::socket_readyRead() {
    message = socket->readAll();
}

void Connection::socket_bytesWritten() {

}

void Connection::socket_disconnected() {

}

void Connection::sendSlot() {
    if (socket)
    {
        socket->write(message.toUtf8());
    }
}

void Connection::disconnectSlot() {
    if (socket)
    {
        delete socket;
        socket = nullptr;
    }
}
