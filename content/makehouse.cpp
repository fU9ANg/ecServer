
#include "makehouse.h"
#include "DataTool.h"

CNode::CNode(int client_fd)
{
    m_client_fd = client_fd;
    m_position_x = 0.f;
    m_position_y = 0.f;
    m_angle = 0.f;
    m_scale = 0.f;
};

CNode::CNode(int client_fd, float x, float y)
{
    m_client_fd = client_fd;
    m_position_x = x;
    m_position_y = y;
};

CNode::CNode(int client_fd, float x, float y, float angle)
{
    m_client_fd = client_fd;
    m_position_x = x;
    m_position_y = y;
    m_angle = angle;
};

CNode::CNode(int client_fd, float x, float y, float angle, float scale)
{
    m_client_fd = client_fd;
    m_position_x = x;
    m_position_y = y;
    m_angle = angle;
    m_scale = scale;
};

CNode::~CNode()
{
}

#ifdef _OLD_MAKEHOUSE_GAME 
int CNode::update (int client_fd, float x, float y, float angle, float scale)
{
    if (this->m_client_fd == client_fd)
    {
        m_position_x = x;
        m_position_y = y;
        m_angle = angle;
        m_scale = scale;
    }
    else
    {
        cout << "update node 'client_fd != m_client_fd' in CNode::update" << endl;
        return (1);
    }
    return (0);
}
#else
int  CNode::modifyScale (float scale)
{
    this->set_scale (scale);

    return (0);
}

int  CNode::modifyAngle (float angle)
{
    this->set_angle (angle);
    return (0);
}

int  CNode::modifyMove (float x, float y)
{
    m_position_x = x;
    m_position_y = y;
    return (0);
}

#endif

bool CNode::lock(int client_fd) {
    if ( 0 == m_client_fd) {
        m_client_fd = client_fd;
        return true;
    }
    return false;
}

bool CNode::unlock(int client_fd) {
    if ( 0 != m_client_fd) {
        m_client_fd = 0;
        return true;
    }
    return false;
}
int CNode::get_node_id() {
    return m_node_id;
}

void CNode::set_node_id(int node_id) {
    m_node_id = node_id;
}

void CNode::set_name(string name)
{
    m_name = name;
}

string CNode::get_name (void)
{
    return (m_name);
}

void CNode::set_sname (short name) {
    m_sname = name;
}

short CNode::get_sname (void)
{
    return (m_sname);
}

void CNode::get_location (float& x, float& y)
{
    x = m_position_x;
    y = m_position_y;
}

void CNode::get_location (float& x, float& y, float& angle, float& scale)
{
    x = m_position_x;
    y = m_position_y;
    angle = m_angle;
    scale = m_scale;
}

int CNode::get_layer (void)
{
    if (m_layer < 1)
    {
        cout << "error: in get_layer()" << endl;
    }
    return (m_layer);
}

float CNode::get_scale (void)
{
    return (m_scale);
}

float CNode::get_angle (void)
{
    return (m_angle);
}

void  CNode::set_layer (int layer)
{
    if (layer < 1)
    {
        cout << "error: in set_layer()" << endl;
    }
    m_layer = layer;
}

void  CNode::set_scale (float scale)
{
    if (scale < 0.0f)
    {
        cout << "error: in set_scale()" << endl;
    }
    m_scale = scale;
}

void  CNode::set_angle (float angle)
{
    if (angle < 0.0f)
    {
        cout << "error: in set_angle()" << endl;
    }
    m_angle= angle;
}

int   CNode::get_fd (void)
{
    return (m_client_fd);
}

void  CNode::set_fd (int fd)
{
    if (fd > 0)
        m_client_fd = fd;
}

int   CNode::get_student_id (void)
{
    return (m_student_id);
}

void  CNode::set_student_id (int sid)
{
    m_student_id = sid;
}

///////////////////////////// CMakeHouse /////////////////////////////////
//
CMakeHouse::CMakeHouse()
{
    // TODO:
    m_current_layer = 1;
}

