#include "ServiceMgr.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "Utils/Log/Log.h"
#include "Include/Constant.h"
#include "Utils/Shell/System.h"

CServiceMgr::CServiceMgr()
{
}

CServiceMgr::~CServiceMgr()
{
}

int CServiceMgr::ServiceCtrl(const int& svcnum, const int& ctrl)
{
        if (!svcnum)
        {
                LOG_ERR("ServiceCtrl(): none svcnum");
                return -1;
        }
        if (!ctrl)
        {
                LOG_ERR("ServiceCtrl(): none control");
                return -1;
        }

        std::string svcstr = SvcNumToName(svcnum);
        std::string ctrlstr;
        std::string cmd;
        int param = 0;

        if (svcstr.empty())
        {
                LOG_ERR("ServiceCtrl(): cannot dispatch service");
                return -1;
        }

        if (SC_START == ctrl)
        {
                ctrlstr = "start";
        }
        else if (SC_STOP == ctrl)
        {
                ctrlstr = "stop";
        }
        else if (SC_RESTART == ctrl)
        {
                ctrlstr = "restart";
        }
        else if (SC_RELOAD == ctrl)
        {
                ctrlstr = "reload";
        }
        else if (SC_RUNSTAT == ctrl)
        {
                return GetServiceStat(svcnum, param);
        }
        else if (SC_INITSTAT == ctrl)
        {
                return GetInitStat(svcnum, param);
        }
        else if (SC_INITON == ctrl || SC_INITOFF == ctrl)
        {
                return InitStart(svcnum, SC_INITOFF-ctrl);
        }

        if (ctrlstr.empty())
        {
                LOG_ERR("ServiceCtrl(): illegal control");
                return -1;
        }
        cmd = std::string("service ") + svcstr + " " + ctrlstr;
        if (-1 == system(cmd.c_str()))
        {
                LOG_ERR("ServiceCtrl(): system(): %s",
                        cmd.c_str());
                return -1;
        }
        LOG_DBG("ServiceCtrl() success: %s:%s", 
                svcstr.c_str(), ctrlstr.c_str());
        return param;
}

int CServiceMgr::InitStart(const int& svcnum, const bool& enable)
{
        if (!svcnum)
        {
                LOG_ERR("InitStart(): none service name");
                return -1;
        }
        std::string cmd = std::string("chkconfig --level 35 ") + SvcNumToName(svcnum);
        if (enable)
        {
                cmd += " on";
        }
        else
        {
                cmd += " off";
        }
        if (-1 == system(cmd.c_str()))
        {
                LOG_ERR("InitStart(): system() failed: %s", 
                        cmd.c_str());
                return -1;
        }
        LOG_DBG("InitStart() success: %s", cmd.c_str());
        return 0;
}

int CServiceMgr::GetAllServiceStat(std::vector<stServiceStat>& vsvc)
{
        vsvc.clear();
        vsvc.push_back(stServiceStat(SNC_SMB, "Samba/CIFS"));
        vsvc.push_back(stServiceStat(SNC_NFS, "NFS"));
        vsvc.push_back(stServiceStat(SNC_RSYNC, "Rsync"));
        vsvc.push_back(stServiceStat(SNC_FTP, "ProFTP"));
        //vsvc.push_back(stServiceStat(SNC_WEB, "WebDAV"));
        vsvc.push_back(stServiceStat(SNC_ISCSITARGET, "ISCSI-Target"));
        vsvc.push_back(stServiceStat(SNC_FC, "SCST(FC)"));
        vsvc.push_back(stServiceStat(SNC_UPS, "Apcupsd(UPS)"));
        vsvc.push_back(stServiceStat(SNC_DDFS, "DDFS"));

        int ret = 0;
        uint i = 0;
        for (i=0; i<vsvc.size(); ++i)
        {
                if (0 != GetServiceStat(vsvc[i].svcnum, vsvc[i].runstat, vsvc[i].initstat))
                {
                        ret = -1;
                }
        }

        if (0 != ret)
        {
                LOG_ERR("GetAllServiceStat(): failed");
                return -1;
        }

        LOG_DBG("GetAllServiceStat(): success");
        return 0;
}

