#include "IstriMgr.h"
#include "Protocol.h"
#include "IstriJsonMgr.h"
#include "Utils/Log/Log.h"

IstriJsonMgr::IstriJsonMgr()
{
}

IstriJsonMgr::~IstriJsonMgr()
{
}

Packet *IstriJsonMgr::Handle(const Packet *p)
{
        switch(p->Cmd) {
        case CMD_FBS_LOOKUP:
                return LookupIstriList(p);
        case CMD_FBS_START_INDEX:
                return StartIstriIndex(p);
        case CMD_FBS_START_DATA:
                return StartIstriData(p);
        case CMD_FBS_STOP_INDEX:
                return StopIstriIndex(p);
        case CMD_FBS_STOP_DATA:
                return StopIstriData(p);
        case CMD_FBS_EDIT_INDEX:
                return EditIstriIndex(p);
        case CMD_FBS_EDIT_DATA:
                return EditIstriData(p);
        case CMD_FBS_FIX_INDEX:
                return FixIstriIndex(p);
        case CMD_FBS_FIX_DATA:
                return FixIstriData(p);
        case CMD_FBS_CLEAR_ID:
                return ClearIstriDataId(p);
        default:
                return RequestError("unknown request");
        }
}

Packet *IstriJsonMgr::LookupIstriList(const Packet *p)
{
        LOG_DBG("LookupIstriList()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }
        if (1) {
                return RequestError("do not support this interface");
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::StartIstriIndex(const Packet *p)
{
        LOG_DBG("StartIstriIndex()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        if (IstriMgr().StartIstriIndex(errInfo) < 0) {
                return RequestError(string("start index error: ") + errInfo.c_str());
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::StartIstriData(const Packet *p)
{
        LOG_DBG("StartIstriData()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        if (IstriMgr().StartIstriData(errInfo) < 0) {
                return RequestError(string("start data error: ") + errInfo.c_str());
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::StopIstriIndex(const Packet *p)
{
        LOG_DBG("StopIstriIndex()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        if (IstriMgr().StopIstriIndex(errInfo) < 0) {
                return RequestError(string("stop index error: ") + errInfo.c_str());
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::StopIstriData(const Packet *p)
{
        LOG_DBG("StopIstriData()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        if (IstriMgr().StopIstriData(errInfo) < 0) {
                return RequestError(string("stop data error: ") + errInfo.c_str());
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::EditIstriIndex(const Packet *p)
{
        LOG_DBG("EditIstriIndex()");
        vector<string>::iterator vsIt;
        vector<string> oldIdxRootPath, newIdxRootPath, allIdxRootPath;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        if (request["isFirstEdit"].asString() == "no") {
                for (size_t i = 0; i < request["oldIdxRootPath"].size(); ++i) {
                        oldIdxRootPath.push_back(request["oldIdxRootPath"][i].asString());
                }
        }
        for (size_t i = 0; i < request["allIdxRootPath"].size(); ++i) {
                string path = request["allIdxRootPath"][i].asString();
                for (vsIt = oldIdxRootPath.begin(); vsIt != oldIdxRootPath.end(); ++vsIt) {
                        if (*vsIt == path) {
                                break;
                        }
                }
                if (vsIt == oldIdxRootPath.end()) {
                        newIdxRootPath.push_back(path);
                }
        }

        if (IstriMgr().EditIstriIndex(oldIdxRootPath, newIdxRootPath) < 0) {
                return RequestError("edit index error");
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::EditIstriData(const Packet *p)
{
        LOG_DBG("EditIstriData()");
        vector<string>::iterator vsItAll, vsItOld;
        vector<string> oldDataPath, allDataPath, newDataPath;
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        for (size_t i = 0; i < request["allDataPath"].size(); ++i) {
                allDataPath.push_back(request["allDataPath"][i].asString());
        }
        if (request["isFirstEdit"].asString() == "no") {
                for (size_t i = 0; i < request["oldDataPath"].size(); ++i) {
                        oldDataPath.push_back(request["oldDataPath"][i].asString());
                }
                for (vsItAll = allDataPath.begin(); vsItAll != allDataPath.end(); ++vsItAll) {
                        for (vsItOld = oldDataPath.begin(); vsItOld != oldDataPath.end(); ++vsItOld) {
                                if (*vsItOld == *vsItAll) {
                                        break;
                                }
                        }
                        if (vsItOld == oldDataPath.end()) {
                                newDataPath.push_back(*vsItAll);
                        }
                }
                if (IstriMgr().EditIstriData(oldDataPath, newDataPath) < 0) {
                        return RequestError("edit data error");
                }
        } else {
                string mp = "/istri/ddfs";
                string metaPath  = request["metaPath"].asString();
                string indexPath = request["indexPath"].asString();
                string isImgDedup  = request["imgDedup"].asString();
                if (IstriMgr().FirstEditIstriData(mp, indexPath, metaPath, allDataPath, isImgDedup) < 0) {
                        return RequestError("edit data error");
                }
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::FixIstriIndex(const Packet *p)
{
        LOG_DBG("FixIstriIndex()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        if (IstriMgr().FixIstriIndex(errInfo) < 0) {
                return RequestError(string("fix index error: ") + errInfo.c_str());
        }
        return RequestOk(respond);
}

Packet *IstriJsonMgr::FixIstriData(const Packet *p)
{
        LOG_DBG("FixIstriData()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        if (IstriMgr().FixIstriData(errInfo) < 0) {
                return RequestError(string("fix data error: ") + errInfo.c_str());
        }
        return RequestOk(respond);
}


Packet *IstriJsonMgr::ClearIstriDataId(const Packet *p)
{
        LOG_DBG("ClearIstriDataId()");
        Json::Value request, respond;
        if (JsonValue(p, request) < 0) {
                return RequestError("parse request error");
        }

        string errInfo;
        string id = request["id"].asString();
        if (IstriMgr().ClearIstriDataId(id, errInfo) < 0) {
                return RequestError("clear data id error: " + errInfo);
        }
        return RequestOk(respond);
}
