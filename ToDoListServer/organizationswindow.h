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

private slots:

    void this_org_maker(QString);

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

    void on_sort_teams_BTN_clicked();

private:
    Organization this_org;
    Team team;
    OrgPerson temp_org_person;
    void makeAllpersonsFile();
    void search_org_user();
    void search_org_team();

    Ui::OrganizationsWindow *ui;

signals:
    void team_id_signal(QString);
    void team_name_signal(QString);
    void org_name_signal(QString);
};

#endif // ORGANIZATIONSWINDOW_H
