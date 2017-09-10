#ifndef _ANGENT_FACTORY_H_
#define _ANGENT_FACTORY_H_

#include "Utils/Lightev/EventHandlerFactory.h"

using namespace lightev;

class TaskQueue;
class AgentFactory : public EventHandlerFactory {
public:
        ~AgentFactory(){}

public:
        IEventHandler* CreateHandler(int cmd);
        void DeleteHandler(IEventHandler* obj);

};

#endif  //_SERVER_FACTORY_H_
