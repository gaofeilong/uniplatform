/*******************************************************************
 Copyright(C), 2010-2011, Scidata Tech. Co.,Ltd.
 Description:   本类涉及服务器的初始化，建立及结束。负责建立服务器
                后多线程的维护，libev时间循环的启动等。
 Author:        预研组，郭文超
 Others:        无。
 ******************************************************************/
#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "EVCommon.h"

namespace lightev 
{

class EventHandlerFactory;
class Acceptor;
class TCPServer
{
public:
        /**
          * @note 根据自定义工厂、端口、线程数建立TCPServer对象
          *
          * @param     factory: 指向自定义工厂的指针
          * @param        port: 端口
          * @param threadCount: 线程数
          */
        TCPServer(EventHandlerFactory* factory, int port, int threadCount);
        ~TCPServer();

public:

        /**
          * @note 启动TCPServer 启动前调用SetMagic SetFcntlOpt
          * 若之前调用过SetFcntlOpt
          * 则会把其该fcntl option设置到ServerFd上
          *
          * @return 失败返回-errno，成功返回0.
          */
        int Start();

        /**
          * @note 建立TCPServer后使用该函数设置ServerFd的fcntl option
          * 如果两个标志都需要设置，调用该函数两次。
          * 比如设置FD_CLOEXEC则调用SetFcntlOpt(F_SETFD, FD_CLOEXEC)
          * 另外需要在*Handler::Open中调用fcntl(F_SETFD, FD_CLOEXEC)
          *
          * @param cmd   设置File Status Flags(F_SETFL)或File discriptor Flags(F_SETFD)
          *        flags m_FcntlOpt的int值
          */

        void SetFcntlOpt(int cmd, int flags);

        /**
          * @note 停止TCPServer
          *
          * @return 失败返回-errno，成功返回0.
          */
        int Stop();

        /**
          * @note 建立TCPServer后使用该函数设置Packet的Magic字段
          * 若服务器与客户端设置的相同，包才有意义。
          * 否则认为是非法包，不予接受。
          *
          * @param m Magic的char值
          */
        void SetMagic(char m);

        /**
          * @note 获取已设置的Magic值。
          *
          * @return 返回设置的Magic值
          */
        char GetMagic();

        struct RWThread* GetRWThread(); //only used in Acceptor::AcceptCallback
        Acceptor* GetAcceptor();        //only used in Acceptor::AcceptCallback

private:
        static void* LoopFunc(void* arg);
        int StopAcceptLoop();
        int StopReadWriteThread();
        int StartSocket();
        int StartReadWriteThread();
        int StartAcceptLoop();

private:
        int                 m_Port;
        int                 m_EventLoopNum;
        bool                m_Started;
        int                 m_ServerSockfd;
        char                m_Magic;
        int                 m_FcntlFSFOpt;       //File status flags           : F_SETFL
        int                 m_FcntlFDFOpt;       //File discriptor flags       : F_SETFD
        int                 m_DispatchCount;

        int                 m_InitCount;
        pthread_mutex_t     m_Lock;
        pthread_cond_t      m_InitCond;
        pthread_mutex_t     m_AcceptLock;

        sockaddr_in         m_ServerAddr;
        struct RWThread*    m_RWThread;

        WatcherIo*          m_Accept;
        Acceptor*           m_Acceptor;
        pthread_t           m_AcceptThread;

};

}
#endif  //_TCPSERVER_H_
