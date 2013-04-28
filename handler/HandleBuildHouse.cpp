
/*
 * HandleBuildHouse.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "makehouse.h"
#include "roommanager.h"

/*
=====================
 彩虹小屋文字 (教师端)
 显示彩虹小屋文字 (白板端)
=====================
*/
void CHandleMessage::handleBuildHouse_CaiHongXiaoWu (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_CaiHongXiaoWu" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_CaiHongXiaoWu) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_CaiHongXiaoWu);
        cout << "content: " << ((sBuildHouseCaiHongXiaoWu*)(((char*)p->ptr()) + MSG_HEAD_LEN))->sText << endl;
    }

    return;
}

/*
=====================
 彩虹谷背景 (教师端) *
 显示彩虹谷背景 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_CaiHongGu (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_CaiHongGu" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_CaiHongGu) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_CaiHongGu);
    }

    return;
}

/*
=====================
 小怪物图片 (教师端) *
 显示小怪物图片 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_XiaoGuaiWu (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_XiaoGuaiWu" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_XiaoGuaiWu) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_XiaoGuaiWu);
    }

    return;
}

/*
=====================
 幕布 (教师端) *
 显示幕布 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_MuBu (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_MuBu" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_MuBu) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_MuBu);
    }

    return;
}


/*
=====================
 操作演示 (教师端) *
 响应教师的操作演示 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_OperatorDemo (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_OperatorDemo" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_OperatorDemo) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_OperatorDemo);
    }

    return;
}

/*
=====================
 单人模式 (教师端) *
 响应所有学生的单人模式操作情况 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_SingleMode (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_SingleMode" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_SingleMode) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_SingleMode);
    }

    return;
}

/*
=====================
 分组模式 (教师端) *
 响应分组模式操作情况 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_GroupMode (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_GroupMode" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_GroupMode) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_GroupMode);
    }

    return;
}


/*
=====================
 设置学生组信息数量 (教师端)
=====================
*/
void CHandleMessage::handleBuildHouse_SetStuGroupCount (Buf* p)
{
    //todo:
}

/*
=====================
 设置学生组信息 (教师端)
=====================
*/
void CHandleMessage::handleBuildHouse_SetStuGroup (Buf* p)
{
    //todo:
}

/*
=====================
 获得学生组信息数量 (所有客户端)
=====================
*/
void CHandleMessage::handleBuildHouse_GetStuGroupCount (Buf* p)
{
    //todo:
}

/*
=====================
 获得学生组信息 (所有客户端)
=====================
*/
void CHandleMessage::handleBuildHouse_GetStuGroup (Buf* p)
{
    //todo:
}

/*
=====================
 造房子游戏开始 (教师端) *
 同步所有(学生端和白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_GameStart (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_GameStart" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();
    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if (NULL == p_room) {
        SINGLE->bufpool.free(p);
        return;
    }

    //造房子开始
    p_room->build_house_start();

    if (head->cType == CT_BuildHouse_GameStart) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_GameStart);
    }

    return;
}

/*
=====================
 学生添加/移动/删除素材 (所有学生端) **
 同步造房子进度 (白板端) **
=====================
*/
void CHandleMessage::handleBuildHouse_UpdatePos (Buf* p)
{
    //todo:
}

/*
=====================
 发起造房子游戏结束 (教师端) *
 同步所有(学生端和白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_GameEnd (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_GameEnd" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_GameEnd) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_GameEnd);
    }

    return;
#endif
}

/*
=====================
 发送创建好的房子到服务器 (所有学生端) ***
 发送创建好的房子到 (白板端)
=====================
*/
void CHandleMessage::handleBuildHouse_Save (Buf* p)
{
    //todo:
}

/*
=====================
 启动邮件1 (教师端) *
 显示邮件1信息 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_EmailTibet (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_EmailTibet" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_EmailTibet) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_EmailTibet);
    }

    return;
#endif
}

/*
=====================
 启动邮件2 (教师端) *
 显示邮件2信息 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_EmailAmerica (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_EmailAmerica" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_EmailAmerica) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_EmailAmerica);
    }

    return;
#endif
}


/*
=====================
 激活 (教师端) *
 激活场景 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_Activation (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_Activation" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_Activation) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_Activation);
    }

    return;
#endif
}

/*
=====================
 角色登场 (教师端) *
 角色登场 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_Role (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_Role" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_Role) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_Role);
    }

    return;
#endif
}

/*
=====================
 盖章 (教师端) *
 盖章 (白板端) *
=====================
*/
void CHandleMessage::handleBuildHouse_Stamp (Buf* p)
{
#ifdef _EXECUTE_
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_Stamp" << endl;
    MSG_HEAD* head = (MSG_HEAD*)p->ptr();

    if (head->cType == CT_BuildHouse_Stamp) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_Stamp);
    }

    return;
