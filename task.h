#ifndef _TASK_H_
#define _TASK_H_
#include <glog/logging.h>

#include "database.h"
#include "protocol.h"
/**
 * @class task基类
 */
class task{
    public:
        task():autorelease(true){
        }
        virtual ~task(){}
        virtual int work() = 0;
        /**任务完成后是否自动释放*/
        bool autorelease;
};
#endif
