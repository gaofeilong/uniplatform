#ifndef _NAS_HANDLER_H_
#define _NAS_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class NasHandler : public IEventHandler {
public:
        NasHandler();
        ~NasHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_NAS_HANDLER_H_
