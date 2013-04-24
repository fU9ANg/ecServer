
/*
 * teacher.h
 */

#ifndef _C_TEACHER_H
#define _C_TEACHER_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "database.h"
#include "SQL.h"

using namespace std;

class CTeacher
{
	//typedef list<cCourse*> COURSELIST;

private:
	//COURSELIST  m_CourseList;
	string      m_TeacherName;
    string      m_Account;
    int         m_SocketFd;

public:
    CTeacher();
	//bool inCourseList (string& sName);

	//void addCourse (string& sName);
	//bool delCourse (string& sName);
	
	string getName (void);
    int getSocket();
	void setName (string& fName, string& lName);
    void setSocket(int fd);

    void setAccount (string& sAccount);
    string getAccount (void);
};

#endif // _C_TEACHER_H
