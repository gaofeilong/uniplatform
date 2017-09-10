#include "Utils/Lightev/Socket.h"
#include "Utils/Lightev/EVCommon.h"
#include "Utils/Json/include/json.h"
#include "Include/Protocol.h"
#include "Include/Types.h"
#include "Utils/Log/Log.h"
#include "Server/Install/Protocol.h"
#include "Agent/Nfs/Protocol.h"

#include <stdio.h>
#include <string.h>

#define LOCALHOST       "127.0.0.1"
#define JSONLENGTH(j)   ((j).toStyledString().size())
#define SET_PACK_HEAD(p, m, v, c, l)\
        do {\
                (p)->Magic = m;\
                (p)->Version = v;\
                (p)->Cmd = c;\
                (p)->Length = l;\
        } while (0)

using namespace lightev;
void SystemInfo();
void SysTimeInfo();
void SysTimeSet();
void NetBaseInfo();
void NetBaseSet();
void GetNetworkInfo();
void SetNetworkInfo();
void FbsConnectIndex();
void FbsConnectData();
void FbsInstallIndex();
void FbsInstallData();
void FbsUninstallIndex();
void FbsUninstallData();
void LicenseInfo();
void LicenseImport();
void LicenseExport();
//nfs
void NfsLookupList();
void NfsCreate();
void NfsEdit();
void NfsRemove();
// adfs
void LookupAdfsMpList();     //ADFS挂载点列表
void CreateAdfsMp();         //新建ADFS挂载点
void ModifyAdfsMp();         //编辑ADFS挂载点
void MountAdfsMp();          //挂载ADFS挂载点
void UmountAdfsMp();         //卸载ADFS挂载点
void RemoveAdfsMp();         //删除ADFS挂载点

int exInfo(int cmd, Json::Value request = Json::Value()); 

// 测试发送所有协议
int main(int argc, char *argv[])
{
        if (argc == 2) {
#define ADFS
#if defined NFS
                if (strcmp(argv[1], "a") == 0) {
                        NfsCreate();
                } else if (strcmp(argv[1], "r") == 0) {
                        NfsRemove();
                } else if (strcmp(argv[1], "l") == 0) {
                        NfsLookupList();
                } else if (strcmp(argv[1], "e") == 0) {
                        NfsEdit();
                }
#elif defined ADFS
                if (strcmp(argv[1], "list") == 0) {
                        LookupAdfsMpList();
                } else if (strcmp(argv[1], "create") == 0) {
                        CreateAdfsMp();
                } else if (strcmp(argv[1], "modify") == 0) {
                        ModifyAdfsMp();
                } else if (strcmp(argv[1], "mount") == 0) {
                        MountAdfsMp();
                } else if (strcmp(argv[1], "umount") == 0) {
                        UmountAdfsMp();
                } else if (strcmp(argv[1], "remove") == 0) {
                        RemoveAdfsMp();
                }
#endif
        } 
        return 0;
}

int exInfo(int cmd, Json::Value request)
{
        int ret = 0;
        request["agentIp"] = LOCALHOST;
        Packet *p = (Packet *)new char[sizeof(Packet) + JSONLENGTH(request)];
        SET_PACK_HEAD(p, 'B', '1', cmd, JSONLENGTH(request));
        memcpy(p->Data, request.toStyledString().c_str(), JSONLENGTH(request));

        int fd = Socket::Connect(LOCALHOST, SERVER_PORT);
        ret = Socket::SendPacket(fd, p);
        if (ret < 0) {
                perror("Send packet error");
                return -1;
        }
        printf("\033[40;32mSended\033[0m: %c, %c, 0x%08X, %d\n%s", 
                p->Magic, p->Version, p->Cmd, p->Length, 
                request.toStyledString().c_str());
        delete [] p;

        Packet pkHead;
        ret = Socket::Receive(fd, (char *)&pkHead, sizeof(Packet));
        if (ret < 0) {
                perror("Recv packet head error");
                return -1;
        }
        p = (Packet *)new char[sizeof(Packet) + pkHead.Length];
        memcpy(p, &pkHead, sizeof(Packet));
        ret = Socket::Receive(fd, p->Data, p->Length);
        if (ret < 0) {
                perror("Recv packet body error");
                return -1;
        }

        Json::Value respond;
        Json::Reader reader;
        reader.parse(p->Data, respond);
        printf("\033[40;32mReceived\033[0m: %c, %c, 0x%08X, %d\n%s", 
                p->Magic, p->Version, p->Cmd, p->Length, 
                respond.toStyledString().c_str());
        delete[] p;
        close(fd);

        return 0;
}

void SystemInfo() { exInfo(CMD_SYS_INFO_LOOKUP); }

void SysTimeInfo() { exInfo(CMD_SYS_TIME_LOOKUP); }
void SysTimeSet()
{
        Json::Value request;
        request["sync"] = 0;
        request["time"] = "2014-01-01 10:10:10";
        request["zone"] = "GMT+10";
        request["ntp"] = "192.168.100.100";
        exInfo(CMD_SYS_TIME_SET, request);
}

void GetNetBaseInfo() { exInfo(CMD_NET_BASE_LOOKUP); }
void SetNetBaseInfo()
{
        Json::Value request;
        request["host"] = "my host";
        request["dns1"] = "192.168.0.10";
        request["dns2"] = "192.168.0.20";
        exInfo(CMD_NET_BASE_SET, request);
}

void GetNetworkInfo() { exInfo(CMD_NET_CARD_LOOKUP); }
void SetNetworkInfo()
{
}

