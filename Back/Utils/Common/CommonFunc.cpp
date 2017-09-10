#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include "CommonFunc.h"
#include "Utils/Shell/ShellOpr.h"
//#include "Include/Types.h"
//
#define BUF32 32
#define BUF64 64

/* 合法性判断 */
int IpLegal(const std::string& ip)
{
        return !(0xFFFFFFFF == inet_addr(ip.c_str()));
}

unsigned int MaskLegal(const string& mask)
{
        if (mask.empty()) {
                return 0;
        }

        int addr1, addr2, addr3, addr4;
        int dots = 0;
        unsigned int value = 0, param = 0;
        unsigned int i = 0;
        unsigned int length = 0;

        for (i=0; i<mask.size() && i<32; ++i) {
                if ('.' == mask[i]) {
                        ++dots;
                }
        }

        if (3 != dots) {
                return 0;
        }

        if (4 != (dots = sscanf(mask.c_str(), "%d.%d.%d.%d", 
                                &addr1, &addr2, &addr3, &addr4))) {
                return 0;
        }

        if (addr1>255 || addr2>255 || addr3>255 || addr4>255 ||
                         addr1<0 || addr2<0 || addr3<0 || addr4<0) {
                return 0;
        }

        value = addr1*0x1000000 + addr2*0x10000+ addr3*0x100 + addr4;

        for (i=0; ((value<<i) & 0x80000000) && i<=32; ++i);
        length = i>32 ? 32 : i;
        //LOG_DBG("i=[%d], value=[%X], value<<i=[%X]\n", i, value, value<<i);
        param = 0xFFFFFFFF<<(32-length);
        if (param != value) {
                return 0;
        }
        return length;
}

string MasklenToMask(const size_t& masklen)
{
        string str;
        if (masklen > 32 || masklen < 0) {
                return str;
        }
        size_t sum = 0xFFFFFFFF << (32-masklen);
        size_t mask1, mask2, mask3, mask4;
        mask1 = (sum >> 24) & 0xFF;
        mask2 = (sum >> 16) & 0xFF;
        mask3 = (sum >> 8)  & 0xFF;
        mask4 = (sum)       & 0xFF;
        char strMask[32] = {0};
        sprintf(strMask, "%ld.%ld.%ld.%ld", mask1, mask2, mask3, mask4);
        return strMask;
}

int IsAlphaNumStr(const char* ctest)
{
        const char* ctestchar = ctest;
        int illegalchar = 0;

        while (*ctestchar)
        {
                if (!isalnum(*ctestchar))
                {
                        illegalchar = 1;
                        break;
                }
                ++ctestchar;
        }
        return !illegalchar;
}

int NameLegal(const char* name)
{
        if (!name || !*name)
        {
                return 0;
        }
        const char* ctmp = name;
        int illegalchar = 0;
        if (!isalpha(*ctmp))
        {
                return 0;
        }
        for (; *ctmp; ++ctmp)
        {
                if (!(('a'<=*ctmp && 'z'<=*ctmp) ||
                      ('A'<=*ctmp && 'Z'<=*ctmp) ||
                      ('0'<=*ctmp && '9'<=*ctmp) ||
                       '-'!=*ctmp || '_'!=*ctmp))
                {
                        illegalchar = 1;
                        break;
                }
        }
        return !illegalchar;
}

/*
 ********************* 时间操作函数 ***********************************
*/
string GetCurDay ()
{
        //YYYY_MM_DD
        string date = GetCurMicroSec();
        const int len = strlen("yyyy_mm_dd");
        date = date.substr(0, len);
        return date;
}

time_t TimeStrToUtcSec(const string &timeStr)
{
        struct tm t;
        strptime(timeStr.c_str(), "%Y-%m-%d %H:%M:%S", &t);
        return mktime(&t);
}

string UtcSecToTimeStr(time_t t)
{
        char buffer[32] = {0};
        time_t sec = time(NULL);
        strftime(buffer, 32, "%Y-%m-%dT%H:%M:%S", localtime(&sec));
        return string(buffer);
}

string GetCurSec ()
{
        return UtcSecToTimeStr(time(NULL));
}


string GetCurMicroSec ()
{
        const int BufLen = 32;
        struct timeval timer;
        struct tm *tblock;
        char buffer[BufLen];

        memset(buffer, 0, BufLen);
        timerclear(&timer);

        //YYYY_MM_DD_HH_MM_SS_MS
        gettimeofday(&timer, NULL);
        tblock = localtime(&timer.tv_sec);
        strftime(buffer, BufLen, "%Y_%m_%d_%H_%M_%S", tblock);
        sprintf(buffer, "%s_%ld", buffer, timer.tv_usec);
        return string(buffer);
}

time_t GetUTCByString(const string& strTime)
{
        struct tm tmp;
        memset(&tmp, 0, sizeof(tmp));
        strptime(strTime.c_str(), "%Y%m%d", &tmp);
        return mktime(&tmp);
}


/*
 ******************** 字符串操作函数 **********************************
*/
void Trim(std::string& line, const std::string& letters)
{
        line.erase(0, line.find_first_not_of(letters.c_str()));
        line.erase(line.find_last_not_of(letters.c_str()) + 1);
}

