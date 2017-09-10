#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Nfs.h"
#include "Utils/Shell/System.h"
#include "Utils/Common/String.h"
#include "Include/Constant.h"
#include "Utils/Log/Log.h"
#include "Utils/Common/CommonFunc.h"
#include <string.h>

#define STR_NOROOTSQUASH    "no_root_squash"
#define STR_ROOTSQUASH      "root_squash"
#define STR_ALLSQUASH       "all_squash"
#define STR_ANONUID         "anonuid"
#define STR_ANONGID         "anongid"
using namespace std;

CNfs::CNfs()
        :m_filepath(NFSCONF), m_ready(0)
{
}

CNfs::~CNfs()
{
        m_vnd.clear();
}

int CNfs::AddDir(const char* path, uint perm)
{
        if (!path || !*path)
        {
                LOG_ERR("AddDir(): empty path!");
                return RET_FAILED;
        }
        if (RET_FAILED == Load())
        {
                LOG_ERR("AddDir(): load conf failed!");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;
        stNfsDir newnd;
        newnd.path = path;
        Trim(newnd.path);

        for (i=0; i<m_vnd.size(); ++i)
        {
                if (m_vnd[i].path == newnd.path)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (name_exist)
        {
                LOG_WRN("AddDir(): path already exist! path=%s", path);
                return RET_SUCCESS;
        }

        m_vnd.push_back(newnd);

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("AddDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("AddDir(): success! path=%s", path);
        return RET_SUCCESS;
}

int CNfs::DelDir(const char* path)
{
        if (!path || !*path)
        {
                LOG_ERR("DelDir(): empty path!");
                return RET_FAILED;
        }
        if (RET_FAILED == Load())
        {
                LOG_ERR("DelDir(): load conf failed!");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vnd.size(); ++i)
        {
                if (m_vnd[i].path == path)
                {
                        name_exist = 1;
                        m_vnd.erase(m_vnd.begin() + i);
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_WRN("DelDir(): cannot find path! path=%s", path);
                return RET_SUCCESS;
        }

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("DelDir(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("DelDir(): success! path=%s", path);
        return RET_SUCCESS;
}

int CNfs::GetDir(const char* path, uint& perm)
{
        return RET_SUCCESS;
}

int CNfs::SetDir(const char* path, uint perm)
{
        return RET_SUCCESS;
}

int CNfs::GetUserAccess(const char* path, std::vector<std::string>& va)
{
        return RET_SUCCESS;
}

int CNfs::SetUserAccess(const char* path, const std::vector<std::string>& va)
{
        return RET_SUCCESS;
}

int CNfs::GetIpAccess(const char* path, std::vector<std::string>& va)
{
        va.clear();
        m_perm = SA_NO;
        uint i = 0;
        vector<stNfsIp> vni;
        if (RET_SUCCESS != GetIpAccess(path, vni))
        {
                return RET_FAILED;
        }
        char cmasklen[32] = {0};
        for (i=0; i<vni.size(); ++i)
        {
                sprintf(cmasklen, "/%u", vni[i].nai.masklen);
                va.push_back(vni[i].nai.ip + cmasklen);
                m_perm = m_perm > vni[i].perm ? m_perm : vni[i].perm;
        }
        return RET_SUCCESS;
}

int CNfs::SetIpAccess(const char* path, const std::vector<std::string>& va)
{
        uint i = 0;
        unsigned long pos = 0;
        vector<stNfsIp> vni;

        for (i=0; i<va.size(); ++i)
        {
                stNfsIp newip;
                pos = va[i].find('/');
                if (string::npos == pos)
                {
                        continue;
                }
                newip.nai.ip = va[i].substr(0, pos);
                if (string::npos != va[i].find('.', pos+1))
                {
                        newip.nai.mask = va[i].c_str() + pos + 1;
                        newip.nai.masklen = MaskLegal(va[i].c_str() + pos + 1);
                        vni.push_back(newip);
                        continue;
                }
                sscanf(va[i].c_str()+pos+1, "%u", &newip.nai.masklen);
                newip.nai.mask = MasklenToMask(newip.nai.masklen);
                vni.push_back(newip);
        }

        return SetIpAccess(path, vni);
}

// 获取网段 参数格式 ip/masklen
int CNfs::GetIpAccess(const char* path, std::vector<stNfsIp>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("GetIpAccess(): empty path!");
                return RET_FAILED;
        }
        va.clear();
        if (RET_FAILED == Load())
        {
                LOG_ERR("GetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        uint i = 0;
        int name_exist = 0;

        for (i=0; i<m_vnd.size(); ++i)
        {
                if (m_vnd[i].path == path)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("GetIpAccess(): cannot find  path! path=%s", path);
                return RET_FAILED;
        }
        va = m_vnd[i].vnis;

        LOG_DBG("GetIpAccess(): success! path=%s", path);
        return RET_SUCCESS;
}

// 设置网段 参数格式 ip/masklen 参数结构体至少要有masklen
int CNfs::SetIpAccess(const char* path, const std::vector<stNfsIp>& va)
{
        if (!path || !*path)
        {
                LOG_ERR("SetIpAccess(): empty path!");
                return RET_FAILED;
        }
        if (RET_FAILED == Load())
        {
                LOG_ERR("SetIpAccess(): load conf failed!");
                return RET_FAILED;
        }

        uint i = 0, nPath = 0;
        int name_exist = 0;

        for (i=0; i<m_vnd.size(); ++i)
        {
                if (m_vnd[i].path == path)
                {
                        name_exist = 1;
                        break;
                }
        }

        if (!name_exist)
        {
                LOG_ERR("SetIpAccess(): cannot find  path! path=%s", path);
                return RET_FAILED;
        }

        m_vnd[i].vnis.clear();
        nPath = i;
        for (i=0; i<va.size(); ++i)
        {
                InsertIntoVector(m_vnd[nPath].vnis, va[i]);
        }

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("SetIpAccess(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("SetIpAccess(): success! path=%s", path);
        return RET_SUCCESS;
}

int CNfs::RemoveUser(const std::vector<std::string>& vu)
{
        return RET_SUCCESS;
}

int CNfs::RemoveIp  (const std::vector<std::string>& vip)
{
        if (RET_FAILED == Load())
        {
                LOG_ERR("RemoveIp(): load conf failed!");
                return RET_FAILED;
        }

        uint nPath = 0, nIp = 0, nCome = 0;
        char cmasklen[16] = {0};
        for (nPath=0; nPath<m_vnd.size(); ++nPath)
        {
                for (nIp=0; nIp<m_vnd[nPath].vnis.size(); ++nIp)
                {
                        sprintf(cmasklen, "%u", m_vnd[nPath].vnis[nIp].nai.masklen);
                        for (nCome=0; nCome<vip.size(); ++nCome)
                        {
                                if (vip[nCome] == (m_vnd[nPath].vnis[nIp].nai.ip+'/'+cmasklen))
                                {
                                        m_vnd[nPath].vnis.erase(m_vnd[nPath].vnis.begin() + nIp);
                                        --nIp;
                                        break;
                                }
                        }
                }
        }

        if (RET_SUCCESS != Write())
        {
                LOG_ERR("RemoveIp(): write conf failed!");
                return RET_FAILED;
        }

        LOG_DBG("RemoveIp(): success!");
        return RET_SUCCESS;
}

void CNfs::SetPerm(int perm)
{
        m_perm = perm;
}

uint CNfs::GetPerm() const
{
        return (uint)m_perm;
}

int CNfs::LoadIn()
{
        const char* cmd = "exportfs -rv";
        int ret = ExeCmd(cmd);
        return ret;
}

// 加载 /etc/exports  不加载 NETACCESSCFG 内容
int CNfs::Load()
{
        if (m_filepath.empty())
        {
                LOG_ERR("Load(): empty filepath");
                return -1;
        }
        if (m_ready)
        {
                LOG_DBG("Load(): no need to load twice!");
                return RET_SUCCESS;
        }

        m_vnd.clear();
        m_ready = 0;

        FILE* fifo = fopen(m_filepath.c_str(), "r");
        if (NULL == fifo)
        {
                LOG_ERR("Load(): NULL file!");
                return -1;
        }

        char cline[PAGE_INFO_LEN] = {0};
        std::string sline;
        int lineno = 0;

        m_vnd.push_back(stNfsDir("*"));

        // 读取行 一行一目录
        while (NULL != fgets(cline, PAGE_INFO_LEN, fifo))
        {
                ++lineno;
                sline = Trim(cline);
                if (sline.empty())
                {
                        m_vnd.push_back(stNfsDir(cline));
                        continue;
                }
                if ('#'==sline[0])
                {
                        stNfsDir newnd;
                        // 遇到保留目录 #RSV:/mnt/share/vg/vol 读取为正式目录
                        newnd.path = (0 == memcmp("#NAS:",sline.c_str(),5)) ? sline.substr(5, sline.find(' ')) : cline;
                        m_vnd.push_back(newnd);
                        continue;
                }
                // 正确行格式: 目录 网段(选项,选项) 网段(选项,选项) 网段(选项,选项)
                std::vector<std::string> vnet;
                Split(sline.c_str(), vnet, ' ', " \t", 0);
                if (vnet.empty())
                {
                        LOG_WRN("Load(): this line has empty option and seems illegal: %d", lineno);
                        continue;
                }
                stNfsDir newnd(vnet[0]);
                uint i = 0, j = 0;
                // 网段中选项解析
                for (i=1; i<vnet.size(); ++i)
                {
                        unsigned long brapos = vnet[i].find('(');
                        if (string::npos == brapos)
                        {
                                continue;
                        }
                        stNfsIp newni;
                        string ipmask = vnet[i].substr(0, brapos);
                        string opt = vnet[i].substr(brapos, string::npos);
                        // 解析 ip/mask
                        unsigned long slapos = ipmask.find('/');
                        stNetAccessIp& newnai = newni.nai;
                        newnai.ip = ipmask.substr(0, slapos);
                        if (!IpLegal(newnai.ip))
                        {
                                LOG_WRN("Load(): illegal ip:%s", newnai.ip.c_str());
                                continue;
                        }
                        newnai.mask = (string::npos == slapos) ? "255.255.255.255" : ipmask.c_str()+slapos+1;
                        // 接受 mask/masklen 两种方式, 同时赋值 mask 和 masklen
                        if (string::npos == newnai.mask.find('.'))
                        {
                                sscanf(newnai.mask.c_str(), "%u", &newnai.masklen);
                                newnai.mask = MasklenToMask(newnai.masklen);
                        }
                        else
                        {
                                newnai.masklen = MaskLegal(newnai.mask);
                        }
                        // nfs 网段选项
                        Trim(opt, "()");
                        std::vector<std::string> vopt;
                        Split(opt.c_str(), vopt, ',');
                        for (j=0; j<vopt.size(); ++j)
                        {
                                if (vopt[j] == "ro")
                                {
                                        newni.perm = SA_RO;
                                }
                                else if (vopt[j] == "rw")
                                {
                                        newni.perm = SA_RW;
                                }
                                // no_root_squash ? perm |= 0x10 : perm &= 0xF
                                else if (vopt[j] == STR_NOROOTSQUASH)
                                {
                                        newni.perm |= NFS_NOROOTSQUASHMARK;
                                        newni.squash = NFS_NOROOTSQUASH;
                                }
                                else if (vopt[j] == STR_ROOTSQUASH)
                                {
                                        newni.perm &= 0xF;
                                        newni.squash = NFS_ROOTSQUASH;
                                }
                                else if (vopt[j] == STR_ALLSQUASH)
                                {
                                        newni.perm &= 0xF;
                                        newni.squash = NFS_ALLSQUASH;
                                }
                        }
                        InsertIntoVector(newnd.vnis, newni);
                }
                m_vnd.push_back(newnd);
        }
        fclose(fifo);

        LOG_DBG("Load() success! path=%s", m_filepath.c_str());
        m_ready = 1;

        return RET_SUCCESS;
}

int CNfs::Write() const
{
        if (m_vnd.empty() || m_ready == 0)
        {
                LOG_ERR("Write(): empty vector, Load first!");
                return -1;
        }

        FILE* fifo = fopen(m_filepath.c_str(), "w");
        if (NULL == fifo)
        {
                LOG_ERR("Write(): NULL file!");
                return -1;
        }

        uint i = 0, j = 0;

        // 一行一目录
        for (i=0; i<m_vnd.size(); ++i)
        {
                if (m_vnd[i].path.empty() || '*' == m_vnd[i].path[0])
                {
                        continue;
                }
                if ('#' == m_vnd[i].path[0] || '\n' == m_vnd[i].path[0])
                {
                        fputs(m_vnd[i].path.c_str(), fifo);
                        continue;
                }
                // "没有网段的目录 输出为保留目录 #NAS:"
                if (m_vnd[i].vnis.empty())
                {
                        fprintf(fifo, "#NAS:%s\n", m_vnd[i].path.c_str());
                        continue;
                }
//printf("<<[%s] ", m_vnd[i].path.c_str());
                fputs(m_vnd[i].path.c_str(), fifo);
                // ips 空格
                for (j=0; j<m_vnd[i].vnis.size(); ++j)
                {
                        if (m_vnd[i].vnis[j].nai.ip.empty() || m_vnd[i].vnis[j].nai.mask.empty())
                        {
                                LOG_WRN("Write(): vector exist empty ip: %s:%u", m_vnd[i].path.c_str(), j);
                                continue;
                        }
                        //uint masklen = MaskLegal(m_vnd[i].vnis[j].nai.mask);

                        if (SA_NO == m_vnd[i].vnis[j].perm)
                        {
                                continue;
                        }

                        fprintf(fifo, " %s/%u%s", m_vnd[i].vnis[j].nai.ip.c_str(),
                                m_vnd[i].vnis[j].nai.masklen, m_vnd[i].vnis[j].OptStr(i));
                }
                fprintf(fifo, "\n");
        }

        fclose(fifo);
        LOG_DBG("Write(): success! path=%s", m_filepath.c_str());

        return RET_SUCCESS;
}

int CNfs::Clear()
{
        m_vnd.clear();
        m_ready = 0;
        return RET_SUCCESS;
}

// NFS 的网段需要排序 以避免权限交叉 返回坐标 pos 比较依据为masklen
uint CNfs::InsertIntoVector(std::vector<stNfsIp>& vnis, const stNfsIp& stni)
{
        uint pos = vnis.size();
        if (vnis.empty())
        {
                vnis.push_back(stni);
                return pos;
        }
        for (; pos!=0; --pos)
        {
                if (stni.nai.masklen <= vnis[pos-1].nai.masklen)
                {
                        if (stni.nai.ip == vnis[pos-1].nai.ip && stni.nai.masklen == vnis[pos-1].nai.masklen)
                        {
                                LOG_WRN("InsertIp: ip already exist! ip=%s", 
                                        stni.nai.ip.c_str());
                                return pos;
                        }
                        vnis.insert(vnis.begin()+pos, stni);
                        return pos;
                }
        }
        vnis.insert(vnis.begin(), stni);
        return 0;
}

// 生成网段选项字符串(不包含网段本身信息,带括号), squash 由 perm 决定
const char* stNfsIp::OptStr(uint fsid) const
{
        static char line[256] = {0};
        // nfs 权限: 0=no 1=ro 2=rw +0x10=no_root_squash <0x10=all_squash
        int t_squash = (perm & NFS_NOROOTSQUASHMARK) ? NFS_NOROOTSQUASH : NFS_ALLSQUASH;
        const char* cperm = (SA_RW == (perm&0xF)) ? "rw" : "ro";
        const char* sq_str = (t_squash == NFS_NOROOTSQUASH) ? STR_NOROOTSQUASH : STR_ALLSQUASH;
        sprintf(line, "(%s,%s,anonuid=%d,anongid=%d,fsid=%u)", 
                cperm, sq_str, anonuid, anongid, fsid);
        return line;
}
