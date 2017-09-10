/* 
 * @ Date        : 2013-04-01
 * @ Auther      : wangbo
 * @ Description : 处理系统信息模块协议
 * @ Email       : wang.bo@scistor.com
 *
 * @ History     : 2013-04-01 创建
 *               : 2013-04-07 实习协议分发
 *
 */

#include "SysInfoJsonMgr.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SystemMgr.h"
#include "SysTimeMgr.h"
#include "UpsMgr.h"
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Include/Protocol.h"
#include "Public/Email/Email.h"
#include "Utils/Common/CommonFunc.h"
#include "Server/Handler/BadRequestHandler.h"
#include "Public/Config/ConfigReader.h"

using std::vector;

SysInfoJsonMgr::SysInfoJsonMgr()
{
}

SysInfoJsonMgr::~SysInfoJsonMgr()
{
}

Packet *SysInfoJsonMgr::Handle(const Packet *p)
{
        Json::Value respond;
        switch(p->Cmd) {
        case CMD_SYS_INFO_LOOKUP:
                return LookupSysInfo(p);        //查看系统信息
        case CMD_SYS_TIME_LOOKUP:
                return LookupSysTime(p);	//查看系统时间
        case CMD_SYS_TIME_SET:
                return SetSysTime(p);	        //配置系统时间
        case CMD_SYS_WARNING_LOOKUP:
                return LookupMsgInfo(p);	//查看事件通知配置
        case CMD_SYS_WARNING_SET:
                return SetSysMsgConf(p);	//修改事件通知配置
        case CMD_SYS_UPS_LOOKUP:
                return LookupUpsInfo(p);	//查看UPS配置信息
        case CMD_SYS_UPS_SET:
                return SetUpsConf(p);	        //修改UPS配置
        case CMD_SYS_LICENSE_LOOKUP:
                return LookupLic(p);	        //查看license信息
        case CMD_SYS_LICENSE_EXPORT:
                return ExportLic(p);	        //导出license
        case CMD_SYS_LICENSE_IMPORT:
                return ImportLic(p);	        //导入license
        case CMD_SYS_UPGRADE_IMPORT:
                return UpdatePro(p);	        //导入升级包,升级程序
        case CMD_SYS_EMAIL_TEST:
                return EmailTest(p);	        //邮件测试
        default:
                return RequestError("unknown request");
        }
        return 0;
}

Packet *SysInfoJsonMgr::LookupSysInfo(const Packet *p)
{
        LOG_DBG("LookupSysInfo()\n");
        int ret = 0;

        Json::Value tmp, request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        SystemMgr sysMgr;
        struct SystemInfo sysInfo;
        ret = sysMgr.GetSystemInfo(sysInfo);
        if (ret < 0) {
                return RequestError("get system info error");
        }

        // sys
        tmp.clear();
        tmp["scigw"] = sysInfo.Os.Scigw;
        tmp["kernel"] = sysInfo.Os.Kernel;
        tmp["release"] = sysInfo.Os.Release;
        respond["os"] = tmp;

        // cpu
        vector<struct CpuInfo>::iterator vcIt = sysInfo.Cpu.begin();
        for (; vcIt != sysInfo.Cpu.end(); ++vcIt) {
                tmp.clear();
                tmp["hz"]     = vcIt->Freq;
                tmp["name"]   = vcIt->Name;
                tmp["vendor"] = vcIt->Vendor;
                tmp["cache"]  = vcIt->Cache;
                respond["cpu"].append(tmp);
        }

        //mem
        tmp.clear();
        tmp["size"] = sysInfo.Mem.PTotal;
        respond["mem"] = tmp;

        //disk
        vector<struct DiskInfo>::iterator vdIt = sysInfo.Disk.begin();
        for (vdIt = sysInfo.Disk.begin(); vdIt != sysInfo.Disk.end(); ++vdIt) {
                tmp.clear();
                tmp["name"] = vdIt->Name;
                tmp["size"] = vdIt->Size;
                tmp["vendor"] = vdIt->Vendor;
                respond["disk"].append(tmp);
        }
        
        //net
        vector<struct NetInfo>::iterator vnIt = sysInfo.Net.begin();
        for (; vnIt != sysInfo.Net.end(); ++vnIt) {
                tmp.clear();
                if (vnIt->Name.find("bond") != string::npos) {
                        continue;
                }
                tmp["name"] = vnIt->Name;
                tmp["state"] = vnIt->State;
                tmp["vendor"] = vnIt->Vendor;
                respond["net"].append(tmp);
        }

        // //fan
        // vector<struct FanInfo>::iterator vfIt = sysInfo.Fan.begin();
        // for (; vfIt != sysInfo.Fan.end(); ++vfIt) {
        //         tmp.clear();
        //         tmp["rpm"] = vfIt->RPM;
        //         tmp["name"] = vfIt->Name;
        //         tmp["state"] = vfIt->State;
        //         respond["fan"].append(tmp);
        // }

        // //temperature
        // vector<struct TempInfo>::iterator vtIt = sysInfo.Temp.begin();
        // for (; vtIt != sysInfo.Temp.end(); ++vtIt) {
        //         tmp.clear();
        //         tmp["name"]        = vtIt->Name;
        //         tmp["state"]       = vtIt->State;
        //         tmp["temperature"] = vtIt->Temperature;
        //         respond["temp"].append(tmp);
        // }

        return RequestOk(respond);
}

