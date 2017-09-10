#include "SysHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/SysInfo/SysInfoJsonMgr.h"

SysHandler::SysHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

SysHandler::~SysHandler()
{
}

int SysHandler::Open(int sockfd)
{
        LOG_DBG("SysInfo-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int SysHandler::Read(Packet* p)
{
        LOG_DBG("SysInfo-->Read()\n");
        return 0;
}

int SysHandler::Write(Packet* p)
{
        LOG_DBG("SysInfo-->Write()\n");

        SysInfoJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int SysHandler::Timeout()
{
        LOG_DBG("SysInfo-->Timeout()\n");
        return 0;
}

int SysHandler::Close()
{
        LOG_DBG("SysInfo-->Close()\n");
        return 0;
}
