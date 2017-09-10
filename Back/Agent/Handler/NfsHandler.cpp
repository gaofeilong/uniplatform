#include "NfsHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/Nfs/NfsJsonMgr.h"

NfsHandler::NfsHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

NfsHandler::~NfsHandler()
{
}

int NfsHandler::Open(int sockfd)
{
        LOG_DBG("NFS-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int NfsHandler::Read(Packet* p)
{
        LOG_DBG("NFS-->Read()\n");
        return 0;
}

int NfsHandler::Write(Packet* p)
{
        LOG_DBG("NFS-->Write()\n");

        NfsJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int NfsHandler::Timeout()
{
        LOG_DBG("NFS-->Timeout()\n");
        return 0;
}

int NfsHandler::Close()
{
        LOG_DBG("NFS-->Close()\n");
        return 0;
}
