// compiling:
// g++ mongodb.cc  -I. -lboost_system -lboost_thread -lboost_program_options -lboost_filesystem -lrt -lpthread

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include <string.h>
#include <pthread.h>
#include <iostream>
#include "mongodb.h"

using namespace mongo;
using namespace std;

#define SUCCESS         0
#define INTERR          1
#define FAILURE         2

#define CONN_NUMBER     10

#define DB_DB           "localhost"
#define DB_COLLECTION   "unix"

Mongodb* Mongodb::m_instance = NULL;

Mongodb* Mongodb::instance ()
{
    if (m_instance == NULL)
        m_instance = new Mongodb (CONN_NUMBER);

    return (m_instance);
}

Mongodb::Mongodb (int number)
{
    m_doc_db = string (DB_DB) + "." + string (DB_COLLECTION);
    wr_timeout = 3;

    if (number < 0)
        number = 1;
    if (number > 20)
        number = 20;

    if(connlst.size () > 0) return;

    bool auto_conn=true;

    if ((sem_init (&m_sem, 0, 0)) < 0) return;

    MutexLockGuard guard (m_lock);
    for(int i=0; i<number; ++i) {
        DBClientConnection* pconn = new DBClientConnection (auto_conn, 0, wr_timeout);
        if(pconn != NULL) {
            connlst[pconn] = false;
        }
    }
}

Mongodb::~Mongodb ()
{
    m_doc_db = "";
    MutexLockGuard guard (m_lock);

    map <DBClientConnection*,bool>::iterator it = connlst.begin();
    while (it != connlst.end()) {
        delete it->first;
        it++;
    }

}
int Mongodb::set_wr_timeout (double t)
{
    wr_timeout = t;
    return SUCCESS;
}

int Mongodb::connect (string host,int port)
{
    MutexLockGuard guard (m_lock);

    map<DBClientConnection*,bool>::iterator it = connlst.begin();
    while (it != connlst.end()) {
        string errmsg = "";
        HostAndPort host_port (host,port);
        if (!(it->first->connect (host_port, errmsg))) {
            cerr << "try connect host:"<< host << ", port:"<< port << ", msg:"<< errmsg <<endl;
            it->second = true;
        }
        sem_post (&m_sem);
        it++;
    }

    return SUCCESS;
}

int Mongodb::setdb (string database, string collection)
{
    if (database.empty() || collection.empty()) {
        return FAILURE;
    }
    m_doc_db = database + "." + collection;

    return SUCCESS;
}

int Mongodb::setindex (string key)
{
    if (key.empty()) {
        return FAILURE;
    }

    MutexLockGuard guard (m_lock);
    sem_wait (&m_sem);

    map<DBClientConnection*,bool>::iterator it = connlst.begin();
    while (it!=connlst.end()) {
        if (it->second == false) {
            it->second =true;
            break;
        }
        it++;
    }

    string bindex = "{" + key + ":1}";
    it->first->ensureIndex (m_doc_db, fromjson (bindex));
 
    it->second = false;
    sem_post (&m_sem);
 
    return SUCCESS;
}

// get value of key
int Mongodb::get (map<string,string>& out, vector<string> in, string key, string value)
{
    if(key.empty() || value.empty() || in.size()<=0){
        return FAILURE;
    }
    BSONObjBuilder b;
    for (vector<string>::iterator iter = in.begin(); iter != in.end(); ++iter) {
        b.append (*iter, 1);
    }

    MutexLockGuard guard (m_lock);
    sem_wait (&m_sem);

    map<DBClientConnection*,bool>::iterator it = connlst.begin();
    while (it!=connlst.end()) {
        if (it->second == false) {
            it->second = true;
            break;
        }
        it++;
    }

    BSONObj ob=b.obj();
 
    BSONObj p=it->first->findOne (m_doc_db, QUERY(key<<value), &ob);
 
    map<string,string> temp;
    for(vector<string>::iterator iter = in.begin(); iter != in.end(); ++iter) {
        string mkey = *iter;
        temp[*iter] = p.getStringField (mkey.c_str());
    }
    out = temp;
 
    it->second = false;
    sem_post (&m_sem);
 
    return SUCCESS;
}

// get data-sets format is "<key, map<key, value>>"
int Mongodb::gets(map< string,map<string,string> >& rout,vector<string> fields,vector<string> in,string key)
{
    if (key.empty()) {
        return FAILURE;
    }
    MutexLockGuard guard (m_lock);
    sem_wait(&m_sem);
    //pthread_mutex_lock(&_jobmux);
    map<DBClientConnection*,bool>::iterator it=connlst.begin();
    while(it!=connlst.end()){
        if(it->second == false){
            it->second=true;
            break;
        }
        it++;
    }
    //pthread_mutex_unlock(&_jobmux);
 
    BSONObjBuilder b;
    b.append(key,1);
    for(vector<string>::iterator iter=fields.begin();iter!=fields.end();++iter){
        b.append(*iter,1);
    }
 
    BSONObj p=b.obj();
    for(vector<string>::iterator iter2=in.begin();iter2!=in.end();++iter2){
        BSONObj ob=it->first->findOne(m_doc_db, QUERY(key<<*iter2),&p);
        map<string,string> temp;
        for(vector<string>::iterator iter=fields.begin();iter!=fields.end();++iter){
            string mkey=*iter;
            temp[*iter]=ob.getStringField(mkey.c_str());   
        }
        rout[ob.getStringField(key.c_str())]=temp;
    }
 
    //pthread_mutex_lock(&_jobmux);
    it->second=false;
    //pthread_mutex_unlock(&_jobmux);
    sem_post(&m_sem);
 
    return SUCCESS;
}

