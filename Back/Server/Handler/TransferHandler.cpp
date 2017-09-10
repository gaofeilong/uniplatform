#include "TransferHandler.h"

#include <fcntl.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Utils/Lightev/Socket.h"
#include "Utils/Json/include/json.h"
#include "Include/Protocol.h"
#include "Include/Types.h"

using std::string;

TransferHandler::TransferHandler():IEventHandler(TIMEOUT, PACKET_SIZE)
{
}

TransferHandler::~TransferHandler()
{
}

int TransferHandler::Open(int sockfd)
{
        LOG_DBG("Transfer-->Open()\n");
        fcntl(sockfd, F_SETFD, FD_CLOEXEC);
        return 0;
}

int TransferHandler::Read(Packet* p)
{
        LOG_DBG("Transfer-->Read()\n");
        return 0;
}

//把协议转发到指定节点上
int TransferHandler::Write(Packet* p)
{
        int ret = 0;
        LOG_DBG("Transfer-->Write()\n");

        //1. 解析包结构并转换成JSon结构
        Json::Reader reader;
        Json::Value  data;
        if ( !reader.parse(p->Data, data) ) {
                TransferError(p, "parse request error!");
                return 0;
        }
        // LOG_DBG("Received: %c, %c, 0x%08X, %d\n%s", p->Magic, p->Version,
        //                 p->Cmd, p->Length, data.toStyledString().c_str());

        // 2. 获取IP, 连接指定IP
        string ip = data["agentIp"].asString();
        int sockFd = Socket::Connect(ip.c_str(), AGENT_PORT);
        if ( sockFd < 0 ) {
                TransferError(p, "connect agent ip:" + ip + " error!");
                return 0;
        }

        // 3. 把包信息转发到该IP
        ret = Socket::SendPacket(sockFd, p);
        if ( ret < 0 ) {
                TransferError(p, "send request to ip:" + ip + " error!");
                return 0;
        }

        // 4. 接收 Agent 处理结果 - 包头
        // 注意：这里如果用 ReceiveTimeout 或 ReceivePacket 会报超时错误
        Packet pack;
        ret = Socket::Receive(sockFd, (char *)&pack, sizeof(Packet));
        if (ret < 0) {
                TransferError(p, "receive respond head from ip:" + ip + " error!");
                return 0;
        }

        // 5. 接收 Agent 处理结果 - 包体
        // 注意：传过来的包p是不能释放的
        memset(p, 0, sizeof(Packet)+p->Length);
        memcpy(p, &pack, sizeof(Packet));
        if (pack.Length > 0) {
                ret = Socket::Receive(sockFd, p->Data, p->Length);
                if (ret < 0) {
                        TransferError(p, "receive respond data from ip:" + ip + " error!");
                        return 0;
                }
                if (!reader.parse(p->Data, data) ) {
                        TransferError(p, "parse respond error");
                        return 0;
                }
                // LOG_DBG("Sended: %c, %c, 0x%08X, %d\n%s", p->Magic, p->Version, 
                //         p->Cmd, p->Length, data.toStyledString().c_str());
        } else {
                // LOG_DBG("Sended: %c, %c, 0x%08X, %d\n{}", p->Magic, p->Version, 
                //         p->Cmd, p->Length);
        }

        close(sockFd);
        return 0;
}

int TransferHandler::Timeout()
{
        LOG_ERR("Transfer-->Timeout()\n");
        return 0;
}

int TransferHandler::Close()
{
        LOG_DBG("Transfer-->Close()\n");
        return 0;
}

void TransferHandler::TransferError(Packet *p, const string &info)
{
        Json::Value respond;
        respond["info"] = info;
        LOG_ERR("%s", info.c_str());
        memset(p, 0, sizeof(Packet)+p->Length);

        p->Magic     = 'B';
        p->Version   = '1';
        p->Cmd       = CMD_ERR;
        p->Length    = respond.toStyledString().size();
        memcpy(p->Data, respond.toStyledString().c_str(), p->Length);
}
