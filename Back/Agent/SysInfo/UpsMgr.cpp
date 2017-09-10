#include <vector>
#include <stdio.h>

#include "UpsMgr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

UpsMgr::UpsMgr()
{
}

UpsMgr::~UpsMgr()
{
}

int UpsMgr::GetState(int &state)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_UPS_STATE, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_UPS_STATE.c_str());
                return -1;
        }
        state = buf[0] == "on"? 1: 0;
        return 0;
}

// current state?
int UpsMgr::SetState(int state)
{
        int curState, ret = 0;
        ret = GetState(curState);
        if (ret < 0) {
                LOG_ERR("get apcups status error");
                return -1;
        }

        string action;
        vector<string> buf;
        if (curState == 0) {                    // 当前服务在关闭状态
                if (state == curState) {
                        return 0;
                } else {
                        // 查看UPS是否连接
                        action = "start";
                }
        } else {                                // 当前服务在启动状态
                if (state == curState) {
                        action = "restart";
                } else {
                        action = "stop";
                }
        }

        ret = Execute(CL_SET_UPS_STATE(action), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", 
                                CL_SET_UPS_STATE(action).c_str());
                return -2;
        }
        // 如果是启动服务，需要稍微延时一下执行apcaccess来判断是否连接UPS，
        // 如果没有连接，需要关闭服务并返回启动失败
        if (action == "start") {
                sleep(1);
                if (Execute(CL_IS_UPS_LINKED, buf) != 0) {
                        LOG_ERR("execute cmd [%s] error: %s", 
                                CL_IS_UPS_LINKED.c_str(), buf[0].c_str());
                        if (Execute(CL_SET_UPS_STATE("stop"), buf) != 0) {
                                LOG_ERR("execute cmd [%s] error: %s", 
                                        CL_SET_UPS_STATE("stop").c_str());
                        }
                        return -3;
                }
        }
        return 0;
}

int UpsMgr::GetTime(int &time)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_UPS_TIME, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_UPS_TIME.c_str());
                return -1;
        }
        time = buf[0] == ""? 0: atoi(buf[0].c_str());
        return 0;
}

int UpsMgr::SetTime(int time)
{
        int ret;
        vector<string> buf;
        char strTime[BUF8] = {0};
        sprintf(strTime, "%d", time);

        ret = Execute(CL_SET_UPS_TIME(strTime), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_SET_UPS_TIME(strTime).c_str());
                return -1;
        }
        return 0;
}


int UpsMgr::GetPower(int &power)
{
        int ret;
        vector<string> buf;

        ret = Execute(CL_GET_UPS_POWER, buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_GET_UPS_POWER.c_str());
                return -1;
        }
        power = buf[0] == ""? 0: atoi(buf[0].c_str());
        return 0;
}

int UpsMgr::SetPower(int power)
{
        int ret;
        vector<string> buf;
        char strPower[BUF8] = {0};
        sprintf(strPower, "%d", power);

        ret = Execute(CL_SET_UPS_POWER(strPower), buf);
        if (ret != 0) {
                LOG_ERR("execute cmd [%s] error", CL_SET_UPS_POWER(strPower).c_str());
                return -1;
        }
        return 0;
}

