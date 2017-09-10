#include "ArchiveHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/Archive/ArchiveJsonMgr.h"

ArchiveHandler::ArchiveHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

ArchiveHandler::~ArchiveHandler()
{
}

int ArchiveHandler::Open(int sockfd)
{
        LOG_DBG("Archive-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int ArchiveHandler::Read(Packet* p)
{
        LOG_DBG("Archive-->Read()\n");
        return 0;
}

int ArchiveHandler::Write(Packet* p)
{
        LOG_DBG("Archive-->Write()\n");

        ArchiveJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int ArchiveHandler::Timeout()
{
        LOG_DBG("Archive-->Timeout()\n");
        return 0;
}

int ArchiveHandler::Close()
{
        LOG_DBG("Archive-->Close()\n");
        return 0;
}
