#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include "ConfParser.h"

using namespace std;

CConfParser::CConfParser()
        :m_ready(0)
{
}

CConfParser::CConfParser(const string path)
        :m_filepath(path), m_ready(0)
{
}

CConfParser::~CConfParser()
{
        m_vca.clear();
}

int CConfParser::AddSect(const string& sect)
{
        string newSect = sect;
        Trim(newSect, " \t\r\n");

        if (newSect.empty())
        {
                LOG_ERR("AddSect(): empty sect");
                return -1;
        }
        // 如果域名是以下字母开头 则将 KeyVal 添加到最前面,当全局参数
        if('*' == newSect[0] || '#' == newSect[0])
        {
                // 如果"#" Sect 不存在,则添加
                stConfSect newca("#");
                if (m_vca.empty())
                {
                        m_vca.push_back(newca);
                }
                else
                {
                        // 如果 m_vca 有东西,而且第一个不是#则添加
                        if (m_vca[0].sect[0] != '#')
                        {
                                m_vca.insert(m_vca.begin(), newca);
                        }
                }
                return 0;
        }
        if ('[' != newSect[0])
        {
                newSect = std::string("[") + newSect;
        }
        if (']' != newSect[strlen(newSect.c_str())-1])
        {
                newSect += "]";
        }

        // 如果不存在 则添加
        if (NOT_FIND == FindSect(newSect.c_str()))
        {
                m_vca.push_back(stConfSect(newSect));
        }

        return 0;
}

int CConfParser::AddKeyVal(const string& sect, const string& key, const string& value)
{
        unsigned int nSect = NOT_FIND, nKey = NOT_FIND;

        nSect = FindSect(sect.c_str());
        if (nSect >= m_vca.size())
        {
                LOG_ERR("AddKeyVal(): sect not find: %s", sect.c_str());
                return -1;
        }

        // 如果 key 存在 不要重复添加 会修改原来的 value
        nKey = FindKey(nSect, key.c_str());
        if (NOT_FIND != nKey && nKey < m_vca[nSect].vkey.size())
        {
                // 如果 value='#'，不更新值。
                if (value != "#")
                {
                        LOG_WRN("AddKeyVal(): key already exist, updated: %s:%s=%s", 
                                sect.c_str(), key.c_str(), value.c_str());
                        m_vca[nSect].vkey[nKey].val = value;
                }
                return 0;
        }

        stConfKeyVal newckv(key);
        if (value != "#")
        {
                newckv.val = value;
        }

        m_vca[nSect].vkey.push_back(newckv);
        return 0;
}

int CConfParser::AddComment(const string& sect, const string& cmt)
{
        unsigned int nSect = NOT_FIND;
        nSect = FindSect(sect.c_str());
        if (nSect >= m_vca.size())
        {
                LOG_ERR("AddComment(): sect not find: %s", sect.c_str());
                return -1;
        }

        // 处理注释, 给注释带上换行\n
        string strcmt = cmt;
        if ('\n' == strcmt[0])
        {
                strcmt = "\n";
        }
        else if ('#' != strcmt[0])
        {
                strcmt = string("#") + strcmt;
        }
        if ('\n' != strcmt[strlen(strcmt.c_str())-1])
        {
                strcmt += "\n";
        }
        // 注释中间有'\n'则去掉后面多余的字符串
        strcmt = strcmt.substr(0, strcmt.find('\n') + 1);

        stConfKeyVal newckv;
        newckv.cmt = strcmt;

        m_vca[nSect].vkey.push_back(newckv);
        return 0;
}

int CConfParser::DelSect(const string& sect)
{
        unsigned int nSect = NOT_FIND;

        nSect = FindSect(sect.c_str());
        if (NOT_FIND == nSect  || nSect >= m_vca.size())
        {
                LOG_WRN("DelSect(): sect not find: %s", sect.c_str());
                return 0;
        }

        m_vca.erase(m_vca.begin() + nSect);
        return 0;
}

int CConfParser::DelKey(const string& sect, const string& key)
{
        unsigned int nSect = NOT_FIND, nKey = NOT_FIND;

        nSect = FindSect(sect.c_str());
        if (nSect >= m_vca.size())
        {
                LOG_ERR("DelKeyVal(): sect not find: %s", sect.c_str());
                return -1;
        }

        nKey = FindKey(nSect, key.c_str());
        if (nKey >= m_vca[nSect].vkey.size())
        {
                LOG_ERR("DelKeyVal(): key not find: %s:%s", sect.c_str(), key.c_str());
                return -1;
        }

        m_vca[nSect].vkey.erase(m_vca[nSect].vkey.begin() + nKey);
        return 0;
}
// 一般包含最前面一个空的注释域
unsigned int CConfParser::GetSectCount() const
{
        return m_vca.size();
}

