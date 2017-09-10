#ifndef _COMMON_FUNC_H
#define _COMMON_FUNC_H

#include <string>
#include <vector>
#include <stdint.h>

#include "Include/Types.h"
#define NOT_FIND    0xFFFFFFFF
using std::string;
using std::vector;

/**
 * 常用函数接口，包含对时间，字符串和目录的处理
*/

/* 合法性判断 */

unsigned int MaskLegal(const std::string& mask);
int IpLegal(const std::string& ip);

/* 将掩码长度转换为掩码字符串 */
string MasklenToMask(const size_t& masklen);

//判断是否为 字母和数字 组成的字符串 是返回1
int IsAlphaNumStr(const char* ctest);

//名字合法性 只能是字母，数字，下划线，减号，而且字母开头 是返回1
int NameLegal(const char* name);

/********************** 时间操作函数 ************************************/

//返回当前时间，格式：YYYY_MM_DD
string GetCurDay ();

// 传入UTC秒 ，传出YYYY_MM_DDTHH_MI_SS 格式的字符串
string UtcSecToTimeStr(time_t t);

// 传入 YYYY_MM_DD HH_MI_SS 格式的字符串，传出UTC秒
time_t TimeStrToUtcSec(const string &timeStr);

//返回当前时间，格式：YYYY_MM_DDTHH_MI_SS
string GetCurSec();

//返回当前时间，格式：YYYY_MM_DD _HH_MI_SS_MS
string GetCurMicroSec ();

//将传入的时间字符串转换为utc时间 输入时间格式YYYYMMDD
time_t GetUTCByString (const string& strTime);

/********************* 字符串操作函数 ***********************************/

//返回去除字符串的空格和换行符
void Trim(std::string& line, const std::string& letters = " \t\r\n");
std::string Trim(const char* str, const std::string& letters = " \t\r\n");

//      将字符串分割为字符 参数(源字符串,数组容器,分隔符,过滤字符,是否保留空行)
bool Split(const char* line, std::vector<std::string>& array, const char& dot = ' ', const char* trimletters = " \t\r\n", int isKeepEmpty = 1);

//      在容器中增加删除成员
template <class T>
int DelMem(std::vector<T>& vmain, const std::vector<T>& vsub)
{
        unsigned int i = 0, j = 0;
        for (i=0; i<vsub.size(); ++i)
        {
                for (j=0; j<vmain.size();)
                {
                        if (vmain[j] == vsub[i])
                        {
                                vmain.erase(vmain.begin() + j);
                                continue;
                        }
                        ++j;
                }
        }
        return 0;
}

template <class T>
int AddMem(std::vector<T>& vmain, const std::vector<T>& vsub)
{
        unsigned int i = 0, j = 0;
        for (i=0; i<vsub.size(); ++i)
        {
                int name_exist = 0;
                for (j=0; j<vmain.size(); ++j)
                {
                        if (vmain[j] == vsub[i])
                        {
                                name_exist = 1;
                                break;
                        }
                }
                if (!name_exist)
                {
                        vmain.push_back(vsub[i]);
                }
        }
        return 0;
}

template <class T>
unsigned int FindMem(const std::vector<T>& vs, const T& t)
{
        unsigned int i = 0;
        for (i=0; i<vs.size(); ++i)
        {
                if (t == vs[i])
                {
                        return i;
                }
        }
        return NOT_FIND;
}

// 以指定分隔符分隔字符串，分隔后的字符串放入vector中
void SplitFlag(const string& recv, const string& flag, vector<string>& recvList);

//      组合数组为字符串
std::string VectorToStr(const std::vector<std::string>& vs, const char& dot = ' ');

//      在vector<string>查找字符串
unsigned int FindStr(const std::vector<std::string>& vs, const std::string& str);

//      转换大小写
std::string StrUpper(const std::string);
std::string StrLower(const std::string);


// 可以自动扩容的 sprintf(), 返回值就是生成的字符串数组， 需要手动释放 free()
char * s_printf(const char *fmt, ...);

std::string MakeSect(const char* path, const char* lsqr = "[", const char* rsqr = "]");
std::string StrReplace(const std::string& str, const char& oldch = '/', const char& newch = '.');
/********************* 目录操作函数 *************************************/

//得到传入目录的父目录
string GetParPath (const string& path);

//判断这个路径是否存在
bool HasPath (const string& path);

//创建目录
int MakeDir(const string& path);

//判断是否是目录
bool IsDir(const string& path);

//判断是否时块设备
bool IsBlockDevice(const string& path);

//判断目录是否为空
bool IsDirEmpty (const string& path);

//路径添加/
void AppendBias (string& path);

/* 带单位数值 -> 整数， 支持的单位：B/KB/MB/GB/TB/PB/EB/ZB/YB
 * exp: 100Mb -> 107374182400 */
int64_t UserToDigit(const string &str);

/* 整数 -> 字符串 */
string IntToString(int64_t n);

/* 浮点数 -> 字符串 */
string DoubleToString(double n);

/* UTC时间 -> YYYY-MM-DD hh:mm:ss 字符串 */
string TimeToString(time_t t);

/* YYYY-MM-DD hh:mm:ss字符串 -> UTC时间 */
time_t StringToTime(const string strTime);

/********************** 系统设备挂载卸载操作 ************************************/
int Umount(const string& mp);
int Mount(const string& dev, const string& mp);

/********************** 磁盘相关操作 ************************************/
int GetPartitionFromDir(const string& path, string& partition);
int GetLeftSpaceByMp(const string& mp, int64_t& totalSpace, int64_t& leftSpace);
int GetDeviceUsageFromDir(string dir, string& par,int64_t& totalSize, int64_t& leftSize);

#endif //_COMMON_FUNC_H
