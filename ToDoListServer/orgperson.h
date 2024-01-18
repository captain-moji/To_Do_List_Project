#ifndef ORGPERSON_H
#define ORGPERSON_H

#include "person.h"

class OrgPerson : public Person
{
private:
    QString per_org_id;
    bool org_is_admin;

public:
    OrgPerson();
    OrgPerson(QString, Person);
    OrgPerson(const Person &);
    ~OrgPerson();
    QString orgPerGetId() const;
    void orgPerSetID(QString);
    bool orgPerGetIsAdmin() const;
    void orgPerSetIsAdmin(bool);
    QString orgIdGenerator();
};

#endif // ORGPERSON_H
