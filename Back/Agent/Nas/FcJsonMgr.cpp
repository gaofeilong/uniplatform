#include "Protocol.h"
#include "FcJsonMgr.h"
#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Utils/Shell/System.h"
#include "Agent/Disk/LvmMgr.h"
#include "Agent/Disk/FsOper.h"
#include "Agent/Disk/LvConf.h"

#include <stdio.h>
#include <algorithm>

using std::string;
using std::vector;
using namespace FsOper;
//static FcMgr fc;

FcJsonMgr::FcJsonMgr()
{
}

FcJsonMgr::~FcJsonMgr()
{
}

Packet *FcJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_NAS_FC_LOOKUP:
                return ListFcLun(p);
        case CMD_NAS_FC_MAP:
                return AddFcLun(p);
        case CMD_NAS_FC_UNMAP:
                return DelFcLun(p);
        }
        LOG_ERR("FcJsonMgr: unknown cmd: %08X", p->Cmd);
        return RequestError("unknown request");
}

Packet *FcJsonMgr::ListFcLun(const Packet *p)
{
        LOG_DBG("ListFcLun()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};

        // ----------------------------
        std::string cmd = "service scst status";
        int         rt  = ExeCmd(cmd.c_str());
        if (rt != 0)
        {
                ///return RequestError("FC 服务未启动或无法启动");
        }

        LvConf conf;
        std::vector<FcMgr::Target>  tars;
        std::vector<std::string>    cards;
        std::string confPath = CONF_FCCONF;
        vector<stLvInfo> vli;
        std::string target = request["tar"].isNull() ? string() : request["tar"].asString();
        uint nTar = NOT_FIND;
/*
        if (!fc.WriteConfig(confPath)) {
                sprintf(errinfo, "WriteConfig error");
                LOG_ERR("ListFcLun(): %s", errinfo);
                ///return RequestError(errinfo);
        }
        else if (!fc.ListDevice(tars)) {
                sprintf(errinfo, "ListDevice error");
                ///return RequestError(errinfo);
        }
        else*/ if (RET_SUCCESS != GetLvSql(vli, LVPATH_FREE))
        {
                sprintf(errinfo, "get unmap lun failed");
                LOG_ERR("ListFcLun(): %s", errinfo);
                return RequestError(errinfo);
        }
        else {
                // testing json info
                tars.push_back(FcMgr::Target("HELLO:HELLO:HELLO"));
                tars.push_back(FcMgr::Target("WORLD:WORLD:WORLD"));
                tars.push_back(FcMgr::Target("GANG:GANG:GANG"));
                tars.push_back(FcMgr::Target("QWERTY:QWERTY:QWERTY"));
                tars[1].luns.push_back(FcMgr::Lun("/dev/volume00/bbbb", "/dev/smKITTY", 0));
                tars[1].luns.push_back(FcMgr::Lun("/dev/volume00/cccc", "/dev/smHELLO", 1));
                tars[1].luns.push_back(FcMgr::Lun("/dev/volume00/aaaa", "/dev/smKITTY", 3));
                tars[3].luns.push_back(FcMgr::Lun("/dev/volume00/dddd", "/dev/smHELLO", 0));
                tars[3].luns.push_back(FcMgr::Lun("/dev/volume00/eeee", "/dev/smHELLO", 2));
                tars[2].luns.push_back(FcMgr::Lun("/dev/volume00/ffff", "/dev/smFOOL", 0));
                tars[2].luns.push_back(FcMgr::Lun("/dev/volume00/tttt", "/dev/smTEAR", 5));
                tars[2].luns.push_back(FcMgr::Lun("/dev/volume00/iiii", "/dev/smIDIOT", 2));
                // card
                respond["card"] = Json::Value();
                respond["lun"] = Json::Value();
                if (target.empty() && !tars.empty())
                {
                        target = tars[0].name;
                        nTar = 0;
                }
                respond["tar"] = target;
                
                //JSONNode card(JSON_ARRAY);
                //card.set_name("card");
                //uint size = tars.size();
                uint luncount = 0;
                for (uint i = 0; i < tars.size(); ++i)
                {
                        FcMgr::Target &tar = tars[i];

                        //JSONNode node;
                        //node.push_back(JSONNode("name", tar.name));
                        respond["card"][i]["name"] = tar.name;
                        //node.push_back(JSONNode("nextlun", tar.nextlun));
                        ///respond["card"][i]["nextlun"] = tar.nextlun;
                        respond["card"][i]["nextlun"] = tar.GetNextLun();

                        //JSONNode nodelun(JSON_ARRAY);
                        //nodelun.set_name("lun");
                        //respond["card"][i]["lun"] = Json::Value();
                        
                        if (target == tars[i].name)
                        {
                                nTar = i;
                        }
                        /*
                        for (uint j = 0; j < tar.luns.size(); ++j)
                        {
                                FcMgr::Lun &lun = tar.luns[j];
                                uint64_t size = 0;
                                if (!LvmMgr::GetLvSize(lun.path, size)) {
                                        LOG_ERR("Get lv(%s) size error", lun.path.c_str());
                                }
                                //nodelun.push_back(JSONNode("lun", buf));
                                //respond["card"][i]["lun"][j] = buf;
                                respond["lun"][luncount]["path"] = lun.path;
                                respond["lun"][luncount]["size"] = IntToString(size);
                                respond["lun"][luncount]["mapped"] = tar.name;
                                respond["lun"][luncount]["lunnum"] = lun.lun;
                                ++luncount;
                        }
                        */
                        ++tar.nextlun;
                        //node.push_back(nodelun);
                        //card.push_back(node);
                }
                if (nTar < tars.size())
                {
                        for (uint j = 0; j < tars[nTar].luns.size(); ++j)
                        {
                                FcMgr::Lun &lun = tars[nTar].luns[j];
                                uint64_t size = 0;
                                if (!LvmMgr::GetLvSize(lun.path, size)) {
                                        LOG_ERR("Get lv(%s) size error", lun.path.c_str());
                                }
                                //nodelun.push_back(JSONNode("lun", buf));
                                //respond["card"][i]["lun"][j] = buf;
                                respond["lun"][luncount]["path"] = lun.path;
                                vector<string> vname;
                                Split(lun.path.c_str(), vname, '/');
                                if (vname.size() > 3)
                                {
                                        respond["lun"][luncount]["lv"] = vname[3];
                                        respond["lun"][luncount]["vg"] = vname[2];
                                }
                                respond["lun"][luncount]["size"] = IntToString(size);
                                respond["lun"][luncount]["mapped"] = tars[nTar].name;
                                respond["lun"][luncount]["lunnum"] = lun.lun;
                                ++luncount;
                        }
                }
                for (uint i=0; i<vli.size(); ++i)
                {
                        respond["lun"][luncount]["path"] = vli[i].path;
                        respond["lun"][luncount]["lv"] = vli[i].name;
                        respond["lun"][luncount]["vg"] = vli[i].vgname;
                        respond["lun"][luncount]["size"] = IntToString(vli[i].size);
                        respond["lun"][luncount]["mapped"] = "";
                        ++luncount;
                }

        }
        return RequestOk(respond);
}

Packet *FcJsonMgr::AddFcLun(const Packet *p)
{
        LOG_DBG("AddFcLun()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        LvConf conf;
        std::string confPath = CONF_FCCONF;
        bool bRet = false;
        //std::vector<FcMgr::Target>  mappedTa, newTa, vDel;
        FcMgr::Target oTa, nTa;
        //std::vector<std::string>    cards;
        vector<stLvInfo> vli;
        string target = request["tar"].isNull() ? string() : request["tar"].asString();
        if (target.empty())
        {
                sprintf(errinfo, "no target in packet!");
                LOG_ERR("AddFcLun(): %s", errinfo);
                return RequestError(errinfo);
        }
        oTa.name = nTa.name = target;
        //return RequestError(errinfo);

        // 来自页面的上次映射的lun
        for (uint i=0; i<request["mapped"].size(); ++i)
        {
                string mappedStr = request["mapped"][i].isNull() ? string() : request["mapped"][i].asString();
                if (mappedStr.empty())
                {
                        continue;
                }
                vector<string> vs;
                Split(mappedStr.c_str(), vs, ',');
                if (vs.size() < 3 || vs[1].empty() || vs[2].empty())
                {
                        continue;
                }
                //oTa.name = vs[2];
                oTa.luns.push_back(FcMgr::Lun(atoi(vs[0].c_str()), vs[1]));
                LOG_DBG("oTa -> lun=[%s] luns.size=%lu", mappedStr.c_str(), oTa.luns.size());
        }
        // 来自页面要新建的lun
        for (uint i=0; i<request["newmap"].size(); ++i)
        {
                string mapStr = request["newmap"][i].isNull() ? string() : request["newmap"][i].asString();
                if (mapStr.empty())
                {
                        continue;
                }
                vector<string> vs;
                Split(mapStr.c_str(), vs, ',');
                if (vs.size() < 2 || vs[1].empty())
                {
                        continue;
                }
                //int nNextLun = oTa.GetNextLun();
                nTa.luns.push_back(FcMgr::Lun(atoi(vs[0].c_str()), vs[1]));
                LOG_DBG("nTa -> lun=[%s] luns.size=%lu", mapStr.c_str(), nTa.luns.size());
        }
        bRet = true;
        // 选择出要删除的映射,要新建的映射.
        vector<FcMgr::Lun> vlDel, vlAdd, vlSum;
        AddMem(vlSum, oTa.luns);
        AddMem(vlSum, nTa.luns);
        vlDel = vlAdd = vlSum;
        DelMem(vlAdd, oTa.luns);
        DelMem(vlDel, nTa.luns);
        LOG_DBG("vlDel.size=%lu vlAdd.size=%lu", vlDel.size(), vlAdd.size());
        // 执行删除的 lun
        for (uint i=0; i<vlDel.size(); ++i)
        {
                //LOG_DBG("DEL: %lu,%s i=%u", vlDel[i].lun, vlDel[i].path.c_str(), i);
                vlDel[i].devname = fc.SetName(oTa.name, vlDel[i].lun);
                if (vlDel[i].lun == 0 && nTa.luns.size()>1 && vlAdd.empty()) {
                        sprintf(errinfo, "Cannot delete lun when lun=0 and members count > 1 in target(%s)!", oTa.name.c_str());
                        LOG_ERR("AddFcLun(): %s", errinfo);
                        bRet = false;
                        break;
                }/*
                else if (!fc.DeleteLun(vlDel[i].lun, oTa.name, vlDel[i].devname)) {
                        sprintf(errinfo, "Cannot delete lun: %d,%s,%s", vlDel[i].lun, vlDel[i].path.c_str(), oTa.name.c_str());
                        LOG_ERR("AddFcLun(): %s", errinfo);
                        bRet = false;
                        break;
                }
                else if(!fc.DeleteDevice(vlDel[i].devname)) {
                        sprintf(errinfo, "Cannot delete device: dev=%s lun=%d,%s,%s", vlDel[i].devname.c_str(), vlDel[i].lun, vlDel[i].path.c_str(), oTa.name.c_str());
                        LOG_ERR("AddFcLun(): %s", errinfo);
                        bRet = false;
                        break;
                }*/
                else if (RET_SUCCESS != SetLvSql(stLvInfo(vlDel[i].path, oTa.name, LVUSED_FC), LVSQL_FREEFS)) {
                        sprintf(errinfo, "SetLvSql failed! lun=%d,%s,%s", vlDel[i].lun, vlDel[i].path.c_str(), oTa.name.c_str());
                        LOG_ERR("AddFcLun(): %s", errinfo);
                        bRet = false;
                        break;
                }
                else {
                //LOG_DBG("SUCCESS DEL: %lu,%s i=%u", vlDel[i].lun, vlDel[i].path.c_str(), i);
                        oTa.luns.erase(oTa.luns.begin() + FindMem(oTa.luns, vlDel[i]));
                        bRet = true;
                        continue;
                }
        }
        if (bRet)
        {
                // 执行添加的 lun
                for (uint i=0; i<vlAdd.size(); ++i)
                {
                        //LOG_DBG("ADD: %lu,%s", vlAdd[i].lun, vlAdd[i].path.c_str());
                        // 空名称过滤
                        if (vlAdd[i].path.empty())
                        {
                                continue;
                        }
                        // 数据库记录存在检测
                        if (RET_SUCCESS != GetLvSql(vli, vlAdd[i].path) && vli.empty())
                        {
                                LOG_ERR("AddFcLun(): cannot get lv info from sql: %s", vlAdd[i].path.c_str());
                                continue;
                        }
                        // 有mount的, umount, 并去掉fstab记录
                        if (!vli[0].mount.empty())
                        {
                                if (!UmountLv(vli[0].path))
                                {
                                        LOG_ERR("AddFcLun(): cannot umount lv: %s", vli[0].path.c_str());
                                        continue;
                                }
                                RmToFstab(vli[0].path);
                        }
                        // 条件合格, 执行添加
                        vlAdd[i].devname = fc.SetName(oTa.name, vlAdd[i].lun);
                        vlAdd[i].lun = oTa.GetNextLun();
                        /*
                        if (!fc.AddDevice(vlAdd[i].devname, vlAdd[i].path)) {
                                sprintf(errinfo, "Cannot add device! dev=%s lun=%d,%s,%s", vlAdd[i].devname.c_str(), vlAdd[i].lun, vlAdd[i].path.c_str(), oTa.name.c_str());
                                LOG_ERR("AddFcLun(): %s", errinfo);
                                bRet = false;
                                break;
                                //return RequestError(errinfo);
                        }
                        else if(!fc.AddLun(vlAdd[i].lun, oTa.name, vlAdd[i].devname)) {
                                if (!fc.DeleteDevice(vlAdd[i].devname)) {
                                        sprintf(errinfo, "DeleteFc error! lun=%d,%s,%s", vlAdd[i].lun, vlAdd[i].path.c_str(), oTa.name.c_str());
                                        LOG_ERR("AddFcLun(): %s", errinfo);
                                }
                                sprintf(errinfo, "Cannot add lun! lun=%d,%s,%s", vlAdd[i].lun, vlAdd[i].path.c_str(), oTa.name.c_str());
                                LOG_ERR("AddFcLun(): %s", errinfo);
                                bRet = false;
                                break;
                        }
                        else if (!fc.EnableTarget(oTa.name)) {
                                fc.DeleteLun(vlAdd[i].lun, oTa.name, vlAdd[i].devname);
                                fc.DeleteDevice(vlAdd[i].devname);
                                sprintf(errinfo, "Cannot enable target! lun=%d,%s,%s", vlAdd[i].lun, vlAdd[i].path.c_str(), oTa.name.c_str());
                                LOG_ERR("AddFcLun(): %s", errinfo);
                                bRet = false;
                                break;
                        }
                        else */if (RET_SUCCESS != SetLvSql(stLvInfo(vlDel[i].path, oTa.name, LVUSED_FC), LVSQL_SETFS)) {
                                sprintf(errinfo, "SetLvSql failed! lun=%d,%s,%s", vlDel[i].lun, vlDel[i].path.c_str(), oTa.name.c_str());
                                LOG_ERR("AddFcLun(): %s", errinfo);
                                bRet = false;
                                break;
                        }
                        else {
                        //LOG_DBG("SUCCESS ADD: %lu,%s", vlAdd[i].lun, vlAdd[i].path.c_str());
                                bRet = true;
                                oTa.luns.push_back(vlAdd[i]);
                                continue;
                                //return RequestOk(respond);
                        }
                }
        }
        // 服务写入服务的配置
        /*
        if (!fc.WriteConfig(confPath)) {
                sprintf(errinfo, "writeConfig error! target=%s", oTa.name.c_str());
                LOG_ERR(errinfo);
                bRet = false;
        }*/
        if (bRet)
        {
                return RequestOk(respond);
        }
        return RequestError(errinfo);
}
// 此函数未使用,功能包含于 AddFcLun()
Packet *FcJsonMgr::DelFcLun(const Packet *p)
{
        LOG_DBG("DelFcLun()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        std::string path, card;
        int         lun = -1;

        path = request["path"].isNull() ? string() : request["path"].asString();
        card = request["card"].isNull() ? string() : request["card"].asString();
        request["lun"].isNull() ? -1 : sscanf(request["lun"].asString().c_str(), "%d", &lun);

        //bool    bRet = false;
        //CFcOper fc;
        LvConf conf;
        std::string confPath = CONF_FCCONF;

        std::string dev = fc.SetName(card, lun);
        //outpack.m_type = CPacket::PACKET_ERR;

        if (path.size() <= 0 || (card.size() <= 0 && lun == -1)) {
                sprintf(errinfo, "node is not exist");
                LOG_ERR(errinfo);
                //return RequestError(errinfo);
        }
        else if (!fc.DeleteLun(lun, card, dev)) {
                sprintf(errinfo, "DelDevice error");
                LOG_ERR(errinfo);
                //return RequestError(errinfo);
        }
        else if(!fc.DeleteDevice(dev)) {
                
                sprintf(errinfo, "DeleteDevice failure");
                LOG_ERR(errinfo);
                //outpack.m_info += "DeleteDevice failure";
        }
        else if (!conf.AddLvConf(path, LvConf::LV_FC, false)) {
                sprintf(errinfo, "AddLvConf error");
                LOG_ERR(errinfo);
                //outpack.m_info = "AddLvConf to false error";
        }
        else if (!fc.WriteConfig(confPath)) {
                sprintf(errinfo, "WriteConfig error");
                LOG_ERR(errinfo);
                //outpack.m_info = "WriteConfig error";
        }
        else {
                RequestOk(respond);
        }
        return RequestError(errinfo);
}

