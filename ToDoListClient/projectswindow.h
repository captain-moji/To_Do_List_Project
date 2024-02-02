#ifndef PROJECTSWINDOW_H
#define PROJECTSWINDOW_H

#include <QMainWindow>
#include <team.h>
#include <orgperson.h>
#include <task.h>
#include <project.h>
#include "organizationpersonswindow.h"
#include <QTreeWidgetItem>
#include <taskwindow.h>
#include <QTcpSocket>


namespace Ui {
class ProjectsWindow;
}

class ProjectsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectsWindow(QWidget *parent = nullptr);
    ~ProjectsWindow();
    void addNewProjectPerson (QString);
    void loadProjectPersons();
    void removeProjectPerson(QString);
    void changeProjectAdmin(QString);
    void thisProjectShowAdmin(QString);

    void loadOrgTeamsComboBox();
    void addTeamToProject(QString);
    void loadProjectTeams();
    void removeTeamfromProject(QString);

    void AddNewTaskToProject(Task);



public slots:
    void this_project_maker (QString);
    void this_org_maker(QString);
    void project_this_person_maker (OrgPerson);
    void project_admin_access_maker (bool);

    void socket_readyRead();
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();

    void sendRequest(QString s);
    void responseChecker(QString);
    void connectionMaker(QString,int);

private slots:

    void add_project_person (QString);

    void on_projects_add_person_BTN_clicked();

    void on_projects_remove_person_BTN_clicked();

    void on_projects_set_admin_BTN_clicked();

    void on_project_teams_add_BTN_clicked();

    void on_project_teams_remove_BTN_clicked();

    void on_project_teams_sort_BTN_clicked();

    void on_project_teams_search_line_edit_textChanged(const QString &arg1);

    void on_search_line_edit_textChanged(const QString &arg1);

    void on_add_new_task_BTN_clicked();

    void project_task_maker(Task);

    void closeEvent(QCloseEvent *event);

private:
    bool admin_access;

    OrgPerson this_person;
    QTcpSocket * socket;
    QString this_ip;
    int this_port;

    void search_project_teams();
    void search_project_persons();
    Ui::ProjectsWindow *ui;
    Team temp_team;
    OrgPerson temp_person;
    Task temp_task;
    QString this_org;
    Project this_project;
    QString this_project_admin;

signals:
    void reconnect (QString,int);
};

#endif // PROJECTSWINDOW_H
