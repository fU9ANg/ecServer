
// written by fU9ANg
// bb.newlife@gmail.com

#include "memcached.h"

Memcached* Memcached::m_instance = NULL;

Memcached* Memcached::instance ()
{
    if (m_instance == NULL)
    {
        m_instance = new Memcached ();
        m_instance->init ();
    }

    return (m_instance);
}

bool Memcached::init ()
{
    memcached_return mrc;
    memcached_server_st* server = NULL;

    mem_cached  = memcached_create (NULL);
    server      = memcached_server_list_append (server, MC_SERVER, MC_PORT, &mrc);

    mrc = memcached_server_push (mem_cached, server);

    if (mrc != MEMCACHED_SUCCESS) {
        return (false);
        cout << "[error]: memcached_server_push failed! result: " << mrc << endl;
    }

    return (true);
}

Memcached::~Memcached ()
{
    if (mem_cached)
        memcached_free (mem_cached);
}

bool Memcached::op_insert (const char* key, const char* value, time_t expir)
{
    if (key == NULL || value == NULL)
        return (false);

    uint32_t flags = 0;
    memcached_return mrc;

    mrc = memcached_set (mem_cached, key, strlen (key), value, strlen (value), expir, flags);
    
    if (mrc != MEMCACHED_SUCCESS)
        return (false);

    return (true);
}

void* Memcached::op_select (const char* key)
{
    if (key == NULL)
        return (NULL);

    uint32_t flags = 0;
    memcached_return mrc;
    size_t value_len;
    char* value = memcached_get (mem_cached, key, strlen (key), &value_len, &flags, &mrc);

    if (mrc != MEMCACHED_SUCCESS)
        return (NULL);

    return ((void*)value);
}

bool Memcached::op_delete (const char* key)
{
    if (key == NULL)
        return (false);

    // TODO:
    return (true);
}

bool Memcached::op_update (const char* key, const char* value, time_t expir)
{
    if (key == NULL || value == NULL)
        return (false);

    // TODO:
    return (true);
}
