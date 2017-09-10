#ifndef _ISCSI_JSON_MGR_H_
#define _ISCSI_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"
#include "Map/IscsiTarget.h"

class IscsiJsonMgr : public IJsonMgr {
public:
        IscsiJsonMgr();
        ~IscsiJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        // Iscsi target handles
        Packet *ListTarget(const Packet *p);
        Packet *AddTarget(const Packet *p);
        Packet *DelTarget(const Packet *p);
        Packet *MapTarget(const Packet *p);
        Packet *UnmapTarget(const Packet *p);
        Packet *ShowMap(const Packet *p);
        Packet *ShowUnmap(const Packet *p);
        Packet *AddChapUser(const Packet *p);
        Packet *DelChapUser(const Packet *p);
        Packet *GetChapUser(const Packet *p);
        Packet *GetAllowHosts(const Packet *p);
        Packet *SetAllowHosts(const Packet *p);

        CIscsiTarget conf;
};

#endif //_ISCSI_JSON_MGR_H_
