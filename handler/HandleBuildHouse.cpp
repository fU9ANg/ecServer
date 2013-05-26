
/*
 * HandleBuildHouse.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "makehouse.h"
#include "roommanager.h"
#include "protocol.h"
#include <time.h>

/*
   =====================
   彩虹小屋文字 (教师端)
   显示彩虹小屋文字 (白板端)
   =====================
   */

void CHandleMessage::handleBuildHouse_CaiHongXiaoWu (Buf* p)
{
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

	cout << "process: CT_BuildHouse_OperatorDemo" << endl;
	MSG_HEAD* head = (MSG_HEAD*)p->ptr();

	if (head->cType == CT_BuildHouse_OperatorDemo) {
		//CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_OperatorDemo);
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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
    MSG_HEAD* head = NULL;
    CRoom* room = NULL;

	cout << "process: CT_BuildHouse_GameStart (one student one group)" << endl;

    if ((head = (MSG_HEAD*) p->ptr()) == NULL)
    {
        cout << "message header is NULL" << endl;
        SINGLE->bufpool.free (p);
        return;
    }
#if 1 // only test
    {
        (void) room;

        if (*(int*) (((char*)p->ptr()) + MSG_HEAD_LEN) == 0)
        {
            cout << "[BUILDHOUSE]: student connection" << endl;
            CStudent* student = new CStudent;
            student->setId (3);
            test_group.add_student_to_group (p->getfd(), student);
        }

        else if (*(int*) (((char*)p->ptr()) + MSG_HEAD_LEN) == 1)
        {
            cout << "[BUILDHOUSE]: whiteboard connection" << endl;
            test_white_fd = p->getfd();
            // send student count to whiteboard
            head = (MSG_HEAD*) p->ptr();
            head->cLen = MSG_HEAD_LEN + sizeof (int);
            head->cType = ST_GetStudentCount;
            *(int*) (((char*) p->ptr()) + MSG_HEAD_LEN) = 35;
            p->setsize (head->cLen);
            SINGLE->sendqueue.enqueue (p);
        }
    }
#else
    if ((room = ROOMMANAGER->get_room_by_fd (p->getfd ())) == NULL)
    {
        cout << "not found room by fd=" << p->getfd () << endl;
        SINGLE->bufpool.free (p);
        return;
    }

    room->build_house_start ();
#endif
}

#if 0
void CHandleMessage::handleBuildHouse_GameStart (Buf* p)
{
	if (p == NULL)
	{
		return;
	}

	cout << "process: CT_BuildHouse_GameStart" << endl;
	MSG_HEAD* head = (MSG_HEAD*) p->ptr();
	CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd ());
	if (p_room == NULL) {
		SINGLE->bufpool.free (p);
		return;
	}

if (p_room->m_is_show == 0)
    {

	//造房子开始
    if ((p_room != NULL) && (p_room->get_teacher_fd () == p->getfd ()))
    {
        cout << "initialize ...... build house   =======================" << endl;
	    p_room->build_house_start ();

        // sync message to whiteboard that dispaly data
	    if (head->cType == CT_BuildHouse_GameStart) {
		    //CHandleMessage::postTeacherToWhite (p, ST_BuildHouse_GameStart);
        }
	}
    else
    {
        cout << "[WARNING] -- fd is not teacher_fd for BuildHouse" << endl;
        return;
    }
    }
#if 0 // for test
else if (p_room->m_is_show == 1)
    {

    // only for testing (set the ID of student to 0)
	if (NULL == p)
	{
		return;
	}

	cout << "process: CT_BuildHouse_GameStart" << endl;
	CStudent* student = new CStudent;
    student->setId (0);
	test_group.add_student_to_group(p->getfd(), student);

    }
#endif
	return;
}
#endif

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
	if (NULL == p)
	{
		return;
	}

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
#if 0
	CRoom* room = ROOMMANAGER->get_room_by_fd (p->getfd());

    if (room != NULL)
    {
        CGroup* group = room->get_group_by_fd (p->getfd());
        CStudent* student = group->get_student_by_fd (p->getfd());
        student->isBuildHouseFinished = 1;
    }
