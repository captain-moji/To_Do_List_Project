#include "unarchivedtask.h"

UnarchivedTask::UnarchivedTask() {}

UnarchivedTask::UnarchivedTask(QString n, QString de, QString da, QString p)
    : task_name(n), description(de), task_date(da), priority(p)
{

}

UnarchivedTask::UnarchivedTask(const UnarchivedTask & task)
{
    *this = task;
}

UnarchivedTask::~UnarchivedTask()
{

}

void UnarchivedTask::taskSetName(QString n)
{
    task_name = n;
}

QString UnarchivedTask::taskGetName() const
{
    return task_name;
}

void UnarchivedTask::taskSetDescription(QString de)
{
    description = de;
}

QString UnarchivedTask::taskGetDescription() const
{
    return description;
}

void UnarchivedTask::taskSetDate(QString da)
{
    task_date = da;
}

QString UnarchivedTask::taskGetDate() const
{
    return task_date;
}

void UnarchivedTask::taskSetPriority(QString p)
{
    priority = p;
}

QString UnarchivedTask::taskGetPriority() const
{
    return priority;
}

QVector<Comment> &UnarchivedTask::taskGetComments()
{
    return comments;
}

const QVector<Comment> &UnarchivedTask::taskGetComments() const
{
    return comments;
}
