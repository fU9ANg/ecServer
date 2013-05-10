
//
// Opcodes
//

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#pragma pack(1)
enum CommandType
{
    CT_Test = 1,
    CT_ChangeScene = 100,
    ST_ChangeScene,
    CT_ControlChangeScene,
    ST_ControlChangeScene,
    CT_InitSceneFinished,       // 当白板端初始化数据后,发送此消息给服务器
    ST_InitSceneFinished,       // 服务器转发此消息给教师端


    ST_ConfirmIntoClassRoom,    // 当教师进入电子教室后,发送此消息给所有的学生端
    CT_ConfirmIntoClassRoom,    // 学生端

    CT_Login = 150,      // 登录 (所有端)
    CT_LoginResult,      // 登录结果 (所有端)
    CT_GetCourseDBCount, // 获得数据库中的课程信息数量 (教师端)
    CT_GetCourseDB,      // 获得数据库中的课程信息 (教师端)
    CT_SetCourseGroup,   // 设置开启的课程列表 (教师端)
    CT_GetCourseGroup,      // 获得课程列表 (教师端和所有学生端)
    ST_GetCourseGroup,      // 服务器回应课程列表 (教师端和所有学生端)
    CT_GetCourseItemCount,  // 获得课程列表的子项数量 (教师端)
    CT_GetCourseItem,    // 获得课程列表的子项 (教师端)
    CT_Logout,           // 登出 (所有端)
    CT_LogoutReuslt,     // 登出结果 (所有端)
    CT_GetClassRoomDBCount, //获得教室班级数据库信息数量 (教师端)
    CT_GetClassRoomDB,   // 获得教室班级数据库信息 (教师端)
    CT_GetGradeDBCount,  // 获得教室年级数据库信息数量 (教师端)
    CT_GetGradeDB,       // 获得教室年级数据库信息 (教师端)
    CT_GetClassDBCount,  // 获得班级信息数量 (教师端)
    CT_GetClassDB,       // 获得班级信息 (教师端)
    CT_LoginClassRoom,   // 进入教室  老師->服務器
    CT_LogoutClassRoom,  // 退出教室 (教师端和所有学生端)
    CT_GetStudentDetailInfo,    // 获得某一学生详细信息  所有端->服务器
    ST_GetStudentDetailInfo,    // 返回某一学生详细信息  服务器->所有端
    CT_GetAllStudentInfoCount,  // 获得所有学生列表数量  所有端->服务器
    CT_GetAllStudentInfo,       // 获得所有学生列表   所有端->服务器
    CT_LeaveEarly,              // 早退  教师端->服务器
    ST_LeaveEarly,              // 早退通知给  服务器->学生端
    CT_ConfirmLeaveEarly,       // 早退确认  学生端->服务器 *
    CT_GetTeacherInfo,          // 获得教师信息  所有学生端和教师端->服务器
    ST_GetTeacherInfo,          // 返回教师信息  服务器->所有端
    CT_LockStudent,             // 锁定学生端  教师端->服务器 *
    ST_LockStudent,             // 锁定学生端  服务器->学生端 *
    CT_SelectedClassRoom,       // 选择某一平板属于某一教室  所有端->服务器
    CT_CourseFinished,      // 请求课程是否结束 (所有端)
    ST_CourseFinished,      // 服务请求课程是否结束
    CT_GetDBRecordFinished, // 请求所有数据库记录是否完成
    ST_GetDBRecordFinished, // 服务器回应数据库记录是否完成
    CT_GetCourseItemKeyInfoReq, // 教师端请求CourseItem的按键数据请求
    CT_GetCourseItemKeyInfoRsp, // 教师端请求CourseItem的按键数据回应
    ST_SendStudentStatus,   //学生在线状态

    CT_Common_PlayorPause,
    ST_Common_PlayorPause,
    //CT_Common_Pause,
    //ST_Common_Pause,
    CT_Common_Resume,
    ST_Common_Resume,
    CT_Common_Stop,
    ST_Common_Stop,
    /////////////////////

    CT_ShowSpriteAnimation = 200,   // 串场动画 (教师端) *
    ST_ShowSpriteAnimation,         // 显示串场动画 (白板端) *
    
    CT_ShowSpriteControl = 300,     // 控场动画 (教师端) *
    ST_ShowSpriteControl,           // 控场动画显示 (白板端) *
    CT_ControlLevel,                // 控场级别 (教师端)
    ST_ControlLevel,                // 显示控场动画级别 (白板端)

