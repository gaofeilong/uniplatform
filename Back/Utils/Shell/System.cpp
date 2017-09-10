#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "System.h"
#include "Utils/Log/Log.h"

bool GetHighTime(std::string& strTime, bool toSecond)
{
        timeval tv;

        int rt = gettimeofday(&tv, NULL);
        if (rt < 0) {
                LOG_DBG("function gettimeofday error: %m");
                return false;
        }

        tm* tm1 = localtime(&tv.tv_sec);

        strTime.resize(30, 0);

        if (toSecond) {
                sprintf((char *)strTime.data(),
                                        "%04d-%02d-%02d %02d:%02d:%02d",
                                        tm1->tm_year + 1900,
                                        tm1->tm_mon + 1,
                                        tm1->tm_mday,
                                        tm1->tm_hour,
                                        tm1->tm_min,
                                        tm1->tm_sec);

        } else {
                sprintf((char *)strTime.data(),
                                        "%04d-%02d-%02d %02d:%02d:%02d.%06d",
                                        tm1->tm_year + 1900,
                                        tm1->tm_mon + 1,
                                        tm1->tm_mday,
                                        tm1->tm_hour,
                                        tm1->tm_min,
                                        tm1->tm_sec,
                                        (int)tv.tv_usec);
        }

        return true;
}

bool Popen(const char* cmd, std::string& msg, std::string& err)
{
        // define 2 fd , one for stdout, other for stderr
        int outfd[2], errfd[2];

        if (pipe(outfd) == -1 || pipe(errfd) == -1) {
                return false;
        }

        pid_t cpid = fork();
        if (cpid == -1) {
                return false;
        }
        else if (cpid == 0) {
                close(outfd[0]);
                if (outfd[1] != STDOUT_FILENO) {
                        dup2(outfd[1], STDOUT_FILENO);
                        close(outfd[1]);
                }

                close(errfd[0]);
                if (errfd[1] != STDERR_FILENO) {
                        dup2(errfd[1], STDERR_FILENO);
                        close(errfd[1]);
                }

                execl("/bin/sh", "sh", "-c", cmd, (char *)0);
                _exit(127);

        } else {
                int status = 0;
                waitpid(cpid, &status, 0);

                if (!WIFEXITED(status)) {
                        return false;
                }

                status = WEXITSTATUS(status);
                if (status == 127) {
                        return false;
                }

                char buf[1024];
                int  ret = 0;

                close(outfd[1]);
                while ((ret = read(outfd[0], buf, 1023)) != -1) {
                        if (ret == 0) {
                                break;
                        }
                        buf[ret]  = 0;
                        msg      += buf;
                }

                close(errfd[1]);
                while ((ret = read(errfd[0], buf, 1023)) != -1) {
                        if (ret == 0) {
                                break;
                        }
                        buf[ret]  = 0;
                        err      += buf;
                }
        }

        return true;
}

bool Popen(const char* cmd, std::string& result)
{
        bool        bRt = true;

        std::string        strCmd;

        FILE        *file = popen(cmd, "r");
        if (file == NULL){
                LOG_DBG("Popen error, FILE* is NULL");
                return false;
        }

        char        buf[256]  = {0};
        while (NULL != fgets(buf, 256, file)) {
                result += buf;
        }

        pclose(file);

        return bRt;
}

bool ExeCmd(const char* cmd)
{
        int ret = System(cmd);
        if (ret == 0) {
                return true;
        }
        return false;
}

int System(const char* cmd)
{
        if (cmd == NULL) {
                return -1;
        }

        int ret = system(cmd);

        // 执行失败
        if (ret == -1) {
                LOG_DBG("fork() is error, cannot execute command");
                return -1;
        }
        else if (!WIFEXITED(ret)) {
                LOG_DBG("system return value is (%d)", WEXITSTATUS(ret));
                return -1;
        }
        // 执行成功
        else {
                ret = WEXITSTATUS(ret);
                // 若返回值为 127 ，则可能是 sh 或者 sh 对应的命令无法执行，比如命令无效，此时，也认为执行失败
                if (ret == 127) {
                        ret = -1;
                }
        }

        return ret;
}