#endif
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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

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
	if (NULL == p)
	{
		return;
	}

	cout << "process: CT_BuildHouse_Stamp" << endl;
	MSG_HEAD* head = (MSG_HEAD*)p->ptr();
    *(int*) ((char*) p->ptr() + MSG_HEAD_LEN) = 1;

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
	if (NULL == p)
	{
		return;
	}

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
#if 1
void CHandleMessage::handleBuildHouse_Update (Buf* p)
{
	cout << "CT_BuildHouse_Update" << endl;

#if 0   // Build House
	CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
	if (NULL == p_room)
	{
		return;
	}

	CGroup* p_group = p_room->get_group_by_fd (p->getfd());
	if (NULL == p_group)
	{
	    return;
	}
    p_group->save_data (p);
    p_group->sendToWhite (p, ST_BuildHouse_Update, p_room->get_white_fd());
#else
    //test_group.save_data (p);
    int studentId = *(int*)(((char*)p->ptr()) + MSG_HEAD_LEN + sizeof (int));
    cout << "studentId:::::::::::::::::: " << studentId << endl;
    if (studentId <= 35)
    {
        (void) memcpy ((char*) &test_group.get_make_house()->longlong_data[studentId][0], (char*)p->ptr(), 1024);
        test_group.sendToWhite (p, ST_BuildHouse_Update, test_white_fd);
    }
#endif
	/*
	 *         TMake_House_Update* t_update = (TMake_House_Update*) (((char*)p->ptr()) + MSG_HEAD_LEN);
	 * 
	 *         CMakeHouse* p_make_house = p_group->get_make_house();
	 *         if (p_make_house == NULL)
	 *                 return;
	 * 
	 *         if (p_make_house->update (p->getfd(), t_update->node_id, \
	 *                                 t_update->x, t_update->y, t_update->angle, t_update->zoom) != 0)
	 *         {
	 *                 cout << "CT_BuildHouse_Update Failure!" << endl;
	 *                 return ;
	 *         }
	 */

	// 同步同一组所有图片。
	//p_group->set_buf (p);
	//p_group->sendToOtherStudent (p, ST_BuildHouse_Update);
}
#endif

// 移动房屋。
void CHandleMessage::handleBuildHouse_Move (Buf* p)
{
	if (NULL == p)
	{
		return;
	}

	CMakeHouse *p_make_house = NULL; 
	movePic *p_move_pic = (movePic *)((char *)p->ptr() + MSG_HEAD_LEN);
	if (NULL == p_move_pic)
	{
		return;
	}
    
#ifdef DEBUG
	// 打印结构体。
	cout << "Move : ";
	cout << p_move_pic->layer << ", ";
	cout << p_move_pic->toX << ", ";
	cout << p_move_pic->toY << endl;
#endif

	CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
	if (NULL == p_room)
	{
		return;
	}

if (p_room->m_is_show == 0)
    {

	CGroup* p_group = p_room->get_group_by_fd (p->getfd());
	if (NULL == p_group)
	{
		return;
	}

	p_make_house = p_group->get_make_house();
	p_make_house->modifyMove(p_make_house->get_node_id_by_layer(p_move_pic->layer), p_move_pic->toX, p_move_pic->toY);

	// 同步同一组所有图片。
	p_group->set_buf (p);
	p_group->sendToOtherStudent (p, ST_BuildHouse_Update);
    }
else if (p_room->m_is_show == 1)
    {
	p_make_house = test_group.get_make_house();
	p_make_house->modifyMove(p_make_house->get_node_id_by_layer(p_move_pic->layer), p_move_pic->toX, p_move_pic->toY);

	test_group.set_buf(p);
	test_group.sendToOtherStudent(p, ST_BuildHouse_Update);

    }
}

