#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "Smb.h"
#include "Utils/Shell/System.h"
#include "Include/LvmConf.h"
#include "Utils/Common/CommonFunc.h"
#include "Utils/Log/Log.h"
#include <stdio.h>

using namespace std;

CSmb::CSmb()
        :m_conf(SMBCONF)
{
}

CSmb::~CSmb()
{
}

int CSmb::AddDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("AddDir(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        const char* waval = "no";
        if (SA_RW == perm)
        {
                waval = "yes";
        }
        else if (SA_RO != perm)
        {
                LOG_ERR("AddDir(): illegal perm! path=%s perm=%u", path, perm);
                return RET_FAILED;
        }
        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("AddDir(): load conf failed!");
                return RET_FAILED;
        }

        string value;
        char ccreatemode[16] = {0};
        char cdirmode[16]    = {0};
        sprintf(ccreatemode, "%04o", SMB_CREATEMODE);
        sprintf(cdirmode, "%04o", SMB_DIRMODE);

        if (RET_SUCCESS != m_conf.AddSect(sect.c_str()))
        {
                LOG_ERR("AddDir(): cannot add dir in #");
                return RET_FAILED;
        }
        m_conf.SetVal(sect.c_str(), "path", path);
        m_conf.SetVal(sect.c_str(), STR_WRITABLE, waval);
        m_conf.SetVal(sect.c_str(), STR_BROWSABLE, "yes");
        m_conf.SetVal(sect.c_str(), STR_CREATEMODE, ccreatemode);
        m_conf.SetVal(sect.c_str(), STR_DIRMODE, cdirmode);
        //m_conf.SetVal(sect.c_str(), STR_DENYHOSTS, "all");
        m_conf.SetVal(sect.c_str(), STR_ALLOWHOSTS, "all");
        m_conf.SetVal(sect.c_str(), STR_VALIDUSERS, "nobody,@nobody");
        //m_conf.SetVal(sect.c_str(), STR_INVALIDUSERS, "#");
        //m_conf.SetVal(sect.c_str(), STR_READLIST, "#");
        m_conf.SetVal(sect.c_str(), STR_WRITELIST, "#");
        m_conf.SetVal(sect.c_str(), STR_GUESTOK, "no");

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("AddDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("AddDir(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

int CSmb::DelDir(const char* path)
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

int CSmb::GetDir(const char* path, uint& perm)
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
        string waval;
        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_WRITABLE, waval))
        {
                LOG_ERR("GetDir(): cannot get value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        if (waval == "yes")
        {
                perm = SA_RW;
        }
        else
        {
                perm = SA_RO;
        }

        LOG_DBG("GetDir(): success! path=%s, perm=%u", sect.c_str(), perm);
        return RET_SUCCESS;
}

int CSmb::SetDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("SetDir(): empty path!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        const char* waval = "no";
        if (SA_RW == perm)
        {
                waval = "yes";
        }
        else if (SA_RO == perm)
        {
                waval = "no";
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

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_WRITABLE, waval))
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

// 返回两个字符串  只读和读写 空格间隔
int CSmb::GetUserAccess(const char* path, std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("GetUserAccess(): empty path!");
                return RET_FAILED;
        }
        va.clear();

        string sect = MakeSect(path);
        //string sauth;
        string sro, srw;
        vector<string>vro, vrw;

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("GetUserAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_VALIDUSERS, sro)
         || RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_WRITELIST, srw))
        {
                LOG_WRN("GetUserAccess(): cannot get value in vector! sect=%s", sect.c_str());
                //return RET_FAILED;
        }
        //Split(sauth.c_str(), va, ',');
        Split(sro.c_str(), vro, ',', " ");
        Split(srw.c_str(), vrw, ',', " ");

        int usertype = -1;
        std::string short_domain;
        GetADdomain(usertype, short_domain);
        short_domain.append("\\");
        // 去掉AD域名前缀
        if(usertype == 3)
        {
                uint i = 0;
                for (i = 0; i < vro.size(); i++)
                {
                        if (vro[i].find(short_domain) != string::npos)
                        {
                                vro[i].erase(0,short_domain.size());
                        }
                }
                for (i = 0; i < vrw.size(); i++)
                {
                        if(vrw[i].find(short_domain) != string::npos)
                        {
                                vrw[i].erase(0,short_domain.size());
                        }
                }
        }

        va.push_back(VectorToStr(vro));
        va.push_back(VectorToStr(vrw));

        LOG_DBG("GetUserAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}
// 设置组权限   传入参数：两个字符串，空格间隔
int CSmb::SetUserAccess(const char* path, const std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("SetUserAccess(): empty path!");
                return RET_FAILED;
        }
        if (va.size() < 2)
        {
                LOG_ERR("SetUserAccess(): empty user param");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        string sro, srw;
        vector<string> vro, vrw;

        Split(va[0].c_str(), vro, ' ');
        Split(va[1].c_str(), vrw, ' ');
        AddMem(vro, vrw);

        uint i = 0;
        int usertype = -1;
        std::string short_domain;
        GetADdomain(usertype, short_domain);
        short_domain.append("\\");
        // 增加AD域名前缀
        if(usertype == 3)
        {
                for (i=0; i<vro.size(); ++i)
                {
                        if(vro[i].find("@") == string::npos)
                        {
                                sro += short_domain + vro[i] + ", ";
                        }
                }
                for (i=0; i<vrw.size(); ++i)
                {
                        if(vrw[i].find("@") == string::npos)
                        {
                                srw += short_domain + vrw[i] + ", ";
                        }
                }
        }
        else
        {
                sro = VectorToStr(vro, ',');
                srw = VectorToStr(vrw, ',');
        }
        Trim(sro, " ,");
        Trim(srw, " ,");
//printf("sro=%s  srw=%s\n", sro.c_str(), srw.c_str());

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetUserAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_VALIDUSERS, sro.c_str())
         || RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_WRITELIST, srw.c_str()))
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

