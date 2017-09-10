#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "Agent/Net/Protocol.h"
#include "Agent/Disk/Protocol.h"
#include "Agent/Fs/Protocol.h"
#include "Agent/SysInfo/Protocol.h"

// 0x X    X    X    X    X    X    X    X
// 0b 0000 0000 0000 0000 0000 0000 0000 0000
//    ---- ---- --------- -------------------
//    保留 控制 模块      功能

// 保留 [31:28] 保留位(0x0)
// 控制 [27:24] 请求类型:
//              0x00000000 对远程主机的请求
//              0x01000000 对服务器本地的请求
//              0x02000000 对节点管理的请求
// 模块 [23:16] 标识请求属于哪个模块，最多256个模块(0x00-0xFF)
//              0x00010000 SYS    
//              0x00020000 NET    
//              0x00030000 DISK   
//              0x00040000 FS   
//              0x00050000 ARCHIVE
//              0x00060000 NAS    
//              0x00070000 VTL    
//              0x00080000 FBS    
//              0x00090000 NFS    
// 功能 [15:0 ] 标识请求属于模块内的哪个功能，最多64K个功能(0x0000-0xFFFF)
//              正常功能位用前三位，最后一位预留，用以功能扩充

#define CMD_OK                              0x00000000
#define CMD_ERR                             0x00000001
#define CMD_UNKNOWN                         0x000000FF

#define REMOTE_REQUEST                      0x00000000
#define LOCAL_REQUEST                       0x01000000
#define INSTALL_REQUEST                     0x02000000
#define REQUEST_TYPE_MASK                   0x0F000000

#define CMD_SYS                             0x00010000
#define CMD_NET                             0x00020000
#define CMD_DISK                            0x00030000
#define CMD_DDFS                            0x00040000
#define CMD_ARCHIVE                         0x00050000
#define CMD_NAS                             0x00060000
#define CMD_VTL                             0x00070000
#define CMD_ISTRI                           0x00080000
#define CMD_NFS                             0x00090000
#define MODULE_TYPE_MASK                    0x00FF0000

#endif //_PROTOCOL_H_
