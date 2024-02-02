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
    void edit_org(QString user,QString old_org_name,QString new_org_name);
    void check_org_admin(QString username,QString org);
    void org_persons_list_maker(QString org);
    void add_new_user_to_org(QString,QString);
    bool check_user_exsist_in_server(QString username, QString org);
    void remove_user_from_org(QString,QString);
    void change_user_role_in_org(QString user, QString org);
    void load_org_teams(QString );
    void add_team_to_org(QString ,QString);
    bool check_team_exsist_in_org(QString team_name, QString org);
    void remove_team_from_org(QString,QString);
    void edit_team_in_org(QString new_name, QString old_name,QString org);
    void load_org_projects(QString org_name);


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