// 获取网段的格式为 ip/mask 192.168.0.1/255.255.254.0 如果只有ip，补上255.255.255.255
int CSmb::GetIpAccess(const char* path, std::vector<std::string>& va)
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
        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_ALLOWHOSTS, strip))
        {
                LOG_ERR("GetIpAccess(): cannot get value in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        if (strip == "all" || strip == "ALL" || strip == "All")
        {
                return 1;
        }

        Split(strip.c_str(), va, ' ', ", ");
        uint i = 0;
        for (i=0; i<va.size(); ++i)
        {
                unsigned long pos = va[i].find('/');
                if (string::npos == pos)
                {
                        va[i] += "/255.255.255.255";
                }
        }

        LOG_DBG("GetIpAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

// 设置网段，参数格式: ip/mask ip/masklen, 后者会被转换成ip/mask
int CSmb::SetIpAccess(const char* path, const std::vector<std::string>& va)
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

        if (va.size() > 0 && va[0] == "all")
        {
                strip = "all";
        }
        else
        {
        for (i=0; i<va.size(); ++i)
        {
                pos = va[i].find('/');
                if (string::npos == pos)
                {
                        strip = strip + " " + va[i];
                        continue;
                }
                if (string::npos == va[i].find('.', pos + 1))
                {
                        sscanf(va[i].c_str() + pos + 1, "%u", &masklen);
                        strip = strip + " " + va[i].substr(0, pos);
                        strip += masklen<32 ? string("/")+MasklenToMask(masklen) : "";
                        continue;
                }
                else if (va[i].substr(pos + 1, string::npos) == "255.255.255.255")
                {
                        strip = strip + " " + va[i].substr(0, pos);
                        continue;
                }
                strip = strip + " " + va[i];
        }
        }
        Trim(strip, " ");

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_ALLOWHOSTS, strip.c_str()))
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

int CSmb::RemoveUser(const std::vector<std::string>& vu)
{
        vector<string> vline, vlu;
        FILE* file = fopen(SMBCONF, "r");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", SMBCONF);
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
                unsigned long pos = sline.find(STR_VALIDUSERS);
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

        file = fopen(SMBCONF, "w");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", SMBCONF);
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

int CSmb::RemoveIp  (const std::vector<std::string>& vip)
{
        vector<string> vline, vlip, vsmbip;
        // smb 网段特殊处理 格式转化为192.168.1.0/255.255.254.0 192.168.1.123 (32掩码时后面不写)
        uint i = 0;
        unsigned long legalpos = 0;
        for (i=0; i<vip.size(); ++i)
        {
                legalpos = vip[i].find('/');
                if (string::npos == legalpos)
                {
                        vsmbip.push_back(vip[i]);
                        continue;
                }
                string smask = vip[i].substr(legalpos+1, string::npos);
                if (smask == "255.255.255.255" || smask == "32")
                {
                        vsmbip.push_back(vip[i].substr(0, legalpos));
                        continue;
                }
                if (string::npos == smask.find('.'))
                {
                        uint masklen;
                        sscanf(smask.c_str(), "%u", &masklen);
                        if (masklen < 32)
                        {
                                vsmbip.push_back(vip[i].substr(0, legalpos+1) + MasklenToMask(masklen));
                        }
                        else
                        {
                                vsmbip.push_back(vip[i].substr(0, legalpos));
                        }
                }
                vsmbip.push_back(vip[i]);
        }
        // read
        FILE* file = fopen(SMBCONF, "r");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", SMBCONF);
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
                unsigned long pos = sline.find(STR_ALLOWHOSTS);
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
                DelMem(vlip, vsmbip);
                sline = VectorToStr(vlip, ' ');
                sleft = sleft + ' ' + sline + "\n";
                vline.push_back(sleft);
        }

        fclose(file);

        // write
        file = fopen(SMBCONF, "w");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", SMBCONF);
                return RET_FAILED;
        }

        for (i=0; i<vline.size(); ++i)
        {
                fprintf(file, "%s", vline[i].c_str());
        }

        fclose(file);

        LOG_DBG("RemoveUser(): success!");
        return RET_SUCCESS;
}

