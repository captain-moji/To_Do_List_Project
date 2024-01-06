#include "person.h"

Person::Person() {}

QString Person::perGetName()
{
    return this->per_name;
}

void Person::perSetName(QString n)
{
    this->per_name = n;
}

int Person::perGetId()
{
    return this->per_id;
}

void Person::perSetID(int i)
{
    this->per_id = i;
}

QString Person::perGetUsername()
{
    return this->username;
}

void Person::perSetUsername(QString u)
{
    this->username = u;
}

QString Person::perGetPassword()
{
    return this->password;
}

void Person::perSetPassword(QString p)
{
    this->password = p;
}