    CT_Exercise_GameStart = 400,    // 暖身操环节开始 (教师端) *
    ST_Exercise_GameStart,          // 暖身操开始播放 (白板端) *
    CT_Exercise_Play,               // 暖身操播放 (教师端) *
    ST_Exercise_Play,               // 暖身操播放 (白板端) *
    CT_Exercise_Pause,              // 暖身操暂停 (教师端) *
    ST_Exercise_Pause,              // 暖身操暂停 (白板端) *
    CT_Exercise_Stop,               // 暖身操停止 (教师端) *
    ST_Exercise_Stop,               // 暖身操停止 (白板端) *
    CT_Exercise_GameEnd,            // 暖身操环节结束 (教师端) *
    ST_Exercise_GameEnd,            // 暖身操结束播放 (白板端) *

    CT_Cartoon_GameStart = 500,     // 动画片环节开始 (教师端) *
    ST_Cartoon_GameStart,           // 动画片开始 (白板端) *
    CT_Cartoon_Play,                // 动画片播放 (教师端) *
    ST_Cartoon_Play,                // 动画片播放 (白板端) *
    CT_Cartoon_Pause,               // 动画片暂停 (教师端) *
    ST_Cartoon_Pause,               // 动画片暂停 (白板端) *
    CT_Cartoon_Stop,                // 动画片停止 (教师端) *
    ST_Cartoon_Stop,                // 动画片停止 (白板端) *
    CT_Cartoon_GameEnd,             // 动画片环节结束 (教师端) *
    ST_Cartoon_GameEnd,             // 动画片结束播放 (白板端) *

    ////////////////////

    CT_BuildHouse_CaiHongXiaoWu = 600,  // 彩虹小屋文字 (教师端)
    ST_BuildHouse_CaiHongXiaoWu,        // 显示彩虹小屋文字 (白板端)
    CT_BuildHouse_CaiHongGu,            // 彩虹谷背景 (教师端) *
    ST_BuildHouse_CaiHongGu,            // 显示彩虹谷背景 (白板端) *
    CT_BuildHouse_XiaoGuaiWu,           // 小怪物图片 (教师端) *
    ST_BuildHouse_XiaoGuaiWu,           // 显示小怪物图片 (白板端) *
    CT_BuildHouse_MuBu,                 // 幕布 (教师端) *
    ST_BuildHouse_MuBu,                 // 显示幕布 (白板端) *

    CT_BuildHouse_OperatorDemo,         // 操作演示 (教师端) *
    ST_BuildHouse_OperatorDemo,         // 响应教师的操作演示 (白板端) *
    CT_BuildHouse_SingleMode,           // 单人模式 (教师端) *
    ST_BuildHouse_SingleMode,           // 响应所有学生的单人模式操作情况 (白板端) *
    CT_BuildHouse_GroupMode,            // 分组模式 (教师端) *
    ST_BuildHouse_GroupMode,            // 响应分组模式操作情况 (白板端) *

    CT_BuildHouse_SetStuGroupCount,     // 设置学生组信息数量 (教师端)
    CT_BuildHouse_SetStuGroup,          // 设置学生组信息 (教师端)
    CT_BuildHouse_GetStuGroupCount,     // 获得学生组信息数量 (所有客户端)
    CT_BuildHouse_GetStuGroup,          // 获得学生组信息 (所有客户端)
    CT_BuildHouse_GameStart,            // 造房子游戏开始 (教师端) *
    ST_BuildHouse_GameStart,            // 同步所有(学生端和白板端) *
    //CT_BuildHouse_Material,         // 获得素材 (所有学生端) **
    CT_BuildHouse_UpdatePos,            // 学生添加/移动/删除素材 (所有学生端) **
    ST_BuildHouse_UpdatePos,            // 同步造房子进度 (白板端) **
    CT_BuildHouse_GameEnd,              // 发起造房子游戏结束 (教师端) *
    ST_BuildHouse_GameEnd,              // 同步所有(学生端和白板端) *
    CT_BuildHouse_Save,                 // 发送创建好的房子到服务器 (所有学生端) ***
    ST_BuildHouse_Show,                 // 发送创建好的房子到 (白板端)
    CT_BuildHouse_EmailTibet,           // 启动邮件1 (教师端) *
    ST_BuildHouse_EmailTibet,           // 显示邮件1信息 (白板端) *
    CT_BuildHouse_EmailAmerica,         // 启动邮件2 (教师端) *
    ST_BuildHouse_EmailAmerica,         // 显示邮件2信息 (白板端) *

