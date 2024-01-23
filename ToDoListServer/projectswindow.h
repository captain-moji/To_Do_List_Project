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
    void thisProjectShowAdmin();

    void loadOrgTeamsComboBox();
    void addTeamToProject(QString);
    void loadProjectTeams();
    void removeTeamfromProject(QString);

    void AddNewTaskToProject(Task);



private slots:

    void this_org_maker(QString);
    void this_project_maker (QString);
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

private:
    void search_project_teams();
    void search_project_persons();
    Ui::ProjectsWindow *ui;
    Team temp_team;
    OrgPerson temp_person;
    Task temp_task;
    QString this_org;
    Project this_project;
};

#endif // PROJECTSWINDOW_H
