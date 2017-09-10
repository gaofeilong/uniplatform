#ifndef _INI_CONFIG_H_
#define _INI_CONFIG_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

/* 域下的字段信息,包括字段名和信息,以及该字段的描述 */
typedef struct _KeyInfo {
        _KeyInfo(){}
        _KeyInfo(const string& tkey):key(tkey){}
        _KeyInfo(const string& tkey, const string& val):key(tkey), value(val){}
        string         key;
        string         value;
        vector<string> keyComment;
        bool operator()(const _KeyInfo& ki)
        {
                return (key == ki.key);
        }
} KeyInfo;

/* 域信息,包括域名,域下的字段集合,以及该域的描述 */
typedef struct _FieldInfo {
        _FieldInfo(){}
        _FieldInfo(const string& fname):fieldName(fname){}
        string          fieldName;
        string          specialLine;
        vector<KeyInfo> keySet;
        vector<string>  fieldComment;
        bool operator()(const _FieldInfo& fi)
        {
                return (fieldName == fi.fieldName);
        }
} FieldInfo;

class IniConfig {
public:
        IniConfig(const string& configPath);
        ~IniConfig();

        int Open();
        int Close();

        bool FindField(const string& fieldName);
        int  AddField(const string& fieldName);
        void DelField(const string& fieldName);
        int AddSpecialLine(const string& fieldName, const string& specialLine);

        int GetVal(const string& fieldName, const string& key, string& value);
        int SetVal(const string& fieldName, const string& key, const string& value);

        void DelKey(const string& fieldName, const string& key);


private:
        int  ReadFromFile();
        int  WriteToFile();
        string Trim(const string& s, const string &letters = " \t\n\r");

private:
        vector<FieldInfo> m_FiledSet;
        string            m_ConfigPath;
};

#endif //_INI_CONFIG_H_
