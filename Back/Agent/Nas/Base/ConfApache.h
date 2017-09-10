#ifndef __CCONFAPACHE__
#define __CCONFAPACHE__

#include <string>
#include <vector>
#include <algorithm>
#include "Include/Constant.h"

#define NOT_FIND        0xFFFFFFFF
#define MAX_CFG_LINE    1024
#define KVDOT_L         '<'
#define KVDOT_R         '>'
#define STR_DIR         "Directory"
#define STR_LMT         "Limit"

class CConfApache
{
public:
        typedef struct stKeyVal
        {
                std::string             key;
                std::string             val;
                std::string             cmt;
        } stKeyVal;

        typedef struct stSection
        {
                std::string             name;
                std::string             val;
                std::vector<stKeyVal>   vkv;
                std::vector<stSection>  vs;
                uint                    deep;           // deep root=0, dir=1
                stSection*              parent;
                stSection()             {deep = 1; parent = NULL;}
                void    PrintHead(FILE* file) const;
                void    PrintBody(FILE* file) const;
                void    PrintTail(FILE* file) const;
                void    PrintMe  (FILE* file) const;
                void    ParseLine(const char* line);
        } stSection;

public:
        CConfApache(const char* path);
        ~CConfApache();

        // API for old
        int     Load(const char* path = NULL);
        int     Write(const char* path = NULL) const;
        int     Clear();

        int     AddSect  (const char* name, const char* val, stSection* psect = NULL);
        int     DelSect  (const char* name, const char* val, stSection* psect = NULL);

        int     AddKeyVal(const char* path, const char* key, const char* val = NULL, stSection* psect = NULL);
        int     DelKeyVal(const char* path, const char* key, stSection* psect = NULL);
        int     GetKeyVal(const char* path, const char* key, std::string& val, const stSection* psect = NULL) const;

        // API for new
        int     Open();
        int     Close() const;

        int     AddField(const std::string& val, const std::string& name = STR_DIR);
        int     DelField(const std::string& val, const std::string& name = STR_DIR);

        int     SetVal  (const std::string& path, const std::string& key, const std::string& val = "");
        int     GetVal  (const std::string& path, const std::string& key,       std::string& val) const;
        void    DelKey  (const std::string& path, const std::string& key);
private:
        uint    FindSect (const std::vector<stSection>& vs, const char* sect, int kvdot = KVDOT_R) const;
        uint    FindKey  (const std::vector<stKeyVal>& vkv, const char* key) const;

        const char*             m_filepath;
        std::vector<stSection>  m_vs;
        int                     m_ready;
};

#endif /*__CCONFAPACHE__*/

