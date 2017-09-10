#ifndef _MYSQL_CONFIG_OPR_H_
#define _MYSQL_CONFIG_OPR_H_

#include <string>
#include <vector>
#include "Public/Config/ConfigOpr.h"

using std::string;
using std::vector;

struct _MysqlConfig {
        _MysqlConfig()
        {
        }
        string server;
        string user;
        string passwd;
        string database;
        string socket;
        int    port;
};


class MysqlConfigOpr : public ConfigOpr<struct _MysqlConfig> {
public:
        MysqlConfigOpr(const string& configPath);
        ~MysqlConfigOpr();

public:
        int  Read(struct _MysqlConfig& ddfsConfig);
        int  Write(const struct _MysqlConfig& ddfsConfig);
};

#endif //_MYSQL_CONFIG_OPR_H_
