#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Utils/Shell/System.h"
#include "Agent/Nas/Share/ServiceAccess.h"
#include "NetUser.h"
#include <stdio.h>
#include <string.h>

#define READ_LINE_LEN 1024

using namespace std;

int CNetUser::AddUser(const char* user, const char* pwd, const char* group)
{
        int ret = AddUserOfRsync(user, pwd);
        if (RET_SUCCESS == ret)
        {
                ret = AddUserOfWebdav(user, pwd, group);
        }
        return ret;
}
int CNetUser::DelUser(const char* user, int nReload)
{
        int ret = DelUserOfRsync(user);
        if (RET_SUCCESS == ret)
        {
                ret = DelUserOfWebdav(user);
        }
        vector<string> vu;
        vu.push_back(user);
        if (RET_SUCCESS == ret)
        {
                ret = CServiceAccess::RemoveUser(vu, nReload);
        }
        return ret;
}
int CNetUser::SetPwd(const char* user, const char* pwd)
{
        int ret = SetPwdOfRsync(user, pwd);
        if (RET_SUCCESS == ret)
        {
                ret = SetPwdOfWebdav(user, pwd);
        }
        return ret;
}

int CNetUser::AddGroup(const char* group)
{
        return AddGroupOfWebdav(group);
}
int CNetUser::DelGroup(const char* group, int nReload)
{
        int ret = DelGroupOfWebdav(group);
        vector<string> vg;
        vg.push_back(group);
        if (RET_SUCCESS == ret)
        {
                ret = CServiceAccess::RemoveGroup(vg, nReload);
        }
        return ret;
}

int CNetUser::AddUserOfRsync(const char* user, const char* pwd)
{
        if (!user || !*user)
        {
                LOG_ERR("AddUserOfRsync(): empty param!");
                return RET_FAILED;
        }

        FILE* file = fopen(RSYNCUSERFILE, "r");
        if (NULL == file)
        {
                LOG_ERR("AddUserOfRsync(): cannot open file: %s %m", RSYNCUSERFILE);
                return RET_FAILED;
        }

        vector<string> vline;
        char cline[MAX_INFO_LEN] = {0};
        string sline, name_in;
        unsigned long pos;
        int name_exist = 0;
        int lineno = 0;
        while (NULL != fgets(cline, MAX_INFO_LEN, file))
        {
                ++lineno;
                if ('#' == cline[0] || '\n' == cline[0] || name_exist)
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                Trim(sline, " \t\n");
                pos = sline.find(':');
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                name_in = sline.substr(0, pos);
                if (name_in == user)
                {
                        name_exist = 1;
                        vline.push_back(name_in + ":" + pwd + "\n");
                        continue;
                }
                vline.push_back(cline);
        }
        if (!name_exist)
        {
                vline.push_back(string(user) + ":" + pwd + "\n");
        }

        fclose(file);

        file = fopen(RSYNCUSERFILE, "w");
        if (NULL == file)
        {
                LOG_ERR("AddUserOfRsync(): cannot open file: %s %m", RSYNCUSERFILE);
                return RET_FAILED;
        }

        uint i = 0;
        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }
        fclose(file);

        LOG_DBG("AddUserOfRsync(): success! user=%s", user);
        return RET_SUCCESS;
}
int CNetUser::DelUserOfRsync(const char* user)
{
        if (!user || !*user)
        {
                LOG_ERR("DelUserOfRsync(): empty param!");
                return RET_FAILED;
        }

        FILE* file = fopen(RSYNCUSERFILE, "r");
        if (NULL == file)
        {
                LOG_ERR("DelUserOfRsync(): cannot open file: %s %m", RSYNCUSERFILE);
                return RET_FAILED;
        }

        vector<string> vline;
        char cline[MAX_INFO_LEN] = {0};
        string sline;
        unsigned long pos;
        int name_exist = 0;
        while (NULL != fgets(cline, MAX_INFO_LEN, file))
        {
                if ('#' == cline[0] || '\n' == cline[0])
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                pos = sline.find(':');
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                if (0 == memcmp(cline, user, pos))
                {
                        name_exist = 1;
                        continue;
                }
                vline.push_back(cline);
        }
        fclose(file);

        if (0 == name_exist)
        {
                LOG_WRN("DelUserOfRsync(): user not exist! user=%s", user);
                return RET_SUCCESS;
        }

        file = fopen(RSYNCUSERFILE, "w");
        if (NULL == file)
        {
                LOG_ERR("DelUserOfRsync(): cannot open file: %s %m", RSYNCUSERFILE);
                return RET_FAILED;
        }

        uint i = 0;
        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }
        fclose(file);

        LOG_DBG("DelUserOfRsync(): success! user=%s", user);
        return RET_SUCCESS;
}
int CNetUser::SetPwdOfRsync(const char* user, const char* pwd)
{
        return AddUserOfRsync(user, pwd);
}

