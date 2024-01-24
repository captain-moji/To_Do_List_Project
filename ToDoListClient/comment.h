#ifndef COMMENT_H
#define COMMENT_H
#include <QString>
#include <QVector>

class Comment
{
private:
    QString text;
    QString comment_date;
    QVector<Comment *> replys;

public:
    Comment();
    Comment(QString, QString);
    Comment(const Comment &);
    ~Comment();
};

#endif // COMMENT_H
