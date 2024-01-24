#include "teamperson.h"


TeamPerson::TeamPerson(): OrgPerson() {}

TeamPerson::TeamPerson(QString i, OrgPerson o): OrgPerson(o), per_team_id(i)
{

}

TeamPerson::TeamPerson(const TeamPerson & t)
{
    *this = t;
}

TeamPerson::~TeamPerson()
{

}

QString TeamPerson::teamPerGetId() const
{
    return per_team_id;
}

void TeamPerson::teamPerSetID(QString i)
{
    per_team_id = i;
}

bool TeamPerson::teamPerGetIsAdmin() const
{
    return team_is_admin;
}

void TeamPerson::teamPerSetIsAdmin(bool a)
{
    team_is_admin = a;
}
