#ifndef PERSON_H
#define PERSON_H
#include <QString>

class Person
{
private:
    QString per_name;
    int per_id;
    QString username;
    QString password;

public:
    Person();
    QString perGetName();
    void perSetName(QString);
    int perGetId();
    void perSetID(int);
    QString perGetUsername();
    void perSetUsername(QString);
    QString perGetPassword();
    void perSetPassword(QString);

};

#endif // PERSON_H
