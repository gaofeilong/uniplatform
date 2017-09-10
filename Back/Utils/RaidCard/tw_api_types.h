/**
 * $Id: tw_api_types.h,v 1.140 2010/08/10 01:19:12 sbukhari Exp $
 *
 * 3ware controller API
 *
 * http://www.3ware.com
 *
 *
 * Copyright (c) 2004, 2005 Applied Micro Circuits Corporation (AMCC).
 *   All rights reserved.
 * Copyright (c) 2002, 2003, 2004 3ware, Inc.  All rights reserved.
 *
 * This source code is AMCC confidential information and may not be
 * used without an express written license agreement from AMCC.
 * This source code may not be distributed or provided in any way to
 * a third party without the express written consent of AMCC.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef    _TW_API_TYPES_H
#define    _TW_API_TYPES_H

#include "tw_api_defines.h"


#ifdef    __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 *
 *  Data Structures
 *
 */

#if defined (WIN32)
typedef unsigned __int64    tw_64_bit_type;
#else
typedef unsigned long long  tw_64_bit_type;
#endif   /* (WIN32) */

typedef unsigned char tw_id_base_type; 

typedef struct tw_id_type_tag
{
  tw_id_base_type id[TW_API_ID_SIZE];
} tw_id_type;

typedef struct tw_id_list_type_tag
{
  tw_id_type  list[TW_MAX_API_ID_LIST_LENGTH];   /* ID List               */
  int         length;                            /* List length (in IDs)  */
} tw_id_list_type;


typedef char tw_a_chip_version_type[TW_MAX_A_CHIP_VERSION_LEN]; 

typedef int tw_add_drive_policy_type; 

typedef struct tw_api_error_type_tag
{
  int  valid_error_flags;      /* Which error codes are valid             */
  int  api_error_code;         /* If valid, code for API error            */
  int  os_error_code;          /* If valid, code for OS error             */
  int  driver_error_code;      /* If valid, code for driver error         */
  int  controller_error_code;  /* If valid, code for controller error     */
  char additional_data[TW_MAX_ERROR_DATA_LEN];   /* Controller error data */
  int  text_length;            /* The length of the text (not including   */
                               /*   the terminating \0)                   */
  char text[TW_MAX_ERROR_TEXT_LEN];
} tw_api_error_type;

typedef struct tw_alarm_id_list_type_tag
{
  tw_id_type  list[TW_MAX_ALARMS ];   /* ID List               */
  int         length;                 /* List length (in IDs)  */
} tw_alarm_id_list_type;

typedef tw_id_type tw_alarm_id_type; 

typedef unsigned int tw_alarm_status_type; 

typedef int tw_architecture_id_type;

typedef int tw_auto_spare_policy_type; 

typedef unsigned char tw_avmode_state_type;

typedef int tw_background_flag_type;

typedef int tw_background_scrub_state_type; 

typedef int tw_background_task_rate_type; 

typedef int tw_background_verify_state_type; 

typedef int tw_bbu_is_present_type;

typedef int tw_bbu_battery_is_present_type;

typedef int tw_bbu_status_type;

typedef int tw_bbu_battery_voltage_status_type;

typedef int tw_bbu_battery_temperature_status_type;

typedef int tw_bbu_battery_test_in_progress_type;

typedef int tw_bbu_battery_charge_in_progress_type;

typedef char tw_bbu_serial_number_type[TW_MAX_BBU_SERIAL_NUMBER_LEN]; 

typedef char tw_bbu_firmware_version_type[TW_MAX_BBU_FIRMWARE_VERSION_LEN]; 

typedef char tw_bbu_boot_code_version_type[TW_MAX_BBU_BOOT_CODE_VERSION_LEN]; 

typedef int  tw_bbu_pcb_revision_type;

typedef char tw_bbu_factory_data_type[TW_MAX_BBU_FACTORY_DATA_LEN]; 

typedef char tw_bbu_date_type[TW_MAX_BBU_DATE_LEN]; 

typedef int  tw_bbu_battery_capacity_type;

typedef unsigned char  tw_bbu_battery_charge_voltage_type;

typedef unsigned short  tw_bbu_battery_voltage_type;

typedef int  tw_bbu_status_summary_type;

typedef int  tw_bbu_enabled_type;

typedef char tw_bios_version_type[TW_MAX_BIOS_VERSION_LEN]; 

