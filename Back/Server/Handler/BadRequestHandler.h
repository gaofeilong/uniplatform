#ifndef _BAD_REQUEST_HANDLER_H_
#define _BAD_REQUEST_HANDLER_H_

#include "Utils/Lightev/IEventHandler.h"
#include "Utils/Lightev/EVCommon.h"
#include <string>

using std::string;
using namespace lightev;

class BadRequestHandler : public IEventHandler {
public:
        BadRequestHandler();
        ~BadRequestHandler();

public:
        int Open(int sockfd);
        int Read(Packet* p);
        int Write(Packet* p);
        int Timeout();
        int Close();
        static Packet *BadRequest(const string &info);
};

#endif //_BAD_REQUEST_HANDLER_H_
