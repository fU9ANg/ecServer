
#ifndef _MAKEHOUSE_H_
#define _MAKEHOUSE_H_

#include <map>
#include <stdio.h>
#include <iostream>
#include "student.h"
#include "Mutex.h"
#include "Buf.h"
#include "Single.h"

using namespace std;

class CNode
{
    public:
        CNode(int client_fd);
        CNode(int client_fd, float x, float y);
        CNode(int client_fd, float x, float y, float angle);
        CNode(int client_fd, float x, float y, float angle, float zoom);
        ~CNode();
        int  update (int client_fd, float x, float y, float angle, float zoom);
        bool lock(int client_fd);
        bool unlock(int client_fd);
        int  get_node_id();
        void set_node_id(int node_id);
        void set_name(string name);
        void get_location(float& x, float& y);
        void get_location(float& x, float& y, float& angle, float& zoom);
        friend class CHandleMessage;    //fck code;
    private:
        float m_position_x; /**x*/
        float m_position_y;
        int m_layer; //层次
        int m_client_fd;  // client_fd
        int m_node_id;
        float m_angle; //角度
        float m_zoom; //放大倍数
        char   m_path[512];
        string m_name;
};

class CMakeHouse
{
    public:
        typedef std::map<int, CNode*> NODEMAP; // node_id, CNode
        typedef std::list<CNode*> NODELIST;

    private:
        NODEMAP   m_node_map;
        NODELIST  m_node_layer_list;
        MutexLock m_node_layer_list_lock;

    public:
        CMakeHouse ();
        ~CMakeHouse ();
        bool lock (int client_fd, int node_id);
        bool unlock (int client_fd, int node_id);
        int  add (int node_id, CNode* p_node);
        int  del (int node_id);
        int  update (int clientfd, int node_id, float x, float y, float angle, float zoom);

        /**
         * @brief 将节点上移count层
         * @param  node_id   需要上调的节点id
         * @param  count     上移层数
         */
        int layer_up (int node_id, int count);
        /**
         * @brief 将节点下移count层
         * @param  node_id   需要下调的节点id
         * @param  count     下移层数
         */
        int layer_down (int node_id, int count);
        friend class CHandleMessage; // fck code
        
    private:
        NODELIST::iterator get_iterator_by_node_id(int node_id);
};

class CGroup
{
    typedef std::map <int, CStudent*> STUDENTMAP;   /* <student_fd, CStudent> */
    
    private:
        string m_group_name;
        CMakeHouse m_make_house;
        STUDENTMAP m_student_map;
        int m_white_position;

    public:
        CGroup (string name);
        ~CGroup ();
        CMakeHouse* get_make_house (void);
        bool add_student_to_group (int fd, CStudent* stu);
        CGroup* get_group_by_fd (int fd);
        CStudent* get_student_by_fd (int fd);
        void broadcast(Buf* p);
        void sendToOtherStudent (Buf* p, enum CommandType eType);       
};

#endif
