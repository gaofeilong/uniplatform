#include "NasHandler.h"

#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Json/include/json.h"
#include "Agent/Nas/NasJsonMgr.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

NasHandler::NasHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

NasHandler::~NasHandler()
{
}

int NasHandler::Open(int sockfd)
{
        //LOG_DBG("Nas-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int NasHandler::Read(Packet* p)
{
        //LOG_DBG("Nas-->Read()\n");
        return 0;
}

int NasHandler::Write(Packet* p)
{
        LOG_DBG("Nas-->Write() --------------> Cmd=%08X", p->Cmd);
        int ret = 0;
        //NasJsonMgr jsonMgr;
        Packet *retPack = NasJsonMgr().Handle(p);
        // 修改p的内容为返回的包信息
        memset(p, 0, sizeof(Packet) + p->Length);
        memcpy(p, retPack, sizeof(Packet) + retPack->Length);
        delete [] (char *)retPack;

        return ret;
}

int NasHandler::Timeout()
{
        LOG_DBG("Nas-->Timeout()\n");
        return 0;
}

int NasHandler::Close()
{
        LOG_DBG("Nas-->Close() <--------------\n");
        return 0;
}
