#include "IstriHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/Istri/IstriJsonMgr.h"

IstriHandler::IstriHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

IstriHandler::~IstriHandler()
{
}

int IstriHandler::Open(int sockfd)
{
        LOG_DBG("IstriInfo-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int IstriHandler::Read(Packet* p)
{
        LOG_DBG("IstriInfo-->Read()\n");
        return 0;
}

int IstriHandler::Write(Packet* p)
{
        LOG_DBG("IstriInfo-->Write()\n");

        IstriJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int IstriHandler::Timeout()
{
        LOG_DBG("IstriInfo-->Timeout()\n");
        return 0;
}

int IstriHandler::Close()
{
        LOG_DBG("IstriInfo-->Close()\n");
        return 0;
}
