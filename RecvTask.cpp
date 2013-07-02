
#include "RecvTask.h"
#include "LoginCheck.h"
#include "protos/proto.pb.h"

RecvTask::RecvTask ()
{
}

RecvTask::~RecvTask ()
{
}

int RecvTask::work ()
{
    while (true)
    {
        Buf* p = NULL;
        if (SINGLE->recvqueue.dequeue (p, 3) != 0)
        {
            continue;
        }
#if 0
        string temp_str((char*)p->ptr());

        PKT_DBRecordFinished db_recordfinished;
        db_recordfinished.ParseFromString (temp_str);

        cout << db_recordfinished.iflagfinished() << endl;
#endif
        if (p == NULL)
        {
            cout << "ERROR: p==NULL in RecvTask::work()" << endl;
            return (0);
        }

        MSG_HEAD* head = (MSG_HEAD*) p->ptr();
        if (head != NULL)
        {
            debugProtocol (p);
            cout << "recvTask: cType=" << head->cType << endl;
            handlefunc pfun = CHandleMessage::getHandler (head->cType); 
            if (NULL == pfun)
            {
                printf ("ERROR: the pointer of callback, cType=[%d]\n", head->cType);
                continue;
            }
            pfun (p);
        }
    }
    return 0;
}
