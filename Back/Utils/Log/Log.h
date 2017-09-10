/*******************************************************************************
 *
 * @ File Name  : ConfigTypes.h
 * @ Date       : 2012-06-28
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 日志功能, 接口: LOG_DBG(), LOG_INF(), LOG_WRN(), LOG_ERR()
 * @ History    : 2012-06-28：创建文件
 *              : 2012-07-25：添加对第一次初始化的判断
 *              : 2012-07-30：不使用颜色
 *              : 2012-10-31：锁宏定义改为函数形式
 *
 * ****************************************************************************/
#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Include/Types.h"
#define GW_LOG_FILE "/var/log/gateway.log"

#if 0
        #define  __DBG__        "\033[40;32mDBG\033[0m"
        #define  __INF__        "\033[40;34mINF\033[0m"
        #define  __WRN__        "\033[40;33mWRN\033[0m"
        #define  __ERR__        "\033[40;31mERR\033[0m"
#else 
        #define  __DBG__        "DBG"
        #define  __INF__        "INF"
        #define  __WRN__        "WRN"
        #define  __ERR__        "ERR"
#endif

#define LOG_LOCK()      pthread_mutex_lock(&Log::m_Mutex)
#define LOG_UNLOCK()    pthread_mutex_unlock(&Log::m_Mutex)
#define LOG_DBG(fmt, ...) do { \
                                Log::GetInstance()->WriteLog(__DBG__, \
                                __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
                            } while (0)
#define LOG_INF(fmt, ...) do { \
                                Log::GetInstance()->WriteLog(__INF__, \
                                __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
                            } while (0)
#define LOG_WRN(fmt, ...) do { \
                                Log::GetInstance()->WriteLog(__WRN__, \
                                __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
                            } while (0)
#define LOG_ERR(fmt, ...) do { \
                                Log::GetInstance()->WriteLog(__ERR__, \
                                __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
                            } while (0)

struct LogInfo {
        int         line;
        std::string type;
        std::string time;
        std::string file;
        std::string contents;
};

/* config types */
struct LogConfig {      
        /* bool IsDatabaseEnable;    是否输出终端日志   */
        bool IsFileEnable;        /* 是否输出数据库日志 */
        bool IsConsoleEnable;     /* 是否输出文件日志   */

        bool IsDebugEnable;       /* 是否输出debug      */
        bool IsInfoEnable;        /* 是否输出info       */
        bool IsWarningEnable;     /* 是否输出warnning   */
        bool IsErrorEnable;       /* 是否输出error      */

        std::string LogFile;      /* 日志路径           */
        LogConfig(bool enFile = 1, bool enConsole = 0, bool enDBG = 1,
                  bool enINF = 1, bool enWRN = 1, bool enERR = 1):
                IsFileEnable(enFile), IsConsoleEnable(enConsole),
                IsDebugEnable(enDBG), IsInfoEnable(enINF),
                IsWarningEnable(enWRN), IsErrorEnable(enERR) {}
};

/******************** writer ****************************/
class LogWriter {
public:
        virtual ~LogWriter() {}
public:
        virtual void Write(const struct LogInfo &logInfo) = 0;
};

class FileLogWriter: public LogWriter {
public:
        FileLogWriter(const std::string &logFile);
public:
        void Write(const struct LogInfo &logInfo);
private:
        std::string m_LogFile;
};

/**
 * class DatabaseLogWriter: public LogWriter {
 * public:
 *         void Write(const struct LogInfo &logInfo);
 * };
 */

class ConsoleLogWriter: public LogWriter {
public:
        void Write(const struct LogInfo &logInfo);
};
/******************* writer end *************************/

typedef BOOSTPTR<LogWriter> Writer;
class Log {
public:
        static BOOSTPTR<Log> GetInstance();
        static void Init(const std::string &conf = GW_LOG_FILE);
        void WriteLog(const char *type, const char *file, int line, 
                                const char *fmt, ...);
private:
        Log() {}
        std::string GetTime();
private:
        static BOOSTPTR<Log> m_pLog;
        static pthread_mutex_t m_Mutex;
        static struct LogConfig m_Config;
        static std::vector<Writer> m_WriterList;
        static bool m_Inited;
};

#endif //_LOG_H_
