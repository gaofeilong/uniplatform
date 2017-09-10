#include "Rsync.h"
#include "Agent/Nas/Auth/UserOper.h"
#include "Utils/Shell/System.h"
#include "Utils/Log/Log.h"
#include "Include/Constant.h"
#include "Utils/Common/CommonFunc.h"
#include <stdio.h>
using namespace std;

CRsync::CRsync()
        : m_conf(RSYNCCONF)
{
}

CRsync::~CRsync()
{
}

int CRsync::AddDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("AddDir(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        const char* roval = "yes";
        const char* woval = "yes";
        if (SA_RW == perm)
        {
                roval = "no";
                woval = "no";
        }
        else if (SA_RO == perm)
        {
                woval = "no";
        }
        else
        {
                LOG_ERR("AddDir(): illegal perm! path=%s perm=%u", path, perm);
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("AddDir(): load conf failed!");
                return RET_FAILED;
        }
        if (RET_SUCCESS != m_conf.AddSect(sect.c_str()))
        {
                LOG_ERR("AddDir(): cannot add dir in #");
                return RET_FAILED;
        }
        m_conf.SetVal(sect.c_str(), "path", path);
        m_conf.SetVal(sect.c_str(), STR_AUTHUSERS, "#");
        m_conf.SetVal(sect.c_str(), STR_HOSTSALLOW, "#");
        m_conf.SetVal(sect.c_str(), STR_READONLY, roval);
        m_conf.SetVal(sect.c_str(), STR_WRITEONLY, woval);

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("AddDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("AddDir(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

int CRsync::DelDir(const char* path)
{
        if (!path || !*path)
        {
                LOG_ERR("DelDir(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("DelDir(): load conf failed!");
                return RET_FAILED;
        }
        m_conf.DelSect(sect.c_str());

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("DelDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("DelDir(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

int CRsync::GetDir(const char* path, uint& perm)
{
        if (!path || !*path)
        {
                LOG_ERR("GetDir(): empty path!");
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("GetDir(): load conf failed!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        string roval, woval;
        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_READONLY, roval)
         || RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_WRITEONLY, woval))
        {
                LOG_ERR("GetDir(): cannot get value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        int a_ro = roval == "yes";
        int a_wo = woval == "yes";

        if (0 == a_ro && 0 == a_wo)
        {
                perm = SA_RW;
        }
        else if (1 == a_ro && 0 == a_wo)
        {
                perm = SA_RO;
        }
        else
        {
                perm = SA_NO;
        }

        LOG_DBG("GetDir(): success! path=%s, perm=%u", sect.c_str(), perm);

        return RET_SUCCESS;
}
int CRsync::SetDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("SetDir(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        const char* roval = "yes";
        const char* woval = "yes";
        if (SA_RW == perm)
        {
                roval = "no";
                woval = "no";
        }
        else if (SA_RO == perm)
        {
                woval = "no";
        }
        else
        {
                LOG_ERR("SetDir(): illegal perm! path=%s perm=%u", path, perm);
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetDir(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_READONLY, roval)
                || RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_WRITEONLY, woval))
        {
                LOG_ERR("SetDir(): cannot set access in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetDir(): write conf failed!");
                return RET_FAILED;
        }
        LOG_DBG("SetDir(): success! %s:%u", sect.c_str(), perm);

        return RET_SUCCESS;
}

int CRsync::GetUserAccess(const char* path, std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("GetUserAccess(): empty path!");
                return RET_FAILED;
        }
        va.clear();

        string sect = MakeSect(path);
        string sauth;

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("GetUserAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_AUTHUSERS, sauth))
        {
                LOG_ERR("GetUserAccess(): cannot get value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }
        Split(sauth.c_str(), va, ',');

        LOG_DBG("GetUserAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

int CRsync::SetUserAccess(const char* path, const std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("SetUserAccess(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        string users;
        uint i = 0;
        for (i=0; i<va.size(); ++i)
        {
                if (va[i].empty() || '@' == va[i][0])
                {
                        continue;
                }
                users += va[i] + ", ";
        }
        Trim(users, " ,");

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetUserAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_AUTHUSERS, users.c_str()))
        {
                LOG_ERR("SetUserAccess(): cannot set value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetUserAccess(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetUserAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

// Rsync 中使用的 ip 配置格式为 ipaddr/masklen 192.168.0.123/32
int CRsync::GetIpAccess(const char* path, std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("GetIpAccess(): empty path!");
                return RET_FAILED;
        }
        va.clear();

        string sect = MakeSect(path);

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("GetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        string strip;
        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_HOSTSALLOW, strip))
        {
                LOG_ERR("GetIpAccess(): cannot get value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        Split(strip.c_str(), va, ' ', ", ");

        LOG_DBG("GetIpAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

// 设置网段 参数格式 ipaddr/mask ip/masklen 前者将转换为 ip/masklen
int CRsync::SetIpAccess(const char* path, const std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("SetIpAccess(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        string strip;
        uint i = 0, masklen;
        unsigned long pos;
        char cmasklen[8] = {0};
        for (i=0; i<va.size(); ++i)
        {
                pos = va[i].find('/');
                if (string::npos == pos)
                {
                        LOG_WRN("SetIpAccess(): illegal ip: %s", va[i].c_str());
                        continue;
                }
                if (string::npos != va[i].find('.', pos + 1))
                {
                        masklen = MaskLegal(va[i].c_str() + pos + 1);
                        sprintf(cmasklen, "%2u", masklen);
                        strip = strip + " " + va[i].substr(0, pos + 1) + cmasklen;
                        continue;
                }
                strip = strip + " " + va[i];
        }
        Trim(strip, " ");

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_HOSTSALLOW, strip.c_str()))
        {
                LOG_ERR("SetIpAccess(): cannot set value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetIpAccess(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetIpAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

int CRsync::RemoveUser(const std::vector<std::string>& vu)
{
        vector<string> vline, vlu;
        FILE* file = fopen(RSYNCCONF, "r");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", RSYNCCONF);
                return RET_FAILED;
        }

        string sline;
        char cline[LONG_INFO_LEN] = {0};

        while (NULL != fgets(cline, LONG_INFO_LEN, file))
        {
                if ('#' == cline[0] || ';' == cline[0] || '[' == cline[0] || '<' == cline[0] || '\n' == cline[0])
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                unsigned long pos = sline.find(STR_AUTHUSERS);
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                pos = sline.find('=');
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                vlu.clear();
                string sleft = sline.substr(0, pos+1);
                sline = sline.c_str() + pos + 1;
                Split(sline.c_str(), vlu, ',');
                DelMem(vlu, vu);
//printf("vlu=");for(uint k=0; k<vlu.size(); ++k)printf("[%s]", vlu[k].c_str());printf("\n");
                sline = VectorToStr(vlu, ',');
                sleft = sleft + ' ' + sline + "\n";
                vline.push_back(sleft);
//printf("left=[%s] oldline = [%s], newline = [%s]\n", sleft.c_str(), cline, sleft.c_str());
        }

        fclose(file);

        file = fopen(RSYNCCONF, "w");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", RSYNCCONF);
                return RET_FAILED;
        }

        uint i = 0;
        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }

        fclose(file);

        LOG_DBG("RemoveUser(): success!");
        return RET_SUCCESS;
}

int CRsync::RemoveIp  (const std::vector<std::string>& vip)
{
        vector<string> vline, vlip;
        // read
        FILE* file = fopen(RSYNCCONF, "r");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", RSYNCCONF);
                return RET_FAILED;
        }

        string sline;
        char cline[LONG_INFO_LEN] = {0};

        while (NULL != fgets(cline, LONG_INFO_LEN, file))
        {
                // continue when comment
                if ('#' == cline[0] || ';' == cline[0] || '[' == cline[0] || '<' == cline[0] || '\n' == cline[0])
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                unsigned long pos = sline.find(STR_HOSTSALLOW);
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                pos = sline.find('=');
                if (string::npos == pos)
                {
                        vline.push_back(cline);
                        continue;
                }
                // get key
                vlip.clear();
                string sleft = sline.substr(0, pos+1);
                sline = sline.c_str() + pos + 1;
                Split(sline.c_str(), vlip, ' ');
                DelMem(vlip, vip);
                sline = VectorToStr(vlip, ' ');
                sleft = sleft + ' ' + sline + "\n";
                vline.push_back(sleft);
        }

        fclose(file);

        // write
        file = fopen(RSYNCCONF, "w");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", RSYNCCONF);
                return RET_FAILED;
        }

        uint i = 0;
        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }

        fclose(file);

        LOG_DBG("RemoveUser(): success!");
        return RET_SUCCESS;
}

int CRsync::LoadIn()
{
        const char* cmdkill = "kill -9 $(cat /var/run/rsyncd.pid)";
        int ret = ExeCmd(cmdkill);
        ret = ExeCmd("rm -f /var/run/rsyncd.pid");
        const char* cmdstart = "rsync --daemon --config=" RSYNCCONF;
        ret = ExeCmd(cmdstart);
        return ret;
}