// 缩放房屋。
void CHandleMessage::handleBuildHouse_Scale (Buf* p)
{
#if 0//_BUILD_HOUSE_GMAE
	if (NULL == p)
	{
		return;
	}

	CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
	if (NULL == p_room)
	{
		return;
	}

	CGroup* p_group = p_room->get_group_by_fd (p->getfd());
	if (NULL == p_group)
	{
		return;
	}

	scalePic *p_scale_pic = (scalePic *)((char *)p->ptr() + MSG_HEAD_LEN);
	if (NULL == p_scale_pic)
	{
		return;
	}

#ifdef DEBUG
	// 打印结构体。
	cout << "Scale : ";
	cout << p_scale_pic->layer << ", ";
	cout << p_scale_pic->toScale << endl;
#endif

	CMakeHouse *p_make_house = p_group->get_make_house();
	p_make_house->modifyScale(p_make_house->get_node_id_by_layer(p_scale_pic->layer), p_scale_pic->toScale);

	// 同步同一组所有图片。
	p_group->set_buf(p);
	p_group->sendToOtherStudent (p, ST_BuildHouse_Scale);
#else
	scalePic *p_scale_pic = (scalePic *)((char *)p->ptr() + MSG_HEAD_LEN);

	CMakeHouse *p_make_house = test_group.get_make_house();
	p_make_house->modifyScale(p_make_house->get_node_id_by_layer(p_scale_pic->layer), p_scale_pic->toScale);

	test_group.set_buf(p);
	test_group.sendToOtherStudent(p, ST_BuildHouse_Update);
#endif
}

// 旋转房屋。
void CHandleMessage::handleBuildHouse_Angle (Buf* p)
{
#if 0//_BUILD_HOUSE_GMAE
	if (NULL == p)
	{
		return;
	}

	CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
	if (NULL == p_room)
	{
		return;
	}

	CGroup* p_group = p_room->get_group_by_fd (p->getfd());
	if (NULL == p_group)
	{
		return;
	}

	rotatePic *p_rotate_pic = (rotatePic *)((char *)p->ptr() + MSG_HEAD_LEN);
	if (NULL == p_rotate_pic)
	{
		return;
	}

#ifdef DEBUG
	// 打印结构体。
	cout << "Angle : ";
	cout << p_rotate_pic->layer << ", ";
	cout << p_rotate_pic->toAngle << endl;
#endif

	CMakeHouse *p_make_house = p_group->get_make_house();
	p_make_house->modifyAngle(p_make_house->get_node_id_by_layer(p_rotate_pic->layer), p_rotate_pic->toAngle);

	// 同步同一组所有图片。
	p_group->set_buf(p);
	p_group->sendToOtherStudent (p, ST_BuildHouse_Angle);
#else
	rotatePic *p_rotate_pic = (rotatePic *) ((char *)p->ptr() + MSG_HEAD_LEN);

	CMakeHouse *p_make_house = test_group.get_make_house();
	p_make_house->modifyAngle(p_make_house->get_node_id_by_layer(p_rotate_pic->layer), p_rotate_pic->toAngle);

	test_group.set_buf(p);
	test_group.sendToOtherStudent(p, ST_BuildHouse_Update);
#endif
}

