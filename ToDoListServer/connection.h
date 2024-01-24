#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>
#include <QMessageBox>

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
    void server_newConnection();

    void socket_readyRead();
    void socket_bytesWritten();
    void socket_disConnected();

// private slots:
    void connectSlot();

    void sendSlot();

    void disconnectSlot();


private:
    QString ip;
    int port;
    QString message;
    QTcpSocket *socket = nullptr;
    QTcpServer *server = nullptr;};

#endif // CONNECTION_H
