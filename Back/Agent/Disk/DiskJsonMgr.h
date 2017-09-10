#ifndef _DISK_JSON_MGR_H_
#define _DISK_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"

class DiskJsonMgr : public IJsonMgr {
public:
        DiskJsonMgr();
        ~DiskJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *ListVolume(const Packet *p);
        Packet *ShowSingle(const Packet *p);
        Packet *MergeVg(const Packet *p);
        Packet *ListLv(const Packet *p);
        Packet *CreateLv(const Packet *p);
        Packet *ModifyLv(const Packet *p);
        Packet *DeleteLv(const Packet *p);
};

#endif //_DISK_JSON_MGR_H_
