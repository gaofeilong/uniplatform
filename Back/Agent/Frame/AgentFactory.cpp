#include "AgentFactory.h"
#include "Include/Protocol.h"
#include "Agent/Handler/NasHandler.h"
#include "Agent/Handler/VtlHandler.h"
#include "Agent/Handler/SysHandler.h"
#include "Agent/Handler/NetHandler.h"
#include "Agent/Handler/DiskHandler.h"
#include "Agent/Handler/FsHandler.h"
#include "Agent/Handler/ArchiveHandler.h"
#include "Agent/Handler/IstriHandler.h"
#include "Agent/Handler/NfsHandler.h"
#include "Server/Handler/BadRequestHandler.h"

IEventHandler* AgentFactory::CreateHandler(int cmd)
{
        switch (cmd & MODULE_TYPE_MASK) {       /* 区分模块,按模块来下发协议 */
        case CMD_SYS:
                return new SysHandler;          /* 处理系统信息模块业务 */
        case CMD_NET:
                return new NetHandler;          /* 处理网络管理模块业务 */
        case CMD_DISK:
                return new DiskHandler;         /* 处理磁盘管理模块业务 */
        case CMD_DDFS:
                return new FsHandler;           /* 处理DDFS管理模块业务 */
        case CMD_NAS:
                return new NasHandler;          /* 处理NAS模块业务 */
        case CMD_VTL:
                return new VtlHandler;          /* 处理VTL模块业务 */
        case CMD_ARCHIVE:
                return new ArchiveHandler;      /* 处理归档模块业务 */
        case CMD_ISTRI:
                return new IstriHandler;        /* 处理istri模块业务 */
        case CMD_NFS:
                return new NfsHandler;          /* 处理NFS模块业务 */
        default:
                return new BadRequestHandler;   /* 不识别的请求 */
        }
        return NULL;
}

void AgentFactory::DeleteHandler(IEventHandler* obj)
{
        if (obj != NULL) {
                 delete obj;
        }
}
