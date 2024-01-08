#ifndef TEAM_H
#define TEAM_H
#include <QString>
#include <person.h>
#include <QVector>

class Team
{
private:
    QString team_name;
    int team_admin_id;
    QVector<Person> team_persons;

public:
    Team();
    Team(QString, int);
    Team(const Team &);
    ~Team();
    void teamSetName(QString);
    QString teamGetName() const;
    void teamSetAdminId(int);
    int teamGetAdminId();
    QVector<Person>& teamGetPersons();
    const QVector<Person>& teamGetPersons() const;
};

#endif // TEAM_H