// return value of key by dump
int Mongodb::dumpkey(map< string,string >& rout,string key,string val)
{
    if(key.empty()){
        return FAILURE;
    }
    sem_wait(&m_sem);
    MutexLockGuard guard (m_lock);
    //pthread_mutex_lock(&_jobmux);
    map<DBClientConnection*,bool>::iterator it=connlst.begin();
    while(it!=connlst.end()){
        if(it->second == false){
            it->second=true;
            break;
        }
        it++;
    }
    //pthread_mutex_unlock(&_jobmux);
 
    BSONObjBuilder b;
    b.append(key,1);
    if(!val.empty()){
        b.append(val,1);
    }
 
    BSONObj p=b.obj();
 
    pthread_mutex_lock(&m_db_mutex);
    auto_ptr<DBClientCursor> cursor = it->first->query(m_doc_db, Query(),0,0,&p);
    while(cursor->more()){
        BSONObj ob=cursor->next();
        rout[ob.getStringField(key.c_str())]=ob.getStringField(val.c_str());
    }
    pthread_mutex_unlock(&m_db_mutex);
 
    //pthread_mutex_lock(&_jobmux);
    it->second=false;
    //pthread_mutex_unlock(&_jobmux);
    sem_post(&m_sem);
 
    return SUCCESS;
}

// return: <key, map<key, value>>
int Mongodb::dumpvals(map< string,map<string,string> >& rout,vector<string> in,string key)
{
    if(key.empty()){
        return FAILURE;
    }
    sem_wait(&m_sem);
    MutexLockGuard guard (m_lock);

    map<DBClientConnection*,bool>::iterator it=connlst.begin();
    while(it!=connlst.end()){
        if(it->second == false){
            it->second=true;
            break;
        }
        it++;
    }
 
    BSONObjBuilder b;
    b.append (key,1);
    for(vector<string>::iterator iter=in.begin(); iter != in.end(); ++iter) {
        b.append (*iter,1);
    }
 
    BSONObj p=b.obj();
 
    pthread_mutex_lock(&m_db_mutex);
    auto_ptr<DBClientCursor> cursor = it->first->query (m_doc_db, Query(), 0, 0, &p);
    while(cursor->more()){
        BSONObj ob=cursor->next();
        map<string,string> temp;
        for(vector<string>::iterator iter=in.begin();iter!=in.end();++iter){
            string val=*iter;
            temp[val]=ob.getStringField(val.c_str());
        }
        rout[ob.getStringField(key.c_str())]=temp;
        temp.clear();
    }
    pthread_mutex_unlock(&m_db_mutex);
 
    it->second=false;
    sem_post(&m_sem);
 
    return SUCCESS;
}

// update:
// example key = "e-mail", value = "bb.newlife@gmail.com"
int Mongodb::set (map<string,string>in, string key, string value)
{

    if (in.size()<=0 || key.empty() || value.empty()){
        return FAILURE;
    }
    BSONObjBuilder b;
    map<string,string>::iterator iter;
    for (iter = in.begin(); iter!=in.end(); ++iter) {
        b.append (iter->first, iter->second);
    }
     
    MutexLockGuard guard (m_lock);
    sem_wait (&m_sem);

    map<DBClientConnection*,bool>::iterator it = connlst.begin();
    while (it != connlst.end()) {
        if (it->second == false) {
            it->second = true;
            break;
        }
        it++;
    }

    BSONObj ob = b.obj();
    it->first->update (m_doc_db, QUERY(key<<value), BSON(""<<ob), true);
 
    int ret = SUCCESS;
    string errmsg = it->first->getLastError();
    if (!errmsg.empty()) {
        ret = INTERR;
    }
 
    it->second = false;
    sem_post (&m_sem);
 
    return ret;
}

// update multi-fields format is (key = "e-mail")
//  "__id123": <key1,value1>, <key2,value2>
//  "__id124": <key1,value1>, <key2,value2>
int Mongodb::sets (map<string, map<string,string> > in, string key)
{
    if (in.size() <= 0 || key.empty()) {
        return FAILURE;
    }
 
    MutexLockGuard guard (m_lock);
    sem_wait(&m_sem);

    map<DBClientConnection*,bool>::iterator it=connlst.begin();
    while(it!=connlst.end()){
        if(it->second == false){
            it->second=true;
            break;
        }
        it++;
    }
 
    int ret=SUCCESS;
    map< string,map<string,string> >::iterator iter;
    for(iter=in.begin();iter!=in.end();++iter){
        BSONObjBuilder b;
        for(map<string,string>::iterator iter2=iter->second.begin();iter2!=iter->second.end();++iter2){
            b.append(iter2->first,iter2->second);
        }
        BSONObj ob=b.obj();
        it->first->update(m_doc_db, QUERY(key<<iter->first),BSON(""<<ob),true);
        string errmsg=it->first->getLastError();
        if(!errmsg.empty()){
            ret=INTERR;
        }
    }
     
    it->second=false;
    sem_post(&m_sem);
 
    return ret;
}

// key = e-mail, value = "bb.newlife@gmail.com"
// delete record e-mail = "bb.newlife@gmail.com"
int Mongodb::remove (string key, string value)
{
    if (key.empty () || value.empty ()) {
        return FAILURE;
    }

    MutexLockGuard guard (m_lock);
    sem_wait (&m_sem);

    map<DBClientConnection*,bool>::iterator it = connlst.begin ();
    while (it != connlst.end()) {
        if (it->second == false){
            it->second = true;
            break;
        }
        it++;
    }
 
    it->first->remove (m_doc_db, BSON (key << value)); 
    it->second = false;

    sem_post (&m_sem);
 
    return SUCCESS;
}
