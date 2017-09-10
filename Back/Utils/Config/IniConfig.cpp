/* 
 * @ Date        : 2012-07-12
 * @ Auther      : wangbo
 * @ Description : 对配置文件的操作。添加删除键值，添加删除域，设置键值，域
 *
 * @ History     : 2012-07-12创建
 *
 */
#include "IniConfig.h"

#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <sys/syscall.h>

//#include "Include/Types.h"
#include "Utils/Common/CommonFunc.h"

#define  GLOBAL_FIELD      "*"
#define  GLOBAL_FIELD_NAME "[*]"
#define  BUF256            256
#define  BUF4K             4096

/**
 * IniConfig() - 配置文件构造函数
 * @configPath: 配置文件路径
 */
IniConfig::IniConfig(const string& configPath)
{
        m_ConfigPath = configPath;
}

IniConfig::~IniConfig()
{
}

/**
 * Open() - 初始化函数，用来读取已经存在的配置文件
 * @return  0: 成功
 *         -1: 失败，读取文件失败,如果设置空的文件可以忽略该错误
 */
int IniConfig::Open()
{
        int ret = 0;
        ret = access(m_ConfigPath.c_str(), F_OK);
        /* 不存在该配置文件 */
        if (ret != 0) {
                return -1;
        }

        /* 存在该配置文件 */
        ret = ReadFromFile();
        if (ret < 0) {
                ret = -1;
        }
        return ret;
}

/**
 * Close() - 把内存中设置好的配置文件结构，写入文件中
 * @return  0: 成功
 *         -1：失败，写入文件失败
 */
int IniConfig::Close()
{
        int ret = WriteToFile();
        if (ret < 0) {
                ret = -1;
        }
        return ret;
}

/**
 * FindField() - 查找域信息
 * @fieldName: 查找域字段的名字
 * @return true: 存在
 *         false:不存在
 *
 */
bool IniConfig::FindField(const string& fieldName)
{
        bool bret = false;
        if(Trim(fieldName).empty()){
                return bret;
        }

        string tFieldName = "[" + Trim(fieldName) + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        if (fieldIter != m_FiledSet.end()) {
                bret = true;
        }
        return bret;
}

/**
 * AddField() - 添加域信息
 * @fieldName: 准备添加的域字段的名字
 * @return 0:成功
 *         -1:fieldname不符合要求，为空
 *
 */
int IniConfig::AddField(const string& fieldName)
{
        if(Trim(fieldName).empty()){
                return -1;
        }

        string tFieldName = "[" + Trim(fieldName) + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        if (fieldIter == m_FiledSet.end()) {
                /* 添加域时注释和字段集合为空 */
                FieldInfo fi;
                fi.fieldName = tFieldName;
                m_FiledSet.push_back(fi);
        }

        return 0;
}

/**
 *AddSpecialLine -- 为域添加特出行，该行不属于配置文件格式,开头不含#或[，行中不包含= 不可以为空
 *@fieldName  要添加特出行所在的域
 *@specialLine 要添加的特出行
 *@return 0 成功 -1 失败,格式不符合 -2 没有该域
 */
int IniConfig::AddSpecialLine(const string& fieldName, const string& specialLine)
{
        string line = Trim(specialLine);
        if (line.empty() || line[0] == '#' || line[0] == ';' ||
                            line[0] == '[' || line.find("=") != string::npos){
                return -1;
        }

        string tFieldName = "[" + fieldName + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        if (fieldIter != m_FiledSet.end()) {
                fieldIter->specialLine = line;
        } else {
                return -2;
        }
        return 0;
}

/**
 * DelField() - 删除域信息，该域下得所有字段信息都被删除
 * @fieldName: 准备添加的域字段的名字
 */
void IniConfig::DelField(const string& fieldName)
{
        string tFieldName = "[" + Trim(fieldName) + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        /* 删除对应域 */
        if (fieldIter != m_FiledSet.end()) {
                m_FiledSet.erase(fieldIter);
        }
}

/**
 * GetVal() - 获取指定字段信息
 * @fieldName: 准备添加的域字段的名字
 * @key: 字段名
 * @value: 存放该字段名对应的值
 * @return : 0: 成功
 *          -1: 失败,没有找到对应域
 *          -2: 失败,没有找到对应字段
 *
 */