int CNetUser::AddUserOfWebdav(const char* user, const char* pwd, const char* group)
{
        if (!user || !*user)
        {
                LOG_ERR("AddUserOfWebdav(): empty param!");
                return RET_FAILED;
        }

        char cmd[256] = {0};
        sprintf(cmd, "htpasswd -b %s %s \"%s\"", WEBUSERFILE, user, pwd);
        int ret = ExeCmd(cmd) -1;

        if (!group || !*group)
        {
                LOG_WRN("AddUserOfWebdav(): user not in a group! user=%s", user);
                return RET_SUCCESS;
        }

        vector<string> vline, vlu, vnew;
        FILE* file = fopen(WEBGROUPFILE, "r");
        if (NULL == file)
        {
                LOG_ERR("AddUserOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        char cline[READ_LINE_LEN] = {0};
        string sline;
        unsigned long pos = 0;
        uint lineno = 0;
        vnew.push_back(user);

        while (NULL != fgets(cline, READ_LINE_LEN, file))
        {
                ++lineno;
                sline = cline;
                Trim(sline, " \t\n");
                pos = sline.find(':');
                if (string::npos == pos)
                {
                        LOG_WRN("AddUserOfWebdav(): illegal line in %s:%u", WEBGROUPFILE, lineno);
                        vline.push_back(cline);
                        continue;
                }
                if (sline.substr(0, pos) == group)
                {
                        vlu.clear();
                        Split(sline.c_str()+pos+1, vlu, ' ', " ");
                        AddMem(vlu, vnew);
                        sline = sline.substr(0, pos+1) + VectorToStr(vlu, ' ') + '\n';
                        vline.push_back(sline);
                        continue;
                }
                vline.push_back(cline);
        }

        fclose(file);

        file = fopen(WEBGROUPFILE, "w");
        if (NULL == file)
        {
                LOG_ERR("AddUserOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        uint i = 0;
        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }
        fclose(file);

        LOG_DBG("AddUserOfWebdav(): user=%s ret=%d", user, ret);
        return ret;
}
int CNetUser::DelUserOfWebdav(const char* user)
{
        if (!user || !*user)
        {
                LOG_ERR("DelUserOfWebdav(): empty param!");
                return RET_FAILED;
        }

        char cmd[256] = {0};
        sprintf(cmd, "htpasswd -D %s %s", WEBUSERFILE, user);
        int ret = ExeCmd(cmd) -1;

        // 删除掉webdav.group中的user
        vector<string> vline, vlu, vnew;
        FILE* file = fopen(WEBGROUPFILE, "r");
        if (NULL == file)
        {
                LOG_ERR("DelUserOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        char cline[READ_LINE_LEN] = {0};
        string sline;
        unsigned long pos = 0;
        uint lineno = 0;
        vnew.push_back(user);

        while (NULL != fgets(cline, READ_LINE_LEN, file))
        {
                ++lineno;
                sline = cline;
                Trim(sline, " \t\n");
                pos = sline.find(':');
                if (string::npos == pos)
                {
                        LOG_WRN("DelUserOfWebdav(): illegal line in %s:%u", WEBGROUPFILE, lineno);
                        vline.push_back(cline);
                        continue;
                }
                vlu.clear();
                Split(sline.c_str()+pos+1, vlu, ' ', " ");
                DelMem(vlu, vnew);
                sline = sline.substr(0, pos+1) + VectorToStr(vlu, ' ') + '\n';
                vline.push_back(sline);
        }

        fclose(file);

        file = fopen(WEBGROUPFILE, "w");
        if (NULL == file)
        {
                LOG_ERR("DelUserOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        uint i = 0;
        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }
        fclose(file);
        LOG_DBG("DelUserOfWebdav(): user=%s ret=%d", user, ret);
        return ret;
}
int CNetUser::SetPwdOfWebdav(const char* user, const char* pwd)
{
        return AddUserOfWebdav(user, pwd, NULL);
}

int CNetUser::AddGroupOfWebdav(const char* group)
{
        if (!group || !*group)
        {
                LOG_ERR("AddGroupOfWebdav(): empty param!");
                return RET_FAILED;
        }

        FILE* file = fopen(WEBGROUPFILE, "r");
        if (NULL == file)
        {
                LOG_ERR("AddGroupOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        char cline[READ_LINE_LEN] = {0};
        string sline;
        vector<string> vline;
        unsigned long pos;
        int name_exist = 0;
        while (NULL != fgets(cline, READ_LINE_LEN, file))
        {
                if ('\n' == cline[0] || '#' == cline[0] || name_exist)
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                Trim(sline, " \t\n");
                pos = sline.find(':');
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                if (sline.substr(0, pos) == group)
                {
                        vline.push_back(cline);
                        name_exist = 1;
                        continue;
                }
                vline.push_back(cline);
        }
        fclose(file);

        if (name_exist)
        {
                LOG_WRN("AddGroupOfWebdav(): group already exist! group=%s", group);
                return RET_SUCCESS;
        }
        sline = string(group) + ":" + "\n";
        vline.push_back(sline);

        file = fopen(WEBGROUPFILE, "w");
        if (NULL == file)
        {
                LOG_ERR("AddGroupOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        for (uint i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }
        fclose(file);

        LOG_DBG("AddGroupOfWebdav(): success! group=%s", group);
        return RET_SUCCESS;
}
int CNetUser::DelGroupOfWebdav(const char* group)
{
        if (!group || !*group)
        {
                LOG_ERR("DelGroupOfWebdav(): empty param!");
                return RET_FAILED;
        }

        FILE* file = fopen(WEBGROUPFILE, "r");
        if (NULL == file)
        {
                LOG_ERR("DelGroupOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        char cline[READ_LINE_LEN] = {0};
        string sline, name_in;
        vector<string> vline;
        unsigned long pos;
        int name_already = 0;
        while (NULL != fgets(cline, READ_LINE_LEN, file))
        {
                if ('\n' == cline[0] || '#' == cline[0] || name_already)
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                Trim(sline, " \t\n");
                pos = sline.find(':');
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                name_in = sline.substr(0, pos);
                if (name_in == group)
                {
                        name_already = 1;
                        continue;
                }
                vline.push_back(cline);
        }
        fclose(file);

        if (0 == name_already)
        {
                LOG_WRN("DelGroupOfWebdav(): group not exist! group=%s", group);
                return RET_SUCCESS;
        }

        file = fopen(WEBGROUPFILE, "w");
        if (NULL == file)
        {
                LOG_ERR("DelGroupOfWebdav(): cannot open file: %s %m", WEBGROUPFILE);
                return RET_FAILED;
        }

        for (uint i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }
        fclose(file);

        LOG_DBG("DelGroupOfWebdav(): success! group=%s", group);
        return RET_SUCCESS;
}

