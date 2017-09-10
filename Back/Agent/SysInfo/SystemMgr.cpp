#include "SystemMgr.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>

#include "Include/String.h"
#include "Include/Types.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

SystemMgr::SystemMgr()
{
}

SystemMgr::~SystemMgr()
{
}

int SystemMgr::GetOsInfo(struct OsInfo &info) 
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_SYS, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_SYS.c_str());
                return -1;
        }
        info.Release = buf[0];
        info.Kernel  = buf[1];
        info.Scigw   = buf[2];
        return 0;
}

int SystemMgr::GetCpuInfo(vector<struct CpuInfo> &info) 
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_CPU, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_CPU.c_str());
                return -1;
        }

        int record = 4;
        for (size_t i = 0; i < buf.size() / record; i++) {
                struct CpuInfo cpu;
                cpu.Name   = buf[i * record];
                cpu.Vendor = buf[i * record + 1];
                cpu.Freq   = buf[i * record + 2];
                cpu.Cache  = buf[i * record + 3];
                info.push_back(cpu);
        }
        return 0;
}

int SystemMgr::GetMemoryInfo(struct MemoryInfo &info) 
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_MEM, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_MEM.c_str());
                return -1;
        }
        info.PTotal = buf[0];
        return 0;
}

int SystemMgr::GetDiskInfo(vector<struct DiskInfo> &info)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_DISK, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_DISK.c_str());
                return -1;
        }
        for (size_t i = 0; i < buf.size() / 3; i++) {
                struct DiskInfo disk;
                disk.Name   = buf[i * 3];
                disk.Size   = buf[i * 3 + 1];
                disk.Vendor = buf[i * 3 + 2];
                info.push_back(disk);
        }
        return 0;
}

int SystemMgr::GetNetInfo(vector<struct NetInfo> &info)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_NET_NAME, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_NET_NAME.c_str());
                return -1;
        }
        for (size_t i = 0; i < buf.size(); i++) {
                struct NetInfo net;
                net.Name = buf[i];
                info.push_back(net);
        }

        vector<NetInfo>::iterator vnIt;
        for (vnIt = info.begin(); vnIt != info.end(); ++vnIt) {
                ret = Execute(CL_GET_NET_INFO(vnIt->Name.c_str()), buf);
                if (ret != 0) {
                        LOG_ERR("execute cmd [%s] error", 
                                CL_GET_NET_INFO(vnIt->Name.c_str()).c_str());
                        return -1;
                }
                for (size_t i = 0; i < buf.size(); i++) {
                        vnIt->Vendor = buf[7];
                        vnIt->State  = buf[8];
                }
        }
        return 0;
}

int SystemMgr::GetFanInfo(vector<struct FanInfo> &info)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_FAN, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_FAN.c_str());
                return -1;
        }
        for (size_t i = 0; i < buf.size() / 3; i++) {
                struct FanInfo fan;
                fan.Name  = buf[i * 3];
                fan.RPM   = buf[i * 3 + 1];
                fan.State = buf[i * 3 + 2];
                info.push_back(fan);
        }
        return 0;
}

int SystemMgr::GetTempInfo(vector<struct TempInfo> &info)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_TEMP, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_TEMP.c_str());
                return -1;
        }
        for (size_t i = 0; i < buf.size() / 3; i++) {
                struct TempInfo temp;
                temp.Name        = buf[i * 3];
                temp.Temperature = buf[i * 3 + 1];
                temp.State       = buf[i * 3 + 2];
                info.push_back(temp);
        }
        return 0;
}

int SystemMgr::GetSystemInfo(struct SystemInfo &info)
{
        if (GetOsInfo(info.Os) != 0) {
                LOG_ERR("get os info error");
                // return -1;
        }
        if (GetCpuInfo(info.Cpu) != 0) {
                LOG_ERR("get cpu info error");
                // return -1;
        }
        if (GetMemoryInfo(info.Mem) != 0) {
                LOG_ERR("get mem info error");
                // return -1;
        }
        if (GetDiskInfo(info.Disk) != 0) {
                LOG_ERR("get disk info error");
                // return -1;
        }
        if (GetNetInfo(info.Net) != 0) {
                LOG_ERR("get net info error");
                // return -1;
        }
        // if (GetFanInfo(info.Fan) != 0) {
        //         LOG_ERR("get fan info error");
        //         // return -1;
        // }
        // if (GetTempInfo(info.Temp) != 0) {
        //         LOG_ERR("get temp info error");
        //         // return -1;
        // }

        return 0;
}