CMakeHouse::~CMakeHouse()
{
    // TODO:
}

int  CMakeHouse::get_node_id_by_layer (int layer)
{
    NODEMAP::iterator it;
    for (it = m_node_map.begin(); it != m_node_map.end(); it++)
    {
        if (it->second->get_layer() == layer)
            return (it->first);
    }

    cout << "[Warning]: can't found node_id by layer =" << layer << endl;
    return (ERRORNODEID);
}

int  CMakeHouse::get_layer_by_node_id (int node_id)
{
    NODEMAP::iterator it;
    for (it = m_node_map.begin(); it != m_node_map.end(); it++)
    {
        if (it->first == node_id)
            return (it->second->get_layer());
    }

    cout << "[Warning]: can't found layer by node_id =" << node_id << endl;
    return (ERRORLAYER);
}

#ifdef _OLD_MAKEHOUSE_GAME

int CMakeHouse::update (int client_fd, int node_id, float x, float y, float angle, float scale)
{
    NODEMAP::iterator it = m_node_map.find (node_id);
    if (it != m_node_map.end()) {
        return (it->second->update (client_fd, x, y, angle, scale));
    }

    return (1);
}

#else

int  CMakeHouse::modifyScale (int node_id, float scale)
{
    MutexLockGuard guard (m_node_lock); // scale picture

    NODEMAP::iterator it = m_node_map.find (node_id);
    if (it != m_node_map.end ())
        return (it->second->modifyScale (scale));

    return (1);
}

int  CMakeHouse::modifyAngle (int node_id, float angle)
{
    MutexLockGuard gurad (m_node_lock); // rotate picture

    NODEMAP::iterator it = m_node_map.find (node_id);
    if (it != m_node_map.end ())
        return (it->second->modifyAngle (angle));

    return (1);
}

int  CMakeHouse::modifyMove (int node_id, float x, float y)
{
    MutexLockGuard guard (m_node_lock); // move picture

    NODEMAP::iterator it = m_node_map.find (node_id);
    if (it != m_node_map.end ())
        return (it->second->modifyMove (x, y));
    return (1);
}

#endif

bool CMakeHouse::lock (int client_fd, int node_id)
{
    return true;
}

bool CMakeHouse::unlock (int client_fd, int node_id)
{
    return true;
}

int CMakeHouse::add (int node_id, CNode* p_node)
{
    MutexLockGuard guard (m_node_lock); // add picture to node map

    if (p_node == NULL)
        return (1);

    cout << "nodeid=" << node_id << endl;
    m_node_layer_list.push_back (p_node);
#if 1
    std::map<int, CNode*>::iterator it;
    for (it = m_node_map.begin(); it != m_node_map.end(); it++)
    {
        cout << "-- node_id = " << it->first << endl;
        cout << "-- node = " << it->second << endl;
        cout << "---- layer = " << it->second->get_layer() << endl;
    }
#endif
    m_node_map.insert (pair<int, CNode*>(node_id, p_node));
    return 0;
}

int CMakeHouse::del (int node_id)
{
    MutexLockGuard guard (m_node_lock); // delete picture from node map

    NODEMAP::iterator it;
    it = m_node_map.find (node_id);

    if (it != m_node_map.end())
    {
        delete it->second;
        m_node_map.erase(node_id);
        return (0);
    }
    return (1);
}

int CMakeHouse::cleanall (void)
{
    //MutexLockGuard gurad (m_node_lock); // clean all picture node on the map

    NODEMAP::iterator it;
    for (it = m_node_map.begin(); it != m_node_map.end();)
    {
        delete it->second;
        m_node_map.erase (it++);
    }

    if (m_node_map.empty())
        return (0);
    else
    {
        cout << "[Fail]: clean all picture node" << endl;
        return (1);
    }

    return (0);
}

std::list <CNode*>::iterator CMakeHouse::get_iterator_by_node_id (int node_id)
{
    std::list <CNode*>::iterator iter;
    for (iter  = m_node_layer_list.begin();
         iter != m_node_layer_list.end();
         iter++)
    {
        if ( (*iter)->get_node_id() == node_id) {
            return iter;
        }
    }
    return m_node_layer_list.end();
}

