/**
 * @file AtomicT.h
 */
#ifndef _ATOMICT_H_
#define _ATOMICT_H_

#include <pthread.h>
#include <algorithm>
#include "Mutex.h"

template<typename TYPE>

class AtomicT
{
    public:
        AtomicT(){
            value_ = 0;
        }
        AtomicT(TYPE a){
            value_ = a;
        }
        ~AtomicT(){
        }

        TYPE value() {
            MutexLockGuard guard(lock_);
            return this->value_;
        }

        TYPE operator+= (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ += a;
        }

        TYPE operator-= (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ -= a;
        }

        /**
         * @brief pre-increment
         */
        TYPE operator++ (void){
            MutexLockGuard guard(lock_);
            return ++this->value_;
        }

        /**
         * @brief post-increment
         */
        TYPE operator++ (int){
            MutexLockGuard guard(lock_);
            return this->value_++;
        }

        TYPE operator-- (void){
            MutexLockGuard guard(lock_);
            return --this->value_;
        }

        TYPE operator-- (int){
            MutexLockGuard guard(lock_);
            return this->value_--;
        }

        AtomicT<TYPE>& operator= (TYPE a){
            AtomicT<TYPE> tmp(a);
            MutexLockGuard guard(lock_);
            std::swap(this->value_, tmp.value_);
            return *this;
        }

        bool operator== (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ == a;
        }

        bool operator!= (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ != a;
        }

        bool operator> (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ > a;
        }

        bool operator>= (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ >= a;
        }

        bool operator< (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ < a;
        }

        bool operator<= (TYPE a){
            MutexLockGuard guard(lock_);
            return this->value_ <= a;
        }

    private:
        TYPE value_;
        MutexLock lock_;
};

#endif
