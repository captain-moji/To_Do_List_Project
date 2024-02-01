#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMainWindow>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QList>
#include <QJsonArray>
#include <orgperson.h>

namespace Ui {
class Connection;
}

class Connection : public QMainWindow
{
    Q_OBJECT
public:

    void serverTurnOn();
    void serverTurnOff();
    void serverResponse(QString);
    void serverReqActions(QString);
    explicit Connection(QWidget *parent = nullptr);
    ~Connection();


public slots:
    void server_newConnection();
    void socket_readyRead();
    void socket_bytesWritten();
    void socket_disConnected();

private slots:

    void login_user(QString,QString);
    void signup_user(QString, QString , QString, QString, QString);
    void send_user_question (QString username);
    void load_user_organizations(QString);
    void change_user_password(QString, QString);
    void add_new_org(QString);
    void add_user_to_org(bool is_admin, QString org);
    void remove_org(QString username,QString org);

    void on_pushButton_clicked();

private:

    OrgPerson temp_org_person;



    int port = 1234;
    QString resp;
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
    Ui::Connection *ui;
};

#endif // CONNECTION_H