std::string Trim(const char* str, const std::string& letters)
{
        string line = str;
        line.erase(0, line.find_first_not_of(letters.c_str()));
        line.erase(line.find_last_not_of(letters.c_str()) + 1);
        return line;
}

bool Split(const char* line, std::vector<std::string>& array, const char& dot, const char* trimletters, int isKeepEmpty)
{
        if (line == NULL || !*line){
                return false;
        }

        int                 p1  = 0;
        std::string         temp;

        for (int i = 0; ; i++)
        {
                if ( *(line + i) == dot ){
                        temp.assign( (char *)(line + p1), i - p1 );
                        Trim(temp, trimletters);
                        if (isKeepEmpty || !temp.empty())
                        {
                                array.push_back(temp);
                        }
                        p1 = i + 1;
                }
                else if ( *(line + i) == 0 ) {
                        if (i == p1){
                                break;
                        }
                        temp.assign( (char *)(line + p1), i - p1 );
                        Trim(temp, trimletters);
                        if (isKeepEmpty || !temp.empty())
                        {
                                array.push_back(temp);
                        }
                        break;
                }
        }
        return true;
}

void SplitFlag(const string& recv, const string& flag, vector<string>& recvList)
{
        string tmp = recv;

        size_t idx = 0;
        while ((idx=tmp.find_first_of(flag))!= string::npos) {
                string s = tmp.substr(0, idx);
                tmp      = tmp.substr(idx+1);
                recvList.push_back(s);
        }
        if (tmp.size() > 1) {
                recvList.push_back(tmp);
        }
}

std::string VectorToStr(const std::vector<std::string>& vs, const char& dot)
{
        string stmp;
        for (size_t i = 0; i<vs.size(); ++i)
        {
                stmp = stmp + dot + vs[i];
        }
        const char dots[4] = {dot, '\0'};
        Trim(stmp, dots);
        return stmp;
}

unsigned int FindStr(const std::vector<std::string>& vs, const std::string& str)
{
        unsigned int i = 0;
        for (i=0; i<vs.size(); ++i)
        {
                if (str == vs[i])
                {
                        return i;
                }
        }
        return NOT_FIND;
}

std::string StrUpper(const std::string strin)
{
        std::string str;
        const char* input = strin.c_str();
        while (*input)
        {
                str += (char)toupper(*input);
                ++input;
        }
        return str;
}

std::string StrLower(const std::string strin)
{
        std::string str;
        const char* input = strin.c_str();
        while (*input)
        {
                str += (char)tolower(*input);
                ++input;
        }
        return str;
}


char * s_printf(const char *fmt, ...)
{
        /* Guess we need no more than 100 bytes. */
        int n, size = 1024;
        char *p, *np;
        va_list ap;

        p = (char *)malloc (size);
        if ( p == NULL) {
                return NULL;
        }

        while (1) {
                /* Try to print in the allocated space. */
                va_start(ap, fmt);
                n = vsnprintf (p, size, fmt, ap);
                va_end(ap);
                /* If that worked, return the string. */
                if (n > -1 && n < size)
                        return p;
                /* Else try again with more space. */
                if (n > -1)    /* glibc 2.1 */
                        size = n+1; /* precisely what is needed */
                else           /* glibc 2.0 */
                        size *= 2;  /* twice the old size */
                if ((np = (char *)realloc (p, size)) == NULL) {
                        free(p);
                        return NULL;
                } else {
                        p = np;
                }
        }
}

std::string MakeSect(const char* path, const char* lsqr, const char* rsqr)
{
        string sect;
        const char* stmp = path;
        if (0 == memcmp("/mnt/share/", path, 11))
        {
                stmp += 11;
        }

        if (!*stmp)
        {
                return sect;
        }

        sect = StrReplace(stmp);
        sect = string(lsqr) + sect + rsqr;
        return sect;
}

std::string StrReplace(const std::string& str, const char& oldch, const char& newch)
{
        string stmp = str;
        for (uint i=0; i<stmp.length(); ++i)
        {
                oldch == stmp[i] ? stmp[i] = newch : 0;
        }
        return stmp;
}

/*
 ******************** 目录操作函数 ************************************
*/
string GetParPath (const string& path)
{
        size_t idx = 0;
        string ParPath;
        string tpath = Trim(path.c_str());
        if('/' == path[path.length()-1]){
                tpath.erase(path.length()-1);
        }

        idx = tpath.find_last_of("/");

        if(idx == string::npos)
              return path;

        ParPath = tpath.substr(0, idx + 1);

        return ParPath;
}

bool HasPath (const string& path)
{
        int ret = 0;
        ret = access(path.c_str(), F_OK);
        if (ret != 0) {
                return false;
        }
        return true;
}

