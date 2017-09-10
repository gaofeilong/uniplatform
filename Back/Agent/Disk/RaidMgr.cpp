#include "RaidMgr.h"

#include <stdio.h>
#include <string.h>
#include "Utils/Log/Log.h"
#include "Utils/Shell/System.h"

//
// RaidMgr
//

        
//获取板卡信息
int RaidMgr::GetCardInfo(RaidCardInfo_t* rci)
{
        int Ret = TW_RET_SUCCESS;
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        //tw_id_base_type                 controller_number;
        tw_driver_version_type          driver_version;
        tw_model_type                   model;
        tw_memory_size_type             memory_size;
        tw_firmware_version_type        firmware_version;
        tw_bios_version_type            bios_version;
        tw_serial_number_type           serial_number;
        int                             max_drives_per_unit;
        tw_hba_bus_type                 bus_mode;
        
        Ret = tw_get_controller_id_list(&controller_id_list);
        if(Ret == TW_RET_SUCCESS)
        {
                controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);                
                /*for(int index = 0; index < controller_id_list.length; index++)
                {
                        Ret = tw_get_controller_number(*controller_id_ptr,&controller_number);
                        if(Ret == TW_RET_SUCCESS)
                        {
                                LOG_ERR("con num is %d \n",controller_number);
                                break;
                        }
                        else
                        {
                             controller_id_ptr ++;   
                        }                        
                }*/
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }
        
        //板卡驱动版本
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_driver_version(*controller_id_ptr, &driver_version);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver version");
                }
        }
        
        //产品类型
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_model(*controller_id_ptr, &model);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get model");
                }
        }
        
        //板卡内存大小
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_memory_size(*controller_id_ptr, &memory_size);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get model");
                }
        }
        
        //获取板卡固件版本
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_firmware_version(*controller_id_ptr, &firmware_version);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get fw version");
                }
        }
        
        //获取板卡BIOS版本
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_bios_version(*controller_id_ptr, &bios_version);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to BIOS version");
                }
        }
        
        //获取板卡序列号
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_serial_number(*controller_id_ptr, &serial_number);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get SN");
                }
        }
        
        //获取每个阵列支持最大磁盘数
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_max_drives_per_unit( *controller_id_ptr, &max_drives_per_unit);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get max_drives_per_unit");
                }
        }
        
        //板卡BUS类型
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_controller_bus_mode(*controller_id_ptr, &bus_mode);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get bus mode");
                }
        }
        if(Ret == TW_RET_SUCCESS)
        {
                strcpy(rci->card_driver_version, driver_version);
                strcpy(rci->card_model_type, model);
                //MB
                rci->card_memory_size = memory_size/(1024*1024);
                strcpy(rci->card_fw_version, firmware_version);
                strcpy(rci->card_bios_version, bios_version);
                strcpy(rci->card_sn, serial_number);
                rci->max_drives_per_unit = max_drives_per_unit;
                Convert_Bus_Mode(rci->card_bus_mode,bus_mode);                
        }
        return Ret;
}       

void RaidMgr::Convert_Bus_Mode(char *str,const int mode)
{
        if(mode >= TW_HBA_BUS_PCI_MODE64 && mode <= TW_HBA_BUS_PCI_66)
        {
                strcpy(str,"HBA_BUS_PCI");
        }
        else if(mode >= TW_HBA_BUS_PCIX_66 && mode <= TW_HBA_BUS_PCIX_133)
        {
                strcpy(str,"HBA_BUS_PCIX");
        }
        else if(mode >= TW_HBA_BUS_PCIE_G1 && mode <= TW_HBA_BUS_PCIE_LANE_16X)
        {
                strcpy(str,"HBA_BUS_PCIE");
        }
        else
        {                
                strcpy(str, "HBA_BUS_UNKNOWN");
        }
}       
        
