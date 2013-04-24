
/*
 * HandleRainbowValley.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "roommanager.h"
#include "teacher.h"
#include "Single.h"
#include "room.h"

#include "makehouse.h"

/*
=====================
 获得房子数量
=====================
*/
void CHandleMessage::handleRainbowValley_HouseNum (Buf* p)
{
    cout << "CT_RainbowValley_HouseNum" << endl;
#if 0
    CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
    if (p_room == NULL)
        return;

    int groupSize = p_room->m_buildhouse_groups.size();
#endif
    MSG_HEAD* head = (MSG_HEAD* )(char*) p->ptr();
    head->cLen = MSG_HEAD_LEN + sizeof (sRainbowValley_House_Num);
    head->cType = ST_RainbowValley_HouseNum;
    sRainbowValley_House_Num* p_house_num = \
            (sRainbowValley_House_Num*)((char*)p->ptr() + MSG_HEAD_LEN);

    cout << "group Size of RainbowValley" << endl;
    p_house_num->num = 1;//groupSize;

    p->setsize (head->cLen);
    SINGLE->sendqueue.enqueue (p);
}

/*
=====================
 获得房子信息
=====================
//|MWG_HEAD|int len|sRainbowValley_HouseItem_Info1|sRainbowValley_HouseItem_Info2|...|
typedef struct sRainbowValley_HouseItem_Info{
	char picture_name[40];
	int layer;
	float x;
	float y;
	float zoom;
	float angle;
}THouseItem_Info;
*/

void CHandleMessage::handleTest (Buf* p)
{
    cout << "CT_RainbowValley_HouseInfo" << endl;

    CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd ());
    if (p_room == NULL)
        return;

    sRainbowValley_HouseItem_Info* info = NULL;
    MSG_HEAD* head = NULL;
    Buf* pp = NULL;
    int type = 0;
    int idex = 0;
    //GROUPMAP::iterator it;
    std::map<int, CGroup*>::iterator it;
    for (it = p_room->m_buildhouse_groups.begin(); \
            it != p_room->m_buildhouse_groups.end(); it++)
    {
        pp = SINGLE->bufpool.malloc ();
        if (pp == NULL) {
            cout << "out of memory" << endl;
            return;
        }
        pp->reset ();

        head = (MSG_HEAD*) pp->ptr();
        type = 20000 + idex++;
        memcpy (&head->cType, &type, sizeof (int));
        //head->cType = ST_RainbowValley_HouseInfo; // 20000
        head->cLen = MSG_HEAD_LEN + sizeof (int);
        *(int*)(((char*)pp->ptr()) + MSG_HEAD_LEN) = it->second->get_make_house()->m_node_map.size();
        std::map<int, CNode*>::iterator cit;
        for (cit = it->second->get_make_house()->m_node_map.begin(); cit != it->second->get_make_house()->m_node_map.end(); cit++) {
            info = (sRainbowValley_HouseItem_Info*) (((char*)pp->ptr()) + head->cLen);
            info->x = cit->second->m_position_x;
            info->y = cit->second->m_position_y;
            info->angle = cit->second->m_angle;
            info->zoom = cit->second->m_zoom;
            info->layer = cit->second->m_layer;
            strcpy (info->picture_name, cit->second->m_name.c_str());

            head->cLen += sizeof (sRainbowValley_HouseItem_Info);
        }
    }

    pp->setfd (p->getfd());
    pp->setsize (head->cLen);
    SINGLE->sendqueue.enqueue (pp);
}

void CHandleMessage::handleRainbowValley_HouseInfo (Buf* p)
{
    cout << "CT_RainbowValley_HouseInfo" << endl;
#if 0
    CRoom* p_room = ROOMMANAGER->get_room_by_fd (p->getfd());
    if (p_room == NULL)
        return;
    CGroup* p_group = p_room->get_group_by_fd (p->getfd());
    if (p_group == NULL)
        return;

    CMakeHouse* p_make_house = p_group->get_make_house();
    if (p_make_house == NULL)
        return;
#endif
    sRainbowValley_HouseItem_Info* info;
    MSG_HEAD* head;
    int icount = 0;
    Buf* pp = SINGLE->bufpool.malloc ();
    if (pp == NULL) {
        cout << "out of memory" << endl;
        return;
    }
    pp->reset ();
    head = (MSG_HEAD *)pp->ptr();
    head->cType = ST_RainbowValley_HouseInfo;
    head->cLen = MSG_HEAD_LEN + sizeof (int);

    *(int*)(((char*)pp->ptr()) + MSG_HEAD_LEN) = icount;
#if 0
    std::map<int, CNode*>::iterator it;
    for (it = p_make_house->m_node_map.begin(); it != p_make_house->m_node_map.end(); it++)
#else
    float x = 10.5f;
    float y = 20.5f;
    float angle = 30.5f;
    float zoom = 40.5f;
    int layer = 1;
    string picture_name ("caocao.png");
    for (int i = 0; i < 2; i++)
#endif
    {
        info = (sRainbowValley_HouseItem_Info*)(((char*)pp->ptr()) + head->cLen);
        info->x = 2 * (x++);//it->second->m_position_x;
        info->y = 2 * (y++);//it->second->m_position_y;
        info->angle = angle==30 ? angle * 2 : 30;//it->second->m_angle;
        info->zoom = zoom++;//it->second->m_zoom;
        info->layer = layer++;//it->second->m_layer;
        //strcpy (info->picture_name, it->second->m_name.c_str());
        strcpy (info->picture_name, picture_name.c_str());

        head->cLen += sizeof (sRainbowValley_HouseItem_Info);
        icount++;
    }
    // update count of struct sRainbowValley_HouseItem_Info
    *(int*) ((char*)pp->ptr() + MSG_HEAD_LEN) = 2;//icount;

#if 1
    char *ptest = ((char*) pp->ptr()) + MSG_HEAD_LEN + sizeof (int);
    //for (int idx=0; idx < icount; idx++) {
    for (int idx=0; idx < 2; idx++) {
        cout << "NODE" << idx << ": " << endl;
        cout << "picture_name: " << ((sRainbowValley_HouseItem_Info*)ptest)->picture_name << endl;
        cout << "layer: " << ((sRainbowValley_HouseItem_Info*)ptest)->layer << endl;
        cout << "position_x: " << ((sRainbowValley_HouseItem_Info*)ptest)->x << endl;
        cout << "position_y: " << ((sRainbowValley_HouseItem_Info*)ptest)->y << endl;
        cout << "zoom: " << ((sRainbowValley_HouseItem_Info*)ptest)->zoom << endl;
        cout << "angle: " << ((sRainbowValley_HouseItem_Info*)ptest)->angle << endl;
        ptest += sizeof (sRainbowValley_HouseItem_Info);
    }
#endif

    pp->setfd (p->getfd());
    pp->setsize (head->cLen);
    SINGLE->sendqueue.enqueue (pp);
}
