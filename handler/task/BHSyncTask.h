
/**
 * @addtogroup framework
 * @{
 */

#ifndef BHSYNCTASK_H_
#define BHSYNCTASK_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "task.h"
#include "Sock.h"
#include "Single.h"
#include "Buf.h"
#include "HandleMessage.h"
#include "protocol.h"

/**
 * @brief 处理线程
 */
class BHSyncTask: public task
{
    public:
        BHSyncTask();
        ~BHSyncTask();
        virtual int work();
};

#endif

///@}
