#include "BadRequestHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Utils/Json/include/json.h"

BadRequestHandler::BadRequestHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

BadRequestHandler::~BadRequestHandler()
{
}

int BadRequestHandler::Open(int sockfd)
{
        LOG_DBG("Local-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int BadRequestHandler::Read(Packet* p)
{
        LOG_DBG("Local-->Read()\n");
        return 0;
}

int BadRequestHandler::Write(Packet* p)
{
        LOG_DBG("Local-->Write()\n");
        Packet *retPack = BadRequestHandler::BadRequest("unknown request");
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;
        return 0;
}

int BadRequestHandler::Timeout()
{
        LOG_DBG("Local-->Timeout()\n");
        return 0;
}

int BadRequestHandler::Close()
{
        LOG_DBG("Local-->Close()\n");
        return 0;
}

Packet *BadRequestHandler::BadRequest(const string &info) {
        Json::Value respond;
        LOG_ERR("%s", info.c_str());
        respond["info"] = info;
        int len         = respond.toStyledString().size();
        Packet *pack    = (Packet *)new char[sizeof(Packet) + len];
        pack->Magic     = 'B';
        pack->Version   = '1';
        pack->Cmd       = CMD_ERR;
        pack->Length    = len;
        memcpy(pack->Data, respond.toStyledString().c_str(), pack->Length);
        return pack;
}