typedef tw_64_bit_type tw_capacity_type; 

typedef int tw_configuration_type; 

//typedef tw_id_list_type tw_controller_id_list_type; 
typedef struct tw_controller_id_list_type_tag
{
  tw_id_type  list[TW_MAX_CONTROLLERS];   /* ID List               */
  int         length;                     /* List length (in IDs)  */
} tw_controller_id_list_type;


typedef tw_id_type tw_controller_id_type; 

typedef char tw_diagnostic_log_type[TW_MAX_DIAGNOSTIC_LOG_LEN]; 

typedef struct tw_door_lock_id_list_type_tag
{
  tw_id_type  list[TW_MAX_DOOR_LOCKS];   /* ID List               */
  int         length;                     /* List length (in IDs)  */
} tw_door_lock_id_list_type;

typedef tw_id_type tw_door_lock_id_type; 

typedef unsigned int tw_door_lock_status_type; 

typedef unsigned char tw_drive_flags_type;

typedef unsigned char tw_drive_link_type;

typedef unsigned char tw_drive_queue_mode_type;

typedef tw_id_list_type tw_drive_slot_id_list_type; 

typedef tw_id_type tw_drive_slot_id_type; 

typedef unsigned int tw_drive_slot_status_type; 

typedef int tw_drive_degrade_policy_type; 

typedef tw_id_list_type tw_drive_id_list_type; 

typedef tw_id_type tw_drive_id_type; 

typedef unsigned char tw_drive_status_type; 

typedef char tw_driver_version_type[TW_MAX_DRIVER_VERSION_LEN]; 

typedef struct tw_enclosure_id_list_type_tag
{
  tw_id_type  list[TW_MAX_ENCLOSURES];    /* ID List               */
  int         length;                     /* List length (in IDs)  */
} tw_enclosure_id_list_type;

typedef tw_id_type tw_enclosure_id_type; 

typedef unsigned char tw_enclosure_protocol_type; 

typedef unsigned char tw_auto_rebuild_policy_type;

typedef unsigned char tw_enclosure_configuration_type[TW_MAX_EPCT_LENGTH];

typedef unsigned int tw_enclosure_status_type;

#pragma pack(1)
typedef struct tw_event_info_type_tag
{
  unsigned int   sequence_id;
  unsigned int   time_stamp_sec;
  unsigned short aen_code;
  unsigned char  severity;
  unsigned char  reserved0;
  unsigned char  reserved1;
  unsigned char  parameter_len;
  unsigned char  parameter_data[TW_MAX_AEN_DATA_LEN];
  unsigned char  text_len;
  char           text[TW_MAX_AEN_TEXT_LEN];
} tw_event_info_type;
#pragma pack()

typedef unsigned int tw_event_sequence_type;

//----------------------------------------------------------------
//Start of logging Application Event to controller AEN queue.
//----------------------------------------------------------------

#pragma pack(1)
typedef struct tw_scsi_sense_data_type_tag
{
  unsigned char      valid_code;
  unsigned char      reserved_0;
  unsigned char      sense_key;          
  unsigned int       information;
  unsigned char      add_sense_len;
  unsigned int       cmd_specific_info;
  unsigned char      as_code;              
  unsigned char      asc_qualifier;
  unsigned char      fru_code;
  unsigned char      skey_specific[3];
} tw_scsi_sense_data_type;

typedef struct tw_create_event_type_tag
{
  unsigned short           aen_code;           // AEN code
  unsigned char            severity;           // Severity levels, 1- Error, 2- Warning, 3- Info, 4- Debug
  unsigned char            source;             // 0- 3dm, 1- Enclosure Monitor, 2- CLI, 3- Scout, 4- 3rd Party
  unsigned int             src_attr;           // Source attributes; An enclosure number, if source = Enclosure Monitor.
  unsigned int             reserved_0;
  unsigned int             reserved_1;
  //short or a parameter string in ASCII, NULL terminated.
  // example: "enclosure=0, logicalID=0x50A1B2C3D4E5F6G7 fan=2"
  char                     param_data[TW_MAX_AEN_DATA_LEN]; 
  unsigned char            param_len;        
  //Description of an AEN in ASCII, NULL terminated.
  // example: "Enclosure fan failed".
  char                     desc[TW_MAX_AEN_TEXT_LEN];        
  unsigned char            desc_len;
  unsigned char            sense_data[TW_AEN_SCSI_SENSE_DATA_SIZE];         // 18 bytes scsi sense data, Response code = 0x71
} tw_create_event_type;
#pragma pack()

