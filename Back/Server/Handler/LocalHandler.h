#ifndef _LOCAL_HANDLER_H_
#define _LOCAL_HANDLER_H_

#include "Utils/Lightev/IEventHandler.h"
#include "Utils/Lightev/EVCommon.h"

using namespace lightev;

class LocalHandler : public IEventHandler {
public:
        LocalHandler();
        ~LocalHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_LOCAL_HANDLER_H_
