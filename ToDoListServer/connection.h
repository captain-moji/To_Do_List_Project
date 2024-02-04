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
#include <signup.h>
#include <person.h>
#include <projectswindow.h>

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
    void SetPort(int p);
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
    void add_project_to_org(QString proj_name,QString org_name);
    bool check_project_exsist_in_org(QString proj_name, QString org);
    void edit_project_in_org(QString proj_old_name,QString proj_new_name,QString org_name);
    void remove_project_from_org(QString proj_name, QString org);
    void check_org_team_access(QString username,QString org_name,QString team_name);
    void load_team_persons(QString team, QString org);
    void check_org_project_access(QString username,QString org_name,QString proj_name);
    void add_person_to_team(QString username,QString org_name,QString team_name);
    void change_person_role_in_team(QString username,QString org_name,QString team_name);
    void remove_person_from_team(QString username,QString org_name,QString team_name);
    void load_project_persons(QString project, QString org);
    void load_project_teams(QString project, QString org);
    void load_project_tasks(QString project, QString org);
    void add_person_to_project (QString user,QString project,QString org);
    void remove_person_from_project(QString username,QString org_name,QString proj_name);
    void change_person_role_in_project(QString username,QString org_name,QString proj_name);
    void load_org_teams_in_project(QString org_name);
    void add_team_to_project(QString team_name, QString project_name,QString org_name);
    void remove_team_from_project(QString team_name, QString project_name,QString org_name);
    void add_task_to_project(Task temp_task , QString proj , QString org);
    void remove_task_from_project(QString taskname, QString project_name,QString org_name);
    void edit_task_archive_in_project(QString taskname, QString project_name,QString org_name);
    void edit_task_in_project(QString taskname, QString project_name,QString org_name);
    void save_edited_task_in_project(Task temp_task, QString old_task_title, QString projname, QString org );
    void load_task_comments(QString org_name, QString proj_name, QString task_title);
    void save_task_comments(QString org_name, QString proj_name, QString task_title, QString comments);

    void on_pushButton_clicked();


private:

    OrgPerson temp_org_person;



    int this_port;
    QString resp;
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
    Ui::Connection *ui;
};

#endif // CONNECTION_H