int CMakeHouse::layer_up (int node_id, int count)
{
    MutexLockGuard guard(this->m_node_layer_list_lock);
    NODELIST::iterator iter = get_iterator_by_node_id(node_id);
    if (iter == m_node_layer_list.end()) {
        return -1;
    }
    int i = 0;
    for (i = 0; i < count; i++) {
        if (iter != m_node_layer_list.begin()){
            std::swap(*iter, *(iter--));
        } else{
            break;
        }
    }
    return i;
}

int CMakeHouse::layer_down (int node_id, int count)
{
    MutexLockGuard guard(this->m_node_layer_list_lock);
    NODELIST::iterator iter = get_iterator_by_node_id(node_id);
    if (iter == m_node_layer_list.end()) {
        return -1;
    }
    int i = 0;
    for (i = 0; i < count; i++) {
        if (iter != m_node_layer_list.end()){
            std::swap(*iter, *(iter++));
        } else{
            break;
        }
    }
    return i;
}

int CMakeHouse::set_current_layer (unsigned int layer)
{
    if (layer < 1)
    {
        cout << "[ERROR]: layer < 1" << endl;
        return (1);
    }
    m_current_layer = layer;
    return (0);
}

unsigned int CMakeHouse::get_current_layer (void)
{
    return (m_current_layer);
}

////////// Class CGroup //////////

CGroup::CGroup ()
{
    m_auto_nodeid = 1;
}

CGroup::CGroup (string name) : m_group_name(name)
{
    m_auto_nodeid = 1;
}

CGroup::~CGroup (void)
{
    //
}

CMakeHouse* CGroup::get_make_house (void)
{
    return &m_make_house;
}

bool CGroup::add_student_to_group (int fd, CStudent* stu)
{
    if (fd > 0 && stu)
    {
        m_student_map.insert (std::pair<int, CStudent*> (fd, stu));
        return true;
    }
    else
    {
        cout << "[ERROR]: add_student_to_group() " << endl;
    }

    return false;
}

CGroup* CGroup::get_group_by_fd (int fd)
{
    STUDENTMAP::iterator it;
    for (it=m_student_map.begin(); it!=m_student_map.end();++it)
    {
        if (it->first == fd)
            return this;
    }

    return NULL;
}

CStudent* CGroup::get_student_by_fd (int fd)
{
    STUDENTMAP::iterator it;

    it = m_student_map.find (fd);
    if (it != m_student_map.end())
    {
        return it->second;
    }

    return NULL;
}

int  CGroup::set_buf (Buf* p)
{
    MutexLockGuard guard (m_lock);
    if (p == NULL)
    {
        cout << "[ERROR]: p is NULL" << endl;
        return (1);
    }

    std::map<int, CNode*>::iterator it; // node_id, CNode
    long long ll;
    struct Picture pic;
    char* pdata = ((char*) p->ptr()) + MSG_HEAD_LEN;
    MSG_HEAD* head = (MSG_HEAD*) ((char*)p->ptr());
    int icount = 0;
    *(int*) pdata = icount;
    pdata += sizeof (int);

    for (it = m_make_house.m_node_map.begin (); it != m_make_house.m_node_map.end (); it++)
    {
        //cout << "+++++++++++++++++++" << endl;
        (void) memset (&pic, 0x00, sizeof (pic));
        it->second->get_location (pic.x, pic.y, pic.angle, pic.scale);
        pic.layer = it->second->get_layer ();
        pic.name = it->second->get_sname ();
        //pic.studentId = get_student_by_fd (p->getfd())->getId ();
        pic.studentId = it->second->get_student_id();
#if 1
        cout << "StudentId" << pic.studentId << endl;
        cout << "x=" << pic.x << endl;
        cout << "y=" << pic.y << endl;
        cout << "angle=" << pic.angle << endl;
        cout << "scale=" << pic.scale << endl;
        cout << "layer=" << pic.layer << endl;
        cout << "name=" << pic.name << endl;
#endif
        DataTool dt (pic);
        //cout << "into getData()" << time(NULL) << endl;
        ll = dt.getData ();
        //cout << "out getData()" << time(NULL) << endl;

        cout << "ll = " << ll << endl;

        *(long long*) pdata = ll;
        pdata += sizeof (long long);

        icount++;

    }

    *(int *) (((char*) p->ptr()) + MSG_HEAD_LEN) = icount;
    head->cLen = MSG_HEAD_LEN + sizeof (int) + sizeof (long long) * icount;
    p->setsize (head->cLen);
    return (0);   
}

