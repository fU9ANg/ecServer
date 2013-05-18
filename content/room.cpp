/*
 * class.cpp
 */

#include "room.h"
#include "roommanager.h"

CRoom::CRoom(int id, string class_name, string white_board):
    m_room_id(id),m_room_name(class_name),m_white_board(white_board) {
        m_white_fd = -1;
        m_teacher_fd = -1;
        m_node_id = 1;
        m_isUsed = 0;
        m_is_show = 0;
        m_current_scene = 100;
}

int CRoom::getIsUsed ()
{
    return (m_isUsed);
}

void CRoom::setIsUsed (int used)
{
    m_isUsed = used;
}

int CRoom::get_room_id() {
    return m_room_id;
}

void CRoom::set_teacher_fd(int fd) {
    m_teacher_fd = fd;
}

void CRoom::set_teacher_name(string name) {
    m_teacher_name = name;
}

string CRoom::get_teacher_name ()
{
    return this->m_teacher_name;
}

void CRoom::set_class_name(string name) {
    m_class_name = name;
}

void CRoom::set_white_fd(int fd) {
    m_white_fd = fd;
}

void CRoom::add_student(int fd, CStudent* pstudent) {
    STUDENTMAP::iterator iter = m_student_map.find(fd);

    if (iter != m_student_map.end()) {
        printf("fd [%d] is exist in the room!", fd);
        delete iter->second;
        m_student_map.erase(fd);
    }

    m_student_map.insert(pair<int, CStudent*>(fd, pstudent));
    printf ("student=%p, used socket_fd=%d, into room (id=%d)\n", \
            pstudent, fd, m_room_id);
}

