#include "FsHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Agent/Fs/FsJsonMgr.h"

FsHandler::FsHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

FsHandler::~FsHandler()
{
}

int FsHandler::Open(int sockfd)
{
        LOG_DBG("FS-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int FsHandler::Read(Packet* p)
{
        LOG_DBG("FS-->Read()\n");
        return 0;
}

int FsHandler::Write(Packet* p)
{
        LOG_DBG("FS-->Write()\n");

        FsJsonMgr jsonMgr;
        Packet *retPack = jsonMgr.Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int FsHandler::Timeout()
{
        LOG_DBG("FS-->Timeout()\n");
        return 0;
}

int FsHandler::Close()
{
        LOG_DBG("FS-->Close()\n");
        return 0;
}
