#include "Mysql.h"

#include "Utils/Log/Log.h"
#include "Include/String.h"
#include "Public/Config/ConfigReader.h"

int GetDB(MysqlClient** mysqlClient)
{
        int ret = 0;
        struct _MysqlConfig mysqlConf;
        ConfigReader<struct _MysqlConfig> crMysql(DEFAULT_MYSQL_CONF);
        ret = crMysql.Read(mysqlConf);
        if (ret < 0) {
                LOG_ERR("mysql config read error! path=%s\n", DEFAULT_MYSQL_CONF);
                return ret;
        }
        LOG_DBG("server[%s], user[%s], passwd[%s], db[%s], port[%d], sock[%s]\n",
                mysqlConf.server.c_str(), mysqlConf.user.c_str(), mysqlConf.passwd.c_str(),
                mysqlConf.database.c_str(), mysqlConf.port, mysqlConf.socket.c_str());

        *mysqlClient = new MysqlClient(mysqlConf.server, mysqlConf.user, mysqlConf.passwd,
                                mysqlConf.database, mysqlConf.port, mysqlConf.socket.c_str());
        if (!(*mysqlClient)->IsConnected()) {
                LOG_ERR("mysql connected error!\n");
                delete *mysqlClient;
                ret = -1;
        }
        return ret;
}

int GetIpById(const string& nodeId, string& ip)
{
        MysqlData md;
        MysqlClient* mysql = NULL;
        int ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        string sql = "select ip from fbs_data_node where id=" + nodeId;
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                delete mysql;
                return -1;
        }
        delete mysql;
        ip = md.GetData(0, "ip");
        return 0;
}

int GetIdByIp(const string& ip, string& nodeId)
{
        MysqlData md;
        MysqlClient* mysql = NULL;
        int ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }
        string sql = "select id from fbs_data_node where ip='" + ip + "'";
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                delete mysql;
                return -1;
        }
        delete mysql;
        nodeId = md.GetData(0, "id");
        return 0;
}