    CT_BuildHouse_Move,                // 更新操作 (包括 移动)
    ST_BuildHouse_Move,                // 服务器->关联的学生端
    CT_BuildHouse_Angle,               // 更新操作 (包括 旋转)
    ST_BuildHouse_Angle,               // 服务器->关联的学生端
    CT_BuildHouse_Scale,               // 更新操作 (包括 缩放)
    ST_BuildHouse_Scale,               // 服务器->关联的学生端

    CT_BuildHouse_Update,               // 更新操作 (包括 移动,旋转,缩放)
    ST_BuildHouse_Update,               // 服务器->关联的学生端
    CT_BuildHouse_Change_Layer,         // 更新层次
    ST_BuildHouse_Change_Layer,         // 服务器->关联的学生端
    CT_BuildHouse_Add_Pic,              // 添加一张图片
    ST_BuildHouse_Add_Pic,              // 服务器->关联的学生端
    CT_BuildHouse_Del_Pic,              // 删除一张图片
    ST_BuildHouse_Del_Pic,              // 服务器->关联的学生端

    CT_BuildHouse_Activation,           // 激活 (教师端) *
    ST_BuildHouse_Activation,           // 激活场景 (白板端) *
    CT_BuildHouse_Role,                 // 角色登场 (教师端) *
    ST_BuildHouse_Role,                 // 角色登场 (白板端) *
    CT_BuildHouse_Stamp,                // 盖章 (教师端) *
    ST_BuildHouse_Stamp,                // 盖章 (白板端) *
    CT_BuildHouse_SaveHouse,            // 盖章确认 (教师端) ***
    CT_BuildHouse_ScreenShot,           // 截图 (教师端)
    ST_BuildHouse_Screenshot,           // 截图 (白板端)


    //////////////////
    CT_Puzzle_GameStart = 700,  // 点击拼图开始 (教师端)
    ST_Puzzle_GameStart,        // 发送拼图游戏开始信息 (学生端和白板端)
    CT_Puzzle_GetPic,         // 发送某一拼图原图 (学生端和白板端) **
    ST_Puzzle_GetPic,         // 发送某一拼图原图 (学生端和白板端) **
    CT_Puzzle_UpdatePic,        // 学生端拼成功一块后发送
    ST_Puzzle_UpdatePic,        // 学生端拼成功一块后转发到白板端
    CT_Puzzle_IconStatus,       // 获得拼图是否正确的状态 (学生端) *
    ST_Puzzle_IconStatus,       // 发送拼图是否正确的状态 (白板端) *
    CT_Puzzle_Play,             // 拼图播放 (学生端) *
    ST_Puzzle_Play,             // 拼图播放 (白板端) *
    CT_Puzzle_Pause,            // 拼图暂停 (学生端) *
    ST_Puzzle_Pause,            // 拼图暂停 (白板端) *
    CT_Puzzle_MovePic,          // 拼图区中移动某一拼图块 (客户端) **
    ST_Puzzle_MovePic,          // 拼图区中移动某一拼图块 (白板端) **
    CT_Puzzle_DeletePic,        // 拼图区中删除某一拼图块 (客户端) **
    ST_Puzzle_DeletePic,        // 拼图区中删除某一拼图块 (白板端) **
    CT_Puzzle_AddPic,           // 拼图区中添加某一拼图块 (客户端) **
    ST_Puzzle_AddPic,           // 拼图区中添加某一拼图块 (白板端) **
    CT_Puzzle_CalcScore,        // 拼图的结果 (教师端) &*
    CT_Puzzle_EndGame,          // 结束拼图课程 (教师端) *
    ST_Puzzle_EndGame,          // 结束拼图课程 (白板端和所有学生端) *

    CT_Puzzle_SelectedType,
    ST_Puzzle_SelectedType,
    CT_Puzzle_SelectedSize,
    ST_Puzzle_SelectedSize,



    CT_Scene_End,
    ST_Scene_End,
    CT_RainbowValley_HouseNum = 800,
    ST_RainbowValley_HouseNum,
    CT_RainbowValley_HouseInfo,
    ST_RainbowValley_HouseInfo,
    ST_Dynamic_MessageType = 10000,  // 要求所发送的学生信息从10000号开始
};



