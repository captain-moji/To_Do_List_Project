#ifndef ORGANIZATION_H
#define ORGANIZATION_H
#include <QString>
#include <person.h>
#include <team.h>
#include <project.h>
#include <archivedtask.h>
#include <unarchivedtask.h>
#include <QVector>

class Organization
{
private:
    QString org_name;
    int org_admin_id;
    QVector<Person> org_persons;
    QVector<ArchivedTask> org_archive_tasks;
    QVector<UnarchivedTask> org_unarchived_task;
    QVector<Team> teams;
    QVector<Project> projects;

public:
    Organization();
    Organization(QString, int);
    Organization(const Organization &);
    ~Organization();
    void orgSetName(QString);
    QString orgGetName() const;
    void orgSetAdminId(int);
    int orgGetAdminId();
    QVector<Person> &orgGetPersons();
    const QVector<Person> &orgGetPersons() const;
    QVector<ArchivedTask> &orgGetArchivedTasks();
    const QVector<ArchivedTask> &orgGetArchivedTasks() const;
    QVector<UnarchivedTask> &orgGetUnarchivedTasks();
    const QVector<UnarchivedTask> &orgGetUnarchivedTasks() const;
    QVector<Team> &orgGetTeams();
    const QVector<Team> &orgGetTeams() const;
    QVector<Project> &orgGetprojects();
    const QVector<Project> &orgGetprojects() const;
};

#endif // ORGANIZATION_H
