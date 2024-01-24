#ifndef TASK_H
#define TASK_H
#include <QString>
#include <comment.h>
#include <QVector>

class Task
{
private:
    QString task_title;
    QString description;
    QString task_date;
    QString priority;
    bool is_archived;
    QString owner_name;
    QString owner_id;
    QString owner_type;

    QVector<Comment> comments;

public:
    Task();
    Task(QString, QString, QString, QString);
    Task(const Task&);
    ~Task();
    void taskSetTitle(QString);
    QString taskGetTitle() const;
    void taskSetDescription(QString);
    QString taskGetDescription() const;
    void taskSetDate(QString);
    QString taskGetDate() const;
    void taskSetPriority(QString);
    QString taskGetPriority() const;

    void taskSetOwnerName(QString);
    QString taskGetOwnerName() const;
    void taskSetOwnerID(QString);
    QString taskGetOwnerID() const;
    void taskSetOwnerType(QString);
    QString taskGetOwnerType() const;

    void taskSetIsArchived(bool);
    bool taskGetIsArchived() const;

    QVector<Comment> &taskGetComments();
    const QVector<Comment> &taskGetComments() const;

};

#endif // TASK_H
