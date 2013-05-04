
/*
 * HandleClassRoom.cpp
 */

#include "HandleMessage.h"

#include "roommanager.h"
#include "protocol.h"
#include "Buf.h"
#include "LoginCheck.h"

/*
=====================
 
=====================
*/
#if 0
void CHandleMessage::handleTest (Buf* p)
{
    cout << "CT_Test\n";
    //todo:
}
#endif
/*
=====================
 
=====================
*/
void CHandleMessage::handleChangeScene (Buf* p)
{
    cout << "CT_ChangeScene\n";
    //printf ("change scene: %d\n", *(int *)((char *)p->ptr () + sizeof (MSG_HEAD)));
////测试使用!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#if 0
    MSG_HEAD* p_head = (MSG_HEAD*)p->ptr();
    p_head->cLen = sizeof(MSG_HEAD) + sizeof(int);
    int* p_int = (int*)p_head->cData();
    if ( *p_int == 1) {
        *p_int = 3;
        p->setsize(p_head->cLen);

        SINGLE->sendqueue.enqueue(p);
        printf("send change scence!\n");
    }
#endif
    //CHandleMessage::postTeacherToAllStudent(p, ST_ChangeScene);
}

void CHandleMessage::handleControlChangeScene (Buf* p)
{
    cout << "CT_ControlChangeScene\n";
    printf ("control change scene: %d\n", *(int *)((char *)p->ptr () + sizeof (MSG_HEAD)));
    Buf* pp = SINGLE->bufpool.malloc();

    pp->setfd (p->getfd());
    pp->setsize (MSG_HEAD_LEN + sizeof (int));
    MSG_HEAD* head = (MSG_HEAD*) pp->ptr();
    head->cLen = MSG_HEAD_LEN + sizeof (int);
    head->cType = ST_ControlChangeScene;

    *(int *)((char *)pp->ptr () + sizeof (MSG_HEAD)) = *(int *)((char *)p->ptr () + sizeof (MSG_HEAD));
    //cout << "---------size: " << pp->size() << "--------------fd: " << pp->getfd() << endl;

    CRoom* pc = ROOMMANAGER->get_room_by_fd (pp->getfd());

    if (pc != NULL && pc->get_teacher_fd() == pp->getfd()) 
    {
        //cout << "hereeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
    
        pp->setfd (pc->get_white_fd());
        //cout << "here len = " << head->cLen << endl;
        pp->setsize (head->cLen);
        SINGLE->sendqueue.enqueue (pp);
    }
    else 
    {
        cout << "Error: not found 'teacher_fd' in Room" << endl;
        SINGLE->bufpool.free(pp);
    }

    //CHandleMessage::postTeacherToWhite (pp, ST_ControlChangeScene);
    CHandleMessage::postTeacherToAllStudent (p, ST_ControlChangeScene);
}

void CHandleMessage::handleInitSceneFinished (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_InitSceneFinished" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_InitSceneFinished) {
        CHandleMessage::postWhiteToTeacher (p, ST_InitSceneFinished);
    }

    return;
}

/*
=====================
 CT_ConfirmIntoClassRoom
=====================
*/
void CHandleMessage::handleConfirmIntoClassRoom (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_ConfirmIntoClassRoom" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_ConfirmIntoClassRoom)
    {
        head->cType = ST_ConfirmIntoClassRoom;
        ((MSG_HEAD*) ((char*) p->ptr()))->cLen = MSG_HEAD_LEN + sizeof (sCommonStruct);

        CRoom* proom = ROOMMANAGER->get_room_by_fd (p->getfd());
        if (proom == NULL) {
            cout << "Warning: not found class room" << endl;
            return;
        }
        if (proom->get_teacher_fd() <= 0)
            *(int*) (((char*) p->ptr()) + MSG_HEAD_LEN) = TT_LOGOUT_CLASSROOM;
        else
            *(int*) (((char*) p->ptr()) + MSG_HEAD_LEN) = TT_LOGIN_CLASSROOM;

        p->setsize (MSG_HEAD_LEN + sizeof (sCommonStruct));
        //p->setfd (p->getfd());

        cout << "Send ST_ConfirmIntoClassRoom, size=" << p->size() << ", " << "value="
             << *(int*) (((char*) p->ptr()) + MSG_HEAD_LEN);

        SINGLE->sendqueue.enqueue (p);
    }

    return;
}

