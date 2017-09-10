#ifndef _INSTALL_JSON_MGR_H_
#define _INSTALL_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"
#include "Include/Types.h"
#include "LicenseMgr.h"

using std::string;

class InstallJsonMgr : public IJsonMgr {
public: 
        InstallJsonMgr();
        ~InstallJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *ConnectIndexNode(const Packet *p);
        Packet *ConnectDataNode(const Packet *p);
        Packet *InstallIndexNode(const Packet *p);
        Packet *InstallDataNode(const Packet *p);
        Packet *RemoveIndexNode(const Packet *p);
        Packet *RemoveDataNode(const Packet *p);
        Packet *LookupLic(const Packet *p);
        Packet *ExportLic(const Packet *p);
        Packet *ImportLic(const Packet *p);

private:
        BOOSTPTR<LicenseMgr> GetLicenseObj(const string &ip);
};

#endif //_INSTALL_JSON_MGR_H_
