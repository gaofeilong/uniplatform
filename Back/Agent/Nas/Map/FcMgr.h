#ifndef __FCOPER_H__
#define __FCOPER_H__

#include <string>
#include <vector>

using std::string;
using std::vector;

class FcMgr
{
public:
        FcMgr();
        ~FcMgr();

private:
        const static char* m_handler; //"vdisk_blockio";
        const static char* m_driver; // "qla2x00t";

public:
        typedef struct Lun
        {
                int    lun;
                string devname;
                string path;
                Lun()
                    : lun(0) {}
                Lun(std::string p, std::string d, int l = 0)
                    : lun(l), devname(d), path(p) {}
                Lun(int l, std::string p, std::string d = "")
                    : lun(l), devname(d), path(p) {}
                bool operator==(const Lun& ol) const
                {
                        return path == ol.path;
                }
        }Lun;

        typedef struct Target
        {
                string      name;
                vector<Lun> luns;
                int         nextlun;
                Target()
                    : nextlun(0) {}
                Target(std::string s)
                    : name(s), nextlun(0) {}
                unsigned int GetNextLun() const
                {
                        int num = 0, zero_exist = 0;
                        for (unsigned int i=0; i<luns.size(); ++i)
                        {
                                if (luns[i].lun > num)
                                {
                                        num = luns[i].lun;
                                }
                                if (0 == luns[i].lun)
                                {
                                        zero_exist = 1;
                                }
                                num = i;
                        }
                        return zero_exist ? num+1 : 0;
                }
                bool operator==(const Target& ot) const
                {
                        return name == ot.name;
                }
        }Target;

public:

        // 启动 FC 卡设备
        int EnableTarget(const std::string& target);
        // 添加路径成FC设备
        int AddDevice(const std::string& devName, const std::string& devPath);
        // 删除某路径所对应的FC设备
        int DeleteDevice(const std::string& devName);
        // 添加路径至 LUN 设备
        int AddLun(int lunNum, const std::string& target, const std::string& devName);
        // 删除 LUN 所对应的 FC 设备
        int DeleteLun(int lunNum, const std::string& target, const std::string& dev);
        // 列出所有的 FC 设备
        int ListDevice(std::vector<Target> &targets);
        // 写入到配置文件
        int WriteConfig(const std::string& path);
        // 根据 target, lun 号来生成 device 名称
        std::string SetName(const std::string& name, int lun);
        // 列出所有的 target
        //int ListTargets(vector<string>& targets);
};

#endif /* __FCOPER_H__ */
