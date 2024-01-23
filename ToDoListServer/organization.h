#ifndef ORGANIZATION_H
#define ORGANIZATION_H
#include <QString>
#include <orgperson.h>
#include <team.h>
#include <project.h>
#include <task.h>
#include <QVector>

class Organization
{
private:
    QString org_name;

public:
    Organization();
    Organization(QString);
    Organization(const Organization &);
    ~Organization();
    void orgSetName(QString);
    QString orgGetName() const;
};

#endif // ORGANIZATION_H
