#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "ConfApache.h"

CConfApache::CConfApache(const char* path)
        :m_filepath(path), m_ready(0)
{
}

CConfApache::~CConfApache()
{
}

// Load 将会修改m_filepath指针为path
int CConfApache::Load(const char* path)
{
        if (path && *path && m_filepath != path)
        {
                m_filepath = path;
                m_ready = 0;
        }
        if (!m_filepath || !*m_filepath)
        {
                LOG_ERR("Load(): NULL filename!");
                return RET_FAILED;
        }
        if (m_ready)
        {
                LOG_WRN("Load(): no need to load twice.");
                return RET_SUCCESS;
        }
        m_vs.clear();

        FILE* file = fopen(m_filepath, "r");
        if (NULL == file)
        {
                LOG_ERR("Load(): NULL file!");
                return RET_FAILED;
        }

        uint lineno = 0, deep = 0;
        char cline[MAX_CFG_LINE] = {0};
        std::string sline;
        stSection sts_global;
        sts_global.name = sts_global.val = "#";
        sts_global.deep = 0;
        m_vs.push_back(sts_global);

        stSection* plast = &m_vs[0];

        while (NULL != fgets(cline, MAX_CFG_LINE, file))
        {
                ++lineno;
                sline = cline;
                Trim(sline, " \t\r\n");
                // empty line, push_back
                if (3 > strlen(sline.c_str()))
                {
                        stKeyVal newkv;
                        newkv.cmt = cline;
                        plast->vkv.push_back(newkv);
                        continue;
                }
                // comment, push_back
                if ('#' == sline[0] || ';' == sline[0])
                {
                        stKeyVal newkv;
                        newkv.cmt = cline;
                        if ('\n' != newkv.cmt[newkv.cmt.length()-1])
                        {
                                newkv.cmt += "\n";
                        }
                        plast->vkv.push_back(newkv);
                        continue;
                }
                // start of section
                if ('<' == sline[0] && '/' != sline[1])
                {
                        ++deep;
                        stSection newsect;
                        newsect.ParseLine(sline.c_str());
                        newsect.deep = deep;
                        newsect.parent = plast;
                        plast->vs.push_back(newsect);
                        plast = &(plast->vs[plast->vs.size() - 1]);
                        continue;
                }
                // end of sectoin
                if ('<' == sline[0] && '/' == sline[1])
                {
                        --deep;
                        plast = plast->parent;
                        continue;
                }

                // parse line, must match ONE SPACE between key and value
                unsigned long hassign = sline.find(' ');
                // illegal line, push_back as comment
                if (std::string::npos == hassign)
                {
                        stKeyVal newkv;
                        newkv.key = sline;
                        newkv.cmt = cline;
                        plast->vkv.push_back(newkv);
                        continue;
                }

                stKeyVal newkv;
                // special key, find SPACE twice, make it unique, too.
                if (0 == memcmp(sline.c_str(), "Allow from", 10)
                 || 0 == memcmp(sline.c_str(), "Require user", 12)
                 || 0 == memcmp(sline.c_str(), "Require group", 13)
                 || 0 == memcmp(sline.c_str(), "Alias", 5))
                {
                        hassign = sline.find(' ', hassign+1);
                        newkv.key = sline.substr(0, hassign);
                        if (std::string::npos != hassign)
                        {
                                newkv.val = sline.substr(hassign+1, sline.length());
                        }
                }
                else
                {
                        newkv.key = sline.substr(0, hassign);
                        if (std::string::npos != hassign)
                        {
                                newkv.val = sline.substr(hassign+1, sline.length());
                        }
                }
                Trim(newkv.key);
                Trim(newkv.val);
                newkv.cmt = cline;
                plast->vkv.push_back(newkv);
        }

        fclose(file);
        m_ready = 1;
        LOG_DBG("Load(): success! path=%s", m_filepath);

        return RET_SUCCESS;
}