void CRoom::update_all_student_status (int fd, int flag)
{
    STUDENTMAP::iterator it;
    TSendStudentStatusReq body;
    Buf* p = SINGLE->bufpool.malloc();
    Buf* pp;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();
    char* p_current_data = NULL;
    unsigned int iStudentCnt = 0;

    head->cLen = MSG_HEAD_LEN + sizeof (unsigned int);
    head->cType = ST_SendStudentStatus;
    p_current_data = (((char* )p->ptr ()) + MSG_HEAD_LEN + sizeof (unsigned int));

    // 设置某一学生的状态为OFFLINE
    it = m_student_map.find (fd);
    if (m_student_map.end() != it) {
        printf("room [%d] student disconnected fd [%d], id [%i], name [%s]\n",
                m_room_id,
                it->first,
                it->second->getId(),
                it->second->getName().c_str());
        it->second->setSocket (0);
        if (flag == 0)
            it->second->setStudentStatus (eCS_OFFLINE);
    }

    // 获得所有学生的在线状态信息
    for (it = m_student_map.begin(); it != m_student_map.end(); ++it)
    {
        (void) memset (&body, 0x00, sizeof (body));
        body.student_id = it->second->getId();
        body.status = it->second->getStudentStatus();
        //if (body.student_id == 0)
        //    continue;
        (void) memcpy (p_current_data, &body, sizeof (body));
        cout << "body.student_id = " << body.student_id << endl;
        cout << "body.status = " << body.status << endl;
        p_current_data += sizeof (TSendStudentStatusReq);
        head->cLen += sizeof (TSendStudentStatusReq);
        iStudentCnt++;
    }

    *(unsigned int *)(((char *)p->ptr()) + MSG_HEAD_LEN) = iStudentCnt;

    // 清除学生Map中的某一学生
    it = m_student_map.find (fd);
    if (m_student_map.end() != it) {
    // FIXME: don't copy code
#if 1   // 向教师发送所有学生的当前状态
        Buf* pt = SINGLE->bufpool.malloc ();
        MSG_HEAD* head = (MSG_HEAD*) ((char*) pt->ptr());
        head->cLen = MSG_HEAD_LEN + sizeof (unsigned int) + sizeof (TSendStudentStatusReq);
        head->cType = ST_SendStudentStatus;
        *(unsigned int *)(((char *)pt->ptr()) + MSG_HEAD_LEN) = 1;
        TSendStudentStatusReq* req = (TSendStudentStatusReq*)(((char*)pt->ptr()) + MSG_HEAD_LEN + sizeof (unsigned int));
        req->student_id = it->second->getId ();
        req->status = it->second->getStudentStatus ();
        pt->setsize (head->cLen);
        pt->setfd (m_teacher_fd);
        SINGLE->sendqueue.enqueue (pt);
#endif
#if 1   // 向白板端发送所有学生的当前状态
        pt = SINGLE->bufpool.malloc ();
        head = (MSG_HEAD*) ((char*) pt->ptr());
        head->cLen = MSG_HEAD_LEN + sizeof (unsigned int) + sizeof (TSendStudentStatusReq);
        head->cType = ST_SendStudentStatus;
        *(unsigned int *)(((char *)pt->ptr()) + MSG_HEAD_LEN) = 1;
        req = (TSendStudentStatusReq*)(((char*)pt->ptr()) + MSG_HEAD_LEN + sizeof (unsigned int));
        req->student_id = it->second->getId ();
        req->status = it->second->getStudentStatus ();
        pt->setsize (head->cLen);
        pt->setfd (m_white_fd);
        SINGLE->sendqueue.enqueue (pt);
#endif
        if (flag == 0) {
            delete it->second;
            m_student_map.erase(it);
        }
    }
    // 向每个学生发送所有学生的当前状态
    for (it = m_student_map.begin(); it != m_student_map.end(); it++) {
        pp = SINGLE->bufpool.malloc ();
        (void*) memcpy ((char*)pp->ptr(), (char*)p->ptr(), head->cLen);
        pp->setsize (head->cLen);
        pp->setfd (it->first);
#if 0
        cout << "================================--------------------===============" << endl;
        cout << "CTYPE: " << ((MSG_HEAD*) (pp->ptr()))->cType << endl;
        cout << "LEN: " << *(unsigned int*) (((char*)pp->ptr()) + MSG_HEAD_LEN) << endl;
        cout << "STUDENT_ID: " << *(unsigned int*) (((char*)pp->ptr()) + MSG_HEAD_LEN + sizeof (unsigned int)) << endl;
        cout << "STATUS: " << *(unsigned int*) (((char*)pp->ptr()) + MSG_HEAD_LEN + sizeof (int) + sizeof (int)) << endl;
        cout << "STUDENT_ID: " << *(unsigned int*) (((char*)pp->ptr()) + MSG_HEAD_LEN + sizeof (int) + sizeof (int) + sizeof (int)) << endl;
        cout << "STATUS: " << *(unsigned int*) (((char*)pp->ptr()) + MSG_HEAD_LEN + sizeof (int) + sizeof (int)+ sizeof (int) + sizeof (int)) << endl;
        printf ("address: %p,  %p\n", p, pp);
        cout << "================================++++++++++++++++++++===============" << endl;
#endif
        SINGLE->sendqueue.enqueue (pp);
    }

#if 0
    if (iStudentCnt > 0) {
        // 向教师发送所有学生的当前状态
        Buf* pt = SINGLE->bufpool.malloc ();
        (void*) memcpy ((char*)pt->ptr(), (char*)p->ptr(), head->cLen);
        cout << "packet len: " << head->cLen << endl;
        pt->setsize (head->cLen);
        pt->setfd (m_teacher_fd);
        SINGLE->sendqueue.enqueue (pt);
    }
#endif
    //
    p->reset();
    SINGLE->bufpool.free(p);
}

