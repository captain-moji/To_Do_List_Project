#ifndef ArchivedTASK_H
#define ArchivedTASK_H
#include <QString>
#include <comment.h>
#include <QVector>

class ArchivedTask
{
private:
    QString task_name;
    QString description;
    QString task_date;
    QString priority;
    QVector<Comment> comments;

public:
    ArchivedTask();
    ArchivedTask(QString, QString, QString, QString);
    ArchivedTask(const ArchivedTask&);
    ~ArchivedTask();
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

#endif // ArchivedTASK_H
