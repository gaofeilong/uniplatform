/*******************************************************************************
 *
 * @ File Name  : FsJsonMgr.cpp
 * @ Date       : 2013-10-30
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 处理消冗文件系统模块的协议分发
 * @ History    : 2013-10-30：创建
 *
 * ****************************************************************************/


#include "FsJsonMgr.h"

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "DdfsMgr.h"
#include "AdfsMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Include/Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Config/ConfigReader.h"

FsJsonMgr::FsJsonMgr()
{
}

FsJsonMgr::~FsJsonMgr()
{
}

Packet *FsJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        // ddfs
        case CMD_DDFS_LIST:
                return LookupDdfsMpList(p);     //DDFS挂载点列表
        case CMD_DDFS_CREATE:
                return CreateDdfsMp(p);         //新建DDFS挂载点
        case CMD_DDFS_MODIFY:
                return ModifyDdfsMp(p);         //编辑DDFS挂载点
        case CMD_DDFS_MOUNT:
                return MountDdfsMp(p);          //挂载DDFS挂载点
        case CMD_DDFS_UMOUNT:
                return UmountDdfsMp(p);         //卸载DDFS挂载点
        case CMD_DDFS_FIX:
                return FsckDdfsMp(p);           //修复DDFS挂载点
        case CMD_DDFS_REMOVE:
                return RemoveDdfsMp(p);         //删除DDFS挂载点
        // adfs
        case CMD_ADFS_LIST:
                return LookupAdfsMpList(p);     //ADFS挂载点列表
        case CMD_ADFS_CREATE:
                return CreateAdfsMp(p);         //新建ADFS挂载点
        case CMD_ADFS_MODIFY:
                return ModifyAdfsMp(p);         //编辑ADFS挂载点
        case CMD_ADFS_MOUNT:
                return MountAdfsMp(p);          //挂载ADFS挂载点
        case CMD_ADFS_UMOUNT:
                return UmountAdfsMp(p);         //卸载ADFS挂载点
        case CMD_ADFS_REMOVE:
                return RemoveAdfsMp(p);         //删除ADFS挂载点

        }
        return 0;
}

Packet *FsJsonMgr::LookupDdfsMpList(const Packet *p)
{
        LOG_DBG("LookupMpList()\n");

        int ret = 0;
        Json::Value respond;

        DdfsMgr dm;
        vector<struct _DdfsPair> mpList;
        ret = dm.GetMpList(mpList);
        if (ret < 0) {
                LOG_ERR("GetMpList Error!\n");
                return RequestError("get mp list error");
        }

        for (size_t i=0; i<mpList.size(); ++i) {
                Json::Value tmp;
                tmp["id"]             = mpList[i].id;
                tmp["mountPoint"]     = mpList[i].mp;   
                tmp["configPath"]     = mpList[i].config;
                PacketDdfsMpConf(mpList[i].config, tmp);
                respond.append(tmp);
        }
        return RequestOk(respond);
}

