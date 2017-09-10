#include "Webdav.h"
#include "Utils/Shell/System.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include <stdio.h>
#include <string.h>

using namespace std;

CWebdav::CWebdav()
        : m_conf(WEBCONF)
{
}

CWebdav::~CWebdav()
{
}

int CWebdav::AddDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("AddDir(): empty path!");
                return RET_FAILED;
        }

        const char* ctmp = path + 11*(0 == memcmp(path, "/mnt/share/", 11));
        string alias = string("Alias /") + StrReplace(ctmp, '/', '-');
        string aliasval = string("\"") + path + "\"";

        if (RET_SUCCESS != m_conf.Open())
        {
                LOG_ERR("AddDir(): load conf failed!");
                return RET_FAILED;
        }
        m_conf.SetVal("#", alias.c_str(), aliasval.c_str());
        if (RET_SUCCESS != m_conf.AddField(path))
        {
                LOG_ERR("AddDir(): cannot add dir in #");
                return RET_FAILED;
        }
        m_conf.SetVal(path, STR_WEB_DAVON, "On");
        m_conf.SetVal(path, STR_WEB_OPTIONS, "+Indexes");
        m_conf.SetVal(path, STR_WEB_AUTHTYPE, "Basic");
        m_conf.SetVal(path, STR_WEB_AUTHNAME, STR_WEB_AN_VAL);
        m_conf.SetVal(path, STR_WEB_AUTHUF, WEBUSERFILE);
        m_conf.SetVal(path, STR_WEB_AUTHGF, WEBGROUPFILE);
        m_conf.SetVal(path, STR_WEB_ALLOWOR, "None");
        m_conf.SetVal(path, STR_WEB_ORDER, STR_WEB_ORDERAD);
        m_conf.SetVal(path, STR_ALLOWFROM, "all");
        if (SA_RO == perm)
        {
//                m_conf.SetVal(string(path) + KVDOT_L + STR_LMT, "#", STR_WEB_RO);
                m_conf.AddField(string(path) + KVDOT_R + STR_WEB_RO, STR_LMT);
                m_conf.SetVal(string(path) + KVDOT_R + STR_WEB_RO, STR_REQUIREUSER, "nobody");
                m_conf.SetVal(string(path) + KVDOT_R + STR_WEB_RO, STR_REQUIREGROUP, "nobody");
                m_conf.AddField(string(path) + KVDOT_R + STR_WEB_RW, STR_LMT);
                m_conf.SetVal(string(path) + KVDOT_R + STR_WEB_RW, STR_REQUIREUSER, "nobody");
                m_conf.SetVal(string(path) + KVDOT_R + STR_WEB_RW, STR_REQUIREGROUP, "nobody");
        }
        else if (SA_RW == perm)
        {
                m_conf.SetVal(string(path) + KVDOT_L + STR_LMT, "#", STR_WEB_RW);
                m_conf.AddField(string(path) + KVDOT_R + STR_WEB_RW, STR_LMT);
                m_conf.DelField(string(path) + KVDOT_R + STR_WEB_RO);
        }
        else
        {
                m_conf.Clear();
                LOG_ERR("AddDir(): illegal perm! path=%s perm=%u", path, perm);
                return RET_FAILED;
        }

        m_conf.SetVal(string(path) + KVDOT_L + STR_LMT, STR_REQUIREUSER, "#");
        m_conf.SetVal(string(path) + KVDOT_L + STR_LMT, STR_REQUIREGROUP, "#");

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("AddDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("AddDir(): success! path=%s", path);
        return RET_SUCCESS;
}