// 下面是所有用到的数据结构体
//
struct sCommonStruct
{
    unsigned int iValue;
};

struct sDBRecordFinished
{
    unsigned int iFlagFinished;
};
/*
    CT_GetDBRecordFinished,
    ST_GetDBRecordFinished,
*/

struct sSceneEnd
{
    int type;
};
/*
    CT_Scene_End,
    ST_Scene_End,
*/

struct sDBCount
{
    int count;
};

enum mClientType
{
    MCT_STUDENT = 1,
    MCT_TEACHER,
    MCT_WHITEBOARD,
};

enum eTeacherIntoRoomType
{
    TT_LOGIN_CLASSROOM  = 1,
    TT_LOGOUT_CLASSROOM = 2,
};

enum eClientStatus
{
    eCS_ONLINE  = 1,
    eCS_OFFLINE = 2,
    eCS_LEAVEEARLY = 3,
    eCS_NOLOGIN = 4
};

struct sLogin
{
    int type;   // 客户端的类型(学生,教师或白板)
    char username[20];
    char password[20];
};
/*
| len | CT_Login | struct sLogin |
*/

enum mLoginOutResult
{
    RES_SUCCEED = 1,
    RES_FAIL,
};
struct sLoginOutResult
{
    int succeed;
    char message[50];
};
/*
| len | CT_LoginResult | struct sLoginOutResult |
*/

struct sLogout
{
    int type; // 客户端的类型(学生,教师或白板)
};
/*
| len | CT_Logout | struct sLogout |
*/

struct sCourseGroup
{
    char sGradeName[20];    // 服务器没有处理这个字段
    char sCourseGroupName[20];
};
/*
| len | CT_SetCourseGroup | struct sCourseGroup |
*/

struct sGetCourseGroup
{
    char sCourseList[128];   // 动画片,暖身操,xxxx,
};
/*
| len | CT_GetCourseGroup |
| len | CT_GetCourseGroup | struct sGetCourseList |
*/

struct sGetCourseDB
{
    char sGradeName[20];
    char sGroupName[20];
    char sCourseName[20];
    int  iLanguage;
    int  iArt;
    int  iCommunity;
    int  iHealth;
    int  iScience;
};
/*
+------------+-------------+----------+-----+-----------+--------+---------+------------+
| group_name | course_name | language | art | community | health | science | grade_name |
+------------+-------------+----------+-----+-----------+--------+---------+------------+
| 课件1      | 暖身操      |       73 |  56 |        85 |     45 |      62 | 中班       |
+------------+-------------+----------+-----+-----------+--------+---------+------------+
| len | CT_GetCourseDB | struct sGetCourseDB |
*/

/*
| len | CT_CourseFinished | int course_id |
| len | ST_CourseFinished | int course_id |
*/

struct sGetGradeDB
{
    char sGradeName[20];
};
/*
+----------+------------+
| grade_id | grade_name |
+----------+------------+
|        1 | 小班       |
|        2 | 中班       |
+----------+------------+
| len | CT_sGetGradeDB | struct sGetGradeDB |
*/

struct sGetClassDB
{
    char sClassName[20];
};
/*
+----------+------------+
| grade_id | class_name |
+----------+------------+
|        1 | 一班     |
|        1 | 二班     |
|        1 | 三班     |
+----------+------------+
| len | CT_GetClassDB | struct sGetClassDB |
*/

struct sGetClassRoomDB
{
    char sClassRoomName[20];
};
/*
+----------------+
| classroom_name |
+----------------+
| A教室          |
| B教室          |
| C教室          |
+----------------+
| len | CT_GetClassRoomDB | struct sGetClassRoomDB |
*/

typedef struct sSelectedClassRoom{
    unsigned int classroom_id;
    mClientType client_type;
}TSelectedClassRoom;

struct sGetCourseItem
{
    char sCourseName[20];
};

/**
 * 学生在线状态
 */

//| MSG_HEAD | len | struct1 | student2 | ... | studentx |
//unsigned int len;
typedef struct ST_SendStudentStatusReq {
    unsigned int student_id;
    unsigned int status;
}TSendStudentStatusReq;


/*
| len | CT_GetCourseItem | struct sGetCourseItem |
*/

