#include "team.h"

Team::Team() {}

Team::Team(QString n, QString  i)
    : team_name(n)
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

void Team::teamSetId(QString i)
{
    team_id = i;
}

QString Team::teamGetid() const
{
    return team_id;
}

QVector<TeamPerson> &Team::teamGetPersons()
{
    return team_persons;
}

const QVector<TeamPerson> &Team::teamGetPersons() const
{
    return team_persons;
}


QString Team::teamIdGenerator() const
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString ThisTime = currentDateTime.toString("hhmmss");
    return ThisTime;
}
