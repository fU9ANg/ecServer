
/*
 * HandleCartoon.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"


/*
=====================
 动画片环节开始 (教师端) *
 动画片开始 (白板端) *
=====================
*/
void CHandleMessage::handleCartoon_GameStart (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Cartoon_GameStart" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Cartoon_GameStart) {
        CHandleMessage::postTeacherToWhite (p, ST_Cartoon_GameStart);
    }

    return;
}

/*
=====================
 动画片播放 (教师端) *
 动画片播放 (白板端) *
=====================
*/
void CHandleMessage::handleCartoon_Play (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Cartoon_Play" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Cartoon_Play) {
        CHandleMessage::postTeacherToWhite (p, ST_Cartoon_Play);
    }

    return;
}

/*
=====================
 动画片暂停 (教师端) *
 动画片暂停 (白板端) *
=====================
*/
void CHandleMessage::handleCartoon_Pause (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Cartoon_Pause" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Cartoon_Pause) {
        CHandleMessage::postTeacherToWhite (p, ST_Cartoon_Pause);
    }

    return;
}

/*
=====================
 动画片停止 (教师端) *
 动画片停止 (白板端) *
=====================
*/
void CHandleMessage::handleCartoon_Stop (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Cartoon_Stop" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Cartoon_Stop) {
        CHandleMessage::postTeacherToWhite (p, ST_Cartoon_Stop);
    }

    return;
}

/*
=====================
 动画片环节结束 (教师端) *
 动画片结束播放 (白板端) *
=====================
*/
void CHandleMessage::handleCartoon_GameEnd (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_Cartoon_GameEnd" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_Cartoon_GameEnd) {
        CHandleMessage::postTeacherToWhite (p, ST_Cartoon_GameEnd);
    }

    return;
}
