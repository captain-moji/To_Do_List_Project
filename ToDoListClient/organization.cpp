#include "organization.h"

Organization::Organization() {}

Organization::Organization(QString n)
    : org_name(n)
{

}

Organization::Organization(const Organization & org)
{
    *this = org;
}

Organization::~Organization()
{

}

void Organization::orgSetName(QString n)
{
    org_name = n;
}

QString Organization::orgGetName() const
{
    return org_name;
}