int CConfApache::Write(const char* path) const
{
        if (!path || !*path)
        {
                path = m_filepath;
        }
        if (!path || !*path)
        {
                LOG_ERR("Write(): NULL filename!");
                return RET_FAILED;
        }
        if (m_vs.empty())
        {
                LOG_ERR("Write(): Load first, please!");
                return RET_FAILED;
        }

        FILE* file = fopen(path, "w");
        if (NULL == file)
        {
                LOG_ERR("Write(): NULL file!");
                return RET_FAILED;
        }
        // every body should print itself
        m_vs[0].PrintBody(file);
        fclose(file);
        LOG_DBG("Write(): success! path=%s", m_filepath);

        return RET_SUCCESS;
}

int CConfApache::Clear()
{
        m_vs.clear();
        m_ready = 0;
        return RET_SUCCESS;
}

// val为路径，其中'<'或者'>'是路径层次分割符
int CConfApache::AddSect  (const char* name, const char* val, stSection* psect)
{
        if (!name || !*name || !val || !*val)
        {
                LOG_ERR("AddSect(): empty param! name=%s val=%s", name, val);
                return RET_FAILED;
        }
        if (!psect)
        {
                psect = &m_vs[0];
        }

        const char* ctmp = val;
        char thus[256] = {0};
        uint i = 0;
        char kvdot = (KVDOT_L==*ctmp || KVDOT_R==*ctmp) ? *(ctmp++): KVDOT_R;
        for (i=0; ctmp[i] && ctmp[i]!=KVDOT_L && ctmp[i]!=KVDOT_R; ++i)
        {
                thus[i] = ctmp[i];
        }
        ctmp += i;

        uint nSect = FindSect(psect->vs, thus, kvdot);
        if (NOT_FIND != nSect)
        {
                if (*ctmp)
                {
                        return AddSect(name, ctmp, &(psect->vs[nSect]));
                }
                LOG_WRN("AddSect(): section already exist! parent=%s name=%s val=%s", 
                        psect->val.c_str(), name, val);
                return RET_SUCCESS;
        }

        if (*ctmp)
        {
                LOG_ERR("AddSect(): invalid path: %s&%s", val, *ctmp);
                return RET_FAILED;
        }

        stSection newsect;
        newsect.name = name;
        newsect.val = thus;
        newsect.parent = psect;
        newsect.deep = psect->deep + 1;
        psect->vs.push_back(newsect);
        LOG_DBG("AddSect(): success! parent=%s name=%s val=%s",
                psect->val.c_str(), name, val);

        return RET_SUCCESS;
}

int CConfApache::DelSect  (const char* name, const char* val, stSection* psect)
{
        if (!name || !*name || !val || !*val)
        {
                LOG_ERR("DelSect(): empty param! val=%s", val);
                return RET_FAILED;
        }
        if (!psect)
        {
                psect = &m_vs[0];
        }

        const char* ctmp = val;
        char thus[256] = {0};
        uint i = 0;
        char kvdot = (KVDOT_L==*ctmp || KVDOT_R==*ctmp) ? *(ctmp++): KVDOT_R;
        for (i=0; ctmp[i] && ctmp[i]!=KVDOT_L && ctmp[i]!=KVDOT_R; ++i)
        {
                thus[i] = ctmp[i];
        }
        ctmp += i;

        uint nSect = FindSect(psect->vs, thus, kvdot);
        if (NOT_FIND == nSect)
        {
                LOG_WRN("DelSect(): cannot find section! parent=%s val=%s",
                        psect->val.c_str(), val);
                return RET_SUCCESS;
        }
        if (*ctmp)
        {
                return DelSect(name, ctmp, &(psect->vs[nSect]));
        }

        psect->vs.erase(psect->vs.begin() + nSect);
        LOG_DBG("DelSect(): success! parent=%s val=%s", psect->val.c_str(), val);

        return RET_SUCCESS;
}