//获取所有逻辑磁盘信息
int RaidMgr::GetAllUnitInfo(vector<RaidUnitInfo_t>* vecUnitInfo)
{

        int Ret = TW_RET_SUCCESS;
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        
        tw_unit_id_list_type            unit_id_list;
        tw_unit_id_type                 *unit_id_ptr;
        
        
        Ret = tw_get_controller_id_list(&controller_id_list);
        if(Ret == TW_RET_SUCCESS)
        {
                controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);
                Ret = tw_get_unit_id_list(*controller_id_ptr, &unit_id_list);                                     
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }        
        if(Ret == TW_RET_SUCCESS)
        {
                if (unit_id_list.length >= 1)
                {
                        unit_id_ptr = (tw_unit_id_type *)&(unit_id_list.list[TW_FIRST_UNIT]);
                        for (int j = 0; j < unit_id_list.length; j++)
                        {
                                RaidUnitInfo_t UnitInfo;
                                memset(&UnitInfo, 0, sizeof(UnitInfo));
                                Ret = GetUnitInfo(unit_id_ptr, &UnitInfo);
                                if(Ret == TW_RET_SUCCESS)
                                {
                                        vecUnitInfo->push_back(UnitInfo);
                                }                                
                                unit_id_ptr ++;
                        }
                        //GetUnitDisk(*vecUnitInfo);
                }
        }
        else
        {
                LOG_ERR("Failed to get unit id list");
        }
        return Ret;
}
//获取单个逻辑盘信息
int RaidMgr::GetUnitInfo(const tw_unit_id_type* unit_id_ptr, RaidUnitInfo_t* pUnitInfo)
{
        int Ret = TW_RET_SUCCESS;
        tw_configuration_type           configuration;
        tw_unit_mode_type               unit_mode;
        tw_percentage_complete_type     percentage_complete;
        int                             unit_stripe_size;
        tw_unit_cache_policy_type       unit_rcache_state;
        tw_unit_cache_state_type        unit_wcache_state;
        tw_unit_serial_number_type      unit_serial_number;
        tw_drive_id_list_type           drive_id_list;
        tw_drive_id_type                *drive_id_ptr;
        tw_capacity_type                unit_capacity;
        
        tw_id_base_type controller_number;
        tw_id_base_type port_number;
        tw_id_base_type logical_port_number;
        tw_id_base_type unit_number;
        tw_id_base_type subunit_number;
        tw_id_base_type subsubunit_number;
        
        Ret = tw_get_unit_number(*unit_id_ptr,
                                &controller_number,
                                &logical_port_number,
                                &unit_number, 
                                &subunit_number, 
                                &subsubunit_number);
        if(Ret != TW_RET_SUCCESS)
        {
                LOG_ERR("Failed to get unit_number");
        }
        //空间大小
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_capacity(*unit_id_ptr, &unit_capacity);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit capacity");
                }
        }             
        //Raid级别 
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_configuration(*unit_id_ptr, &configuration);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get raid level");
                }
        }
        
        //阵列状态
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_mode(*unit_id_ptr, &unit_mode);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit status");
                }
        }
        
        //后台任务完成比率
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_percentage_complete(*unit_id_ptr, &percentage_complete);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit percentage complete");
                }
        }
        //获取条带大小(kBytes)
        if(Ret == TW_RET_SUCCESS)
        {
                if(configuration == TW_UNIT_CONFIG_RAID_0
                    || configuration == TW_UNIT_CONFIG_RAID_5
                    || configuration == TW_UNIT_CONFIG_RAID_6
                    || configuration == TW_UNIT_CONFIG_RAID_10
                    || configuration == TW_UNIT_CONFIG_RAID_50)
                {
                        Ret = tw_get_unit_stripe_size(*unit_id_ptr, &unit_stripe_size);
                        if(Ret != TW_RET_SUCCESS)
                        {
                                LOG_ERR("Failed to get unit stripe size");
                        }
                }
        }
        
        //获取缓存信息
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_read_cache_policy(*unit_id_ptr, &unit_rcache_state);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit rcache");
                }
                Ret = tw_get_unit_write_cache_state(*unit_id_ptr, &unit_wcache_state);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit wcache");
                }
        }
        
        //获取阵列序列号
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_serial_number(*unit_id_ptr, &unit_serial_number);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit SN");
                }
        }
        
        //阵列的硬盘
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_id_list_from_unit_id(*unit_id_ptr, &drive_id_list);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit drivers");
                }
        }
        if(Ret == TW_RET_SUCCESS)
        {
                pUnitInfo->unit_id = (int)unit_number;
                sprintf(pUnitInfo->unit_diskinfo,"/dev/sd%c",(char)('a' + pUnitInfo->unit_id));
                int Raidlvl = RAID_UNKNOWN;
                ConvertRaidConfig((int)configuration, &Raidlvl);
                pUnitInfo->unit_raid_type = Raidlvl;
                ConvertUnitStatus((int)unit_mode,pUnitInfo->unit_mode);
                if(unit_mode != TW_UNIT_MODE_VERIFYING
                        && unit_mode != TW_UNIT_MODE_INITIALIZING
                        && unit_mode != TW_UNIT_MODE_REBUILDING
                        && unit_mode != TW_UNIT_MODE_MIGRATING)
                {
                        percentage_complete = -1;
                }
                pUnitInfo->unit_percent_complete = percentage_complete;
                pUnitInfo->unit_capacity = unit_capacity/(1024*1024*2);
                if(Raidlvl >= RAID_0 && Raidlvl <= RAID_50 && Raidlvl != RAID_1)
                {
                        pUnitInfo->unit_stripe_size = unit_stripe_size;
                }
                else
                {
                        pUnitInfo->unit_stripe_size = -1;
                }                
                ConvertCacheInfo((char)unit_rcache_state,(char)unit_wcache_state,pUnitInfo->unit_cahce_info);
                strcpy(pUnitInfo->unit_sn, unit_serial_number);
                if(drive_id_list.length >= 1)
                {
                        drive_id_ptr = (tw_drive_id_type *) 
                               &(drive_id_list.list[TW_FIRST_DRIVE]);
                        for(int i = 0 ; i < drive_id_list.length; i++)
                        {
                                Ret = tw_get_drive_number(*drive_id_ptr,&controller_number, &port_number); 
                                if(Ret == TW_RET_SUCCESS)
                                {
                                        pUnitInfo->unit_drive_id_list[i] = port_number;
                                        drive_id_ptr++;        
                                }
                                else
                                {
                                        LOG_ERR("Failed to get unit drive_number");
                                        break;
                                }
                                
                        }
                }
                pUnitInfo->unit_drive_id_num = drive_id_list.length;
        }                
        return Ret;
}

int RaidMgr::RaidLvlToStr(int raidlvl, char * str)
{
        int Ret = 0;
        switch(raidlvl)
        {
                case RAID_0:
                        strcpy(str, "RAID0");
                        break;
                case RAID_1:
                        strcpy(str, "RAID1");
                        break;
                case RAID_5:
                        strcpy(str, "RAID5");
                        break;
                case RAID_6:
                        strcpy(str, "RAID6");
                        break;
                case RAID_10:
                        strcpy(str, "RAID10");
                        break;
                case RAID_50:
                        strcpy(str, "RAID50");
                        break;   
                default:
                        strcpy(str, "UNKNOWN");
                        Ret = -1;
                        break;
        }
        return Ret;
}
void RaidMgr::ConvertCacheInfo(char rcache, char wcache, char * cache_info)
{
        if(rcache == TW_UNIT_READ_CACHE_BASIC && wcache == TW_UNIT_CACHE_ENABLED)
        {
                strcpy(cache_info, "RW");
        }
        else if(rcache == TW_UNIT_READ_CACHE_BASIC && wcache == TW_UNIT_CACHE_DISABLED)
        {
                strcpy(cache_info, "R");
        }
        if(rcache == TW_UNIT_READ_CACHE_INTELLIGENT && wcache == TW_UNIT_CACHE_ENABLED)
        {
                strcpy(cache_info, "RiW");
        }
        else if(rcache == TW_UNIT_READ_CACHE_INTELLIGENT && wcache == TW_UNIT_CACHE_DISABLED)
        {
                strcpy(cache_info, "Ri");
        }
        else if(rcache == TW_UNIT_READ_CACHE_DISABLED && wcache == TW_UNIT_CACHE_ENABLED)
        {
                strcpy(cache_info, "W");
        }
        else
        {
                strcpy(cache_info, "OFF");
        }
}        
void RaidMgr::ConvertUnitStatus(const int unit_mode,char * status)
{
        switch(unit_mode)
        {                      
                case TW_UNIT_MODE_OK:
                        strcpy(status, "OK");                          
                        break;                      
                case TW_UNIT_MODE_VERIFYING:
                        strcpy(status, "VERIFYING");            
                        break;                      
                case TW_UNIT_MODE_INITIALIZING:
                        strcpy(status, "INITIALIZING");                          
                        break;                      
                case TW_UNIT_MODE_DEGRADED:
                        strcpy(status, "DEGRADED");              
                        break;                      
                case TW_UNIT_MODE_REBUILDING:
                        strcpy(status, "REBUILDING");                          
                        break;                      
                case TW_UNIT_MODE_MIGRATING:
                        strcpy(status, "MIGRATING");
                        break;                      
                case TW_UNIT_MODE_INOPERABLE:
                        strcpy(status, "INOPERABLE");       
                        break;                      
                default:
                        strcpy(status, "UNKNOWN");                          
                        break;                  
        }
}

