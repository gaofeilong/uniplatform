/* 
 * @ Date        : 2013-04-15
 * @ Auther      : wangbo
 * @ Description : 对Mysql配置文件进行操作(读,写)
 * @ Email       : wang.bo@scistor.com
 *
 * @ History     : 2013-04-15 创建
 *
 */

#include "MysqlConfigOpr.h"

#include <stdio.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

MysqlConfigOpr::MysqlConfigOpr(const string& configPath):
        ConfigOpr<struct _MysqlConfig>(configPath)
{
}

MysqlConfigOpr::~MysqlConfigOpr()
{
}

int MysqlConfigOpr::Read(struct _MysqlConfig &ddfsConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Open Error! configPath=%s\n", m_ConfigPath.c_str());
                return -1;
        }
        string Field = "mysql";

        ini.GetVal(Field, "server", ddfsConfig.server);
        ini.GetVal(Field, "user", ddfsConfig.user);
        ini.GetVal(Field, "passwd", ddfsConfig.passwd);
        ini.GetVal(Field, "database", ddfsConfig.database);
        ini.GetVal(Field, "socket", ddfsConfig.socket);

        string port;
        ini.GetVal(Field, "port", port);
        ddfsConfig.port = atoi(port.c_str());

        return 0;
}

int MysqlConfigOpr::Write(const struct _MysqlConfig &ddfsConfig)
{
        int ret = 0;
        IniConfig ini(m_ConfigPath);
        ret = ini.Open();
        if (ret < 0) {
                LOG_ERR("Open Error! configPath=%s\n", m_ConfigPath.c_str());
                return -1;
        }

        string Field = "*";
        ini.SetVal(Field, "server", ddfsConfig.server);
        ini.SetVal(Field, "user", ddfsConfig.user);
        ini.SetVal(Field, "passwd", ddfsConfig.passwd);
        ini.SetVal(Field, "database", ddfsConfig.database);
        ini.SetVal(Field, "socket", ddfsConfig.socket);

        string port = IntToString(ddfsConfig.port);
        ini.SetVal(Field, "port", port);

        ret = ini.Close();
        if (ret < 0) {
                LOG_ERR("ini close! path=%s\n", m_ConfigPath.c_str());
                ret = -2;
        }
        return ret;
}