int CWebdav::DelDir(const char* path)
{
        if (!path || !*path)
        {
                LOG_ERR("DelDir(): empty path!");
                return RET_FAILED;
        }

        const char* ctmp = path + 11*(0 == memcmp(path, "/mnt/share/", 11));
        string alias = string("Alias /") + StrReplace(ctmp, '/', '-');

        if (RET_SUCCESS != m_conf.Open())
        {
                LOG_ERR("DelDir(): load conf failed!");
                return RET_FAILED;
        }
        m_conf.DelKey("#", alias.c_str());
        if (RET_SUCCESS != m_conf.DelField(path))
        {
                LOG_ERR("DelDir(): cannot delete in vector! path=%s", path);
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("DelDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("DelDir(): success! path=%s", path);
        return RET_SUCCESS;
}

int CWebdav::GetDir(const char* path, uint& perm)
{
        if (!path || !*path)
        {
                LOG_ERR("GetDir(): empty path!");
                return RET_FAILED;
        }
        perm = SA_NO;

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("GetDir(): load conf failed!");
                return RET_FAILED;
        }

        string key = string(path) + KVDOT_L + STR_LMT;
        string val;
        if (RET_SUCCESS != m_conf.GetVal(key.c_str(), "#", val))
        {
                LOG_ERR("GetDir(): cannot get value in vector! path=%s", path);
                return RET_FAILED;
        }

        if (val == STR_WEB_RW)
        {
                perm = SA_RW;
        }
        else if (val== STR_WEB_RO)
        {
                perm = SA_RO;
        }
        else
        {
                LOG_WRN("GetDir(): illegal Limit value in conf! path=%s val=%s", path, val.c_str());
        }

        LOG_DBG("GetDir(): success! path=%s perm=%u", path, perm);
        return RET_SUCCESS;
}

int CWebdav::SetDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("SetDir(): empty path!");
                return RET_FAILED;
        }

        if (RET_FAILED == m_conf.Open())
        {
                LOG_ERR("SetDir(): load conf failed!");
                return RET_FAILED;
        }

        string val;
        if (RET_SUCCESS != m_conf.GetVal(string(path) + KVDOT_L + STR_LMT, "#", val))
        {
                LOG_ERR("SetDir(): cannot get value in vector! path=%s", path);
                return RET_FAILED;
        }

        if (SA_RO == perm)
        {
                m_conf.SetVal(string(path) + KVDOT_L + STR_LMT, "#", STR_WEB_RO);
                m_conf.AddField(string(path) + KVDOT_R + STR_WEB_WO, STR_LMT);
                m_conf.SetVal(string(path) + KVDOT_R + STR_WEB_WO, STR_REQUIREUSER, "nobody");
        }
        else if (SA_RW == perm)
        {
                m_conf.SetVal(string(path) + KVDOT_L + STR_LMT, "#", STR_WEB_RW);
                m_conf.AddField(string(path) + KVDOT_R + STR_WEB_RW, STR_LMT);
                m_conf.DelField(string(path) + KVDOT_R + STR_WEB_WO);
        }
        else
        {
                LOG_ERR("SetDir(): illegal perm! path=%s perm=%u", path, perm);
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetDir(): success! path=%s perm=%u", path, perm);
        return RET_SUCCESS;
}

int CWebdav::GetUserAccess(const char* path, std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("GetUserAccess(): empty path!");
                return RET_FAILED;
        }
        va.clear();

        if (RET_SUCCESS != m_conf.Open())
        {
                LOG_ERR("GetUserAccess(): load conf failed!");
                return RET_FAILED;
        }

        string sect = path;
        string suro, surw, sgro, sgrw;
        vector<string> vuro, vurw, vgro, vgrw;
        uint i = 0;

        if (RET_SUCCESS != m_conf.GetVal(sect + KVDOT_R + STR_WEB_RO, STR_REQUIREUSER, suro)
         || RET_SUCCESS != m_conf.GetVal(sect + KVDOT_R + STR_WEB_RO, STR_REQUIREGROUP, sgro)
         || RET_SUCCESS != m_conf.GetVal(sect + KVDOT_R + STR_WEB_RW, STR_REQUIREUSER, surw)
         || RET_SUCCESS != m_conf.GetVal(sect + KVDOT_R + STR_WEB_RW, STR_REQUIREGROUP, sgrw)
         )
        {
                LOG_ERR("GetUserAccess(): cannot get value in vector! path=%s", path);
                return RET_FAILED;
        }

        Split(suro.c_str(), vuro, ' ', ", ");
        Split(surw.c_str(), vurw, ' ', ", ");
        Split(sgro.c_str(), vgro, ' ', ", ");
        Split(sgrw.c_str(), vgrw, ' ', ", ");

        for (i=0; i<vgro.size(); ++i)
        {
                vgro[i] = string("@") + vgro[i];
        }
        for (i=0; i<vgrw.size(); ++i)
        {
                vgrw[i] = string("@") + vgrw[i];
        }

        AddMem(vuro, vgro);
        AddMem(vurw, vgrw);
        va.push_back(VectorToStr(vuro));
        va.push_back(VectorToStr(vurw));

        LOG_DBG("GetUserAccess(): success! path=%s", sect.c_str());
        return RET_SUCCESS;
}

