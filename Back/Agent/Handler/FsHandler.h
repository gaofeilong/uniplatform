#ifndef _DDFS_HANDLER_H_
#define _DDFS_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class FsHandler : public IEventHandler {
public:
        FsHandler();
        ~FsHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_DDFS_HANDLER_H_