void CRoom::del_student(int fd) {
    STUDENTMAP::iterator it = m_student_map.find(fd);
    if ( m_student_map.end() != it) {
        printf("room [%d] student disconnected fd [%d] id [%i], name [%s]\n",
                m_room_id,
                it->first,
                it->second->getId(),
                it->second->getName().c_str());

        Buf* p = SINGLE->bufpool.malloc();
        MSG_HEAD* p_head = (MSG_HEAD*)p->ptr();
        p_head->cLen = sizeof(MSG_HEAD) + sizeof(TSendStudentStatusReq);
        p_head->cType = ST_SendStudentStatus;

        TSendStudentStatusReq body;
        body.student_id = it->second->getId();
        body.status = eCS_OFFLINE;//0xFFFFFFFF;
        memcpy(p_head->cData(), &body, sizeof(body));

        p->setsize(p_head->cLen);
        p->setfd(m_teacher_fd);
        SINGLE->sendqueue.enqueue(p);

        delete it->second;
        m_student_map.erase(it);
        STUDENTMAP::iterator itt;
        //向每个学生发送该学生断开消息
        for( itt = m_student_map.begin(); itt != m_student_map.end(); ++itt) {
            Buf* pp = SINGLE->bufpool.malloc();
            MSG_HEAD* pp_head = (MSG_HEAD*)pp->ptr();
            pp_head->cLen = sizeof(MSG_HEAD) + sizeof(TSendStudentStatusReq);
            pp_head->cType = ST_SendStudentStatus;
            memcpy(pp_head->cData(), &body, sizeof(body));
            pp->setsize(pp_head->cLen);
            pp->setfd(itt->first);
            SINGLE->sendqueue.enqueue(pp);
        }
    }
}

void CRoom::del_client(int fd) {
    if (fd == m_teacher_fd) {
        printf("room [%d] teacher disconnected!\n", m_room_id);
        if (ROOMMANAGER->get_room_by_fd (fd)->m_isUsed == 1)
            ROOMMANAGER->get_room_by_fd (fd)->reset ();
        teacher_disconnect();
        m_teacher_fd = 0;
        return;
    }

    if (fd == m_white_fd) {
        printf("room [%d] whiteboard disconnected!\n", m_room_id);
        m_white_fd = 0;
        return;
    }
    //del_student(fd);

    // 改为向所有的学生端发送数据 (学生掉线处理也在其中)
    update_all_student_status (fd, 0);
}

CStudent* CRoom::get_student_by_fd (int fd)
{
    STUDENTMAP::iterator it = m_student_map.find(fd);
    if (it != m_student_map.end()) {
        return it->second;
    }
    return NULL;
}

CTeacher* CRoom::get_teacher_by_fd (int fd)
{
    if (fd == this->m_teacher_fd)
        return &m_teacher;
    else
        return NULL;
}

CCourse* CRoom::get_course_by_name (string course_name)
{
    COURSELIST::iterator it;
    for (it = m_course_list.begin (); it!= m_course_list.end (); it++) {
        if ((*it)->getName () == course_name)
            return (*it);
    }
    return NULL;
}

CCourse* CRoom::get_course_by_id (int id)
{
    COURSELIST::iterator it;
    for (it = m_course_list.begin (); it != m_course_list.end (); it++) {
        if ((*it)->getId() == id)
            return (*it);
    }
    return NULL;
}

void CRoom::add_course (CCourse* pCourse)
{
    if (pCourse != NULL)
        m_course_list.push_back (pCourse);
}

void CRoom::clear_all_course ()
{
    COURSELIST::iterator it;
    for (it = m_course_list.begin(); it != m_course_list.end(); ) {
        delete *it;
        m_course_list.erase(it++);
    }
}

int CRoom::get_teacher_fd ()
{
    return this->m_teacher_fd;
}

string CRoom::get_room_name ()
{
    return this->m_room_name;
}

string CRoom::get_class_name()
{
    return this->m_class_name;
}

int CRoom::get_white_fd ()
{
    return this->m_white_fd;
}

CRoom* CRoom::get_room_by_fd (int fd)
{
    STUDENTMAP::iterator itt;
    for (itt = m_student_map.begin(); itt != m_student_map.end(); ++itt) {
        printf("[student_fd=%d][student=%p]\n", itt->first, itt->second);
    }

    STUDENTMAP::iterator it = m_student_map.find(fd);
    if (it != m_student_map.end()) {
        return this;//it->second;
    }
    else if (fd == this->m_teacher_fd)
        return this;
    else if (fd == this->m_white_fd)
        return this;
    else
        return NULL;
}

