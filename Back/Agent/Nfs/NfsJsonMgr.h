/*******************************************************************************
 *
 * @ File Name  : NfsJsonMgr.cpp
 * @ Date       : 2013-10-21
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: NFS 模块协议解析类
 * @ History    : 2013-10-21：创建
 *
 * ****************************************************************************/
#ifndef _NFS_JSON_MGR_H_
#define _NFS_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"

class NfsJsonMgr : public IJsonMgr {
public:
        NfsJsonMgr();
        ~NfsJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *LookupNfsList(const Packet *p);
        Packet *CreateNfs(const Packet *p);
        Packet *RemoveNfs(const Packet *p);
        Packet *EditNfs(const Packet *p);
        Packet *StartNfs(const Packet *p);
        Packet *StopNfs(const Packet *p);
        Packet *MountNfsClient(const Packet *p);
        Packet *MountAllNfsClient(const Packet *p);
        Packet *UmountNfsClient(const Packet *p);
        Packet *UmountAllNfsClient(const Packet *p);
        Packet *RemoveNfsClient(const Packet *p);
};

#endif //_NFS_JSON_MGR_H_
