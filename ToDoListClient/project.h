#ifndef PROJECT_H
#define PROJECT_H
#include <QString>
#include <person.h>
#include <task.h>
#include <QVector>

class Project
{
private:
    QString proj_name;
    QString proj_admin_id;


public:
    Project();
    Project(QString, QString);
    Project(const Project &);
    ~Project();
    void projSetName(QString);
    QString projGetName();
    void projSetAdminId(QString);
    QString projGetAdminId();

};

#endif // PROJECT_H
