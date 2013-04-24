
#include "RecvTask.h"
#include "LoginCheck.h"
RecvTask::RecvTask() {
}

RecvTask::~RecvTask() {
}

int RecvTask::work ()
{
    while(true) {
        Buf* p = NULL;
        if (0 != SINGLE->recvqueue.dequeue(p, 3)) {
            continue;
        }

        if (p == NULL) {
            cout << "ERROR: p==NULL in RecvTask::work()" << endl;
            return (0);
        }

        MSG_HEAD* head = (MSG_HEAD*) p->ptr();
        if (head != NULL)
        {
            cout << "recvTask: cType=" << head->cType << endl;
            handlefunc pfun = CHandleMessage::getHandler (head->cType); 
            if (NULL == pfun) {
                printf("ERROR: the pointer of callback, cType=[%d]\n",head->cType);
                continue;
            }
            pfun(p);
        }
    }
    return 0;
}