/*
=====================
 登录 (所有端)
=====================
*/
void CHandleMessage::handleLogin (Buf* p)
{
    cout << "CT_Login\n";
    LoginCheck::check(p);
}

/*
=====================
 登录结果 (所有端)
=====================
*/
void CHandleMessage::handleLoginResult (Buf* p)
{
    cout << "CT_LoginResult\n";
}

/*
=====================
 获得数据库中的课程信息数量 (教师端)
=====================
*/
void CHandleMessage::handleGetCourseDBCount (Buf* p)
{
    cout << "CT_GetDBRecordCount\n";
    CHandleMessage::postDBRecordCount (p, 1);
}

/*
=====================
 获得数据库中的课程信息 (教师端)
=====================
*/
void CHandleMessage::handleGetCourseDB (Buf* p)
{
    cout << "CT_GetDBCourseDB\n";
    CHandleMessage::postDBRecord (p, 1);
}

/*
=====================
 设置开启的课程列表 (教师端)
=====================
*/
void CHandleMessage::handleSetCourseGroup (Buf* p)
{
    cout << "CT_SetCourseGroup\n";

    try {
        MutexLockGuard guard (DATABASE->m_mutex);
        PreparedStatement* pstmt = DATABASE->preStatement(SQL_SELECT_USED_COURSE);

        sCourseGroup* cg = (sCourseGroup*) (((char*)p->ptr()) + MSG_HEAD_LEN);
        cout << "gradeName: " << cg->sGradeName << endl;
        cout << "courseName: " << cg->sCourseGroupName << endl;
        pstmt->setString (1, cg->sGradeName);
        pstmt->setString (2, cg->sCourseGroupName);
        ResultSet* prst = pstmt->executeQuery ();

        cout << "get roooooooooooooooooooooooooooom" << endl;
        CRoom* room = ROOMMANAGER->get_room_by_fd (p->getfd());
        if (room != NULL) {
            cout << "clear all course on list" << endl;
            room->clear_all_course ();
        }
        while (prst->next())
        {
            if (room != NULL)
            {
                CCourse* course = new CCourse ();
                if (course != NULL){
                    cout << "course_id: " << prst->getInt ("course_id") << endl;
                    cout << "course_name: " << prst->getString ("course_name") << endl;
                    course->setId (prst->getInt ("course_id"));
                    course->setName (prst->getString ("course_name"));
                    room->add_course (course);
                }
            }
        }

        delete pstmt;
        delete prst;
    }
    catch (SQLException e) {
        cout << e.what() << endl;
    }

    // select record of coruseitem from database, not 
    // from item_list of course get information (record)
    // 同步教师进行电子教室到所有的学生端 (其实应该放在LoginClassRoom的handler中去处理)
    Buf* tmp = SINGLE->bufpool.malloc ();
    if (tmp == NULL)
    {
        cout << "out of memory" << endl;
        return;
    }
    
    ((MSG_HEAD*) ((char*) tmp->ptr()))->cLen = MSG_HEAD_LEN + sizeof (sCommonStruct);
    *(int*) (((char*) tmp->ptr()) + MSG_HEAD_LEN) = TT_LOGIN_CLASSROOM;
    tmp->setsize (MSG_HEAD_LEN + sizeof (sCommonStruct));
    tmp->setfd (p->getfd());

    cout << "Send ST_ConfirmIntoClassRoom, size=" << tmp->size() << ", " << "value="
        << *(int*) (((char*) tmp->ptr()) + MSG_HEAD_LEN);
    CHandleMessage::postTeacherToAllStudent (tmp, ST_ConfirmIntoClassRoom);

    // Send teacher login information to whiteboard.
    Buf* pp;
    if ((pp = SINGLE->bufpool.malloc ()) == NULL)
    {
        cout << "out of memory" << endl;
        return;
    }
    MSG_HEAD* head = NULL;
    head = (MSG_HEAD*) ((char*) pp->ptr());
    head->cLen = MSG_HEAD_LEN + sizeof (int);
    head->cType = ST_ConfirmIntoClassRoom;
    *(int*) (((char*)pp->ptr()) + MSG_HEAD_LEN) = TT_LOGOUT_CLASSROOM;
    CHandleMessage::postTeacherToWhite (pp, ST_ConfirmIntoClassRoom);
}

