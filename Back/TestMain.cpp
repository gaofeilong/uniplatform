#include <vector>
#include <string>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>

#include "Utils/Json/include/json.h"
#include "Public/Config/ConfigReader.h"
#include "Utils/Mail/Mail.h"
#include "Include/String.h"
#include "Agent/Net/NetworkMgr.h"
#include "Agent/Archive/Core/Scanner/Scanner.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Config/IniConfig.h"
#include "Agent/SysInfo/SystemMgr.h"
#include "Agent/SysInfo/UpsMgr.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Mysql/Mysql.h"

using namespace std;

void BondNC();
void UbondNC();
void SendMail();
void ReadMysql();
void Ups();
void Scan(const string& configPath);
void FbsTest();
void IniTest();
void TimeStringTest();
void CmdTest();
void MysqlTest();
void StlTest();
void JsonTest();
void SplitTest();
void FindTest();

int main(int argc, char* argv[])
{
        // if (argc != 2) {
        //         printf("invalid argument\n");
        //         return -1;
        // }
        // printf("%ld\n", UserToDigit(argv[1]));

        FindTest();
        // JsonTest();
        // SplitTest();
        // StlTest();
        // MysqlTest();
        // CmdTest();
        // TimeStringTest();
        // IniTest();
        // FbsTest();
        // SystemMgr sm;
        // vector<struct NetInfo> infoVec;
        // sm.GetNetInfo(infoVec);
        // for(size_t i=0; i<infoVec.size(); ++i) {
        //         cout << "name=" << infoVec[i].Name << endl;
        //         cout << "vendor=" << infoVec[i].Vendor << endl;
        //         cout << "status=" << infoVec[i].State << endl;
        // }
        // if (argc != 2) {
        //         cout << "参数不对!" << endl;
        //         return 0;
        // }
        // Scan(argv[1]);
        // ReadMysql();
        // BondNC();
        // UbondNC();
        // SendMail();
        return 0;
}

void Ups()
{
        int ret = 0;
        LOG_DBG("LookupUpsInfo()\n");
        Json::Value request, respond;

        UpsMgr upsMgr;
        int time, power, state;
        ret = upsMgr.GetState(state);
        if (ret != 0) {
                LOG_DBG("get ups state error");
        }
        ret = upsMgr.GetTime(time);
        if (ret != 0) {
                LOG_DBG("get ups time error");
        }
        ret = upsMgr.GetPower(power);
        if (ret != 0) {
                LOG_DBG("get ups power error");
        }

        respond["state"] = state;
        respond["time"] = time;
        respond["power"] = power;
}

void Scan(int configId)
{
        // Scanner scanner(configId);
        // scanner.GetAllArvFile();
}

void ReadMysql()
{
        struct _MysqlConfig mysqlConfig;

        ConfigReader<struct _MysqlConfig> cr("./mysql.conf");

        cr.Read(mysqlConfig);

        cout << mysqlConfig.server << endl;
        cout << mysqlConfig.user   << endl;
        cout << mysqlConfig.passwd << endl;
        cout << mysqlConfig.database << endl;
        cout << mysqlConfig.socket << endl;
        cout << mysqlConfig.port   << endl;
}

void BondNC()
{
        struct _NetworkConfig bondNc;
        bondNc.ip   = "192.168.4.80";
        bondNc.mask = "255.255.255.0";
        bondNc.gateway = "192.168.0.1";
        bondNc.name = "bond0";
        
        vector<string> ncNameSet;
        ncNameSet.push_back("eth1");
        ncNameSet.push_back("eth2");

        NetworkMgr nm;
        nm.AddBond(ncNameSet, bondNc);
}

void SendMail()
{
        SysMsgConfig sc;

        ConfigReader<SysMsgConfig> cr("./mail.conf");
        cr.Read(sc);

        vector<string> toUser;
        map<string,int>::iterator iter = sc.receiverSet.begin();
        for (; iter != sc.receiverSet.end(); ++iter) {
                toUser.push_back(iter->first);
        }

        Mail mail(sc.user, sc.password, sc.server, toUser, sc.autoType, sc.port);
        mail.SendMail("Test", "Test");

        /*
        LOG_DBG("server=%s\n", sc.server.c_str());
        LOG_DBG("port=%d\n", sc.port);
        LOG_DBG("password=%s\n", sc.password.c_str());
        LOG_DBG("autoType=%d\n", sc.autoType);
        LOG_DBG("user=%s\n", sc.user.c_str());
        */
        /*
        map<string,int>::iterator iter = sc.receiverSet.begin();
        for (; iter != sc.receiverSet.end(); ++iter) {
                LOG_DBG("first=%s ,second=%d\n", iter->first.c_str(), iter->second);
        }

        sc.server   = "192.168.0.200";
        sc.password = "111111";
        sc.autoType = 0;
        sc.port= 25;
        sc.user= "wb123wx@163.com";

        sc.receiverSet["bwang@scidata.com"] = 100;
        sc.receiverSet["flgao@scidata.com"] = 222;

        cr.Write(sc);
        */
}
void FbsTest()
{
        string cmd;
        vector<string> err;
        map<string, string> connectList;

        connectList.insert(make_pair<string, string>("192.168.1.86", "gfl"));   // no route
        connectList.insert(make_pair<string, string>("192.168.1.87", "gfl"));   // no route
        connectList.insert(make_pair<string, string>("192.168.1.88", "gfl"));   // invalid password
        connectList.insert(make_pair<string, string>("192.168.1.89", "gfl"));   // no route
        connectList.insert(make_pair<string, string>("192.168.1.90", "gfl"));   // no route
        connectList.insert(make_pair<string, string>("192.168.1.91", "gfl"));   // ok
        connectList.insert(make_pair<string, string>("192.168.1.92", "gfl"));   // ok
        connectList.insert(make_pair<string, string>("192.168.1.93", "gfl"));   // no route
        connectList.insert(make_pair<string, string>("192.168.1.94", "gfl"));   // no route
        connectList.insert(make_pair<string, string>("192.168.1.95", "gfl"));   // no route

        LOG_DBG("begin connection...");
        map<string, string>::iterator mssIt;
        for (mssIt = connectList.begin(); mssIt != connectList.end(); ++mssIt) {
                cmd = CL_FBS_CONNECT_TEST(mssIt->first, "root", mssIt->second);
                if (Execute(cmd, err) != 0) {
                        LOG_ERR("execute cmd %s error: %s", cmd.c_str(), err[err.size() - 1].c_str());
                } else {
                        LOG_ERR("connect to root@%s ok", mssIt->first.c_str());
                }
                LOG_DBG("====================================");
        }
}

