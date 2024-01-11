#ifndef TASK_H
#define TASK_H
#include <QString>
#include <comment.h>
#include <QVector>

class Task
{
private:
    QString task_name;
    QString description;
    QString task_date;
    QString priority;
    QVector<Comment> comments;

public:
    Task();
    Task(QString, QString, QString, QString);
    Task(const Task&);
    ~Task();
    void taskSetName(QString);
    QString taskGetName() const;
    void taskSetDescription(QString);
    QString taskGetDescription() const;
    void taskSetDate(QString);
    QString taskGetDate() const;
    void taskSetPriority(QString);
    QString taskGetPriority() const;
    QVector<Comment> &taskGetComments();
    const QVector<Comment> &taskGetComments() const;

};

#endif // TASK_H