int CGroup::save_data (Buf* p)
{
    cout << "save_data ----------------------- " << endl;
    std::map<int, CNode*>::iterator it;
    CStudent* stu = NULL;
    MSG_HEAD* head = NULL;
    char* pp = NULL;
    int   idx = 0;
    int count, student_id;

    head = (MSG_HEAD*) ((char*)p->ptr());
    (void) head;

    pp = ((char*) p->ptr()) + MSG_HEAD_LEN;

    count = *(int*) pp;
    student_id = *(int*) (pp + sizeof (int));
    (void) student_id; 

    cout << "studentid = " << student_id << ", count = " << count << endl;
    if ((stu = get_student_by_fd (p->getfd())) == NULL)
    {
        cout << "[save_data]: not found student on the STUDENTMAP" <<  endl;
        return (1);
    }

    //reset
    resetAutoNodeId (0);
    resetAutoLayer (0);
    if (m_make_house.cleanall() != 0)
    {
        cout << "[save_data]: can't clean all picture node" << endl;
        return (1);
    }

    DataTool* dt = NULL;
    long long*ll = NULL;
    long long ldata;
    struct Picture pic;
    CNode* p_node = NULL;
    pp += sizeof (int) + sizeof (int);
    // add node to map
    for (idx = 0; idx < count; idx++)
    {
        ll = (long long*) pp;
        ldata = *ll;
        cout << "process: DataTool" << endl;
        dt = new DataTool (ldata);
        pic = dt->getPicture();
        cout << "process: DataTool   --- endl" << endl;
#if 1
        cout << "[PICTURE NODE] idx=" << idx+1 << endl;
        cout << "sname=" << pic.name << ", x=" << pic.x << ", y=" << pic.y << ", angle=" << pic.angle << ", scale=" << pic.scale << ", layer=" << pic.layer << endl;
#endif
	    if ((p_node = new CNode (p->getfd (), pic.x, pic.y)) == NULL)
	    {
            printf ("[ERROR] -- new CNode\n");
		    return (1);
	    }

        p_node->set_angle (pic.angle);
        p_node->set_scale (pic.scale);
	    p_node->set_node_id (getAutoNodeId());
        p_node->set_layer (pic.layer);
	    p_node->set_sname (pic.name);
        cout << "save_data: student_id=" << get_student_by_fd(p->getfd())->getId() << endl;
        p_node->set_student_id (get_student_by_fd (p->getfd())->getId ());
        cout << "NODEID=" << p_node->get_node_id() << endl;
        m_make_house.add (p_node->get_node_id(), p_node);

        // clean temp object
        delete dt;
        pp += sizeof (long long);
    }
    cout << "---------------------------------- save_data" << endl;
    return (0);
}

void CGroup::broadcast(Buf* p)
{
    MutexLockGuard guard (m_lock);

    if (p == NULL)
    {
        cout << "[Warning] - p is NULL" << endl;
        return;
    }

    STUDENTMAP::iterator it;
    Buf* p_buf = NULL;
    MSG_HEAD* head = NULL;
    char* pp = NULL;

    for (it=m_student_map.begin(); it!=m_student_map.end(); ++it)
    {
        if ((p_buf = SINGLE->bufpool.malloc()) == NULL)
        {
            cout << "[Error] - out of memory" << endl;
            return;
        }
        head = (MSG_HEAD*)((char*) p_buf->ptr());
        pp = ((char*)p_buf->ptr()) + MSG_HEAD_LEN;

        head->cType = \
        ((MSG_HEAD*) ((char*) p->ptr()))->cType;

        head->cLen  = \
        ((MSG_HEAD*) ((char*) p->ptr()))->cLen;
        
        p_buf->setfd (p->getfd());
        p_buf->setsize (head->cLen);

        (void) memcpy (pp, ((char*)p->ptr()) + MSG_HEAD_LEN, head->cLen);
        SINGLE->sendqueue.enqueue (p_buf);
    }

    SINGLE->bufpool.free (p);
}

