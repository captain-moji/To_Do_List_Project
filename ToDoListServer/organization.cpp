#include "organization.h"

Organization::Organization() {}

Organization::Organization(QString n)
    : org_name(n)
{

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

QVector<OrgPerson> &Organization::orgGetPersons()
{
    return org_persons;
}

const QVector<OrgPerson> &Organization::orgGetPersons() const
{
    return org_persons;
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
