/* 
 * @ Date        : 2013-03-26
 * @ Auther      : wangbo
 * @ Description : 对邮件通知配置文件进行操作(读,写)
 * @ Email       : wang.bo@scistor.com
 *
 * @ History     : 2012-03-26 创建
 *
 */

#ifndef _SYS_MSG_CONFIG_OPR
#define _SYS_MSG_CONFIG_OPR

#include "Public/Config/ConfigOpr.h"

#include <map>
#include <string>
using std::map;
using std::string;

typedef struct _SysMsgConfig {
        string           server;      //邮件服务器IP
        int              port;        //发送端口
        int              autoType;    //验证方式
        string           user;        //发送者邮箱
        string           password;    //发送者邮箱密码
        map<string, int> receiverSet; //接受者邮箱集合, 
                                      //first为邮箱,second为邮箱接受邮件的类型
} SysMsgConfig;

class SysMsgConfigOpr : public ConfigOpr<SysMsgConfig> {
public:
        SysMsgConfigOpr(const string& configPath);
        ~SysMsgConfigOpr();

public:
        int Read(SysMsgConfig& sysMsgConfig);
        int Write(const SysMsgConfig& sysMsgConfig);
};

#endif //_SYS_MSG_CONFIG_OPR