void RaidMgr::ConvertRaidConfig(const int RaidConf,int * Raidlvl)
{
        if(RaidConf == TW_UNIT_CONFIG_RAID_0)
        {
                *Raidlvl = RAID_0;
        }
        else if(RaidConf == TW_UNIT_CONFIG_RAID_1 || RaidConf == TW_UNIT_CONFIG_TWINSTOR)
        {
                *Raidlvl = RAID_1;
        }
        else if(RaidConf == TW_UNIT_CONFIG_RAID_5)
        {
                *Raidlvl = RAID_5;
        }
        else if(RaidConf == TW_UNIT_CONFIG_RAID_6)
        {
                *Raidlvl = RAID_6;
        }
        else if(RaidConf == TW_UNIT_CONFIG_RAID_10)
        {
                *Raidlvl = RAID_10;
        }
        else if(RaidConf == TW_UNIT_CONFIG_RAID_50)
        {
                *Raidlvl = RAID_50;
        }
        else if(RaidConf == TW_UNIT_CONFIG_IBOD)
        {
                *Raidlvl = SINGLE;
        }
        else if(RaidConf == TW_UNIT_CONFIG_MIGRATOR)
        {
                *Raidlvl = MIGRATOR;
        }
        else
        {
                *Raidlvl = RAID_UNKNOWN;
        }
}
//获取所有硬盘信息
int RaidMgr::GetAllPDInfo(vector<RaidPDInfo_t>* vecPDInfo)
{
        int Ret = TW_RET_SUCCESS;
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        
        tw_drive_id_list_type           drive_id_list;
        tw_drive_id_type                *drive_id_ptr;
        
        Ret = tw_get_controller_id_list(&controller_id_list);
        if(Ret == TW_RET_SUCCESS)
        {
                 controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_id_list(*controller_id_ptr, &drive_id_list);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver list");
                }
        }
        if(Ret == TW_RET_SUCCESS)
        {
                if (drive_id_list.length >= 1)
                {
                        drive_id_ptr = (tw_drive_id_type *)&(drive_id_list.list[TW_FIRST_DRIVE]);
                        for(int j = 0; j < drive_id_list.length; j++)
                        {
                                RaidPDInfo_t PDInfo;
                                Ret = GetPDInfo(drive_id_ptr, &PDInfo);
                                if(Ret == TW_RET_SUCCESS)
                                {
                                        vecPDInfo->push_back(PDInfo);
                                }
                                drive_id_ptr++;
                        }
                }                
        }
        
        return Ret;
}


