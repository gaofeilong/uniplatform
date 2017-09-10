#include <fcntl.h>
#include <stdio.h>

#include "LocalHandler.h"
#include "Utils/Log/Log.h"
#include "Include/Types.h"
#include "Include/Protocol.h"

LocalHandler::LocalHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

LocalHandler::~LocalHandler()
{
}

int LocalHandler::Open(int sockfd)
{
        LOG_DBG("Local-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int LocalHandler::Read(Packet* p)
{
        LOG_DBG("Local-->Read()\n");
        return 0;
}

int LocalHandler::Write(Packet* p)
{
        LOG_DBG("Local-->Write()\n");
        return 0;
}

int LocalHandler::Timeout()
{
        LOG_DBG("Local-->Timeout()\n");
        return 0;
}

int LocalHandler::Close()
{
        LOG_DBG("Local-->Close()\n");
        return 0;
}
