/**
 * ingroup database database
 * @{
 */ 

#ifndef _DATABASE_H_
#define _DATABASE_H_
#include <string>
#include <iostream>


#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/resultset.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <glog/logging.h>

#include "Config.h"
#include "Mutex.h"

using namespace sql;
using namespace std;

class DataBase {
    public:
        ~DataBase();
        /**
         * @brief init the connection with mysql
         * @param[in] host      address of mysql
         * @param[in] username  username of mysql
         * @param[in] password  password of mysql
         * @param[in] database  database of mysql
         * @retval    true      success
         * @retval    false     faild
         */
        bool Init(string host, string username, string password, string database);

        /**
         * @brief get a Connection of mysql
         */
        Connection* getConnection();

        /**
         * @brief get a Statement of mysql
         */
        Statement* getStatement();

        /**
         * @brief get a prepare Statement of mysql
         */
        PreparedStatement* preStatement(string sql);

        static DataBase* instance();
        MutexLock m_mutex;
    private:
        DataBase();
        Driver* m_pDriver;
        Connection* m_pConn;
        string m_host;
        string m_user_name;
        string m_password;
        string m_database;
        static DataBase *pinstance;
};
#define DATABASE DataBase::instance()
#endif
/**
 * @}
 */
