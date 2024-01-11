#include "task.h"

Task::Task() {}

Task::Task(QString n, QString de, QString da, QString p)
    : task_name(n), description(de), task_date(da), priority(p)
{

}

Task::Task(const Task & task)
{
    *this = task;
}

Task::~Task()
{

}

void Task::taskSetName(QString n)
{
    task_name = n;
}

QString Task::taskGetName() const
{
    return task_name;
}

void Task::taskSetDescription(QString de)
{
    description = de;
}

QString Task::taskGetDescription() const
{
    return description;
}

void Task::taskSetDate(QString da)
{
    task_date = da;
}

QString Task::taskGetDate() const
{
    return task_date;
}

void Task::taskSetPriority(QString p)
{
    priority = p;
}

QString Task::taskGetPriority() const
{
    return priority;
}

QVector<Comment> &Task::taskGetComments()
{
    return comments;
}

const QVector<Comment> &Task::taskGetComments() const
{
    return comments;
}