//----------------------------------------------------------------
//End of logging Application Event to controller AEN queue.
//----------------------------------------------------------------

typedef struct tw_fan_id_list_type_tag
{
  tw_id_type  list[TW_MAX_FANS];    /* ID List               */
  int         length;               /* List length (in IDs)  */
} tw_fan_id_list_type;


typedef tw_id_type tw_fan_id_type; 

typedef unsigned char tw_fan_status_type; 

typedef struct tw_temp_sensor_id_list_type_tag
{
  tw_id_type  list[TW_MAX_TEMP_SENSORS ];   /* ID List               */
  int         length;                       /* List length (in IDs)  */
} tw_temp_sensor_id_list_type;

typedef tw_id_type tw_temp_sensor_id_type; 

typedef int tw_temperature_type; 

typedef unsigned char tw_temp_sensor_status_type;

typedef unsigned char tw_temp_threshold_status_type; 

typedef struct tw_power_supply_id_list_type_tag
{
  tw_id_type  list[TW_MAX_POWER_SUPPLYS ];   /* ID List               */
  int         length;                        /* List length (in IDs)  */
} tw_power_supply_id_list_type;
 

typedef tw_id_type tw_power_supply_id_type;

typedef unsigned char tw_power_supply_status_type;

typedef unsigned char tw_power_threshold_status_type;

typedef char tw_firmware_version_type[TW_MAX_FIRMWARE_VERSION_LEN]; 

typedef struct tw_ident_data_type_tag
{
  char ident_data[TW_MAX_DRIVE_IDENT_DATA_LEN];
} tw_ident_data_type; 

typedef unsigned char tw_identify_status_type;

typedef unsigned int tw_memory_size_type;

typedef char tw_model_type[TW_MAX_MODEL_NUMBER_LEN]; 

typedef char tw_monitor_version_type[TW_MAX_MONITOR_VERSION_LEN]; 

typedef int tw_n_hour_check_type; 

typedef tw_id_base_type tw_local_unit_id_type; 

typedef char tw_p_chip_version_type[TW_MAX_P_CHIP_VERSION_LEN]; 

typedef char tw_pcb_revision_type[TW_MAX_PCB_REVISION_LEN]; 

typedef int tw_percentage_complete_type; 

typedef tw_id_list_type tw_port_id_list_type; 

typedef tw_id_list_type tw_phy_id_list_type; 

typedef tw_id_type tw_port_id_type; 

typedef tw_id_type tw_phy_id_type; 

typedef struct tw_sata_link_speed_tag
{
    unsigned char           supported;
    unsigned char           enabled;
} tw_sata_link_speed_type;

typedef char tw_serial_number_type[TW_MAX_SERIAL_NUMBER_LEN]; 

typedef struct tw_smart_data_type_tag
{
  char smart_data[TW_MAX_DRIVE_SMART_DATA_LEN];
} tw_smart_data_type; 

typedef int tw_spinups_allowed_type; 

typedef int tw_stagger_time_type;

typedef int tw_rollcall_pause_type; 

typedef unsigned int tw_firmware_srl_type; 

typedef unsigned int tw_amplitude_type; 

typedef unsigned int tw_preemphasis_type;

typedef unsigned char tw_interface_type;

typedef unsigned char tw_storsave_type;

typedef tw_id_list_type tw_subunit_id_list_type; 

typedef unsigned char tw_schedule_state_type; 

typedef unsigned short tw_task_item_type;

typedef unsigned short tw_selftest_enable_flags;

typedef unsigned short tw_selftest_supported;

typedef struct tw_schedule_1_item_type_tag
{
  tw_task_item_type start_time;
  tw_task_item_type duration;
} tw_schedule_1_item_type; 

typedef struct tw_schedule_2_item_type_tag
{
  tw_task_item_type start_time;
} tw_schedule_2_item_type; 

typedef tw_schedule_1_item_type tw_rebuild_schedule_type;

typedef tw_schedule_2_item_type tw_selftest_schedule_type;

typedef tw_schedule_1_item_type tw_verify_schedule_type;

