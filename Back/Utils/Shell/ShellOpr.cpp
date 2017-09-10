/*******************************************************************************
 * @ File Name  : ShellOpr.cpp
 * @ Date       : 2012-06-28
 * @ Author     : wangbo <wang.bo@scidata.cn>
 * @ Description: 执行Shell脚本,查看运行程序
 * @ History    : 2012-06-28：创建文件
 *
 * ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/syscall.h>

#include "ShellOpr.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"

/**
 * IsProRunning() -查看cmd是否在运行状态
 * @cmd: 准备查看的进程名字符串
 * @isRunning: 进程的运行状态 true(运行)/false(没运行)
 * @return:  0: 成功
 *          -1: popen() 失败
 *          -2: pclose()失败
 */
int IsProRunning(const string& cmd, bool& isRunning)
{
        string tcmd = "ps aux | grep -v 'grep' | egrep \"" + cmd + "\"";
        LOG_INF("%s", tcmd.c_str());

        isRunning = false;
        FILE *file = popen(tcmd.c_str(), "r");
        if (file == NULL) {
                return -1;
        }

        char buffer[BUF1K] = {0};

        if (fgets(buffer, BUF1K, file) != NULL) {
                isRunning = true;
        }

        int ret = pclose(file);
        if (ret < 0) {
                return -2;     
        }
        return 0;
}

/**
 * Execute() -运行cmd命令并返回执行结果
 * @cmd: shell命令字符串
 * @revInfo: shell命令返回的信息
 * @return:  0: 成功
 *          -1: popen()执行失败
 *          -2: pclose()执行失败
 *          -3: fopen()执行失败
 *          -4: fclose()执行失败
 *          -5: remove()执行失败
 */
int Execute(const string& cmd, vector<string>& revInfo)
{
        int st  = 0;
        int ret = 0;
        revInfo.clear();
        char tmpBuf[BUF4K];
        sprintf(tmpBuf, "%s_%ld", TMP_FILE_PATH, syscall(SYS_gettid));

        string tCmd = cmd + "&>" + tmpBuf;
        LOG_INF("%s", tCmd.c_str());
        st = system(tCmd.c_str());
        /* 执行的脚本没有返回信息 */
        if (access(tmpBuf, F_OK) != 0) {
                return st == 0? 0: -1;
        }

        /* 打开临时文件 */
        FILE* tmpFile = fopen(tmpBuf, "r");
        if (tmpFile == NULL) {
                return -1;
        }

        size_t idx = 0;
        string line;
        char   buffer[BUF1K];
        while (fgets(buffer, BUF1K, tmpFile) != NULL) {
                line = buffer;
                idx = line.find("\n");
                if (idx != string::npos) {
                        line.erase(idx);
                }
                revInfo.push_back(line);
        }
        ret = fclose(tmpFile);
        if (ret != 0) {
                return -2;
        }

        ret = remove(tmpBuf);
        if (ret != 0) {
                return -3;
        }
        return st == 0? 0: -1;
}