//获取单个硬盘信息
int RaidMgr::GetPDInfo(const tw_drive_id_type* drive_id_ptr, RaidPDInfo_t* pPDInfo)
{
        int Ret = TW_RET_SUCCESS;
        tw_drive_status_type            drive_status;
        tw_model_type                   drive_model;
        tw_firmware_version_type        drive_firmware_version;
        tw_serial_number_type           drive_serial_number;
        tw_capacity_type                drive_capacity;
        tw_sas_wwn_type                 sas_wwn;
        tw_drive_interface_type         drive_interface;
        tw_drive_link_type              link_status;
        int                             power_on_hours;
        int                             temperature;
        int                             spindle_speed;
        tw_unit_id_type                 unit_id;
        
        tw_id_base_type controller_number;
        tw_id_base_type port_number;
        tw_id_base_type logical_port_number;
        tw_id_base_type unit_number;
        tw_id_base_type subunit_number;
        tw_id_base_type subsubunit_number;
        
        tw_id_base_type enclosure_number;
        tw_id_base_type drive_slot_number;        
        tw_port_id_type port_id;
        tw_drive_slot_id_type drive_slot_id;
        
        Ret = tw_get_drive_number(*drive_id_ptr,&controller_number, &port_number); 
        if(Ret != TW_RET_SUCCESS)
        {
                LOG_ERR("Failed to get drive_number");
        }
        
        //port id
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_port_id(*drive_id_ptr, &port_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver port id");
                }
        }
        
        //slot num
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_slot_id_from_port_id(port_id, &drive_slot_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        Ret = TW_RET_SUCCESS;
                        drive_slot_number = TW_NULL_DRIVE_SLOT;
                }
                else
                {
                        Ret = tw_get_drive_slot_number(drive_slot_id, &controller_number,
                                        &enclosure_number,&drive_slot_number);
                        if(Ret != TW_RET_SUCCESS)
                        {
                                LOG_ERR("Failed to get driver slot id");
                        }
                }
        }        
        
        //硬盘状态
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_status(*drive_id_ptr, &drive_status);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver status");
                }
        }
        
        //硬盘类型
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_model(*drive_id_ptr, &drive_model);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver model");
                }
        }
        
        //硬盘固件版本
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_firmware_version(*drive_id_ptr, &drive_firmware_version);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get fw version");
                }
        }
        
        //硬盘序列号
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_serial_number(*drive_id_ptr, &drive_serial_number);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver SN");
                }
        }
        
        //硬盘大小
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_capacity(*drive_id_ptr, &drive_capacity);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver capacity");
                }
        }
        
        //硬盘WWN
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_sas_wwn(*drive_id_ptr, &sas_wwn);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver WWN");
                }
        }
        
        //硬盘接口类型
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_interface(*drive_id_ptr, &drive_interface);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver interface");
                }
        }
        
        //硬盘接口速率
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_link_status(*drive_id_ptr, &link_status);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver link speed");
                }
        }
        
        //硬盘运行时长
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_power_on_hours(*drive_id_ptr, &power_on_hours);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver power_on_hours");
                }
        }
        
        //硬盘温度
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_temperature(*drive_id_ptr, &temperature);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver temperature");
                }
        }
        
        //硬盘转速
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_spindle_speed(*drive_id_ptr, &spindle_speed);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver splindle_speed");
                }
        }
        
        //硬盘所属阵列
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_id_from_drive_id(*drive_id_ptr, &unit_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get driver unit");
                }
                else
                {
                        Ret = tw_get_unit_number(unit_id,
                                                &controller_number,
                                                &logical_port_number,
                                                &unit_number, 
                                                &subunit_number, 
                                                &subsubunit_number);
                        if(Ret != TW_RET_SUCCESS)
                        {
                                LOG_ERR("Failed to get driver unit number");
                        }                                                
                }  
        }
        if(Ret == TW_RET_SUCCESS)
        {
                //drive_slot_number
                if(drive_slot_number == TW_NULL_DRIVE_SLOT)
                {
                        pPDInfo->drive_slot_num = -1;
                }       
                else
                {
                        pPDInfo->drive_slot_num = drive_slot_number;
                }
                pPDInfo->driver_id = port_number;                
                ConvertDriveStatus(drive_status,pPDInfo->drive_status);
                strcpy(pPDInfo->drive_model, drive_model);
                strcpy(pPDInfo->drive_fw_version, drive_firmware_version);
                strcpy(pPDInfo->drive_sn, drive_serial_number);
                //GB
                pPDInfo->drive_size = drive_capacity/(1024*1024*2);
                
                char sas_wwn_str[16+1]; // 2 char space per byte + null char
                char *ptr = &sas_wwn_str[0];
                memset(sas_wwn_str, 0xff, sizeof(sas_wwn_str));
                for (int k=0;k<8;k++)
                {
                        sprintf(ptr,"%02x", ((unsigned char *)&sas_wwn)[k]);
                        ptr+=2;
                }
                strcpy(pPDInfo->drive_wwn,sas_wwn_str);
                
                ConvertDriveInterface(drive_interface, pPDInfo->drive_interface);
                
                if(link_status == TW_DRIVE_LINK_1_5_GBPS)
                {
                        strcpy(pPDInfo->drive_link_speed,"1.5");
                }
                else if(link_status == TW_DRIVE_LINK_3_0_GBPS)
                {
                        strcpy(pPDInfo->drive_link_speed,"3.0");
                }
                else
                {
                        strcpy(pPDInfo->drive_link_speed,"6.0");
                }
                pPDInfo->drive_power_on_hours = power_on_hours;
                pPDInfo->drive_temperature = temperature;
                pPDInfo->drive_spindle_speed = spindle_speed;
                pPDInfo->drive_unit_id = unit_number;
                if(unit_number == 0xff)
                {
                        pPDInfo->drive_unit_id = -1;
                }
        }        
        return Ret;
}        

void RaidMgr::ConvertDriveInterface(const char interface, char* interface_str)
{
        if(interface == TW_DRIVE_INTERFACE_SATA_DIRECT_ATTACHED 
           || interface == TW_DRIVE_INTERFACE_SATA_EXPANDER_ATTACHED)
        {
                strcpy(interface_str, "SATA");
        }
        else if(interface == TW_DRIVE_INTERFACE_SAS_DIRECT_ATTACHED 
           || interface == TW_DRIVE_INTERFACE_SAS_EXPANDER_ATTACHED)
        {
                strcpy(interface_str, "SAS");
        }
        else
        {
                strcpy(interface_str, "UNKNOWN");
        }                
                           
}       

