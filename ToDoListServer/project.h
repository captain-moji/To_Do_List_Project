#ifndef PROJECT_H
#define PROJECT_H
#include <QString>
#include <person.h>
#include <task.h>
#include <QVector>

class Project
{
private:
    QString proj_name;
    QString proj_admin_id;
    QVector<Person> proj_persons;
    QVector<Task> proj_archived_tasks;
    QVector<Task> proj_unarchived_task;

public:
    Project();
    Project(QString, QString);
    Project(const Project &);
    ~Project();
    void projSetName(QString);
    QString projGetName();
    void projSetAdminId(QString);
    QString projGetAdminId();
    QVector<Person> projGetPersons();
    const QVector<Person> projGetPersons() const;
    QVector<Task> projGetArchivedTasks();
    const QVector<Task> projGetArchivedTasks() const;
    QVector<Task> projGetUnarchivedTasks();
    const QVector<Task> projGetUnarchivedTasks() const;
};

#endif // PROJECT_H
