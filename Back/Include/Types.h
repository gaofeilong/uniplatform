#ifndef _TYPES_H_
#define _TYPES_H_

#include <string>
using std::string;

#define THREAD_COUNT                        5
#define TIMEOUT                             30
#define SERVER_PORT                         40001
#define AGENT_PORT                          40002
#define PACKET_SIZE                         (1024*1024*4)

/* buf size */
#define BUF8                    8
#define BUF16                   16
#define BUF32                   32
#define BUF64                   64
#define BUF128                  128
#define BUF256                  256
#define BUF512                  512
#define BUF1K                   1024 
#define BUF2K                   2048 
#define BUF4K                   4096

#define SEC_PER_MINUTE          60
#define SEC_PER_HOUR            3600
#define SEC_PER_DAY             (3600 * 24)
#define SEC_PER_MONTH           (3600 * 24 * 30)
#define MINUTE_PER_DAY          (60 * 24)
#define MINUTE_PER_MONTH        (60 * 24 * 30)
#define BYTE_PER_G              (1024*1024*1024)

/* boost */
//#define BOOSTPTR        boost::shared_ptr

/* mysql tables */
//globe
#define FIELD_ID                "id" 
#define FIELD_NODE_ID           "node_id"

//mp_info;
#define TABLE_MP_INFO           "sci_mp_info"
#define FIELD_MP                "mp"
#define FIELD_CONFIG            "config"

// node_info;
#define FIELD_IP                "ip"
#define FIELD_NODE_KEY          "node_key"
#define TABLE_NODE_INFO         "sci_node_info"

// boost
#include <boost/shared_ptr.hpp>
#define  BOOSTPTR       boost::shared_ptr

/* config types */
//struct _LogConfig {      
//        bool IsDatabaseEnable;    /* 是否输出终端日志   */
//        bool IsFileEnable;        /* 是否输出数据库日志 */
//        bool IsConsoleEnable;     /* 是否输出文件日志   */
//
//        bool IsDebugEnable;       /* 是否输出debug      */
//        bool IsInfoEnable;        /* 是否输出info       */
//        bool IsWarningEnable;     /* 是否输出warnning   */
//        bool IsErrorEnable;       /* 是否输出error      */
//
//        string LogFile;           /* 日志路径           */
//};

//struct _MysqlConfig {
//        string User;              /* 用户名      */
//        string Password;          /* 用户密码    */
//        string Database;          /* 数据库名    */
//        string Server;            /* 服务器IP    */
//        string Socket;            /* 连接文件路经*/
//};                       
                         
//extern struct _GatewayConfig gwConf;

#endif  //_TYPES_H_
