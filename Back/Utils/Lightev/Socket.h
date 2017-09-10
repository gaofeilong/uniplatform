/*************************************************************
 Copyright(C), 2010-2011, Scidata Tech. Co.,Ltd.
 Description:   本类提供几个静态的循环读写方法及建立连接的函数
                使用时需要做返回值判断。提供以非结构化数据收发
                的方式，及结构化超时接收的方式。
 Author:        预研组，郭文超
 Others:        无
 ************************************************************/
#ifndef _LIGHTEV_SOCKET_H_
#define _LIGHTEV_SOCKET_H_

struct timeval;

namespace lightev 
{

struct Packet;
class Socket
{
public:
        /**
          * @note 根据指定ip及端口返回套接字
          *
          * @param   ip:服务器的ip
          * @param port:服务器的端口
          *
          * @return 成功返回建立的套接字描述符，失败返回-errno
          */
        static int Connect(const char* ip, unsigned short port);

        /**
          * @note 将buffer里面的size字节数据发送到fd。
          *
          * @param     fd: 套接字描述符
          * @param buffer: 传入的buffer
          * @param   size: 要发送的大小
          *
          * @return 返回发送的大小，若发送错误返回-errno，
          * 发送不完整返回发送了的字节数。
          */
        static int Send(int fd, const char* buffer, int size);

        /**
          * @note 通过fd接收size字节数据到buffer中。
          *
          * @param     fd: 套接字描述符
          * @param buffer: 传入的buffer
          * @param   size: 要接收的大小
          *
          * @return 返回接收的大小，若接收错误返回-errno，
          * 接收不完整返回接收了的字节数。
          */
        static int Receive(int fd, char* buffer, int size);

        /**
          * @note 通过fd接收size字节到buffer中。
          * 但如果在sec秒内未发生完成，会触发超时。
          *
          * @param     fd: 套接字描述符
          * @param buffer: 传入的buffer
          * @param   size: 要接收的大小
          * @param    sec: 指定的超时时间
          *
          * @return 返回接收的大小，若接收错误返回-errno，
          * 接收不完整返回接收了的字节数。
          */
        static int ReceiveTimeout(int fd, char* buffer, int size, int sec);

        /**
          * @note 根据包体长度，魔法字段，及cmd创建Packet.
          * 
          * @param  size: 包体长度，标识包体的字节数
          * @param magic: 魔法字段，需保证客户端跟服务器设置的相同
          * @param   cmd: 跟工厂中对应的，标识使用哪个事件处理对象
          *
          * @return 创建的包。创建失败返回NULL。
          */
        static Packet* CreatePacket(int size, int magic, int cmd);

        /**
          * @note 将p对应的Packet删掉。
          *
          * @param  p: Packet包
          *
          * @return 无
          */
        static void DeletePacket(Packet* p);

        /**
          * @note 通过fd接收最多size字节到数据包p中。
          * 但如果在sec秒内未发生完成，会触发超时。
          * 以数据包的方式接收时，size的大小为包缓冲区的大小
          * 包括包头的长度。而上述几个函数中size是要收发的实际大小
          * ReceiveTimeout中若接收不到size字节，便会超时。
          * 而实际上并没有size字节需要接收。此时需要使用本方法。
          *
          * @param      fd: 套接字描述符
          * @param       p: 传入的buffer指针
          * @param maxSize: 要接收的最大长度，包含包头。
          * @param     sec: 指定的超时时间
          *
          * @return 返回接收的大小，若接收错误返回-errno，
          * 接收不完整返回接收了的字节数。
          */
        static int ReceivePacket(int fd, Packet* p, int maxSize, int sec);

        /**
          * @note 将已保存至p中的数据发送到fd。
          * 数据的大小为sizeof(Packet) + p->Length
          *
          * @param     fd: 套接字描述符
          * @param      p: 传入的Packet
          *
          * @return 返回发送的大小，若发送错误返回-errno，
          * 发送不完整返回发送了的字节数。
          */
        static int SendPacket(int fd, Packet* p);

private:
        static int Wait(int fd, struct timeval* tv);

};

}
#endif //_LIGHTEV_SOCKET_H_
