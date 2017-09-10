/************************************************************
 Copyright(C), 2010-2011, PreResearch, Scidata Tech. Co.,Ltd.
 Description:   本类是自定义工厂类的基类。提供创建及删除
                Handler的接口由派生类实现。
 Others:        无。
 ***********************************************************/
#ifndef _EVENT_HANDLER_FACTORY_H_
#define _EVENT_HANDLER_FACTORY_H_

#include "IEventHandler.h"

namespace lightev 
{

class EventHandlerFactory 
{
public:
        virtual ~EventHandlerFactory(){}

public:
        /**
          *@note 由派生类实现，由封装库调用。
          *根据Packet的cmd字段产生对不同对象的进行分发。
          *
          *@param cmd:Packet的cmd的值
          *
          *@return 返回对应的事件处理指针
          */
        virtual IEventHandler* CreateHandler(int cmd) = 0;

        /**
          *@note 由派生类实现，由封装库调用。
          *对应于CreateHandler的new，提供delete。
          *
          *@param obj:需要销毁的IEventHandler
          */
        virtual void DeleteHandler(IEventHandler* obj) = 0;

};

}
#endif  //_EVENT_HANDLER_FACTORY_H_
