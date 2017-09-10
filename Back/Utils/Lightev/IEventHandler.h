/************************************************************
 Copyright(C), 2010-2011, Scidata Tech. Co.,Ltd.
 Description:   本类负责初始化事件循环，libev循环中的读写处理，
                超时及关闭，为派生类提供接口。
 Author:        预研组，郭文超
 Others:        无
 ***********************************************************/
#ifndef _IEVENTHANDLER_H_
#define _IEVENTHANDLER_H_

#include "EVCommon.h"

#define REGISTER_WRITE 0
#define REGISTER_READ  1
#define REGISTER_NULL  2

namespace lightev 
{

class EventHandlerFactory;
struct Packet;
class Acceptor;

class IEventHandler
{
public:
        /**
         * @note 根据传入的超时时间及数据包缓冲区大小建立事件处理对象。
         * packetSize大小为包缓冲区数据域的大小，由IEventHandler负责申请及释放。
         *
         * @param timeoutSec: 超时时间（秒）
         *
         * @param packetSize: 数据包数据域的大小（字节）不包含包头
         */
        IEventHandler(int timeoutSec, int packetSize);
        virtual ~IEventHandler();

public:
        void SetFactory(EventHandlerFactory* factory);  //used by Acceptor
        EventHandlerFactory* GetFactory();              //used by TCPServer

        /**
          * @note 本函数由派生类实现。一般情况下派生类不需调用该函数。
          * 可使用setsokopt设置传入参数的相关属性。
          * 可添加在Read操作前的初始化，如打开读写文件之类。
          *
          * @param sockfd: 经由accept返回的socket文件描述符
          *
          * @return 0表示成功，小于0表示失败。
          */
        virtual int Open(int sockfd) = 0;

        /**
          * @note 本函数由派生类实现。一般情况下派生类不需调用该函数。
          * 收到客户端发来的数据后,向客户端返回之前，调用Read或Write进行处理。
          * 一般情况下，调用二者之一皆可。
          *
          * @param p: 派生类与基类之间传递的数据包
          *      
          * @return Read的返回值为REGISTER_WRITE, REGISTER_READ, REGISTER_NULL中的一个。
          * 通常情况下是服务器接收到数据后写回应答，返回REGISTER_WRITE即可。
          * 如有使用数据处理队列的方式等需求，可将返回值设置为REGISTER_NULL，
          * 然后再手动调用RegisterWrite去注册写事件，或使用Socket::Send等写回应答。
          * 如果不需要向客户端发送应答，Read返回REGISTER_READ即可。
          */
        virtual int Read(Packet* p) = 0;
	
        /**
          * @note 本函数由派生类实现。一般情况下派生类不需调用该函数。
          * 收到客户端发来的数据后,向客户端返回之前，由该函数进行处理。
          *
          * @param p: 派生类与基类之间传递的数据包
          *
          * @return 返回0即可。
          */
        virtual int Write(Packet* p) = 0;

        /**
          * @note 本函数由派生类实现。一般情况下派生类不需调用该函数。
          * 在IEventHandler派生类指定的参数时间内,检测到读超时。由该函数进行处理。
          * 超时后会调用下面的Close。主动关闭套接字。
          *
          * @return 返回0即可。
          */
	virtual int Timeout() = 0;

        /**
          * @note 本函数由派生类实现。一般情况下派生类不需调用该函数。
          * 当因为超时而主动关闭时，或因为对方已关闭时调用该函数
          * 该函数返回后，IEventHandler会调用close关闭socket。
          *
          * @return 返回0即可。
          */
	virtual int Close() = 0; 
	
        /**
          * @note 注册读事件。
          * 当Read的返回值为REGISTER_WRITE时，才需手动调用该函数。
          * Read返回其他值，不需手动调用。
          *
          * @return 无
          */
        void RegisterWrite();

        int GetTimeout();       //used by Acceptor

        void SetWatcher(Watcher* w);

        static void ReadCallback(struct ev_loop* loop, struct ev_io* watcher, int events);
	static void ReadTimeoutCallback(struct ev_loop* loop, struct ev_timer* watcher, int events);
        static void ExitCallback(struct ev_loop* loop, struct ev_async* watcher, int events);

private:
	static void WriteCallback(struct ev_loop* loop, struct ev_io* watcher, int events);

protected:
        int                     m_Timeout;
        int                     m_PacketSize;
        Packet*                 m_Packet;
        Watcher*                m_Watcher;

private:
        EventHandlerFactory*    m_Factory;

};

}
#endif  //_IEVENTHANDLER_H_
