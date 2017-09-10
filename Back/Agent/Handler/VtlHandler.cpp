#include "VtlHandler.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "Include/Types.h"
#include "Include/Protocol.h"
#include "Utils/Log/Log.h"
#include "Utils/Json/include/json.h"

VtlHandler::VtlHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

VtlHandler::~VtlHandler()
{
}

int VtlHandler::Open(int sockfd)
{
        LOG_DBG("Vtl-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int VtlHandler::Read(Packet* p)
{
        LOG_DBG("Vtl-->Read()\n");
        return 0;
}

int VtlHandler::Write(Packet* p)
{
        LOG_DBG("Vtl-->Write()\n");
        //把服务端的包转换成Json结构
        Json::Reader reader;
        Json::Value  data;
        if ( !reader.parse(p->Data, data) ) {
                LOG_DBG("Transfer json error!");
                return -1;
        }

        // 调用VTL接口，把Json传入，返回修改后的Json
        // VTL.Execute(Json::Value jv)

        Json::Value jv;
        jv["test"] = "test data!";

        // 组织包头内容
        Packet pack;
        pack.Length  = jv.toStyledString().size();
        pack.Cmd     = 0x01;
        pack.Magic   = 'B';
        pack.Version = 1;

        // 初始化包结构
        memset(p, 0, sizeof(Packet) + p->Length);

        // 赋值包头
        memcpy(p, &pack, sizeof(Packet));

        // 赋值包体内容
        memcpy(p->Data, jv.toStyledString().c_str(), pack.Length);

        return 0;
}

int VtlHandler::Timeout()
{
        LOG_DBG("Vtl-->Timeout()\n");
        return 0;
}

int VtlHandler::Close()
{
        LOG_DBG("Vtl-->Close()\n");
        return 0;
}
