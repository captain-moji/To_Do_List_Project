#include "project.h"

Project::Project() {}

Project::Project(QString n, QString i)
    : proj_name(n), proj_admin_id(i)
{
}

Project::Project(const Project & project)
{
    *this = project;
}

Project::~Project()
{

}

void Project::projSetName(QString n)
{
    proj_name = n;
}

QString Project::projGetName()
{
    return proj_name;
}

void Project::projSetAdminId(QString i)
{
    proj_admin_id = i;
}

QString Project::projGetAdminId()
{
    return proj_admin_id;
}
