#ifndef _TRANSFER_HANDLER_H_
#define _TRANSFER_HANDLER_H_

#include <string>

#include "Utils/Lightev/IEventHandler.h"
#include "Utils/Lightev/EVCommon.h"

using std::string;
using namespace lightev;

class TransferHandler : public IEventHandler {
public:
        TransferHandler();
        ~TransferHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
private:
        void TransferError(Packet *p, const string &info);
};

#endif //_TRANSFER_HANDLER_H_
