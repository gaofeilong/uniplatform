#ifndef _FC_JSON_MGR_H_
#define _FC_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"
#include "Map/FcMgr.h"

class FcJsonMgr : public IJsonMgr {
public:
        FcJsonMgr();
        ~FcJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        // Fc Lun handles
        Packet *ListFcLun(const Packet *p);
        Packet *AddFcLun(const Packet *p);
        Packet *DelFcLun(const Packet *p);

        FcMgr fc;
};

#endif //_FC_JSON_MGR_H_
