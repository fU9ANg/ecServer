
/*
 * student.cpp
 */

#include "student.h"

CStudent::CStudent () {
}

CStudent::~CStudent () {
}

void CStudent::setName (string& s1, string& s2) {
    m_StuName = s1 + s2;
}

string CStudent::getName (void) {
    return m_StuName;
}

void CStudent::setAccount (string& acc) {
    m_AccountName = acc;
}

string CStudent::getAccount (void) {
    return m_AccountName;
}

void CStudent::setOnLine (bool b) {
    m_onLine = b;
}

bool CStudent::getOnLine (void) {
    return m_onLine;
}

void CStudent::setId(int id) {
    m_Id = id;
}

int CStudent::getId() {
    return m_Id;
}

void CStudent::setSocket(int sock) {
    m_Socket = sock;
}

int CStudent::getSocket() {
    return m_Socket;
}

void CStudent::setPictureName (string name)
{
    this->m_picture_name = name;
}

string CStudent::getPictureName (void)
{
    return this->m_picture_name;
}

eClientStatus CStudent::getStudentStatus (void)
{
    return m_current_status;
}

void CStudent::setStudentStatus (eClientStatus ess)
{
    m_current_status = ess;
}
