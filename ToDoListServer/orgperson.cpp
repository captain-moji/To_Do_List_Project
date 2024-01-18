#include "orgperson.h"

OrgPerson::OrgPerson(): Person() {}

OrgPerson::OrgPerson(QString i, Person p): Person(p), per_org_id(i)
{

}

OrgPerson::OrgPerson(const Person & o)
{
    *this = o;
}

OrgPerson::~OrgPerson()
{

}

QString OrgPerson::orgPerGetId() const
{
    return per_org_id;
}

void OrgPerson::orgPerSetID(QString i)
{
    per_org_id = i;
}

bool OrgPerson::orgPerGetIsAdmin() const
{
    return org_is_admin;
}

void OrgPerson::orgPerSetIsAdmin(bool is)
{
    org_is_admin = is;
}

QString OrgPerson:: orgIdGenerator()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString ThisTime = currentDateTime.toString("ddhhmmss");
    return ThisTime;
}
