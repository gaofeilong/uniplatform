#include "FcMgr.h"
#include "Include/LvmConf.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/System.h"
#include "Utils/Common/CommonFunc.h"

//
// FcMgr
//
FcMgr::FcMgr()
{}

FcMgr::~FcMgr()
{}

const char* FcMgr::m_handler = "vdisk_blockio";
const char* FcMgr::m_driver  = "qla2x00t";

int FcMgr::AddDevice(const std::string& devName, const std::string& devPath)
{
        int   bRet = false;
        char *cmd  = NULL;

        cmd = s_printf("scstadmin -noprompt -open_dev %s -handler %s -attributes filename=%s",
                       devName.c_str(), m_handler, devPath.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execute cmd(%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);

        return bRet;
}

int FcMgr::DeleteDevice(const std::string& devName)
{
        int   bRet = false;
        char *cmd  = NULL;

        cmd = s_printf("scstadmin -noprompt -close_dev %s -handler %s",
                       devName.c_str(), m_handler);

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execute cmd(%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);

        return bRet;
}

int FcMgr::EnableTarget(const std::string& target)
{
        int   bRet = false;
        char *cmd  = NULL;

        cmd = s_printf("scstadmin -noprompt -enable_target %s -driver %s", target.c_str(), m_driver);

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execute cmd(%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);

        return bRet;
}

int FcMgr::AddLun(int lunNum, const std::string& target, const std::string& devName)
{
        int   bRet = false;
        char *cmd  = NULL;

        cmd = s_printf("scstadmin -noprompt -add_lun %d -driver %s -target %s -device %s",
                       lunNum, m_driver, target.c_str(), devName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execute cmd(%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);

        return bRet;
}

int FcMgr::DeleteLun(int lunNum, const std::string& target, const std::string& dev)
{
        int   bRet = false;
        char *cmd  = NULL;

        cmd = s_printf("scstadmin -noprompt -rem_lun %d -driver %s -target %s -device %s",
                       lunNum, m_driver, target.c_str(), dev.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execute cmd(%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);

        return bRet;
}

int FcMgr::ListDevice(std::vector<Target>& targets)
{
        int   bRet = false;
        char *cmd  = NULL;

        std::string result;

        cmd = s_printf("%s %s/%s %s", PERL_HEAD, MAIN_PATH, PL_FC, CONF_FCCONF);

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!Popen(cmd, result)) {
                LOG_ERR("Popen cmd(%s) error", cmd);
        }
        else if (result.size() < 0) {
                LOG_ERR("Popen reslult size < 0,  cmd is (%s)", cmd);
        }
        else {
                vector<string> array1;
                if (Split(result.c_str(), array1, '\n')) {
                        for (uint i = 0; i < array1.size(); ++i)
                        {
                                vector<string> array2;
                                if (Split(array1[i].c_str(), array2, ',')) {

                                        int size = array2.size() / 3;

                                        Target tar;
                                        tar.name = array2[0];

                                        for (int i = 0; i < size; ++i)
                                        {
                                                Lun lun;
                                                lun.lun     = atoi(array2[i*3+1].c_str());
                                                lun.devname = array2[i*3+2];
                                                lun.path    = array2[i*3+3];
                                                tar.luns.push_back(lun);
                                                tar.nextlun = tar.nextlun < lun.lun+1 ? lun.lun+1 : tar.nextlun;
                                        }
                                        targets.push_back(tar);
                                }
                        }
                }
                bRet = true;
        }

        free(cmd);
        return bRet;
}

int FcMgr::WriteConfig(const std::string& path)
{
        int   bRet = false;
        char *cmd  = NULL;

        cmd = s_printf("scstadmin -noprompt -write_config %s", path.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execute cmd(%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);

        return bRet;
}

std::string FcMgr::SetName(const std::string& name, int lun)
{
        std::string dev;

        char a = '0' + lun;

        if (name.size() > 2) {
                dev = "disk";
                dev += name.substr(name.size()-2, 2);
                dev = dev + a;
        }

        return dev;
}

// int FcMgr::ListTargets(vector<string>& targets)
// {
//         int   bRet = false;
//         char *cmd  = NULL;

//         std::string result;

//         cmd = s_printf("cat /sys/class/fc_host/host*/port_name");

//         if (cmd == NULL) {
//                 LOG_ERR("s_printf error", cmd);
//         }
//         else if (!Popen(cmd, result)) {
//                 LOG_ERR("Execute cmd(%s) error", cmd);
//         }
//         else {
//                 bRet = true;
//         }

//         if (result.size() > 0) {
//                 Split(result.c_str(), targets, '\n');
//         }

//         // change every item from 0x21000024ff349c3c to 21:00:00:24:ff:34:9c:3c
//         for (uint i = 0; i < targets.size(); ++i)
//         {
//                 char temp[30] = {0};
//                 for (uint j = 2, h = 0; j < strlen(targets[i].c_str()); ++j)
//                 {
//                         if (j != 2 && j % 2 == 0) {
//                                 temp[h++] = ':';
//                         }
//                         temp[h++] = targets[i][j];
//                 }
//                 targets[i] = temp;
//         }

//         free(cmd);

//         return bRet;
// }