int CServiceMgr::GetServiceStat(const int& svcnum, int& runstat)
{
        if (!svcnum)
        {
                LOG_ERR("GetServiceStat(): none service");
                return -1;
        }

        std::string svcstr = SvcNumToName(svcnum);
        std::string cmd = std::string("service ") + svcstr + " status";
        std::string strtmp;
        FILE* fifo = NULL;
        char cline[MAX_INFO_LEN] = {0};
        runstat = SVC_NUL;

        if (NULL == (fifo = popen(cmd.c_str(), "r")))
        {
                LOG_ERR("popen(): %s", cmd.c_str());
                return -1;
        }
        while (NULL != fgets(cline, MAX_INFO_LEN, fifo))
        {
                '\n' == *(cline+strlen(cline)-1) ? *(cline+strlen(cline)-1) = '\0' : 0;
                strtmp = cline;
                if (strtmp.find("stopped") != std::string::npos || 
                        strtmp.find("已停") != std::string::npos)
                {
                        runstat = 0;
                        break;
                }
                else if (strtmp.find("running") != std::string::npos || 
                        strtmp.find("正在运行") != std::string::npos)
                {
                        runstat = 1;
                }
        }
        if (-1 == pclose(fifo))
        {
                LOG_ERR("pclose(): %s", cmd.c_str());
        }
        LOG_DBG("GetServiceStat(): success");
        return runstat;
}

int CServiceMgr::GetServiceStat(const int& svcnum, int& runstat, int& initstat)
{
        if (!svcnum)
        {
                LOG_ERR("GetServiceStat(): none service");
                return -1;
        }

        if (-1 == GetServiceStat(svcnum, runstat))
        {
                LOG_DBG("GetServiceStat() failed: %d", svcnum);
        }
        if (-1 == GetInitStat(svcnum, initstat))
        {
                LOG_DBG("GetServiceStat() failed: %d", svcnum);
        }
        return 0;
}

int CServiceMgr::GetInitStat(const int& svcnum, int& initstat)
{
        if (!svcnum)
        {
                LOG_ERR("GetInitStat(): none service");
                return -1;
        }
        initstat = SVC_NUL;

        int chkon3      = SVC_NUL;
        int chkon5      = SVC_NUL;
        std::string svcstr = SvcNumToName(svcnum);
        if (svcstr.empty())
        {
                LOG_ERR("GetInitStat(): illegal service: %X", svcnum);
                return -1;
        }
        std::string cmd = 
        std::string("chkconfig --list ") + svcstr;// + " | awk '{print $5, $7}'";
        std::string strret;

        if (false == Popen(cmd.c_str(), strret))
        {
                LOG_ERR("Popen(): %s", cmd.c_str());
                return SVC_NUL;
        }

        if (std::string::npos != strret.find("3:启用") || std::string::npos != strret.find("3:on"))
        {
                chkon3 = 1;
        }
        else if (std::string::npos != strret.find("3:关闭") || std::string::npos != strret.find("3:off"))
        {
                chkon3 = 0;
        }
        if (std::string::npos != strret.find("5:启用") || std::string::npos != strret.find("5:on"))
        {
                chkon5 = 1;
        }
        else if (std::string::npos != strret.find("5:关闭") || std::string::npos != strret.find("5:off"))
        {
                chkon5 = 0;
        }

        if (1 == chkon3 && 1 == chkon5)
        {
                initstat = 1;
        }
        if (0 == chkon3 || 0 == chkon5)
        {
                initstat = 0;
        }
        if (-1 == chkon3 || -1 == chkon5)
        {
                initstat = SVC_NUL;
        }
        LOG_DBG("GetInitStat() success: %s %d", svcstr.c_str(), initstat);
        return initstat;
}

const char* CServiceMgr::SvcNumToName(const int& svcnum)
{
        if (SNC_SMB == svcnum)
        {
                return SN_SMB;
        }
        else if (SNC_NFS == svcnum)
        {
                return SN_NFS;
        }
        else if (SNC_RSYNC == svcnum)
        {
                return SN_RSYNC;
        }
        else if (SNC_FTP == svcnum)
        {
                return SN_FTP;
        }
        else if (SNC_WEB == svcnum)
        {
                return SN_WEB;
        }
        else if (SNC_ISCSITARGET == svcnum)
        {
                return SN_ISCSITARGET;
        }
        else if (SNC_NETWORK == svcnum)
        {
                return SN_NETWORK;
        }
        else if (SNC_DDFS == svcnum)
        {
                return SN_DDFS;
        }
        else if (SNC_FC == svcnum)
        {
                return SN_FC;
        }
        else if (SNC_UPS == svcnum)
        {
                return SN_UPS;
        }
        return NULL;
}

const char* CServiceMgr::ControlNumToStr(const int& ctrl)
{
        const char* str = NULL;
        switch (ctrl)
        {
        case SC_START:
                str = "start";
                break;
        case SC_STOP:
                str = "stop";
                break;
        case SC_RESTART:
                str = "restart";
                break;
        case SC_RELOAD:
                str = "reload";
                break;
        case SC_RUNSTAT:
                str = "runstat";
                break;
        case SC_INITSTAT:
                str = "initstat";
                break;
        case SC_INITON:
                str = "chkon";
                break;
        case SC_INITOFF:
                str = "chkoff";
                break;
        default:
                break;
        }
        return str;
}