typedef struct tw_day_schedule_type_tag
{
  tw_selftest_schedule_type   selftest;
  tw_rebuild_schedule_type    rebuild;
  tw_verify_schedule_type     verify;
} tw_day_schedule_type;

typedef tw_day_schedule_type tw_task_schedule_type[TW_TOTAL_NUMBER_OF_TASKS];

typedef unsigned short tw_preferred_time_type;

typedef unsigned char tw_unit_cache_state_type; 

typedef unsigned char tw_unit_cache_policy_type;

typedef unsigned char tw_ignore_bbu_not_ready_type;

typedef unsigned short tw_unit_standby_time_type;

typedef unsigned char tw_unit_standby_mode_type;

typedef tw_id_list_type tw_unit_id_list_type; 

typedef tw_id_list_type tw_parity_id_list_type; 

typedef tw_id_list_type tw_lun_id_list_type; 

typedef tw_id_type tw_unit_id_type; 

typedef tw_id_type tw_parity_id_type;

typedef tw_id_type tw_lun_id_type; 

typedef unsigned char tw_unit_mode_type;

typedef char tw_unit_name_type[TW_MAX_UNIT_NAME_LEN]; 

typedef char tw_unit_serial_number_type[TW_MAX_UNIT_SERIAL_NUMBER_LEN]; 

typedef unsigned char tw_unit_status_type;

typedef unsigned char tw_unit_queue_mode_type;

typedef int tw_stripe_size_type;

typedef unsigned char tw_initialized_state_type;

typedef unsigned char tw_bgtask_active_type;

typedef unsigned char tw_policy_state_type;

typedef unsigned char tw_rapid_recovery_control_type;

typedef unsigned char tw_write_dpo_policy_type;

typedef unsigned char tw_btap_policy_type;

typedef struct tw_api_compatibility_tag
{
    unsigned int low;
    unsigned int hi;
} tw_api_compatibility_type;


typedef unsigned short tw_target_command_queue_type[TW_TARGET_COMMAND_QUEUE_SIZE];
typedef struct tw_command_queue_tag
{
  unsigned short host_queue;
  tw_target_command_queue_type target_queue;
} tw_command_queue_type;

typedef unsigned int tw_hba_bus_type;


#define TW_UPDATE_VOTE_NO           0x0
#define TW_UPDATE_VOTE_YES          0x1
#define TW_UPDATE_VOTE_RECOMMENDED  0x2

#pragma pack(1)
typedef struct tw_update_vote_tag
{
    struct
    {
        unsigned char           vote;
        char                    cur_ver[TW_MAX_FIRMWARE_VERSION_LEN];
        
    } api;
    struct
    {
        unsigned char           vote;
        char                    cur_ver[TW_MAX_DRIVER_VERSION_LEN];
    } driver;
    struct
    {
        unsigned char           vote;
        char                    cur_ver[TW_MAX_FIRMWARE_VERSION_LEN];
        char                    new_ver[TW_MAX_FIRMWARE_VERSION_LEN];
    } firmware;
} tw_update_vote_type;
#pragma pack()

#pragma pack(1)
typedef  struct _tw_enclosure_compatibility_type
{
  unsigned char     comp;
  char              curr_ver[TW_MAX_FIRMWARE_VERSION_LEN];
  char              new_ver[TW_MAX_FIRMWARE_VERSION_LEN];
} tw_enclosure_compatibility_type;
#pragma pack()

#pragma pack(1)
typedef  struct _tw_enclosure_fw_version_type
{
  unsigned char  sep_count;
  struct 
  {
    unsigned char  sep_num;
    char           fw_version[TW_MAX_FIRMWARE_VERSION_LEN];
  }sep_info[TW_MAX_SEPS];
} tw_enclosure_fw_version_type;
#pragma pack()


#pragma pack(1)
typedef  struct _tw_drive_compatibility_type
{
  unsigned char     comp;
  char              curr_ver[TW_MAX_FIRMWARE_VERSION_LEN];
  char              new_ver[TW_MAX_FIRMWARE_VERSION_LEN];
} tw_drive_compatibility_type;
#pragma pack()

typedef  char  tw_drive_fw_version_type[TW_MAX_FIRMWARE_VERSION_LEN];


typedef tw_64_bit_type tw_lun_size_type;

typedef tw_64_bit_type tw_feature_bitmap_type;

typedef unsigned char tw_lba_reorder_state_type;

