#ifndef PERSON_H
#define PERSON_H
#include <QString>
#include <QDateTime>


class Person
{
private:
    QString per_name;
    QString per_id;
    QString username;
    QString password;
    QString question;
    QString answer;

public:
    Person();
    Person(QString n,QString u , QString p ,QString q, QString a );
    QString perGetName() const;
    void perSetName(QString);
    QString perGetId ()const;
    void perSetID(QString);
    QString perGetUsername ()const;
    void perSetUsername(QString);
    QString perGetPassword ()const;
    void perSetPassword(QString);
    QString perGetQuestion () const;
    void perSetQuestion(QString);
    QString perGetAnswer() const;
    void perSetAnswer(QString);
    QString idGenerator();
};

#endif // PERSON_H
