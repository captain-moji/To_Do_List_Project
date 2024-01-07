#ifndef PROJECT_H
#define PROJECT_H
#include <QString>
#include <person.h>
#include <archivedtask.h>
#include <unarchivedtask.h>
#include <QVector>

class Project
{
private:
    QString proj_name;
    int proj_admi_id;
    QVector<Person> proj_persons;
    QVector<ArchivedTask> proj_archived_tasks;
    QVector<UnarchivedTask> proj_unarchived_task;

public:
    Project();
    Project(QString, int);
    Project(const Project &);
    ~Project();
    void projSetName(QString);
    QString projGetName();
    QVector<Person> projGetPersons();
    const QVector<Person> projGetPersons() const;
    QVector<ArchivedTask> projGetArchivedTasks();
    const QVector<ArchivedTask> projGetArchivedTasks() const;
    QVector<UnarchivedTask> projGetUnarchivedTasks();
    const QVector<UnarchivedTask> projGetUnarchivedTasks() const;
};

#endif // PROJECT_H
