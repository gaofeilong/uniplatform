#ifndef AUTH_JSON_MGR_H_
#define AUTH_JSON_MGR_H_

#include "Agent/Frame/IJsonMgr.h"
#include "Agent/Nas/Base/ConfParser.h"
#include "Agent/Nas/Auth/UserOper.h"

enum AUTH_TYPE {
        AUTH_NULL = -1,
        AUTH_LOCAL = 1,
        AUTH_LDAP = 2,
        AUTH_AD = 3,
        AUTH_NIS = 4
};

class AuthJsonMgr : public IJsonMgr {
public:
        AuthJsonMgr();
        ~AuthJsonMgr();

public:
        Packet *Handle(const Packet *p);

private:
        // Auth handles
        Packet *GetAuthType(const Packet *p);
        Packet *SetAuthType(const Packet *p);

        Packet *AddGroup(const Packet *p);
        Packet *DelGroup(const Packet *p);
        Packet *GetGroup(const Packet *p);
        Packet *SetGroup(const Packet *p);

        Packet *AddUser(const Packet *p);
        Packet *DelUser(const Packet *p);
        Packet *GetUser(const Packet *p);
        Packet *SetUser(const Packet *p);

        Packet *GetGroupQuota(const Packet *p);
        Packet *SetGroupQuota(const Packet *p);
        Packet *GetUserQuota(const Packet *p);
        Packet *SetUserQuota(const Packet *p);

        void        ResumeUserType();
        int         m_type;
        CConfParser conf;
        CUserOper*  m_user;
};
#endif //AUTH_JSON_MGR_H_
