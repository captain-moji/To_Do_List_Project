#include "task.h"

Task::Task() {}

Task::Task(QString n, QString de, QString da, QString p)
    : task_title(n), description(de), task_date(da), priority(p)
{

}

Task::Task(const Task & task)
{
    *this = task;
}

Task::~Task()
{

}

void Task::taskSetTitle(QString n)
{
    task_title = n;
}

QString Task::taskGetTitle() const
{
    return task_title;
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

void Task::taskSetOwnerName(QString n)
{
    owner_name = n;
}

QString Task::taskGetOwnerName() const
{
    return owner_name;
}

void Task::taskSetOwnerID(QString i)
{
    owner_id = i;
}

QString Task::taskGetOwnerID() const
{
    return owner_id;
}

void Task::taskSetOwnerType(QString t)
{
    owner_type = t;
}

QString Task::taskGetOwnerType() const
{
    return owner_type;
}

void Task::taskSetIsArchived(bool t)
{
    is_archived = t;
}

bool Task::taskGetIsArchived() const
{
    return is_archived;
}



QVector<Comment> &Task::taskGetComments()
{
    return comments;
}

const QVector<Comment> &Task::taskGetComments() const
{
    return comments;
}
