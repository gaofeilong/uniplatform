#ifndef __RAID_MANAGE_H__
#define __RAID_MANAGE_H__

#include <vector>
#include <ctype.h>
#include "Utils/RaidCard/tw_api.h"

#define  MAX_BUS_MODE_LEN       128
#define  MAX_RAID_TYPE_LEN      16
#define  MAX_RAID_MODE_LEN      16

#define RAID_CONF "/etc/nas/Conf/raid.conf"
#define MAIN_PATH "/etc/nas/"
#define PL_RAID   "Script/raid.pl"

using std::vector;
typedef struct RaidCardInfo {
        char         card_driver_version[TW_MAX_DRIVER_VERSION_LEN]; //驱动版本,最长128
        char         card_model_type[TW_MAX_MODEL_NUMBER_LEN];       //类型,最长64字节
        unsigned int card_memory_size;                               //板卡内存,byte
        char         card_fw_version[TW_MAX_FIRMWARE_VERSION_LEN];   //固件版本,最长32字节
        char         card_bios_version[TW_MAX_BIOS_VERSION_LEN];     //BIOS版本,最长32字节
        char         card_sn[TW_MAX_SERIAL_NUMBER_LEN];              //序列号,最长32字节
        unsigned int max_drives_per_unit;                            //阵列最大硬盘数
        char         card_bus_mode[MAX_BUS_MODE_LEN];                //板卡BUS类型
} RaidCardInfo_t;

//磁盘阵列信息
typedef struct RaidUnitInfo {
        int  unit_id;                                //ID
        int  unit_capacity;                          //大小
        int  unit_raid_type;                         //raid级别
        char unit_mode[MAX_RAID_MODE_LEN];           //磁盘阵列状态 
        int  unit_percent_complete;                  //任务完成率(%)
        int  unit_stripe_size;                       //条带大小(kb)
        char unit_cahce_info[4];                     //缓存,R/W/RW/OFF
        char unit_sn[TW_MAX_SERIAL_NUMBER_LEN];      //序列号,最长21字节
        char unit_drive_id_list[32];                 //占用的硬盘,最多32块   
        char unit_diskinfo[12];                      //对应的系统分区
        char unit_drive_id_num;                      //占用的硬盘数,最多32块
        char Reserved[3]; 
} RaidUnitInfo_t;

//硬盘信息
typedef struct RaidPDInfo {
        int                 driver_id;                              //硬盘ID
        char                drive_status[16];                               //硬盘状态
        char                drive_model[TW_MAX_MODEL_NUMBER_LEN];           //型号,最长64字节
        char                drive_fw_version[TW_MAX_FIRMWARE_VERSION_LEN];  //固件版本,最长32
        char                drive_sn[TW_MAX_SERIAL_NUMBER_LEN];             //序列号,最长32字节
        unsigned long long  drive_size;                                     //磁盘大小,byte
        char                drive_wwn[20];                                  //WWN,20字节
        char                drive_interface[8];                             //接口类型:SAS/SATA/UNKNOWN
        char                drive_link_speed[4];                            //接口速率(Gps),1.5/3.0/6.0
        int                 drive_power_on_hours;                           //运行小时数
        int                 drive_temperature;                              //硬盘温度(摄氏度)
        int                 drive_spindle_speed;                            //转速(RPM)
        int                 drive_unit_id;                                  //硬盘所属阵列ID
        int                 drive_slot_num;                                 //硬盘所在槽位
} RaidPDInfo_t; 

enum Raid_Level {
        RAID_0 = 0,
        RAID_1, 
        RAID_5,
        RAID_6,
        RAID_10,
        RAID_50,
        
        SINGLE,
        MIGRATOR,
        RAID_UNKNOWN
};

class RaidMgr {
public:
        //获取板卡信息
        static int GetCardInfo(RaidCardInfo_t* rci);

        //获取所有逻辑磁盘信息  
        static int GetAllUnitInfo(vector<RaidUnitInfo_t>* vecUnitInfo); 

        //获取所有硬盘信息
        static int GetAllPDInfo(vector<RaidPDInfo_t>* vecPDInfo);

        //添加磁盘阵列
        static int AddUnit(int Raidlvl, int* drive_id_list, int driver_num, int stripe_size, int group_number, int& unit_id);

        //验证磁盘阵列
        static int StartVerifyUnit(int unit_id);

        //停止磁盘阵列验证
        static int StopVerifyUnit(int unit_id);

        //重建磁盘阵列
        static int RebuildUinit(int unit_id, int* drive_id_list, const int driver_num, const int raidlvl);
        
        //删除磁盘阵列
        static int DelUnit(int unit_id);

        //磁盘阵列扩容
        static int MigrateUnit(int Raidlvl, int* drive_id_list, int driver_num, int stripe_size, int group_number, int unit_id);

private:
        //获取单个逻辑盘信息
        static int GetUnitInfo(const tw_unit_id_type* uinit_id, RaidUnitInfo_t* pUnitInfo);

        //获取单个硬盘信息
        static int GetPDInfo(const tw_drive_id_type* drive_id, RaidPDInfo_t* pPDInfo);

        //磁盘是否是boot盘
        static int IsBootUnit(tw_unit_id_type unit_id, bool *isBoot);
        
        //硬盘是否可用
        static int IsPDAvailable(tw_drive_id_type drive_id, tw_id_base_type controller_number);

        //bus mode 转换成字符串
        static void Convert_Bus_Mode(char *str,const int mode);
        
        //RaidConfig转换成NAS系统的Raid级别
        static void ConvertRaidConfig(const int RaidConf, int* Raidlvl);
        
        //将Raid级别转换成LSI系统识别的RaidConfig
        static int CovertRaiLvlToConfig(int Raidlvl);
        
        //转换unit 状态
        static void ConvertUnitStatus(const int unit_mode,char * status);
        
        //转换缓存信息
        static void ConvertCacheInfo(char Rcache, char Wcahce, char * cache_info);
        
        //转换硬盘状态信息
        static void ConvertDriveStatus(const int status, char* status_str);
        
        //转换接口类型
        static void ConvertDriveInterface(const char interface, char* interface_str);
        
        //raid级别和硬盘数量是否对应
        static int InvalidRaidAndDriveNum(const int Raidlvl, const int driver_num);
        
        //转换条带大小
        static int ConvertStripeSize(int stripe_size);
        
        //RaidLvl转换成string
        static int RaidLvlToStr(int raidlvl, char * str);
        
        //根据unit_number找到对应的unit_id
        static int GetUnitIdFromUnitNum(const int unit_num, tw_unit_id_type& tw_unit_id);
        //使用tw_cli命令添加raid,不用重启
        static int AddUnitByCli(int controlnum,int Raidlvl, int* drive_id_list, 
                                int driver_num, int stripe_size, int group_number, int& unit_id);        
};

#endif /* __RAID_MANAGE_H__ */
