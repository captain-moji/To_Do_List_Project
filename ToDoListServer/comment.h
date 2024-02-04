#ifndef COMMENT_H
#define COMMENT_H
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class Comment
{
private:
    QString comment_person;
    QString text;
    QString comment_date;
    QVector<Comment> replies;

public:
    Comment();
    Comment(QString, QString);
    ~Comment();
    void commentSetPerson(QString);
    QString commentGetPerson();
    void commentSetText(QString);
    QString commentGetText();
    const QString commentGetText() const;
    void commentSetDate(QString);
    QString commentGetDate();
    const QString commentGetDate() const;
    QVector<Comment> &commentGetReplies();
    void fromJson(const QJsonObject &);
    QJsonObject toJson() const;
};

#endif // COMMENT_H
