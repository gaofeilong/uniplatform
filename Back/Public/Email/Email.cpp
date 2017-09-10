#include "Email.h"

#include <vector>
#include <string>

#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Include/String.h"
#include "Public/Mysql/Mysql.h"

using std::string;
using std::vector;

Mail *GetEmailObj(const string &level)
{
        MysqlData md;
        MysqlClient* mysql = NULL;
        string sql = CL_GET_EMAIL_CONF(level);

        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB error!\n");
                return NULL;
        }
        if (!mysql->Query(sql, md)) { 
                LOG_ERR("mysql query error: sql=%s\n", sql.c_str());
                delete mysql;
                return NULL;
        }
        if (md.Rows() == 0) {
                LOG_INF("empty record");
                return NULL;
        }
        delete mysql;

        int type;
        vector<string> recvList;
        string server, send, sendPass, verify;

        MysqlDataRow mdr;
        for (size_t i = 0; i < md.Rows(); i++) {
                mdr = md.GetRow(i);
                if (i == 0) {
                        server   = mdr["server"];
                        send     = mdr["send"];
                        sendPass = mdr["send_passwd"];
                        type     = mdr["verify"]=="ssl"? 1: 0;
                }
                SplitFlag(mdr["recv"], ";", recvList);
        }
        return new Mail(send, sendPass, server, recvList, type, 25);
}
