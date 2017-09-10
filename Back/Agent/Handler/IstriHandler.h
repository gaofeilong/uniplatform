#ifndef _ISTRI_HANDLER_H_
#define _ISTRI_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class IstriHandler : public IEventHandler {
public:
        IstriHandler();
        ~IstriHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_ISTRI_HANDLER_H_
