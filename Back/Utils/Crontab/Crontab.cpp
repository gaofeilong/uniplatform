#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "Crontab.h"
#include "Include/Types.h"
#include "Include/String.h"
#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"

Crontab::Crontab()
{
}

Crontab::~Crontab()
{
}

int Crontab::Add(const string& cmd)
{
        if (Find(cmd)) {
                LOG_INF("task already exist: %s", cmd.c_str());
                return 0;
        }

        FILE *stream = fopen(CRON_FILE, "a");
        if (NULL == stream) {
                LOG_ERR("open %s error: %s", CRON_FILE, strerror(errno));
                return -1;
        }
        fprintf(stream, "%s\n", cmd.c_str());
        fclose(stream);
        return 0;
}

int Crontab::Del(const string& cmd)
{
        if (!Find(cmd)) {
                LOG_INF("no cron task: %s", cmd.c_str());
                return 0;
        }

        vector<string> tasks;
        string cmdLine = "cat " + string(CRON_FILE);
        int ret = Execute(cmdLine, tasks);
        if (ret < 0) {
                LOG_ERR("execute errro! cmd=%s\n", cmdLine.c_str());
                return ret;
        }

        // 重启配置crontab信息
        FILE* stream = fopen(CRON_FILE, "w+");
        if (stream == NULL) {
                LOG_ERR("fopen error! path=%s\n", CRON_FILE);
                return -1;
        }

        for (size_t i=0; i<tasks.size(); ++i) {
                if (tasks[i] != cmd) {
                        fprintf(stream, "%s\n", tasks[i].c_str());
                }
        }
        fclose(stream);

        return 0;
}

bool Crontab::Find(const string& cmd)
{
        FILE *stream = fopen(CRON_FILE, "r");
        if (NULL != stream) {
                char lineBuf[BUF4K] = {0};
                while (fgets(lineBuf, BUF4K, stream)) {
                        if (string(lineBuf) == cmd + "\n") {
                                fclose(stream);
                                return true;
                        }
                        memset(lineBuf, 0, BUF4K);
                }
                fclose(stream);
        }
        return false;
}
