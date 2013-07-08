
#ifndef __CONN_MONGODB_H
#define __CONN_MONGODB_H

#include <iostream>
#include <string>
#include <pthread.h>
#include <map>

#include "Mutex.h"
#include "client/dbclient.h"

using namespace std;
using namespace mongo;

class Mongodb
{
    typedef map<string, string> KVMAP;
    typedef map<string, KVMAP> KVsMAP;
    typedef vector<string> KEYVECTOR;
    public:
        Mongodb ();
        Mongodb (int conn_number);
        ~Mongodb ();
    public:
        int set_wr_timeout (double t);
        int connect (string host = "localhost", int port = 27017);
        int setdb (string database, string collection);
 
        int setindex (string key);
        int get (KVMAP& out, KEYVECTOR in, string key, string value);
        int gets (KVsMAP& rout, KEYVECTOR fields, KEYVECTOR in, string key);
        int dumpkey (KVMAP& rout, string key, string val);
        int dumpvals (KVsMAP& rout, KEYVECTOR in, string key);

        int set (KVMAP in, string key,string value);
        int sets (KVsMAP in, string key);
        int remove (string key, string value);

        static Mongodb* instance ();

    private:
        string m_doc_db;
        static Mongodb* m_instance;
        double wr_timeout; // time-out with write operator of mongodb
        MutexLock m_lock;
        sem_t m_sem;
        map <DBClientConnection*,bool> connlst;
        pthread_mutex_t m_db_mutex;
};

#define MONGODB Mongodb::instance()

#endif
