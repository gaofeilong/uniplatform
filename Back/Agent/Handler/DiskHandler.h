#ifndef _DISK_HANDLER_H_
#define _DISK_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class DiskHandler : public IEventHandler {
public:
        DiskHandler();
        ~DiskHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_DISK_HANDLER_H_
