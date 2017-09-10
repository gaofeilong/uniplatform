#include "DiskHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/Disk/DiskJsonMgr.h"

DiskHandler::DiskHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

DiskHandler::~DiskHandler()
{
}

int DiskHandler::Open(int sockfd)
{
        LOG_DBG("Disk-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int DiskHandler::Read(Packet* p)
{
        LOG_DBG("Disk-->Read()\n");
        return 0;
}

int DiskHandler::Write(Packet* p)
{
        LOG_DBG("Disk-->Write()\n");
        int ret = 0;
        DiskJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return ret;
}

int DiskHandler::Timeout()
{
        LOG_DBG("Disk-->Timeout()\n");
        return 0;
}

int DiskHandler::Close()
{
        LOG_DBG("Disk-->Close()\n");
        return 0;
}
