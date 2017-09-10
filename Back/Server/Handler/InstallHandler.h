#ifndef _INSTALL_HANDLER_H_
#define _INSTALL_HANDLER_H_

#include "Utils/Lightev/IEventHandler.h"
#include "Utils/Lightev/EVCommon.h"

using namespace lightev;

class InstallHandler : public IEventHandler {
public:
        InstallHandler();
        ~InstallHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_INSTALL_HANDLER_H_
