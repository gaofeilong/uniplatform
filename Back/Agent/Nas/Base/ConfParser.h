#ifndef __CONFPARSER_H__
#define __CONFPARSER_H__

#include <string>
#include <vector>
#include <algorithm>
#include "Include/Constant.h"

#define NOT_FIND 0xFFFFFFFF

//using namespace std;


class CConfParser
{
typedef struct stConfKeyVal
{
        std::string key;
        std::string val;
        std::string cmt;
        stConfKeyVal() {}
        stConfKeyVal(std::string k, std::string v = "") :key(k), val(v) {}
} stConfKeyVal;

typedef struct stConfSect
{
        std::string sect;
        std::vector<stConfKeyVal> vkey;
        stConfSect() {}
        stConfSect(std::string name) :sect(name) {}
} stConfSect;

public:
        CConfParser();
        CConfParser(const std::string path);
        ~CConfParser();

// 增加
        int AddSect(const std::string& sect);
        int AddKeyVal(const std::string& sect, const std::string& key, const std::string& value = "");
        int AddComment(const std::string& sect, const std::string& cmt);

// 删除
        int DelSect(const std::string& sect);
        int DelKey(const std::string& sect, const std::string& key);

// 获取
        int GetVal(const std::string& sect, const std::string& key, std::string& value) const;
        unsigned int GetSectCount() const;
        unsigned int GetSectList(std::vector<std::string>& vs) const;
        unsigned int GetKeyCount(const std::string& sect) const;
        unsigned int GetKeyList(const std::string& sect, std::vector<std::string>& vs) const;

// 设置
        int SetVal(const std::string& sect, const std::string& key, const std::string& value = "");

// 其他操作
        int Load (const char* filepath = NULL);
        int Write(const char* filepath = NULL) const;
        int Clear();

        int Open();
        int Close() const;
private:

        unsigned int FindSect(const char* sect) const;
        unsigned int FindKey(const unsigned int nSect, const char* key) const;

        std::string             m_filepath;
        std::vector<stConfSect> m_vca;
        int                     m_ready;
};

#endif

