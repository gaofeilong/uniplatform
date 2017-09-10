/************************************************************
 Copyright(C), 2010-2011, Scidata Tech. Co.,Ltd.
 Description:   本函数库的公共头文件。包括libev相关的头文件，
                公用的数据结构，数据包格式，调试宏等。
 Author:        预研组，郭文超
 Others:        无。
 ***********************************************************/
#ifndef _EVCOMMON_H_
#define _EVCOMMON_H_

#include <list>
#include "ev.h"

namespace lightev 
{

struct WatcherIo
{
        ev_io                   Io;
        struct ev_loop*         Loop;
        struct ev_async         Exit;
        void*                   Obj;
};

struct Watcher
{
        struct ev_loop*         Loop;
        ev_io                   Io;
        ev_timer                Timer;
        int                     Cursor;
        struct RWThread*        Thread;
        bool                    ObjIsAcceptor;
        void*                   Obj;
};

struct RWThread
{
        pthread_t               Pid;
        struct ev_loop*         Loop;
        struct ev_async         Async;
        struct ev_async         Exit;
        std::list<Watcher*>     WatcherSet;
        int                     Sockfd;
        pthread_mutex_t         Lock;
        pthread_cond_t          AsyncWait;
        void*                   Obj;
};

//由于RWThread中 有std::list的原因 会报 
// 警告对NULL对象非静态数据成员lightev::RWThread::Async的访问无效 
// 警告可能错误的使用了offset宏 
//
//
//
//
//
// 1byte对齐，紧缩，成对出现。
//
#pragma pack(1)  
struct Packet
{
        char Magic;
        char Version;
        int  Cmd;
        int  Length;
        char Data[];
};
#pragma pack()

}

#ifdef DEBUG_PRINT
#include <stdio.h>
#include <errno.h>
#define DEBUG_PRINT_INT(i) \
        fprintf(stderr, "[File: %s, Fun: %s, Line: %d, %d] \n", __FILE__, __FUNCTION__, __LINE__, (i))
#define DEBUG_PRINT_S(s) \
        fprintf(stderr, "[File: %s, Fun: %s, Line: %d, %s] \n", __FILE__, __FUNCTION__, __LINE__, (s))
#define DEBUG_PRINT_E(s) \
        perror((s))
#define DEBUG_PRINT_S_INT(s, i) \
        fprintf(stderr, "[File: %s, Fun: %s, Line:%d, %s, %i] \n", __FILE__, __FUNCTION__, __LINE__, (s), (i))
#else 
#define DEBUG_PRINT_INT(i)
#define DEBUG_PRINT_S(s)
#define DEBUG_PRINT_E(s)
#define DEBUG_PRINT_S_INT(s, i)
#endif

#ifndef container_of 
#define container_of(ptr, type, member) ({\
        const typeof( ((type *)0)->member ) *_mptr = (ptr);\
        (type *)( (char *)_mptr - offsetof(type, member) ) ;})
#endif

#endif //_EVCOMMON_H_

