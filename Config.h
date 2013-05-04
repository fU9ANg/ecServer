/**
 * @addtogroup framework
 * @{
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <string>

#include "lua5.2/lua.hppp"
using namespace std;

/**
 * @class 配置文件类
 */
class Config {
    public:
        ~Config();
        static class Config* instance();
        /**
         * @brief 读取配置文件
         * @param file 配置文件路径
         */
        void readconfig(string file);
        string db_host;
        string db_username;
        string db_password;
        string db_database;
        string server_ip;
        int server_port;
        int school_id;
        int game_count;

    private:
        Config();
        static class Config* p_;
};

#define CONFIG Config::instance()
#endif

///@}
