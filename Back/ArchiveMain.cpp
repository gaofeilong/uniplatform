#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Public/Mysql/Mysql.h"
#include "Agent/Archive/Core/Archive.h"

int UpdateArvState(MysqlClient* mysql, const string& arvId, const string& nodeId, const string& state);

int main(int argc, char* argv[])
{
        int ret = 0;
        // 参数检查 0.main 1.arvId 2.nodeId
        if (argc != 3) {
                LOG_ERR("usage: ArchiveMain ConfigFile", argv[0]);
                return 0;
        }
        // 数据库连接
        MysqlClient* mysql = NULL;
        ret = GetDB(&mysql);
        if (ret < 0) {
                LOG_ERR("GetDB error!\n");
                return ret;
        }

        string arvId  = argv[1];
        string nodeId = argv[2];
        // 更新归档状态(running)
        ret = UpdateArvState(mysql, arvId, nodeId, "running");
        if (ret < 0) {
                LOG_ERR("UpdateArvState Error!");
                return ret;
        }
        // 获取IP
        string ip;
        if (nodeId == "-1") {
                ip = LOOP_BACK_IP;
        } else {
                ret = GetIpById(nodeId, ip);
                if (ret < 0) {
                        LOG_ERR("GetIpById() Error");
                        return ret;
                }
        }

        // 读取归档配置，启动归档
        Archive arv(arvId, ip, nodeId);
        ret = arv.CronStart();
        if (ret < 0) {
                LOG_ERR("start archive error, conf=%s\n", argv[1]);
        }
        // 更新归档状态(idle)
        ret = UpdateArvState(mysql, arvId, nodeId, "idle");
        if (ret < 0) {
                LOG_ERR("UpdateArvState Error!");
        }
        return ret;
}

int UpdateArvState(MysqlClient* mysql, const string& arvId, const string& nodeId, const string& state)
{
        int ret = 0;
        string sql = "update node_archive_state set archive_state='" + state + 
                     "' where archive_id=" + arvId + " and data_node_id=" + nodeId;
        if (!mysql->Query(sql)) {
                LOG_ERR("mysql query error! sql=%s\n", sql.c_str());
                ret = -1;
        }
        return ret;
}
