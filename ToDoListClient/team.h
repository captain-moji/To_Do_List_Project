#ifndef TEAM_H
#define TEAM_H
#include <QString>
#include <teamperson.h>
#include <QVector>

class Team
{
private:
    QString team_name;
    QString team_id;
    QVector<TeamPerson> team_persons;

public:
    Team();
    Team(QString, QString );
    Team(const Team &);
    ~Team();
    void teamSetName(QString);
    QString teamGetName() const;
    void teamSetId(QString);
    QString teamGetid() const;
    QVector<TeamPerson>& teamGetPersons();
    const QVector<TeamPerson>& teamGetPersons() const;
    QString teamIdGenerator() const;
};

#endif // TEAM_H
