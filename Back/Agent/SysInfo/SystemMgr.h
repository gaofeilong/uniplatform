#ifndef _SYSTEM_MGR_H_
#define _SYSTEM_MGR_H_

#include <string>
#include <vector>
using std::string;
using std::vector;

struct CpuInfo{
        string Name;       /* 主频   */
        string Vendor;     /* 型号   */
        string Freq;       /* 主频   */
        string Cache;      /* 缓存   */
};

struct MemoryInfo {
        string PTotal;     /* 物理总内存   */
};

struct OsInfo{
        string Release;    /* 系统版本 */
        string Kernel;     /* 内核版本 */
        string Scigw;      /* 软件版本 */
};

struct DiskInfo {
        string Name;
        string Size;
        string Vendor;
};

struct NetInfo {
        string Name;
        string Vendor;
        string State;
};

struct FanInfo {
        string Name;
        string RPM;
        string State;
};

struct TempInfo {
        string Name;
        string Temperature;
        string State;
};

struct SystemInfo {
        struct OsInfo Os;
        struct MemoryInfo Mem;
        vector<struct CpuInfo> Cpu;
        vector<struct DiskInfo> Disk;
        vector<struct NetInfo> Net;
        vector<struct FanInfo> Fan;
        vector<struct TempInfo> Temp;
};

class SystemMgr {
public:
        SystemMgr();
        ~SystemMgr();

public:
        int GetSystemInfo(struct SystemInfo &info);

private:
        int GetOsInfo(struct OsInfo &info);
        int GetCpuInfo(vector<struct CpuInfo> &info);
        int GetMemoryInfo(struct MemoryInfo &info);
        int GetDiskInfo(vector<struct DiskInfo> &info);
        int GetNetInfo(vector<struct NetInfo> &info);
        int GetFanInfo(vector<struct FanInfo> &info);
        int GetTempInfo(vector<struct TempInfo> &info);

private:
        struct SystemInfo Sysinfo;
};

#endif  //_SYSTEM_MGR_H_
