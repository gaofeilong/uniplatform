#ifndef _NFS_HANDLER_H_
#define _NFS_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class NfsHandler : public IEventHandler {
public:
        NfsHandler();
        ~NfsHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_NFS_HANDLER_H_
