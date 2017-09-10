#include <stdio.h>
//#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "Utils/Log/Log.h"
#include "Utils/Shell/ShellOpr.h"
#include "Utils/Common/DirFileOpr.h"

using std::ifstream;

DirFileOpr::DirFileOpr()
{
}

DirFileOpr::~DirFileOpr()
{
}

int  DirFileOpr::GetDirSet(const string& path, set<string>& dirSet)
{
        int ret = 0;
        set<string> fileSet;
        ret = TravelDir(path, dirSet, fileSet);
        if (ret < 0) {
                LOG_ERR("TravelDir Error!\n");
        }
        return ret;
}

int  DirFileOpr::GetFileSet(const string& path, set<string>& fileSet)
{
        int ret = 0;
        set<string> dirSet;
        ret = TravelDir(path, dirSet, fileSet);
        if (ret < 0) {
                LOG_ERR("TravelDir Error!\n");
        }
        return ret;
}

string DirFileOpr::GetFileNameByFullPath(const string& path)
{
        string sub;
        size_t idx = path.find_last_of("/");

        if (idx != string::npos) {
                sub = path.substr(idx+1);
        } else {
                sub = path;
        }
        return sub;
}

void DirFileOpr::GetParPath(const string& path, string& parPath)
{
        int idx = 0;
        string tpath = path;
        EraseLastBias(tpath);
        
        idx     = tpath.find_last_of("/");
        parPath = tpath.substr(0, idx);
}

void DirFileOpr::GetDestPath(const string& refSrcPath, const string& refDestPath, 
                 const string& srcPath, string& destFullPath)
{

        destFullPath = refDestPath;

        if ( '/' != refDestPath[refDestPath.size()-1] ) { 
                destFullPath.append("/"); 
        }

        string strTmp;

        int idx = srcPath.find(refSrcPath) + refSrcPath.size();
        strTmp = srcPath.substr(idx);
        destFullPath += strTmp;
}

int  DirFileOpr::GetPathSize(string path, int64_t& size)
{
        int ret = 0;

        string cmdLine = "du -sb ";
        cmdLine.append("\'");
        cmdLine.append(path);
        cmdLine.append("\'");

        char buffer[255];
        FILE* file = popen(cmdLine.c_str(), "r");
        if (file == NULL) {
                LOG_ERR("popen error! cmd=%s ,strerror=%s\n", cmdLine.c_str(), strerror(errno));
                return -1;
        }

        char* pathSizeStr = fgets(buffer, 255, file);
        string line;
        if (pathSizeStr != NULL) {
                line = pathSizeStr;
                size_t idx = line.find_first_of(" ");
                if (idx != string::npos) {
                        size = atoll(line.substr(0, idx).c_str());
                } else {
                        size = atoll(line.c_str());
                }
        }
        ret = pclose(file);
        if (ret < 0) {
                LOG_ERR("pclose error! path=%s\n", path.c_str());
        }
        return ret;
}

bool DirFileOpr::HasPath(const string& path)
{
        int ret = 0;
        ret = access(path.c_str(), F_OK);
        if (ret != 0) {
                return false;
        }
        return true;
}

int  DirFileOpr::GetFileMTime(const string& path, int64_t& sec)
{
        int ret = 0;
        struct stat statbuf; 
        ret = lstat(path.c_str(), &statbuf);
        if (ret < 0) {
                LOG_ERR("lstat error! path=%s ,strerror=%s\n", path.c_str() ,strerror(errno));
        }
        sec = statbuf.st_mtime;
        return ret;
}

int  DirFileOpr::MakeDir(const string& path)
{
        int ret = 0;
        string tPath = "'" + path + "'";
        string cmd   = "mkdir -p " + tPath;

        vector<string> errInfo;
        ret = Execute(cmd, errInfo);
        if (ret < 0) {
                LOG_ERR("ExecuteCmd Error! cmd=%s\n", cmd.c_str());
        }
        return ret;
}


