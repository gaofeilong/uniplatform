#ifndef _SYS_INFO_JSON_MGR_H_
#define _SYS_INFO_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"

class SysInfoJsonMgr : public IJsonMgr {
public:
        SysInfoJsonMgr();
        ~SysInfoJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        Packet *LookupSysInfo(const Packet *p);

        Packet *SetSysTime(const Packet *p);
        Packet *LookupSysTime(const Packet *p);

        Packet *SetSysMsgConf(const Packet *p);
        Packet *LookupMsgInfo(const Packet *p);

        Packet *SetUpsConf(const Packet *p);
        Packet *LookupUpsInfo(const Packet *p);

        Packet *LookupLic(const Packet *p);
        Packet *ExportLic(const Packet *p);
        Packet *ImportLic(const Packet *p);

        Packet *UpdatePro(const Packet *p);
        Packet *EmailTest(const Packet *p);
};

#endif //_SYS_INFO_JSON_MGR_H_
