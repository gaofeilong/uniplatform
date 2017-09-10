#ifndef _NET_JSON_MGR_H_
#define _NET_JSON_MGR_H_

#include "NetJsonMgr.h"
#include "NetworkMgr.h"
#include "Agent/Frame/IJsonMgr.h"

class NetJsonMgr : public IJsonMgr {
public:
        NetJsonMgr();
        ~NetJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *LookupNetBaseInfo(const Packet *p);
        Packet *SetNetBaseInfo(const Packet *p);

        Packet *LookupNetInfo(const Packet *p);
        Packet *SetNetConf(const Packet *p);

        Packet *LookupBondInfo(const Packet *p);
        Packet *BondNetCard(const Packet *p);
        Packet *UbondNetCard(const Packet *p);
        Packet *ModifyBond(const Packet *p);

        Packet *LookupFcInfo(const Packet *p);
        Packet *ModifyFcConf(const Packet *p);

        Packet *LookupIscsiInfo(const Packet *p);
        Packet *ModifyIscsiConf(const Packet *p);
private:
        NetworkMgr m_NetworkMgr;
};

#endif //_NET_JSON_MGR_H_