int IniConfig::GetVal(const string& fieldName, const string& key, string& value)
{
        string tFieldName = "[" + Trim(fieldName) + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        /* 没找到对应的域 */
        if (fieldIter == m_FiledSet.end()) {
                return -1;
        }

        /* 查找对应字段 */
        vector<KeyInfo>::iterator keyIter = (fieldIter->keySet).begin();
        keyIter = std::find_if(keyIter, (fieldIter->keySet).end(), KeyInfo(Trim(key)));
        /* 没找到该字段 */
        if (keyIter == (fieldIter->keySet).end()) {
                return -2;
        }

        /* 找到该字段 */
        value = keyIter->value;
        return 0;
}

/**
 * SetVal() - 设置指定字段信息
 * @fieldName: 准备添加的域字段的名字
 * @key: 字段名
 * @value: 该字段名对应的值
 * @return: 0: 成功
 *         -1: 失败，没有该域
 *
 */
int IniConfig::SetVal(const string& fieldName, const string& key, const string& value)
{
        if(Trim(fieldName).empty() || Trim(key).empty()){
                return -1;
        }

        if (fieldName == GLOBAL_FIELD) {
                AddField(Trim(fieldName));
        }

        string tFieldName = "[" + Trim(fieldName) + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        /* 没找到找到对应的域 */
        if (fieldIter == m_FiledSet.end()) {
                return -1;
        }
        /* 查找对应字段 */
        vector<KeyInfo>::iterator keyIter = (fieldIter->keySet).begin();
        keyIter = std::find_if(keyIter, (fieldIter->keySet).end(), KeyInfo(Trim(key)));
        /* 找到该字段 */
        if (keyIter != (fieldIter->keySet).end()) {
                if (value[0] != '#' && value[0] != ';') {
                        keyIter->value = Trim(value);
                }
        } else {
                KeyInfo ki(Trim(key), Trim(value));
                (fieldIter->keySet).push_back(ki);
        }
        return 0;
}

/**
 * DelKey() - 删除指定字段信息
 * @fieldName: 准备添加的域字段的名字
 * @key: 字段名
 * @return: 0: 成功
 *         -1: 失败，没有该域
 *
 */
void IniConfig::DelKey(const string& fieldName, const string& key)
{
        string tFieldName = "[" + Trim(fieldName) + "]";
        vector<FieldInfo>::iterator fieldIter = m_FiledSet.begin();
        fieldIter = std::find_if(fieldIter, m_FiledSet.end(), FieldInfo(tFieldName));

        /* 找到对应域 */
        if (fieldIter != m_FiledSet.end()) {
                vector<KeyInfo>::iterator keyIter = (fieldIter->keySet).begin();
                keyIter = std::find_if(keyIter, (fieldIter->keySet).end(), KeyInfo(Trim(key)));
                /* 找到对应字段 */
                if (keyIter != (fieldIter->keySet).end()) {
                        (fieldIter->keySet).erase(keyIter);
                }
        }
}

string IniConfig::Trim(const string& s, const string &letters)
{
    return ::Trim(s.c_str(), letters.c_str());
}

/**
 * ReadFromFile() - 读取配置文件
 * @return: 0: 成功
 *         -1: 失败, 打开配置文件失败
 *
 */
int IniConfig::ReadFromFile()
{
        FILE* file = fopen(m_ConfigPath.c_str(), "r");
        if (file == NULL) {
                return -1;
        }
        char buffer[BUF256];

        string tmpLine;
        vector<string> comment;
        size_t pos = string::npos;

        while (fgets(buffer, BUF256, file) != NULL) {
                tmpLine = Trim(buffer);
                pos = tmpLine.find("=");
                if ('#' == tmpLine[0] || ';' == tmpLine[0]) {
                        /* 注释信息 */
                        comment.push_back(tmpLine);
                } else if ('[' == tmpLine[0]) {
                        /* 域标示符 */
                        FieldInfo fi;
                        fi.fieldName    = tmpLine;
                        fi.fieldComment = comment;
                        m_FiledSet.push_back(fi);
                        comment.clear();
                } else if (pos != string::npos) {
                        /* 判断是否为全局字段信息 */
                        if (m_FiledSet.empty()) {
                                //这些字段信息为全局变量, *代表全局域
                                AddField(GLOBAL_FIELD);
                        }

                        /* 字段信息 */
                        KeyInfo ki;
                        ki.key   = Trim(tmpLine.substr(0, pos));
                        ki.value = Trim(tmpLine.substr(pos + 1));
                        ki.keyComment = comment;
                        /* 去掉value中的头和尾的" */
                        size_t size = ki.value.size();
                        if (ki.value[size-1] == '"') {
                                ki.value.erase(size-1);
                        }
                        if (ki.value[0] == '"') {
                                ki.value.erase(0, 1);
                        }

                        /* 添加到集合中 */
                        m_FiledSet[m_FiledSet.size()-1].keySet.push_back(ki);

                        pos = string::npos;
                        comment.clear();
                } else if (!tmpLine.empty()) {
                        m_FiledSet[m_FiledSet.size()-1].specialLine = tmpLine;
                }
        }
        int ret = fclose(file);
        if (ret < 0) {
                return -2;
        }
        return 0;
}