void IniTest()
{
        IniConfig ini("./istri.conf");
        printf("ini.Open(): %d\n", ini.Open());
        printf("%d\n", ini.SetVal("IdxVolumePath", "count", "10"));
        ini.Close();
}

void TimeStringTest()
{
        // printf("%s\n", GetCurSec().c_str());
        // string timeStr;         
        // char buffer[32] = {0};
        // time_t sec = time(NULL);
        // strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S", localtime(&sec));
        // string timeStr = buffer;

        struct tm t;
        string timeStr = "2013-08-21T10:50:59";
        strptime(timeStr.c_str(), "%Y-%m-%dT%H:%M:%S", &t);
        printf("%ld, %d年%d月%d日%d时%d分%d秒\n", mktime(&t), t.tm_year + 1900, 
                        t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

        // memset(buffer, 0, 32);
        // strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S", localtime(&sec));
        // printf("%s\n", buffer);
}

void CmdTest()
{
        vector<string> err;
        //string cmd ="ifconfig eth2| egrep -o 'inet addr:([0-9]{1,3}.){3}[0-9]{1,3}' | egrep -o '([0-9]{1,3}.){3}[0-9]{1,3}'";
        // string cmd ="echo '/mnt/nfsserver *(rw,async,no_root_squash,fsid=0)' >> /root/exports";
        string cmd ="print 'asdf' >> /root/exports";
        if (Execute(cmd, err) != 0) {
                LOG_ERR("execute %s error", cmd.c_str());
        } else {
                LOG_DBG("execute %s ok", cmd.c_str());
        }
}

void MysqlTest()
{
        MysqlData md;
        MysqlClient* mysql;
        string sql = SQL_SELECT_NFS_INFO;
        // string sql = "select * from user";

        if (GetDB(&mysql) < 0) {
                LOG_ERR("GetDB Error!\n");
                return;
        }
        if (!mysql->Query(sql, md)) {
                LOG_ERR("mysql query error: %s", sql.c_str());
                return;
        }

        for (size_t i = 0; i < md.Rows(); i++) {
                // LOG_DBG("id: %s", md.GetData(i, "id").c_str());
                // LOG_DBG("username: %s", md.GetData(i, "username").c_str());
                // LOG_DBG("password: %s", md.GetData(i, "password").c_str());
                LOG_DBG("name: %s", md.GetData(i, "name").c_str());
                LOG_DBG("path: %s", md.GetData(i, "path").c_str());
                LOG_DBG("mp: %s", md.GetData(i, "mp").c_str());
                LOG_DBG("parameter: %s", md.GetData(i, "parameter").c_str());
                LOG_DBG("ip: %s", md.GetData(i, "ip").c_str());
                LOG_DBG("password: %s", md.GetData(i, "password").c_str());
        }

        delete mysql;
}

void StlTest()
{
        vector<string> v;
        string ip = "192.168.2.95";

        v.push_back(ip);
        printf("%s\n", find(v.begin(), v.end(), ip) == v.end()? "not founed": "founed");
}

void SplitTest()
{
        vector<string> s;
        Split("gao;fei;long;;", s, ';');
        for (vector<string>::iterator ssIt = s.begin(); ssIt != s.end(); ++ssIt) {
                printf("%s\n", ssIt->c_str());
        }
        printf("done\n");
}

void JsonTest()
{
        Json::Value v;
        v["a"].append("abc");
        v["a"].append("bcd");
        v["a"].append("cde");
        v["a"].append("ace");
        printf("%s", v.toStyledString().c_str());
}

void FindTest()
{
        string s = "Lo";
        vector<string> v;
        vector<string>::iterator i;
        v.push_back("gao");
        v.push_back("fei");
        v.push_back("Loo");
        v.push_back("long");
        i = find(v.begin(), v.end(), s);
        printf("%s\n", v.end() == i? "not exist": i->c_str());
}
