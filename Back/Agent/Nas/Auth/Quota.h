#ifndef __QUOTA_H__
#define __QUOTA_H__

#include <vector>

#include "Include/Constant.h"
#include "Utils/Common/String.h"
//#include "../Base/Base.h"

#define MAX_FILE_COUNT 31998
class CQuota
{
public:
        CQuota();
        ~CQuota();
public:
        struct Quota
        {
                std::string name;

                uint   id;
                int    type;
                ulong  bused;
                ulong  bsoft;
                ulong  bhard;
                ulong  fused;
                ulong  fsoft;
                ulong  fhard;
                Quota(){
                        id    = 0;
                        type  = 0;
                        bused = 0;
                        bsoft = 0;
                        bhard = 0;
                        fused = 0;
                        fsoft = 0;
                        fhard = 0;
                }
        };

        typedef std::vector<Quota> QuotaList;
        
public:
        bool    GetQuotaDisk(String::StringArray& ary);
        bool    GetUser(const std::string& path, QuotaList& list);
        bool    GetGroup(const std::string& path, QuotaList& list);
        bool    SetUser(const std::string& path, const uint uid, const Quota& val);
        bool    SetGroup(const std::string& path, const std::string& name, const Quota& val);
        int     GetMaxBlockSize(const std::string& path);
        int     GetMaxFileSize(const std::string& path);
};

#endif /* __QUOTA_H__ */
