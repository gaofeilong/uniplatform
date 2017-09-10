#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "InstallHandler.h"
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Server/Install/InstallJsonMgr.h"

InstallHandler::InstallHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

InstallHandler::~InstallHandler()
{
}

int InstallHandler::Open(int sockfd)
{
        LOG_DBG("Install-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int InstallHandler::Read(Packet* p)
{
        LOG_DBG("Install-->Read()\n");
        return 0;
}

int InstallHandler::Write(Packet* p)
{
        LOG_DBG("Install-->Write()\n");

        InstallJsonMgr installer;
        Packet *retPack = installer.Handle(p);
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return 0;
}

int InstallHandler::Timeout()
{
        LOG_DBG("Install-->Timeout()\n");
        return 0;
}

int InstallHandler::Close()
{
        LOG_DBG("Install-->Close()\n");
        return 0;
}
