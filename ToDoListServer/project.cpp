#include "project.h"

Project::Project() {}

Project::Project(QString n, QString i)
    : proj_name(n), proj_admin_id(i)
{
    // A code to add a person as an admin by searching in the PERSONS.json
}

Project::Project(const Project & project)
{
    *this = project;
}

Project::~Project()
{

}

void Project::projSetName(QString n)
{
    proj_name = n;
}

QString Project::projGetName()
{
    return proj_name;
}

void Project::projSetAdminId(QString i)
{
    proj_admin_id = i;
}

QString Project::projGetAdminId()
{
    return proj_admin_id;
}

QVector<Person> Project::projGetPersons()
{
    return proj_persons;
}

const QVector<Person> Project::projGetPersons() const
{
    return proj_persons;
}

QVector<Task> Project::projGetArchivedTasks()
{
    return proj_archived_tasks;
}

const QVector<Task> Project::projGetArchivedTasks() const
{
    return proj_archived_tasks;
}

QVector<Task> Project::projGetUnarchivedTasks()
{
    return proj_unarchived_task;
}

const QVector<Task> Project::projGetUnarchivedTasks() const
{
    return proj_unarchived_task;
}
