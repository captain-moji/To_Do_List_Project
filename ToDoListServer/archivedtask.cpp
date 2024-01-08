#include "archivedtask.h"

ArchivedTask::ArchivedTask() {}

ArchivedTask::ArchivedTask(QString n, QString de, QString da, QString p)
    : task_name(n), description(de), task_date(da), priority(p)
{

}

ArchivedTask::ArchivedTask(const ArchivedTask & task)
{
    *this = task;
}

ArchivedTask::~ArchivedTask()
{

}

void ArchivedTask::taskSetName(QString n)
{
    task_name = n;
}

QString ArchivedTask::taskGetName() const
{
    return task_name;
}

void ArchivedTask::taskSetDescription(QString de)
{
    description = de;
}

QString ArchivedTask::taskGetDescription() const
{
    return description;
}

void ArchivedTask::taskSetDate(QString da)
{
    task_date = da;
}

QString ArchivedTask::taskGetDate() const
{
    return task_date;
}

void ArchivedTask::taskSetPriority(QString p)
{
    priority = p;
}

QString ArchivedTask::taskGetPriority() const
{
    return priority;
}

QVector<Comment> &ArchivedTask::taskGetComments()
{
    return comments;
}

const QVector<Comment> &ArchivedTask::taskGetComments() const
{
    return comments;
}
