#include "SysTimeMgr.h"

#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

/*
vector<string> SysTimeMgr::m_Zone;
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-1");  //阿姆斯特丹
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-2");  //布加勒斯特
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-3");  //莫斯科时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-4");  //第比利斯时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-5");  //新德里时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-6");  //科伦坡时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-7");  //曼谷雅加达
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-8");  //北京香港新加坡
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-9");  //东京平壤时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-10"); //悉尼关岛时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-11"); //所罗门群岛
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-12"); //惠灵顿时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT");    //格林威治标准时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+1");  //佛德尔群岛
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+2");  //大西洋中部
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+3");  //布宜诺斯艾利斯
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+4");  //智利和巴西
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+5");  //纽约渥太华
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+6");  //墨西哥城时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+7");  //美国丹佛时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+8");  //旧金山时间
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+9");  //阿拉斯加
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+10"); //夏威夷群岛
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+11"); //东萨摩亚群岛
SysTimeMgr::m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+12"); //艾尼威托克岛
*/

SysTimeMgr::SysTimeMgr()
{
        /* 这里的 /usr/share/zoneinfo/Etc/GMT-N 对应的 date -R 为 +0N00 */
        /*
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-1");  //阿姆斯特丹
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-2");  //布加勒斯特
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-3");  //莫斯科时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-4");  //第比利斯时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-5");  //新德里时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-6");  //科伦坡时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-7");  //曼谷雅加达
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-8");  //北京香港新加坡
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-9");  //东京平壤时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-10"); //悉尼关岛时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-11"); //所罗门群岛
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT-12"); //惠灵顿时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT");    //格林威治标准时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+1");  //佛德尔群岛
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+2");  //大西洋中部
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+3");  //布宜诺斯艾利斯
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+4");  //智利和巴西
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+5");  //纽约渥太华
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+6");  //墨西哥城时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+7");  //美国丹佛时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+8");  //旧金山时间
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+9");  //阿拉斯加
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+10"); //夏威夷群岛
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+11"); //东萨摩亚群岛
        m_Zone.push_back("/usr/share/zoneinfo/Etc/GMT+12"); //艾尼威托克岛
        */
}

SysTimeMgr::~SysTimeMgr()
{
}

int SysTimeMgr::GetTime(string& timeStr)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_TIME, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_TIME.c_str());
                return -1;
        }
        timeStr = buf[0];
        return 0;
}

int SysTimeMgr::SetTime(const string& timeStr)
{
        /* timerStr 格式必须为："yyyy-mm-dd HH:MM:SS" */
        int ret;
        vector<string> buf;

        ret = Execute(CL_SET_TIME(timeStr), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_SET_TIME(timeStr).c_str());
                return -1;
        }
        return 0;
}

int SysTimeMgr::GetNtpServer(string& ip)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_NTP_SERVER, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_NTP_SERVER.c_str());
                return -1;
        }
        ip = buf[0];
        return 0;
}

int SysTimeMgr::SetNtpServer(const string& ip)
{
        int ret;
        vector<string> buf;
        string ntp = ip.empty()? "clear": ip;

        ret = Execute(CL_SET_NTP_SERVER(ntp), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_SET_NTP_SERVER(ntp).c_str());
                return -1;
        }

        return 0;
}

// GMT+N
int SysTimeMgr::GetZone(string &zone)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_TIME_ZONE, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_TIME_ZONE.c_str());
                return -1;
        }
        zone = buf[0];
        return 0;
}

int SysTimeMgr::SetZone(const string &zone)
{
        int ret;
        vector<string> buf;
        string file = "/usr/share/zoneinfo/Etc/";
        file += zone;

        ret = Execute(CL_SET_TIME_ZONE(file), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_SET_TIME_ZONE(file).c_str());
                return -1;
        }
        return 0;
}

int SysTimeMgr::SyncTime(const string& ip)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_SYNC_TIME(ip), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_SYNC_TIME(ip).c_str());
                return -1;
        }
        return 0;
}
