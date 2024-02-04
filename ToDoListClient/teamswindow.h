#ifndef TEAMSWINDOW_H
#define TEAMSWINDOW_H

#include <QMainWindow>
#include "organizationpersonswindow.h"
#include <team.h>
#include <QTreeWidgetItem>
#include <QTcpSocket>
#include <orgdialog.h>

namespace Ui {
class TeamsWindow;
}

class TeamsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeamsWindow(QWidget *parent = nullptr);
    ~TeamsWindow();

    void socket_readyRead();
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();

    void sendRequest(QString s);
    void responseChecker(QString);
    void connectionMaker(QString,int);

    void loadTeamPersons();
    void addNewTeamPerson(QString);
    void removeTeamPerson(QString);
    void changeTeamPersonRole(QString);
    void changeOrgPersonRole(QString);

    void this_team_maker(QString);
    void this_org_maker(QString);
    void admin_access(bool is_admin, bool is_org_admin );
    void this_team_id_maker(QString);
    void this_user_maker(QString username);

private slots:
    void show_team_persons(QString s);

    void add_team_person(QString);
    void on_teams_add_person_BTN_clicked();
    void on_teams_remove_person_BTN_clicked();
    void on_teams_change_role_BTN_clicked();

    void on_search_line_edit_textChanged(const QString &arg1);
    void on_members_checkbox_stateChanged(int arg1);
    void on_Admins_checkbox_stateChanged(int arg1);

    void closeEvent(QCloseEvent *event);

private:

    Team this_team;
    QString this_org;
    bool is_admin;
    bool is_org_admin;

    QString this_user;

    TeamPerson temp_team_person;

    QTcpSocket * socket;
    QString this_ip;
    int this_port;

    void search_user();
    Ui::TeamsWindow *ui;

signals:

    void reconnect (QString,int);

};

#endif // TEAMSWINDOW_H