/*
=====================
 获得课程列表 (教师端和所有学生端)
=====================
*/
void CHandleMessage::handleGetCourseGroup (Buf* p)
{
    cout << "CT_GetCourseGroup\n";
    string sResult;

#if 1
    CRoom* room = ROOMMANAGER->get_room_by_fd (p->getfd());
    if (room != NULL)
    {
        CRoom::COURSELIST::iterator it;
        for (it = room->m_course_list.begin (); it != room->m_course_list.end (); ++it) {
            sResult += (*it)->getName();
            sResult += ",";
        }
    }
#else
    sResult += "拼图,造房子,暖身操,动画片,";
#endif
    struct sGetCourseGroup cList;
    memset (&cList, 0x00, sizeof (sGetCourseGroup));
    cout << "Course Name List: " << sResult.c_str() << "Course List Size: " << sResult.size() << endl;
    strncpy (cList.sCourseList, sResult.c_str(), sResult.size());
    cout << "get course List: " << cList.sCourseList << endl;
    cout << "get course List: " << sResult.c_str() << endl;
    cout << "Buf.fd: " << p->getfd() << endl;

    MSG_HEAD* head = (MSG_HEAD*) p->ptr();
    head->cType = ST_GetCourseGroup;
    head->cLen = MSG_HEAD_LEN + sizeof (struct sGetCourseGroup);
    memcpy ((char*)p->ptr() + MSG_HEAD_LEN, &cList, sizeof (struct sGetCourseGroup));

    p->setsize (head->cLen);
    SINGLE->sendqueue.enqueue (p);

    return;
}

/*
=====================
 获得课程列表的子项数量 (教师端)
=====================
*/
void CHandleMessage::handleGetCourseItemCount (Buf* p)
{
    cout << "CT_GetCourseItemCount\n";
    CHandleMessage::postDBRecordCount (p, 6);
}

/*
=====================
 获得课程列表的子项 (教师端)
=====================
*/
void CHandleMessage::handleGetCourseItem (Buf* p)
{
    cout << "CT_GetCourseItem\n";
    CHandleMessage::postDBRecord (p, 6);
}

/*
=====================
 登出 (所有端)
=====================
*/
void CHandleMessage::handleLogout (Buf* p)
{
    cout << "CT_Logout\n";
}

/*
=====================
 登出结果 (所有端)
=====================
*/
void CHandleMessage::handleLogoutReuslt (Buf* p)
{
    cout << "CT_LogoutReuslt\n";
}

/*
=====================
 获得教室数据库信息数量 (教师端)
=====================
*/
void CHandleMessage::handleGetClassRoomDBCount (Buf* p)
{
    cout << "CT_GetClassRoomDBCount\n";
    CHandleMessage::postDBRecordCount (p, 4);
}

/*
=====================
 获得教室数据库信息 (教师端)
=====================
*/
void CHandleMessage::handleGetClassRoomDB (Buf* p)
{
    cout << "CT_GetClassRoomDB\n";
    CHandleMessage::postDBRecord (p, 4);
}

/*
=====================
 获得教室年级数据库信息数量 (教师端)
=====================
*/
void CHandleMessage::handleGetGradeDBCount (Buf* p)
{
    cout << "CT_GetGradeDBCount\n";
    CHandleMessage::postDBRecordCount (p, 2);
}

/*
=====================
 获得教室年级数据库信息 (教师端)
=====================
*/
void CHandleMessage::handleGetGradeDB (Buf* p)
{
    cout << "CT_GetGradeDB\n";
    CHandleMessage::postDBRecord (p, 2);
}

/*
=====================
 获得班级信息数量 (教师端)
=====================
*/
void CHandleMessage::handleGetClassDBCount (Buf* p)
{
    cout << "CT_GetClassDBCount\n";
    CHandleMessage::postDBRecordCount (p, 3);
}

/*
=====================
 获得班级信息 (教师端)
=====================
*/
void CHandleMessage::handleGetClassDB (Buf* p)
{
    cout << "CT_GetClassDB\n";
    CHandleMessage::postDBRecord (p, 3);
}

