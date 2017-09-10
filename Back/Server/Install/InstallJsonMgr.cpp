#include "Protocol.h"
#include "InstallJsonMgr.h"
#include "Utils/Log/Log.h"
#include "Include/String.h"
#include "Include/Protocol.h"
#include "Public/Config/ConfigReader.h"
#include "Server/Install/InstallMgr.h"
#include "Server/Install/DdfsLicenseMgr.h"
#include "Server/Install/IstriLicenseMgr.h"
#include "Server/Handler/BadRequestHandler.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/CommonFunc.h"

InstallJsonMgr::InstallJsonMgr()
{
}

InstallJsonMgr::~InstallJsonMgr()
{
}

Packet *InstallJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_CONNECT_INDEX:
                return ConnectIndexNode(p);
        case CMD_CONNECT_DATA:
                return ConnectDataNode(p);
        case CMD_INSTALL_INDEX:
                return InstallIndexNode(p);
        case CMD_INSTALL_DATA:   
                return InstallDataNode(p);
        case CMD_REMOVE_INDEX:
                return RemoveIndexNode(p);
        case CMD_REMOVE_DATA:
                return RemoveDataNode(p);
        case CMD_LICENSE_LOOKUP:
                return LookupLic(p);	        //查看license信息
        case CMD_LICENSE_EXPORT:
                return ExportLic(p);	        //导出license
        case CMD_LICENSE_IMPORT:
                return ImportLic(p);	        //导入license
        default:
                return RequestError("unknown request");
        }
}