int CRoom::reset() {
    STUDENTMAP::iterator it;
    for (it = m_student_map.begin(); it != m_student_map.end();) {
#if 0
        delete it->second;
        m_student_map.erase(it++);
#else
        it->second->setSocket (0);
        it++;
#endif
    }

    COURSELIST::iterator it1;
    for (it1 = m_course_list.begin(); it1 != m_course_list.end();) {
        delete *it1;
        m_course_list.erase(it1++);
    }

    GAMELIST::iterator it2;
    for (it2 = m_game_list.begin(); it2 != m_game_list.end();) {
        delete *it2;
        m_game_list.erase(it2++);
    }
    //m_teacher_fd = 0;
    m_isUsed = 0;
    //m_teacher_name.clear();
    //m_white_fd = 0; // 老师退出电子教室时, 调用reset, 所有白板不用清理
    return 0;
}

void CRoom::teacher_disconnect() {
    STUDENTMAP::iterator it;
    for (it = m_student_map.begin(); it != m_student_map.end(); ++it) {
        Buf* p = SINGLE->bufpool.malloc();
        MSG_HEAD* p_head = (MSG_HEAD*)p->ptr();
        p_head->cLen = sizeof(MSG_HEAD) + sizeof(TSendStudentStatusReq);
        p_head->cType = ST_SendStudentStatus;

        TSendStudentStatusReq body;
        body.student_id = 0xFFFFFFFF;
        body.status = 0xFFFFFFFF;
        memcpy(p_head->cData(), &body, sizeof(body));

        p->setsize(p_head->cLen);
        p->setfd(it->first);
        SINGLE->sendqueue.enqueue(p);
    }

    /// FIXME: 所有的发送信息代码，应提供一个通用的工具类

    /// 发送教室掉线信息给白板端
    Buf* p = SINGLE->bufpool.malloc ();
    MSG_HEAD* p_head = (MSG_HEAD*) p->ptr();
    p_head->cLen = sizeof (MSG_HEAD) + sizeof (TSendStudentStatusReq);
    p_head->cType = ST_SendStudentStatus;
    TSendStudentStatusReq body;
    body.student_id = 0xFFFFFFFF;
    body.status = 0xFFFFFFFF;
    (void) memcpy (p_head->cData(), &body, sizeof (body));
    p->setsize (p_head->cLen);
    p->setfd (m_white_fd);
    SINGLE->sendqueue.enqueue (p);

    /// 释放某个电子教室
    this->reset();
    
}

CGroup* CRoom::get_group_by_fd (int fd)
{
    CGroup* group = NULL;

    GROUPMAP::iterator it;
    for (it = m_buildhouse_groups.begin(); \
            it != m_buildhouse_groups.end(); it++)
    {
        group = it->second->get_group_by_fd (fd);
        if (group != NULL)
            return group;
    }

    return NULL;
}

bool CRoom::add_group (int id, CGroup* cg)
{   
    if (cg)
    {
        m_buildhouse_groups.insert(std::pair<int,CGroup*> (id, cg));
        return true;
    }
    return false;
}

SceneType CRoom::get_current_st (void)
{
    return m_current_st;
}

SceneState CRoom::get_current_ss (void)
{
    return m_current_ss;
}

void CRoom::set_current_st (SceneType st)
{
    m_current_st = st;
}

void CRoom::set_current_ss (SceneState ss)
{
    m_current_ss = ss;
}

void CRoom::puzzle_start() {
    STUDENTMAP::iterator iter;
    for (iter = m_student_map.begin(); iter != m_student_map.end(); ++iter) {
        CPuzzle* p_puzzle = new CPuzzle();
        p_puzzle->start();
        m_puzzle_map.insert(pair<int, CPuzzle*>(iter->first, p_puzzle));
    }
}
void CRoom::puzzle_end() {
    char buf[1024] = {0};
    PUZZLEMAP::iterator iter;
    for (iter = m_puzzle_map.begin(); iter != m_puzzle_map.end(); ++iter) {
        iter->second->end();
        char tmp[64] = {0};
        CStudent* p_student = m_student_map[iter->first];
        if (NULL == p_student) {
            continue;
        }
        snprintf(tmp, 
                64,
                "%s:%ld,",
                p_student->getName().c_str(),
                iter->second->get_time());
        strcat(buf, tmp);
    }
    MSG_HEAD head;
    head.cType = ST_Puzzle_EndGame;
    head.cLen = sizeof(MSG_HEAD) + 1024;
    if (0 == m_white_fd ) {
        printf("%s %d no white fd!\n", __FILE__, __LINE__);
        return;
    }
    Buf* p_buf = SINGLE->bufpool.malloc();
    p_buf->setfd(m_white_fd);
    p_buf->setsize(head.cLen);
    memcpy(p_buf->ptr(), &head, sizeof(MSG_HEAD));
    memcpy((char*)p_buf->ptr() + sizeof(MSG_HEAD), buf, 1024);
    SINGLE->sendqueue.enqueue(p_buf);
    printf("%s %d send stop to white!\n", __FILE__, __LINE__);
}

