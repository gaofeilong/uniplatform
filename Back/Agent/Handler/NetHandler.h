#ifndef _NET_HANDLER_H_
#define _NET_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class NetHandler : public IEventHandler {
public:
        NetHandler();
        ~NetHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();

};

#endif //_NET_HANDLER_H_
