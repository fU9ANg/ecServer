
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

#define ERRORNODEID     -1
#define ERRORLAYER      -1

class CNode
{
    public:
        CNode(int client_fd);
        CNode(int client_fd, float x, float y);
        CNode(int client_fd, float x, float y, float angle);
        CNode(int client_fd, float x, float y, float angle, float scale);
        ~CNode();
#ifdef _OLD_MAKEHOUSE_GAME 
        int  update (int client_fd, float x, float y, float angle, float scale);
#else
        int  modifyScale (float scale);
        int  modifyAngle (float angle);
        int  modifyMove (float x, float y);
#endif
        bool lock(int client_fd);
        bool unlock(int client_fd);
        int  get_node_id();
        void set_node_id(int node_id);

        void set_sname(short name);
        short get_sname (void);
        void set_name(string name);
        string get_name (void);

        void get_location(float& x, float& y);
        void get_location(float& x, float& y, float& angle, float& scale);

        int  get_layer (void);
        float get_scale (void);
        float get_angle (void);

        void  set_layer (int);
        void  set_scale (float);
        void  set_angle (float);

        int   get_fd (void);
        void  set_fd (int);

        int   get_student_id (void);
        void  set_student_id (int);

        friend class CHandleMessage;    //fck code;
    private:
        float m_position_x; /**x*/
        float m_position_y;
        int   m_layer; //层次
        int   m_client_fd;  // client_fd
        int   m_node_id;
        float m_angle; //角度
        float m_scale; //放大倍数
        char   m_path[512];
        string m_name;
        short m_sname;
        int  m_student_id;
};

class CMakeHouse
{
    friend class CGroup;
    public:
        typedef std::map<int, CNode*> NODEMAP; // node_id, CNode
        typedef std::list<CNode*> NODELIST;

        int set_current_layer(unsigned int);
        unsigned int get_current_layer (void);

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
        int  cleanall (void);
#ifdef _OLD_MAKEHOUSE_GAME 
        int  update (int clientfd, int node_id, float x, float y, float angle, float scale);
#else
        int  modifyScale (int node_id, float scale);
        int  modifyAngle (int node_id, float angle);
        int  modifyMove (int node_id, float x, float y);
#endif

        int  get_node_id_by_layer (int layer);
        int  get_layer_by_node_id (int node_id);

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

    private:
        unsigned int m_current_layer;
        MutexLock m_node_lock;
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
        CGroup ();
        ~CGroup ();
        CMakeHouse* get_make_house (void);
        bool add_student_to_group (int fd, CStudent* stu);
        CGroup* get_group_by_fd (int fd);
        CStudent* get_student_by_fd (int fd);

        int  set_buf (Buf* p);
        int  save_data (Buf* p);    // save current picture information
        void broadcast(Buf* p);
        void sendToOtherStudent (Buf* p, enum CommandType eType);
        void sendToWhite (Buf* p, enum CommandType eType, int w_fd);
        void sendToStudentAndWhite (Buf* p, enum CommandType eType); // send data to student and white

        unsigned int getAutoNodeId ();
        unsigned int getAutoLayer ();

        void resetAutoNodeId (int value);
        void resetAutoLayer (int value);
    public:
        void setName (string name);
        string getName (void);

        unsigned int get_current_layer ();
        void set_current_layer (unsigned int layer);

    private:
        MutexLock m_lock;
        unsigned int m_auto_nodeid;
        unsigned int m_auto_layer;
        unsigned int m_current_layer;
};

#endif