Packet * SysInfoJsonMgr::SetSysTime(const Packet *p)
{
        LOG_DBG("SetSysTime()\n");

        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        SysTimeMgr timeMgr;
        string time, timeServer;

        if (request["sync"].asInt() == 1) {
                /* set ntp & set zone & sync time */
                ret = timeMgr.SetNtpServer(request["ntp"].asString());
                if (ret != 0) {
                        return RequestError("set ntp server error");
                }
                ret = timeMgr.SetZone(request["zone"].asString());
                if (ret != 0) {
                        return RequestError("set time zone error");
                }
                ret = timeMgr.SyncTime(request["ntp"].asString());
                if (ret != 0) {
                        return RequestError("sync time error");
                }
        } else {
                /* set ntp & set zone & set time */
                if (!request["ntp"].isNull()) {
                        ret = timeMgr.SetNtpServer(request["ntp"].asString());
                        if (ret != 0) {
                                return RequestError("set ntp server error");
                        }
                }
                ret = timeMgr.SetZone(request["zone"].asString());
                if (ret != 0) {
                        return RequestError("set time zone error");
                }
                ret = timeMgr.SetTime(request["time"].asString());
                if (ret != 0) {
                        return RequestError("set time error");
                }
        }

        return RequestOk(respond);
}

Packet * SysInfoJsonMgr::LookupSysTime(const Packet *p)
{
        LOG_DBG("LookupSysTime()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string time, zone, ntp;
        SysTimeMgr timeMgr;
        ret = timeMgr.GetZone(zone);
        if (ret != 0) {
                return RequestError("get time zone error");
        }
        ret = timeMgr.GetTime(time);
        if (ret != 0) {
                return RequestError("get time error");
        }
        ret = timeMgr.GetNtpServer(ntp);
        if (ret != 0) {
                return RequestError("get ntp server ip error");
        }

        respond["ntp"] = ntp;
        respond["time"] = time;
        respond["zone"] = zone;

        return RequestOk(respond);
}

Packet * SysInfoJsonMgr::SetSysMsgConf(const Packet *p)
{
        LOG_DBG("SetSysMsgConf()\n");

        Packet *retPack  = NULL;
        retPack->Cmd     = CMD_OK;
        retPack->Version = '1';
        retPack->Magic   = 'B';
        retPack->Length  = 0;

        return 0;
}

Packet * SysInfoJsonMgr::LookupMsgInfo(const Packet *p)
{
        LOG_DBG("LookupMsgInfo()\n");

        Packet *retPack  = NULL;
        retPack->Cmd     = CMD_OK;
        retPack->Version = '1';
        retPack->Magic   = 'B';
        retPack->Length  = 0;

        return 0;
}

Packet * SysInfoJsonMgr::SetUpsConf(const Packet *p)
{
        LOG_DBG("SetUpsConf()\n");

        int ret = 0;
        UpsMgr upsMgr;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        ret = upsMgr.SetTime(request["time"].asInt());
        if (ret != 0) {
                return RequestError("set ups time error");
        }
        ret = upsMgr.SetPower(request["power"].asInt());
        if (ret != 0) {
                return RequestError("set ups power error");
        }
        ret = upsMgr.SetState(request["state"].asInt());
        if (ret != 0) {
                string errInfo = (ret == -2? "set service state error":
                                  ret == -3? "ups device not connected":
                                             "unknown error");
                return RequestError(errInfo);
        }

        return RequestOk(respond);
}

Packet * SysInfoJsonMgr::LookupUpsInfo(const Packet *p)
{
        LOG_DBG("LookupUpsInfo()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        UpsMgr upsMgr;
        int time, power, state;
        ret = upsMgr.GetState(state);
        if (ret != 0) {
                return RequestError("get ups state error");
        }
        ret = upsMgr.GetTime(time);
        if (ret != 0) {
                return RequestError("get ups time error");
        }
        ret = upsMgr.GetPower(power);
        if (ret != 0) {
                return RequestError("get ups power error");
        }

        respond["state"] = state;
        respond["time"] = time;
        respond["power"] = power;

        return RequestOk(respond);
}

Packet * SysInfoJsonMgr::EmailTest(const Packet *p)
{
        LOG_DBG("EmailTest()\n");
        int ret = 0;

        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string host   = request["host"].asString();
        string user   = request["user"].asString();
        string passwd = request["password"].asString();
        string type   = (request["type"].asString() == "ssl") ? "1" : "0";
        string recv   = request["recv"].asString();

        vector<string> recvList;
        SplitFlag(recv, ";", recvList);

        Mail mail(user, passwd, host, recvList, atoi(type.c_str()), 25);
        ret = mail.SendMail("Test", "This's a mail for test");
        if (ret < 0) {
                return RequestError("send test mail error");
        }
        return RequestOk(respond);
}

Packet * SysInfoJsonMgr::LookupLic(const Packet *p)
{
        LOG_DBG("LookupLic()\n");
        return RequestError("don't supported");
}

Packet * SysInfoJsonMgr::ExportLic(const Packet *p)
{
        LOG_DBG("ExportLic()\n");
        return RequestError("don't supported");
}

Packet * SysInfoJsonMgr::ImportLic(const Packet *p)
{
        LOG_DBG("ImportLic()\n");
        return RequestError("don't supported");
}

Packet * SysInfoJsonMgr::UpdatePro(const Packet *p)
{
        LOG_DBG("UpdatePro()\n");

        Packet *retPack  = NULL;
        retPack->Cmd     = CMD_OK;
        retPack->Version = '1';
        retPack->Magic   = 'B';
        retPack->Length  = 0;

        return 0;
}
