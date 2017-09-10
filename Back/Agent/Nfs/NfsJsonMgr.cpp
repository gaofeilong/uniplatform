/*******************************************************************************
 *
 * @ File Name  : NfsJsonMgr.cpp
 * @ Date       : 2013-10-21
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: NFS 模块协议解析类
 * @ History    : 2013-10-21：创建
 *
 * ****************************************************************************/


#include <stdio.h>
#include <string.h>

#include "NfsJsonMgr.h"

#include "Protocol.h"
#include "NfsMgr.h"
#include "Utils/Log/Log.h"

NfsJsonMgr::NfsJsonMgr()
{
}

NfsJsonMgr::~NfsJsonMgr()
{
}

Packet *NfsJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_NFS_LOOKUP:
                return LookupNfsList(p);
        case CMD_NFS_CREATE:
                return CreateNfs(p);
        case CMD_NFS_REMOVE:
                return RemoveNfs(p);
        case CMD_NFS_EDIT:
                return EditNfs(p);
        case CMD_NFS_START:
                return StartNfs(p);
        case CMD_NFS_STOP:
                return StopNfs(p);
        case CMD_NFS_MOUNT_CLIENT:
                return MountNfsClient(p);
        case CMD_NFS_MOUNT_ALL_CLIENT:
                return MountAllNfsClient(p);
        case CMD_NFS_UMOUNT_CLIENT:
                return UmountNfsClient(p);
        case CMD_NFS_UMOUNT_ALL_CLIENT:
                return UmountAllNfsClient(p);
        case CMD_NFS_REMOVE_CLIENT:
                return RemoveNfsClient(p);
        }
        return 0;
}

Packet *NfsJsonMgr::LookupNfsList(const Packet *p)
{
        LOG_DBG("LookupNfsList()\n");

        NfsMgr nfs;
        Json::Value request, respond;
        vector<struct NfsServerInfo> nfsServerInfoList;

        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }
        if (nfs.GetInfo(nfsServerInfoList) < 0) {
                return RequestError("get nfs information error");
        }

        vector<struct NfsServerInfo>::iterator vsIt = nfsServerInfoList.begin();
        for (; vsIt != nfsServerInfoList.end(); ++vsIt) {
                Json::Value tmpServerItem, tmpClientItem;
                tmpServerItem["name"]      = vsIt->name;
                tmpServerItem["path"]      = vsIt->path;
                tmpServerItem["mp"]        = vsIt->mp;
                tmpServerItem["state"]     = vsIt->state;
                tmpServerItem["mpState"]   = vsIt->mpState;
                tmpServerItem["parameter"] = vsIt->parameter;
                vector<struct NfsClientInfo>::iterator vcIt = vsIt->clientList.begin();
                for (; vcIt != vsIt->clientList.end(); ++vcIt) {
                        tmpClientItem["ip"] = vcIt->ip;
                        tmpClientItem["password"] = vcIt->password;
                        tmpClientItem["state"] = vcIt->state;
                        tmpServerItem["client"].append(tmpClientItem);
                }
                respond["nfsList"].append(tmpServerItem);
        } 

        return RequestOk(respond);
}