bool DirFileOpr::IsDir(const string& path)
{
        bool ret = false;
        struct stat st;
        if (lstat(path.c_str(), &st) < 0) {
                LOG_ERR("lstat error! path=%s, errstr=%s\n", path.c_str(), strerror(errno));
                ret = false;
        }
        if (S_ISDIR(st.st_mode)) {
                ret = true;
        }
        return ret;
}

bool DirFileOpr::IsLink(const string& path)
{
        bool ret = false;
        struct stat st;
        if (lstat(path.c_str(), &st) < 0) {
                LOG_ERR("lstat error! path=%s, errstr=%s\n", path.c_str(), strerror(errno));
                ret = false;
        }

        if (S_ISLNK(st.st_mode)) {
                ret = true;
        }
        return ret;
}

bool DirFileOpr::IsBlockDevice(const string& path)
{
        bool ret = false;
        struct stat st;
        if (lstat(path.c_str(), &st) < 0) {
                LOG_ERR("lstat error! path=%s, errstr=%s\n", path.c_str(), strerror(errno));
                ret = false;
        }

        if (S_ISBLK(st.st_mode)) {
                ret = true;
        }
        return ret;
}

bool DirFileOpr::IsDirEmpty(const string& path)
{
        int cnt = 0;
        DIR* dir = NULL;
        dir = opendir(path.c_str());
        if (dir == NULL) {
                LOG_ERR("opendir error! path=%s, errstr=%s\n", path.c_str(), strerror(errno));
                return false;
        }

        while (readdir(dir) != NULL) {
                ++cnt;
                if (cnt > 2) {
                        closedir(dir);
                        return false;
                }
        }

        closedir(dir);
        return true;
}

void DirFileOpr::AppendBias(string& path)
{
        if ( '/' != path[path.size()-1] ) { 
                path.append("/");
        }
}

void DirFileOpr::EraseLastBias(string& path)
{
        int idx = path.size() - 1;
        if ( '/' == path[idx] ) { 
                path.erase(idx);
        }
}

//private
int DirFileOpr::TravelDir(const string& path, set<string>& dirSet, set<string>& fileSet)
{
        int ret = 0;
        string tpath = path;

        AppendBias(tpath);

        DIR* dp;
        struct dirent* dir;
        struct stat statbuf;  

        dp = opendir(path.c_str());
        if (dp == NULL) {
                LOG_ERR("opendir error! path=%s, errstr=%s\n", path.c_str(), strerror(errno));
                return -1;
        }

        string pt;
        while ( (dir=readdir(dp)) != NULL) {
                pt = tpath; 
                pt.append(dir->d_name);
                ret = lstat(pt.c_str(), &statbuf);
                if (ret < 0) {
                        LOG_ERR("lstat error! path=%s, errstr=%s\n", pt.c_str(), strerror(errno));
                        continue;
                }

                if (S_ISDIR(statbuf.st_mode)) {
                        if (strcmp(".", dir->d_name)==0 || strcmp("..", dir->d_name)==0) {
                                continue;
                        }
                        dirSet.insert(pt);
                } else if (S_ISREG(statbuf.st_mode)) {
                        fileSet.insert(pt);
                }
        }

        ret = closedir(dp);
        if (ret < 0) {
                LOG_ERR("closedir error!");
        }
        return ret;
}

string DirFileOpr::RebulidPath(const string& srcFullPath, const string& srcPath, const string& destPath)
{
        int idx = srcFullPath.find(srcPath);
        string sub = srcFullPath.substr(idx+srcPath.size());
        string tmp = destPath;
        AppendBias(tmp);
        sub = tmp + sub;
        return sub;
}

