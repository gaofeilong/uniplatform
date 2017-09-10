#ifndef __LV_CONF_H__
#define __LV_CONF_H__

#include "LvmMgr.h"

class LvConf : public LvmMgr
{
        std::string m_path;

private:
        // 比较 src 中, dst 是否存在， 其中 rt 指的是返回 dst 所在 src 中开始部分的长度
        bool Compare(const std::string& src, const std::string& dst, int& rt);

public:
        LvConf(std::string conf);
        LvConf(const char* conf = "");
        virtual ~LvConf();

        // 关于配置文件的操作, 此时的status只有0与1两种，0为未使用，1为使用
        // 把某个LV的类型写入配置文件，分别为 iscsi, ddfs, fc...，并写入状态
        // 若存在，返回正确；若不存在，则添加；若状态相异，则改变状态
        bool AddLvConf(const std::string& path, int type, int status);
        // 把某个LV从配置文件中删除掉, 若不存在，返回FALSE
        bool RemLvConf(const std::string& path, int type);
        // 列出某个类型的所有的LV及其状态信息 
        bool ListLvConf(int type, int status, std::string& list);
        // 获取某个路径的类型与状态， false 时未找到
        bool GetLvConf(const std::string& path, int &type, int &status);
};

#endif /* __LV_CONF_H__ */