Packet *NfsJsonMgr::CreateNfs(const Packet *p)
{
        LOG_DBG("CreateNfs()\n");

        int ret;
        NfsMgr nfs;
        vector<string> failed;
        Json::Value request, respond;
        struct NfsServerInfo nfsServerInfo;
        struct NfsClientInfo nfsClientInfo;

        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }
        nfsServerInfo.name      = request["name"].asString();
        nfsServerInfo.path      = request["path"].asString();
        nfsServerInfo.mp        = request["mp"].asString();
        nfsServerInfo.parameter = request["parameter"].asString();
        for (size_t i = 0; i < request["client"].size(); ++i) {
                nfsClientInfo.ip       = request["client"][i]["ip"].asString();
                nfsClientInfo.password = request["client"][i]["password"].asString();
                nfsServerInfo.clientList.push_back(nfsClientInfo);
        }

        ret = nfs.Create(nfsServerInfo, failed);
        if (ret < 0) {
                string info = "create nfs task error";
                if (ret == -6) {
                        info += ", shared path not mounted";
                }
                return RequestError(info);
        }

        if (failed.size() != 0) {
                string info = "add ";
                vector<string>::iterator vsIt = failed.begin(); 
                for (; vsIt != failed.end(); ++vsIt) {
                        info += *vsIt + ";";
                }
                // respond["info"] = "add 192.168.2.92;192.168.2.93 error
                respond["info"] = info.substr(0, info.length() - 1) + " error";
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::EditNfs(const Packet *p)
{
        LOG_DBG("EditNfs()\n");

        NfsMgr nfs;
        vector<string> failed;
        Json::Value request, respond;
        struct NfsServerInfo nfsServerInfo;
        struct NfsClientInfo nfsClientInfo;

        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }
        nfsServerInfo.name      = request["name"].asString();
        nfsServerInfo.path      = request["path"].asString();
        nfsServerInfo.mp        = request["mp"].asString();
        nfsServerInfo.parameter = request["parameter"].asString();
        for (size_t i = 0; i < request["newClient"].size(); ++i) {
                nfsClientInfo.ip       = request["newClient"][i]["ip"].asString();
                nfsClientInfo.password = request["newClient"][i]["password"].asString();
                nfsServerInfo.clientList.push_back(nfsClientInfo);
        }
        if (nfs.Edit(nfsServerInfo, failed) < 0) {
                return RequestError("edit nfs task error");
        }

        if (failed.size() != 0) {
                string info = "add ";
                vector<string>::iterator vsIt = failed.begin(); 
                for (; vsIt != failed.end(); ++vsIt) {
                        info += *vsIt + ";";
                }
                // respond["info"] = "add 192.168.2.92;192.168.2.93 error
                respond["info"] = info.substr(0, info.length() - 1) + " error";
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::RemoveNfs(const Packet *p)
{
        LOG_DBG("RemoveNfs()\n");

        NfsMgr nfs;
        bool isError = false;
        string info = "remove nfs ";
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        for (size_t i = 0; i < request["name"].size(); ++i) {
                string name = request["name"][i].asString();
                if(nfs.Remove(name) < 0) {
                        LOG_ERR("remove nfs %s error", name.c_str()); 
                        info += name + ";";
                        isError = true;
                }
        }
        if (isError) {
                respond["info"] = info.substr(0, info.length() - 1) + " error";
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::StartNfs(const Packet *p)
{
        LOG_DBG("StartNfs()\n");

        NfsMgr nfs;
        Json::Value respond;
        if (nfs.Start() != 0) {
                return RequestError("start nfs service error");
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::StopNfs(const Packet *p)
{
        LOG_DBG("StopNfs()\n");

        NfsMgr nfs;
        Json::Value respond;
        if (nfs.Stop() != 0) {
                return RequestError("stop nfs service error");
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::MountNfsClient(const Packet *p)
{
        LOG_DBG("MountNfsClient()\n");

        NfsMgr nfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string name   = request["name"].asString();
        string client = request["client"].asString();
        if (nfs.MountClient(name, client) < 0) {
                return RequestError("mount client " + client + " error");
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::MountAllNfsClient(const Packet *p)
{
        LOG_DBG("MountAllNfsClient()\n");

        NfsMgr nfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        bool isError = false;
        string info = "mount client ";
        string name   = request["name"].asString();
        for (size_t i = 0; i < request["client"].size(); ++i) {
                string client = request["client"][i].asString();
                if (nfs.MountClient(name, client) < 0) {
                        isError = true;
                        info += client + ";";
                }
        }

        if (isError) {
                respond["info"] = info.substr(0, info.length() - 1) + " error";
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::UmountNfsClient(const Packet *p)
{
        LOG_DBG("UmountNfsClient()\n");

        NfsMgr nfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string name   = request["name"].asString();
        string client = request["client"].asString();
        if (nfs.UmountClient(name, client) < 0) {
                return RequestError("umount client " + client + " error");
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::UmountAllNfsClient(const Packet *p)
{
        LOG_DBG("UmountAllNfsClient()\n");

        NfsMgr nfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        bool isError = false;
        string info = "umount client ";
        string name   = request["name"].asString();
        for (size_t i = 0; i < request["client"].size(); ++i) {
                string client = request["client"][i].asString();
                if (nfs.UmountClient(name, client) < 0) {
                        isError = true;
                        info += client + ";";
                }
        }

        if (isError) {
                respond["info"] = info.substr(0, info.length() - 1) + " error";
        }
        return RequestOk(respond);
}

Packet *NfsJsonMgr::RemoveNfsClient(const Packet *p)
{
        LOG_DBG("RemoveNfsClient()\n");

        NfsMgr nfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string name   = request["name"].asString();
        string client = request["client"].asString();
        if (nfs.RemoveClient(name, client) < 0) {
                return RequestError("remove client " + client + " error");
        }
        return RequestOk(respond);
}