Packet *FsJsonMgr::CreateDdfsMp(const Packet *p)
{
        LOG_DBG("CreateDdfsMp()\n");

        int ret = 0;
        struct _DdfsConfig ddfsConfig;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        // FileSystem
        ddfsConfig.delayedCloseTime   = request["delayedCloseTime"].asString();
        ddfsConfig.maxDelayedCloseNum = request["maxDelayedCloseNum"].asString();
        ddfsConfig.raChunkCnt         = request["raChunkCnt"].asString();
        ddfsConfig.raStrategy         = request["raStrategy"].asString();
        ddfsConfig.fsBucketCount      = request["fsBucketCount"].asString();
        ddfsConfig.mmapSize           = request["mmapSize"].asString();
        ddfsConfig.mountPoint         = request["mountPoint"].asString();
        // DPL
        ddfsConfig.dplCpuNum          = request["dplCpuNum"].asString();
        ddfsConfig.dplTimeOutSec      = request["dplTimeOutSec"].asString();
        ddfsConfig.dplCapacity        = request["dplCapacity"].asString();
        ddfsConfig.dplReadMode        = request["dplReadMode"].asString();
        // LDS
        ddfsConfig.ldsBucketCount     = request["bucketCount"].asString();
        ddfsConfig.minSpace           = request["minSpace"].asString();
        ddfsConfig.biPath             = request["biPath"].asString();
        ddfsConfig.fileTypeCount      = request["fileTypeCount"].asString();
        //PLDS
        ddfsConfig.blockSize          = request["blockSize"].asString();
        ddfsConfig.chunkSize          = request["chunkSize"].asString();
        ddfsConfig.dedupOption        = request["dedupOption"].asString();
        ddfsConfig.crcOption          = request["crcOption"].asString();
        ddfsConfig.byteCmpOption      = request["byteCmpOption"].asString();
        ddfsConfig.hashType           = request["hashType"].asString();
        ddfsConfig.compressType       = request["compressType"].asString();
        ddfsConfig.mapPath            = request["mapPath"].asString();
        ddfsConfig.metaPath           = request["metaPath"].asString();
        for (size_t i=0; i<request["dataPath"].size(); ++i) {
                ddfsConfig.dataPathList.push_back(request["dataPath"][i].asString());
        }

        string tmp;
        DdfsMgr dm;
        vector<string> errInfo;
        ret = dm.Create(ddfsConfig, errInfo);
        if (ret < 0) {
                LOG_ERR("Create Error!\n");
                for (size_t i=0; i<errInfo.size(); ++i) {
                        tmp += errInfo[i] + " ";
                }
                return RequestError(tmp);
        }

        return RequestOk(respond);
}

Packet *FsJsonMgr::ModifyDdfsMp(const Packet *p)
{
        LOG_DBG("ModifyDdfsMp()\n");

        int ret = 0;
        vector<string> errInfo;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string configPath = request["configPath"].asString();

        // 得到新的所有路径
        vector<string> allPathList;
        Json::Value newPathList = request["dataPath"];
        for (size_t i = 0; i < newPathList.size(); ++i) {
                string newPath = newPathList[i].asString();
                AppendBias(newPath);
                allPathList.push_back(newPath);
        }

        DdfsMgr dm;
        ret = dm.Modify(configPath, allPathList, errInfo);
        if (ret < 0) {
                LOG_ERR("Modify Error!\n");
                string tmp;
                for (size_t i=0; i<errInfo.size(); ++i) {
                        tmp += errInfo[i] + " ";
                }
                return RequestError(tmp);
        }
        return RequestOk(respond);
}

Packet *FsJsonMgr::MountDdfsMp(const Packet *p)
{
        LOG_DBG("MountDdfsMp()\n");

        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        struct _DdfsPair ddfsPair;
        ddfsPair.mp     = request["mountPoint"].asString();
        ddfsPair.config = request["configPath"].asString();

        vector<string> errInfo;
        DdfsMgr dm;
        ret = dm.Mount(ddfsPair, errInfo);
        if (ret < 0) {
                LOG_ERR("Mount Error!\n");
                string tmp;
                for (size_t i=0; i<errInfo.size(); ++i) {
                        tmp += errInfo[i] + " ";
                }
                return RequestError(tmp);
        }
        return RequestOk(respond);
}

Packet *FsJsonMgr::UmountDdfsMp(const Packet *p)
{
        LOG_DBG("UmountDdfsMp()\n");

        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        struct _DdfsPair ddfsPair;
        ddfsPair.mp     = request["mountPoint"].asString();
        ddfsPair.config = request["configPath"].asString();

        vector<string> errInfo;
        DdfsMgr dm;
        ret = dm.Umount(ddfsPair, errInfo);
        if (ret < 0) {
                LOG_ERR("Umount Error!\n");
                string tmp;
                for (size_t i=0; i<errInfo.size(); ++i) {
                        tmp += errInfo[i] + " ";
                }
                return RequestError(tmp);
        }
        return RequestOk(respond);
}

