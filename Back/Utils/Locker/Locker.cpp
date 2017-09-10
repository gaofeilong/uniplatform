#include "Locker.h"

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

Locker::Locker()
{
}

Locker::~Locker()
{
}

int Locker::LockFile(const string& filePath)
{
        int ret = 0;
        int fd  = 0;
        /* 给指定文件加锁 */
        ret = DoLockFile(filePath, fd);
        if (ret < 0) {
                return -1;
        } else if (ret == LOCK_FAIL){
                /* 文件已经在锁状态 */
                return LOCK_FAIL;
        }

        ret = ftruncate(fd, 0);
        if (ret < 0) {
                return -2;
        }
        char buf[10];
        sprintf(buf, "%ld", (long)getpid());

        ret = write(fd, buf, strlen(buf)+1);
        if (ret < 0) {
                ret = -3;
        }
        return ret;
}

int Locker::TryLockFile(const string& filePath)
{
        int ret = 0;
        int fd  = 0;
        /* 给指定文件加锁 */
        ret = DoLockFile(filePath, fd);
        if (ret < 0) {
                return -1;
        } else if (ret == LOCK_FAIL) {
                /* 文件已经在锁状态 */
                return LOCK_FAIL;
        }
        /* 如果文件能加上锁的话去掉锁 */ 
        ret = close(fd);
        if (ret != 0) {
                ret = -2;
        }
        return ret;
}

int Locker::UnLockFile(int fd)
{
        struct flock fl;
        fl.l_type    = F_UNLCK;
        fl.l_start   = 0;
        fl.l_whence  = SEEK_SET;
        fl.l_len     = 0;

        int ret = 0;
        ret = fcntl(fd, F_SETLK, &fl);
        if (ret < 0) {
                ret = -1;
        }
        return ret;
}

int Locker::DoLockFile(const string& filePath, int& fd)
{
        fd = open(filePath.c_str(), O_RDWR | O_CREAT, LOCKMODE);
        if (fd < 0) {
                return -1;
        }

        struct flock fl;
        fl.l_type    = F_WRLCK;
        fl.l_start   = 0;
        fl.l_whence  = SEEK_SET;
        fl.l_len     = 0;

        int ret = 0;
        ret = fcntl(fd, F_SETLK, &fl);
        if (ret < 0) {
                if (errno == EACCES || errno == EAGAIN) {
                        close(fd);
                        return LOCK_FAIL;
                }
        }
        return ret;
}

int Locker::GetLockStatus(const string& filePath)
{
        int ret = 0;
        int fd  = open(filePath.c_str(), O_RDWR | O_CREAT, LOCKMODE);
        if (fd < 0) {
                return -1;
        }

        struct flock lock;
        ret = fcntl(fd, F_GETLK, &lock);
        if (ret < 0) {
                return -2;
        }
        ret = close(fd);
        if (ret != 0) {
                ret = -3;
        }
        return ret;
}
