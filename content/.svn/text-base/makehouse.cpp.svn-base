
#include "makehouse.h"

CNode::CNode(int client_fd)
{
    m_client_fd = client_fd;
    m_position_x = 0.f;
    m_position_y = 0.f;
    m_angle = 0.f;
    m_zoom = 0.f;
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

CNode::CNode(int client_fd, float x, float y, float angle, float zoom)
{
    m_client_fd = client_fd;
    m_position_x = x;
    m_position_y = y;
    m_angle = angle;
    m_zoom = zoom;
};

CNode::~CNode()
{
}

int CNode::update (int client_fd, float x, float y, float angle, float zoom)
{
    if (this->m_client_fd == client_fd) {
        m_position_x = x;
        m_position_y = y;
        m_angle = angle;
        m_zoom = zoom;
    }
    else {
        cout << "update node 'client_fd != m_client_fd' in CNode::update" << endl;
        return (1);
    }
    return (0);
}

int CMakeHouse::update (int client_fd, int node_id, float x, float y, float angle, float zoom)
{
    NODEMAP::iterator it = m_node_map.find (node_id);
    if (it != m_node_map.end()) {
        return (it->second->update (client_fd, x, y, angle, zoom));
    }

    return (1);
}

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

void CNode::set_name(string name) {
    m_name = name;
}

void CNode::get_location(float& x, float& y) {
    x = m_position_x;
    y = m_position_y;
}
void CNode::get_location(float& x, float& y, float& angle, float& zoom)
{
    x = m_position_x;
    y = m_position_y;
    angle = m_angle;
    zoom = m_zoom;
}

CMakeHouse::CMakeHouse()
{
}

CMakeHouse::~CMakeHouse()
{
}

bool CMakeHouse::lock(int client_fd, int node_id)
{
    return true;
}

bool CMakeHouse::unlock(int client_fd, int node_id)
{
    return true;
}

int CMakeHouse::add(int node_id, CNode* p_node)
{
    if (p_node == NULL)
        return (1);

    m_node_map.insert(pair<int, CNode*>(node_id, p_node));
    return 0;
}

int CMakeHouse::del(int node_id)
{
    NODEMAP::iterator it;
    it = m_node_map.find (node_id);
    if (it != m_node_map.end()) {
        delete it->second;
        m_node_map.erase(node_id);
        return (0);
    }
    return (1);
}

std::list <CNode*>::iterator CMakeHouse::get_iterator_by_node_id(int node_id)
{
    std::list <CNode*>::iterator iter;
    for( iter = m_node_layer_list.begin();
         iter != m_node_layer_list.end(); iter++ ) {
        if ( (*iter)->get_node_id() == node_id) {
            return iter;
        }
    }
    return m_node_layer_list.end();
}

int CMakeHouse::layer_up(int node_id, int count)
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

int CMakeHouse::layer_down(int node_id, int count)
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

////////// Class CGroup //////////
CGroup::CGroup (string name) : m_group_name(name)
{
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
        cout << "ERROR: add_student_to_group() " << endl;
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
#if 1
    it = m_student_map.find (fd);
    if (it != m_student_map.end())
    {
        return it->second;
    }
#else
    for (it = m_student_map.begin(); it != m_student_map.end(); it++)
    {
        if (it->first == fd)
            return it->second;
    }
#endif
    return NULL;
}

void CGroup::broadcast(Buf* p)
{
    STUDENTMAP::iterator it;
    for (it=m_student_map.begin(); it!=m_student_map.end();++it) {
        Buf* p_buf = SINGLE->bufpool.malloc();
        *p_buf = *p;
        SINGLE->sendqueue.enqueue(p_buf);
    }
}

void CGroup::sendToOtherStudent (Buf* p, enum CommandType eType)
{
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
        (void) memcpy (pp, p, ((MSG_HEAD *)p)->cLen);
        ((MSG_HEAD*)p)->cType = eType;
        pp->setfd (it->first);
        SINGLE->sendqueue.enqueue (pp);
    }

    p->reset ();
    SINGLE->bufpool.free (p);

    return;
}
