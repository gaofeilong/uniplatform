#ifndef _MYSQL_H_
#define _MYSQL_H_

#include "Utils/Mysql/MysqlClient.h"

int GetDB(MysqlClient** mysqlClient);

int GetIpById(const string& nodeId, string& ip);
int GetIdByIp(const string& ip, string& nodeId);
#endif //_MYSQL_H_
