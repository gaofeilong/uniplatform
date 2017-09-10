#ifndef _VTL_HANDLER_H_
#define _VTL_HANDLER_H_

#include "Utils/Lightev/IEventHandler.h"
#include "Utils/Lightev/EVCommon.h"

using namespace lightev;

class VtlHandler : public IEventHandler {
public:
        VtlHandler();
        ~VtlHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_VTL_HANDLER_H_