// int main()
//         if (argc != 2) {
//                 printf("\tinvalid argument\n");
//         } else if (strcmp(argv[1], "installindex") == 0) {
//                 FbsInstallIndex();
//         } else if (strcmp(argv[1], "installdata") == 0) {
//                 FbsInstallData();
//         } else if (strcmp(argv[1], "connectdata") == 0) {
//                 FbsConnectData();
//         } else if (strcmp(argv[1], "connectindex") == 0) {
//                 FbsConnectIndex();
//         } else if (strcmp(argv[1], "uninstallindex") == 0) {
//                 FbsUninstallIndex();
//         } else if (strcmp(argv[1], "uninstalldata") == 0) {
//                 FbsUninstallData();
//         } else {
//                 printf("\tinvalid argument\n");
//         }
// }
void FbsConnectIndex()
{
        Json::Value request;
        request["user"] = "root";
        request["password"] = "gfl";
        request["connectIp"] = "192.168.1.92";
        exInfo(CMD_CONNECT_INDEX, request);
}

void FbsConnectData()
{
        Json::Value request;
        request["user"] = "root";
        request["password"] = "gfl";
        request["indexId"] = "1";
        //request["connectIp"].append("192.168.1.86");
        //request["connectIp"].append("192.168.1.87");
        //request["connectIp"].append("192.168.1.88");
        //request["connectIp"].append("192.168.1.89");
        //request["connectIp"].append("192.168.1.90");
        //request["connectIp"].append("192.168.1.91");
        //request["connectIp"].append("192.168.1.92");
        request["connectIp"].append("192.168.1.93");
        request["connectIp"].append("192.168.1.94");
        //request["connectIp"].append("192.168.1.95");
        exInfo(CMD_CONNECT_DATA, request);
}

void FbsInstallIndex()
{
        Json::Value request;
        request["user"] = "root";
        request["password"] = "gfl";
        request["installIp"] = "192.168.1.92";
        exInfo(CMD_INSTALL_INDEX, request);
}

void FbsUninstallIndex()
{
        Json::Value request;
        request["user"] = "root";
        request["password"] = "gfl";
        request["removeIp"] = "192.168.1.92";
        exInfo(CMD_REMOVE_INDEX, request);
}

void FbsInstallData()
{
        Json::Value request;
        request["user"] = "root";
        request["password"] = "gfl";
        request["indexId"] = "1";
        request["installIp"].append("192.168.1.93");
//        request["installIp"].append("192.168.1.94");
        exInfo(CMD_INSTALL_DATA, request);
}

void FbsUninstallData()
{
        Json::Value request;
        request["user"] = "root";
        request["password"] = "gfl";
        request["indexId"] = "1";
        request["removeIp"].append("192.168.1.93");
//        request["removeIp"].append("192.168.1.94");
        exInfo(CMD_REMOVE_DATA, request);
}

void LicenseInfo()
{
        Json::Value request;
        exInfo(CMD_SYS_LICENSE_LOOKUP, request);
}

void LicenseImport()
{
}

void LicenseExport()
{
}

void NfsLookupList()
{
        Json::Value request;
        exInfo(CMD_NFS_LOOKUP, request);
}

void NfsCreate()
{
        Json::Value request, c1, c2;
        c1["ip"]        = "192.168.2.92";
        c1["password"]  = "gfl";
        c2["ip"]        = "192.168.2.93";
        c2["password"]  = "gfl";
        c2["ip"]        = "192.168.2.96";
        c2["password"]  = "gfl";
        request["mp"]   = "/mnt/nfsclient";
        request["name"] = "share1";
        request["path"] = "/mnt/nfsserver";
        request["parameter"] = "rw,async,no_root_squash,fsid=0";
        request["client"].append(c1);
        request["client"].append(c2);
        exInfo(CMD_NFS_CREATE, request);
}

void NfsEdit()
{
        Json::Value request, c1, c2;
        // c1["ip"]        = "192.168.2.92";
        // c1["password"]  = "gfl";
        // c2["ip"]        = "192.168.2.93";
        // c2["password"]  = "gfl";
        // c2["ip"]        = "192.168.2.96";
        // c2["password"]  = "gfl";
        // request["newClient"].append(c1);
        // request["newClient"].append(c2);
        request["mp"]   = "/mnt/nfsclient";
        request["name"] = "share1";
        request["path"] = "/mnt/nfsserver";
        request["parameter"] = "rw,async,no_root_squash,fsid=1";
        exInfo(CMD_NFS_EDIT, request);
}

void NfsRemove()
{
        Json::Value request;
        request["name"].append("share1");
        exInfo(CMD_NFS_REMOVE, request);
}

void LookupAdfsMpList()
{
        Json::Value request;
        exInfo(CMD_ADFS_LIST, request);
}

void CreateAdfsMp()
{
        Json::Value request;
        request["firstDataPath"] = "/mnt/adfs/db";
        request["otherDataPath"].append("/mnt/sda6/db");
        exInfo(CMD_ADFS_CREATE, request);
}

void ModifyAdfsMp()
{
        Json::Value request;
        request["firstDataPath"] = "/mnt/adfs/db";
        request["newDataPath"].append("/mnt/sda6/db");
        exInfo(CMD_ADFS_MODIFY, request);
}

void MountAdfsMp()
{
        Json::Value request;
        request["firstDataPath"] = "/mnt/adfs/db";
        exInfo(CMD_ADFS_MOUNT, request);
}

void UmountAdfsMp()
{
        Json::Value request;
        request["firstDataPath"] = "/mnt/adfs/db";
        exInfo(CMD_ADFS_UMOUNT, request);
}

void RemoveAdfsMp()
{
        Json::Value request;
        request["firstDataPath"].append("/mnt/adfs/db");
        exInfo(CMD_ADFS_REMOVE, request);
}