Packet *FsJsonMgr::FsckDdfsMp(const Packet *p)
{
        LOG_DBG("FsckDdfsMp()\n");

        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        string configPath = request["configPath"].asString();
        string mountPoint = request["mountPoint"].asString();

        vector<string> errInfo;
        DdfsMgr dm;
        ret = dm.Fsck(mountPoint, configPath, errInfo);
        if (ret < 0) {
                LOG_ERR("Fsck Error!\n");
                string tmp;
                for (size_t i=0; i<errInfo.size(); ++i) {
                        tmp += errInfo[i] + " ";
                }
                return RequestError(tmp);
        }
        return RequestOk(respond);
}

Packet *FsJsonMgr::RemoveDdfsMp(const Packet *p)
{
        LOG_DBG("RemoveDdfsMp()\n");

        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0) {
                return RequestError("parse request error");
        }

        DdfsMgr dm;
        for (size_t i=0; i<request["configPath"].size(); ++i) {
                string result;
                vector<string> errInfo;
                string config = request["configPath"][i].asString();

                ret = dm.Remove(config);
                if (ret < 0) {
                        LOG_ERR("remove mountpoint error");
                        return RequestError("remove mountpoint error");
                }
        }

        return RequestOk(respond);
}

void FsJsonMgr::PacketDdfsMpConf(const string &conf, Json::Value &pack)
{
        DdfsMgr dm;
        int ret = 0;
        string dataPathSet;
        vector<string> errInfo;
        struct _DedupInfo dedupInfo;
        struct _DdfsConfig ddfsConfig;

        ret = dm.LookConf(conf, ddfsConfig);
        if (ret < 0) {
                LOG_ERR("LookConf Error!\n");
                pack["state"] = MP_STATE_UNKNOWN;
                return;
        }

        // FileSystem
        // mountPoint 在函数外部设置了
        pack["raChunkCnt"]         = ddfsConfig.raChunkCnt;
        pack["raStrategy"]         = ddfsConfig.raStrategy;
        pack["fsBucketCount"]      = ddfsConfig.fsBucketCount;
        pack["mmapSize"]           = ddfsConfig.mmapSize;
        pack["delayedCloseTime"]   = ddfsConfig.delayedCloseTime;
        pack["maxDelayedCloseNum"] = ddfsConfig.maxDelayedCloseNum;
        pack["mountPoint"]         = ddfsConfig.mountPoint;
        // DPL
        pack["dplCpuNum"]     = ddfsConfig.dplCpuNum;
        pack["dplTimeOutSec"] = ddfsConfig.dplTimeOutSec;
        pack["dplCapacity"]   = ddfsConfig.dplCapacity;
        pack["dplReadMode"]   = ddfsConfig.dplReadMode;
        // LDS
        pack["bucketCount"]   = ddfsConfig.ldsBucketCount;
        pack["minSpace"]      = ddfsConfig.minSpace;
        pack["biPath"]        = ddfsConfig.biPath;
        pack["fileTypeCount"] = ddfsConfig.fileTypeCount;
        // PLDS
        pack["blockSize"]     = ddfsConfig.blockSize; 
        pack["chunkSize"]     = ddfsConfig.chunkSize;
        pack["dedupOption"]   = ddfsConfig.dedupOption;
        pack["crcOption"]     = ddfsConfig.crcOption;
        pack["byteCmpOption"] = ddfsConfig.byteCmpOption;
        pack["hashType"]      = ddfsConfig.hashType;
        pack["compressType"]  = ddfsConfig.compressType;
        pack["mapPath"]       = ddfsConfig.mapPath;
        pack["metaPath"]      = ddfsConfig.metaPath;
        pack["state"]         = dm.GetMpStatus(ddfsConfig.mountPoint);
        pack["nfsState"]       = Execute(CL_NFS_STATUS, errInfo) == 0? "running": "stopped";
        for (size_t i=0; i<ddfsConfig.dataPathList.size(); ++i) {
                pack["dataPath"].append(ddfsConfig.dataPathList[i]);
                dataPathSet = dataPathSet.empty()?  ddfsConfig.dataPathList[i]: 
                                dataPathSet + " " + ddfsConfig.dataPathList[i];
        }

        // 挂载点状态：如果获取到的挂载点为挂载或者只读状态，需要获取数据空间
        // 信息若数据剩余容量为0，则为noapace状态。如果获取到的挂载点状态不是
        // 挂载或者只读，那么将页面显示的所有数据容量信息和消冗比都置0
        if (pack["state"].asString() == MP_STATE_MOUNTED || 
                pack["state"].asString() == MP_STATE_READONLY) {
                ret = Execute(CL_GET_PATH_CAPACITY(dataPathSet), errInfo);
                if (ret != 0) {
                        LOG_ERR("get ddfs mountpoint %s capacity error: %s",
                                ddfsConfig.mountPoint.c_str(), errInfo[0].c_str());
                        pack["capacity"] = "0"; 
                        pack["used"]     = "0"; 
                        pack["left"]     = "0"; 
                        pack["state"]    = MP_STATE_UNKNOWN;
                } else {
                        pack["capacity"] = errInfo[0];
                        pack["used"]     = errInfo[1];
                        pack["left"]     = errInfo[2];

                        if (pack["state"].asString() == MP_STATE_READONLY && 
                                atoll(pack["left"].asString().c_str()) <= 
                                atoll(ddfsConfig.minSpace.c_str()) * BYTE_PER_G) {
                                pack["state"] = MP_STATE_NOSPACE;
                        }
                }

                ret = dm.GetDedupInfo(ddfsConfig.mountPoint, dedupInfo);         
                if (ret < 0) {
                        LOG_ERR("get ddfs mountpoint %s dedup info error!\n", 
                                                ddfsConfig.mountPoint.c_str());
                        pack["state"]    = MP_STATE_UNKNOWN;
                        pack["capacity"] = "0";
                        pack["used"]     = "0";
                        pack["left"]     = "0"; 
                } else {
                        pack["ratio"]    = dedupInfo.ratio.empty()? "0": dedupInfo.ratio;
                        pack["real"]     = dedupInfo.real.empty()? "0B": dedupInfo.real;
                        pack["total"]    = dedupInfo.total.empty()? "0B": dedupInfo.total;
                }
        } else {
                pack["capacity"] = "0";
                pack["used"]     = "0";
                pack["left"]     = "0";
                pack["total"]    = "0B";
                pack["real"]     = "0B";
                pack["ratio"]    = "0";
        }
}