bool DirFileOpr::IsMaxRecord(const string& path)
{
        //获取每个文件记录条数
        /*
        IniParser iniOpr("/etc/scigw/default/GWconfig");
        int ret = iniOpr.Init();
        if (ret < 0) {
                LOG_ERR("Init Error!");
                return false;
        }

        string tmpRecord;
        ret = iniOpr.GetVal("GatewayArchive", "mvMaxRecord", tmpRecord);
        if (ret < 0) {
                LOG_ERR("GetVal ERROR! ret=" << ret);
                return false;
        }
        int maxRecord = atoi(tmpRecord.c_str());
        */
        int maxRecord = 100;

        string cmd = "wc ";
        cmd += path;
        FILE *stream = popen(cmd.c_str(), "r");
        if (NULL == stream) {
                LOG_ERR("excute cmd:wc error! cmd=%s, strerror=%s\n", cmd.c_str(), strerror(errno));
                return false;
        }

        const int bufLen = 32;
        char buf[bufLen];
        memset(buf, 0, bufLen);
        fgets(buf, bufLen, stream);

        int rcdNum = 0;
        sscanf(buf, "%d", &rcdNum);

        pclose(stream);
        return (rcdNum >= maxRecord)? true: false;
}

int DirFileOpr::CheckPath(const string& path, const string& type, string& errInfo)
{
        if (type == "r") {
                return IsDirReadable(path, errInfo)? 0: -1; 
        } else if (type == "w") {
                return IsDirWritable(path, errInfo)? 0: -1;
        }
        return 0;
}

bool DirFileOpr::IsDirReadable(const string& path, string& errInfo)
{
        FILE* file = fopen(path.c_str(), "r");
        if (file == NULL) {
                errInfo = strerror(errno);
                LOG_INF("fopen error! path=%s, errInfo=%s\n", path.c_str(), errInfo.c_str()); 
                return false;
        }

        char buffer[128];
        int ret = fread(buffer, 128, 1, file);
        if (ret < 0) {
                errInfo = strerror(errno);
                LOG_INF("can't read! path=%s, errInfo=%s\n", path.c_str(), errInfo.c_str());
                fclose(file);
                return false;
        }
        fclose(file);
        return true;
}

bool DirFileOpr::IsDirWritable(const string& path, string& errInfo)
{
        char buffer[10] = {0}, tmpName[1024] = {0};
        sprintf(tmpName, "%s/%s", path.c_str(), "XXXXXX");

        int fd = mkstemp(tmpName);
        if (fd < 0) {
                errInfo = strerror(errno);
                LOG_ERR("mkstemp error! errInfo=%s\n", errInfo.c_str());
                return false;
        }
        if (write(fd, buffer, 10) < 0) {
                errInfo = strerror(errno);
                LOG_INF("can,t write! errInfo=%s\n", errInfo.c_str()); 
                close(fd);
                return false;
        }
        close(fd);

        if (remove(tmpName) < 0) {
                LOG_ERR("remove error! errInfo=%s\n", strerror(errno));
        }
        return true;
}

int  DirFileOpr::GetAllFileSet(const string& path, set<string>& fileSet)
{
        int ret = 0;
        set<string> pathList;
        ret = TravelDir(path, pathList, fileSet);
        if (ret < 0) {
                LOG_ERR("TravelDir Error! ret=%d\n", ret);
                return ret;
        }
        if (pathList.size() == 0) {
                return 0;
        }

        set<string>::iterator lsIter = pathList.begin();
        for (; lsIter != pathList.end(); ++lsIter) {
                ret = GetAllFileSet(*lsIter, fileSet);
                if (ret < 0) {
                        LOG_ERR("FindAllFile Error! ret=%d\n", ret);
                        return ret;
                }
        }
        return 0;
}

