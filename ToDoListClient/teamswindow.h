#ifndef TEAMSWINDOW_H
#define TEAMSWINDOW_H

#include <QMainWindow>
#include "organizationpersonswindow.h"
#include <team.h>
#include <QTreeWidgetItem>

namespace Ui {
class TeamsWindow;
}

class TeamsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TeamsWindow(QWidget *parent = nullptr);
    ~TeamsWindow();
    void loadTeamPersons();
    void addNewTeamPerson(QString);
    void removeTeamPerson(QString);
    void changeTeamPersonRole(QString);
    void changeOrgPersonRole(QString);
private slots:

    void this_team_maker(QString);
    void this_org_maker(QString);
    void this_team_id_maker(QString);
    void add_team_person(QString);
    void on_teams_add_person_BTN_clicked();
    void on_teams_remove_person_BTN_clicked();
    void on_teams_change_role_BTN_clicked();

    void on_search_line_edit_textChanged(const QString &arg1);
    void on_members_checkbox_stateChanged(int arg1);
    void on_Admins_checkbox_stateChanged(int arg1);

private:

    Team this_team;
    QString this_org;
    TeamPerson temp_team_person;

    void search_user();
    Ui::TeamsWindow *ui;
};

#endif // TEAMSWINDOW_H
