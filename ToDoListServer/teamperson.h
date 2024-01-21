#ifndef TEAMPERSON_H
#define TEAMPERSON_H

#include "orgperson.h"

class TeamPerson : public OrgPerson
{
private:
    QString per_team_id;
    bool team_is_admin;

public:
    TeamPerson();
    TeamPerson(QString, OrgPerson);
    TeamPerson(const TeamPerson &);
    ~TeamPerson();
    QString teamPerGetId ()const;
    void teamPerSetID(QString);
    bool teamPerGetIsAdmin() const;
    void teamPerSetIsAdmin(bool);
};

#endif // TEAMPERSON_H
