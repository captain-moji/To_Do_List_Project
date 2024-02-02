#ifndef ORGANIZATIONSWINDOW_H
#define ORGANIZATIONSWINDOW_H

#include <QMainWindow>
#include <person.h>
#include <QString>
#include <organization.h>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <allserverusers.h>
#include <orgperson.h>
#include <QTreeWidgetItem>
#include <QMap>
#include <QTimer>
#include <orgdialog.h>
#include <QListWidget>
#include <QSet>
#include "teamswindow.h"
#include <project.h>
#include <projectswindow.h>
#include <QTcpSocket>

namespace Ui {
class OrganizationsWindow;
}

class OrganizationsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OrganizationsWindow(QWidget *parent = nullptr);
    ~OrganizationsWindow();

    void loadAllOrgPersons();
    void removeUserFromOrganization(QString);
    void addUserToOrganization(QString);
    void promoteToAdmin(QString user);

    void addTeamToOrganization(QString);
    void editTeaminOrganization(QString,QString);
    void removeTeamFromOrganization(QString);
    void loadAllOrgTeams();

    void addProjectToOrganization(QString);
    void loadAllOrgProjects();
    void editProjectinOrganization(QString,QString);
    void removeProjectFromOrganization(QString);

private slots:

    void add_new_person_to_organization(QString);

    void on_org_page_add_person_BTN_clicked();

    void on_org_page_edit_person_BTN_clicked();

    void on_org_page_remove_person_BTN_clicked();

    void on_org_users_search_line_edit_textChanged(const QString &arg1);

    void on_is_org_admin_checkbox_stateChanged(int arg1);

    void on_is_org_member_checkbox_stateChanged(int arg1);

    void add_new_team_to_organization(QString);

    void edit_team_in_organization(QString);

    void on_add_team_BTN_clicked();
  
    void on_remove_team_BTN_clicked();
  
    void on_edit_team_name_BTN_clicked();
  
    void on_teams_list_widget_itemDoubleClicked(QListWidgetItem *item);

    QString getTeamIdByName(QString name);

    void on_search_teams_line_edit_textChanged(const QString &arg1);

    void show_org_projects(QString s);

    void on_add_project_BTN_clicked();

    void add_new_project_to_organization(QString);

    void on_edit_project_BTN_clicked();

    void edit_project_in_organization(QString);

    void on_remove_project_BTN_clicked();

    void on_search_projects_line_edit_textChanged(const QString &arg1);

    void on_sort_projects_BTN__clicked();

    void on_projects_list_widget_itemDoubleClicked(QListWidgetItem *item);


public slots:
    void socket_readyRead();
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();

    void sendRequest(QString s);
    void responseChecker(QString);
    void connectionMaker(QString,int);  //(QTcpSocket *);
    void this_user_maker(Person,QString);
    void this_user_check_admin(bool res);
    void this_org_maker(QString);
    void load_org_persons(QString s);
    void show_org_teams(QString s);

private:
    QTcpSocket * socket;
    QString this_ip;
    int this_port;
    Organization this_org;
    Team team;
    OrgPerson this_user;
    Project temp_project;
    void search_org_user();
    void search_org_team();
    void search_org_project();

    Ui::OrganizationsWindow *ui;
    void closeEvent(QCloseEvent *event);

signals:
    void team_id_signal(QString);
    void team_name_signal(QString);
    void org_name_signal(QString);
    void project_name_signal(QString);
};

#endif // ORGANIZATIONSWINDOW_H
