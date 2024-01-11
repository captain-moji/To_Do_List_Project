#ifndef ORGANIZATION_H
#define ORGANIZATION_H
#include <QString>
#include <orgperson.h>
#include <team.h>
#include <project.h>
#include <task.h>
#include <QVector>

class Organization
{
private:
    QString org_name;
    QVector<OrgPerson> org_persons;
    QVector<Team> teams;
    QVector<Project> projects;

public:
    Organization();
    Organization(QString);
    Organization(const Organization &);
    ~Organization();
    void orgSetName(QString);
    QString orgGetName() const;
    QVector<OrgPerson> &orgGetPersons();
    const QVector<OrgPerson> &orgGetPersons() const;
    QVector<Team> &orgGetTeams();
    const QVector<Team> &orgGetTeams() const;
    QVector<Project> &orgGetprojects();
    const QVector<Project> &orgGetprojects() const;
};

#endif // ORGANIZATION_H
