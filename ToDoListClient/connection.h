#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QObject>

class Connection : public QObject
{
    Q_OBJECT

public:
    Connection();
    ~Connection();
    void setIp(QString);
    QString getIp() const;
    void setPort(int);
    int getPort() const;
    void setMessage(QString);
    QString getMessage() const;

public slots:
    void socket_connected();
    void socket_readyRead();
    void socket_bytesWritten();
    void socket_disconnected();

private slots:
    void connectSlot();

    void sendSlot();

    void disconnectSlot();


private:
    QString ip;
    int port;
    QString message;
    QTcpSocket *socket = nullptr;
};

#endif // CONNECTION_H