void CGroup::sendToWhite (Buf* p, enum CommandType eType, int w_fd)
{
    MutexLockGuard guard (m_lock);

    if (p == NULL)
    {
        cout << "[sendToWhite] -- p is NULL" << endl;
        return;
    }

    MSG_HEAD* head = NULL;
    head = (MSG_HEAD*) ((char*) p->ptr());
    head->cType = eType;
    p->setfd (w_fd);
    p->setsize (head->cLen);
    cout << "BH: white fd=" << w_fd <<endl;
    cout << "head->cLen = " << head->cLen << endl;
    cout << "head->cType = " << head->cType << endl;

    SINGLE->sendqueue.enqueue (p);
}

void CGroup::sendToOtherStudent (Buf* p, enum CommandType eType)
{
    MutexLockGuard guard (m_lock);
    if (p == NULL)
    {
        cout << "[Warning] - p is NULL" << endl;
        return;
    }

    STUDENTMAP::iterator it;
    Buf* p_buf = NULL;
    MSG_HEAD* head = NULL;
    char* pp = NULL;

    for (it=m_student_map.begin(); it!=m_student_map.end(); ++it) {

        cout << "student fd = " << it->first;

        //if (p->getfd() == it->first) continue;

        if ((p_buf = SINGLE->bufpool.malloc()) == NULL)
        {
            cout << "[Error] - out of memory" << endl;
            return;
        }
        head = (MSG_HEAD*)((char*) p_buf->ptr());
        pp = ((char*)p_buf->ptr()) + MSG_HEAD_LEN;

        head->cType = eType;

        head->cLen  = \
        ((MSG_HEAD*) ((char*) p->ptr()))->cLen;
        
        p_buf->setfd (it->first);
        p_buf->setsize (head->cLen);
        cout << "head->cLen = " << head->cLen << endl;
        (void) memcpy (pp, ((char*)p->ptr()) + MSG_HEAD_LEN, head->cLen);
        SINGLE->sendqueue.enqueue (p_buf);
    }

    SINGLE->bufpool.free (p);

    return;
#if 0
    STUDENTMAP::iterator it;
    for (it = m_student_map.begin(); it != m_student_map.end (); it++) {
        if (p->getfd() == it->first) {
            continue;
        }

        Buf* pp = SINGLE->bufpool.malloc ();
        if (pp == NULL) {
            cout << "out of memory" << endl;
            return;
        }
        (void) memcpy (pp->ptr(), p->ptr(), ((MSG_HEAD *)p->ptr())->cLen);
        ((MSG_HEAD*)p->ptr())->cType = eType;
        pp->setfd (it->first);
        SINGLE->sendqueue.enqueue (pp);
    }

    //p->reset ();
    SINGLE->bufpool.free (p);

    return;
#endif
}

void CGroup::setName (string name)
{
    m_group_name = name;
}

string CGroup::getName (void)
{
    return (m_group_name);
}

unsigned int CGroup::getAutoNodeId ()
{
    MutexLockGuard guard (m_lock);

    return (m_auto_nodeid++);
}

unsigned int CGroup::getAutoLayer ()
{
    MutexLockGuard guard (m_lock);

    return (m_auto_layer++);
}

void CGroup::resetAutoNodeId (int value)
{
    MutexLockGuard guard (m_lock);
    m_auto_nodeid = value;
}

void CGroup::resetAutoLayer (int value)
{
    MutexLockGuard gurad (m_lock);
    m_auto_layer = value;
}