// 调整层次
void CHandleMessage::handleBuildHouse_Change_Layer(Buf* p)
{
#if 0//_BUILD_HOUSE_GMAE
	if (NULL == p)
	{
		return;
	}

	CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
	if ( NULL == p_room)
	{
		return;
	}

	CGroup* p_group = p_room->get_group_by_fd(p->getfd());
	if ( NULL == p_group)
	{
		return;
	}

/*
 *         TMake_House_Change_Layer* t_change_layer = 
 *                 (TMake_House_Change_Layer*) (((char*)p->ptr()) + MSG_HEAD_LEN);
 * 
 *         if (t_change_layer < 0)
 *         {
 *                 p_group->get_make_house()->layer_up(t_change_layer->node_id, t_change_layer->layer);
 *         }
 *         else if (t_change_layer > 0)
 *         {
 *                 p_group->get_make_house()->layer_down(t_change_layer->node_id, t_change_layer->layer);
 *         }
 *         else
 *         {
 *                 cout << "change layer = 0" << endl;
 *                 return;
 *         }
 * 
 *         p_group->sendToOtherStudent (p, ST_BuildHouse_Change_Layer);
 */

	putLayerPic *p_putLayerPic = (putLayerPic *)((char *)p->ptr() + MSG_HEAD_LEN);
	if (NULL == p_putLayerPic)
	{
		return;
	}

#ifdef DEBUG
	// 打印结构体。
	cout << "Change_layer : ";
	cout << p_putLayerPic->layer << ", ";
	cout << p_putLayerPic->toLayer << endl;
#endif

	// 图片 layer 所对应的 id 
	CMakeHouse *p_make_house = p_group->get_make_house();
	int id = p_make_house->get_node_id_by_layer(p_putLayerPic->layer);
	int count = p_putLayerPic->layer - p_putLayerPic->toLayer;

	if (count > 0)
	{
		p_make_house->layer_down(id, count);
	}
	else if (count < 0)
	{
		p_make_house->layer_up(id, count);
	}
	else // 同层。
	{
		return;
	}

	// 同步同一组所有图片。
	p_group->set_buf(p);
	p_group->sendToOtherStudent(p, ST_BuildHouse_Change_Layer);
#else
	putLayerPic *p_putLayerPic = (putLayerPic *)((char *)p->ptr() + MSG_HEAD_LEN);

	CMakeHouse *p_make_house = test_group.get_make_house();
	int id = p_make_house->get_node_id_by_layer(p_putLayerPic->layer);
	int count = p_putLayerPic->layer - p_putLayerPic->toLayer;

	if (count > 0)
	{
		p_make_house->layer_down (id, count);
	}
	else if (count < 0)
	{
		p_make_house->layer_up (id, count);
	}
	else // 同层。
	{
		return;
	}

	test_group.set_buf (p);
	test_group.sendToOtherStudent (p, ST_BuildHouse_Update);
#endif
}

// 添加一张图片
void CHandleMessage::handleBuildHouse_Add_Pic (Buf* p)
{
	cout << "CT_BuildHouse_Add_Pic" << endl;

    CNode* p_node = NULL;

	if (NULL == p)
	{
		return;
	}

	AddPic *p_add_pic = (AddPic *) (((char *)p->ptr ())+ MSG_HEAD_LEN);
	if (NULL == p_add_pic)
	{
        printf ("[ERROR] -- (AddPic): message packet is wrong\n");
		return;
	}

#ifdef DEBUG
	// 打印结构体。
	cout << "Add_Pic : ";
	cout << p_add_pic->studentId << ", ";
	cout << p_add_pic->picName << ", ";
	cout << p_add_pic->x << ", ";
	cout << p_add_pic->y << ", ";
	cout << endl;
#endif

	CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd ());
	if (NULL == p_room)
	{
        printf ("[ERROR] -- not found room by fd (%d)\n", p->getfd ());
		return;
	}

