
/*
 * course.cpp
 */

#include "course.h"

CCourse :: CCourse ()
{
}

CCourse :: ~CCourse ()
{
}

void CCourse::setName (string sName)
{
    this->m_name = sName;
}

string CCourse::getName (void)
{
    return m_name;
}

CCourseItem* CCourse::get_item_by_id (int id)
{
    COURSEITEMLIST::iterator it;
    for (it = m_courseList.begin ();
         it != m_courseList.end (); it++)
    {
        if ((*it)->getId () == id)
            return (*it);
    }
    return (NULL);
}

CCourseItem* CCourse::get_item_by_name (string name)
{
    COURSEITEMLIST::iterator it;
    for (it = m_courseList.begin ();
         it != m_courseList.end (); it++)
    {
        if ((*it)->getName() == name)
            return (*it);
    }
    return NULL;
}

void CCourse::add_item (CCourseItem* pItem)
{
    if (pItem != NULL)
    {
        this->m_courseList.push_back (pItem);     
    }

    return;
}

void CCourse::setId (int id)
{
    this->m_course_id = id;
}

int CCourse::getId (void)
{
    return (this->m_course_id);
}