// path为路径，其中'<'或者'>'是路径层次分割符
int CConfApache::AddKeyVal(const char* path, const char* key, const char* val, stSection* psect)
{
        if (!path || !*path || !key || !*key)
        {
                LOG_ERR("AddKeyVal(): empty param! path=%s key=%s", path, key);
                return RET_FAILED;
        }
        if (!psect)
        {
                psect = &m_vs[0];
        }

        uint i = 0, nSect = 0, nKey = 0;
        // path='#' will add in global section
        if ('#' == *path)
        {
                nKey = FindKey(m_vs[0].vkv, key);
                if (NOT_FIND != nKey)
                {
                        if (val && *val && '#' == *val)
                        {
                                LOG_DBG("AddKeyVal(): dont update value! parent=%s path=%s key=%s val=%s", 
                                        psect->val.c_str(), path, key, val);
                        }
                        else
                        {
                                m_vs[0].vkv[nKey].val = val;
                                LOG_DBG("AddKeyVal(): updated value! parent=%s path=%s key=%s val=%s", 
                                        psect->val.c_str(), path, key, val);
                        }
                        return RET_SUCCESS;
                }
                stKeyVal stkv;
                stkv.key = key;
                if (val && *val && '#' != *val)
                {
                        stkv.val = val;
                }
                m_vs[0].vkv.push_back(stkv);
                LOG_DBG("AddKeyVal(): success! parent=%s path=%s key=%s", 
                        psect->val.c_str(), path, key);
                return RET_SUCCESS;
        }

        const char* ctmp = path;
        char thus[256] = {0};
        char kvdot = (KVDOT_L==*ctmp || KVDOT_R==*ctmp) ? *(ctmp++): KVDOT_R;
        for (i=0; ctmp[i] && ctmp[i]!=KVDOT_L && ctmp[i]!=KVDOT_R; ++i)
        {
                thus[i] = ctmp[i];
        }
        ctmp += i;

        nSect = FindSect(psect->vs, thus, kvdot);
        if (NOT_FIND == nSect)
        {
                LOG_ERR("AddKeyVal(): cannot find section %c%s in %s", 
                        kvdot, thus, psect->val.c_str());
                return RET_FAILED;
        }
        if (*ctmp)
        {
                return AddKeyVal(ctmp, key, val, &(psect->vs[nSect]));
        }

        // 如果 key="#"，就修改section下标签的value。
        // 如果 val="#"，不更新key的值。
        if ('#' == *key)
        {
                //ctmp = key + 1;
                //nSect = FindSect(psect->vs, ctmp, KVDOT_L);
                if (val && *val && '#' == *val)
                {
                        LOG_DBG("AddKeyVal(): dont update value! parent=%s path=%s key=%s val=%s", 
                                psect->val.c_str(), path, key, val);
                }
                else
                {
                        psect->vs[nSect].val = val;
                        LOG_DBG("AddKeyVal(): updated value! parent=%s path=%s key=%s val=%s", 
                                psect->val.c_str(), path, key, val);
                }
                return RET_SUCCESS;
        }
        nKey = FindKey(psect->vs[nSect].vkv, key);
        if (NOT_FIND != nKey)
        {
                if (val && *val && '#' == *val)
                {
                        LOG_DBG("AddKeyVal(): dont update value! parent=%s path=%s key=%s val=%s", 
                                psect->val.c_str(), path, key, val);
                }
                else
                {
                        psect->vs[nSect].vkv[nKey].val = val;
                        LOG_DBG("AddKeyVal(): updated value! parent=%s path=%s key=%s val=%s", 
                                psect->val.c_str(), path, key, val);
                }
                return RET_SUCCESS;
        }

        stKeyVal stkv;
        stkv.key = key;
        if (val && *val && '#' != *val)
        {
                stkv.val = val;
        }
        psect->vs[nSect].vkv.push_back(stkv);
        LOG_DBG("AddKeyVal(): success! parent=%s path=%s key=%s val=%s", 
                psect->val.c_str(), path, key, val);

        return RET_SUCCESS;
}

