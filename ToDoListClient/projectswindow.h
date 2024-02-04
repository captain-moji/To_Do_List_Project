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
#include <orgdialog.h>

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
    void loadProjectTasks();
    void removeTaskFromProject(QString task_name);
    void EditTaskArchive(QString task_title);
    void EditTaskInProject(QString old_task ,Task edited_task);
    void EditAnSpecialTaskInProject(Task edited_task);

public slots:
    void this_project_maker (QString);
    void this_org_maker(QString);
    void project_this_person_maker (OrgPerson);
    void project_admin_access_maker (bool admin_access, bool org_admin_access);

    void socket_readyRead();
    void socket_connected();
    void socket_bytesWritten();
    void socket_disconnected();

    void sendRequest(QString s);
    void responseChecker(QString);
    void connectionMaker(QString,int);

    void thisUserIsTeamMemberSetter(bool a);


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

    void show_project_persons(QString);

    void show_project_teams(QString s);

    void show_project_tasks(QString s);

    void show_teams_in_combobox(QString s);

    void closeEvent(QCloseEvent *event);

    void on_remove_task_BTN_clicked();

    void on_project_tasks_tree_widget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void maked_taskwindow(QString);

    void on_archive_unarchive_BTN_clicked();

    void save_comments(QString);

    void edited_project_task(QString,Task);

    void on_search_task_line_edit_textChanged(const QString &arg1);

    void on_archived_checkbox_stateChanged(int arg1);

    void on_not_archived_checkbox_stateChanged(int arg1);

    void search_task();

    void edit_project_task(QString s);

signals:
    void comment_saved();

private:
    bool is_admin;
    bool is_org_admin;

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
    QStringList temp_teams_list;
    QStringList temp_persons_list;
    Task another_temp_task;
    Task again_temp_task;
    QString this_org;
    Project this_project;
    QString this_project_admin;
    Task this_task;
    bool is_team_member;

signals:
    void this_task_maked(Task);
    void reconnect (QString,int);
};

#endif // PROJECTSWINDOW_H
