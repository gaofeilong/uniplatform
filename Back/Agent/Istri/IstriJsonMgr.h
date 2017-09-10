#ifndef _ISTRI_JSON_MGR_H_
#define _ISTRI_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"

class IstriJsonMgr : public IJsonMgr {
public:
        IstriJsonMgr();
        ~IstriJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *LookupIstriList(const Packet *p);
        Packet *StartIstriIndex(const Packet *p);
        Packet *StartIstriData(const Packet *p);
        Packet *StopIstriIndex(const Packet *p);
        Packet *StopIstriData(const Packet *p);
        Packet *EditIstriIndex(const Packet *p);
        Packet *EditIstriData(const Packet *p);
        Packet *FixIstriIndex(const Packet *p);
        Packet *FixIstriData(const Packet *p);
        Packet *ClearIstriDataId(const Packet *p);
};

#endif //_ISTRI_JSON_MGR_H_
