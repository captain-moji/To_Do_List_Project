#ifndef PROJECTSWINDOW_H
#define PROJECTSWINDOW_H

#include <QMainWindow>
#include <team.h>
#include <orgperson.h>
#include <task.h>
#include <project.h>

namespace Ui {
class ProjectsWindow;
}

class ProjectsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectsWindow(QWidget *parent = nullptr);
    ~ProjectsWindow();

private:
    Ui::ProjectsWindow *ui;
    Team temp_team;
    OrgPerson temp_person;
    Task temp_task;
    QString this_org;
    Project this_project;

};

#endif // PROJECTSWINDOW_H