struct sCourseItem
{
    char sCourseName[20];
    char sItemName[20];
    char sDesc[512];
    int  iId;
};
/*
SELECT c.course_name, i.item_name from course_item AS ci, course AS c, item AS i WHERE ci.course_id=c.course_id AND ci.item_id=i.item_id AND c.course_name="造房子";
+-------------+---------------+
| course_name | item_name     |
+-------------+---------------+
| 造房子      | 空白          |
| 造房子      | 串场          |
| 造房子      | 开场          |
| 造房子      | 互动环节1     |
| 造房子      | 互动环节2     |
+-------------+---------------+
| len | CT_GetCourseItem | struct sCourseItem |
*/

struct sLoginOutClassRoom
{
    int  type;
    char sTeacherName[20];
    char sClassName[20];
    char sClassRoomName[20];
};
/*
| len | CT_LoginClassRoom | struct sLoginOutClassRoom |
| len | CT_LogoutClassRoom | struct sLoginOutClassRoom |
*/

struct sStudentDetail
{
    int  iStudentId;
    char sStudentName[20];
};
/*
| len | CT_GetStudentDetailInfo | struct sStudentDetailInfo |
*/

struct sGetStudentDetailInfo
{
    char sNumber[20];
    char sFirstName[20];
    char sLastName[20];
    char sSex[10];
    char sSchoolName[20];
    char sGradeName[20];
    char sClassName[20];
    char sAccount[20];
    char birthday[20];
    char stFirstName[20];
    char stLastName[20];
    int  iPicture_id;
};
/*
SELECT s.number, s.last_name, s.first_name, s.sex, sc.school_name, g.grade_name, c.class_name, s.account, s.password, s.birthday, t.first_name, t.last_name, s.picture_id FROM student AS s, school AS sc, grade AS g, class AS c, teacher AS t WHERE t.teacher_id = s.class_teacher_id AND sc.school_id = s.school_id AND g.grade_id = s.grade_id AND c.class_id = s.class_id;
+----------+-----------+------------+------+-----------------+------------+------------+---------+------------+------------+-----------+------------+
| number   | last_name | first_name | sex  | school_name     | grade_name | class_name | account | birthday   | first_name | last_name | picture_id |
+----------+-----------+------------+------+-----------------+------------+------------+---------+------------+------------+-----------+------------+
| 10000001 | yang      | c1         | 男   | 双楠幼儿园      | 小班       | 一年级     | 1       | 2000-01-01 | 四         | 李        |       1001 |
| 10000035 | yang      | c35        | 男   | 双楠幼儿园      | 小班       | 一年级     | 35      | 2000-01-01 | 四         | 李        |       1035 |
+----------+-----------+------------+------+-----------------+------------+------------+---------+------------+------------+-----------+------------+

| len | ST_GetStudentDetailInfo | struct sGetStudentDetailInfo |
*/

struct sGetAllStudentInfo
{
    int iStudentId;
    char sPicName[128];
    char sStudentName[20];
};
/*
select s.student_id, r.path AS picture_name from student AS s, resource AS r where s.picture_id=r.resource_id;
+------------+----------------+
| student_id | picture_name   |
+------------+----------------+
|          1 | /tmp/pic/9.png |
+------------+----------------+
| len | CT_GetAllStudentInfo | struct sGetAllStudentInfo |
*/

struct sLeaveEarly // LeaveEarly / ConfirmLeaveEarly
{
    //char sStudentName[20];
    //char sTeacharName[20];
    //char sClassroomName[20];
    //char sClassName[20];
    int student_id;
};

enum ePuzzleType
{
    PT_SQUARE = 1,  // 方块
    PT_CIRCLE,      // 圆形
};
struct sPuzzleType
{
    unsigned int type;
};

struct sPuzzleSize
{
    unsigned int rowCount;
    unsigned int columnCount;
};
/*

struct sPuzzleGameStart
{
    char type;
    char size;  // 20/12
};
*/
/*
| len | CT_Puzzle_GameStart | struct sPuzzleGameStart |
| len | ST_Puzzle_GameStart | struct sPuzzleGameStart |
*/

/*
| len | CT_LeaveEarly | struct sLeaveEarly |
| len | CT_ConfirmLeaveEarly | struct sLeaveEarly |
*/