void CRoom::end_puzzle_by_fd (int fd)
{
    CPuzzle* p_puzzle = m_puzzle_map[fd];
    if (NULL != p_puzzle) {
        p_puzzle->end();
    }
}

void CRoom::build_house_start ()
{
#if 1//_BUILD_HOUSE_GMAE
    init_buildhouse_group (NUMS_STU_OF_GROUP);
#else

    // only for testing
    CGroup* p_group = new CGroup("group1");
    STUDENTMAP::iterator iter;
    for (iter = m_student_map.begin(); iter != m_student_map.end(); ++iter)
    {
        p_group->add_student_to_group(iter->first, iter->second);
    }

    //m_buildhouse_groups.insert (pair <int, CGroup*> (1, p_group));
    add_group (1, p_group);
#endif
}

void CRoom::clean_buildhouse_group ()
{
    // clear groups of buildhouse
    GROUPMAP::iterator iter;
    for (iter = m_buildhouse_groups.begin(); iter != m_buildhouse_groups.end();) {
        iter->second->get_make_house()->cleanall ();
        delete iter->second;
        m_buildhouse_groups.erase(iter++);
    }
}

void CRoom::build_house_end ()
{
    clean_buildhouse_group ();
}

void CRoom::init_buildhouse_group (int numsOfStudent)
{
    int  icnt = 0;
    int  iGroup = 1;
    char sGroupName[64];

    if (numsOfStudent < 1) {
        cout << "ERROR: init_buildhouse_group(), numsOfStudent < 1" << endl;
        return;
    }

    CGroup* p_group;
    (void) memset (sGroupName, 0x00, sizeof (sGroupName));
    sprintf (sGroupName, "BuildHouse_Group%d", iGroup++);
    if ((p_group = new CGroup (sGroupName)) == NULL) {
        cout << "ERROR: can't create build_house group" << endl;
        return;
    }

    STUDENTMAP::iterator it;
    for (it = m_student_map.begin(); it != m_student_map.end(); it++) {

        p_group->add_student_to_group (it->first, it->second);

        if (icnt == numsOfStudent) {
            (void) memset (sGroupName, 0x00, sizeof (sGroupName));
            sprintf (sGroupName, "BuildHouse_Group%d", iGroup++);
            if ((p_group = new CGroup (sGroupName)) == NULL) {
                cout << "ERROR: can't create build_house group" << endl;
                return;
            }
            icnt = 0;
            cout << "insert group to map for buildhouse (in for)" << endl;
            m_buildhouse_groups.insert (std::pair <int, CGroup*> (iGroup -1, p_group));
            continue;
        }

        icnt++;
    }

    // added tail member
    if ((icnt > 0) && (icnt < numsOfStudent))
    {
        (void) memset (sGroupName, 0x00, sizeof (sGroupName));
        sprintf (sGroupName, "BuildHouse_Group%d", iGroup);
        if (p_group != NULL)
        {
            cout << "insert group to map for buildhouse (out for)" << endl;
            m_buildhouse_groups.insert (std::pair <int, CGroup*> (iGroup, p_group));
        }
        else
        {
            cout << "[WARNING]: p_group == NULL" << endl;
        }
    }
}

#if 0 /* move function to group class */
unsigned int CRoom::getAutoNodeId ()
{
    return (m_node_id++);
}
#endif
