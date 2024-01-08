#include "team.h"

Team::Team() {}

Team::Team(QString n, int i)
    : team_name(n), team_admin_id(i)
{
    // A code to add a person as an admin by searching in the PERSONS.json
}

Team::Team(const Team & team)
{
    *this = team;
}

Team::~Team()
{

}

void Team::teamSetName(QString n)
{
    team_name = n;
}

QString Team::teamGetName() const
{
    return team_name;
}

void Team::teamSetAdminId(int i)
{
    team_admin_id = i;

}

int Team::teamGetAdminId()
{
    return team_admin_id;
}

QVector<Person> &Team::teamGetPersons()
{
    return team_persons;
}

const QVector<Person> &Team::teamGetPersons() const
{
    return team_persons;
}