void RaidMgr::ConvertDriveStatus(const int status, char* status_str)
{
        switch(status)
        {
                case TW_DRIVE_STATUS_OK:
                        strcpy(status_str, "OK");
                        break;
                case TW_DRIVE_STATUS_UNSUPPORTED:
                        strcpy(status_str, "UNSUPPORTED");
                        break;
                case TW_DRIVE_STATUS_ECC_ERROR:
                        strcpy(status_str, "ECC_ERROR");
                        break;
                case TW_DRIVE_STATUS_SMART_FAILURE:
                        strcpy(status_str, "SMART_FAILURE");
                        break;
                case TW_DRIVE_STATUS_DEVICE_ERROR:
                        strcpy(status_str, "DEVICE_ERROR");
                        break;
                case TW_DRIVE_STATUS_DCB_READ_TIMEOUT:
                        strcpy(status_str, "DCB_READ_TIMEOUT");
                        break;
                case TW_DRIVE_STATUS_DCB_READ_FAILURE:
                        strcpy(status_str, "DCB_READ_FAILURE");
                        break;
                case TW_DRIVE_STATUS_DCB_ORPHAN:
                        strcpy(status_str, "ORPHAN");
                        break;
                case TW_DRIVE_STATUS_DCB_DATA_CHECK:
                        strcpy(status_str, "DCB_DATA_CHECK");
                        break;
                case TW_DRIVE_STATUS_UNSUPPORTED_DCB:
                        strcpy(status_str, "UNSUPPORTED_DCB");
                        break;
                case TW_DRIVE_STATUS_UNCONVERTED_DCB:
                        strcpy(status_str, "UNCONVERTED_DCB");
                        break;
                case TW_DRIVE_STATUS_REMOVED:
                        strcpy(status_str, "REMOVED");
                        break;
                case TW_DRIVE_STATUS_OFFLINE_JBOD:
                        strcpy(status_str, "OFFLINE_JBOD");
                        break;
                case TW_DRIVE_STATUS_OFFLINE:
                        strcpy(status_str, "OFFLINE");
                        break;
                default:
                        strcpy(status_str, "UNKNOWN");
                        break;                                                                                                                                                                                            
        }
}
//添加磁盘阵列
int RaidMgr::AddUnit(int Raidlvl, int* drive_id_list, int driver_num, int stripe_size, int group_number, int& unit_id)
{
        int Ret = TW_RET_SUCCESS;
        tw_drive_id_list_type           tw_drive_id_list;
        //tw_unit_id_type                 tw_unit_id;
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        tw_id_base_type                 controller_number; 
        //int                             unit_id;
        
        int RaidConfig;
        memset (&tw_drive_id_list, 0, sizeof (tw_drive_id_list_type));
        
        Ret = tw_get_controller_id_list(&controller_id_list);
        if(Ret == TW_RET_SUCCESS)
        {
                controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);
                Ret = tw_get_controller_number(*controller_id_ptr,&controller_number);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get controller number");
                }
                
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }
        //raid级别和硬盘数是否对应
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = InvalidRaidAndDriveNum(Raidlvl, driver_num);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Invalid Raid And DriveNum");
                }
                else
                {
                        if(Raidlvl == RAID_50)
                        {
                                if(!(group_number != 0 && driver_num % group_number == 0))
                                {
                                        Ret = TW_RET_FAILED;
                                        LOG_ERR("group number invalid for RAID50");
                                }
                        }
                        
                        if(Raidlvl == RAID_6 && stripe_size != 64)
                        {
                                Ret = TW_RET_FAILED;
                                LOG_ERR("stripe_size is not 64k for RAID6");
                        }
                }
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                tw_drive_id_list.length = driver_num;
                for(int i = 0; i < driver_num; i++)
                {
                        tw_drive_id_type        drive_id;
                        Ret = tw_create_drive_id(controller_number, drive_id_list[i], &drive_id);
                        if(Ret != TW_RET_SUCCESS)
                        {
                                LOG_ERR("Failed to create drive_id");
                                break;
                        }
                        else
                        {
                                memcpy (&(tw_drive_id_list.list[i]), &drive_id, TW_API_ID_SIZE);
                        }
                        
                        if(Ret == TW_RET_SUCCESS)
                        {
                                Ret = IsPDAvailable(drive_id, controller_number);
                                if(Ret != TW_RET_SUCCESS)
                                {
                                        LOG_ERR("PD is not available");
                                        break;
                                }
                        }                                
                }
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                RaidConfig = CovertRaiLvlToConfig(Raidlvl);
                if(RaidConfig == TW_RET_FAILED)
                {
                        LOG_ERR("Invalid Raid Level");
                        Ret = TW_RET_FAILED;
                }                        
        }                
        if(Ret == TW_RET_SUCCESS)
        {
                /*
                if(Raidlvl == RAID_50)
                {
                        Ret = tw_create_unit_with_layout(&tw_drive_id_list, 
                                                        TW_UNIT_CONFIG_RAID_50, 
                                                        ConvertStripeSize(stripe_size),
                                                        TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT,
                                                        &tw_unit_id, group_number);
                }
                else
                {
                        Ret = tw_create_unit(&tw_drive_id_list, 
                                                RaidConfig, 
                                                ConvertStripeSize(stripe_size),
                                                TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT,
                                                &tw_unit_id);
                }*/
                Ret = AddUnitByCli(controller_number,Raidlvl,drive_id_list,
                                        driver_num,stripe_size,group_number,unit_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Create Unit Error");
                }
        }
        //初始化Raid,Raid0除外
        if(Ret == TW_RET_SUCCESS && Raidlvl != RAID_0)
        {
                //Ret = tw_verify_unit(tw_unit_id);
                Ret = StartVerifyUnit(unit_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Initialize Unit Error");
                }                
        }
        
                        
        return Ret;
} 
int RaidMgr::AddUnitByCli(int controlnum,int Raidlvl, int* drive_id_list, 
                        int driver_num, int stripe_size, int group_number,int& unit_id)
{
        unit_id = -1;
        int Ret = TW_RET_SUCCESS;
        char lvlstr[8];
        memset(lvlstr, 0, 8);
        char diskstr[32];
	memset(diskstr, 0, 32);
	
	char addcmd[256];
	memset(addcmd, 0, 256);
	Ret = RaidLvlToStr(Raidlvl, lvlstr);        
	if(Ret != TW_RET_SUCCESS)
        {
               LOG_ERR("Raidlvl unknown");
        }
        if(Ret == TW_RET_SUCCESS)
        {
                for(int i = 0; i < driver_num; i++)
                {
                        char tmp[4];
                        memset(tmp, 0, 4);
		        sprintf(tmp,"%d:",drive_id_list[i]);		        
		        strcat(diskstr,tmp);
                }
                diskstr[strlen(diskstr)-1] = '\0';
                
                sprintf(addcmd,"tw_cli /c%d add type=%s disk=%s ",controlnum, lvlstr, diskstr);
                if(Raidlvl != RAID_1)
                {
                        char tmp[16];
                        memset(tmp, 0, 16);
                        sprintf(tmp,"stripe=%d ",stripe_size);
                        strcat(addcmd,tmp);
                }
                
                if(Raidlvl == RAID_50)
                {
                        char tmp[16];
                        memset(tmp, 0, 16);
                        sprintf(tmp,"group=%d ",driver_num/group_number);
                        strcat(addcmd,tmp);
                }
                strcat(addcmd,"noautoverify 2>&1");
                
                //Ret = system(addcmd);
                FILE    *file   = popen(addcmd, "r");
                if (file == NULL)
                {
                        LOG_ERR("tw_cli is error ");
                        Ret = TW_RET_FAILED;
                }
                if(Ret == TW_RET_SUCCESS)
                {
                        int     rt      = 0;
                        size_t  size    = 0;
                        char    *buf    = NULL;
                        while((rt = getline(&buf, &size, file)) >= 0)
                        {
                                buf[strlen(buf) -1] = '\0';
                                if (strstr(buf, "Error") != NULL)
                                {
                                        Ret = TW_RET_FAILED;
                                        break;
                                }
                                char *tmpunit = strstr(buf,"/u");
                                if(tmpunit != NULL)
                                {
                                        if(strncmp(tmpunit, "/u",2) == 0)
                                        {
                                                int unit_num = -1;
                                                sscanf(tmpunit + 2, "%d", &unit_num);
                                                unit_id = unit_num;
                                        }
                                        break;
                                }
                        }
                        if(buf != NULL)
                        {
                                free(buf);
                                buf = NULL;
                        }
                }
        }
        if(unit_id == -1)
        {
                Ret = TW_RET_FAILED;
                 LOG_ERR("tw_cli not exist or error");
        }
        return Ret;
}
int RaidMgr::CovertRaiLvlToConfig(int Raidlvl)
{
        if(RAID_0 == Raidlvl)
        {
                return TW_UNIT_CONFIG_RAID_0;
        }
        else if(RAID_1 == Raidlvl)
        {
                return TW_UNIT_CONFIG_TWINSTOR;
        }
        else if(RAID_5 == Raidlvl)
        {
                return TW_UNIT_CONFIG_RAID_5;
        }
        else if(RAID_6 == Raidlvl)
        {
                return TW_UNIT_CONFIG_RAID_6;
        }
        else if(RAID_10 == Raidlvl)
        {
                return TW_UNIT_CONFIG_RAID_10;
        }        
        else if(RAID_50 == Raidlvl)
        {
                return TW_UNIT_CONFIG_RAID_50;
        }
        else if(SINGLE == Raidlvl)
        {
                return TW_UNIT_CONFIG_IBOD;
        }
        else
        {
                return TW_RET_FAILED;
        }                
}        