unsigned int CConfParser::GetKeyCount(const string& sect) const
{
        unsigned nSect = FindSect(sect.c_str());
        return (NOT_FIND==nSect) ? 0 : (m_vca[nSect].vkey.size());
}

unsigned int CConfParser::GetSectList(std::vector<std::string>& vs) const
{
        vs.clear();
        for (uint i=0; i<m_vca.size(); ++i)
        {
                vs.push_back(m_vca[i].sect);
        }
        return 0;
}

unsigned int CConfParser::GetKeyList(const std::string& sect, std::vector<std::string>& vs) const
{
        vs.clear();
        unsigned int nSect = NOT_FIND;
        nSect = FindSect(sect.c_str());
        if (nSect >= m_vca.size())
        {
                LOG_WRN("GetKeyList(): sect not find: %s", sect.c_str());
                return NOT_FIND;
        }
        for (uint i=0; i<m_vca[nSect].vkey.size(); ++i)
        {
                vs.push_back(m_vca[nSect].vkey[i].key);
        }
        return 0;
}


int CConfParser::GetVal(const string& sect, const string& key, string& value) const
{
        value.clear();
        unsigned int nSect = NOT_FIND, nKey = NOT_FIND;

        nSect = FindSect(sect.c_str());
        if (nSect >= m_vca.size())
        {
                LOG_WRN("GetVal(): sect not find: %s", sect.c_str());
                return -1;
        }

        nKey = FindKey(nSect, key.c_str());
        if (nKey >= m_vca[nSect].vkey.size())
        {
                LOG_WRN("GetVal(): illegal key: %s:%s", sect.c_str(), key.c_str());
                return -1;
        }

        value = m_vca[nSect].vkey[nKey].val;
        return 0;
}

int CConfParser::SetVal(const std::string& sect, const std::string& key, const std::string& value)
{
        return AddKeyVal(sect, key, value);
}

int CConfParser::Load(const char* filepath)
{
        if (filepath && *filepath && m_filepath != filepath)
        {
                m_filepath = filepath;
                m_ready = 0;
        }
        if (m_filepath.empty())
        {
                LOG_ERR("Load(): empty filename!");
                return RET_FAILED;
        }
        if (m_ready)
        {
                LOG_WRN("Load(%s): no need to load twice.", m_filepath.c_str());
                return RET_SUCCESS;
        }
        m_vca.clear();
        m_ready = 0;
        filepath = m_filepath.c_str();

        FILE* fifo = NULL;
        if (NULL == (fifo = fopen(m_filepath.c_str(), "r")))
        {
                LOG_ERR("fopen(%s): %m", filepath);
                return -1;
        }

        char cline[1024] = {0};
        string sline;
        int lineno = 0;

        m_vca.push_back(stConfSect("#"));
        while (NULL != fgets(cline, 1024, fifo))
        {
                ++lineno;
                sline = cline;
                Trim(sline);
                if (0 == strlen(sline.c_str()))
                {
                        stConfKeyVal newckv;
                        newckv.cmt = cline;
                        m_vca[m_vca.size()-1].vkey.push_back(newckv);
                }
                else if ('#' == sline[0] || ';' == sline[0])
                {
                        stConfKeyVal newckv;
                        newckv.cmt = cline;
                        if ('\n' != newckv.cmt[newckv.cmt.length()-1])
                        {
                                newckv.cmt += "\n";
                        }
                        m_vca[m_vca.size()-1].vkey.push_back(newckv);
                }
                else if ('[' == sline[0])
                {
                        m_vca.push_back(stConfSect(sline));
                }
                else
                {
                        unsigned long insign = sline.find('=');
                        if (std::string::npos == insign)
                        {
                                stConfKeyVal newckv;
                                newckv.cmt = cline;
                                m_vca[m_vca.size()-1].vkey.push_back(newckv);
                                if (strlen(sline.c_str()))
                                {
                                        LOG_WRN("Load(%s): illegal config line %d:%s", 
                                        filepath, lineno, cline);
                                }
                        }
                        else
                        {
                                stConfKeyVal newckv;
                                newckv.key = sline.substr(0, insign);
                                newckv.val = sline.substr(insign+1, sline.length());
                                Trim(newckv.key);
                                Trim(newckv.val);
                                newckv.cmt = cline;
                                m_vca[m_vca.size()-1].vkey.push_back(newckv);
                        }
                }
        }

        fclose(fifo);
        m_ready = 1;
        LOG_DBG("Load(%s) success! count %d lines.", filepath, lineno);

        return RET_SUCCESS;
}