Packet *InstallJsonMgr::ConnectIndexNode(const Packet *p)
{
        LOG_DBG("ConnectIndexNode()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        InstallMgr installer;
        string user     = request["user"].asString();
        string password = request["password"].asString();
        string ip       = request["connectIp"].asString();
        if (installer.ConnectIndexNode(user, password, ip) < 0) {
                return RequestError("connect index node error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::ConnectDataNode(const Packet *p)
{
        LOG_DBG("ConnectDataNode()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        InstallMgr installer;
        vector<string> ipList;
        string user     = request["user"].asString();
        string password = request["password"].asString();
        string indexId  = request["indexId"].asString();
        for (size_t i = 0; i < request["connectIp"].size(); ++i) {
                ipList.push_back(request["connectIp"][i].asString());
        }
        if (installer.ConnectDataNode(user, password, ipList, indexId) < 0) {
                return RequestError("connect data node error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::InstallIndexNode(const Packet *p)
{
        LOG_DBG("InstallIndexNode()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        InstallMgr installer;
        vector<string> idxRootPath;
        string user        = request["user"].asString();
        string password    = request["password"].asString();
        string ip          = request["installIp"].asString();
        if (installer.InstallIndexNode(user, password, ip) < 0) {
                return RequestError("install index node error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::InstallDataNode(const Packet *p)
{
        LOG_DBG("InstallDataNode()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        InstallMgr installer;
        vector<string> ipList, dataPathList;
        string user      = request["user"].asString();
        string password  = request["password"].asString();
        string indexId   = request["indexId"].asString();
        for (size_t i = 0; i < request["installIp"].size(); ++i) {
                ipList.push_back(request["installIp"][i].asString());
        }
        if (installer.InstallDataNode(user, password, ipList, indexId) < 0) {
                return RequestError("install data node error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::RemoveIndexNode(const Packet *p)
{
        LOG_DBG("RemoveIndexNode()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        InstallMgr installer;
        vector<string> ipList;
        string user     = request["user"].asString();
        string password = request["password"].asString();
        string ip       = request["removeIp"].asString();
        if (installer.RemoveIndexNode(user, password, ip) < 0) {
                return RequestError("remove index node error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::RemoveDataNode(const Packet *p)
{
        LOG_DBG("RemoveDataNode()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        InstallMgr installer;
        vector<string> ipList;
        string user     = request["user"].asString();
        string password = request["password"].asString();
        string indexId  = request["indexId"].asString();
        for (size_t i = 0; i < request["removeIp"].size(); ++i) {
                ipList.push_back(request["removeIp"][i].asString());
        }
        if (installer.RemoveDataNode(user, password, ipList, indexId) < 0) {
                return RequestError("remove data node error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::LookupLic(const Packet *p)
{
        LOG_DBG("LookupLic()\n");

        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        struct LicenseInfo licInfo;
        BOOSTPTR<LicenseMgr> lic = GetLicenseObj(request["agentIp"].asString());
        if (!lic) {
                return RequestError("get license obj error");
        }
        if (lic->GetLicenseInfo(licInfo) < 0) {
                return RequestError("get license info error");
        }
        respond["Version"]         = licInfo.Version;
        respond["Activated"]       = licInfo.Activated;
        respond["NodeNumber"]      = licInfo.NodeNumber;
        respond["CurNodeNumber"]   = licInfo.CurNodeNumber;
        respond["TimePoStatus"]    = licInfo.TimePoStatus;
        respond["QuotaPoStatus"]   = licInfo.QuotaPoStatus;
        respond["ABSTimePoStatus"] = licInfo.ABSTimePoStatus;
        respond["AvailTime"]       = IntToString(licInfo.AvailTime);
        respond["CurQuota"]        = IntToString(licInfo.CurQuota);
        respond["ABSQuota"]        = IntToString(licInfo.ABSQuota);
        respond["AvailABSTime"]    = IntToString(licInfo.AvailABSTime);
        respond["SerialCode"]      = licInfo.SerialCode;

        return RequestOk(respond);
}

Packet *InstallJsonMgr::ExportLic(const Packet *p)
{
        LOG_DBG("ExportLic()\n");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        struct LicenseInfo licInfo;
        BOOSTPTR<LicenseMgr> lic = GetLicenseObj(request["agentIp"].asString());
        if (!lic) {
                return RequestError("get license obj error");
        }
        if (lic->ExportLicense() < 0) {
                return RequestError("export license error");
        }
        return RequestOk(respond);
}

Packet *InstallJsonMgr::ImportLic(const Packet *p)
{
        LOG_DBG("ImportLic()\n");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        struct LicenseInfo licInfo;
        BOOSTPTR<LicenseMgr> lic = GetLicenseObj(request["agentIp"].asString());
        if (!lic) {
                return RequestError("get license obj error");
        }
        if (lic->ImportLicense() < 0) {
                return RequestError("import license error");
        }
        return RequestOk(respond);
}

BOOSTPTR<LicenseMgr> InstallJsonMgr::GetLicenseObj(const string &ip)
{
        string cmd;
        vector<string> err;
        struct _GatewayConfig c;
        BOOSTPTR<LicenseMgr> licPtr;
        ConfigReader<struct _GatewayConfig> gatewayConfigReader(SCIGW_CONF);
        if (gatewayConfigReader.Read(c) < 0) {
                LOG_ERR("read system config error");
                return licPtr;
        }
        if (c.systemType != SYSTEM_TYPE_DISTRIBUTE && c.systemType != SYSTEM_TYPE_SINGLE) {
                LOG_ERR("system type not supported");
                return licPtr;
        }
        cmd = CL_GET_NETWORK_IP(c.networkId);
        if (Execute(cmd, err) != 0) {
                LOG_ERR("execute %s error", cmd.c_str());
                return licPtr;
        }
        string localIp = err[err.size() - 1];

        if ((ip == localIp || ip == "127.0.0.1") && (c.systemType != SYSTEM_TYPE_DISTRIBUTE)) {
                licPtr = BOOSTPTR<LicenseMgr>(new DdfsLicenseMgr);
        } else {
                MysqlData md;
                MysqlClient* mysql = NULL;
                if (GetDB(&mysql) < 0) {
                        LOG_ERR("GetDB Error!\n");
                        return licPtr;
                }
                if (!mysql->Query(SQL_SELECT_INDEX_NODE_INFO(ip), md)) {
                        LOG_ERR("mysql query error: \n", SQL_SELECT_INDEX_NODE_INFO(ip).c_str());
                        delete mysql;
                        return licPtr;
                }
                delete mysql;
                if (md.Rows() != 0) {
                        licPtr = BOOSTPTR<LicenseMgr>(new IstriLicenseMgr(ip, "root", md.GetData(0,0)));
                } else {
                        LOG_INF("cannot find index node: %s", ip.c_str());
                } 
        }
        return licPtr;
}
