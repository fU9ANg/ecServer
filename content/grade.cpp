
/*
 * grade.cpp
 */

#include "grade.h"

cGrade::cGrade()
{
}

cGrade::~cGrade ()
{
}

string cGrade::getName (void)
{
    return this->m_name;
}

void cGrade::setName (string& sName)
{
    this->m_name = sName;
}
