#include "IscsiTarget.h"
#include "Include/LvmConf.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "Agent/Disk/FsOper.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define NOT_FIND    0xFFFFFFFF

using namespace std;
using namespace FsOper;
//using namespace Scidata;

CIscsiTarget::CIscsiTarget()
    :m_filepath(ISCSITARGETCONF), m_ready(0)
{
}

CIscsiTarget::~CIscsiTarget()
{
}

// Target
int CIscsiTarget::AddIscsiTarget(const std::string& targetIQN, const stIscsiTarget* pit)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("AddIscsiTarget(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }
        if (RET_SUCCESS != Load())
        {
                LOG_ERR("AddIscsiTarget(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        // 如果存在, 只设置基本属性
        if (name_exist)
        {
                if (pit)
                {
                        m_vis[i].HeaderDigest   = 1 == pit->HeaderDigest;
                        m_vis[i].DataDigest     = 1 == pit->DataDigest;
                        m_vis[i].InitialR2T     = 1 == pit->InitialR2T;
                        m_vis[i].ImmediateData  = 1 == pit->ImmediateData;
                        LOG_WRN("AddIscsiTarget(): targetIQN already exist, updated: %s", targetIQN.c_str());
                        return RET_SUCCESS;
                }
                LOG_ERR("AddIscsiTarget(): targetIQN already exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        stIscsiTarget newit(targetIQN);
        if (pit)
        {
                newit.HeaderDigest   = 1 == pit->HeaderDigest;
                newit.DataDigest     = 1 == pit->DataDigest;
                newit.InitialR2T     = 1 == pit->InitialR2T;
                newit.ImmediateData  = 1 == pit->ImmediateData;
        }
        m_vis.push_back(newit);
        if (RET_SUCCESS != Write())
        {
                LOG_ERR("AddIscsiTarget(): write file failed!");
                return RET_FAILED;
        }

        LOG_DBG("AddIscsiTarget() success: %s", targetIQN.c_str());

        return RET_SUCCESS;
}

int CIscsiTarget::DelIscsiTarget(const std::string& targetIQN)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("DelIscsiTarget(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }
        if (RET_SUCCESS != Load())
        {
                LOG_ERR("DelIscsiTarget(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("DelIscsiTarget(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (!m_vis[i].vLunMap.empty())
        {
                LOG_ERR("DelIscsiTarget(): LunMap must be empty: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        m_vis.erase(m_vis.begin() + i);

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("DelIscsiTarget(): write file failed!");
                return RET_FAILED;
        }

        LOG_DBG("DelIscsiTarget() success: %s", targetIQN.c_str());
        return DelTargetFromAllowHosts(targetIQN.c_str());
}

int CIscsiTarget::GetAllTargetIQN(vector<string>& vtarget)
{
        vtarget.clear();
        if (RET_SUCCESS != Load())
        {
                LOG_ERR("GetAllTargetIQN(): load iscsi config error");
                return RET_FAILED;
        }


        for (uint i=0; i<m_vis.size(); ++i)
        {
                if ('\n' != m_vis[i].targetIQN[0] && '#' != m_vis[i].targetIQN[0])
                {
                        vtarget.push_back(m_vis[i].targetIQN);
                }
        }

        //LOG_DBG("GetAllTargetIQN(): success");
        return RET_SUCCESS;
}

int CIscsiTarget::GetIscsiTarget(const std::string& targetIQN, stIscsiTarget& stti)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("GetIscsiTarget(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }
        if (RET_SUCCESS != Load())
        {
                LOG_ERR("GetIscsiTarget(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("GetIscsiTarget(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        stti.targetIQN          = targetIQN;
        stti.mIncomingUser      = m_vis[i].mIncomingUser;
        stti.mOutgoingUser      = m_vis[i].mOutgoingUser;
        stti.vLunMap            = m_vis[i].vLunMap;
        stti.HeaderDigest       = m_vis[i].HeaderDigest;
        stti.DataDigest         = m_vis[i].DataDigest;
        stti.InitialR2T         = m_vis[i].InitialR2T;
        stti.ImmediateData      = m_vis[i].ImmediateData;

        //LOG_DBG("GetIscsiTarget() success: %s"), targetIQN.c_str());
        return RET_SUCCESS;
}


// Lun Map
int CIscsiTarget::GetMap(const std::string& targetIQN, std::vector<stLunMap>& vlm)
{
        vlm.clear();

        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("GetMap(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }
        if (RET_SUCCESS != Load())
        {
                LOG_ERR("GetMap(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("GetMap(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        vlm = m_vis[i].vLunMap;

        //LOG_DBG("GetMap() success: %s"), targetIQN);
        return RET_SUCCESS;
}

// 只获取 path // 无需 Load
int CIscsiTarget::GetUnmapLun(std::vector<std::string>& vl) const
{
        vl.clear();
        vector<stLvInfo> vli;
        if (0 != GetLvSql(vli))
        {
                LOG_ERR("GetLvSql(*) error");
                return RET_FAILED;
        }
        for (uint i = 0; i<vli.size(); ++i)
        {
                if (vli[i].fsfmt.empty() && 0 == vli[i].used)
                {
                        vl.push_back(vli[i].path);
                }
        }

        //LOG_DBG("GetUnmapLun(vector all): success");
        return RET_SUCCESS;
}

int CIscsiTarget::MapIscsi(const std::string& targetIQN, const vector<stLunMap>& vlm)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("MapIscsi(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }
        if (RET_SUCCESS != Load())
        {
                LOG_ERR("MapIscsi(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("MapIscsi(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }
/*
        // 已经存在的iscsi-lun,删除其在数据库的记录
        for (uint j=0; j<m_vis[i].vLunMap.size(); ++j)
        {
                if (RET_SUCCESS != SetLvSql(stLvInfo(m_vis[i].vLunMap[j].path, targetIQN, LVUSED_ISCSI), LVSQL_FREEFS))
                {
                        LOG_ERR("update mysql error for set fs free: %s", m_vis[i].vLunMap[j].path.c_str());
                        return RET_FAILED;
                }
        }*/
        // 更新配置vector中已存在的lun
        m_vis[i].vLunMap = vlm;
        if (RET_SUCCESS != Write())
        {
                LOG_ERR("MapIscsi(): write file failed!");
                return RET_FAILED;
        }
/*
        // 将新加的lun记录入数据库
        for (uint j=0; j<vlm.size(); ++j)
        {
                if (RET_SUCCESS != SetLvSql(stLvInfo(vlm[j].path, targetIQN, LVUSED_ISCSI), LVSQL_SETFS))
                {
                        LOG_ERR("update mysql error for set fs=iscsi: %s", vlm[j].path.c_str());
                        return RET_FAILED;
                }
        }
*/
        return RET_SUCCESS;
}

int CIscsiTarget::MapIscsi(const std::string& targetIQN, const stLunMap& stlm)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("MapIscsi(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (stlm.path.empty())
        {
                LOG_ERR("MapIscsi(): empty Lun Map Path: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (RET_SUCCESS != Load())
        {
                LOG_ERR("MapIscsi(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0, j = 0;
        int name_exist = 0, map_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("MapIscsi(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        for (j=0; j<m_vis[i].vLunMap.size(); ++j)
        {
                if (m_vis[i].vLunMap[j].path == stlm.path)
                {
                        map_exist = 1;
                        break;
                }
        }

        if (map_exist)
        {
                LOG_ERR("MapIscsi(): LunMap already exist: %s:%s", targetIQN.c_str(), stlm.path.c_str());
                return RET_FAILED;
        }
/*
        CLvConf     conf;
        if (!conf.AddLvConf(stlm.path, CLvConf::LV_ISCSI, true)) {
                LOG_ERR("AddLvConf error");
                return RET_FAILED;
        }
        */
        m_vis[i].vLunMap.push_back(stlm);
        m_vis[i].vLunMap[m_vis[i].vLunMap.size()-1].LunNum = m_vis[i].GetNextLun();
        if (RET_SUCCESS != Write())
        {
                LOG_ERR("MapIscsi(): write file failed!");
                return RET_FAILED;
        }
/*
        if (!SetLvSql(stLvInfo(stlm.path, targetIQN, LVUSED_ISCSI), LVSQL_SETFS))
        {
                LOG_ERR("update mysql error.");
                return RET_FAILED;
        }
*/
        LOG_DBG("MapIscsi() success: %s", targetIQN.c_str());
        return RET_SUCCESS;
}

int CIscsiTarget::UnmapIscsi(const std::string& targetIQN, const string& path)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("UnmapIscsi(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (path.empty())
        {
                LOG_ERR("UnmapIscsi(): empty Lun Map Path");
                return RET_FAILED;
        }

        if (RET_SUCCESS != Load())
        {
                LOG_ERR("UnmapIscsi(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0, j = 0;
        int name_exist = 0, map_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("UnmapIscsi(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        for (j=0; j<m_vis[i].vLunMap.size(); ++j)
        {
                if (m_vis[i].vLunMap[j].path == path)
                {
                        map_exist = 1;
                        break;
                }
        }

        if (!map_exist)
        {
                LOG_ERR("UnmapIscsi(): LunMap not exist: %s:%s", targetIQN.c_str(), path.c_str());
                return RET_FAILED;
        }

        if (0 < m_vis[i].lastLunNum && m_vis[i].lastLunNum <= m_vis[i].vLunMap[j].LunNum)
        {
                m_vis[i].lastLunNum -= 1;
        }
        m_vis[i].vLunMap.erase(m_vis[i].vLunMap.begin() + j);
/*
        CLvConf     conf;
        if (!conf.AddLvConf(path, CLvConf::LV_ISCSI, false)) {
                LOG_ERR("AddLvConf error");
                return RET_FAILED;
        }
*/
        if (RET_SUCCESS != Write())
        {
                LOG_ERR("UnmapIscsi(): write file failed!");
                return RET_FAILED;
        }
/*
        if (!SetLvSql(stLvInfo(path), LVSQL_FREEFS))
        {
                LOG_ERR("cannot set lv free: %s", path.c_str());
                return RET_FAILED;
        }
*/
        // restart ISCSI-TARGET

        LOG_DBG("UnmapIscsi() success: %s:%s", targetIQN.c_str(), path.c_str());
        return RET_SUCCESS;
}

int CIscsiTarget::DelTargetFromAllowHosts(const char* targetIQN) const
{
        assert(targetIQN != NULL);
        int Ret = 0;

        if(Ret == 0)
        {
                FILE * fpR = fopen(ISCSIHOSTSALLOW,"r");
                FILE * fpW = fopen(ISCSIHOSTSALLOW_TMP,"w");
                if(fpR != NULL && fpW != NULL)
                {
                        char buf[1024];
                        memset(buf, 0 ,1024);

                        char target[256];
                        memset(target, 0 ,256);
                        sprintf(target,"%s ",targetIQN);
                        while (fgets(buf, 1024, fpR) != NULL)
                        {
                                if (strncmp(buf, "#", 1) == 0)
                                {
                                        continue;
                                }

                                if(strstr(buf, target) == NULL)
                                {
                                        fprintf(fpW, buf);
                                }
                        }

                        fflush(fpW);

                        fclose(fpR);
                        fclose(fpW);
                }
                else
                {
                        LOG_ERR("open %s or %s error", ISCSIHOSTSALLOW, ISCSIHOSTSALLOW_TMP);
                        Ret = -1;
                }
        }
        if(Ret == 0)
        {
                Ret = rename(ISCSIHOSTSALLOW_TMP, ISCSIHOSTSALLOW);

                if(Ret != 0)
                {
                        LOG_ERR("recover %s failed", ISCSIHOSTSALLOW);
                }
        }

        return Ret;
}

// 接受 ip/mask, ip/masklen, ip/32, ip/255.255.255.255
int CIscsiTarget::SetAllowHosts(const std::string targetIQN, const std::vector<std::string>& va, int isAllowAll) const
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("SetAllowHosts(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        string sip;
        // mask 转换为 masklen, 32位去掉斜杠'/'
        for (uint i=0; i<va.size(); ++i)
        {
                string ip, mask;
                uint masklen = 32;
                unsigned long long slapos = va[i].find('/');
                if (string::npos == slapos)
                {
                        ip = va[i];
                        sip += ip + ",";
                }
                else
                {
                        ip = va[i].substr(0, slapos);
                        if (!IpLegal(ip))
                        {
                                continue;
                        }
                        mask = va[i].c_str()+slapos+1;
                        // 将mask转换为masklen
                        if (string::npos != mask.find('.'))
                        {
                                masklen = MaskLegal(mask);
                                mask = IntToString(masklen);
                        }
                        if (mask != "32")
                        {
                                sip = ip + "/" + mask + ",";
                        }
                        else
                        {
                                sip += ip + ",";
                        }
                }
                LOG_ERR("SIP=%s", sip.c_str());
        }
        Trim(sip, " ,");

        FILE* f = fopen(ISCSIHOSTSALLOW, "r");
        if (NULL == f)
        {
                LOG_ERR("SetAllowHosts(): fopen(%s): %m", ISCSIHOSTSALLOW);
                return RET_FAILED;
        }

        char cline[1024] = {0};
        string sline;
        int name_exist = 0;
        vector<string> vl;  // 带回车
        while (NULL != fgets(cline, 1024, f))
        {
                sline = Trim(cline);
                if (sline.empty() || sline[0] == '#')
                {
                        vl.push_back(cline);
                        continue;
                }
                vector<string> vopt;
                Split(sline.c_str(), vopt, ' ', " \t", 0);
                if (vopt.size() < 2)
                {
                        vl.push_back(cline);
                        LOG_WRN("SetAllowHosts(): %s has illegal line: %s", sline.c_str());
                        continue;
                }
                if (vopt[0] != targetIQN)
                {
                        vl.push_back(cline);
                        continue;
                }
                name_exist = 1;
                if (isAllowAll)
                {
                        vl.push_back(vopt[0] + " ALL\n");
                        continue;
                }
                else
                {
                        vl.push_back(vopt[0] + " " + sip + "\n");
                        continue;
                }
        }
        fclose(f);

        if (!name_exist)
        {
                vl.push_back(targetIQN + " " + sip + "\n");
        }

        // 写入文件
        f = fopen(ISCSIHOSTSALLOW, "w");
        if (NULL == f)
        {
                LOG_ERR("SetAllowHosts(): fopen(%s): %m", ISCSIHOSTSALLOW);
                return RET_FAILED;
        }
        for (uint j=0; j<vl.size(); ++j)
        {
                fprintf(f, "%s", vl[j].c_str());
        }
        fclose(f);
        return RET_SUCCESS;
}

// va 格式为 ip/masklen, 32位为 ip/32 无需加载 Load
int CIscsiTarget::GetAllowHosts(const std::string targetIQN, std::vector<std::string>& va, int& isAllowAll) const
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("GetAllowHosts(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        FILE* f = fopen(ISCSIHOSTSALLOW, "r");
        if (NULL == f)
        {
                LOG_ERR("GetAllowHosts(): fopen(%s): %m", ISCSIHOSTSALLOW);
                return RET_FAILED;
        }

        va.clear();
        char cline[1024] = {0};
        string sline;
        int name_exist = 0;
        while (NULL != fgets(cline, 1024, f))
        {
                sline = Trim(cline);
                if (sline.empty() || sline[0] == '#')
                {
                        continue;
                }
                vector<string> vopt;
                Split(sline.c_str(), vopt, ' ', " \t", 0);
                if (vopt.size() < 2)
                {
                        LOG_WRN("GetAllowHosts(): %s has illegal line: %s", sline.c_str());
                        continue;
                }
                if (vopt[0] != targetIQN)
                {
                        continue;
                }
                name_exist = 1;
                if (vopt[1] == "all" || vopt[1] == "ALL")
                {
                        isAllowAll = 1;
                        break;
                }
                isAllowAll = 0;
                Split(vopt[1].c_str(), va, ',', " \t", 0);
                break;
        }
        fclose(f);

        if (!name_exist)
        {
                LOG_ERR("GetAllowHosts(): cannot find iqn in %s", ISCSIHOSTSALLOW);
                return RET_FAILED;
        }
        //格式iqn.2012-03.localhost:iqn.2012-03.com.Nas:tsn.123456 192.168.2.1,192.168.0.0/24
        for (uint i=0; i<va.size(); ++i)
        {
                if (string::npos == va[i].find('/'))
                {
                        va[i] += "/32";
                }
        }
        return RET_SUCCESS;
}

int CIscsiTarget::ReadIscsiHosts(const char* buf, std::vector<stIscsiHost>& netHosts)
{
        const char *tmp = buf;
        while(*tmp != ' ' && *tmp != '\0')
        {
                tmp ++;
        }
        vector<string> arr;
        Split(tmp, arr, ',');
        for (int i = 0; i < (int)arr.size(); i++)
        {
                Trim(arr[i]);
                stIscsiHost host;
                vector<string> arr2;
                Split(arr[i].c_str(), arr2, '/');
                for (int j = 0; j < (int)arr2.size(); j++)
                {
                        host.ip = arr2[0];
                        if(arr2.size() == 1)
                        {
                                host.mask = "255.255.255.255";
                        }
                        else
                        {
                                host.mask = MasklenToMask(atoi(arr2[1].c_str()));
                        }
                }
                netHosts.push_back(host);
        }
        return RET_SUCCESS;
}

int CIscsiTarget::SetAllowAll(std::string targetIQN, int isAllow) const
{
        char cmd[512] = {0};
        if (isAllow == 1)
        {
                sprintf(cmd, "cat %s | grep -v -w '%s' >%s_tmp", ISCSIHOSTSALLOW, targetIQN.c_str(), ISCSIHOSTSALLOW);
                system(cmd);
                sprintf(cmd, "echo '%s ALL' >>%s_tmp", targetIQN.c_str(), ISCSIHOSTSALLOW);
                system(cmd);
                sprintf(cmd, "mv %s_tmp %s", ISCSIHOSTSALLOW, ISCSIHOSTSALLOW);
                system(cmd);
        }
        return RET_SUCCESS;
}

// CHAP User
// AddChapUser: 已存在的用户更新密码
int CIscsiTarget::AddChapUser(const std::string& targetIQN, const std::string& username, const std::string& passwd, const int& usertype)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("AddChapUser(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (username.empty() || passwd.length() < 12 || 20 < passwd.length())
        {
                LOG_ERR("AddChapUser(): illegal username or passwd: %s:%s", username.c_str(), passwd.c_str());
                return RET_FAILED;
        }

        if (ISCSI_INCOMING != usertype && ISCSI_OUTGOING != usertype)
        {
                LOG_ERR("AddChapUser(): illegal usertype: %d", usertype);
                return RET_FAILED;
        }

        if (RET_SUCCESS != Load())
        {
                LOG_ERR("AddChapUser(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0, user_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("AddChapUser(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        map<string, string>& users =
                (ISCSI_INCOMING == usertype) ? m_vis[i].mIncomingUser : m_vis[i].mOutgoingUser;

        if (ISCSI_OUTGOING == usertype && !users.empty())
        {
                users[username] = passwd;
                LOG_WRN("AddChapUser(): OutgoingUser is already exist, updated: %s:%s!", targetIQN.c_str(), username.c_str());
                return RET_SUCCESS;
        }

        typeof(users.begin()) iter;

        for (iter=users.begin(); iter!=users.end(); ++iter)
        {
                if (iter->first == username)
                {
                        user_exist = 1;
                        break;
                }
        }

        if (user_exist)
        {
                LOG_WRN("AddChapUser(): username already exist, updated: %s:%s", targetIQN.c_str(), username.c_str());
                users[username] = passwd;
                return RET_SUCCESS;
        }

        //users.insert(users.end(), make_pair(username, passwd));
        users[username] = passwd;

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("AddChapUser(): write file failed!");
                return RET_FAILED;
        }

        LOG_DBG("AddChapUser() success: %s:%s", targetIQN.c_str(), username.c_str());
        return RET_SUCCESS;
}

int CIscsiTarget::DelChapUser(const std::string& targetIQN, const std::string& username, const int& usertype)
{
        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("DelChapUser(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (username.empty())
        {
                LOG_ERR("DelChapUser(): empty username: %s", username.c_str());
                return RET_FAILED;
        }

        if (ISCSI_INCOMING != usertype && ISCSI_OUTGOING != usertype)
        {
                LOG_ERR("DelChapUser(): illegal usertype: %d", usertype);
                return RET_FAILED;
        }

        if (RET_SUCCESS != Load())
        {
                LOG_ERR("DelChapUser(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0, user_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("DelChapUser(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_SUCCESS;
        }

        map<string, string>& users =
                (ISCSI_INCOMING == usertype) ? m_vis[i].mIncomingUser : m_vis[i].mOutgoingUser;

        typeof(users.begin()) iter;

        for (iter=users.begin(); iter!=users.end(); ++iter)
        {
                if (iter->first == username)
                {
                        user_exist = 1;
                        break;
                }
        }

        if (!user_exist)
        {
                LOG_WRN("DelChapUser(): username not exist: %s:%s", targetIQN.c_str(), username.c_str());
                return RET_SUCCESS;
        }

        users.erase(iter);

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("DelChapUser(): write file failed!");
                return RET_FAILED;
        }

        LOG_DBG("DelChapUser() success: %s:%s", targetIQN.c_str(), username.c_str());
        return RET_SUCCESS;
}

int CIscsiTarget::GetChapUserList(const std::string& targetIQN, std::map<std::string, std::string>& vu, const int& usertype)
{
        vu.clear();

        if (targetIQN.empty() || targetIQN.length() > MAX_IQN_LEN)
        {
                LOG_ERR("GetChapUserList(): illegal targetIQN: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        if (ISCSI_INCOMING != usertype && ISCSI_OUTGOING != usertype)
        {
                LOG_ERR("GetChapUserList(): illegal usertype: %d", usertype);
                return RET_FAILED;
        }

        if (RET_SUCCESS != Load())
        {
                LOG_ERR("GetChapUserList(): load iscsi config error");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("GetChapUserList(): targetIQN not exist: %s", targetIQN.c_str());
                return RET_FAILED;
        }

        const map<string, string>& users =
                (ISCSI_INCOMING == usertype) ? m_vis[i].mIncomingUser : m_vis[i].mOutgoingUser;
/*
        typeof(users.begin()) iter;

        for (iter=users.begin(); iter!=users.end(); ++iter)
        {
                stChapUser newcu = {{0}, {0}};
                strcpy(newcu.username, iter->first.c_str());
                memcpy(newcu.password, iter->second.c_str(), (iter->second.length()<32)?iter->second.length():16);
                vcu.push_back(newcu);
        }
*/
        vu = users;
        return RET_SUCCESS;
}


// File Operator
int CIscsiTarget::Load()
{
        if (m_filepath.empty())
        {
                LOG_ERR("empty filepath");
                return RET_FAILED;
        }
        if (1 == m_ready)
        {
                LOG_WRN("Load(%s): no need to load twice!", m_filepath.c_str());
                return RET_SUCCESS;
        }
        m_vis.clear();
        m_ready = 0;

        FILE* fifo = NULL;
        char cline[LONG_INFO_LEN] = {0};
        int linenum = 0;
        string sline;

        stIscsiTarget emptyit;
        emptyit.targetIQN = "#";

        m_vis.push_back(emptyit);

        if (NULL == (fifo = fopen(m_filepath.c_str(), "r")))
        {
                LOG_ERR("fopen(%s): %m", m_filepath.c_str());
                return RET_FAILED;
        }

        while (NULL != fgets(cline, LONG_INFO_LEN, fifo))
        {
                '\n' == *(cline+strlen(cline)-1) ? *(cline+strlen(cline)-1)=0 : 0;
                ++linenum;
                sline = cline;
                Trim(sline, " \t");
                string stmp;

                if (sline.empty() || '#' == sline[0] || ';' == sline[0])
                {
                        m_vis[m_vis.size()-1].vcmt.push_back(string(cline) + "\n");
                        continue;
                }

                if (0 == memcmp(sline.c_str(), "Target", 6))
                {
                        stmp = sline.c_str() + 6;
                        Trim(stmp, " \t");
                        stIscsiTarget newit;
                        newit.targetIQN  = stmp.substr(0, MAX_IQN_LEN);
                        newit.lastLunNum = 0;
                        m_vis.push_back(newit);
                        continue;
                }

                if (0 == memcmp("HeaderDigest", sline.c_str(), 12))
                {
                        m_vis[m_vis.size()-1].HeaderDigest = string::npos!=sline.find("CRC32C");
                        continue;
                }
                if (0 == memcmp("DataDigest", sline.c_str(), 10))
                {
                        m_vis[m_vis.size()-1].DataDigest   = string::npos!=sline.find("CRC32C");
                        continue;
                }
                if (0 == memcmp("InitialR2T", sline.c_str(), 10))
                {
                        m_vis[m_vis.size()-1].InitialR2T   = string::npos!=sline.find("Yes");
                        continue;
                }
                if (0 == memcmp("ImmediateData", sline.c_str(), 13))
                {
                        m_vis[m_vis.size()-1].ImmediateData = string::npos!=sline.find("Yes");
                        continue;
                }

                if (0 == memcmp("IncomingUser", sline.c_str(), 12))
                {
                        stmp = sline.c_str() + 12;
                        Trim(stmp, " \t");
                        stChapUser newcu = {{0}, {0}};
                        sscanf(stmp.c_str(), "%s %s", newcu.username, newcu.password);
                        newcu.username[255] = newcu.password[31] = '\0';

                        if (!NameLegal(newcu.username))
                        {
                                LOG_ERR("Load(): illegal IncomingUser name: %s", newcu.username);
                                continue;
                        }
                        if (!IsAlphaNumStr(newcu.password))
                        {
                                LOG_ERR("Load(): illegal IncomingUser password: %s:%s",
                                        newcu.username, newcu.password);
                                continue;
                        }
                        if (strlen(newcu.password)<12 || 16<strlen(newcu.password))
                        {
                                LOG_ERR("Load(): illegal IncomingUser password length: %s:%s",
                                        newcu.username, newcu.password);
                                continue;
                        }

                        m_vis[m_vis.size()-1].mIncomingUser.insert(make_pair(newcu.username, newcu.password));

                        continue;
                }

                if (0 == memcmp("OutgoingUser", sline.c_str(), 12))
                {
                        stmp = sline.c_str() + 12;
                        Trim(stmp, " \t");
                        stChapUser newcu = {{0}, {0}};
                        sscanf(stmp.c_str(), "%s %s", newcu.username, newcu.password);
                        newcu.username[255] = newcu.password[31] = '\0';

                        if (!NameLegal(newcu.username))
                        {
                                LOG_WRN("Load(): illegal OutgoingUser name: %s", newcu.username);
                                continue;
                        }
                        if (!IsAlphaNumStr(newcu.password))
                        {
                                LOG_WRN("Load(): illegal OutGoingUser password: %s:%s",
                                        newcu.username, newcu.password);
                                continue;
                        }
                        if (strlen(newcu.password)<12 || 16<strlen(newcu.password))
                        {
                                LOG_WRN("Load(): illegal OutGoingUser password length: %s:%s",
                                        newcu.username, newcu.password);
                                continue;
                        }

                        pair<string, string> newkeyval(newcu.username, newcu.password);
                        m_vis[m_vis.size()-1].mOutgoingUser.insert(newkeyval);

                        continue;
                }

                if (0 == memcmp("Lun ", sline.c_str(), 4))
                {
                        stmp = sline.c_str() + 4;
                        Trim(stmp, " \t");
//printf("stmp=[%s]\n", stmp.c_str());
                        stLunMap newLm;
                        newLm.targetIQN = m_vis[m_vis.size()-1].targetIQN;
                        newLm.LunNum    = 0;
                        sscanf(stmp.c_str(), "%u", &(newLm.LunNum));
//printf("lunNum=%u\n", newLm.LunNum);

                        unsigned long pos       = 0;
                        unsigned long commapos  = 0;
                        string sword;

                        if (string::npos != (pos = stmp.find("Path=")))
                        {
                                sword = stmp.c_str() + pos + 5;
                                if (string::npos != (commapos = sword.find(",")))
                                {
                                        sword = sword.substr(0, commapos);
                                }
                                newLm.path = sword;
                        }

                        newLm.type = 0;
                        if (string::npos != (pos = stmp.find("Type=")))
                        {
                                if (0 == memcmp("fileio", stmp.c_str()+pos+5, 6))
                                {
                                        newLm.type = ISCSI_FILEIO;
                                }
                                if (0 == memcmp("blockio", stmp.c_str()+pos+5, 6))
                                {
                                        newLm.type = ISCSI_BLOCKIO;
                                }
                        }

                        newLm.iomode = 0;
                        if (string::npos != (pos = stmp.find("IOMode=")))
                        {
                                if (0 == memcmp("wt", stmp.c_str()+pos+7, 2))
                                {
                                        newLm.iomode = ISCSI_WRITETHRU;
                                }
                                if (0 == memcmp("wb", stmp.c_str()+pos+7, 2))
                                {
                                        newLm.iomode = ISCSI_WRITEBACK;
                                }
                                if (0 == memcmp("ro", stmp.c_str()+pos+7, 2))
                                {
                                        newLm.iomode = ISCSI_READONLY;
                                }
                        }

                        newLm.blockSize = 512;
                        if (string::npos != (pos = stmp.find("BlockSize=")))
                        {
                                sscanf(stmp.c_str()+pos+10, "%d", &newLm.blockSize);
                        }

                        if (string::npos != (pos = stmp.find("ScsiId=")))
                        {
                                sword = stmp.c_str() + pos + 7;
                                if (string::npos != (commapos = sword.find(',')))
                                {
                                        sword = sword.substr(0, commapos);
                                }
                                newLm.ScsiId = sword;
                        }

                        if (string::npos != (pos = stmp.find("ScsiSN=")))
                        {
                                sword = stmp.c_str() + pos + 7;
                                if (string::npos != (commapos = sword.find(',')))
                                {
                                        sword = sword.substr(0, commapos);
                                }
                                newLm.ScsiSN = sword;
                        }

                        m_vis[m_vis.size()-1].vLunMap.push_back(newLm);
                        m_vis[m_vis.size()-1].lastLunNum =
                                (m_vis[m_vis.size()-1].lastLunNum > newLm.LunNum) ?
                                 m_vis[m_vis.size()-1].lastLunNum : newLm.LunNum;

                        continue;
                }

                LOG_WRN("Load(): illegal line: %s:%d", m_filepath.c_str(), linenum);
        }

        fclose(fifo);
        m_ready = 1;
        LOG_DBG("Load() success: %s", m_filepath.c_str());

        return RET_SUCCESS;
}

int CIscsiTarget::Write() const
{
        if (m_vis.empty())
        {
                LOG_WRN("Write(): empty content");
                return RET_FAILED;
        }

        FILE* fifo = stdout;

        if (NULL == (fifo = fopen(m_filepath.c_str(), "w")))
        {
                LOG_ERR("Write(): fopen(%s): %m", m_filepath.c_str());
                return RET_FAILED;
        }

        uint i = 0, j = 0;
        for (i=0; i<m_vis.size(); ++i)
        {
                // Pre Comments
                if (m_vis[i].targetIQN.empty() || '\n' == m_vis[i].targetIQN[0])
                {
                        fprintf(fifo, "\n");
                        continue;
                }
                if ('#' == m_vis[i].targetIQN[0] || ';' == m_vis[i].targetIQN[0])
                {
                        for (j=0; j<m_vis[i].vcmt.size(); ++j)
                        {
                                fprintf(fifo, "%s", m_vis[i].vcmt[j].c_str());
                        }
                        continue;
                }
                // TargetIQN
                fprintf(fifo, "Target %s\n", m_vis[i].targetIQN.c_str());

                // HeaderDigest DataDigest InitialR2T ImmediateData
                char cyesno[2][4] = {"No", "Yes"};
                char ccrcno[2][8] = {"None", "CRC32C"};
                fprintf(fifo, "        HeaderDigest %s\n",  ccrcno[1==m_vis[i].HeaderDigest]);
                fprintf(fifo, "        DataDigest %s\n",    ccrcno[1==m_vis[i].DataDigest]);
                fprintf(fifo, "        InitialR2T %s\n",    cyesno[1==m_vis[i].InitialR2T]);
                fprintf(fifo, "        ImmediateData %s\n", cyesno[1==m_vis[i].ImmediateData]);

                //map<string, string>::const_iterator iter;
                typeof(m_vis[i].mIncomingUser.begin()) iter;
                // IncomingUser
                for (iter=m_vis[i].mIncomingUser.begin(); iter!=m_vis[i].mIncomingUser.end(); ++iter)
                {
                        fprintf(fifo, "        IncomingUser %s %s\n", (*iter).first.c_str(), (*iter).second.c_str());
                }

                // OutgoingUser 只要一个就够了
                //for (iter=m_vis[i].mOutgoingUser.begin(); iter!=m_vis[i].mOutgoingUser.end(); ++iter)
                iter = m_vis[i].mOutgoingUser.begin();
                if (iter != m_vis[i].mOutgoingUser.end())
                {
                        fprintf(fifo, "        OutgoingUser %s %s\n", (*iter).first.c_str(), (*iter).second.c_str());
                }

                // Lun Map
                char cLunmaptype[2][8] = {"fileio", "blockio"};
                char cLunmapmode[3][3] = {"wt", "wb", "ro"};
                for (j=0; j<m_vis[i].vLunMap.size(); ++j)
                {
                        char cmapline[256] = {0};
                        string smapline;

                        sprintf(cmapline, "        Lun %u Path=%s",
                                m_vis[i].vLunMap[j].LunNum, m_vis[i].vLunMap[j].path.c_str());
                        smapline = string(cmapline) + ",Type=" + cLunmaptype[m_vis[i].vLunMap[j].type];
                        smapline += ",BlockSize=" + IntToString(m_vis[i].vLunMap[j].blockSize);

                        if (!m_vis[i].vLunMap[j].ScsiId.empty())
                        {
                                smapline = smapline + ",ScsiId=" + m_vis[i].vLunMap[j].ScsiId;
                        }
                        if (!m_vis[i].vLunMap[j].ScsiSN.empty())
                        {
                                smapline = smapline + ",ScsiSN=" + m_vis[i].vLunMap[j].ScsiSN;
                        }

                        smapline += string(",IOMode=") + cLunmapmode[m_vis[i].vLunMap[j].iomode];

                        fprintf(fifo, "%s\n", smapline.c_str());
                }
                // Comments
                for (j=0; j<m_vis[i].vcmt.size(); ++j)
                {
                        fprintf(fifo, "%s", m_vis[i].vcmt[j].c_str());
                }
        }

        fclose(fifo);

        LOG_DBG("Write(): success");

        return RET_SUCCESS;
}

int CIscsiTarget::Clear()
{
        m_vis.clear();
        //LOG_DBG("Clear(): success");
        return RET_SUCCESS;
}

int CIscsiTarget::LoadIn() const
{
        const char* cmd = "service iscsi-target restart";
        int Ret = system(cmd);
        return Ret;
}

int CIscsiTarget::GetPathInfo(const std::string& path, stLunMap& stlm) const
{
        if (path.empty())
        {
                LOG_ERR("GetPathInfo(): empty path");
                return RET_FAILED;
        }
        FILE* fifo = NULL;
        char cline[MAX_INFO_LEN] = {0};
        string cmd = string("lvdisplay ") + path + " | grep \"LV UUID\" | awk '{print $3}'";

        if (NULL == (fifo = popen(cmd.c_str(), "r")))
        {
                LOG_ERR("GetPathInfo(): popen(): %s", cmd.c_str());
                return RET_FAILED;
        }
        if (NULL == fgets(cline, MAX_INFO_LEN, fifo))
        {
                LOG_ERR("GetPathInfo(): cannot get line info");
                pclose(fifo);
                return RET_FAILED;
        }
        pclose(fifo);
        stlm.ScsiId = cline;
        stlm.ScsiId = stlm.ScsiId.substr(0, 16);
        stlm.ScsiSN = stlm.ScsiId;

        //LOG_DBG("GetPathInfo(): success");
        return RET_SUCCESS;
}

uint CIscsiTarget::FindTarget(const std::string& targetIQN) const
{
        uint nTar = NOT_FIND;
        for (uint i=0; i<m_vis.size(); ++i)
        {
                if (m_vis[i].targetIQN == targetIQN)
                {
                        nTar = i;
                        break;
                }
        }
        return nTar;
}

uint CIscsiTarget::FindLun(uint nTar, const std::string& lv) const
{
        uint nLv = NOT_FIND;
        if (nTar >= m_vis.size())
        {
                return NOT_FIND;
        }
        for (uint i=0; i<m_vis[nTar].vLunMap.size(); ++i)
        {
                if (m_vis[nTar].vLunMap[i].path == lv)
                {
                        nLv = i;
                        break;
                }
        }
        return nLv;
}

std::map<std::string, std::string>::const_iterator CIscsiTarget::FindUser(uint nTar, const std::string& user, const int& usertype) const
{
        std::map<std::string, std::string>::const_iterator itStart, itEnd;
        if (nTar >= m_vis.size())
        {
                return itStart;
        }
        if (ISCSI_INCOMING == usertype)
        {
                itStart = m_vis[nTar].mIncomingUser.begin();
                itEnd = m_vis[nTar].mIncomingUser.end();
        }
        else if (ISCSI_OUTGOING == usertype)
        {
                itStart = m_vis[nTar].mOutgoingUser.begin();
                itEnd = m_vis[nTar].mOutgoingUser.end();
        }
        for (; itStart!=itEnd; itStart++)
        {
                if (itStart->first == user)
                {
                        //return itStart;
                        break;
                }
        }
        return itStart;
}

uint stIscsiTarget::GetNextLun() const
{
        uint num = 0, zero_exist = 0;
        for (uint i=0; i<vLunMap.size(); ++i)
        {
        /*
                if (vLunMap[i].LunNum > num)
                {
                        num = vLunMap[i].LunNum;
                }
                if (0 == vLunMap[i].LunNum)
                {
                        zero_exist = 1;
                }*/
                (vLunMap[i].LunNum > num) && (num = vLunMap[i].LunNum);
                (0 == vLunMap[i].LunNum) && (zero_exist = 1);
        }
        return zero_exist ? num+1 : 0;
}