int CConfParser::Write(const char* filepath) const
{
        if (!filepath || !*filepath)
        {
                if (m_filepath.empty())
                {
                        LOG_ERR("Write(): empty filepath");
                        return -1;
                }
                filepath = m_filepath.c_str();
        }
        if (m_vca.empty())
        {
                LOG_ERR("Write(): empty config vector");
                return -1;
        }

        unsigned int nSect = 0, nKey = 0;
        FILE* fifo = NULL;
        if (NULL == (fifo = fopen(filepath, "w")))
        {
                LOG_ERR("fopen(%s): %m", filepath);
                return -1;
        }

        for (nSect=0; nSect<m_vca.size(); ++nSect)
        {
                if (m_vca[nSect].sect.empty())
                {
                        LOG_WRN("Write(%s): empty sect in config vector: %d", filepath, nSect);
                        continue;
                }
                else
                {
                        if (' ' == m_vca[nSect].sect[0] || '*' == m_vca[nSect].sect[0] || 
                                '#' == m_vca[nSect].sect[0] || '\n' == m_vca[nSect].sect[0])
                        {
                                // this is first sect, global sect
                        }
                        else if ('[' == m_vca[nSect].sect[0])
                        {
                                fprintf(fifo, "%s\n", m_vca[nSect].sect.c_str());
                        }
                }
                for (nKey=0; nKey<m_vca[nSect].vkey.size(); ++nKey)
                {
                        if (!m_vca[nSect].vkey[nKey].key.empty())
                        {
                                fprintf(fifo, "%s=%s\n", 
                                m_vca[nSect].vkey[nKey].key.c_str(), m_vca[nSect].vkey[nKey].val.c_str());
                        }
                        else
                        {
                                fprintf(fifo, "%s", m_vca[nSect].vkey[nKey].cmt.c_str());
                        }
                }
        }

        fclose(fifo);
        LOG_DBG("Write(%s) success! count %lu sections.", filepath, m_vca.size());

        return 0;
}

int CConfParser::Clear()
{
        m_vca.clear();
        m_ready = 0;
        //LOG_DBG("Clear() cleared: %s", m_filepath.c_str());
        return 0;
}

int CConfParser::Open()
{
        return Load();
}

int CConfParser::Close() const
{
        return Write();
}

unsigned int CConfParser::FindSect(const char* sect) const
{
        unsigned int nSect = NOT_FIND;

        if (!sect || !*sect)
        {
                return NOT_FIND;
        }
        if (m_vca.empty())
        {
                return NOT_FIND;
        }
        if (' ' == m_vca[0].sect[0] || '*' == m_vca[0].sect[0] || '#' == m_vca[0].sect[0])
        {
                if (' ' == *sect || '*' == *sect || '#' == *sect || '\n' == *sect)
                {
                        return 0;
                }
        }

        for (nSect=0; nSect<m_vca.size(); ++nSect)
        {
                if (m_vca[nSect].sect == sect)
                {
                        return nSect;
                }
        }

        return NOT_FIND;
}

unsigned int CConfParser::FindKey(const unsigned int nSect, const char* key) const
{
        unsigned int nKey = NOT_FIND;

        if (nSect >= m_vca.size())
        {
                return NOT_FIND;
        }
        if (m_vca[nSect].vkey.empty())
        {
                return NOT_FIND;
        }
        if (!key || !*key)
        {
                return NOT_FIND;
        }

        for (nKey=0; nKey<m_vca[nSect].vkey.size(); ++nKey)
        {
                if (m_vca[nSect].vkey[nKey].key == key)
                {
                        return nKey;
                }
        }

        return NOT_FIND;
}

// 配置文件第一个域没有域名, 设置为空格" "或者"#", 不允许 NULL, 为保留开头注释. 所有函数执行结果: 成功0,失败-1
