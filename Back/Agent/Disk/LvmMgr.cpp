#include "LvmMgr.h"

#include "Utils/Log/Log.h"
#include "Include/LvmConf.h"
#include "Utils/Shell/System.h"
#include "Utils/Common/CommonFunc.h"
#include "Public/Mysql/Mysql.h"
#include "Utils/Shell/ShellOpr.h"
#include <stdio.h>

using std::string;
using std::vector;

// 1=普通共享;2=消冗共享;3=FCSAN;4=IPSAN;
const char* LvmMgr::LV_STR[] = {NULL, "share", "ddfs", "fc", "iscsi", NULL};

bool LvmMgr::InitDisk()
{
        bool  bRet = false;
        char* cmd = NULL;

        cmd = s_printf("%s %s%s", PERL_HEAD, MAIN_PATH, PL_DISK_INIT);

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::GetVgList(VgArray &vgArray)
{
        bool bRt = true;

        std::string path;
        path = "vgs --unit m --noheading --separator ',' -o vg_name,vg_size,vg_free,lv_count";

        std::string result;
        if (!Popen(path.c_str(), result)) {
                LOG_ERR("Popen cmd (%s) error", path.c_str());
        }
        Trim(result, " \r\n");

        strArray vglist;
        Split(result.c_str(), vglist, '\n');

        for (uint i = 0; i < vglist.size(); ++i)
        {
                strArray infolist;
                Split(vglist[i].c_str(), infolist, ',');

                if (infolist.size() < 4) {
                        LOG_ERR("the size (%d) is error", infolist.size());
                        break;
                }

                //int    lvCount = 0;
                VgInfo vg;
                vg.name = infolist[0];
                sscanf(infolist[1].c_str(), "%d", &vg.vgSize);//vg.vgSize      = atoi(list[1].c_str());
                sscanf(infolist[2].c_str(), "%d", &vg.vgFree);//vg.vgFree      = atoi(list[2].c_str());
                sscanf(infolist[3].c_str(), "%d", &vg.lvcount);//vg.lvcount     = atoi(list[3].c_str());

                //LvInfo lv;
                //vg.innerLvs.assign(lvCount, lv);

                vgArray.push_back(vg);
        }

        return bRt;
}

bool LvmMgr::GetVgInfo(const std::string &vgName, VgInfo &vg)
{
        bool bRt = true;

        std::string path;
        path = GW_BIN_PATH;
        path = path + PL_SINGEL_VG + " ";
        path += vgName;

        std::string result;
        if (!Popen(path.c_str(), result)) {
                LOG_ERR("Popen cmd (%s) error", path.c_str());
        }
        Trim(result, " \r\n");

        strArray arr1;
        Split(result.c_str(), arr1, '\n');
        for (uint i = 0; i < arr1.size(); ++i)
        {
                strArray list;
                Split(arr1[i].c_str(), list, ',');

                if (list.size() < 4) {
                        LOG_ERR("the size (%d) is error", list.size());
                        break;
                }

                if (i == 0) {
                        vg.name   = list[1];
                        vg.vgSize = atoi(list[2].c_str());
                        vg.vgFree = atoi(list[3].c_str());
                }
                else {
                        LvInfo lv;
                        lv.lvPath = list[0];
                        lv.lvSize = atoi(list[1].c_str());
                        lv.fsSize = atof(list[2].c_str());
                        lv.fsFree = atof(list[3].c_str());
                        vg.innerLvs.push_back(lv);
                }
        }

        return bRt;
}

bool LvmMgr::MergeVg(const std::string &destVg, const std::string &srcVg)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("vgmerge %s %s", destVg.c_str(), srcVg.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::Rmpv2vg(const std::string &vgName, const std::string &pvName)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("vgreduce %s %s", vgName.c_str(), pvName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::RmMissing(const std::string &vgName)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("vgreduce --removemissing %s", vgName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::RmVg(const std::string &vgName)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("vgremove %s", vgName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::RmPv(const std::string& pvName)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("pvremove %s", pvName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::GetLvList(std::vector<LvInfo>& vl)
{
        const char* cmd = GW_BIN_PATH"lvs.sh";
        bool bRet = true;

        string result;
        if (!Popen(cmd, result)) {
                LOG_ERR("Popen cmd (%s) error", cmd);
                return false;
        }
        Trim(result, " \r\n");

        vector<string> lvlist;
        Split(result.c_str(), lvlist, '\n');
        for (uint i = 0; i < lvlist.size(); ++i)
        {
                vector<string> lvinfo;
                Split(lvlist[i].c_str(), lvinfo, ',');

                if (lvinfo.size() < 4) {
                        LOG_ERR("the lvinfo size (%d) is error", lvinfo.size());
                        continue;
                }

                LvInfo lv;
                lv.lvPath = "/dev/"+lvinfo[1]+"/"+lvinfo[0];
                sscanf(lvinfo[2].c_str(), "%d", &lv.lvSize);//lv.lvSize = atoi(list[1].c_str());
                //lv.fsSize = atof(list[2].c_str());
                sscanf(lvinfo[3].c_str(), "%lf", &lv.fsFree);//lv.fsFree = atof(list[3].c_str());
                vl.push_back(lv);
        }

        return bRet;
}

bool LvmMgr::CreateLv(const std::string& lvName, const std::string& vgName, int size /*MB*/)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("lvcreate -L %d -n %s %s", size, lvName.c_str(), vgName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::DeleteLv(const std::string& lvPath)
{
        char* cmd = NULL;
        bool  bRet = false;

        cmd = s_printf("lvremove -f %s", lvPath.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}

bool LvmMgr::ExtentLv(const std::string& lvName, int size /*MB*/)
{
        bool  bRet = false;
        char* cmd = NULL;

        if (size > 0){
                cmd = s_printf("lvextend -L +%d %s", size, lvName.c_str());
        }
        else if (size < 0){
                cmd = s_printf("lvreduce -f -L -%d %s", -size, lvName.c_str());
        }

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!ExeCmd(cmd)) {
                LOG_ERR("Execmd (%s) error", cmd);
        }
        else {
                bRet = true;
        }

        free(cmd);
        return bRet;
}
bool LvmMgr::GetLvSize(const std::string& lvPath, uint64_t& diskSize)
{
        char* cmd  = NULL;
        bool  bRet = false;

        std::string result;

        cmd = s_printf("lvs -o lv_size --noheadings --unit m %s", lvPath.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
        }
        else {
                Trim(result, " mM");
                diskSize = atoll(result.c_str());
                bRet = true;
        }
        free(cmd);

        return bRet;
}

bool LvmMgr::GetPvInfo(const std::string& pvName, std::string& vgName, int& pvCount, bool& removable)
{
        char* cmd  = NULL;
        bool  bRet = false;

        if (pvName.empty()) {
                vgName.clear();
                pvCount = 0;
                removable = true;
                return bRet;
        }

        std::string result;

        cmd = s_printf("pvs --separator ',' --unit m --noheadings -o vg_name,pv_used,pv_count %s", pvName.c_str());

        if (cmd == NULL) {
                LOG_ERR("s_printf error");
        }
        else if (!Popen(cmd, result)) {
                LOG_ERR("Popen (%s) error", cmd);
        }
        else if (!result.empty()){

                std::vector<std::string> arr1;
                Split(result.c_str(), arr1, ',');

                if (arr1.size() < 3) {
                        LOG_ERR("Popen (%s) result (%s) error", cmd, result.c_str());
                }
                else {
                        vgName    = arr1[0];
                        removable = atoi(arr1[1].c_str()) == 0 ? true : false;
                        pvCount   = atoi(arr1[2].c_str());
                        bRet      = true;
                }
        }
        free(cmd);

        return bRet;
}
