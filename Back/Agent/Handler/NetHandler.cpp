#include "NetHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Lightev/Socket.h"
#include "Agent/Net/NetJsonMgr.h"
#include "Agent/Frame/IJsonMgr.h"

NetHandler::NetHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

NetHandler::~NetHandler()
{
}

int NetHandler::Open(int sockfd)
{
        LOG_DBG("Net-->Open()\n");

        return 0;
}

int NetHandler::Read(Packet* p)
{
        LOG_DBG("Net-->Read()\n");

        return 0;
}

int NetHandler::Write(Packet* p)
{
        LOG_DBG("Net-->Write()\n");

        NetJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int NetHandler::Timeout()
{
        LOG_ERR("Net-->Timeout()\n");
        return 0;
}

int NetHandler::Close()
{
        LOG_DBG("Net-->Close()\n");
        return 0;
}
