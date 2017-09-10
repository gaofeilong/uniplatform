#include "ArchiveJsonMgr.h"

#include <stdio.h>
#include "Protocol.h"
#include "ArchiveMgr.h"
#include "Utils/Log/Log.h"
#include "Utils/Crontab/Crontab.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/CommonFunc.h"
#include "Include/String.h"
#include "Include/Protocol.h"
#include "Public/Mysql/Mysql.h"
#include "Public/Config/ConfigReader.h"
#include "Server/Handler/BadRequestHandler.h"

ArchiveJsonMgr::ArchiveJsonMgr()
{
}

ArchiveJsonMgr::~ArchiveJsonMgr()
{
}

Packet *ArchiveJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_ARCHIVE_CREATE:
                // 添加归档
                return AddArchiveTask(p);
        case CMD_ARCHIVE_REMOVE:
                // 删除归档
                return DelArchiveTask(p);
        case CMD_ARCHIVE_SET:
                // 修改归档配置
                return ModifyArchiveTask(p);
        case CMD_ARCHIVE_LOOKUP:
                // 查看归档任务列表
                return LookupArchiveList(p);
        case CMD_ARCHIVE_DETAIL:
                // 查看归档详细信息
                return LookupArchiveInfo(p);
        case CMD_ARCHIVE_START:
                // 启动归档程序
                return StartArchiveTask(p);
        case CMD_ARCHIVE_STOP:
                // 停止归档程序
                return StopArchiveTask(p);
        case CMD_ARCHIVE_SERVICE_START:
                // 启动归档服务
                return StartArchiveService(p);
        case CMD_ARCHIVE_SERVICE_STOP:
                // 停止归档服务
                return StopArchiveService(p);
        }
        return RequestError("unknown request");
}