/*
=====================
 进入教室 (所有学生端)
=====================
*/
void CHandleMessage::handleLoginClassRoom (Buf* p)
{
    struct sLoginOutClassRoom st_login_class_room;
    memcpy(&st_login_class_room,
            (char*)p->ptr() + sizeof(MSG_HEAD),
            sizeof(struct sLoginOutClassRoom));
    CRoom* proom = ROOMMANAGER->get_room_by_name(st_login_class_room.sClassRoomName);
    if( NULL == proom) {
        SINGLE->bufpool.free(p);
        LOG(ERROR) << "can't find class room " << st_login_class_room.sClassRoomName <<endl;
        printf("can't find class room [%s]\n", st_login_class_room.sClassRoomName);
        return;
    }

    if (proom->getIsUsed() == 1)
#if 0
    if ((p->getfd() == proom->get_teacher_fd()) || \
        (strcmp(st_login_class_room.sTeacherName, proom->get_teacher_name().c_str()) == 0))
#endif
    {
        cout << "classroom used by teachername='" << proom->get_teacher_name() << "'" << endl;
        return;
    }

    proom->set_teacher_fd(p->getfd());
    proom->set_teacher_name(st_login_class_room.sTeacherName);
    proom->set_class_name(st_login_class_room.sClassName);
    printf("Teacher login class room [%d] success!\n", proom->get_room_id());
    proom->setIsUsed (1);
    SINGLE->bufpool.free(p);
    //todo:
}

/*
=====================
 退出教室 (教师端和所有学生端)
=====================
*/
void CHandleMessage::handleLogoutClassRoom (Buf* p)
{
    CRoom* room = ROOMMANAGER->get_room_by_fd (p->getfd());
    if (room != NULL) {
        if (room->get_teacher_by_fd (p->getfd ())) {
            room->set_teacher_fd (0);   // set teacher fd to INVALID
            room->setIsUsed (0);
            room->reset ();
            // send teacher logout information to all students.
            // TODO:
            MSG_HEAD* head = (MSG_HEAD*) ((char*)p->ptr());
            head->cLen = MSG_HEAD_LEN + sizeof (int);
            head->cType = ST_ConfirmIntoClassRoom;
            *(int*) (((char*)p->ptr()) + MSG_HEAD_LEN) = TT_LOGOUT_CLASSROOM;
            CHandleMessage::postTeacherToAllStudent (p, ST_ConfirmIntoClassRoom);

            // Send teacher login information to whiteboard.
            Buf* pp;
            if ((pp = SINGLE->bufpool.malloc ()) == NULL)
            {
                cout << "out of memory" << endl;
                return;
            }
            head = (MSG_HEAD*) ((char*) pp->ptr());
            head->cLen = MSG_HEAD_LEN + sizeof (int);
            head->cType = ST_ConfirmIntoClassRoom;
            *(int*) (((char*)pp->ptr()) + MSG_HEAD_LEN) = TT_LOGOUT_CLASSROOM;
            CHandleMessage::postTeacherToWhite (pp, ST_ConfirmIntoClassRoom);
        }
    }
    return;
}

/*
=====================
 获得学生信息 (所有端)
=====================
*/
void CHandleMessage::handleGetStudentDetailInfo (Buf* p)
{
    cout << "CT_GetStudentDetailInfo" << endl;
    
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_GetStudentDetailInfo)
    {
        try {
            sStudentDetail* sd = (sStudentDetail*) ((char*)((MSG_HEAD*)p->ptr()) + MSG_HEAD_LEN);
            cout << "stu_id:" << sd->iStudentId << ", stu_name:" << sd->sStudentName << endl;

            MutexLockGuard guard (DATABASE->m_mutex);
            PreparedStatement* pstmt = NULL;
            struct sGetStudentDetailInfo detail_info;
            (void) memset (&detail_info, 0x00, sizeof (detail_info));

            pstmt = DATABASE->preStatement (SQL_SELECT_STUDENT_DETAILINFO);
            pstmt->setInt (1, sd->iStudentId);
            pstmt->setString (2, sd->sStudentName);
            ResultSet* prst = pstmt->executeQuery();
            while (prst->next()) {
                memset (&detail_info, 0x00, sizeof (detail_info));
                strcpy (detail_info.sNumber, prst->getString ("number").c_str());
                strcpy (detail_info.sLastName, prst->getString ("last_name").c_str());
                strcpy (detail_info.sFirstName, prst->getString ("first_name").c_str());
                strcpy (detail_info.sSex, prst->getString ("sex").c_str());
                strcpy (detail_info.sSchoolName, prst->getString ("school_name").c_str());
                strcpy (detail_info.sGradeName, prst->getString ("grade_name").c_str());
                strcpy (detail_info.sClassName, prst->getString ("class_name").c_str());
                strcpy (detail_info.sAccount, prst->getString ("account").c_str());
                strcpy (detail_info.stFirstName, prst->getString ("tf_name").c_str());
                strcpy (detail_info.stLastName, prst->getString ("tl_name").c_str());
                detail_info.iPicture_id = prst->getInt ("picture_id");
#if 0
                strcpy (detail_info.birthday, prst->getString ("birthday").c_str());
                cout << "6666" << endl;
                cout << " number:" << detail_info.sNumber << endl
                     << " class_name:" << detail_info.sClassName << endl
                     << " sex:" << detail_info.sSex << endl;
#endif
            }
            CHandleMessage::postMessage (p, ST_GetStudentDetailInfo, (void*)&detail_info, sizeof (detail_info));
            delete pstmt;
            delete prst;
        }
        catch (SQLException e) {
            LOG(ERROR) << e.what() << endl;
        }
    }

    return;
}

