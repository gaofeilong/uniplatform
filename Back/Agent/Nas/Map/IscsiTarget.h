#ifndef __ISCSITARGET_H__
#define __ISCSITARGET_H__

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#define ISCSITARGET_HEAD_DEF    1
#define ISCSITARGET_DATA_DEF    1
#define ISCSITARGET_INIT_DEF    1
#define ISCSITARGET_IMME_DEF    1

typedef struct stLunMap
{
        std::string     targetIQN;
        uint            LunNum;
        std::string     path;
        int             type;
        int             iomode;
        std::string     ScsiId;
        std::string     ScsiSN;
        int             blockSize;
        stLunMap() {}
        stLunMap(std::string p, std::string t = "")
            :targetIQN(t), path(p), type(0), iomode(0), blockSize(512) {}
        bool operator==(const stLunMap& lm) const
        {
                return path == lm.path;
        }
} stLunMap;

typedef struct stIscsiTarget
{
        std::string                         targetIQN;
        std::vector<std::string>            vcmt;
        int                                 HeaderDigest;
        int                                 DataDigest;
        int                                 InitialR2T;
        int                                 ImmediateData;
        uint                                lastLunNum;
        std::map<std::string, std::string>  mIncomingUser;
        std::map<std::string, std::string>  mOutgoingUser;
        std::vector<stLunMap>               vLunMap;
        stIscsiTarget()
            :HeaderDigest(ISCSITARGET_HEAD_DEF), DataDigest(ISCSITARGET_DATA_DEF),
             InitialR2T(ISCSITARGET_INIT_DEF), ImmediateData(ISCSITARGET_IMME_DEF) {}
        stIscsiTarget(std::string t)
            :targetIQN(t),
             HeaderDigest(ISCSITARGET_HEAD_DEF), DataDigest(ISCSITARGET_DATA_DEF),
             InitialR2T(ISCSITARGET_INIT_DEF), ImmediateData(ISCSITARGET_IMME_DEF) {}
        uint GetNextLun() const;
} stIscsiTarget;

typedef struct stChapUser
{
        char    username[256];
        char    password[32];
//      int             usertype;
} stChapUser;

typedef struct stIscsiHost
{
        std::string ip;
        std::string mask;
} stIscsiHost;

class CIscsiTarget
{
public:
        CIscsiTarget();
        ~CIscsiTarget();

// Target
        int AddIscsiTarget(const std::string& targetIQN, const stIscsiTarget* pit = NULL);
        int DelIscsiTarget(const std::string& targetIQN);
        int GetAllTargetIQN(std::vector<std::string>& vtarget);
        int GetIscsiTarget(const std::string& targetIQN, stIscsiTarget& stti);

// Lun Map
        int GetMap(const std::string& targetIQN, std::vector<stLunMap>& vlm);
        int GetUnmapLun(std::vector<std::string>& vlm) const;

        int MapIscsi(const std::string& targetIQN, const stLunMap& stlm);
        // 替换已经map的lun
        int MapIscsi(const std::string& targetIQN, const std::vector<stLunMap>& vlm);
        int UnmapIscsi(const std::string& targetIQN, const std::string& path);

// CHAP User
        int AddChapUser(const std::string& targetIQN, const std::string& username, const std::string& passwd, const int& usertype);
        int DelChapUser(const std::string& targetIQN, const std::string& username, const int& usertype);
        int GetChapUserList(const std::string& targetIQN, std::map<std::string, std::string>& vu, const int& usertype);

// File
        // 加载配置, 上面的接口使用之前必须先调用 Load()
        int Load();
        int Write() const;
        int Clear();
        int LoadIn() const;

// Other Require
        int GetPathInfo(const std::string& path, stLunMap& stlm) const;

        int GetAllowHosts(const std::string targetIQN, std::vector<std::string>& va, int&isAllowAll) const;
        int SetAllowHosts(const std::string targetIQN, const std::vector<std::string>& va, int isAllowAll = 0) const;
        int ReadIscsiHosts(const char* buf, std::vector<stIscsiHost>& netHosts);
        int DelTargetFromAllowHosts(const char*targetIQN) const;
        int SetAllowAll(std::string targetIQN, int) const;
private:
        uint FindTarget(const std::string& targetIQN) const;
        uint FindLun(uint nTar, const std::string& lv) const;
        std::map<std::string, std::string>::const_iterator FindUser(uint nTar, const std::string& user, const int& usertype) const;

        std::string                 m_filepath;
        std::vector<stIscsiTarget>  m_vis;
        int                         m_ready;
//      std::string                 m_iSNSServer;
//      std::vector<std::string>    m_vcmt;

};

#endif