int RaidMgr::ConvertStripeSize(int stripe_size)
{
        int stripe;
        switch(stripe_size)
        {
                case 16:
                        stripe = TW_UNIT_STRIPE_SIZE_16K;
                        break;                        
                case 64:
                        stripe = TW_UNIT_STRIPE_SIZE_64K;
                        break;
                case 256:
                        stripe = TW_UNIT_STRIPE_SIZE_256K;
                        break;
                default:
                        stripe = TW_UNIT_STRIPE_SIZE_64K;
                        break;
        }
        return stripe;
}
int RaidMgr::InvalidRaidAndDriveNum(const int Raidlvl, const int driver_num)
{
        int Ret = TW_RET_SUCCESS;
        if( Raidlvl == RAID_0)
        {
                if(driver_num < 2)
                {
                        Ret = TW_RET_FAILED;
                }
        }
        else if( Raidlvl == RAID_1)
        {
                if(driver_num != 2)
                {
                        Ret = TW_RET_FAILED;
                }
        }
        else if( Raidlvl == RAID_5)
        {
                if(driver_num < 3)
                {
                        Ret = TW_RET_FAILED;
                }
        }
        else if( Raidlvl == RAID_6)
        {
                if(driver_num < 5)
                //if(driver_num < 4)
                {
                        Ret = TW_RET_FAILED;
                }
        }
        else if( Raidlvl == RAID_10)
        {
                if(!(driver_num >= 4 && driver_num % 2 ==0))
                {
                        Ret = TW_RET_FAILED;
                }
        }
        else if( Raidlvl == RAID_50)
        {
                if(driver_num < 6)
                {
                        Ret = TW_RET_FAILED;
                }
        }
        else
        {
                Ret = TW_RET_FAILED;
        }
        return Ret;
}

//硬盘是否可用
int RaidMgr::IsPDAvailable(tw_drive_id_type drive_id, tw_id_base_type controller_number)
{
        int Ret = TW_RET_FAILED;
        
        tw_drive_status_type            drive_status;
        tw_unit_id_type                 unit_id;
        
        tw_id_base_type                  logical_port_number;
        tw_id_base_type                  unit_number;
        tw_id_base_type                  subunit_number;
        tw_id_base_type                  subsubunit_number;

        Ret = tw_get_drive_status(drive_id, &drive_status);
        if(Ret == TW_RET_SUCCESS && drive_status == TW_DRIVE_STATUS_OK)
        {
                Ret = TW_RET_SUCCESS;
        }
        else
        {
                Ret = TW_RET_FAILED;
        }
        
        if(Ret == TW_RET_SUCCESS)
        {                
                
                Ret = tw_get_unit_id_from_drive_id(drive_id, &unit_id);
                if(Ret == TW_RET_SUCCESS)
                {
                        Ret = tw_get_unit_number(unit_id,
                                               &controller_number,
                                               &logical_port_number,
                                               &unit_number,
                                               &subunit_number,
                                               &subsubunit_number);
                        if(Ret == TW_RET_SUCCESS && unit_number != TW_NULL_UNIT)
                        {
                                Ret = TW_RET_FAILED;
                        }
                        else
                        {
                                Ret = TW_RET_SUCCESS;
                        }                                
                }
        }
        return Ret;
}

