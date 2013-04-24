
#ifndef _BUF_H_
#define _BUF_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

class Buf 
{
public:
    Buf(size_t inSize = 5120) {
        size_ = inSize;
        ptr_ = malloc(size_);
        fd_ = 0;
        id_ = 0;
    };

    ~Buf(){
        if ( NULL != ptr_) {
            free(ptr_);
            ptr_ = NULL;
        }
    }

    Buf(Buf& b) {
        size_ = b.size_;
        this->ptr_ = malloc(size_);
        memcpy(this->ptr_, b.ptr_, size_);
    };

    void* ptr() {
        return ptr_;
    }

    size_t setsize(size_t in) {
        return used_ = in;
    }

    size_t maxsize() {
        return size_;
    }

    size_t size() {
        return used_;
    }

    void reset() {
        memset(ptr_, 0x00, size_);
    }

    int getfd() {
        return fd_;
    }
    void setfd(int fd) {
        fd_ = fd;
    }

    int getid() {
        return id_;
    }
    void setid(int id) {
        id_ = id;
    }

    Buf& operator= (Buf& b) {
        memcpy(this->ptr_, b.ptr(), size_);
        return *this;
    }
private:
    void* ptr_;
    size_t size_;
    size_t used_;
    int fd_;
    int id_;
};

#endif