Packet *FsJsonMgr::LookupAdfsMpList(const Packet *p)
{
        LOG_DBG("LookupAdfsMpList()\n");

        AdfsMgr adfs;
        vector<string> err;
        Json::Value tmp, respond;
        vector<struct _AdfsInfo> adfsInfoList;
        vector<struct _AdfsInfo>::iterator vaIt;

        if (adfs.GetMpList(adfsInfoList) < 0) {
                return RequestError("get adfs info error");
        }

        for (vaIt = adfsInfoList.begin(); vaIt != adfsInfoList.end(); ++vaIt) {
                tmp.clear();
                tmp["capacity"] = vaIt->capacity;
                tmp["left"]     = vaIt->left;
                tmp["total"]    = vaIt->total;
                tmp["real"]     = vaIt->real;
                tmp["ratio"]    = vaIt->ratio;
                tmp["state"]    = vaIt->state;
                tmp["mountPoint"]    = vaIt->firstDataPath + "/mp";
                tmp["firstDataPath"] = vaIt->firstDataPath;
                tmp["nfsState"]      = Execute(CL_NFS_STATUS, err) == 0? 
                                       "running": "stopped";
                vector<string>::iterator vsIt = vaIt->otherDataPath.begin(); 
                for ( ; vsIt != vaIt->otherDataPath.end(); ++vsIt) {
                        tmp["otherDataPath"].append(*vsIt);
                }
                respond["adfsList"].append(tmp);
        }

        return RequestOk(respond);
}

