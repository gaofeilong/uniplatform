#include "UserOper.h"
#include "UserLocal.h"
#include "UserLdap.h"
#include "UserAD.h"
#include "UserNIS.h"

#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Agent/Nas/Base/ConfParser.h"
#include "Utils/Common/String.h"
#include <stdio.h>

static const std::string authConf = std::string(MAIN_PATH) + CONF_USERTYPE;

CUserOper::CUserOper()
{}

CUserOper::~CUserOper()
{}

CUserOper* CUserOper::Create(int type)
{
        CUserOper * user = NULL;
        switch (type)
        {
        case GROUP_LOCAL:
                user = new CUserLocal;
                break;
        case GROUP_LDAP:
                user = new CUserLdap;
                break;
        case GROUP_AD:
                user = new CUserAD;
                break;
        case GROUP_NIS:
                user = new CUserNIS;
                break;
        default:
                LOG_ERR("DealUser type(%d) error", type);
                break;
        }
        return user;
}

bool CUserOper::GetGroupList(std::vector<std::string>& list)
{
        std::string path = MAIN_PATH;
        path += CONF_USERTYPE;
        CConfParser conf(path);

        std::string value;
        conf.Load();
        conf.GetVal("[global]", "usertype", value);
        conf.Clear();

        int usertype = atoi(value.c_str());

        CUserOper* user = Create(usertype);
        if (user == NULL) {
                return false;
        }

        GroupInfoList glist;
        user->GetGroupList(glist);
        if (glist.size() <= 0) {
                return false;
        }

        for (uint i = 0; i < glist.size(); ++i)
        {
                list.push_back(glist[i].group);
        }

        delete user;

        return true;
}

bool CUserOper::GetGroupAndUserList(GroupInfoList& glist)
{
        std::string path = MAIN_PATH;
        path += CONF_USERTYPE;
        CConfParser conf(path);

        std::string value;
        conf.Load();
        conf.GetVal("[global]", "usertype", value);
        conf.Clear();

        int usertype = atoi(value.c_str());

        CUserOper* user = Create(usertype);
        if (user == NULL) {
                return false;
        }

        user->GetGroupList(glist);
        if (usertype != GROUP_AD && glist.size() <= 0) {
                return false;
        }
        
        CUserOper::UserInfoList ulist;
        user->GetUserList(ulist);
        if (ulist.size() <= 0) {
                return false;
        }
        delete user;
        
        if(usertype != GROUP_AD)
        {
                for(uint i = 0; i < ulist.size();i++)
                {
                        for(uint j = 0; j < glist.size();j++)
                        {
                                if(ulist[i].gid == glist[j].gid)
                                {
                                        glist[j].innerUser.push_back(ulist[i].user);
                                        break;
                                }
                        }
                }
        }
        else
        {
                GroupInfo g;
                g.gid = 0;
                g.group = "unkown";
                glist.push_back(g);
                for(uint i = 0; i < ulist.size();i++)
                {
                        glist[0].innerUser.push_back(ulist[i].user);
                }
        }

        return true;
}

void CUserOper::GetLastUsers(UserInfoList& list)
{
        FILE *fp = fopen(OLD_USERS,"r");
        if(fp == NULL)
        {
                LOG_ERR("fopen(): %s", OLD_USERS);
        }
        else
        {
                char cline[1024] = {0};
                std::string sline;
                while (NULL != fgets(cline, 1024, fp))
                {
                        UserInfo user;
                        sline = cline;
                        Trim(sline,"\t\n\r");
                        String::StringArray arr;
                        String::Split(sline.c_str(), ',', arr);
                                
                        user.user = arr[0];
                        user.uid = atoi(arr[1].c_str());
                        user.gid = atoi(arr[2].c_str());
                        user.issetpwd = atoi(arr[3].c_str());
                        
                        list.push_back(user);
                }
                fclose(fp);
        }
}