#endif
}

/*
 */
void CHandleMessage::handleBuildHouse_ScreenShot (Buf* p)
{
    if (p == NULL)
        return;

    cout << "process: CT_BuildHouse_ScreenShot" << endl;
    MSG_HEAD* head = (MSG_HEAD*) p->ptr();

    if (head->cType == CT_BuildHouse_ScreenShot) {
        CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_Screenshot);
    }
    return;
}

/*
=====================
 盖章确认 (教师端) ***
=====================
*/
void CHandleMessage::handleBuildHouse_SaveHouse (Buf* p)
{
    //todo:
}

/*
=====================
 更新操作 (包括 移动,旋转,缩放)
=====================
*/
void CHandleMessage::handleBuildHouse_Update (Buf* p)
{
    cout << "CT_BuildHouse_Update" << endl;

    CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
    if (p_room == NULL)
        return;
    CGroup* p_group = p_room->get_group_by_fd (p->getfd());
    if (p_group == NULL)
        return;

    TMake_House_Update* t_update = (TMake_House_Update*) (((char*)p->ptr()) + MSG_HEAD_LEN);

    CMakeHouse* p_make_house = p_group->get_make_house();
    if (p_make_house == NULL)
        return;

    if (p_make_house->update (p->getfd(), t_update->node_id, \
            t_update->x, t_update->y, t_update->angle, t_update->zoom) != 0)
    {
        cout << "CT_BuildHouse_Update Failure!" << endl;
        return ;
    }

    p_group->sendToOtherStudent (p, ST_BuildHouse_Update);
}

/*
=====================
 更新层次
=====================
*/
void CHandleMessage::handleBuildHouse_Change_Layer(Buf* p)
{
    cout << "CT_BuildHouse_Change_Layer" << endl;

    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if ( NULL == p_room) {
        return;
    }
    CGroup* p_group = p_room->get_group_by_fd(p->getfd());
    if ( NULL == p_group) {
        return;
    }

    TMake_House_Change_Layer* t_change_layer = \
           (TMake_House_Change_Layer*) (((char*)p->ptr()) + MSG_HEAD_LEN);
    if (t_change_layer < 0)
        p_group->get_make_house()->layer_up(t_change_layer->node_id, t_change_layer->layer);
    else if (t_change_layer > 0)
        p_group->get_make_house()->layer_down(t_change_layer->node_id, t_change_layer->layer);
    else {
        cout << "change layer = 0" << endl;
        return;
    }

    p_group->sendToOtherStudent (p, ST_BuildHouse_Change_Layer);
}

/*
=====================
 添加一张图片
=====================
*/
void CHandleMessage::handleBuildHouse_Add_Pic(Buf* p)
{
    cout << "CT_BuildHouse_Add_Pic" << endl;
    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if ( NULL == p_room) {
        return;
    }

    CGroup* p_group = p_room->get_group_by_fd(p->getfd());
    if ( NULL == p_group) {
        return;
    }

    TMake_House_Add_Pic* p_add_pic = (TMake_House_Add_Pic*)((char*)p->ptr() + MSG_HEAD_LEN); 
    CNode* p_node = new CNode(p->getfd());

    unsigned int node_id = p_room->getAutoNodeId();
    p_add_pic->node_id = node_id;

    p_node->set_node_id(p_add_pic->node_id);
    p_node->set_name(p_add_pic->picture_name);

    p_group->get_make_house()->add(p_node->get_node_id(), p_node);

    p_group->sendToOtherStudent (p, ST_BuildHouse_Add_Pic);
}

/*
=====================
 删除一张图片
=====================
*/
void CHandleMessage::handleBuildHouse_Del_Pic(Buf* p)
{
    cout << "CT_BuildHouse_Del_Pic" << endl;
    CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
    if ( NULL == p_room) {
        return;
    }
    CGroup* p_group = p_room->get_group_by_fd(p->getfd());
    if ( NULL == p_group) {
        return;
    }

    TMake_House_Del_Pic* p_del_pic = (TMake_House_Del_Pic*) ((char*)p->ptr() + MSG_HEAD_LEN);

    if (p_group->get_make_house()->del(p_del_pic->node_id) != 0) {
        cout << "CT_BuildHouse_Del_Pic Failure!" << endl;
        return ;
    }

    p_group->sendToOtherStudent (p, ST_BuildHouse_Del_Pic);
}
