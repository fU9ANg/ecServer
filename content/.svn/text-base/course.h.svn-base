
/*
 * course.h
 */

#ifndef _C_COURSE_H
#define _C_COURSE_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "courseitem.h"

using namespace std;

typedef list<CCourseItem*> COURSEITEMLIST;

class CCourse
{
private:
    string m_name;
    int m_course_id;
    COURSEITEMLIST m_courseList;
public:
    CCourse ();
	~CCourse ();

    CCourseItem* get_item_by_id (int id);
    CCourseItem* get_item_by_name (string name);
    void add_item (CCourseItem *);

    void setName (string sName);
    string getName (void);

    void setId (int id);
    int getId (void);
};

#endif //_C_COURSE_H
