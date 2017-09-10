#ifndef _ARCHIVE_HANDLER_H_
#define _ARCHIVE_HANDLER_H_

#include "Utils/Lightev/EVCommon.h"
#include "Utils/Lightev/IEventHandler.h"

using namespace lightev;

class ArchiveHandler : public IEventHandler {
public:
        ArchiveHandler();
        ~ArchiveHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
};

#endif //_ARCHIVE_HANDLER_H_
