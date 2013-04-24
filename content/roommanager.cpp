/**
 * @ingroup content
 * @{
 */
#include "roommanager.h"

CRoomManager* CRoomManager::m_CRoomManager = NULL;

CRoomManager* CRoomManager::getInstance (void)
{
	if (NULL == m_CRoomManager)
		m_CRoomManager = new CRoomManager;
	return m_CRoomManager; 

}

int CRoomManager::init() {
    try {
        PreparedStatement* pstmt = DATABASE->preStatement (SQL_SELECT_ROOM);
        ResultSet* prst = pstmt->executeQuery (); 
        while (prst->next ()) {
            CRoom* p = new CRoom(prst->getInt("classroom_id"),
                    prst->getString("classroom_name"),
                    prst->getString("white_board"));
            m_room_map.insert(pair<int, CRoom*>(p->get_room_id(), p));
        }   
        delete prst;
        delete pstmt;
    }catch (SQLException e) {
        LOG(ERROR)<<e.what()<<endl;
        return -1;
    }      
    return 0;
}

int CRoomManager::get_room_count() {
    return m_room_map.size();
}

CRoom* CRoomManager::get_room(int id) {
    ROOMMAP::iterator it = m_room_map.find(id);
    if (it != m_room_map.end()) {
        return it->second;
    }
    return NULL;
}

CRoom* CRoomManager::get_room_by_fd (int fd)
{
    CRoom* pRoom = NULL;
    ROOMMAP::iterator it;

    for (it = m_room_map.begin (); it != m_room_map.end (); it++) {
        pRoom = it->second->get_room_by_fd (fd);
        if (NULL != pRoom) {
            return pRoom;
        }
    }
    return NULL;
}

CRoom* CRoomManager::get_room_by_name (string name) {
    ROOMMAP::iterator it;

    for (it = m_room_map.begin (); it != m_room_map.end (); it++) {
        if (name == it->second->get_room_name()) {
            return it->second;
        }
    }
    return NULL;
}

void CRoomManager::del_client(int fd) {
    ROOMMAP::iterator it;
    for(it = m_room_map.begin(); it != m_room_map.end(); ++it) {
        it->second->del_client(fd);
    }
}
///@}
