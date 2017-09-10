#ifndef _DISK_PROTOCOL_H_
#define _DISK_PROTOCOL_H_

// 存储管理 (0x00030000-0x0003FFFF)
#define CMD_STORAGE_DISK_LOOKUP         0x00030010
#define CMD_STORAGE_VG_LOOKUP           0x00031010
#define CMD_STORAGE_VG_SHOWSINGLE       0x00031020
#define CMD_STORAGE_VG_MERGE            0x00031030
#define CMD_STORAGE_LV_LOOGUP           0x00032000
#define CMD_STORAGE_LV_CREATE           0x00032010
#define CMD_STORAGE_LV_REMOVE           0x00032020
#define CMD_STORAGE_LV_EXTEND           0x00032030
#define CMD_STORAGE_FC_CREATE           0x00033010
#define CMD_STORAGE_FC_REMOVE           0x00033020
#define CMD_STORAGE_FC_LIST             0x00033030


#endif //_DISK_PROTOCOL_H_