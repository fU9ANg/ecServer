#include "SendTask.h"

int SendTask::work() {
    while(true){
        Buf* p = NULL;
        if (0 != SINGLE->sendqueue.dequeue(p, 3)) {
            continue;
        }

        int fd = p->getfd();

        if (0 == fd) {
            printf(" fd = 0 !\n");
            SINGLE->bufpool.free(p);
            continue;
        }

        //
        cout << "Send cType: " << ((MSG_HEAD*)p->ptr())->cType <<endl;
        printf ("address: %p\n", p);

        if (((MSG_HEAD*)p->ptr())->cType == 187) {
        cout << "-----------------------BEGIN-------------------------------------" << endl;
        cout << "CTYPE: " << ((MSG_HEAD*) (p->ptr()))->cType << endl;
        cout << "LEN: " << *(unsigned int*) ((char*)p->ptr() + MSG_HEAD_LEN) << endl;
        cout << "STUDENT_ID: " << *(unsigned int*) ((char*)p->ptr() + MSG_HEAD_LEN + sizeof (unsigned int)) << endl;
        cout << "STATUS: " << *(unsigned int*) ((char*)p->ptr() + MSG_HEAD_LEN + sizeof (int) + sizeof (int)) << endl;
        cout << "STUDENT_ID: " << *(unsigned int*) ((char*)p->ptr() + MSG_HEAD_LEN + sizeof (int) + sizeof (int) + sizeof (int)) << endl;
        cout << "STATUS: " << *(unsigned int*) ((char*)p->ptr() + MSG_HEAD_LEN + sizeof (int) + sizeof (int)+ sizeof (int) + sizeof (int)) << endl;

        cout << "-------------------------END-----------------------------------" << endl;
        }

        // send fck message to clients
        // must be need written bytes data finished
        int bytes_left = p->size ();
        int written_bytes;
        char* ptr = (char*) p->ptr();
        while (bytes_left > 0)
        {
            written_bytes = send (fd, ptr, bytes_left, 0);
            if (written_bytes <= 0) {
                if (errno == EINTR) {
                    if (written_bytes < 0) {
                        written_bytes = 0;
                        continue;
                    }
                }
                else if (errno == EAGAIN) {
                    if (written_bytes < 0) {
                        written_bytes = 0;
                        usleep (50);
                        continue;
                    }
                }
                else {
                    break;
                }
            }
            bytes_left -= written_bytes;
            ptr += written_bytes;
        }

        ///printf("Send data...finished. packetLength=%ld, from FD=[%d]\n", p->size(), fd);
        LOG(INFO) << "Send data ... finished. packet len=" << p->size() << ", from FD=" << fd << endl;

        p->reset();
        SINGLE->bufpool.free(p);
    }
    return 0;
}

SendTask::SendTask() 
{
}

SendTask::~SendTask() 
{
}
