#include "organization.h"

Organization::Organization() {}

Organization::Organization(QString n, int i)
    : org_name(n), org_admin_id(i)
{
    // A code to add a person as an admin by searching in the PERSONS.json
}

Organization::Organization(const Organization & org)
{
    *this = org;
}

Organization::~Organization()
{

}

void Organization::orgSetName(QString n)
{
    org_name = n;
}

QString Organization::orgGetName() const
{
    return org_name;
}

void Organization::orgSetAdminId(int i)
{
    org_admin_id = i;
}

int Organization::orgGetAdminId()
{
    return org_admin_id;
}

QVector<Person> &Organization::orgGetPersons()
{
    return org_persons;
}

const QVector<Person> &Organization::orgGetPersons() const
{
    return org_persons;
}

QVector<ArchivedTask> &Organization::orgGetArchivedTasks()
{
    return org_archive_tasks;
}

const QVector<ArchivedTask> &Organization::orgGetArchivedTasks() const
{
    return org_archive_tasks;
}

QVector<UnarchivedTask> &Organization::orgGetUnarchivedTasks()
{
    return org_unarchived_task;
}

const QVector<UnarchivedTask> &Organization::orgGetUnarchivedTasks() const
{
    return org_unarchived_task;
}

QVector<Team> &Organization::orgGetTeams()
{
    return teams;
}

const QVector<Team> &Organization::orgGetTeams() const
{
    return teams;
}

QVector<Project> &Organization::orgGetprojects()
{
    return projects;
}

const QVector<Project> &Organization::orgGetprojects() const
{
    return projects;
}
