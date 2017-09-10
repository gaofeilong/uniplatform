#ifndef _IJSON_MGR_H_
#define _IJSON_MGR_H_

#include "Utils/Json/include/json.h"
#include "Utils/Lightev/EVCommon.h"
#include "Include/Protocol.h"

using namespace lightev;

class IJsonMgr {
public:
        virtual ~IJsonMgr(){};
        virtual lightev::Packet *Handle(const lightev::Packet *p) = 0;

        size_t JsonSize(Json::Value j);
        int    JsonValue(const Packet *p, Json::Value& request);
        
        Packet *RequestOk(const Json::Value &respond);
        Packet *RequestError(const std::string &errInfo);
        inline void SetPackHead(Packet *p, char m, char v, int c, int l);
//private:
        Packet *OnRequest(const Json::Value &respond, int cmd);
};


#endif //_IJSON_MGR_H_
