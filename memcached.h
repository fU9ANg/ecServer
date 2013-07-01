
#ifndef __ECSERVER_MEMCACHED_H
#define __ECSERVER_MEMCACHED_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <string>
#include <libmemcached/memcached.h>

#define DEFAULT_EXPIRATION  5
#define MC_SERVER           "192.168.0.254"
#define MC_PORT             11211

using namespace std;

class Memcached
{
    public:
        ~Memcached ();


        bool op_insert (const char* key, const char* value, time_t expir = DEFAULT_EXPIRATION);
        bool op_update (const char* key, const char* value, time_t expir = DEFAULT_EXPIRATION);
        void*op_select (const char* key);
        bool op_delete (const char* key);

        static Memcached* instance ();
    private:
        Memcached () {} ;
        bool init ();

        string m_host;
        string m_port;
        string m_expir;
        static Memcached* m_instance;

    private:
        memcached_st* mem_cached;
};

#define MEMCACHED Memcached::instance()

#endif
