
/*
 * HandleSprite.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"


/////////////////////

/*
=====================
 串场动画 (教师端) *
 显示串场动画 (白板端) *
=====================
*/
void CHandleMessage::handleShowSpriteAnimation (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_ShowSpriteAnimation" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_ShowSpriteAnimation) {
        CHandleMessage::postTeacherToWhite (p, ST_ShowSpriteAnimation);
    }

    return;
#endif
}
    
/*
=====================
 控场动画 (教师端) *
 控场动画显示 (白板端) *
=====================
*/
void CHandleMessage::handleShowSpriteControl (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_ShowSpriteControl" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_ShowSpriteControl) {
        CHandleMessage::postTeacherToWhite (p, ST_ShowSpriteControl);
    }

    return;
#endif
}

/*
=====================
 控场级别 (教师端)
 显示控场动画级别 (白板端)
=====================
*/
void CHandleMessage::handleControlLevel (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_ControlLevel" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();
    cout<<"level : " <<*(int*)head->cData() <<endl;

    if (head->cType == CT_ControlLevel) {
        CHandleMessage::postTeacherToWhite (p, ST_ControlLevel);
    }

    return;
#endif
}