/*
=====================
 获得所有学生列表数量 (所有端)
=====================
*/
void CHandleMessage::handleGetAllStudentInfoCount (Buf* p)
{
    cout << "CT_GetAllStudentInfoCount" << endl;
    CHandleMessage::postDBRecordCount (p, 5);
}

/*
=====================
 获得所有学生列表 (所有端)
=====================
*/
void CHandleMessage::handleGetAllStudentInfo (Buf* p)
{ 
    cout << "CT_GetAllStudentInfo" << endl;
    CHandleMessage::postDBRecord (p, 5);
}

/*
=====================
 早退 (教师端)
 早退通知给 (学生端)
=====================
*/
void CHandleMessage::handleLeaveEarly (Buf* p)
{
    cout << "CT_LeaveEarly" << endl;
    if (NULL == p)
        return;

    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_LeaveEarly) {
        CHandleMessage::postTeacherToStudent (p, ST_LeaveEarly, \
            ((sLeaveEarly*)((char*)p->ptr() + MSG_HEAD_LEN))->student_id);
        //CHandleMessage::postTeacherToAllStudent (p, ST_LeaveEarly);
    }
}

/*
=====================
 早退确认 (学生端) *
=====================
*/
void CHandleMessage::handleConfirmLeaveEarly (Buf* p)
{
    //todo:
    cout << "CT_ConfirmLeaveEarly" << endl;
    if (NULL == p)
        return;

    CRoom* room = ROOMMANAGER->get_room_by_fd (p->getfd());
    if (room == NULL) {
        cout << "room pointer is NULL" << endl;
        return;
    }
    CStudent* stu = room->get_student_by_fd (p->getfd());
    if (stu == NULL) {
        cout << "student pointer is NULL" << endl;
        return;
    }
    stu->setStudentStatus (eCS_LEAVEEARLY);

#if 0   // 向教师发送所有学生的当前状态
        Buf* pt = SINGLE->bufpool.malloc ();
        MSG_HEAD* head = (MSG_HEAD*) ((char*) pt->ptr());
        head->cLen = MSG_HEAD_LEN + sizeof (unsigned int) + sizeof (TSendStudentStatusReq);
        head->cType = ST_SendStudentStatus;
        *(unsigned int *)(((char *)pt->ptr()) + MSG_HEAD_LEN) = 1;
        TSendStudentStatusReq* req = (TSendStudentStatusReq*)(((char*)pt->ptr()) + MSG_HEAD_LEN + sizeof (unsigned int));

        room = ROOMMANAGER->get_room_by_fd (p->getfd());
        if (room == NULL) {
            cout << "room pointer is NULL" << endl;
            return;
        }
        stu = room->get_student_by_fd (p->getfd);
        if (stu == NULL) {
            cout << "student pointer is NULL" << endl;
            return;
        }
        req->student_id = stu->getId ();
        req->status = stu->getStudentStatus ();

        pt->setsize (head->cLen);
        pt->setfd (m_teacher_fd);
        SINGLE->sendqueue.enqueue (pt);
#endif
    room->update_all_student_status (p->getfd(), 1);
}