Packet *ArchiveJsonMgr::AddArchiveTask(const Packet *p)
{
        LOG_DBG("AddArchiveTask()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string ip;
        ArchiveConfig ac;
        // 修改配置文件中需修改的字段
        for (size_t i = 0; i < request["nodes"].size(); ++i) {
                ip = request["nodes"][i].asString();
                ac.arvStateSet.insert(make_pair(ip, ArchiveState(ip, "stop")));
        }

        ac.name           = request["name"].asString();
        ac.modifyTime     = request["dataTime"].asInt();
        ac.interval       = request["intervalTime"].asInt();
        ac.startTime      = request["startTime"].asString();
        ac.duration       = request["duration"].asInt();
        ac.isCheck        = request["check"].asInt();
        ac.isDeleteSource = request["deleteSource"].asInt();

        ac.dirType        = request["dirType"].asInt();
        ac.dir            = request["dir"].asString();
        ac.sizeType       = request["sizeType"].asInt();
        ac.maxSize        = atol(request["maxSize"].asString().c_str());
        ac.minSize        = atol(request["minSize"].asString().c_str());
        ac.prefixType     = request["prefixType"].asInt();
        ac.prefix         = request["prefix"].asString();
        ac.postfixType    = request["postfixType"].asInt();
        ac.postfix        = request["postfix"].asString();

        // 源-目标-备份
        for (size_t i=0; i<request["src"].size(); ++i) {
                string src = request["src"][i].asString();
                string dest = request["dest"][i].asString();
                string backup= src + "/" + GetCurMicroSec() + ARCHIVE_BACKUP_SUFFIX;
                struct PathPair pp(src, dest, backup);

                // 仅在单机归档时，判断路径存在情况
                if (ip == LOOP_BACK_IP) {
                        // 源
                        if (!HasPath(pp.m_Src)) {
                                return RequestError("["+pp.m_Src+"] not exist");
                        }
                        if (!IsDir(pp.m_Src)) {
                                return RequestError("["+pp.m_Src+"] isn't a directory");
                        }

                        // 目标
                        if (!HasPath(pp.m_Dest)) {
                                return RequestError("["+pp.m_Dest+"] not exist");
                        }
                        if (!IsDir(pp.m_Dest)) {
                                return RequestError("["+pp.m_Dest+"] isn't a directory");
                        }
                }
                ac.pathPair.push_back(pp);
        }

        // 调用添加接口
        ArchiveMgr arvMgr;
        ret = arvMgr.AddArchiveTask(ac);
        if (ret < 0) {
                return RequestError("create archive task error");
        }
        return RequestOk(respond);
}

Packet *ArchiveJsonMgr::DelArchiveTask(const Packet *p)
{
        LOG_DBG("DelArchiveTask()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        ArchiveMgr arvMgr;
        for (size_t i=0; i<request["configId"].size(); ++i) {
                string id = IntToString(request["configId"][i].asInt());
                ret = arvMgr.DelArchiveTask(id);
                if (ret < 0) {
                        return RequestError("delete archive task error!");
                }
        }
        return RequestOk(respond);
}

Packet *ArchiveJsonMgr::ModifyArchiveTask(const Packet *p)
{
        LOG_DBG("ModifyArchiveTask()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }
        string configId = IntToString(request["configId"].asInt());

        ArchiveConfig ac;
        // 读取该配置文件参数
        ConfigReader<ArchiveConfig> ca(configId);
        ret = ca.Read(ac);
        if (ret < 0) {
                return RequestError("read archive default conf error!");
        }

        // 修改配置文件中需修改的字段
        string ip;
        ac.arvStateSet.clear();
        for (size_t i = 0; i < request["nodes"].size(); ++i) {
                ip = request["nodes"][i].asString();
                ac.arvStateSet.insert(make_pair(ip, ArchiveState(ip, "stop")));
        }

        ac.name           = request["name"].asString();
        ac.modifyTime     = request["dataTime"].asInt();
        ac.interval       = request["intervalTime"].asInt();
        ac.startTime      = request["startTime"].asString();
        ac.duration       = request["duration"].asInt();
        ac.isCheck        = request["check"].asInt();
        ac.isDeleteSource = request["deleteSource"].asInt();

        ac.dirType        = request["dirType"].asInt();
        ac.dir            = request["dir"].asString();
        ac.sizeType       = request["sizeType"].asInt();
        ac.maxSize        = atol(request["maxSize"].asString().c_str());
        ac.minSize        = atol(request["minSize"].asString().c_str());
        ac.prefixType     = request["prefixType"].asInt();
        ac.prefix         = request["prefix"].asString();
        ac.postfixType    = request["postfixType"].asInt();
        ac.postfix        = request["postfix"].asString();

        // 源-目标
        ac.pathPair.clear();
        for (size_t i=0; i<request["src"].size(); ++i) {
                string src = request["src"][i].asString();
                string dest = request["dest"][i].asString();
                string backup= src + "/" + GetCurMicroSec() + 
                                                ARCHIVE_BACKUP_SUFFIX;
                struct PathPair pp(src, dest, backup);

                // 仅在单机归档时，判断路径存在情况
                if (ip == LOOP_BACK_IP) {
                        // 源
                        if (!HasPath(pp.m_Src)) {
                                return RequestError("["+pp.m_Src+"] not exist");
                        }
                        if (!IsDir(pp.m_Src)) {
                                return RequestError("["+pp.m_Src+"] isn't a directory");
                        }

                        // 目标
                        if (!HasPath(pp.m_Dest)) {
                                return RequestError("["+pp.m_Dest+"] not exist");
                        }
                        if (!IsDir(pp.m_Dest)) {
                                return RequestError("["+pp.m_Dest+"] isn't a directory");
                        }
                }
                ac.pathPair.push_back(pp);
        }
        
        //调用修改接口
        ArchiveMgr arvMgr;
        ret = arvMgr.ModifyArchiveTask(configId, ac);
        if (ret < 0) {
                return RequestError("modify archive task error");
        }
        return RequestOk(respond);
}


Packet *ArchiveJsonMgr::StartArchiveTask(const Packet *p)
{
        LOG_DBG("StartArchiveTask()\n");

        Packet *retPack  = NULL;
        retPack->Cmd     = CMD_OK;
        retPack->Version = '1';
        retPack->Magic   = 'B';
        retPack->Length  = 0;

        return retPack;
}


Packet *ArchiveJsonMgr::StopArchiveTask(const Packet *p)
{
        LOG_DBG("StopArchiveTask()\n");

        Packet *retPack  = NULL;
        retPack->Cmd     = CMD_OK;
        retPack->Version = '1';
        retPack->Magic   = 'B';
        retPack->Length  = 0;

        return retPack;
}

Packet *ArchiveJsonMgr::StartArchiveService(const Packet *p)
{
        LOG_DBG("StartArchiveService()\n");

        int ret;
        Crontab ct;
        Json::Value request, respond;

        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }
        string nodeId;
        string ip    = request["agentIp"].asString();
        string start = request["startTime"].asString();
        // 获取IP对应ID
        if (ip == LOOP_BACK_IP) {
                nodeId = "-1";
        } else {
                ret = GetIdByIp(ip, nodeId);
                if (ret < 0) {
                        LOG_ERR("GetIdByIp Error!\n");
                        return RequestError("get id for ip error");
                }
        }

        string ctCmd = start.substr(start.find(":") + 1) + " " + 
                start.substr(0, start.find(":")) + " * * * " + ARCHIVE_MAIN + 
                " " + request["configId"].asString() + " " + nodeId;
        // 修改数据库对应状态
        ArchiveMgr am;
        ret = am.SetArchiveState(request["configId"].asString(), nodeId, "idle");
        if (ret < 0) {
                LOG_ERR("SetArchiveState Error!");
                return RequestError("set archive error");
        }

        ret = ct.Add(ctCmd);
        if (ret < 0) {
                return RequestError("start archive service error");
        }

        return RequestOk(respond);
}


Packet *ArchiveJsonMgr::StopArchiveService(const Packet *p)
{
        LOG_DBG("StopArchiveService()\n");

        int ret;
        Crontab ct;
        Json::Value request, respond;

        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string nodeId;
        string ip     = request["agentIp"].asString();
        string start  = request["startTime"].asString();
        string id     = request["configId"].asString();
        // 获取IP对应ID
        if (ip == LOOP_BACK_IP) {
                nodeId = "-1";
        } else {
                ret = GetIdByIp(ip, nodeId);
                if (ret < 0) {
                        LOG_ERR("GetIdByIp Error!\n");
                        return RequestError("get id for ip error");
                }
        }

        string arvCmd = ARCHIVE_MAIN + " " + request["configId"].asString();
        string ctCmd  = start.substr(start.find(":") + 1) + " " + 
                start.substr(0, start.find(":")) + " * * * " + ARCHIVE_MAIN + 
                " " + id + " " + nodeId;

        // 获取归档状态，如果正在运行则不能停止
        string state;
        ArchiveMgr am;
        ret = am.GetArchiveState(id, nodeId, state);
        if (ret < 0) {
                return RequestError("get archive state error");
        } else if (state == "running") {
                return RequestError("archive is running");
        }
        // 更改数据库状态
        ret = am.SetArchiveState(id, nodeId, "stop");
        if (ret < 0) {
                LOG_ERR("SetArchiveState Error!");
                return RequestError("set archive error");
        }

        ret = ct.Del(ctCmd);
        if (ret < 0) {
                return RequestError("stop archive service error");
        }
        return RequestOk(respond);
}

Packet *ArchiveJsonMgr::LookupArchiveInfo(const Packet *p)
{
        LOG_DBG("LookupArchiveInfo()\n");
        return RequestError("unknown request");
}

Packet *ArchiveJsonMgr::LookupArchiveList(const Packet *p)
{
        LOG_DBG("LookupArchiveList()\n");
        int ret = 0;
        ArchiveMgr arvMgr;
        Json::Value request, respond;

        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        for (size_t i = 0; i < request["configId"].size(); ++i) {
                string id = IntToString(request["configId"][i].asInt());

                ArchiveConfig ac;
                ret = arvMgr.LookupArchiveInfo(id, ac);
                if (ret < 0) {
                        return RequestError("lookup archive info error");
                }
                Json::Value tmp, tip;
                tmp["check"]        = ac.isCheck;
                tmp["deleteSource"] = ac.isDeleteSource;
                tmp["intervalTime"] = ac.interval;
                tmp["dataTime"]     = ac.modifyTime;
                tmp["startTime"]    = ac.startTime;
                tmp["duration"]     = ac.duration;
                tmp["prefixType"]   = ac.prefixType;
                tmp["prefix"]       = ac.prefix;
                tmp["postfixType"]  = ac.postfixType;
                tmp["postfix"]      = ac.postfix;
                tmp["dirType"]      = ac.dirType;
                tmp["dir"]          = ac.dir;
                tmp["sizeType"]     = ac.sizeType;
                tmp["minSize"]      = IntToString(ac.minSize);
                tmp["maxSize"]      = IntToString(ac.maxSize);
                //tmp["state"]        = "stopped";
                tmp["isModify"]     = "true";

                map<string,ArchiveState>::iterator mssIter = ac.arvStateSet.begin();
                for (; mssIter != ac.arvStateSet.end(); ++mssIter) {
                        tip["ip"]      = mssIter->first;
                        tip["state"]   = (mssIter->second).archiveState;
                        tip["isError"] = (mssIter->second).isError;
                        tmp["nodes"].append(tip);
                        if ((mssIter->second).archiveState == "running") {
                                tmp["isModify"]  = "false";
                        }
                }
                respond[id] = tmp;
        }

        return RequestOk(respond);
}

int64_t ArchiveJsonMgr::GetRun(const string& start, const string& end)
{
        int64_t s = Htom(start);
        int64_t e = Htom(end);
        if (e <= s) {
                e += 24*60;
        }
        int64_t run = (int64_t)((e - s)/60);

        return run;
}

int64_t ArchiveJsonMgr::Htom(const string& time)
{
        size_t pos = time.find(":");
        string h = time.substr(0, pos);
        string m = time.substr(pos + 1);
        int64_t min = atoi(h.c_str())*60 + atoi(m.c_str());
        return min;
}

string ArchiveJsonMgr::CronString(const string& time, const string& interval)
{
        return "";
}
