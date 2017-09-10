#ifndef __LVM_BASE_H__
#define __LVM_BASE_H__

#include <vector>
#include <string>
#include <stdint.h>

class LvmMgr 
{
public:
        // lv的类型
        // 1=普通共享;2=消冗共享;3=FCSAN;4=IPSAN;
        enum LV_TYPE {
                LV_MIN = 0,
                LV_SHARE,
                LV_DDFS,
                LV_FC,
                LV_ISCSI,
                LV_MAX
                };
        static const char* LV_STR[];

        struct LvInfo
        {
                std::string lvPath; // lv路径
                int         lvSize; // lv大小MB

                double fsSize;
                double fsFree;

                LvInfo(std::string p = ""){
                        lvPath    = p;
                        lvSize    = 0;
                        fsSize    = 0;
                        fsFree    = 0;
                }
                bool operator==(const LvmMgr::LvInfo& li)
                {
                        return li.lvPath == lvPath;
                }
        };
        typedef std::vector<LvInfo> LvArray;

        // vg struct
        struct VgInfo
        {
                std::string name;     // VG名字
                int         vgSize;   // VG大小MB
                int         vgFree;   // 剩余空间大小MB
                int         lvcount;
                LvArray     innerLvs; // 属于该VG的LV

                VgInfo(){
                        vgSize = 0;
                        vgFree = 0;
                        lvcount = 0;
                }
        };

        typedef std::vector<VgInfo>      VgArray;
        typedef std::vector<std::string> strArray;

public:
        // 其它操作
        // 初始化磁盘操作
        static bool InitDisk();

// PV操作
        //      获取对应PV的 vg_name,pv_count,pv_free
        static bool GetPvInfo(const std::string& pvName, std::string& vgName, int& pvCount, bool& removable);
        //      删除对应的PV
        static bool RmPv(const std::string& pvName);
        //      删除丢失的PV
        static bool RmMissing(const std::string &vgName);

// VG操作
        //      获取所有的vgInfo
        static bool GetVgList(VgArray &vgArray);
        //      获取某个VG的信息
        static bool GetVgInfo(const std::string &vgName, VgInfo &vg);
        //      合并两个VG
        static bool MergeVg(const std::string &destVg, const std::string &srcVg);
        //      删除 PV 2 VG
        static bool Rmpv2vg(const std::string &vgName, const std::string &pvName);
        //      删除 VG
        static bool RmVg(const std::string &vgName);

// LV操作
        //      获取所有lv
        static bool GetLvList(std::vector<LvInfo>& vl);
        //      创建LV
        static bool CreateLv(const std::string& lvName, const std::string& vgName, int size /*MB*/);
        //      删除LV
        static bool DeleteLv(const std::string& lvPath);
        //      动态改变LV大小
        static bool ExtentLv(const std::string& lvName, int size /*MB*/);
        //      获取 LV 大小信息 MB
        static bool GetLvSize(const std::string& lvPath, uint64_t& diskSize);
};

#endif /* __LVM_BASE_H__ */
