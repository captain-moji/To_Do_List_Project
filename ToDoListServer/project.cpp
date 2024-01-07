#include "project.h"

Project::Project() {}

Project::Project(QString n, int i)
    : proj_name(n), proj_admi_id(i)
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

QVector<Person> Project::projGetPersons()
{
    return proj_persons;
}

const QVector<Person> Project::projGetPersons() const
{
    return proj_persons;
}

QVector<ArchivedTask> Project::projGetArchivedTasks()
{
    return proj_archived_tasks;
}

const QVector<ArchivedTask> Project::projGetArchivedTasks() const
{
    return proj_archived_tasks;
}

QVector<UnarchivedTask> Project::projGetUnarchivedTasks()
{
    return proj_unarchived_task;
}

const QVector<UnarchivedTask> Project::projGetUnarchivedTasks() const
{
    return proj_unarchived_task;
}
