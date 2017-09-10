#ifndef __SERVICEMGR_H__
#define __SERVICEMGR_H__

#include <string>
#include <vector>
#include <algorithm>

typedef struct stServiceStat
{
        int svcnum;
        std::string name;
        int runstat;
        int initstat;
        stServiceStat()
        {
                svcnum = 0;
        }
        stServiceStat(int num, std::string str = "")
        {
                svcnum = num;
                name = str;
        }
} stServiceStat;

class CServiceMgr
{
public:
        CServiceMgr();
        ~CServiceMgr();

        // 基本业务函数
        static int ServiceCtrl(const int& svcnum, const int& ctrl);
        static int InitStart  (const int& svcnum, const bool& enable);
        static int GetAllServiceStat(std::vector<stServiceStat>& vsvc);
        static int GetServiceStat(const int& svcnum, int& runstat);
        static int GetServiceStat(const int& svcnum, int& runstat, int& initstat);
        static int GetInitStat   (const int& svcnum, int& initstat);

        // 功能函数
        static const char* SvcNumToName(const int& svcnum);
        static const char* ControlNumToStr(const int& ctrl);

};

#endif
