#ifndef _NAS_JSON_MGR_H_
#define _NAS_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"

class NasJsonMgr : public IJsonMgr {
public:
        NasJsonMgr();
        ~NasJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:

        // Share handles
        Packet *ListShare(const Packet *p);
        Packet *AddShare(const Packet *p);
        Packet *DelShare(const Packet *p);
        Packet *GetShare(const Packet *p);
        Packet *SetShare(const Packet *p);

        // Service control handles
        Packet *ListService(const Packet *p);
        Packet *CtrlService(const Packet *p);
};

#endif //_NAS_JSON_MGR_H_