void CUserOper::UpdatetoLastUsers(UserInfoList& list)
{
        FILE *fp = fopen(OLD_USERS_TMP,"w");
        if(fp == NULL)
        {
                LOG_ERR("fopen(): %s", OLD_USERS_TMP);
        }
        else
        {
                for(uint i = 0; i < list.size(); i++)
                {
                        fprintf(fp,"%s,%u,%u,%d\n",list[i].user.c_str(),list[i].uid,list[i].gid,list[i].issetpwd);
                        
                }
                
                fclose(fp);
                char cmd[256] = {0};
                sprintf(cmd,"mv -f %s %s",OLD_USERS_TMP,OLD_USERS);
                system(cmd);
        }
}

void CUserOper::UpdateUser(UserInfo& user)
{
        UserInfoList list;
        CUserOper::GetLastUsers(list);
        for(uint i = 0; i < list.size(); i++)
        {
                if(list[i].user.compare(user.user) == 0)
                {
                        list[i].issetpwd = 1;
                        break;
                }
        }
        CUserOper::UpdatetoLastUsers(list);
        
        /*
        FILE *fpw = fopen(OLD_USERS_TMP,"w");
        if(fpw == NULL)
        {
                LOG_ERR("fopen(): %s", OLD_USERS_TMP);
        }
        
        FILE *fpr = fopen(OLD_USERS,"r");
        if(fpr == NULL)
        {
                LOG_ERR("fopen(): %s", OLD_USERS);
        }
        if(fpr != NULL && fpw != NULL)
        {
                char cline[1024] = {0};
                std::string sline;
                while (NULL != fgets(cline, 1024, fpr))
                {
                        UserInfo tmpuser;
                        sline = cline;
                        Trim(sline,"\t\n\r");
                        String::StringArray arr;
                        String::Split(sline.c_str(), ',', arr);
                                
                        tmpuser.user = arr[0];
                        tmpuser.uid = atoi(arr[1].c_str());
                        tmpuser.gid = atoi(arr[2].c_str());
                        tmpuser.issetpwd = atoi(arr[3].c_str());
                        
                        if(tmpuser.user.compare(user.user) == 0 && tmpuser.issetpwd == 0)
                        {
                                tmpuser.issetpwd = 1;
                        }
                        fprintf(fpw,"%s,%u,%u,%d\n",tmpuser.user.c_str(),tmpuser.uid,tmpuser.gid,tmpuser.issetpwd);
                }
                fclose(fpr);                
                fclose(fpw);
                
                char cmd[256] = {0};
                sprintf(cmd,"mv -f %s %s",OLD_USERS_TMP,OLD_USERS);
                system(cmd);
        }*/
}
void CUserOper::GetLastGroups(GroupInfoList& list)
{
        FILE *fp = fopen(OLD_GROUPS,"r");
        if(fp == NULL)
        {
                LOG_ERR("fopen(): %s", OLD_GROUPS);
        }
        else
        {
                char cline[1024] = {0};
                std::string sline;
                while (NULL != fgets(cline, 1024, fp))
                {
                        GroupInfo group;
                        sline = cline;
                        Trim(sline,"\t\n\r");
                        String::StringArray arr;
                        String::Split(sline.c_str(), ',', arr);
                                
                        group.group = arr[0];
                        group.gid = atoi(arr[1].c_str());
                        
                        list.push_back(group);
                }
                fclose(fp);
        }
}

void CUserOper::UpdatetoLastGroups(GroupInfoList& list)
{
        FILE *fp = fopen(OLD_GROUPS_TMP,"w");
        if(fp == NULL)
        {
                LOG_ERR("fopen(): %s", OLD_GROUPS_TMP);
        }
        else
        {
                for(uint i = 0; i < list.size(); i++)
                {
                        fprintf(fp,"%s,%u\n",list[i].group.c_str(),list[i].gid);
                        
                }
                
                fclose(fp);
                char cmd[256] = {0};
                sprintf(cmd,"mv -f %s %s",OLD_GROUPS_TMP,OLD_GROUPS);
                system(cmd);
        }
}

void CUserOper::EmptyQuota(unsigned int id, int type)
{
        char cmd[64] = {0};
        //清除用户配额
        if(type == 0)
        {
                sprintf(cmd,"setquota -u %d 0 0 0 0 -a",id);
                system(cmd);
        }
        //清楚组配额
        else if(type == 1)
        {
                sprintf(cmd,"setquota -g %d 0 0 0 0 -a",id);
                system(cmd);
        }
}
