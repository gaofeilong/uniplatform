#ifndef _SERVER_FACTORY_H_
#define _SERVER_FACTORY_H_

#include "Utils/Lightev/EventHandlerFactory.h"

using namespace lightev;

class TaskQueue;
class ServerFactory : public EventHandlerFactory {
public:
        ~ServerFactory(){}

public:
        IEventHandler* CreateHandler(int cmd);
        void DeleteHandler(IEventHandler* obj);

};

#endif  //_SERVER_FACTORY_H_
