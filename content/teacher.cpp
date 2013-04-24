
/*
 * teacher.cpp
 */

#include "teacher.h"

CTeacher::CTeacher() {
}


void CTeacher::setSocket(int fd) {
    this->m_SocketFd = fd;
}

int CTeacher::getSocket() {
    return this->m_SocketFd;
}

#if 0
bool CTeacher::inCourseList (string& sName) {
    COURSELIST::iterator it;
    for (it = m_CourseList.begin(); it != m_CourseList.end(); it++) {
        if ((*it)->getName() == sName)
            return true;
    }
    return false;
}

void CTeacher::addCourse (string& sName)
{
    PreparedStatement* pstmt = DATABASE->preStatement (SQL_SELECT_COURSE);
    pstmt->setString (1, sName.c_str());
    ResultSet* result = pstmt->executeQuery ();
#if 0
    //
    string sql, resultStr1;
    char str[500];
    sprintf (str, "SELECT course_name FROM course WHERE course_name = '%s'", sName.c_str());
    sql = str;
    result = m_DB->Query (sql);
#endif
    string resultStr1;
    if ((resultStr1 = result->getString ("course_name")) == sName) {
        if (inCourseList (sName)) {
            //LOG::out "course is exists in the courselist
            return;
        }
        else {
            cCourse* tmp = new cCourse;
            m_CourseList.push_back (tmp);
            return;
        }
    }
    else {
        //LOG::out "grade is not exists in the database"
        return;
    }
}

bool CTeacher::delCourse (string& sName)
{
    COURSELIST::iterator it;
    for (it = m_CourseList.begin(); it != m_CourseList.end(); it++) {
        if ((*it)->getName() == sName) {
            m_CourseList.erase (it);
            delete (*it);
            return true;
        }
    }
    //LOG::out "not exist in the CourseList"
    return false;
}
#endif
void CTeacher::setAccount (string& sAccount)
{
    this->m_Account = sAccount;
}

string CTeacher::getAccount (void)
{
    return m_Account;
}

string CTeacher::getName (void)
{
    return this->m_TeacherName;
}

void CTeacher::setName (string& fName, string& lName)
{
    this->m_TeacherName = fName + lName;
}
