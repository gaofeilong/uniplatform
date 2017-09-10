#include "NasJsonMgr.h"
#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "IscsiJsonMgr.h"
#include "FcJsonMgr.h"
#include "AuthJsonMgr.h"
#include "Agent/Nas/Share/Smb.h"
#include <stdio.h>

using std::string;
using std::vector;

NasJsonMgr::NasJsonMgr()
{
}

NasJsonMgr::~NasJsonMgr()
{
}

Packet *NasJsonMgr::Handle(const Packet *p)
{
        if ((p->Cmd & 0xFFFFF000) == CMD_NAS_ISCSI)
        {
                return IscsiJsonMgr().Handle(p);
        }
        else if ((p->Cmd & 0xFFFFF000) == CMD_NAS_FC)
        {
                return FcJsonMgr().Handle(p);
        }
        else if ((p->Cmd & 0xFFFFF000) == CMD_NAS_AUTH)
        {
                return AuthJsonMgr().Handle(p);
        }
        else if ((p->Cmd & 0xFFFFF000) == CMD_NAS_SERVICE)
        {
                //return ServiceJsonMgr().Handle(p);
        }

        switch(p->Cmd) {
        case CMD_NAS_SHARE_LIST:
                return ListShare(p);
        }
        LOG_ERR("NasJsonMgr: unknown cmd: %08X", p->Cmd);
        return RequestError("unknown request");
}

Packet *NasJsonMgr::ListShare(const Packet *p)
{
        LOG_DBG("ListShare()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        vector<string> vt;
        vector<stShare> vd;
        char errinfo[256] = {0};
        if (RET_SUCCESS != CServiceAccess::GetShSql(vd))
        {
                sprintf(errinfo, "cannot Load service.");
                LOG_ERR("ListShare(): %s", errinfo);
                return RequestError(errinfo);
        }
        respond["list"] = Json::Value();
        for (uint i=0; i<vd.size(); ++i)
        {
                respond["list"][i] = vd[i].name;
        }

        return RequestOk(respond);
}