int MakeDir(const string& path)
{
        int ret = 0;
        if(path[0] != '/') {
              return -1;
        }

        string sub;
        string str = path;
        size_t pos = str.find("/", 1);

        while(!HasPath(str)) {
                if(pos != string::npos) {
                        sub = str.substr(0, pos);
                } else {
                        ret = mkdir(str.c_str(), 0755);
                        if(ret < 0) {
                              return -2;
                        }
                }
                if (!HasPath(sub)) {     
                        ret = mkdir(sub.c_str(), 0755);
                        if(ret < 0) {
                              return -3;
                        }
                }
                pos = str.find("/", pos+1);
        }
        return 0;
}

bool IsDir(const string& path)
{
        bool bret = false;
        struct stat st;
        if (lstat(path.c_str(), &st) < 0) {
                bret = false;
        }
        if (S_ISDIR(st.st_mode)) {
                bret = true;
        }
        return bret;
}

bool IsBlockDevice(const string& path)
{
        bool bret = false;
        struct stat st;
        if (lstat(path.c_str(), &st) < 0) {
                bret = false;
        }

        if (S_ISBLK(st.st_mode)) {
                bret = true;
        }
        return bret;
}

bool IsDirEmpty (const string& path)
{
        int cnt = 0;
        DIR* dir = NULL;
        dir = opendir(path.c_str());
        if (dir == NULL) {
                return false;
        }

        bool bret = false;
        while (readdir(dir) != NULL) {
                ++cnt;
                if (cnt > 2) {
                        bret = false;
                        break;
                }
        }
        closedir(dir);
        return bret;
}

void AppendBias (string& path)
{
        if ( '/' != path[path.size()-1] ) {
                path.append("/");
        }
}


int64_t UserToDigit(const string &str)
{
        size_t len = str.size();
        const string unit = "KMGTPEZY";
        char lastCh = toupper(str[len - 1]);
        char lastCh2 = toupper(str[len - 2]);

        if (toupper(lastCh) == 'B') {
                size_t xfer = 1L;
                switch (lastCh2) {
                case 'K':
                        xfer = 1L << 10;
                        break;
                case 'M':
                        xfer = 1L << 20;
                        break;
                case 'G':
                        xfer = 1L << 30;
                        break;
                case 'T':
                        xfer = 1L << 40;
                        break;
                case 'P':
                        xfer = 1L << 50;
                        break;
                case 'E':
                        xfer = 1L << 60;
                        break;
                default:
                        // 字节，直接获取整数位就可以
                        return atoll(str.substr(0, len - 1).c_str());
                }
                return atof(str.substr(0, len - 2).c_str()) * xfer;
        } else {
                return 0;
        }
}

string IntToString(int64_t n)
{
        char buf[BUF64] = {0};
        sprintf(buf, "%ld", (int64_t)n);
        return string(buf);
}

string DoubleToString(double n)
{
        char buf[BUF64] = {0};
        sprintf(buf, "%f", n);
        return string(buf);
}

string TimeToString(time_t t)
{
        struct tm tm1; 

        char buf[BUF32] = {0};
        localtime_r(&t, &tm1 ); 
        sprintf(buf, "%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d",                      
                        tm1.tm_year+1900, tm1.tm_mon+1, tm1.tm_mday, 
                        tm1.tm_hour, tm1.tm_min,tm1.tm_sec); 

        return string(buf);
}

time_t StringToTime(const string strTime)
{
        struct tm tm1;  
        time_t t;  

        sscanf(strTime.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &tm1.tm_year, 
                                &tm1.tm_mon,  &tm1.tm_mday,  &tm1.tm_hour,  
                                &tm1.tm_min,  &tm1.tm_sec);               
        tm1.tm_year -= 1900;  
        tm1.tm_mon--;  
        tm1.tm_isdst=-1;    
        t = mktime(&tm1);  

        return t;  
}

int Umount(const string& mp)
{
        string cmd = "umount " + mp;
        vector<string> errInfo;
        return Execute(cmd, errInfo);
}

int Mount(const string& dev, const string& mp)
{
        string cmd = "mount " + dev + " " + mp;
        vector<string> errInfo;
        return Execute(cmd, errInfo);
}

int GetPartitionFromDir(const string& path, string& partition)
{
        int64_t totalSize = 0;
        int64_t leftSize  = 0;
        return GetDeviceUsageFromDir(path, partition, totalSize, leftSize);
}

int GetLeftSpaceByMp(const string& mp, int64_t& totalSpace, int64_t& leftSpace)
{
        string partition;
        return GetDeviceUsageFromDir(mp, partition, totalSpace, leftSpace);
}

int GetDeviceUsageFromDir(string dir, string& part,
                                int64_t& totalSize, int64_t& leftSize)
{
        string tDir = "'" + dir + "'";
        string cmd = "df " + tDir + "|sed '1d;:a;N;s/\\n//;ba'";

        int64_t total  = 0;
        int64_t left   = 0;
        char fs[4096]  = {0};

        vector<string> retSet;
        int ret = Execute(cmd, retSet);
        if (ret < 0) {
                return -1;
        }

        if (!retSet.empty()) {
                sscanf(retSet[0].c_str(), "%s %ld %*d %ld %*s %*s", fs, &total, &left); 
        }

        part      = fs;
        totalSize = total * 1024;
        leftSize  = left  * 1024;

        return 0;
}