/**
 * WriteToFile() - 把内存中信息写入文件
 * @return: 0: 成功
 *         -1: 备份文件失败
 *         -2: 打开配置文件失败
 *         -3: 关闭文件句柄失败
 *         -4: 当修改配置文件失败后,恢复之前的配置文件失败
 *
 */
int IniConfig::WriteToFile()
{
        int ret = 0;

        char tConPath[BUF4K];
        sprintf(tConPath, "%s_%ld", m_ConfigPath.c_str(), syscall(SYS_gettid));
        /* 1. 如此该配置文件存在保存一份备份 */
        ret = access(m_ConfigPath.c_str(), F_OK);
        if (ret == 0) {
                ret = rename(m_ConfigPath.c_str(), tConPath);
                if (ret < 0) {
                        return -1;
                }
        }

        /* 2. 保存成功后用w+模式打开把内存的信息写入 */
        FILE* file = fopen(m_ConfigPath.c_str(), "w+");
        if (file == NULL) {
                return -2;
        }

        try {
                size_t writeSize = 0;
                vector<FieldInfo>::iterator fiIter = m_FiledSet.begin();
                for (; fiIter != m_FiledSet.end(); ++fiIter) {
                        /* 写域注释 */
                        for (size_t i=0; i<(fiIter->fieldComment).size(); ++i) {
                                writeSize = fprintf(file, "%s\n", (fiIter->fieldComment[i]).c_str());
                                if (writeSize != (fiIter->fieldComment[i]).size()+1) {
                                        throw "error";
                                }
                        }
                        /* 不是全局信息，写入域名信息 */
                        if (fiIter->fieldName != GLOBAL_FIELD_NAME) {
                                writeSize = fprintf(file, "%s\n", (fiIter->fieldName).c_str());
                                if (writeSize != (fiIter->fieldName).size()+1) {
                                        throw "error";
                                }
                                if (!fiIter->specialLine.empty()){
                                        writeSize = fprintf(file, "%s\n", (fiIter->specialLine).c_str());
                                        if (writeSize != (fiIter->specialLine).size()+1) {
                                                throw "error";
                                        }

                                }
                        }
                        /* 写入字段信息 */
                        for (size_t i=0; i<(fiIter->keySet).size(); ++i) {
                                /* 写字段注释 */
                                for (size_t j=0; j<(fiIter->keySet)[i].keyComment.size(); ++j) {
                                        writeSize = fprintf(file, "%s\n", 
                                                                (fiIter->keySet)[i].keyComment[j].c_str());
                                        if (writeSize != (fiIter->keySet)[i].keyComment[j].size()+1) {
                                                throw "error";
                                        }
                                }
                                /* 写具体字段信息 */
                                writeSize = fprintf(file, "%s=%s\n", fiIter->keySet[i].key.c_str(), 
                                                        fiIter->keySet[i].value.c_str());
                                size_t keySize = fiIter->keySet[i].key.size();
                                size_t valSize = fiIter->keySet[i].value.size();
                                if (writeSize != (keySize+valSize+2)) {
                                        throw "error";
                                }
                        }
                }
                ret = fclose(file);
                if (ret < 0) {
                        return -3;
                }
        } catch(...) {
                ret = -1;
        }

        /* 3. 保存配置文件成功后删除备份，失败后恢复备份 */
        if (access(tConPath, F_OK) == 0) {
                if (ret < 0) {
                        ret = rename(tConPath, m_ConfigPath.c_str()); 
                        if (ret < 0) {
                                return -4;
                        }
                } else {
                        /* 修改配置文件成功，只是删除失败，属于正常，不返回错误 */
                        remove(tConPath);
                }
        } else if (ret < 0) {
                /* 创建配置文件失败，把失败的文件删除 */
                remove(m_ConfigPath.c_str());
        }
        return 0;
}
