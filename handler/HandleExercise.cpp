
/*
 * HandleExercise.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"

/*
=====================
 暖身操环节开始 (教师端) *
 暖身操开始播放 (白板端) *
=====================
*/
void CHandleMessage::handleExercise_GameStart (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Exercise_GameStart" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Exercise_GameStart) {
        CHandleMessage::postTeacherToWhite (p, ST_Exercise_GameStart);
    }

    return;
}

/*
=====================
 暖身操播放 (教师端) *
 暖身操播放 (白板端) *
=====================
*/
void CHandleMessage::handleExercise_Play (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Exercise_Play" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Exercise_Play) {
        CHandleMessage::postTeacherToWhite (p, ST_Exercise_Play);
    }

    return;
}

/*
=====================
 暖身操暂停 (教师端) *
 暖身操暂停 (白板端) *
=====================
*/
void CHandleMessage::handleExercise_Pause (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Exercise_Pause" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Exercise_Pause) {
        CHandleMessage::postTeacherToWhite (p, ST_Exercise_Pause);
    }

    return;
}

/*
=====================
 暖身操停止 (教师端) *
 暖身操停止 (白板端) *
=====================
*/
void CHandleMessage::handleExercise_Stop (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Exercise_Stop" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Exercise_Stop) {
        CHandleMessage::postTeacherToWhite (p, ST_Exercise_Stop);
    }

    return;
}

/*
=====================
 暖身操环节结束 (教师端) *
 暖身操结束播放 (白板端) *
=====================
*/
void CHandleMessage::handleExercise_GameEnd (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Exercise_GameEnd" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Exercise_GameEnd) {
        CHandleMessage::postTeacherToWhite (p, ST_Exercise_GameEnd);
    }

    return;
}
