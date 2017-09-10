#ifndef _SYS_HANDLER_H_
#define _SYS_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class SysHandler : public IEventHandler {
public:
        SysHandler();
        ~SysHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_SYS_HANDLER_H_
