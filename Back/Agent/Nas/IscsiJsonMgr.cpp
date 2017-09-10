#include "Protocol.h"
#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "IscsiJsonMgr.h"
#include "Agent/Disk/LvmMgr.h"
#include "Agent/Disk/FsOper.h"
#include <stdio.h>

using std::string;
using std::vector;
using namespace FsOper;
//static CIscsiTarget conf;

IscsiJsonMgr::IscsiJsonMgr()
{
}

IscsiJsonMgr::~IscsiJsonMgr()
{
}

Packet *IscsiJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_NAS_ISCSI_TARGET_LOOKUP:
                return ListTarget(p);
        case CMD_NAS_ISCSI_TARGET_ADD:
                return AddTarget(p);
        case CMD_NAS_ISCSI_TARGET_DEL:
                return DelTarget(p);
        case CMD_NAS_ISCSI_MAP:
                return MapTarget(p);
        case CMD_NAS_ISCSI_UNMAP:
                return UnmapTarget(p);
        case CMD_NAS_ISCSI_SHOWMAP:
                return ShowMap(p);
        case CMD_NAS_ISCSI_SHOWUNMAP:
                return ShowUnmap(p);
        case CMD_NAS_ISCSI_CHAP_ADD:
                return AddChapUser(p);
        case CMD_NAS_ISCSI_CHAP_DEL:
                return DelChapUser(p);
        case CMD_NAS_ISCSI_CHAP_GET:
                return GetChapUser(p);
        case CMD_NAS_ISCSI_GETALLOW:
                return GetAllowHosts(p);
        case CMD_NAS_ISCSI_SETALLOW:
                return SetAllowHosts(p);
        }
        LOG_ERR("IscsiJsonMgr: unknown cmd: %08X", p->Cmd);
        return RequestError("unknown request");
}