Packet *FsJsonMgr::CreateAdfsMp(const Packet *p)
{
        LOG_DBG("CreateAdfsMp()\n");

        AdfsMgr adfs;
        vector<string> otherDataPath;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string firstDataPath = request["firstDataPath"].asString();
        for (size_t i = 0; i < request["otherDataPath"].size(); ++i) {
                otherDataPath.push_back(request["otherDataPath"][i].asString());
        }

        if (adfs.Create(firstDataPath, otherDataPath) < 0) {
                return RequestError("create adfs mp error");
        }

        return RequestOk(respond);
}

Packet *FsJsonMgr::ModifyAdfsMp(const Packet *p)
{
        LOG_DBG("ModifyAdfsMp()\n");

        AdfsMgr adfs;
        vector<string> newDataPath;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        vector<struct _AdfsInfo> adfsInfoList; 
        vector<struct _AdfsInfo>::iterator vaIt;
        string firstDataPath = request["firstDataPath"].asString();

        // 检索出所有路径中，新增的路径，用于添加到adfs配置
        if (adfs.GetMpList(adfsInfoList) < 0) {
                return RequestError("get adfs info error");
        }
        for (vaIt = adfsInfoList.begin(); vaIt != adfsInfoList.end() && 
                    vaIt->firstDataPath != firstDataPath; ++vaIt) { }
        for (size_t i = 0; i < request["allOtherDataPath"].size(); ++i) {
                vector<string>::iterator vsIt;
                string tmpPath = request["allOtherDataPath"][i].asString();

                vsIt = std::find(vaIt->otherDataPath.begin(), 
                     vaIt->otherDataPath.end(), tmpPath);
                if (vaIt->otherDataPath.end() == vsIt) {
                        newDataPath.push_back(tmpPath);
                        LOG_DBG("not find %s", tmpPath.c_str());
                } else {
                        LOG_DBG("find %s", tmpPath.c_str());
                }
        }

        if (adfs.Modify(firstDataPath, newDataPath) < 0) {
                return RequestError("modify adfs mp error");
        }

        return RequestOk(respond);
}

Packet *FsJsonMgr::MountAdfsMp(const Packet *p)
{
        LOG_DBG("MountAdfsMp()\n");

        AdfsMgr adfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string firstDataPath = request["firstDataPath"].asString();
        if (adfs.Mount(firstDataPath) < 0) {
                return RequestError("mount adfs mp error");
        }

        return RequestOk(respond);
}

Packet *FsJsonMgr::UmountAdfsMp(const Packet *p)
{
        LOG_DBG("UmountAdfsMp()\n");

        AdfsMgr adfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string firstDataPath = request["firstDataPath"].asString();
        if (adfs.Umount(firstDataPath) < 0) {
                return RequestError("umount adfs mp error");
        }

        return RequestOk(respond);
}

Packet *FsJsonMgr::RemoveAdfsMp(const Packet *p)
{
        LOG_DBG("RemoveAdfsMp()\n");
        AdfsMgr adfs;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        for (size_t i = 0; i < request["firstDataPath"].size(); ++i) {
                string firstDataPath = request["firstDataPath"][i].asString();
                if (adfs.Remove(firstDataPath) < 0) {
                        string mp = firstDataPath + "/mp";
                        return RequestError("remove adfs mp " + mp + " error");
                }
        }

        return RequestOk(respond);
}
