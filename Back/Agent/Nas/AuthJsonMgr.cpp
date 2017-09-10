#include "Protocol.h"
#include "AuthJsonMgr.h"
#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Utils/Shell/System.h"
#include "Agent/Disk/LvmMgr.h"
#include "Agent/Disk/FsOper.h"
#include <stdio.h>
#include <string.h>

using std::string;
using std::vector;
using namespace FsOper;

static const std::string authConf = std::string(MAIN_PATH) + CONF_USERTYPE;
//static CConfParser conf(authConf);
//static CUserOper* m_user = NULL;

AuthJsonMgr::AuthJsonMgr()
    :m_type(-1), conf(authConf), m_user(NULL)
{
        bool isset = false;
        int usertype = -1;

        std::string value;
        conf.Load();
        conf.GetVal("[global]", "isset", value);
        isset  = atoi(value.c_str());
        conf.GetVal("[global]", "usertype", value);
        usertype = atoi(value.c_str());

        m_type = AUTH_NULL;
        if (isset)
        {
                if(usertype >= AUTH_LOCAL && usertype <= AUTH_NIS)
                {
                        m_type = usertype;
                }
                
        }
        //conf.Clear();

        m_user = CUserOper::Create(m_type);

        if (m_user == NULL)
        {
                m_type = AUTH_NULL;
                LOG_ERR("UserOper Create error!");
        }
}

AuthJsonMgr::~AuthJsonMgr()
{
        if (m_user != NULL)
        {
                delete m_user;
        }
}

Packet *AuthJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_NAS_AUTH_GET:
                return GetAuthType(p);
        case CMD_NAS_AUTH_SET:
                return SetAuthType(p);
        case CMD_NAS_AUTH_GROUP_ADD:
                return AddGroup(p);
        case CMD_NAS_AUTH_GROUP_DEL:
                return DelGroup(p);
        case CMD_NAS_AUTH_GROUP_GET:
                return GetGroup(p);
        case CMD_NAS_AUTH_GROUP_SET:
                return SetGroup(p);
        case CMD_NAS_AUTH_USER_ADD:
                return AddUser(p);
        case CMD_NAS_AUTH_USER_DEL:
                return DelUser(p);
        case CMD_NAS_AUTH_USER_GET:
                return GetUser(p);
        case CMD_NAS_AUTH_USER_SET:
                return SetUser(p);
        case CMD_NAS_QUOTA_GROUP_GET:
                return GetGroupQuota(p);
        case CMD_NAS_QUOTA_GROUP_SET:
                return SetGroupQuota(p);
        case CMD_NAS_QUOTA_USER_GET:
                return GetUserQuota(p);
        case CMD_NAS_QUOTA_USER_SET:
                return SetUserQuota(p);
        }
        LOG_ERR("AuthJsonMgr: unknown cmd: %08X", p->Cmd);
        return RequestError("unknown request");
}

