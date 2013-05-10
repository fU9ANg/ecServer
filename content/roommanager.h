
/**
 * @ingroup content
 * @{
 */

#ifndef _C_ROOMMANAGER_H
#define _C_ROOMMANAGER_H

#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>
#include <iostream>

#include "room.h"

using namespace std;

/**
 * @class 教室管理类
 */
class CRoomManager
{
	//typedef list<CRoom*> CLASSLIST;
    typedef map<int, CRoom*> ROOMMAP;

public:
	static CRoomManager* getInstance (void);
	~CRoomManager () {};

    int init();
    int get_room_count();
    CRoom* get_room(int id);

    CRoom* get_room_by_fd (int fd);
    CRoom* get_room_by_name (string name);
    void del_client(int fd);

private:
	CRoomManager () {}
    /**
     * @brief 教室map  教室id，教室指针 
     */
    ROOMMAP m_room_map;
	static CRoomManager* m_CRoomManager;
};

#define ROOMMANAGER CRoomManager::getInstance()

#endif //_C_CLASSMANAGER_H
///@}
