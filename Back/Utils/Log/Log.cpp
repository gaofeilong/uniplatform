/*******************************************************************************
 *
 * @ File Name  : Log.cpp
 * @ Date       : 2012-06-28
 * @ Author     : gaofeilong <gaofeilonggmail@163.com>
 * @ Description: 日志功能，写日志时调用具体的写类执行写入操作，日志类仅作宏参数
 *                解析及对底层写操作的调用
 * @ History    : 2012-06-28：创建文件
 *              : 2012-07-25：添加对第一次初始化的判
 *              : 2012-08-22：GetInstance() 使用双重判断枷锁
 *
 * ****************************************************************************/
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>

#include "Log.h"

/**
 * constructor of FileLogWriter
 */
FileLogWriter::FileLogWriter(const std::string &logFile): m_LogFile(logFile)
{
}

/**
 * FileLogWriter::Write() -底层写日志，写到终端
 * @logInfo: 日志结构体，包含一条日志所有的必须信息，如时间、文件、行号、内容等
 * @return: void
 */
void ConsoleLogWriter::Write(const struct LogInfo &logInfo)
{
        int newLineCnt = 0;
        std::string info = logInfo.contents;
        while (info[strlen(info.c_str()) - 1] == '\n') {
                info[strlen(info.c_str()) - 1] = '\0';
                newLineCnt++;
        }

        fprintf(stdout, "[%s] %s %s -@%s(%d)", logInfo.type.c_str(),
                        logInfo.time.c_str(), info.c_str(), 
                        logInfo.file.c_str(), logInfo.line);
        newLineCnt = (newLineCnt == 0? 1: newLineCnt);
        while (newLineCnt--) {
                printf("\n"); 
        }
}

/**
 * FileLogWriter::Write() -底层写日志，写入到文件
 * @logInfo: 日志结构体，包含一条日志所有的必须信息，如时间、文件、行号、内容等
 * @return: void
 */
void FileLogWriter::Write(const struct LogInfo &logInfo)
{
        int newLineCnt = 0;
        std::string info = logInfo.contents;
        while (info[strlen(info.c_str()) - 1] == '\n') {
                info[strlen(info.c_str()) - 1] = '\0';
                newLineCnt++;
        }

        FILE *stream = fopen(m_LogFile.c_str(), "a");
        stream = stream? stream: stderr;
        fprintf(stream, "[%s] %s %s -@%s(%d)", logInfo.type.c_str(),
                        logInfo.time.c_str(), info.c_str(), 
                        logInfo.file.c_str(), logInfo.line);
        newLineCnt = (newLineCnt == 0? 1: newLineCnt);
        while (newLineCnt--) {
                fprintf(stream, "\n"); 
        }

        fclose(stream);
}

/**
 * DatabaseLogWriter::Write() -底层写日志，写入到数据库
 * @logInfo: 日志结构体，包含一条日志所有的必须信息，如时间、文件、行号、内容等
 * @return: void
 *
 * void DatabaseLogWriter::Write(const struct LogInfo &logInfo)
 * {
 *         int newLineCnt = 0;
 *         std::string info = logInfo.contents;
 *         while (info[strlen(info.c_str()) - 1] == '\n') {
 *                 info[strlen(info.c_str()) - 1] = '\0';
 *                 newLineCnt++;
 *         }
 * 
 *         printf("[D][%s] %s %s - @ %s(%d)", logInfo.type.c_str(),
 *                         logInfo.time.c_str(), info.c_str(), 
 *                         logInfo.file.c_str(), logInfo.line);
 *         newLineCnt = (newLineCnt == 0? 1: newLineCnt);
 *         while (newLineCnt--) {
 *                 printf("\n"); 
 *         }
 * }
 */

BOOSTPTR<Log> Log::m_pLog;
struct LogConfig Log::m_Config;
std::vector<Writer> Log::m_WriterList;
pthread_mutex_t Log::m_Mutex = PTHREAD_MUTEX_INITIALIZER;
bool Log::m_Inited = false;

/**
 * Log::GetInstance() -获取单例指针
 * @return: 日志指针 
 */
BOOSTPTR<Log> Log::GetInstance()
{
        if (NULL == m_pLog) {
                LOG_LOCK();
                if (NULL == m_pLog) {
                        Log::Init();
                        m_pLog = BOOSTPTR<Log>(new Log);
                }
                LOG_UNLOCK();
        }
        return m_pLog;
}

/**
 * Log::Init() -日志初始化，用于初始化多种输出(文件、数据库..)时的各个输出指针
 * @return: void
 */
void Log::Init(const std::string &conf)
{
        if (!m_Inited) {
                if (m_Config.IsConsoleEnable) {
                        m_WriterList.push_back(Writer(new ConsoleLogWriter())); 
                }
                if (m_Config.IsFileEnable) {
                        Writer w = Writer(new FileLogWriter(conf));
                        m_WriterList.push_back(w);
                }
                m_Inited = true;
        }
}


/**
 * Log::WriteLog() -宏替换函数，解析变参，调用底层的写日志接口
 * @type: 日志类型：DBG/INF/WRN/ERR
 * @file: 产生日志记录的源文件
 * @line: 产生日志记录的行号
 * @fmt: 变参字符串, 同printf()的格式
 * @...: 变参说明 
 * @return: void
 */
void Log::WriteLog(const char *type, const char *file, int line, 
                        const char *fmt, ...)
{
        /* Guess we need no more than 256 bytes. */
        int n, size = 256;
        char *p, *np;
        va_list ap;

        if ((p = (char *)malloc(size)) == NULL) {
                return;
        }
        while (1) {
                /* Try to print in the allocated space. */
                va_start(ap, fmt);
                n = vsnprintf(p, size, fmt, ap);
                va_end(ap);
                /* If that worked, return the string. */
                if (n > -1 && n < size) {
                        break;
                }
                /* else, we need more space. */
                if (n > -1) {        /* glibc 2.1, vsnprintf return byte needed */
                        size = n + 1;/* precisely what is needed */
                } else {             /* glibc 2.0, vsnprintf return -1 */
                        size *= 2;   /* twice the old size */
                }
                if ((np = (char *)realloc(p, size)) == NULL) {
                        free(p);
                        return;
                } else {
                        p = np;
                }
        }

        /* write to log */
        std::string time = GetTime();
        LogInfo logInfo = {line, type, time, file, p};

        std::vector< BOOSTPTR<LogWriter> >::iterator vsIt;
        for (vsIt = m_WriterList.begin(); vsIt != m_WriterList.end(); ++vsIt) {
                if ((logInfo.type == __DBG__ && m_Config.IsDebugEnable)
                 || (logInfo.type == __INF__ && m_Config.IsInfoEnable)
                 || (logInfo.type == __WRN__ && m_Config.IsWarningEnable)
                 || (logInfo.type == __ERR__ && m_Config.IsErrorEnable)){
                        (*vsIt)->Write(logInfo);
                }
        }

        free(p);
        return;
}

/**
 * Log::GetTime() -获取当前的时间字符串，格式：YYYY-MM-DD hh-mm-ss microsec
 * @return: void
 */
std::string Log::GetTime()
{
        struct timeval timer;
        struct tm *tblock;
        char timeBuf[64] = { 0 };

        //YYYY_MM_DD_HH_MM_SS_MS
        gettimeofday(&timer, NULL);
        tblock = localtime(&timer.tv_sec);
        strftime(timeBuf, 64, "%Y-%m-%d %H:%M:%S", tblock);
        sprintf(timeBuf, "%s,%03ld", timeBuf, timer.tv_usec / 1000);       
        return std::string(timeBuf);
}
