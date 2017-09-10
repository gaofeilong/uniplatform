#include "DiskJsonMgr.h"
#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "LvmMgr.h"
#include "FsOper.h"
#include <stdio.h>
#include <algorithm>

using std::string;
using std::vector;
using namespace FsOper;

DiskJsonMgr::DiskJsonMgr()
{
}

DiskJsonMgr::~DiskJsonMgr()
{
}

Packet *DiskJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_STORAGE_VG_LOOKUP:
                return ListVolume(p);
        case CMD_STORAGE_VG_SHOWSINGLE:
                return ShowSingle(p);
        case CMD_STORAGE_VG_MERGE:
                return MergeVg(p);
        case CMD_STORAGE_LV_LOOGUP:
                return ListLv(p);
        case CMD_STORAGE_LV_CREATE:
                return CreateLv(p);
        case CMD_STORAGE_LV_REMOVE:
                return DeleteLv(p);
        case CMD_STORAGE_LV_EXTEND:
                return ModifyLv(p);
        }
        LOG_ERR("DiskJsonMgr: unknown cmd: %08X", p->Cmd);
        return RequestError("unknown request");
}

Packet *DiskJsonMgr::ListVolume(const Packet *p)
{
        LOG_DBG("ListVolume()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        vector<LvmMgr::VgInfo> vl;
        if (!LvmMgr::GetVgList(vl))
        {
                return RequestError("cannot get lv list");
        }
        respond["vglist"] = Json::Value();
        for (uint i=0; i<vl.size(); ++i)
        {
                respond["vglist"][i]["name"] = vl[i].name;
                respond["vglist"][i]["size"] = vl[i].vgSize;
                respond["vglist"][i]["free"] = vl[i].vgFree;
                respond["vglist"][i]["lvcount"] = vl[i].lvcount;
        }
        return RequestOk(respond);
}

Packet *DiskJsonMgr::ShowSingle(const Packet *p)
{
        LOG_DBG("CreateLv()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        return RequestOk(respond);
}

Packet *DiskJsonMgr::MergeVg(const Packet *p)
{
        LOG_DBG("CreateLv()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        return RequestOk(respond);
}

Packet *DiskJsonMgr::ListLv(const Packet *p)
{
        LOG_DBG("ListLv()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        vector<LvmMgr::LvInfo> vlv;
        if (!LvmMgr::GetLvList(vlv))
        {
                return RequestError("cannot get lv list from lvm");
        }
        vector<stLvInfo> vl;
        if (RET_SUCCESS != GetLvSql(vl))
        {
                return RequestError("cannot get lv list from sql");
        }
        respond["lvlist"] = Json::Value();
        for (uint i=0; i<vl.size(); ++i)
        {
                //if (NOT_FIND == FindMem(vlv, LvmMgr::LvInfo(vl[i].path)))
                uint lv_exist = 0;
                for (uint j=0; j<vlv.size(); ++j)
                {
                        if (vlv[j].lvPath == vl[i].path)
                        {
                                lv_exist = 1;
                                if (vlv[j].lvSize > 0)
                                {
                                        vl[i].size = vlv[j].lvSize;
                                }
                                break;
                        }
                }
                if (0 == lv_exist)
                {
                        SetLvSql(vl[i], LVSQL_DEL);
                        LOG_ERR("ListLv(): lv in sql is not exist, deleted: %s", vl[i].path.c_str());
                        continue;
                }
                respond["lvlist"][i]["lvpath"] = vl[i].path;
                respond["lvlist"][i]["type"] = vl[i].fsfmt;
                respond["lvlist"][i]["size"] = (int)vl[i].size;
                respond["lvlist"][i]["isused"] = vl[i].used;
                respond["lvlist"][i]["mount"] = vl[i].mount;
        }
        return RequestOk(respond);
}

Packet *DiskJsonMgr::CreateLv(const Packet *p)
{
        LOG_DBG("CreateLv()\n");
        int ret = 0;
        char errinfo[1024] = {0};
        Json::Value request, respond;
        // 判断包头包体结构正确性
        ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        // 解析包中参数
        string lvname, vgname, fsfmt;
        int size = 0, type = 1;
        request["lvnewname"].isNull() ? lvname : lvname = request["lvnewname"].asString();
        request["lvgroup"].isNull() ? vgname : vgname = request["lvgroup"].asString();
        request["size"].isNull() ? size : size = request["size"].asInt();
        request["format"].isNull() || (fsfmt = request["format"].asString()).empty();
        // 非法参数
        if (lvname.empty() || vgname.empty() || 0 == size)
        {
                sprintf(errinfo, "illegal param: vgname=%s lvname=%s size=%d type=%d", 
                        vgname.c_str(), lvname.c_str(), size, type);
                LOG_ERR("CreateLv(): %s", errinfo);
                return RequestError(errinfo);
        }

        // 正确参数
        int fs = FsOper::FS_MIN;
        fsfmt = StrLower(fsfmt);
        if (fsfmt == "ext2")
        {
                fs = FsOper::FS_EXT2;
        }
        else if (fsfmt == "ext3")
        {
                fs = FsOper::FS_EXT3;
        }
        else if (fsfmt == "ext4")
        {
                fs = FsOper::FS_EXT4;
        }
        else
        {
                fsfmt = "";
        }

        const char *mtOpt = "noatime,nodiratime,barrier=0,grpquota,usrquota";
        const char *fsOpt = "-b 4096";
        bool isCreate, isQuery, isMkfs, isMount, isQuota, isFstab;
        isCreate = isQuery = isMkfs = isMount = isQuota = isFstab = false;

        string lvPath = "/dev/" + vgname + "/" + lvname;
        string mtPath;// = MOUNT_PATH + vgname + "/" + lvname;

        //CLvConf conf;
        bool bRet = false;
        do
        {
                // 判断卷是否存在
                vector<stLvInfo> vl;
                if (RET_SUCCESS == GetLvSql(vl, lvname) && !vl.empty())
                {
                        sprintf(errinfo, "lv %s is already exist!", lvname.c_str());
                        LOG_ERR(errinfo);
                        break;
                }
                // 创建卷
                if (!LvmMgr::CreateLv(lvname, vgname, size)) {
                        sprintf(errinfo, "Create lv error on volume: %s", vgname.c_str());
                        LOG_ERR(errinfo);
                        break;
                }
                isCreate = true;

                if (fs != FS_MIN)
                {
                        mtPath = MOUNT_PATH + vgname + "/" + lvname;
                        // 文件系统
                        if (!Format(lvPath, fs, fsOpt)) {
                                sprintf(errinfo, "Mkfs(%s) with %s error", lvPath.c_str(), fsfmt.c_str());
                                LOG_ERR(errinfo);
                                break;
                        }
                        isMkfs = true;

                        // 创建挂载目录
                        if (!MkDir(mtPath)) {
                                sprintf(errinfo, "MkDir(%s) error", mtPath.c_str());
                                LOG_ERR(errinfo);
                                break;
                        }

                        // 挂载
                        if (!MountLv(lvPath, mtPath, fs, mtOpt)) {
                                sprintf(errinfo, "mount lv %s to %s error", lvPath.c_str(),
                                        mtPath.c_str());
                                LOG_ERR(errinfo);
                                break;
                        }
                        isMount = true;

                        // 加入 fstab
                        //const char* fsType = CFsOper::FS_STR[fs];
                        if (!AddToFstab(lvPath, mtPath, fsfmt.c_str(), mtOpt)) {
                                sprintf(errinfo, "Add2Tab lv (%s) to (%s) error",
                                        lvPath.c_str(), mtPath.c_str());
                                LOG_ERR(errinfo);
                                break;
                        }
                        isFstab = true;

                        // quota check
                        if (!QuotaCheck(mtPath)) {
                                sprintf(errinfo, "QuotaCheck(%s) error", mtPath.c_str());
                                LOG_ERR(errinfo);
                                break;
                        }

                        if (!QuotaOn(lvPath)) {
                                sprintf(errinfo, "QuotaOn(%s) error", lvPath.c_str());
                                LOG_ERR(errinfo);
                                break;
                        }
                        isQuota = true;

                }
                // 将卷记录加入数据库
                // type: 1 = 普通共享;2=消冗共享;3=FCSAN;4=IPSAN;
                if (0 != SetLvSql(stLvInfo(lvPath,mtPath,fsfmt,size,0), LVSQL_ADD))
                {
                        sprintf(errinfo, "SetLvSql(%s) error", lvPath.c_str());
                        LOG_ERR(errinfo);
                        break;
                }
                isQuery = true;
                bRet = true;
        } while (false);

        // 错误时的回滚操作
        if (!bRet) {
                if (isQuery) {
                        if (!SetLvSql(stLvInfo(lvPath), LVSQL_DEL)) 
                        {
                                LOG_ERR("SetLvSql(%s) error, type (%s)", lvPath.c_str(), LvmMgr::LV_STR[type]);
                        }
                }

                if (fs != FS_MIN)
                {
                        if (isQuota || isFstab) {
                                if (!RmToFstab(lvPath)) {
                                        LOG_ERR("Rem2Tab(%s) error", lvPath.c_str());
                                }
                        }

                        if (isMount) {
                                if (!UmountLv(lvPath)) {
                                        LOG_ERR("UmountLv(%s) error", lvPath.c_str());
                                }

                                if (!RmDir(mtPath)) {
                                        LOG_ERR("RmDir(%s) error", mtPath.c_str());
                                }
                        }
                }

                if (isCreate || isMkfs) {
                        LvmMgr::DeleteLv(lvPath);
                }
                return RequestError(errinfo);
        }
        else {
                return RequestOk(respond);
        }

        // user log (in PHP)
        return RequestOk(respond);
}

Packet *DiskJsonMgr::ModifyLv(const Packet *p)
{
        LOG_DBG("ModifyLv()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        string lvpath;
        int size = 0;
        request["lv_name"].isNull() ? lvpath : lvpath = request["lv_name"].asString();
        request["extendspace"].isNull() ? 0 : sscanf(request["extendspace"].asString().c_str(), "%d", &size);
        // lvm 扩展
        if (!LvmMgr::ExtentLv(lvpath, size))
        {
                return RequestError("extend lv error");
        }
        // 根据使用方式
        // 文件系统扩展
        if (!ResizeFs(lvpath))
        {
                LOG_ERR("ModifyLv(): resize false. lv=%s", lvpath.c_str());
                return RequestError("resizefs lv error");
        }
        // 数据库记录更新
        stLvInfo li;
        li.path = lvpath;
        if (!LvmMgr::GetLvSize(lvpath, li.size))
        {
                LOG_ERR("ModifyLv(): cannot get lv size. lv=%s", lvpath.c_str());
        }
        if (!SetLvSql(li, LVSQL_SETSIZE))
        {
                LOG_ERR("ModifyLv(): update lv size error. lv=%s", lvpath.c_str());
        }
        return RequestOk(respond);
}

Packet *DiskJsonMgr::DeleteLv(const Packet *p)
{
        LOG_DBG("DeleteLv()\n");
        int ret = 0;
        Json::Value request, respond;
        ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        string lvlist;
        vector<string> vl;
        //request["lvlist"].isNull() ? lvlist : lvlist = request["lvlist"].asString();
        for (uint i=0; i<request["lvlist"].size(); ++i)
        {
                request["lvlist"][i]["path"].isNull() ? lvlist : lvlist = request["lvlist"][i]["path"].asString();
                if (lvlist.empty())
                {
                        continue;
                }
                vl.push_back(lvlist);
        }

        //Split(lvlist.c_str(), vl, ';');
        uint i = 0;
        char errinfo[1024] = {0};
        vector<stLvInfo> vli;
        for (i=0; i<vl.size(); ++i)
        {
                ret = RET_FAILED;
                if (vl[i].empty() || vl[i].length() < 6)
                {
                        continue;
                }
                // 判断可不可删除
                if (RET_SUCCESS != GetLvSql(vli, vl[i]) || vli.empty())
                {
                        LOG_ERR("DeleteLv(): lv not exist in sql: %s", vl[i].c_str());
                }
                // 判断是哪种使用方式
                if (!vli.empty() && vli[0].used == 0 && !vli[0].mount.empty())
                {

                        // 删除共享目录
                        //std::string mtPath = MOUNT_PATH + vl[i].substr(5, string::npos);
                        if (!(UmountLv(vli[0].mount)))
                        {
                                sprintf(errinfo, "cannot umount lv %s", vli[0].mount.c_str());
                                LOG_ERR("DeleteLv(): %s", errinfo);
                                break;
                        }
                        if (!(RmDir(vli[0].mount)))
                        {
                                LOG_ERR("DeleteLv(): cannot rm dir %s", vli[0].mount.c_str());
                        }
                }
                // 如果忙, 不可删除
                if (!vli.empty() && vli[0].used != 0)
                {
                        sprintf(errinfo, "cannot delete BUSY lv %s!", vli[0].path.c_str());
                        LOG_ERR("DeleteLv(): %s", errinfo);
                        break;
                }

                // 删除卷
                if (!LvmMgr::DeleteLv(vl[i]))
                {
                        sprintf(errinfo, "cannot delete lv: %s", vl[i].c_str());
                        LOG_ERR("DeleteLv(): %s", errinfo);
                        break;
                }
                // 删除数据库记录
                if (0 != SetLvSql(stLvInfo(vl[i],"","",0,0), LVSQL_DEL))
                {
                        LOG_WRN("DeleteLv(): cannot delete lv from db: %s", vl[i].c_str());
                        continue;
                }
                // 删除 fstab 记录
                if (!(RmToFstab(vl[i])))
                {
                        LOG_ERR("DeleteLv(): rm fstab error: %s", vl[i].c_str());
                }
                ret = RET_SUCCESS;
                LOG_DBG("DeleteLv(): delete lv %s success", vl[i].c_str());
        }
        if (RET_SUCCESS != ret)
        {
                //LOG_ERR(errinfo);
                return RequestError(errinfo);
        }
        else
        {
                return RequestOk(respond);
        }
}


