
/**
 * @author  liuyuncheng
 * @version  1.0
 * @addtogroup framework
 * @{
 */

#ifndef _POOLT_H_
#define _POOLT_H_
#include <queue>
#include <iostream>
#include <time.h>
#include <pthread.h>

#include "Mutex.h"

using namespace std;

/**
 * @class   PoolT
 * @brief   memory pool
 * @  简易内存池
 */
template<typename TYPE>
class PoolT
{
public:
    /**
     * @brief   constructor
     * @param[in] n 初始化对象的个数，即池子大小
     */
    PoolT(int n = 20000) {
        this->size_ = n;
        for (int i = 0; i < this->size_; i++) {
            TYPE* p = new TYPE;
            this->queue_.push(p);
        }
    }
    /**
     * @brief   destructor
     */
    ~PoolT() {
        MutexLockGuard guard(lock_);
        while (!queue_.empty()) {
            TYPE* p = this->queue_.front();
            this->queue_.pop();
            delete p;
        }
    }

    /**
     * @brief 分配内存
     * @retval 分配好的内存对象指针
     */
    TYPE* malloc() {
        MutexLockGuard guard(lock_);
        if (queue_.empty()) {
            return NULL;
        }
        TYPE* p = queue_.front();
        queue_.pop();
        return p;
    }

    /**
     * @brief 释放内存
     */
    int free(TYPE* i) {
        MutexLockGuard guard(lock_);
        queue_.push(i);
        return 0;
    }

    /**
     * @retval  池子大小
     */
    int size() {
        return size_;
    }

    /**
     * @retval  池子大小
     */
    int used() {
      MutexLockGuard guard(lock_);
      return size_ - queue_.size();
    }

private:
    queue<TYPE*> queue_;
    MutexLock lock_;
    int size_;
};

/**
 * @}
 */
#endif