Packet *IscsiJsonMgr::ListTarget(const Packet *p)
{
        LOG_DBG("ListTarget()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        vector<string> vt;
        char errinfo[256] = {0};
        if (RET_SUCCESS != conf.GetAllTargetIQN(vt))
        {
                sprintf(errinfo, "cannot GetAllTargetIQN");
                LOG_ERR("ListTarget(): %s", errinfo);
                return RequestError(errinfo);
        }
        respond["targetlist"] = Json::Value();
        for (uint i=0; i<vt.size(); ++i)
        {
                respond["targetlist"][i] = vt[i];
        }
        if (vt.size() > 0)
        {
                respond["iqn"] = vt[0];
        }

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::AddTarget(const Packet *p)
{
        LOG_DBG("AddTarget()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
/*
        stIscsiTarget it;
        it.targetIQN = request["iqn"].isNull() ? string() : request["iqn"].asString();
        request["HeaderDigest"].isNull() ? ISCSITARGET_HEAD_DEF : sscanf(request["HeaderDigest"].asString().c_str(), "%d", &(it.HeaderDigest));
        request["DataDigest"].isNull() ? ISCSITARGET_DATA_DEF : sscanf(request["DataDigest"].asString().c_str(), "%d", &(it.DataDigest));
        request["InitialR2T"].isNull() ? ISCSITARGET_INIT_DEF : sscanf(request["InitialR2T"].asString().c_str(), "%d", &(it.InitialR2T));
        request["ImmediateData"].isNull() ? ISCSITARGET_IMME_DEF : sscanf(request["ImmediateData"].asString().c_str(), "%d", &(it.ImmediateData));
*/
        if (RET_SUCCESS != conf.AddIscsiTarget(iqn))
        {
                sprintf(errinfo, "cannot add iqn: %s", iqn.c_str());
                LOG_ERR("AddTarget(): %s", errinfo);
                return RequestError(errinfo);
        }

        conf.LoadIn();

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::DelTarget(const Packet *p)
{
        LOG_DBG("DelTarget()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        if (RET_SUCCESS != conf.DelIscsiTarget(iqn))
        {
                sprintf(errinfo, "cannot del iqn: %s", iqn.c_str());
                LOG_ERR("DelTarget(): %s", errinfo);
                return RequestError(errinfo);
        }
        if (RET_SUCCESS != conf.DelTargetFromAllowHosts(iqn.c_str()))
        {
                sprintf(errinfo, "cannot del iqn from allowhosts: %s", iqn.c_str());
                LOG_ERR("DelTarget(): %s", errinfo);
                return RequestError(errinfo);
        }

        conf.LoadIn();

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::MapTarget(const Packet *p)
{
        LOG_DBG("MapTarget()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        vector<stLunMap> vlm, vom, vam, vdm;
        vector<stLvInfo> vli;
        string iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        for (uint i=0; i<request["lun"].size(); ++i)
        {
                stLunMap newlm;
                newlm.path = request["lun"][i]["path"].isNull() ? string() : request["lun"][i]["path"].asString();
                request["lun"][i]["type"].isNull() ? 0 : sscanf(request["lun"][i]["type"].asString().c_str(), "%d", &newlm.type);
                request["lun"][i]["iomode"].isNull() ? 0 : sscanf(request["lun"][i]["iomode"].asString().c_str(), "%d", &newlm.iomode);
                request["lun"][i]["blockSize"].isNull() ? 512 : sscanf(request["lun"][i]["blockSize"].asString().c_str(), "%d", &newlm.blockSize);
                newlm.LunNum = NOT_FIND;
                // 空名称过滤
                if (newlm.path.empty())
                {
                        continue;
                }
                // 存在检测
                if (RET_SUCCESS != GetLvSql(vli, newlm.path) || vli.empty())
                {
                        LOG_ERR("MapTarget(): cannot get lv info from sql: %s", newlm.path.c_str());
                        continue;
                }
                // 如果是别的用途的卷,不能进行map操作.
                stLvInfo& li = vli[vli.size()-1];
                if (li.used != LVUSED_NONE && li.used != LVUSED_ISCSI)
                {
                        LOG_ERR("MapTarget(): lv %s is busy for %s(%s)", vli[0].path.c_str(), vli[0].fsfmt.c_str(), vli[0].mount.c_str());
                        continue;
                }
                // 有mount的, umount, 并去掉fstab记录
                if (li.used == LVUSED_NONE && !li.mount.empty())
                {
                        if (!UmountLv(li.path))
                        {
                                LOG_ERR("MapTarget(): cannot umount lv: %s", li.path.c_str());
                                continue;
                        }
                        RmToFstab(li.path);
                }
                vlm.push_back(newlm);
        }
        // 获取已经配置的
        if (RET_SUCCESS != conf.GetMap(iqn, vom))
        {
                sprintf(errinfo, "cannot get map lun in target(%s)", iqn.c_str());
                LOG_ERR("MapTarget(): %s", errinfo);
                return RequestError(errinfo);
        }

        //AddMem(va, vli);
        vam = vom;
        for (uint i=0; i<vam.size();)
        {
                int name_exist = 0;
                for (uint j=0; j<vlm.size(); ++j)
                {
                        if (vlm[j].path == vam[i].path)
                        {
                                name_exist = 1;
                                break;
                        }
                }
                if (!name_exist)
                {
                        vdm.push_back(vam[i]);
                        vam.erase(vam.begin() + i);
                        //LOG_ERR("---- %lu %s", vam[i].LunNum, vam[i].path.c_str());
                        continue;
                }
                ++i;
        }
        // 将新加入的卷,判断是否重复,添加到旧列表中,并自动增加旧列表的Lun号
        for (uint i=0; i<vlm.size(); ++i)
        {
                vlm[i].LunNum = NOT_FIND;
                uint nlun = 0;
                int zero_exist = 0;
                for (uint j=0; j<vam.size(); ++j)
                {
                        if (vam[j].LunNum > nlun)
                        {
                                nlun = vam[j].LunNum;
                        }
                        if (vlm[i].path == vam[j].path)
                        {
                                vlm[i].LunNum = vam[j].LunNum;
                        }
                        // 判断Lun=0是否存在,存在则添加最大号+1,不存在则添加0
                        if (0 == vam[j].LunNum)
                        {
                                zero_exist = 1;
                        }
                }
                if (NOT_FIND == vlm[i].LunNum)
                {
                        vlm[i].LunNum = zero_exist ? nlun+1 : 0;
                        vam.push_back(vlm[i]);
                }
        }

        //AddMem(va, vo);
        //vdm = vam;
        // 所有要操作的卷集合中,去掉新映射的集合,得出要删除的集合
        //DelMem(vdm, vlm);
        // 新映射的卷集合中,去掉已映射的,剩余新映射中需要添加的
        //DelMem(vli, vo);
        // 所有要操作的卷集合中,去掉要删除的集合,得出新映射的集合
        //DelMem(vam, vdm);
        // 要删除的卷,在数据库记录为空闲
        for (uint i=0; i<vdm.size(); ++i)
        {
                LOG_ERR("--的是 %lu %s", vdm[i].LunNum, vdm[i].path.c_str());
                if (RET_SUCCESS != SetLvSql(stLvInfo(vdm[i].path, iqn, LVUSED_ISCSI), LVSQL_FREEFS))
                {
                        sprintf(errinfo, "update mysql error for set fs free: %s", vdm[i].path.c_str());
                        LOG_ERR("MapTarget(): %s", errinfo);
                }
        }

        if (RET_SUCCESS != conf.MapIscsi(iqn, vam))
        {
                //conf.MapIscsi(iqn, vlm[i]);
                sprintf(errinfo, "map iscsi lun(%s) failed", iqn.c_str());
                LOG_ERR("MapTarget(): %s", errinfo);
                conf.LoadIn();
                return RequestError(errinfo);
        }
        // 要添加的卷,在数据库记录为已使用
        for (uint i=0; i<vam.size(); ++i)
        {
                LOG_ERR("++的是 %lu %s", vam[i].LunNum, vam[i].path.c_str());
                if (RET_SUCCESS != SetLvSql(stLvInfo(vam[i].path, iqn, LVUSED_ISCSI), LVSQL_SETFS))
                {
                        sprintf(errinfo, "update mysql error for set fs=iscsi: %s", vam[i].path.c_str());
                        LOG_ERR("MapTarget(): %s", errinfo);
                }
        }

        conf.LoadIn();

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::UnmapTarget(const Packet *p)
{
        LOG_DBG("UnmapTarget()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn, path;
        iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        path = request["path"].isNull() ? string() : request["path"].asString();

        if (RET_SUCCESS != conf.UnmapIscsi(iqn, path))
        {
                sprintf(errinfo, "unmap iscsi lun (%s:%s) failed", iqn.c_str(), path.c_str());
                LOG_ERR("UnmapTarget(): %s", errinfo);
                return RequestError(errinfo);
        }

        conf.LoadIn();

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::ShowMap(const Packet *p)
{
        LOG_DBG("ShowMap()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn, path;
        std::vector<stLunMap> vlm;
        iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();

        if (RET_SUCCESS != conf.GetMap(iqn, vlm))
        {
                sprintf(errinfo, "get map lun (%s) failed", iqn.c_str());
                LOG_ERR("ShowMap(): %s", errinfo);
                return RequestError(errinfo);
        }

        respond["iqn"] = iqn;
        respond["lun"] = Json::Value();
        uint i = 0, nLm = vlm.size();
        for (i=0; i<vlm.size(); ++i)
        {
                respond["lun"][i]["path"] = vlm[i].path;
                vector<string> vname;
                Split(vlm[i].path.c_str(), vname, '/');
                if (vname.size()>3)
                {
                        respond["lun"][i]["lv"] = vname[3];
                        respond["lun"][i]["vg"] = vname[2];
                }
                respond["lun"][i]["type"] = vlm[i].type;
                respond["lun"][i]["iomode"] = vlm[i].iomode;
                respond["lun"][i]["blockSize"] = vlm[i].blockSize;
                //respond["lun"][i]["ScsiSN"] = vlm[i].ScsiSN;
                //respond["lun"][i]["ScsiId"] = vlm[i].ScsiId;
                uint64_t lvsize = 0;
                if (!LvmMgr::GetLvSize(vlm[i].path, lvsize))
                {
                        LOG_WRN("ShowMap(): cannot get size of %s", vlm[i].path.c_str());
                }
                respond["lun"][i]["size"] = IntToString(lvsize);
                respond["lun"][i]["mapped"] = 1;
        }
        vector<stLvInfo> vli;
        //if (RET_SUCCESS != conf.GetUnmapLun(vl))
        if (RET_SUCCESS != GetLvSql(vli, LVPATH_FREE))
        {
                sprintf(errinfo, "get unmap lun failed");
                LOG_ERR("ShowUnmap(): %s", errinfo);
                return RequestError(errinfo);
        }

        for (i=0; i<vli.size(); ++i)
        {
                respond["lun"][i+nLm]["path"] = vli[i].path;
                respond["lun"][i+nLm]["lv"] = vli[i].name;
                respond["lun"][i+nLm]["vg"] = vli[i].vgname;
                respond["lun"][i+nLm]["size"] = IntToString(vli[i].size);
                respond["lun"][i+nLm]["mapped"] = 0;
        }

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::ShowUnmap(const Packet *p)
{
        LOG_DBG("ShowUnmap()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        //vector<string> vl;
        vector<stLvInfo> vli;
        //if (RET_SUCCESS != conf.GetUnmapLun(vl))
        if (RET_SUCCESS != GetLvSql(vli, LVPATH_FREE))
        {
                sprintf(errinfo, "get unmap lun failed");
                LOG_ERR("ShowUnmap(): %s", errinfo);
                return RequestError(errinfo);
        }

        respond["unmaplun"] = Json::Value();
        for (uint i=0; i<vli.size(); ++i)
        {
                respond["unmaplun"][i]["path"] = vli[i].path;
                respond["unmaplun"][i]["size"] = IntToString(vli[i].size);
        }

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::AddChapUser(const Packet *p)
{
        LOG_DBG("AddChapUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn, user, passwd;
        int usertype = ISCSI_INCOMING;
        iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        user = request["user"].isNull() ? string() : request["user"].asString();
        passwd = request["pwd"].isNull() ? string() : request["pwd"].asString();
        usertype = request["type"].isNull() ? ISCSI_INCOMING : atoi(request["type"].asString().c_str());

        if (RET_SUCCESS != conf.AddChapUser(iqn, user, passwd, usertype))
        {
                sprintf(errinfo, "add chap user (%s:%s) failed", iqn.c_str(), user.c_str());
                LOG_ERR("AddChapUser(): %s", errinfo);
                return RequestError(errinfo);
        }

        conf.LoadIn();

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::DelChapUser(const Packet *p)
{
        LOG_DBG("DelChapUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn, user;
        int usertype = ISCSI_INCOMING;
        iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        user = request["users"].isNull() ? string() : request["users"].asString();
        usertype = request["type"].isNull() ? ISCSI_INCOMING : atoi(request["type"].asString().c_str());

        vector<string> vs;
        Split(user.c_str(), vs, ';', " ", 0);
        for (uint i=0; i<vs.size(); ++i)
        {
                if (RET_SUCCESS != conf.DelChapUser(iqn, vs[i], usertype))
                {
                        sprintf(errinfo, "del chap user (%s:%s) failed", iqn.c_str(), user.c_str());
                        LOG_WRN("DelChapUser(): %s", errinfo);
                        return RequestError(errinfo);
                }
        }
        conf.LoadIn();

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::GetChapUser(const Packet *p)
{
        LOG_DBG("GetChapUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        string iqn;
        std::map<std::string, std::string> vinc, vout;
        iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();

        if (RET_SUCCESS != conf.GetChapUserList(iqn, vinc, ISCSI_INCOMING))
        {
                sprintf(errinfo, "get chap IncomingUser failed: %s", iqn.c_str());
                LOG_ERR("GetChapUser(): %s", errinfo);
                return RequestError(errinfo);
        }
        if (RET_SUCCESS != conf.GetChapUserList(iqn, vout, ISCSI_OUTGOING))
        {
                sprintf(errinfo, "get chap OutgoingUser failed: %s", iqn.c_str());
                LOG_ERR("GetChapUser(): %s", errinfo);
                return RequestError(errinfo);
        }

        respond["iqn"] = iqn;
        respond["incominguser"] = Json::Value();
        std::map<std::string, std::string>::const_iterator itinc = vinc.begin();
        for (uint i=0; itinc != vinc.end(); ++itinc, ++i)
        {
                respond["incominguser"][i]["username"] = itinc->first;
                respond["incominguser"][i]["passwd"] = itinc->second;
        }

        respond["outgoinguser"] = Json::Value();
        std::map<std::string, std::string>::const_iterator itout = vout.begin();
        for (uint i=0; itout != vout.end(); ++itout, ++i)
        {
                respond["outgoinguser"][i]["username"] = itout->first;
                respond["outgoinguser"][i]["passwd"] = itout->second;
        }

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::GetAllowHosts(const Packet *p)
{
        LOG_DBG("GetAllowHosts()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        vector<string> va;
        int isAllowAll = 0;
        string iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        if (RET_SUCCESS != conf.GetAllowHosts(iqn, va, isAllowAll))
        {
                sprintf(errinfo, "cannot get allow host of iqn: %s", iqn.c_str());
                LOG_ERR("GetAllowHosts(): %s", errinfo);
                return RequestError(errinfo);
        }

        respond["allowhosts"] = Json::Value();
        for (uint i=0; i<va.size(); ++i)
        {
                respond["allowhosts"][i] = va[i];
        }
        respond["isAllowAll"] = isAllowAll;

        return RequestOk(respond);
}

Packet *IscsiJsonMgr::SetAllowHosts(const Packet *p)
{
        LOG_DBG("SetAllowHosts()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        vector<string> va;
        int isAllowAll = 0;
        string iqn = request["iqn"].isNull() ? string() : request["iqn"].asString();
        request["isAllowAll"].isNull() ? 0 : sscanf(request["isAllowAll"].asString().c_str(), "%d", &isAllowAll);
        for (uint i=0; i<request["allowhosts"].size(); ++i)
        {
                va.push_back(request["allowhosts"][i].asString());
        }

        if (RET_SUCCESS != conf.SetAllowHosts(iqn, va, isAllowAll))
        {
                sprintf(errinfo, "cannot del iqn: %s", iqn.c_str());
                LOG_ERR("SetAllowHosts(): %s", errinfo);
                return RequestError(errinfo);
        }

        return RequestOk(respond);
}