/*
=====================
 获得教师信息 (所有学生端和教师端)
=====================
*/
void CHandleMessage::handleGetTeacherInfo (Buf* p)
{
    if (p == NULL) return;

    cout << "CT_GetTeacherInfo" << endl;

    struct sGetTeacherInfo ti;
    memset (&ti, 0x00, sizeof (ti));
    MSG_HEAD* head = (MSG_HEAD*) p->ptr();

    if (head->cType == CT_GetTeacherInfo)
    {
        try {
            MutexLockGuard guard (DATABASE->m_mutex);
            #ifdef _TEACHER_NOLOGIN
            string teaname("张三");
            #else
            CRoom* room = ROOMMANAGER->get_room_by_fd (p->getfd());
            if (room == NULL) return;
            string teaname = room->get_teacher_name ();
            #endif
            cout << "get teacher info - name: " << teaname << endl;
            PreparedStatement* pstmt = DATABASE->preStatement(SQL_SELECT_TEACHER_DETAILINFO);

            pstmt->setString (1, teaname.c_str());
            ResultSet* prst = pstmt->executeQuery ();

            while (prst->next())
            {
                #ifdef _TEACHER_NOLOGIN
                #else
                if (room != NULL)
                #endif
                {

                    strcpy (ti.sTeacherName, prst->getString ("account").c_str());
                    strcpy (ti.sPicName, prst->getString ("pic_name").c_str());
                    cout << "account: " << ti.sTeacherName << " " << prst->getString ("account") << endl;
                    cout << "pic_name: " << ti.sPicName << " " << prst->getString ("pic_name") << endl;

                }
            }

            postMessage (p, ST_GetTeacherInfo, &ti, sizeof (sGetTeacherInfo));
            delete pstmt;
            delete prst;
        }
        catch (SQLException e) {
            cout << e.what() << endl;
        }
    }
    return;
}

/*
=====================
 锁定学生端 (教师端) *
 锁定学生端 (学生端) *
=====================
*/
void CHandleMessage::handleLockStudent (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_LockStudent" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_LockStudent) {
        postTeacherToAllStudent (p, ST_LockStudent);
    }

    return;
}

/*
=====================
 
=====================
*/
void CHandleMessage::handleSelectedClassRoom (Buf* p)
{
    //todo:
    TSelectedClassRoom* pp = (TSelectedClassRoom*)((char*)p->ptr() + sizeof(MSG_HEAD));

    cout << "classroom_id: " << pp->classroom_id << endl;
    cout << "client_type: " << pp->client_type << endl;
    CRoom* proom = ROOMMANAGER->get_room(pp->classroom_id);
    if (proom == NULL)
        return;
    if (MCT_STUDENT == pp->client_type) {
        CStudent* pstudent = new CStudent();
        pstudent->setId (0);
        pstudent->setOnLine (false);
        pstudent->setStudentStatus (eCS_OFFLINE);
        if (ROOMMANAGER->get_room_by_fd (p->getfd()))
        {
            printf ("student fd exist in the room\n");
            return;
        }
        proom->add_student(p->getfd(), pstudent);
        printf("student login room[%d] fd = [%d]\n", pp->classroom_id, p->getfd());
    }
    else if (MCT_WHITEBOARD == pp->client_type) {
        //printf("white board login classroom[%d]", proom->get_room_id());
        cout << "white board login classroom = " << proom->get_room_id() << ", fd = " << p->getfd() << endl;
        proom->set_white_fd(p->getfd());
    }
    //p->reset();
    SINGLE->bufpool.free(p);
}

/*
=====================
 
=====================
*/
void CHandleMessage::handleCourseFinished (Buf* p)
{
    //todo:
}


/*
=====================
 CT_GetDBRecordFinished, // 请求所有数据库记录是否完成
 ST_GetDBRecordFinished, // 服务器回应数据库记录是否完成
=====================
*/
void CHandleMessage::handleDBRecordFinished (Buf* p)
{
    cout << "CT_GetDBRecordFinished and ST_GetDBRecordFinished" << endl;
    if (p == NULL) {
        return;
    }

    p->reset();
    SINGLE->bufpool.free (p);
#if 0
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();
    struct sDBRecordFinished finished;
    finished.iFlagFinished = 1;

    if (head->cType == CT_GetDBRecordFinished) {
        head->cLen = sizeof (MSG_HEAD) + sizeof (struct sDBRecordFinished);
        head->cType = ST_GetDBRecordFinished;
        memcpy (head->cData(), &finished, sizeof (struct sDBRecordFinished));
        p->setsize (head->cLen);
        SINGLE->sendqueue.enqueue (p);
    }
#endif
    return;
}