Packet *AuthJsonMgr::GetAuthType(const Packet *p)
{
        LOG_DBG("GetAuthType()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        if (RET_SUCCESS != conf.Load())
        {
                sprintf(errinfo, "load auth config error");
                LOG_ERR("GetAuthType(): %s", errinfo);
                //return RequestError(errinfo);
        }
        //respond["isset"] = Json::Value();

        int usertype = AUTH_NULL;
        int isset = 0;

        std::string value;
        conf.GetVal("[global]", "isset", value);
        respond["isset"] = isset = atoi(value.c_str());

        conf.GetVal("[global]", "usertype", value);
        respond["usertype"] = usertype = atoi(value.c_str());

        if(usertype == AUTH_AD)
        {
                std::string domain,serverip,passwd;
                conf.GetVal("[AD]", "ADdomain", domain);
                conf.GetVal("[AD]", "ADserver", serverip);
                conf.GetVal("[AD]", "ADpasswd", passwd);

                respond["domainname"] = domain;
                respond["serverip"] = serverip;
                respond["passwd"] = passwd;
        }

        if(usertype == AUTH_NIS)
        {
                std::string domain,serverip;
                conf.GetVal("[NIS]", "NISdomain", domain);
                conf.GetVal("[NIS]", "NISserver", serverip);
                
                respond["domainname"] = domain;
                respond["serverip"] = serverip;
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::SetAuthType(const Packet *p)
{
        LOG_DBG("SetAuthType()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        if (RET_SUCCESS != conf.Load())
        {
                sprintf(errinfo, "load auth config error");
                LOG_ERR("SetAuthType(): %s", errinfo);
                return RequestError(errinfo);
        }

        char cmd[1024] = {0};

        int usertype = AUTH_NULL;
        std::string ip, basedn;
        std::string domainname,serverip,passwd;
        usertype = request["usertype"].isNull() ? usertype : atoi(request["usertype"].asString().c_str());
        if(usertype == AUTH_AD || usertype == AUTH_NIS)
        {
                domainname = request["domainname"].isNull() ? string() : request["domainname"].asString();
                domainname = StrUpper(domainname);
                serverip = request["serverip"].isNull() ? string() : request["serverip"].asString();
                passwd = request["passwd"].isNull() ? string() : request["passwd"].asString();
        }

        ResumeUserType();

        conf.Load();
        if (usertype == AUTH_LOCAL) 
        {
                conf.SetVal("[global]", "usertype", "1");
                strcpy(cmd,"authconfig --updateall --disableldap --disableldapauth --disablekrb5 --disablekrb5kdcdns --disablekrb5realmdns \
                        --disablewinbind --disablewinbindauth --disablewinbindusedefaultdomain --disablenis ");
        }
        else if(usertype == AUTH_LDAP)
        {
                conf.SetVal("[global]", "usertype", "2");

                conf.GetVal("[ldap]", "ip", ip);
                conf.GetVal("[ldap]", "basedn", basedn);

                strcpy(cmd,"authconfig --updateall --enableldap --enableldapauth --ldapserver='");
                strcat(cmd, ip.c_str());
                strcat(cmd, "' --ldapbasedn='");
                strcat(cmd, basedn.c_str());
                strcat(cmd, "'");
        }
        else if(usertype == AUTH_AD)
        {
                system("\\cp -f /etc/krb5.conf /etc/krb5.conf.NAS_AD_RESERVE_PREINST");
                //authconfig --updateall --enablekrb5 --krb5kdc="192.168.1.58:88" --krb5adminserver="192.168.1.58:749" --krb5realm="SCINAS.COM" 
                //--enablekrb5kdcdns --enablekrb5realmdns --enablewinbind --enablewinbindauth --smbsecurity=ads --smbrealm="SCINAS.COM" 
                //--smbidmapuid=16777216-33554431 --smbidmapgid=16777216-33554431 --winbindtemplateshell=/bin/bash 
                //--enablewinbindusedefaultdomain
                conf.SetVal("[global]", "usertype", "3");

                conf.SetVal("[AD]", "ADdomain", domainname);
                conf.SetVal("[AD]", "ADserver", serverip);

                strcpy(cmd,"authconfig --updateall --enablekrb5 --krb5kdc='");
                strcat(cmd, serverip.c_str());
                strcat(cmd, ":88' --krb5adminserver='");
                strcat(cmd, serverip.c_str());
                strcat(cmd, ":749' --krb5realm='");
                strcat(cmd, domainname.c_str());
                strcat(cmd, "' --enablekrb5kdcdns --enablekrb5realmdns --enablewinbind --enablewinbindauth --smbsecurity=ads --smbrealm='");
                strcat(cmd, domainname.c_str());
                strcat(cmd, "' --smbidmapuid=16777216-33554431 --smbidmapgid=16777216-33554431 --winbindtemplateshell=/bin/bash");
                strcat(cmd, " --enablewinbindusedefaultdomain;");
        }
        else if(usertype == AUTH_NIS)
        {
                //authconfig --updateall --enablenis --nisdomain=nashsu.com --nisserver=192.168.1.59
                conf.SetVal("[global]", "usertype", "4");

                conf.SetVal("[NIS]", "NISdomain", domainname);
                conf.SetVal("[NIS]", "NISserver", serverip);

                strcpy(cmd,"authconfig --updateall --enablenis --nisdomain='");
                strcat(cmd, domainname.c_str());
                strcat(cmd, "' --nisserver='");
                strcat(cmd, serverip.c_str());
                strcat(cmd, "'");
        }

        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                if (usertype==AUTH_AD)
                {
                        system("\\mv -f /etc/krb5.conf.NAS_AD_RESERVE_PREINST /etc/krb5.conf");
                }
                return RequestError("执行验证配置失败!");
        }
        if(usertype == AUTH_AD)
        {
                ///usr/bin/net join -w SCINAS -S 192.168.1.58 -U Administrator
                //添加DNS
                char cmd2[1500] = {0};
                //sprintf(cmd2,"more /etc/resolv.conf | grep -w '%s' | grep -v '^#'",serverip.c_str());
                sprintf(cmd2, "\\cp -f /etc/resolv.conf /etc/resolv.conf.NAS_AD_RESERVE_PREINST");
                system(cmd2);
                memset(cmd2,0,1500);
                sprintf(cmd2,"echo nameserver %s > /etc/resolv.conf",serverip.c_str());
                system(cmd2);
                memset(cmd2,0,1500);
                sprintf(cmd2, "%s%s %s %s %s", MAIN_PATH, PL_ADPW, domainname.c_str(),serverip.c_str(),passwd.c_str());
                conf.SetVal("[AD]", "ADpasswd", passwd.c_str());

                std::string result;
                int joined = 1;
                if (!Popen(cmd2, result)) {
                        LOG_ERR("Popen (%s) error", cmd2);
                        joined = 0;
                }
                else
                {
                        if(result.find("Failed to join domain") != std::string::npos
                                || result.find("Unable to join domain") != std::string::npos
                                || result.find("Could not connect to server") != std::string::npos
                                || result.find("Connection failed") != std::string::npos
                                || result.find("assword:") != std::string::npos
                                )
                        {
                                LOG_ERR("Join AD domain faild");
                                joined = 0;
                        }
                }
                if(joined == 0)
                {
                        ResumeUserType();
                        return RequestError("关联到AD域失败。");
                }
        }
        else if(usertype == AUTH_NIS)
        {
                if(system("ypcat passwd") != 0)
                {
                        ResumeUserType();
                        LOG_ERR("ypcat passwd error");
                        return RequestError("NIS认证密码失败。");
                }
        }
        system("rm -fr /etc/nas/Conf/old*.txt 2>/dev/null");
        conf.SetVal("[global]", "isset", "1");
        conf.Write();

        sprintf(cmd, "service smb restart");
        if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd(%s) error", cmd);
                return RequestError("重启Smb服务失败");
        }

        //outpack.m_type = CPacket::PACKET_OK;
        //outpack.m_info = "设置验证方式成功。";
        return RequestOk(respond);
}
Packet *AuthJsonMgr::AddGroup(const Packet *p)
{
        LOG_DBG("AddGroup()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        CUserOper::GroupInfoList list;

        for (uint i=0; i<request["glist"].size(); ++i)
        {
                CUserOper::GroupInfo group;
                group.group = request["glist"][i]["gname"].isNull() ? string() : request["glist"][i]["gname"].asString();
                if (group.group.empty())
                {
                        continue;
                }
                list.push_back(group);
        }

        char errinfo[256] = {0};
        if (!m_user->AddMultipleGroups(list))
        {
                respond["type"] = m_type;
                if (AUTH_NULL != m_type)
                {
                        for (uint i=0; i<list.size(); ++i)
                        {
                                respond["glist"][i]["gname"] = list[i].group;
                        }
                }
                sprintf(errinfo, "add group list error");
                LOG_ERR("AddGroup(): %s", errinfo);
                respond["info"] = errinfo;
                return OnRequest(respond, CMD_ERR);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::DelGroup(const Packet *p)
{
        LOG_DBG("DelGroup()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        CUserOper::GroupInfoList list;

        for (uint i=0; i<request["glist"].size(); ++i)
        {
                CUserOper::GroupInfo group;
                group.group = request["glist"][i]["gname"].isNull() ? string() : request["glist"][i]["gname"].asString();
                if (group.group.empty())
                {
                        continue;
                }
                //group.gid = request["glist"][i]["gid"].isNull() ? 0 : atoi(request["glist"][i]["gname"].asString().c_str());
                list.push_back(group);
        }

        char errinfo[256] = {0};
        bool bRet = m_user->DelMultipleGroups(list);
        // CServiceAccess::LoadAll();
        if (!bRet)
        {
                respond["type"] = m_type;
                if (AUTH_NULL != m_type)
                {
                        for (uint i=0; i<list.size(); ++i)
                        {
                                respond["glist"][i]["gname"] = list[i].group;
                        }
                }
                sprintf(errinfo, "del group list error");
                LOG_ERR("DelGroup(): %s", errinfo);
                respond["info"] = errinfo;
                return OnRequest(respond, CMD_ERR);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::GetGroup(const Packet *p)
{
        LOG_DBG("GetGroup()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        char errinfo[256] = {0};
        bool bRet = false;
        respond["type"] = m_type;

        do
        {
                if (m_type == AUTH_NULL) {
                        sprintf(errinfo, "no auth type configured!");
                        bRet = true;
                        break;
                }

                CUserOper::GroupInfoList list;
                if (!m_user->GetGroupList(list)) {
                        sprintf(errinfo, "GetGroupList error");
                        LOG_ERR("GetGroup(): %s", errinfo);
                        break;
                }

                for (uint i = 0; i < list.size(); ++i)
                {
                        CUserOper::GroupInfo& info = list[i];
                        respond["glist"][i]["gid"] = info.gid;
                        respond["glist"][i]["gname"] = info.group;
                        respond["glist"][i]["descript"] = info.groupInfo;
                        respond["glist"][i]["type"] = m_type;
                        // 获取用户列表
                        CUserOper::UserInfoList ulist;
                        if (!m_user->GetUserList(ulist))
                        {
                                sprintf(errinfo, "GetUserList error");
                                LOG_ERR("GetGroup(): %s", errinfo);
                                break;
                        }

                        bool removable = true;
                        for (uint j = 0; j < ulist.size(); ++j)
                        {
                                CUserOper::UserInfo& uinfo = ulist[j];
                                if (uinfo.gid == info.gid)
                                {
                                        removable = false;
                                        break;
                                }
                        }

                        respond["glist"][i]["removable"] = removable;
                }

                bRet = true;

        } while (false);

        if (!bRet)
        {
                return RequestError(errinfo);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::SetGroup(const Packet *p)
{
        LOG_DBG("SetGroup()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        vector<string> vt;
        char errinfo[256] = {0};
        if (RET_SUCCESS != RET_FAILED)
        {
                sprintf(errinfo, "load iscsi config error");
                LOG_ERR("SetGroup(): %s", errinfo);
                return RequestError(errinfo);
        }
        respond["auth"] = Json::Value();

        return RequestOk(respond);
}
Packet *AuthJsonMgr::AddUser(const Packet *p)
{
        LOG_DBG("AddUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        CUserOper::UserInfoList list;

        for (uint i=0; i<request["ulist"].size(); ++i)
        {
                CUserOper::UserInfo user;
                user.user = request["ulist"][i]["uname"].isNull() ? string() : request["ulist"][i]["uname"].asString();
                if (user.user.empty())
                {
                        continue;
                }
                user.gid = request["ulist"][i]["gid"].isNull() ? 0 : atoi(request["ulist"][i]["gid"].asString().c_str());
                user.pwd = request["ulist"][i]["pwd"].isNull() ? string() : request["ulist"][i]["pwd"].asString();
                list.push_back(user);
        }
        char errinfo[256] = {0};
        if (!m_user->AddMultipleUsers(list))
        {
                respond["type"] = m_type;
                if (AUTH_NULL != m_type)
                {
                        for (uint i=0; i<list.size(); ++i)
                        {
                                respond["ulist"][i]["uname"] = list[i].user;
                                respond["ulist"][i]["gid"] = list[i].gid;
                                respond["ulist"][i]["pwd"] = list[i].pwd;
                        }
                }
                sprintf(errinfo, "add user list error");
                LOG_ERR("AddUser(): %s", errinfo);
                respond["info"] = errinfo;
                return OnRequest(respond, CMD_ERR);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::DelUser(const Packet *p)
{
        LOG_DBG("DelUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        CUserOper::UserInfoList list;

        for (uint i=0; i<request["ulist"].size(); ++i)
        {
                CUserOper::UserInfo user;
                user.user = request["ulist"][i]["uname"].isNull() ? string() : request["ulist"][i]["uname"].asString();
                if (user.user.empty())
                {
                        continue;
                }
                //user.uid = request["ulist"][i]["uid"].isNull() ? 0 : atoi(request["ulist"][i]["uid"].asString().c_str());
                list.push_back(user);
        }

        char errinfo[256] = {0};
        bool bRet = m_user->DelMultipleUsers(list);
        // CServiceAccess::LoadAll();
        if (!bRet)
        {
                respond["type"] = m_type;
                if (AUTH_NULL != m_type)
                {
                        for (uint i=0; i<list.size(); ++i)
                        {
                                respond["ulist"][i]["uname"] = list[i].user;
                        }
                }
                sprintf(errinfo, "del user list error");
                LOG_ERR("DelUser(): %s", errinfo);
                respond["info"] = errinfo;
                return OnRequest(respond, CMD_ERR);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::GetUser(const Packet *p)
{
        LOG_DBG("GetUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        bool bRet = false;
        char errinfo[256] = {0};
        respond["type"] = m_type;
        do
        {
                if (m_type == AUTH_NULL) {
                        sprintf(errinfo, "no auth type configured!");
                        bRet = true;
                        break;
                }

                CUserOper::UserInfoList list;
                if (!m_user->GetUserList(list)) {
                        sprintf(errinfo, "GetUserList error");
                        LOG_ERR("GetUser(): %s", errinfo);
                        break;
                }

                // 获取组列表
                CUserOper::GroupInfoList glist;
                if (!m_user->GetGroupList(glist)) {
                        sprintf(errinfo, "GetGroupList error");
                        LOG_ERR("GetUser(): %s", errinfo);
                        break;
                }

                for (uint i = 0; i < list.size(); ++i)
                {
                        CUserOper::UserInfo& info = list[i];
                        respond["ulist"][i]["uid"] = info.uid;
                        respond["ulist"][i]["uname"] = info.user;
                        respond["ulist"][i]["issetpwd"] = info.issetpwd;
                        respond["ulist"][i]["type"] = m_type;
                        respond["ulist"][i]["gid"] = info.gid;

                        for (uint j = 0; j < glist.size(); ++j)
                        {
                                if (glist[j].gid == info.gid) {
                                        respond["ulist"][i]["gname"] = glist[j].group;
                                        break;
                                }
                        }
                }

                for (uint i = 0; i < glist.size(); ++i)
                {
                        respond["glist"][i]["gname"] = glist[i].group;
                        respond["glist"][i]["gid"] = glist[i].gid;
                        respond["glist"][i]["type"] = m_type;
                }

                bRet = true;
        } while (false);

        if (!bRet)
        {
                return RequestError(errinfo);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::SetUser(const Packet *p)
{
        LOG_DBG("SetUser()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }

        CUserOper::UserInfoList list;
        char errinfo[256] = {0};
        bool bRet = true;
        for (uint i=0; i<request["ulist"].size(); ++i)
        {
                CUserOper::UserInfo user;
                user.user = request["ulist"][i]["uname"].isNull() ? string() : request["ulist"][i]["uname"].asString();
                if (user.user.empty())
                {
                        continue;
                }
                user.uid = request["ulist"][i]["uid"].isNull() ? 0 : atoi(request["ulist"][i]["uid"].asString().c_str());
                user.pwd = request["ulist"][i]["pwd"].isNull() ? string() : request["ulist"][i]["pwd"].asString();
                if (!m_user->SetUserPwd(user))
                {
                        bRet = false;
                        list.push_back(user);
                }
                bRet || (bRet = true);
        }

        if (!bRet)
        {
                respond["type"] = m_type;
                if (AUTH_NULL != m_type)
                {
                        for (uint i=0; i<list.size(); ++i)
                        {
                                respond["ulist"][i]["uname"] = list[i].user;
                        }
                }
                sprintf(errinfo, "set user passwd error");
                LOG_ERR("SetUser(): %s", errinfo);
                respond["info"] = errinfo;
                return OnRequest(respond, CMD_ERR);
        }

        return RequestOk(respond);
}
Packet *AuthJsonMgr::GetGroupQuota(const Packet *p)
{
        LOG_DBG("GetGroupQuota()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }
        return RequestOk(respond);
}

Packet *AuthJsonMgr::SetGroupQuota(const Packet *p)
{
        LOG_DBG("SetGroupQuota()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }
        return RequestOk(respond);
}

Packet *AuthJsonMgr::GetUserQuota(const Packet *p)
{
        LOG_DBG("GetUserQuota()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }
        return RequestOk(respond);
}

Packet *AuthJsonMgr::SetUserQuota(const Packet *p)
{
        LOG_DBG("SetUserQuota()\n");
        Json::Value request, respond;
        int ret = JsonValue(p, request);
        if (ret < 0)
        {
                return RequestError("parse request error");
        }
        return RequestOk(respond);
}


void AuthJsonMgr::ResumeUserType()
{
        std::string path2 = CONF_SMB;
        CConfParser conf2(path2);
        conf2.Load();
        conf2.SetVal("[global]", "security", "user");
        conf2.Write();
        system("authconfig --updateall --disableldap --disableldapauth --disablekrb5 --disablekrb5kdcdns --disablekrb5realmdns "
               "--disablewinbind --disablewinbindauth --disablewinbindusedefaultdomain --disablenis");
        char cmd[1500] = {0};
        sprintf(cmd, "cat /etc/resolv.conf.NAS_AD_RESERVE_PREINST");

        if (system(cmd) == 0)
        {
                memset(cmd,0,1500);
                sprintf(cmd, "\\mv /etc/resolv.conf.NAS_AD_RESERVE_PREINST /etc/resolv.conf");
                system(cmd);
        }
}
