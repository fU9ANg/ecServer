
/*
 * HandlePuzzle.cpp
 */

#include "HandleMessage.h"
#include "roommanager.h"

#include "protocol.h"
#include "Buf.h"

/*
=====================
 点击拼图开始 (教师端)
 发送拼图游戏开始信息 (学生端和白板端)
=====================
*/
void CHandleMessage::handlePuzzle_GameStart (Buf* p)
{
    //todo:
    CHandleMessage::postTeacherToAllStudent(p, ST_Puzzle_GameStart);
    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if (NULL != p_room) {
        p_room->puzzle_start();
    }
    printf("send  Puzzle_GameStart to students!\n");
    return;
}

/*
=====================
 获得拼图是否正确的状态 (学生端) *
 发送拼图是否正确的状态 (白板端) *
=====================
*/
void CHandleMessage::handlePuzzle_IconStatus (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_Puzzle_IconStatus" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Puzzle_IconStatus) {
        CHandleMessage::postTeacherToWhite (p, ST_Puzzle_IconStatus);
    }

    return;
#endif
}

/*
=====================
 拼图播放 (学生端) *
 拼图播放 (白板端) *
=====================
*/
void CHandleMessage::handlePuzzle_Play (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_Puzzle_Play" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Puzzle_Play) {
        CHandleMessage::postTeacherToWhite (p, ST_Puzzle_Play);
    }

    return;
#endif
}

/*
=====================
 拼图暂停 (学生端) *
 拼图暂停 (白板端) *
=====================
*/
void CHandleMessage::handlePuzzle_Pause (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_Puzzle_Pause" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Puzzle_Pause) {
        CHandleMessage::postTeacherToWhite (p, ST_Puzzle_Pause);
    }

    return;
#endif
}

/*
=====================
 拼图恢复 (学生端) *
 拼图恢复 (白板端) *
=====================
*/
#if 0
void CHandleMessage::handlePuzzle_Resume (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_Puzzle_Resume" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Puzzle_Resume) {
        CHandleMessage::postTeacherToWhite (p, ST_Puzzle_Resume);
    }

    return;
#endif
}
#endif

/*
=====================
 拼图区中移动某一拼图块 (客户端) **
 拼图区中移动某一拼图块 (白板端) **
=====================
*/
void CHandleMessage::handlePuzzle_MovePic (Buf* p)
{
    //todo:
}

/*
=====================
 拼图区中删除某一拼图块 (客户端) **
 拼图区中删除某一拼图块 (白板端) **
=====================
*/
void CHandleMessage::handlePuzzle_DeletePic (Buf* p)
{
    //todo:
}

/*
=====================
 拼图区中添加某一拼图块 (客户端) **
 拼图区中添加某一拼图块 (白板端) **
=====================
*/
void CHandleMessage::handlePuzzle_AddPic (Buf* p)
{
    //todo:
}

/*
=====================
 拼图的结果 (教师端) &*
=====================
*/
void CHandleMessage::handlePuzzle_CalcScore (Buf* p)
{
    //todo:
}

/*
=====================
 结束拼图课程 (教师端) *
 结束拼图课程 (白板端和所有学生端) *
=====================
*/
void CHandleMessage::handlePuzzle_EndGame (Buf* p)
{
    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if (NULL == p_room) {
        SINGLE->bufpool.free(p);
        return;
    }

    if (p->getfd() == p_room->get_teacher_fd()) {
        p_room->puzzle_end();
    }
    else{
        p_room->end_puzzle_by_fd(p->getfd());
    }
    SINGLE->bufpool.free(p);
}

/*
====================
 拼图的类型选择
 CT_Puzzle_SelectedType
====================
*/
void CHandleMessage::handlePuzzle_SelectedType (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Puzzle_SelectedType" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Puzzle_SelectedType) {
        CHandleMessage::postTeacherToWhite (p, ST_Puzzle_SelectedType);
    }

}

