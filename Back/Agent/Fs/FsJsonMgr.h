#ifndef _DDFS_JSON_MGR_H_
#define _DDFS_JSON_MGR_H_

#include <string>
#include "Utils/Json/include/json.h"

#include "Agent/Frame/IJsonMgr.h"

class FsJsonMgr : public IJsonMgr {
public:
        FsJsonMgr();
        ~FsJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *CreateDdfsMp(const Packet *p);
        Packet *ModifyDdfsMp(const Packet *p);
        Packet *MountDdfsMp(const Packet *p);
        Packet *UmountDdfsMp(const Packet *p);
        Packet *FsckDdfsMp(const Packet *p);
        Packet *RemoveDdfsMp(const Packet *p);
        Packet *LookupDdfsMpList(const Packet *p);

        Packet *LookupAdfsMpList(const Packet *p);
        Packet *CreateAdfsMp(const Packet *p);
        Packet *ModifyAdfsMp(const Packet *p);
        Packet *MountAdfsMp(const Packet *p);
        Packet *UmountAdfsMp(const Packet *p);
        Packet *RemoveAdfsMp(const Packet *p);

private:
        void PacketDdfsMpConf(const std::string &conf, Json::Value &pack);

};

#endif //_DDFS_JSON_MGR_H_
