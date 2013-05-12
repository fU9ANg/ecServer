
#include "BHSyncTask.h"
#include "makehouse.h"
#include "roommanager.h"
#include "room.h"
#include "HandleMessage.h"

BHSyncTask::BHSyncTask ()
{
    m_oldtime = time (NULL);
}

BHSyncTask::~BHSyncTask ()
{
}

int BHSyncTask::work ()
{
    time_t now;
    CRoom* room = NULL;
    Buf* p = NULL;
    Buf* p_white = NULL;
    MSG_HEAD* head = NULL;
    int type, index = 1;

    while (1)
    {
        now = time (NULL);
        if (now == m_oldtime)
        {
            if (m_fps < FPSVALUE)
            {
#if 0
                //cout << "FPS: " << m_fps << endl;
                if ((p = SINGLE->bufpool.malloc ()) == NULL)
                {
                    cout <<"[WARNING] - out of memory" << endl;
                    sleep (1);
                    continue;
                }
                p->setsize (0);
                CHandleMessage::test_group.set_buf (p);
                //cout << "size=" << p->size() << endl;
                if (p->size() > 12)
                {
                    CHandleMessage::test_group.sendToOtherStudent (p, ST_BuildHouse_Update);
                    cout << "senddddddddddddddddddddddddddddddddddd.........." << endl;
                }
                else
                {
                    SINGLE->bufpool.free (p);
                }
#else
                map<int, CGroup*>::iterator it;

                // now, only sync message in the same room 
                if ((room = ROOMMANAGER->get_room (1)) == NULL)
                //if ((room = ROOMMANAGER->get_room_by_name ("A教室")) == NULL)
                {
                    cout << "[ERROR]: not found room by id =1" << endl;
                    return 1;
                }

                for (it = room->m_buildhouse_groups.begin(); it != room->m_buildhouse_groups.end(); it++)
                {
                    if ((p = SINGLE->bufpool.malloc ()) == NULL)
                    {
                        cout << "[WARNING]: out of memory" << endl;
                        sleep (1);
                        continue;
                    }

                    p->setsize (0);
                    it->second->set_buf (p);
                    if (p->size() > 12)
                    {
                        if (m_fps == 1)
                        {
                            if ((p_white = SINGLE->bufpool.malloc ()) != NULL)
                            {
                                (void) memcpy ((char*) p_white->ptr(), (char*) p->ptr(), p->size());
                                p_white->setsize (p->size());
                                head = (MSG_HEAD*) ((char*) p_white->ptr());
                                head->cLen = p->size();
                                type = 30000 + index++;
                                (void) memcpy ((char*) &head->cType, &type, sizeof (unsigned int));
                                p_white->setfd (room->get_white_fd());

                                SINGLE->sendqueue.enqueue (p_white);

                                cout << "whitefffffddd = " << room->get_white_fd () <<endl;
                                //it->second->sendToWhite (p_white, ST_BuildHouse_Update, room->get_white_fd());
                            }
                        }
                        it->second->sendToOtherStudent (p, ST_BuildHouse_Update);
                    }
                    else
                    {
                        SINGLE->bufpool.free (p);
                    }
                }
#endif
                m_fps++;
                usleep (1000000/FPSVALUE);
            }
            //else usleep (1000);
        }
        else 
        {
            index = 0;
            type  = 0;
            m_fps = 0;
            m_oldtime = now;
        }
    }

    (void) room;
    (void) p;
    (void) p_white;
    (void) head;
    (void) type;
    (void) index;
#if 0
    if ((room = ROOMMANAGER->get_room (1)) == NULL)
    {
        cout << "not found room by room_id=" << 1 << endl;
        return (1);
    }

    //room->

    if ((p = SINGLE->bufpool.malloc ()) == NULL)
    {
        cout << "out of memory" << endl;
        return (1);
    }

    CHandleMessage::test_group.set_buf (p);

    CHandleMessage::test_group.sendToOtherStudent (p, ST_BuildHouse_Update);
#endif
    return 0;
}