int RaidMgr::GetUnitIdFromUnitNum(const int  unit_num, tw_unit_id_type& tw_unit_id)
{
        int Ret = TW_RET_SUCCESS;
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        tw_unit_id_list_type            unit_id_list;
        tw_unit_id_type                 *unit_id_ptr;
        bool bFound = false;
        
        Ret = tw_get_controller_id_list(&controller_id_list);
        if(Ret == TW_RET_SUCCESS)
        {
                controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);
                Ret = tw_get_unit_id_list(*controller_id_ptr, &unit_id_list);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit_id_list");
                }
                
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                if(unit_id_list.length >= 1)
                {
                        unit_id_ptr = (tw_unit_id_type *)&(unit_id_list.list[TW_FIRST_UNIT]);
                        for(int i = 0; i < unit_id_list.length; i++)
                        {
                                tw_id_base_type controller_number;
                                tw_id_base_type logical_port_number;
                                tw_id_base_type tmp_unit_number;
                                tw_id_base_type subunit_number;
                                tw_id_base_type subsubunit_number;
                        
                                Ret = tw_get_unit_number(*unit_id_ptr,
                                                        &controller_number,
                                                        &logical_port_number,
                                                        &tmp_unit_number, 
                                                        &subunit_number, 
                                                        &subsubunit_number);
                                if(Ret == TW_RET_SUCCESS)
                                {
                                        if(tmp_unit_number == unit_num)
                                        {
                                                tw_unit_id = *unit_id_ptr;
                                                bFound = true;
                                                break;
                                        }
                                }
                                else
                                {
                                        LOG_ERR("Get Unit Num Error");
                                }
                                unit_id_ptr++;
                        }
                }
        }
        if(bFound == false)
        {
                LOG_ERR("Get Unit Num Error");
                Ret = TW_RET_FAILED;
        }
        return Ret;
}
//验证磁盘阵列
int RaidMgr::StartVerifyUnit(int unit_id)
{
        int Ret = TW_RET_SUCCESS;
        tw_unit_id_type tw_unit_id;
        tw_unit_mode_type unit_mode;
        Ret = GetUnitIdFromUnitNum(unit_id, tw_unit_id);
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_mode(tw_unit_id, &unit_mode);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit_mode");
                }
        }
        
        if(Ret == TW_RET_SUCCESS && unit_mode == TW_UNIT_MODE_OK)
        {
                Ret = tw_verify_unit(tw_unit_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Verify Unit Error");
                }
        }
        return Ret;
}  

//停止磁盘阵列验证
int RaidMgr::StopVerifyUnit(int unit_id)
{
        int Ret = TW_RET_SUCCESS;
        tw_unit_id_type tw_unit_id;
        Ret = GetUnitIdFromUnitNum(unit_id,tw_unit_id);
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_stop_verify_unit(tw_unit_id);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Stop Verify Unit Error");
                }
        }
        return Ret;
}  

//重建磁盘阵列
int RaidMgr::RebuildUinit(int unit_id, int* drive_id_list, const int driver_num, const int raidlvl)
{
        int Ret = TW_RET_SUCCESS;
        tw_unit_id_type tw_unit_id;        
        tw_unit_mode_type mode;
        tw_drive_id_list_type tw_drive_id_list;        
        
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        tw_id_base_type                 controller_number; 
        Ret = tw_get_controller_id_list(&controller_id_list);
        if(Ret == TW_RET_SUCCESS)
        {
                controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);
                Ret = tw_get_controller_number(*controller_id_ptr,&controller_number);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get controller number");
                }
                
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }
        
        //Raid0 不可重建
        if(raidlvl == RAID_0)
        {
                Ret = TW_RET_FAILED;
                LOG_ERR("Cannot Rebuild Raid0");
        }
        if(Ret == TW_RET_SUCCESS)
        {
                if((raidlvl == RAID_1 || raidlvl == RAID_5) && driver_num != 1)
                {
                        Ret = TW_RET_FAILED;
                        LOG_ERR("Cannot rebuild more than 1 drive for Raid0/Raid5");
                }
                
                if(raidlvl == RAID_6 && driver_num > 2)
                {
                        LOG_ERR("Cannot rebuild more than 2 drive for Raid5");
                        Ret = TW_RET_FAILED;
                }
        }
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = GetUnitIdFromUnitNum(unit_id, tw_unit_id);
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_unit_mode(tw_unit_id, &mode);
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                if(mode != TW_UNIT_MODE_DEGRADED)
                {
                        LOG_ERR("Unit status must be degraed");
                        Ret = TW_RET_FAILED;
                }
        }
        else
        {
                LOG_ERR("Failed to get unit status");
        }
        
        //PD是否可用
        if(Ret == TW_RET_SUCCESS)
        {
                tw_drive_id_list.length = driver_num;
                for(int i = 0; i < driver_num; i++)
                {
                        tw_drive_id_type        drive_id;
                        Ret = tw_create_drive_id(controller_number, drive_id_list[i], &drive_id);
                        if(Ret != TW_RET_SUCCESS)
                        {
                                LOG_ERR("Failed to create drive_id");
                                break;
                        }
                        else
                        {
                                memcpy (&(tw_drive_id_list.list[i]), &drive_id, TW_API_ID_SIZE);
                        }
                        
                        if(Ret == TW_RET_SUCCESS)
                        {
                                Ret = IsPDAvailable(drive_id, controller_number);
                                if(Ret != TW_RET_SUCCESS)
                                {
                                        LOG_ERR("PD is not available");
                                        break;
                                }
                        }          
                }
                
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_rebuild_unit(tw_unit_id, &tw_drive_id_list, TW_CONTINUE_ON_ECC_ERRORS);
        }
        if(Ret != TW_RET_SUCCESS)
        {
                LOG_ERR("Rebuild unit error");
        }
        return Ret;
}  

