#include <stdio.h>
#include <string.h>

#include "IJsonMgr.h"
#include "Utils/Log/Log.h"
#include "Include/Protocol.h"
#include "Server/Handler/BadRequestHandler.h"

size_t IJsonMgr::JsonSize(Json::Value j)
{
        return j.empty()? 0: j.toStyledString().size();
}

int IJsonMgr::JsonValue(const Packet *p, Json::Value& request)
{
        Json::Reader reader;
        if (!reader.parse(p->Data, request)) {
                return -1;
        }
        LOG_DBG("Received: %c, %c, 0x%08X, %d\n%s", p->Magic, p->Version,
                           p->Cmd, p->Length, request.toStyledString().c_str());
        return 0;
}

Packet *IJsonMgr::OnRequest(const Json::Value &respond, int cmd) {
        Packet *pack = (Packet *)new char[sizeof(Packet) + JsonSize(respond)];
        SetPackHead(pack, 'B', '1', cmd, JsonSize(respond));
        memcpy(pack->Data, respond.toStyledString().c_str(), pack->Length);
        LOG_DBG("Sended: %c, %c, 0x%08X, %d\n%s", pack->Magic, pack->Version,
                pack->Cmd, pack->Length, respond.toStyledString().c_str());
        return pack;
}

Packet *IJsonMgr::RequestOk(const Json::Value &respond)
{
        return OnRequest(respond, CMD_OK);
}

Packet *IJsonMgr::RequestError(const std::string &errInfo)
{
        return BadRequestHandler::BadRequest(errInfo);
}

void IJsonMgr::SetPackHead(Packet *p, char m, char v, int c, int l)
{
        p->Magic   = m;
        p->Version = v;
        p->Cmd     = c;
        p->Length  = l;
}