void CHandleMessage::handleGetCourseItemKeyInfoReq (Buf* p) {
    MSG_HEAD* p_head = (MSG_HEAD*)p->ptr();
    string keys_info;

    GetCourseItemKeyInfoRsp rsp;
    memset (&rsp, 0x00, sizeof (rsp));
    try {
        MutexLockGuard guard (DATABASE->m_mutex);
        PreparedStatement* pstmt = DATABASE->preStatement(SQL_SELECT_ITEM_KEYS);
        //cout << "Req: " << *(int*)p_head->cData();
        //pstmt->setInt(1, *(int*)p_head->cData());
        GetCourseItemKeyInfoReq *req = (GetCourseItemKeyInfoReq*) ((char *)p_head->cData());
        cout << "Req: '" << req->course_item << "'" << endl;
        pstmt->setString (1, req->course_item);
        ResultSet* rst = pstmt->executeQuery();
        while(rst->next()) {
            strcpy(rsp.keys, rst->getString("keys_info").c_str());
        }
        delete rst;
        delete pstmt;
    }catch( SQLException& e) {
        printf(" %s %d:%s", __FUNCTION__, __LINE__, e.what());
        LOG(ERROR) <<e.what()<<endl;
        SINGLE->bufpool.free(p);
        return;
    }

    p_head->cLen = sizeof(MSG_HEAD) + sizeof(GetCourseItemKeyInfoRsp);
    p_head->cType = CT_GetCourseItemKeyInfoRsp;
    memcpy(p_head->cData(), &rsp, sizeof(rsp));
    p->setsize(p_head->cLen);
    SINGLE->sendqueue.enqueue(p);

#if 0
    do {
        cout << "send finished flags handleGetCourseItemKeyInfoReq -----------" << endl;
        Buf* pbuf = SINGLE->bufpool.malloc();
        MSG_HEAD* phead = (MSG_HEAD*)pbuf->ptr();
        sDBRecordFinished finished;
        finished.iFlagFinished = CT_GetCourseItemKeyInfoRsp;

        phead->cType = CT_GetCourseItemKeyInfoRsp;
        phead->cLen = sizeof (MSG_HEAD) + sizeof (sDBRecordFinished);

        memcpy (((char*)pbuf->ptr()) + MSG_HEAD_LEN, &finished, sizeof (sDBRecordFinished));
        pbuf->setfd (p->getfd());
        pbuf->setsize (phead->cLen);
        SINGLE->sendqueue.enqueue (pbuf);
    } while (0);
#endif /////////
}

/*
=====================
 CT_Common_PlayorPause (通用的播放暂停处理)
 ST_Common_PlayorPause (通用的播放暂停处理)
=====================
*/
void CHandleMessage::handleCommonPlayorPause (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Common_PlayorPause" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Common_PlayorPause) {
        postTeacherToWhite (p, ST_Common_PlayorPause);
    }

    return;
}

/*
=====================
 CT_Common_Resume
 ST_Common_Resume
=====================
*/
void CHandleMessage::handleCommonResume (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Common_Resume" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Common_Resume) {
        postTeacherToWhite (p, ST_Common_Resume);
    }

    return;
}

/*
=====================
 CT_Common_Stop
 ST_Common_Stop
=====================
*/
void CHandleMessage::handleCommonStop (Buf * p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Common_Stop" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Common_Stop) {
        postTeacherToWhite (p, ST_Common_Stop);
    }

    return;
}

/*
====================
 CT_Scene_End
 ST_Scene_End
====================
*/
void CHandleMessage::handleSceneEnd (Buf* p)
{
    if (p == NULL)  return;

    cout << "process: CT_Scene_End" << endl;

    MSG_HEAD* head = (MSG_HEAD*) p->ptr();
    sSceneEnd* se = (sSceneEnd*) ((char*)p->ptr() + MSG_HEAD_LEN);
    if (head->cType == CT_Scene_End) {
        postMessage (p, ST_Scene_End, se, head->cLen - MSG_HEAD_LEN);
    }

    return;
}