typedef unsigned char tw_ssc_state_type;

typedef unsigned char tw_spinup_state_type;

typedef unsigned int tw_coercion_factor_type;

typedef unsigned char tw_modepage_control_type;

typedef unsigned int  tw_pcie_signal_type;

typedef unsigned char tw_drive_interface_type;

typedef unsigned char tw_sas_address_type[8];

typedef unsigned char tw_sas_wwn_type[8];

typedef unsigned char tw_enclosure_logical_identifier_type[8];

typedef char tw_enclosure_serial_number_type[254];

typedef unsigned char tw_device_type_type;

typedef unsigned char tw_phy_link_type;

typedef unsigned char tw_port_to_drivehandle_map_type[255];

typedef char tw_enclosure_vendor_type[TW_MAX_ENCL_VENDOR_NAME_LEN+1];  //+1 for null character

typedef char tw_enclosure_product_type[TW_MAX_ENCL_PRODUCT_NAME_LEN+1]; //+1 for null character

typedef char tw_enclosure_product_revision_type[TW_MAX_ENCL_PRODUCT_REVISION_LEN+1]; //+1 for null character

typedef unsigned char tw_fan_speed_type;

typedef unsigned char tw_alarm_tone_type;

typedef unsigned char tw_mute_state_type;

typedef unsigned char tw_onoff_state_type;

typedef unsigned short tw_board_id_type;

typedef unsigned char tw_oem_id_type;

//==============================================================================
//  Controller active-passive failover feature
//==============================================================================

typedef unsigned char tw_auto_export_policy_type;
typedef unsigned short tw_halt_timer_type;
typedef unsigned char  tw_pci_reset_policy_type;

//==============================================================================
//Performance Monitoring and statistics data type
//==============================================================================
// NOTE: This is a Firmware Requirement:
// Data structure members cannot span natural boundaries.
// (i.e. 16 bit value must be on a 16 bit boundary)

typedef unsigned char tw_pmstat_state_type;

// specifies trigger information
typedef struct  tw_pmstat_trigger_info_tag
{
    unsigned char    trig_type;              // TW_PMSTAT_TRIGGER_ value(s)
    unsigned char    cap_mode;               // TW_PMSTAT_CAPMODE_ value

    unsigned short   reserved;               // align to 32 bits before union

    union // following valid only if trigtype is not _TRIGGER_START:
    {
        struct                               // for TW_PMSTAT_TRIGGER_OPCODE:
        {
            unsigned char   cmd_type;        // TW_PMSTAT_CMDTYPE_ value
            unsigned char   op_type;         // TW_PMSTAT_OPTYPE_ value(s)
            unsigned short  opcode;          // opcode to match (if _OPTYPE_ONE bit)
        } trigcmd;

        unsigned short      resp_time;       // response time to exceed (msecs)
    } trigval;                               // value used to trigger capture
} tw_pmstat_trigger_info;

 
// performance monitoring supported configuration.
typedef struct tw_pmstat_config_tag
{
   unsigned char    version;         // performance monitoring interface version
   unsigned char    status;          // performance monitoring running status
   unsigned short   capabilities;    // capabilities
   tw_pmstat_trigger_info trig_info; // what triggered data capture (optional).
                                     // This is valid if not in running state.
   unsigned short   maxcmds_run;     // max number of cmds that can be captured for
                                     // instantaneous performance statistics
   unsigned short   maxcmds_ex;      // max number of higher response time cmds that can be saved
   unsigned int     time_units;      // Units of time in nano-seconds
   unsigned int     time_inc;        // smallest measured increment of time available.
   unsigned int     boot_time;       // local time stamp at offset=0 (secs since 1/1/1970)
                                     // (optinal). Inavalid if zero
} tw_pmstat_config_type;


// performance monitoring control setting.
typedef struct tw_pmstat_control_tag
{
    unsigned char    do_clear;       // clear summary and long command data
    unsigned char    avg_factor;     // factor used in average calculations
                                     // (so 80 is 0.8 etc, and 0 is default)
    unsigned char    cmd_type;       // type of command
    unsigned char    op_type;        // type of opcode
    unsigned short   opcode;         // opcode to match (if _OPTYPE_OPCODE bit)
    unsigned char    reserved;
    unsigned char    run_mode;       // TW_PMSTAT_RUNMODE_XXX values
    tw_pmstat_trigger_info trig_info;  // valid only if runmode is _TRIGGER
    unsigned short   cmd_count;      // number of commands to save, Zero to MAX
                                     // if not 0 restric capture to:
    unsigned char    target;         // SCSI target ID (+1)
    unsigned char    lun;            // SCSI LUN (+1)
                         
} tw_pmstat_control_type;


