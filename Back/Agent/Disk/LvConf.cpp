#include "LvConf.h"
#include "Include/LvmConf.h"
#include "Utils/Log/Log.h"
#include "Utils/Config/IniConfig.h"
#include "Utils/Common/CommonFunc.h"

//
// LvConf
//
LvConf::LvConf(std::string path):
        m_path(path)
{}

LvConf::LvConf(const char* path):
        m_path(path)
{
        if (m_path.empty()) {
                m_path = m_path + MAIN_PATH + CONF_LVTYPE;
        }
}

LvConf::~LvConf()
{}

bool LvConf::Compare(const std::string& src, const std::string& dst, int &rt)
{
        rt = 0;

        if (src.empty() || dst.empty()) {
                return false;
        }

        std::vector<std::string> ary;
        if (!Split(src.c_str(), ary, ',')) {
                LOG_ERR("Splite (%s) with (,) error", src.c_str());
        }
        else if (ary.size() > 0) {
                for (uint i = 0; i < ary.size(); ++i)
                {
                        if (i != 0) {
                                rt ++;
                        }
                        if (ary[i] == dst) {
                                return true;
                        }
                        rt += ary[i].size();
                }
        }
        return false;
}

bool LvConf::AddLvConf(const std::string& path, int type, int status)
{
        bool bRet = false;

        std::string value;
        std::string key  = "V0";
        std::string key1 = "V1";
        std::string area;

        int  rt     = 0;
        bool bWrite = false;

        area = '[';
        area = area + LvmMgr::LV_STR[type] + ']';

        if (status == true) {
                key  = "V1";
                key1 = "V0";
        }

        IniConfig parser(m_path);
        if (parser.Open() < 0) {
                LOG_ERR("IniConfig::Load  path is (%s), error", m_path.c_str());
        }
        else if (parser.GetVal(area, key, value) != 0) {
                LOG_ERR("IniConfig::GetVal error, area (%s) key (%s)", area.c_str(), key.c_str());
        }
        //当在该条目未找到时，则添加进入该项
        else if (!Compare(value, path, rt)) {
                if (value.empty()) {
                        value = path;
                }
                else {
                        value = value + "," + path;
                }
                if ( parser.SetVal(area, key, value) != 0 ) {
                        LOG_ERR("IniConfig::SetVal error, area (%s), key (%s), value(%s)",
                                area.c_str(), key.c_str(), value.c_str());
                }
                bWrite = true;
        }

        // 检查另外一面是否有该项，有的话则删除掉
        if (parser.GetVal(area, key1, value) != 0) {
                LOG_ERR("IniConfig::SetVal error, area (%s), key (%s), value(%s)",
                        area.c_str(), key1.c_str(), value.c_str());
        }
        else if (Compare(value, path, rt)) {

                int size = path.size();
                int delSize = size;
                if (rt > 0) {
                        rt--; // 去掉前面的，号
                        delSize++;
                }
                else if (rt + size + 1  < (int)value.size()) {
                        delSize ++;//去掉后面的，号
                }
                value.erase(rt, delSize);
                if ( parser.SetVal(area, key1, value) != 0 ) {
                        LOG_ERR("IniConfig::SetVal error, area (%s), key (%s), value(%s)",
                                area.c_str(), key1.c_str(), value.c_str());
                }
                bWrite = true;
        }

        if (bWrite && parser.Close() != 0) {
                LOG_ERR("IniConfig::Write to (%s) error", m_path.c_str());
        }
        else {
                bRet = true;
        }

        return bRet;
}

bool LvConf::RemLvConf(const std::string& path, int type)
{
        bool bRet = false;

        std::string value;
        std::string key  = "V0";
        std::string area = "[";

        area = area + LvmMgr::LV_STR[type] + ']';

        IniConfig parser(m_path);
        int         rt = 0;
        if (parser.Open() < 0) {
                LOG_ERR("IniConfig::Open path is (%s), error", m_path.c_str());
                return false;
        }

        for(int i = 0; i < 2; i++)
        {
                if (parser.GetVal(area, key, value) != 0) {
                        LOG_ERR("IniConfig::GetVal error, area (%s) key (%s)",
                                area.c_str(), key.c_str());
                }
                else if ( !Compare(value, path, rt) ) {

                        if (key != "V1") {
                                key = "V1";
                                continue;
                        }
                }
                else {
                        int size = path.size();
                        int delSize = size;
                        if (rt > 0) {
                                rt--; // 去掉前面的，号
                                delSize++;
                        }
                        else if (rt + size + 1  < (int)value.size()) {
                                delSize ++;//去掉后面的，号
                        }
                        value.erase(rt, delSize);

                        if (parser.SetVal(area, key, value) != 0) {
                                LOG_ERR("IniConfig::SetVal error, area (%s), key (%s), value(%s)",
                                        LvmMgr::LV_STR[type], key.c_str(), value.c_str());
                        }
                        else if (parser.Close() != 0) {
                                LOG_ERR("IniConfig::Write to (%s) error", m_path.c_str());
                        }
                        else {
                                bRet = true;
                        }
                }
        }

        return bRet;
}

bool LvConf::ListLvConf(int type, int status, std::string& list)
{
        bool bRet = false;

        std::string value;
        std::string area;
        std::string key;

        if (status == 0) {
                key = "V0";
        }
        else {
                key = "V1";
        }

        area = '[';
        area = area + LvmMgr::LV_STR[type] + ']';

        IniConfig parser(m_path);
        if (parser.Open() < 0) {
                LOG_ERR("IniConfig::Load  path is (%s), error", m_path.c_str());
        }
        else if (parser.GetVal(area, key, value) != 0) {
                LOG_ERR("IniConfig::GetVal error, area (%s) key (%s)", area.c_str(), key.c_str());
        }
        else {
                list = value;
                bRet = true;
        }

        return bRet;
}

bool LvConf::GetLvConf(const std::string& path, int &type, int &status)
{
        bool bRet = false;
        
        std::string value;
        std::string key  = "V0";
        std::string key1 = "V1";
        std::string area;

        int rt;
        rt = type = status = 0;

        IniConfig parser(m_path);
        if (parser.Open() < 0) {
                LOG_ERR("IniConfig::Load  path is (%s), error", m_path.c_str());
        }
        else {
                
                for (int i = LvmMgr::LV_MIN + 1; i < LvmMgr::LV_MAX; ++i)
                {
                        area = '[';
                        area = area + LvmMgr::LV_STR[i] + ']';

                        if (parser.GetVal(area, key, value) != 0) {
                                 LOG_ERR("IniConfig::GetVal error, area (%s) key (%s)",
                                         area.c_str(), key.c_str());
                                 break;
                         }
                         else if (Compare(value, path, rt)) {
                                 type = i;
                                 status = 0;
                                 bRet = true;
                                 break;
                         }

                         if (parser.GetVal(area, key1, value) != 0) {
                                 LOG_ERR("IniConfig::GetVal error, area (%s) key (%s)",
                                         area.c_str(), key.c_str());
                                 break;
                         }
                         else if (Compare(value, path, rt)) {
                                 type = i;
                                 status = 1;
                                 bRet = true;
                                 break;
                         }
                }
        }
        return bRet;
}