int CConfApache::DelKeyVal(const char* path, const char* key, stSection* psect)
{
        if (!path || !*path || !key || !*key)
        {
                LOG_ERR("DelKeyVal(): empty param! path=%s key=%s", path, key);
                return RET_FAILED;
        }
        if (!psect)
        {
                psect = &m_vs[0];
        }

        uint i = 0, nSect = 0, nKey = 0;
        if ('#' == *path)
        {
                nKey = FindKey(m_vs[0].vkv, key);
                if (NOT_FIND == nKey)
                {
                        LOG_WRN("DelKeyVal(): key not exist! parent=%s path=%s key=%s", 
                                psect->val.c_str(), path, key);
                        return RET_SUCCESS;
                }

                m_vs[0].vkv.erase(m_vs[0].vkv.begin() + nKey);
                LOG_DBG("DelKeyVal(): success! parent=%s path=%s key=%s", 
                        psect->val.c_str(), path, key);
                return RET_SUCCESS;
        }

        const char* ctmp = path;
        char thus[256] = {0};
        char kvdot = (KVDOT_L==*ctmp || KVDOT_R==*ctmp) ? *(ctmp++): KVDOT_R;
        for (i=0; ctmp[i] && ctmp[i]!=KVDOT_L && ctmp[i]!=KVDOT_R; ++i)
        {
                thus[i] = ctmp[i];
        }
        ctmp += i;

        nSect = FindSect(psect->vs, thus, kvdot);
        if (NOT_FIND == nSect)
        {
                LOG_WRN("DelKeyVal(): cannot find section: %s", thus);
                return RET_SUCCESS;
        }
        if (*ctmp)
        {
                return DelKeyVal(ctmp, key, &(psect->vs[nSect]));
        }

        nKey = FindKey(psect->vs[nSect].vkv, key);
        if (NOT_FIND == nKey)
        {
                LOG_WRN("DelKeyVal(): key not exist! parent=%s path=%s key=%s", 
                        psect->val.c_str(), path, key);
                return RET_SUCCESS;
        }

        psect->vs[nSect].vkv.erase(psect->vs[nSect].vkv.begin() + nKey);
        LOG_DBG("DelKeyVal(): success! parent=%s path=%s key=%s", psect->val.c_str(), path, key);

        return RET_SUCCESS;
}

// 如果 key='#'，则获取获取标签的值；其他情况获取键值
int CConfApache::GetKeyVal(const char* path, const char* key, std::string& val, const stSection* psect) const
{
        if (!path || !*path || !key || !*key)
        {
                LOG_ERR("GetKeyVal(): empty param! path=%s key=%s", path, key);
                return RET_FAILED;
        }
        if (!psect)
        {
                psect = &m_vs[0];
        }

        uint i = 0, nSect = 0, nKey = 0;
        if ('#' == *path)
        {
                nKey = FindKey(m_vs[0].vkv, key);
                if (NOT_FIND == nKey)
                {
                        LOG_ERR("GetKeyVal(): key not exist! path=%s key=%s", path, key);
                        return RET_FAILED;
                }

                val = m_vs[0].vkv[nKey].val;
                LOG_DBG("GetKeyVal(): success! path=%s key=%s val=%s", path, key, val.c_str());
                return RET_SUCCESS;
        }
        const char* ctmp = path;
        char thus[256] = {0};
        char kvdot = (KVDOT_L==*ctmp || KVDOT_R==*ctmp) ? *(ctmp++): KVDOT_R;
        for (i=0; ctmp[i] && ctmp[i]!=KVDOT_L && ctmp[i]!=KVDOT_R; ++i)
        {
                thus[i] = ctmp[i];
        }
        ctmp += i;

        nSect = FindSect(psect->vs, thus, kvdot);
        if (NOT_FIND == nSect)
        {
                LOG_ERR("GetKeyVal(): cannot find section! path=%s", thus);
                return RET_FAILED;
        }
        if (*ctmp)
        {
                return GetKeyVal(ctmp, key, val, &(psect->vs[nSect]));
        }
        // 如果 key="#Limit"，就获取section下Limit标签的value。
        if ('#' == *key)
        {
                //ctmp = key + 1;
                //nSect = FindSect(psect->vs, ctmp, KVDOT_L);
                val = psect->vs[nSect].val;
                LOG_DBG("GetKeyVal(): success! path=%s key=%s val=%s", path, key, val.c_str());
                return RET_SUCCESS;
        }

        nKey = FindKey(psect->vs[nSect].vkv, key);
        if (NOT_FIND == nKey)
        {
                LOG_ERR("GetKeyVal(): cannot find key! path=%s key=%s", path, key);
                return RET_FAILED;
        }

        val = psect->vs[nSect].vkv[nKey].val;
        LOG_DBG("GetKeyVal(): success! path=%s key=%s val=%s", path, key, val.c_str());

        return RET_SUCCESS;
}