/*
====================
 拼图的大小选择
 CT_Puzzle_SelectedSize
====================
*/
void CHandleMessage::handlePuzzle_SelectedSize (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Puzzle_SelectedSize" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Puzzle_SelectedSize) {
        CHandleMessage::postTeacherToWhite (p, ST_Puzzle_SelectedSize);
    }

}

void CHandleMessage::handlePuzzle_GetPic(Buf* p)
{
#if 0
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    head->cType = CT_Puzzle_GetPic;
    int fd = open("~/pic/p3.png", O_WRONLY|O_CREAT);
    size_t piclen = read(fd, (char*)head->cData() ,102400000);
    close(fd);
    head->cLen = piclen + sizeof(MSG_HEAD);
    CHandleMessage::postTeacherToAllStudent (p, ST_Puzzle_EndGame);
#else
    int fd = -1;
    MSG_HEAD* head;
    Buf* pp;
    unsigned int type;
    unsigned int index;

    if ((fd = open ("~/pic/p3.png", O_WRONLY)) < 0) {
        cout << "ERROR: open() function for open picture." <<endl;
        return;
    }

    while (1) {
        if ((pp = SINGLE->bufpool.malloc ()) != NULL)
        {
            index = 0;
            head = (MSG_HEAD*) pp->ptr();
            size_t picLen = read (fd, (char*) head->cData(), 1000);
            if (picLen == 0)
            {
                cout << "send picture data finished." << endl;
                break;
            }
            head->cLen = picLen + MSG_HEAD_LEN;
            type = 1000 + index++;
            memcpy (&(head->cType), &type, sizeof (unsigned int));
            pp->setsize (head->cLen);
            pp->setfd (p->getfd());
            SINGLE->sendqueue.enqueue (pp);
        }
        else
        {
            cout << "out of memory\n" << endl;
            return;
        }
    }
#if 1
        do {
            cout << "send picture data finished flags -----------" << endl;
            Buf* ppp = SINGLE->bufpool.malloc ();
            MSG_HEAD* phead = (MSG_HEAD*)ppp->ptr();
            struct sDBRecordFinished finished;
            finished.iFlagFinished = 1;

            phead->cLen = sizeof (MSG_HEAD) + sizeof (struct sDBRecordFinished);
            phead->cType = ST_GetDBRecordFinished;
            memcpy (((char*)ppp->ptr()) + MSG_HEAD_LEN, &finished, sizeof (struct sDBRecordFinished));
            ppp->setfd (p->getfd());
            ppp->setsize (phead->cLen);
            SINGLE->sendqueue.enqueue (ppp);
        } while (0);
#endif
#endif
}

void CHandleMessage::handlePuzzle_UpdatePic(Buf* p) {
    MSG_HEAD* pp = (MSG_HEAD*)p->ptr();
    pp->cType = ST_Puzzle_UpdatePic;
    struct sPuzzleUpdatePic *updatepic = (struct sPuzzleUpdatePic*)pp->cData();
    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if (NULL == p_room ) {
        SINGLE->bufpool.free(p);
        printf("cat find room %s %s %d!\n", __FILE__, __FUNCTION__, __LINE__);
        return;
    }

    CStudent* p_student = p_room->get_student_by_fd(p->getfd());

    if (NULL == p_student ) {
        printf("cat find student %s %s %d!\n", __FILE__, __FUNCTION__, __LINE__);
        SINGLE->bufpool.free(p);
        return;
    }

#if 1
    updatepic->student_id = p_student->getId();
#else
    updatepic->student_id = 1;
#endif

    p->setfd(p_room->get_white_fd());
    p->setsize(sizeof(MSG_HEAD) + sizeof(struct sPuzzleUpdatePic));
    SINGLE->sendqueue.enqueue(p);

    //postStudentToWhite(p, ST_Puzzle_UpdatePic);
}
