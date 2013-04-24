/*
 * room.h
 */

#ifndef _C_ROOM_H
#define _C_ROOM_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "teacher.h"
#include "student.h"
#include "grade.h"
#include "database.h"
#include "SQL.h"
#include "game.h"

#include "course.h"
#include "makehouse.h"

#include "HandleMessage.h"
#include "puzzle.h"

#define NUMS_STU_OF_GROUP   5

typedef enum SceneType
{
    ST_SCENE1 = 1,
    ST_SCENE2 = 2,
    ST_SCENE3 = 3,
    ST_SCENE4 = 4,
} SceneType;

typedef enum SceneState
{
    SS_START = 1,
    SS_PAUSE = 2,
    SS_END   = 3,
} SceneState;

class CRoom
{
	typedef map<int, CStudent*> STUDENTMAP;
    typedef list<CCourse*> COURSELIST;
    typedef list<CGame*> GAMELIST;
    typedef map<int, CGroup*>  GROUPMAP;  /* map<group_id, CGroup *> */
    typedef map<int, CPuzzle*> PUZZLEMAP; /* map<group_id, CPuzzle*> */

private:
    GROUPMAP m_buildhouse_groups;
    PUZZLEMAP m_puzzle_map;

public:
    CGroup* get_group_by_fd (int fd);
    bool add_group (int id, CGroup* cg);

    void build_house_start();
    void build_house_end();

    void puzzle_start();
    void puzzle_end();
    void end_puzzle_by_fd(int fd);

public:
	 CRoom(int id, string class_name, string white_board);
	~CRoom ();

    friend class CHandleMessage;
    friend class LoginCheck;

    int  get_room_id();
    void set_teacher_fd(int fd);
    void set_teacher_name(string name);
    string get_teacher_name ();
    void set_class_name(string name);
    string get_class_name ();
    void set_white_fd(int fd);
    void add_student(int fd, CStudent* pstudent);
    void del_student(int fd);
    void del_client(int fd);
    string get_room_name();

    CStudent* get_student_by_fd (int fd);
    CTeacher* get_teacher_by_fd (int fd);
    CCourse * get_course_by_name (string course_name);
    CCourse * get_course_by_id (int id);

    void add_course (CCourse *);
    void clear_all_course ();

    int  get_teacher_fd ();
    int  get_white_fd ();

    CRoom* get_room_by_fd (int fd);

    int reset();
    void teacher_disconnect();

    SceneType  get_current_st (void);
    SceneState get_current_ss (void);
    void set_current_st (SceneType);
    void set_current_ss (SceneState);

    void update_all_student_status (int fd, int flag); // offline = 0, leaveearly = 1
    void init_buildhouse_group (int numsOfStudent);

    unsigned int getAutoNodeId ();

private:
    CTeacher m_teacher;
    int m_room_id;
    int m_teacher_fd;
    int m_white_fd;
    string m_room_name;
    string m_class_name;
    string m_teacher_name;
    string m_white_board;
    STUDENTMAP m_student_map;
    COURSELIST m_course_list;
    GAMELIST   m_game_list;

    SceneType   m_current_st;
    SceneState  m_current_ss;

    unsigned int m_node_id;
};

#endif //_C_ROOM_H