// smb 获取guest ok设置 0=无匿名 1=匿名可读 2=匿名可写
int CSmb::GetAnonyStat(const char* path, uint& stat)
{
        stat = 0;
        if (!path || !*path)
        {
                LOG_ERR("GetAnonyStat(): empty path!");
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("GetAnonyStat(): load conf failed!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        string guestval, writable;
        if (RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_GUESTOK, guestval)
         || RET_SUCCESS != m_conf.GetVal(sect.c_str(), STR_WRITABLE, writable))
        {
                LOG_WRN("GetAnonyStat(): cannot get value in vector! sect=%s", sect.c_str());
                //return RET_FAILED;
        }

        if (guestval == "yes")
        {
                if (writable == "yes")
                {
                        stat = 2;
                }
                else
                {
                        stat = 1;
                }
        }
        else
        {
                stat = 0;
        }

        LOG_DBG("GetAnonyStat(): success! path=%s, perm=%u", sect.c_str(), stat);
        return RET_SUCCESS;
}

int CSmb::SetAnonyStat(const char* path, uint stat)
{
        if (!path || !*path)
        {
                LOG_ERR("SetAnonyStat(): empty path!");
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetAnonyStat(): load conf failed!");
                return RET_FAILED;
        }

        string sect = MakeSect(path);
        const char* guestval = "no";
        const char* writable = "no";
        if (2 == stat)
        {
                guestval = "yes";
                writable = "yes";
        }
        else if (1 == stat)
        {
                guestval = "yes";
                writable = "no";
        }
        else if (0 == stat)
        {
                guestval = "no";
                writable = "no";
        }
        else
        {
                LOG_ERR("SetAnonyStat(): illegal perm! path=%s perm=%u", path, stat);
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetAnonyStat(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_GUESTOK, guestval)
         || RET_SUCCESS != m_conf.SetVal(sect.c_str(), STR_WRITABLE, writable))
        {
                LOG_ERR("SetAnonyStat(): cannot set access in vector! sect=%s", sect.c_str());
                return RET_FAILED;
        }

        if (stat > 0)
        {
                 m_conf.DelKey(sect.c_str(), STR_VALIDUSERS);
                 m_conf.DelKey(sect.c_str(), STR_WRITELIST);
        }
        else
        {
                 m_conf.SetVal(sect.c_str(), STR_VALIDUSERS, "#");
                 m_conf.SetVal(sect.c_str(), STR_WRITELIST, "#");
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetAnonyStat(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetAnonyStat(): success! %s:%u", sect.c_str(), stat);
        return RET_SUCCESS;
}

// 服务控制
int CSmb::LoadIn()
{
        const char* cmd = "service smb restart";
        int ret = ExeCmd(cmd);
        return ret;
}

void CSmb::GetADdomain(int& usertype, std::string& short_domain)
{
        std::string path = MAIN_PATH;
        path += CONF_USERTYPE;
        CConfParser conf(path);

        std::string value, ADdomain;
        conf.Load();
        conf.GetVal("[global]", "usertype", value);
        conf.GetVal("[AD]", "ADdomain", ADdomain);
        conf.Clear();

        usertype = atoi(value.c_str());
        ADdomain = StrUpper(ADdomain.c_str());
        if(usertype == 3)
        {
                if (std::string::npos != ADdomain.find(".COM"))
                {
                        short_domain = ADdomain.erase(ADdomain.find(".COM"));
                }
        }
        else
        {
                short_domain.empty();
        }
}
