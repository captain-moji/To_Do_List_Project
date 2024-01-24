#include "person.h"

Person::Person() {}

Person::Person(QString n, QString u, QString p, QString q, QString a) :
per_name(n), username(u), password (p), question (q), answer (a), per_id (idGenerator())
{ }

QString Person::perGetName () const
{
    return this->per_name;
}

void Person::perSetName(QString n)
{
    this->per_name = n;
}

QString Person::perGetId() const
{
    return this->per_id;
}

void Person::perSetID(QString i)
{
    this->per_id = i;
}

QString Person::perGetUsername() const
{
    return this->username;
}

void Person::perSetUsername(QString u)
{
    this->username = u;
}

QString Person::perGetPassword() const
{
    return this->password;
}

void Person::perSetPassword(QString p)
{
    this->password = p;
}

void Person::perSetQuestion(QString q)
{
    this->question = q;
}


QString Person::perGetQuestion() const
{
    return this->question;
}


QString Person::perGetAnswer() const
{
    return this->answer;
}

void Person::perSetAnswer(QString a)
{
    this->answer = a;
}


QString Person:: idGenerator()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString ThisTime = currentDateTime.toString("MMddhhmmss");
    return ThisTime;
}