// 示范
if (p_room->m_is_show == 0)
    {

	CGroup* p_group = p_room->get_group_by_fd (p->getfd ());
	if (NULL == p_group)
	{
        printf ("[ERROR] -- not found group by fd (%d)\n", p->getfd ());
		return;
	}

    //CStudent* stu = get_student_by_fd (p->getfd());
    //stu->setId (p_add_pic->studentId);

	if ((p_node = new CNode (p->getfd (), p_add_pic->x, p_add_pic->y)) == NULL)
	{
        printf ("[ERROR] -- new CNode\n");
		return;
	}

    p_node->set_angle (0.0f);
    p_node->set_scale (1.0f);
	p_node->set_node_id (p_group->getAutoNodeId());
    p_node->set_layer (p_group->get_make_house()->get_current_layer ());
	p_node->set_sname(p_add_pic->picName);
    cout << "ADDPIC: student=" << p_group->get_student_by_fd(p->getfd())->getId() << endl;
    p_node->set_student_id (p_group->get_student_by_fd (p->getfd())->getId ());
    p_group->get_make_house()->set_current_layer (\
            p_group->get_make_house()->get_current_layer() + 1);
    cout << "NODEID=" << p_node->get_node_id() << endl;
	p_group->get_make_house()->add(p_node->get_node_id(), p_node);


	// 同步同一组所有图片。
	p_group->set_buf(p);
    cout << "herereerererere" << endl;
	p_group->sendToOtherStudent(p, ST_BuildHouse_Update);

	//delete p_node;
	//p_node = NULL;
    }
else if (p_room->m_is_show == 1)
    {

	if ((p_node = new CNode (p->getfd (), p_add_pic->x, p_add_pic->y)) == NULL)
	{
        printf ("[ERROR] -- new CNode\n");
		return;
	}
    
    p_node->set_angle (0.0f);
    p_node->set_scale (1.0f);
	p_node->set_node_id (test_group.getAutoNodeId ()); //???????????????????????
    p_node->set_layer (test_group.get_make_house ()->get_current_layer ());
	p_node->set_sname (p_add_pic->picName);
    p_node->set_student_id (p_add_pic->studentId);

    test_group.get_make_house()->set_current_layer (\
            test_group.get_make_house()->get_current_layer () + 1);

    cout << "NODEID=" << p_node->get_node_id() << endl;
	test_group.get_make_house()->add (p_node->get_node_id(), p_node);

	test_group.set_buf (p);
	test_group.sendToOtherStudent (p, ST_BuildHouse_Update);

	//delete p_node;
	//p_node = NULL;
    }
}

// 删除一张图片
void CHandleMessage::handleBuildHouse_Del_Pic (Buf* p)
{
	cout << "CT_BuildHouse_Del_Pic" << endl;

	CMakeHouse *p_make_house = NULL; 

	if (NULL == p)
	{
		return;
	}
    
	deletePic *p_del_pic = (deletePic *)((char *)p->ptr() + MSG_HEAD_LEN);
	if (NULL == p_del_pic)
	{
		return;
	}

#ifdef DEBUG
	// 打印结构体。
	cout << "layer : ";
	cout << p_del_pic->layer << endl;
#endif

	CRoom* p_room = ROOMMANAGER->get_room_by_fd(p->getfd());
	if ( NULL == p_room)
	{
		return;
	}

if (p_room->m_is_show == 0)
    {

	CGroup* p_group = p_room->get_group_by_fd(p->getfd());
	if ( NULL == p_group)
	{
		return;
	}

	p_make_house = p_group->get_make_house();
	if (0 != p_make_house->del (p_make_house->get_node_id_by_layer (p_del_pic->layer)))
	{
		cout << "CT_BuildHouse_Del_Pic Failure!" << endl;
		return;
	}

	// 同步同一组所有图片。
	p_group->set_buf(p);
    if (p->size() == 12)
        *(int *) (((char*) p->ptr()) + MSG_HEAD_LEN) = 1000;
	p_group->sendToOtherStudent(p, CT_BuildHouse_Update);

    }
else if (p_room->m_is_show == 1)
    {

	p_make_house = test_group.get_make_house ();
	if (0 != p_make_house->del (p_make_house->get_node_id_by_layer (p_del_pic->layer)))
	{
		cout << "[WARNING:] -- CT_BuildHouse_Del_Pic Failure!" << endl;
		return;
	}

	test_group.set_buf (p);
    if (p->size() == 12)
        *(int *) (((char*) p->ptr()) + MSG_HEAD_LEN) = 1000;

	test_group.sendToOtherStudent (p, ST_BuildHouse_Update);
    }
}