int CWebdav::SetUserAccess(const char* path, const std::vector<std::string>& va)
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

        string sect = path;
        string suro, surw, sgro, sgrw, val;
        vector<string> vuro, vurw;//, vgro, vgrw;

        Split(va[0].c_str(), vuro, ' ');
        Split(va[1].c_str(), vurw, ' ');

        uint i = 0;
        for (i=0; i<vuro.size(); ++i)
        {
                if (!vuro[i].empty() && '@'==vuro[i][0])
                {
                        sgro = sgro + (vuro[i].c_str() + 1) + " ";
                        continue;
                }
                suro += vuro[i] + " ";
        }
        Trim(suro, " ,");
        Trim(sgro, " ,");
        for (i=0; i<vurw.size(); ++i)
        {
                if (!vurw[i].empty() && '@'==vurw[i][0])
                {
                        sgrw = sgrw + (vurw[i].c_str() + 1) + " ";
                        continue;
                }
                surw += vurw[i] + " ";
        }
        Trim(surw, " ,");
        Trim(sgrw, " ,");

        if (RET_SUCCESS != m_conf.Open())
        {
                LOG_ERR("SetUserAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.GetVal(sect + KVDOT_R + STR_WEB_RO, "#", val)
         || RET_SUCCESS != m_conf.GetVal(sect + KVDOT_R + STR_WEB_RW, "#", val))
        {
                LOG_ERR("SetUserAccess(): cannot get sect value in vector! path=%s", sect.c_str());
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(sect + KVDOT_R + STR_WEB_RO, STR_REQUIREUSER, suro.c_str())
         || RET_SUCCESS != m_conf.SetVal(sect + KVDOT_R + STR_WEB_RO, STR_REQUIREGROUP, sgro.c_str()))
        {
                m_conf.Clear();
                LOG_ERR("SetUserAccess(): cannot set ro value in vector! path=%s", path);
                return RET_FAILED;
        }
        if (RET_SUCCESS != m_conf.SetVal(sect + KVDOT_R + STR_WEB_RW, STR_REQUIREUSER, surw.c_str())
         || RET_SUCCESS != m_conf.SetVal(sect + KVDOT_R + STR_WEB_RW, STR_REQUIREGROUP, sgrw.c_str()))
        {
                m_conf.Clear();
                LOG_ERR("SetUserAccess(): cannot set rw value in vector! path=%s", path);
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetUserAccess(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetUserAccess(): success! path=%s", path);
        return RET_SUCCESS;
}

int CWebdav::GetIpAccess(const char* path, std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("GetIpAccess(): empty path!");
                return RET_FAILED;
        }
        va.clear();

        if (RET_SUCCESS != m_conf.Open())
        {
                LOG_ERR("GetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        string strip;
        if (RET_SUCCESS != m_conf.GetVal(path, STR_ALLOWFROM, strip))
        {
                LOG_ERR("GetIpAccess(): cannot get value in vector! path=%s", path);
                return RET_FAILED;
        }

        Split(strip.c_str(), va, ' ', " ");

        LOG_DBG("GetIpAccess(): success! path=%s", path);
        return RET_SUCCESS;
}
// 设置网段 参数格式 ip/mask ip/masklen 前者格式将转换成 ip/masklen
int CWebdav::SetIpAccess(const char* path, const std::vector<std::string>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("SetIpAccess(): empty path!");
                return RET_FAILED;
        }

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
        if (strip.empty())
        {
                strip = STR_NONE;
        }

        if (RET_SUCCESS != m_conf.Open())
        {
                LOG_ERR("SetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.SetVal(path, STR_ALLOWFROM, strip.c_str()))
        {
                m_conf.Clear();
                LOG_ERR("SetIpAccess(): cannot set value in vector! path=%s", path);
                return RET_FAILED;
        }

        if (RET_SUCCESS != m_conf.Close())
        {
                LOG_ERR("SetIpAccess(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetIpAccess(): success! path=%s", path);
        return RET_SUCCESS;
}

int CWebdav::RemoveUser(const std::vector<std::string>& vu)
{
        vector<string> vline, vlu, vlg, vou, vog;
        uint i = 0;
        for (i=0; i<vu.size(); ++i)
        {
                if (!vu[i].empty() && '@' == vu[i][0])
                {
                        vog.push_back(vu[i].c_str()+1);
                        continue;
                }
                vou.push_back(vu[i]);
        }
        //printf("vu.size=%lu vog.size=%lu vou.size=%lu\n", vu.size(), vog.size(), vou.size());
        // read
        FILE* file = fopen(WEBCONF, "r");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", WEBCONF);
                return RET_FAILED;
        }

        string sline;
        char cline[MAX_CFG_LINE] = {0};

        while (NULL != fgets(cline, MAX_CFG_LINE, file))
        {
                // continue when comment
                if ('#' == cline[0] || ';' == cline[0] || '[' == cline[0] || '<' == cline[0] || '\n' == cline[0])
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                unsigned long posu = sline.find(STR_REQUIREUSER);
                unsigned long posg = sline.find(STR_REQUIREGROUP);
                if (string::npos != posu)
                {
                        vlu.clear();
                        string sleft = sline.substr(0, posu+strlen(STR_REQUIREUSER)+2);
                        sline = sline.c_str() + posu + strlen(STR_REQUIREUSER) + 2;
                        Split(sline.c_str(), vlu, ' ');
                        DelMem(vlu, vou);
                        sline = VectorToStr(vlu, ' ');
                        sleft += sline + "\n";
                        vline.push_back(sleft);
//printf("修改用户：old=[%s] new=[%s]\n", cline, sleft.c_str());
                        continue;
                }
                else if (string::npos != posg)
                {
                        vlg.clear();
                        string sleft = sline.substr(0, posg+strlen(STR_REQUIREGROUP)+2);
                        sline = sline.c_str() + posg + strlen(STR_REQUIREGROUP) + 2;
                        Split(sline.c_str(), vlg, ' ');
                        DelMem(vlg, vog);
                        sline = VectorToStr(vlg, ' ');
                        sleft += sline + "\n";
                        vline.push_back(sleft);
//printf("修改组：old=[%s] new=[%s]\n", cline, sleft.c_str());
                        continue;
                }
                vline.push_back(cline);
        }

        fclose(file);

        // write
        file = fopen(WEBCONF, "w");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", WEBCONF);
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

int CWebdav::RemoveIp  (const std::vector<std::string>& vip)
{
        vector<string> vline, vlip;
        // read
        FILE* file = fopen(WEBCONF, "r");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", WEBCONF);
                return RET_FAILED;
        }

        string sline;
        char cline[MAX_CFG_LINE] = {0};

        while (NULL != fgets(cline, MAX_CFG_LINE, file))
        {
                // continue when comment
                if ('#' == cline[0] || ';' == cline[0] || '[' == cline[0] || '<' == cline[0] || '\n' == cline[0])
                {
                        vline.push_back(cline);
                        continue;
                }
                sline = cline;
                unsigned long pos = sline.find(STR_ALLOWFROM);
                if (string::npos != pos)
                {
                        vlip.clear();
                        string sleft = sline.substr(0, pos+strlen(STR_ALLOWFROM)+2);
                        sline = sline.c_str() + pos + strlen(STR_ALLOWFROM) + 2;
                        Split(sline.c_str(), vlip, ' ');
                        DelMem(vlip, vip);
                        sline = VectorToStr(vlip, ' ');
                        sleft += sline + "\n";
                        vline.push_back(sleft);
//printf("修改网段：old=[%s] new=[%s]\n", cline, sleft.c_str());
                        continue;
                }
                vline.push_back(cline);
        }

        fclose(file);

        // write
        file = fopen(WEBCONF, "w");
        if (NULL == file)
        {
                LOG_ERR("RemoveUser(): cannot open file: %s :%m", WEBCONF);
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

int CWebdav::LoadIn()
{
        const char* cmd = "sleep 2 && service httpd reload&";
        int ret = ExeCmd(cmd);
        return ret;
}