//删除磁盘阵列
int RaidMgr::DelUnit(int unit_id)
{
        int Ret = TW_RET_SUCCESS;
        tw_unit_id_type tw_unit_id;        
        //bool IsBoot = false;
        
        char cmd[256] = {0};
        sprintf(cmd, "%s%s chkdelraid %d",MAIN_PATH,PL_RAID,unit_id);         
        
        std::string result;        
        Popen(cmd, result);
        if(strstr(result.c_str(),"false") != NULL)
        {
                return -2;
        }
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = GetUnitIdFromUnitNum(unit_id, tw_unit_id);
        }   
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_destroy_unit(tw_unit_id, TW_NOTIFY_OS_OF_DESTROYED_UNIT);
                
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to destroy unit");
                }
                /*char cmd[256] = {0};
                sprintf(cmd,"tw_cli /c%d/u%d del quiet 2>&1",controller_number,unit_id);
                FILE    *file   = popen(cmd, "r");
                if (file == NULL)
                {
                        LOG_ERR("tw_cli del unit error ");
                        Ret = TW_RET_FAILED;
                }*/
        }
        return Ret;
}

int RaidMgr::IsBootUnit(tw_unit_id_type tw_unit_id, bool *isBoot)
{
        int Ret = TW_RET_SUCCESS;
        tw_configuration_type           configuration;
        Ret = tw_get_unit_configuration(tw_unit_id, &configuration);
        
        if(Ret != TW_RET_SUCCESS)
        {
                LOG_ERR("Failed to get raid level");
        }
        if(Ret == TW_RET_SUCCESS)
        {
                if(!(configuration >= TW_UNIT_CONFIG_RAID_0 && configuration <= TW_UNIT_CONFIG_RAID_50))
                {
                       *isBoot =  true;
                }
        }
        
        return Ret;
}

//磁盘阵列扩容
int RaidMgr::MigrateUnit(int Raidlvl, int* drive_id_list, int driver_num, int stripe_size, int group_number, int unit_id)
{
        int Ret = TW_RET_SUCCESS;
        int NewRaidConfig = 0;
        tw_drive_id_list_type tw_drive_id_list;
        tw_unit_id_type       tw_unit_id;
        
        tw_controller_id_list_type      controller_id_list;
        tw_controller_id_type           *controller_id_ptr;
        tw_id_base_type                 controller_number; 
        Ret = tw_get_controller_id_list(&controller_id_list);
        
        char lvlstr[8];
        memset(lvlstr, 0, 8);
                
        char diskstr[32];
        memset(diskstr, 0, 32);

        char cmd[256];
        memset(cmd, 0, 256);
        if(Raidlvl == SINGLE)
        {
                Ret = TW_RET_FAILED;
                LOG_ERR("Cannot migrate OS unit");
        }
        if(Ret == TW_RET_SUCCESS)
        {
                controller_id_ptr = (tw_controller_id_type *) 
                                     &(controller_id_list.list[TW_FIRST_CONTROLLER_INDEX]);
                Ret = tw_get_controller_number(*controller_id_ptr,&controller_number);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get controller number");
                }
                
        }
        else
        {
                LOG_ERR("No Controller(s) found");
        }
        
        if(Raidlvl == RAID_1)
        {
                LOG_ERR("Cannot migrate Raid1");
                Ret = TW_RET_FAILED;
        }    
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = GetUnitIdFromUnitNum(unit_id, tw_unit_id);
        }
                    
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = tw_get_drive_id_list_from_unit_id(tw_unit_id, &tw_drive_id_list);
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Failed to get unit drivers");
                }
        }
        if(Ret == TW_RET_SUCCESS)
        {        
                if(Raidlvl == RAID_50)
                {
                        if(!(group_number != 0 && (driver_num + tw_drive_id_list.length) % group_number == 0))
                        {
                                LOG_ERR("Invalid group number for Raid50");
                                Ret = TW_RET_FAILED;
                        }
                }
                
                if(Raidlvl == RAID_6 && stripe_size != 64)
                {
                        LOG_ERR("Stripe size if not 64k for Raid6");
                        Ret = TW_RET_FAILED;
                }
        }
        //新加硬盘是否有效
        if(Ret == TW_RET_SUCCESS)
        {
                int oldlen = tw_drive_id_list.length;
                tw_drive_id_list.length += driver_num;
                for(int i = 0; i < driver_num; i++)
                {
                        tw_drive_id_type        drive_id;
                        Ret = tw_create_drive_id(controller_number, drive_id_list[i], &drive_id);
                        if(Ret != TW_RET_SUCCESS)
                        {
                                LOG_ERR("Failed to create drive_id");
                                break;
                        }
                        else
                        {
                                memcpy(&(tw_drive_id_list.list[i + oldlen]), &drive_id, TW_API_ID_SIZE);
                        }
                        
                        if(Ret == TW_RET_SUCCESS)
                        {
                                Ret = IsPDAvailable(drive_id, controller_number);
                                if(Ret != TW_RET_SUCCESS)
                                {
                                        LOG_ERR("PD is not available");
                                        break;
                                }
                        }                                
                }
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                Ret = RaidLvlToStr(Raidlvl, lvlstr);
        }
        
        if(Ret == TW_RET_SUCCESS)
        {
                if(NewRaidConfig != TW_UNIT_CONFIG_RAID_50)
                {
                        group_number = TW_UNIT_CONFIG_RAID_50_G_UNUSED;
                }
                /*Ret = tw_migrate_unit(tw_unit_id,
                                        &tw_drive_id_list,
                                        NewRaidConfig,
                                        ConvertStripeSize(stripe_size),
                                        TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT,
                                        group_number);*/
                for(int i = 0; i < driver_num; i++)
                {
                        char tmp[4];
                        memset(tmp, 0, 4);
		        sprintf(tmp,"%d:",drive_id_list[i]);		        
		        strcat(diskstr,tmp);
                }
                diskstr[strlen(diskstr)-1] = '\0';
                sprintf(cmd,"tw_cli /c%d/u%d migrate type=%s disk=%s stripe=%d 2>&1",
                        controller_number,unit_id, lvlstr,diskstr,stripe_size);
                FILE    *file   = popen(cmd, "r");
                if (file == NULL)
                {
                        LOG_ERR("tw_cli is error ");
                        Ret = TW_RET_FAILED;
                }
                
                if(Ret != TW_RET_SUCCESS)
                {
                        LOG_ERR("Migrate unit error");
                }
        }
        
        return Ret;        
}
