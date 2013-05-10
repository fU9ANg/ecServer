
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

    while (1)
    {
        now = time (NULL);
        if (now == m_oldtime)
        {
            if (m_fps < FPSVALUE)
            {
#if 1
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
                CRoom* room = NULL;
                
#endif
                m_fps++;
                usleep (1000000/FPSVALUE);
            }
            //else usleep (1000);
        }
        else 
        {
            m_fps = 0;
            m_oldtime = now;
        }
    }

    (void) room;
    (void) p;
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
