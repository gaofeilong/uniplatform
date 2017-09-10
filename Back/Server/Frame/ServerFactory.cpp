#include "ServerFactory.h"
#include "Include/Protocol.h"
#include "Server/Handler/InstallHandler.h"
#include "Server/Handler/LocalHandler.h"
#include "Server/Handler/TransferHandler.h"
#include "Server/Handler/BadRequestHandler.h"

IEventHandler* ServerFactory::CreateHandler(int cmd)
{
        switch (cmd & REQUEST_TYPE_MASK) {      // 区分模块,按模块来下发协议
        case REMOTE_REQUEST:
                return new TransferHandler;     // 处理其他节点业务,nas, vtl, public
        case LOCAL_REQUEST:
                return new LocalHandler;        // 处理本机业务,如节点管理
        case INSTALL_REQUEST:
                return new InstallHandler;      // 处理安装、删除节点请求
        default:
                return new BadRequestHandler;   // 不识别的请求
        }
        return NULL;
}

void ServerFactory::DeleteHandler(IEventHandler* obj)
{
        if (obj != NULL) {
                 delete obj;
        }
}
