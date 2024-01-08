#ifndef UNARCHIVEDTASK_H
#define UNARCHIVEDTASK_H
#include <QString>
#include <comment.h>
#include <QVector>

class UnarchivedTask
{
private:
    QString task_name;
    QString description;
    QString task_date;
    QString priority;
    QVector<Comment> comments;

public:
    UnarchivedTask();
    UnarchivedTask(QString, QString, QString, QString);
    UnarchivedTask(const UnarchivedTask&);
    ~UnarchivedTask();
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

#endif // UNARCHIVEDTASK_H
