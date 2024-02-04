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
#include "personsandteamswindow.h"
#include <orgperson.h>

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
    void editTeamInProject(QString,QString);

    void AddNewTaskToProject(Task);
    void loadProjectTasks();
    void removeTaskFromProject(QString task_name);
    void EditTaskArchive(QString task_title);
    void EditTaskInProject(QString old_task , Task edited_task);


public slots:
    void this_org_maker(QString);
    void this_project_maker (QString);
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

    void on_remove_task_BTN_clicked();

    void on_project_tasks_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_archive_unarchive_BTN_clicked();

    void edit_project_task(Task);

    void on_search_task_line_edit_textChanged(const QString &arg1);

    void on_archived_checkbox_stateChanged(int arg1);

    void on_not_archived_checkbox_stateChanged(int arg1);

signals:
    void this_task_maked(Task);

private:
    void search_task();
    void search_project_teams();
    void search_project_persons();
    Ui::ProjectsWindow *ui;
    Team temp_team;
    OrgPerson temp_person;
    Task temp_task;
    QString this_org;
    Project this_project;
    Task this_task;
};

#endif // PROJECTSWINDOW_H