uint CConfApache::FindSect(const std::vector<stSection>& vs, const char* sect, int kvdot) const
{
        if (!sect || !*sect)
        {
                return NOT_FIND;
        }

        if ('#' == *sect || '*' == *sect)
        {
                return  0;
        }

        uint nSect = 0;
        // kvdot 用来区分比较左边还是右边
        if (KVDOT_L == kvdot)
        {
                for (nSect=0; nSect<vs.size(); ++nSect)
                {
                        if (vs[nSect].name == sect)
                        {
                                return nSect;
                        }
                }
        }
        else if (KVDOT_R == kvdot)
        {
                for (nSect=0; nSect<vs.size(); ++nSect)
                {
                        if (vs[nSect].val == sect)
                        {
                                return nSect;
                        }
                }
        }

        return NOT_FIND;
}

uint CConfApache::FindKey (const std::vector<stKeyVal>& vkv, const char* key) const
{
        if (!key || !*key)
        {
                return NOT_FIND;
        }

        uint nKey = 0;
        for (nKey=0; nKey<vkv.size(); ++nKey)
        {
                if (vkv[nKey].key == key)
                {
                        return nKey;
                }
        }

        return NOT_FIND;
}

void CConfApache::stSection::PrintHead(FILE* file) const
{
        char line_head[10] = {0};
        for (uint i=1; i < deep; ++i)
        {
                line_head[i-1] = '\t';
        }
        fprintf(file, "%s<%s %s>\n", line_head, name.c_str(), val.c_str());
}

void CConfApache::stSection::PrintTail(FILE* file) const
{
        char line_head[10] = {0};
        for (uint i=1; i < deep; ++i)
        {
                line_head[i-1] = '\t';
        }
        fprintf(file, "%s</%s>\n", line_head, name.c_str());
}

void CConfApache::stSection::PrintBody(FILE* file) const
{
        char line_head[10] = {0};
        uint i = 0;

        for (i=0; i < deep; ++i)
        {
                line_head[i] = '\t';
        }

        for (i=0; i < vkv.size(); ++i)
        {
                if (!vkv[i].key.empty())
                {
                        fprintf(file, "%s%s  %s\n", line_head, 
                                vkv[i].key.c_str(), vkv[i].val.c_str());
                }
                else
                {
                        fprintf(file, "%s", vkv[i].cmt.c_str());
                }
        }

        for (i=0; i < vs.size(); ++i)
        {
                vs[i].PrintMe(file);
        }
}

void CConfApache::stSection::PrintMe(FILE* file) const
{
        PrintHead(file);
        PrintBody(file);
        PrintTail(file);
}

void CConfApache::stSection::ParseLine(const char* line)
{
        const char* ctmp = ('<'==*line) ? line+1 : line;
        char label[64] = {0};
        char value[256] = {0};
        uint i = 0;
        for (i=0; ctmp[i] && ctmp[i]!=' ' && ctmp[i]!='\t'; ++i)
        {
                label[i] = ctmp[i];
        }
        ctmp += i + 1;
        for (i=0; ctmp[i] && ctmp[i]!='>' && ctmp[i]!='\n'; ++i)
        {
                value[i] = ctmp[i];
        }

        name = label;
        val = value;
}

int CConfApache::Open()
{
        return Load();
}

int CConfApache::Close() const
{
        return Write();
}

int CConfApache::AddField(const std::string& val, const std::string& name)
{
        return AddSect(name.c_str(), val.c_str());
}

int CConfApache::DelField(const std::string& val, const std::string& name)
{
        return DelSect(name.c_str(), val.c_str());
}

int CConfApache::SetVal  (const std::string& path, const std::string& key, const std::string& val)
{
        return AddKeyVal(path.c_str(), key.c_str(), val.c_str());
}

int CConfApache::GetVal  (const std::string& path, const std::string& key,       std::string& val) const
{
        return GetKeyVal(path.c_str(), key.c_str(), val);
}

void  CConfApache::DelKey(const std::string& path, const std::string& key)
{
       DelKeyVal(path.c_str(), key.c_str());
}

