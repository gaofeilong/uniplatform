#ifndef _SYS_MSG_MGR_H_
#define _SYS_MSG_MGR_H_

#include "Public/Config/ConfigReader.h"

using std::string;

class SysMsgMgr {
public:
        SysMsgMgr();
        ~SysMsgMgr();

public:
        /* 查看系统通知配置 */
        int GetSysMsg(SysMsgConfig& sysMsgConfig);

        /* 设置系统通知 */
        int SetSysMsg(const SysMsgConfig& sysMsgConfig);
};

#endif //_SYS_MSG_MGR_H_