int DirFileOpr::DeleteEmptyDir(const string& path)
{
        int ret = 0;
        set<string> pathSet;
        set<string> fileSet;
        ret = TravelDir(path, pathSet, fileSet);
        if (ret < 0) {
                LOG_ERR("TravelDir Error! ret=%d\n", ret);
                return ret;
        }

        vector<string> errInfo;
        set<string>::iterator lsIter = pathSet.begin();
        for (; lsIter != pathSet.end(); ++lsIter) {
                //判断目录是否不包含文件，没有文件删除
                if (!HaveFile(*lsIter)) {
                        string cmd = "rm -rf " + *lsIter;
                        ret = Execute(cmd, errInfo);
                        if (ret < 0) {
                                LOG_ERR("ExecuteCmd Error! cmd=%s\n", cmd.c_str());
                        }
                        continue;
                }
                ret = DeleteEmptyDir(*lsIter);
                if (ret < 0) {
                        LOG_ERR("FindAllFile Error! ret=%s\n", ret);
                        return ret;
                }
        }
        return 0;
}

bool DirFileOpr::HaveFile(const string& path)
{
        int ret = 0;

        string cmd = "find " + path + " -type f | wc -l";
        vector<string> fileCntVec;
        ret = Execute(cmd, fileCntVec); 
        if (ret < 0) {
                LOG_ERR("Popen error! cmd=%s\n", cmd.c_str());
                return false;
        }
        
        int fileCnt = 0;
        if (!fileCntVec.empty()) {
                fileCnt = atoi(fileCntVec[0].c_str());
        }
        return (fileCnt==0)?false:true;
}

int DirFileOpr::GetFileSize(const string& path, int64_t& size)
{
        int ret = 0;
        struct stat statbuf; 
        ret = lstat(path.c_str(), &statbuf);
        if (ret < 0) {
                LOG_ERR("lstat error! path=%s, strerror=%s\n", path.c_str(), strerror(errno));
        }
        size = statbuf.st_size;
        return ret;
}

bool DirFileOpr::IsUsing(const string& path)
{
        bool bret = false;

        string cmd = "lsof '" + path + "' | wc -l";
        vector<string> retVec;
        int ret = Execute(cmd, retVec);
        if (ret < 0) {
                LOG_ERR("Popen Error! cmd=%s\n", cmd.c_str());
                return bret;
        }
        if (retVec[0] != "0") {
                bret = true;
        }
        return bret;
}

bool DirFileOpr::IsChanged(const string& src, const string& dest)
{
        int ret = 0;
        int bret = false;

        //获取源文件修改时间
        int64_t srcMTime, srcFileSize;
        ret = GetFileMTime(src, srcMTime);
        if (ret < 0) {
                LOG_ERR("GetFileMTime Error! src=%s\n", src.c_str());
                return bret;
        }
        ret = GetFileSize(src, srcFileSize);
        if (ret < 0) {
                LOG_ERR("GetFileSize Error! src=%s\n", src.c_str());
                return bret;
        }

        //获取目标文件修改时间
        int64_t destMTime, destFileSize;
        ret = GetFileMTime(dest, destMTime);
        if (ret < 0) {
                LOG_ERR("GetFileMTime Error! dest=%s\n", dest.c_str());
                return bret;
        }
        ret = GetFileSize(dest, destFileSize);
        if (ret < 0) {
                LOG_ERR("GetFileSize Error! src=%s\n", dest.c_str());
                return bret;
        }

        //查看是否修改
        if ( (srcMTime != destMTime) || (srcFileSize != destFileSize) ) {
                bret = true;
        }
        return bret;
}

bool DirFileOpr::IsMounted(const string& dev, const string& path)
{
        int  ret  = 0;
        bool bret = false;

        //string cmd = "mount | grep " + dev + " | grep " + path;
        string cmd = "mount | awk -v dev=" + dev + " -v mp=" + path + 
                " '{if($1==dev && $3==mp) {print $1\" \"$2\" \"$3}}'";
        vector<string> recVec;
        ret = Execute(cmd, recVec);
        if (ret < 0) {
                LOG_ERR("popen error! ret=%d\n", ret);
                return false;
        }
        if (recVec.size() > 0) {
                bret = true;
        }
        return bret;
}