struct sGetTeacherInfo
{
    char sTeacherName[20];
    char sPicName[128];
};
/*
| len | CT_GetTeacherInfo | struct sGetTeacherInfo |
*/

struct sControlLevel
{
    int level;  // VALUE: 0 (待机), 1, 2, 3, 4 (下一节)
};
/*
| len | CT_ControlLevel | struct sControlLevel |
| len | ST_ControlLevel | struct sControlLevel |
*/

struct sBuildHouseCaiHongXiaoWu
{
    char sText[12];
};
/*
| len | CT_BuildHouse_CaiHongXiaoWu | struct sBuildHouseCaiHongXiaoWu |
| len | ST_BuildHouse_CaiHongXiaoWu | struct sBuildHouseCaiHongXiaoWu |
*/

struct sBuildHouseSetStuGroup
{
    int  iGroupIndex;
    int  iStudentCount;
};
struct sBuildHouseStudentList
{
    char sStudentName[10];
};
/*
| len | CT_BuildHouse_SetStuGroup | struct sBuildHouseSetStuGroup + struct sBuildHouseStudentList |
| len | CT_BuildHouse_GetStuGroup | struct sBuildHouseSetStuGroup + struct sBuildHouseStudentList |
*/

enum ePuzzleIconStatus
{
    PIS_OK = 1,
    PIS_ERR,
    PIS_WAIT,
};
struct sPuzzleIconStatus
{
    int status;
};
/*
| len | CT_Puzzle_IconStatus | struct sPuzzleIconStatus |
| len | ST_Puzzle_IconStatus | struct sPuzzleIconStatus |
*/

///////////////////

enum ePuzzleActionTypePic
{
    AP_MOVE = 1,
    AP_DELETE,
    AP_ADD,
};

//学生拼图正确时发送发送学生是那个拼图块拼正确了
struct sPuzzleUpdatePic
{
    int student_id;
    int pic_idx; // 某张拼图块
};

//拼图开始数据
struct sJigsawInitData
{
	char picturePath[512];
	int	stencilId;
	int	rowCount;
	int columnCount;

};

///////////////////

typedef struct sSubmitData
{
	unsigned int cLen;          //数据长度
	enum CommandType cType;     //数据编号
	void *cData() {             //指向数据的指针
		return this+1;
	}; 	
}MSG_HEAD;

typedef struct _stCT_GetCourseItemKeyInfoReq{
    //unsigned int course_item_id;
    char course_item[64];
}GetCourseItemKeyInfoReq;

typedef struct _stCT_GetCourseItemKeyInfoRsp{
    char keys[512];
}GetCourseItemKeyInfoRsp;

typedef struct Make_House_Update {
    int node_id;
    float x;
    float y;
    float zoom;
    float angle;
} TMake_House_Update;
/*
typedef struct Make_House_Move{
    int node_id;
    float to_x;
    float to_y;
}TMake_House_Move;

typedef struct Make_House_Zoom{
    int node_id;
    float zoom;
    float angle;
}TMake_House_Zoom;
*/
typedef struct Make_House_Change_Layer{
    int node_id;
    int layer;
}TMake_House_Change_Layer;

typedef struct Make_House_Add_Pic {
    int node_id;
    float x;
    float y;
    char picture_name[40];
}TMake_House_Add_Pic;

typedef struct Make_House_Del_Pic {
    int node_id;
    char picture_name[40];
}TMake_House_Del_Pic;

typedef struct sRainbowValley_House_Num{
	int num;
}THouse_Number;


//|MWG_HEAD|int len|sRainbowValley_HouseItem_Info1|sRainbowValley_HouseItem_Info2|...|
typedef struct sRainbowValley_HouseItem_Info{
	int layer;
	float  x;
	float y;
	float zoom;
	float angle;
	char picture_name[40];
}THouseItem_Info;


// make house

struct AddPic{
	int studentId;
	float x;
	float y;
	//char picName[4];
    short picName;
};//添加
struct movePic{
	int layer; //层作为图片标识符
	float toX;
	float toY;
};//移动

struct scalePic{
   int layer;
	float toScale;
};//缩放

struct rotatePic{
int layer;
float toAngle;
};//旋转

struct putLayerPic{
	int layer;
	int toLayer;
};//调整层次

struct deletePic{
	int layer;

};//删除素材

#define MSG_HEAD_LEN sizeof(struct sSubmitData)

#pragma pack()

#endif //_PROTOCOL_H