// performance monitoring running averages and response histogram.
typedef struct tw_pmstat_summary_tag
{
   unsigned int     iops;            // I/O operations
   unsigned int     transfer_rate;   // in KB per sec
   unsigned int     resp_time;       // response time (in time_units of config structure)
   unsigned int     queue_depth;     // Commands outstanding
} tw_pmstat_summary_type;

typedef struct tw_pmstat_resp_histogram_tag
{
   unsigned int     resp_histogram[TW_PMSTAT_HIST_NUM_BINS_TOTAL];     // response histogram
}tw_pmstat_resp_histogram;

/*
// performance monitoring instantaneous statistics.
typedef struct tw_pmstat_instant_tag
{
   unsigned short   iops;            // I/O operations
   float            transfer_rate;   // in KB per sec
   float            resp_time;       // response time (in time_units of config structure)
   unsigned short   queue_depth;     // Commands outstanding
} tw_pmstat_instant_type;
*/


typedef struct tw_pmstat_instant_tag
{
   unsigned int   iops;            // I/O operations
   unsigned int   transfer_rate;   // in KB per sec
   unsigned int   resp_time;       // response time (in time_units of config structure)
   unsigned int   queue_depth;     // Commands outstanding
} tw_pmstat_instant_type;


// command details for commands with higher response time 
typedef struct tw_pmstat_high_rs_tag 
{
   unsigned char     status;         // command status (TW_PMSTAT_CMD_STATUS bits)
   unsigned char     target;         // SCSI target ID
   unsigned char     lun;            // SCSI LUN
   unsigned char     cmd_type;       // command type
   unsigned short    opcode;         // command opcode to match
   unsigned short    posted;         // posted command count
   unsigned int      data_len;       // data byte count
   unsigned int      tstart;         // time command started (in time_units of config structure)
   unsigned int      tend;           // time command completed (in time_units of config structure)
   unsigned int      local_time;     // timestamp of the command
   unsigned char     cdb[16];        // SCSI CDB or ATA task file register values     
}tw_pmstat_high_rs_type;

typedef tw_pmstat_high_rs_type tw_pmstat_high_rs_array_type[TW_MAX_PMSTAT_COMMANDS];

// Extended drive statistics
typedef struct tw_pmstat_extd_drive_statistics_type_tag 
{
   unsigned int      sectors_read;      // Cumulative number of sectors read from the drive since the last controller reset or pmstat clear.
   unsigned int      sectors_written;   // Cumulative number of sectors written to the drive since the last controller reset or pmstat clear.
                                        //    This includes all writes(with and without FUA set)

   unsigned int      sectors_written_with_fua;  // Cumulative number of sectors written with FUA to the drive since the last controller reset or pmstat clear.
   unsigned int      read_cmds;      // Cumulative number of read commands to the drive since the last controller reset or pmstat clear.
   unsigned int      write_cmds;     // Cumulative number of write commands to the drive since the last controller reset or pmstat clear.
                                     //    This includes all write commands(with and without FUA set)

   unsigned int      write_cmds_with_fua;  // Cumulative number of write commands with FUA to the drive since the last controller reset or pmstat clear.
   unsigned int      flush_cmds;     // Cumulative number of sychronize cache or ATA Flush commands to the drive since the last controller reset or pmstat clear.

}tw_pmstat_extd_drive_statistics_type;


//==============================================================================
// Event Capture data type
//==============================================================================
typedef unsigned int  tw_event_capture_mode_type;
typedef unsigned int  tw_event_capture_enable_type;
typedef unsigned int  tw_event_capture_control_type;


//==============================================================================
// Event Trigger data type
//==============================================================================
typedef unsigned short tw_diagnostic_log_trigger_count_type;
typedef unsigned short tw_diagnostic_reset_count_type;
typedef unsigned short tw_diagnostic_assert_count_type;
typedef unsigned short tw_diagnostic_drive_error_count_type;


#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif  /* _TW_API_TYPES_H */

