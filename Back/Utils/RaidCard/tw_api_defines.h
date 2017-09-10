/**
 * $Id: tw_api_defines.h,v 1.246 2010/07/21 20:22:40 sbukhari Exp $
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
#ifndef    _TW_API_DEFINES_H
#define    _TW_API_DEFINES_H

#ifdef    __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 *Pre-Processor Definitions
 *
 */

#define TW_API_5000_ARCHITECTURE                       0x0001
#define TW_API_6000_ARCHITECTURE                       0x0002
#define TW_API_7000_ARCHITECTURE                       0x0003
#define TW_API_8000_ARCHITECTURE                       0x0203
#define TW_API_9000_ARCHITECTURE                       0x0004
#define TW_API_9500_ARCHITECTURE                       0x0005
#define TW_API_9550_ARCHITECTURE                       0x0006
#define TW_API_9650_ARCHITECTURE                       0x0007  
#define TW_API_9690_ARCHITECTURE                       0x0008   
#define TW_API_NVRAM_ARCHITECTURE                      0x0009   //NVRAM controller for data domain
#define TW_API_9750_ARCHITECTURE                       0x000A   //Olompali 
#define TW_API_UNKNOWN_ARCHITECTURE                        -1

#define TW_MAX_DRIVER_VERSION_LEN                         128    
#define TW_MAX_AEN_DATA_LEN                                98
#define TW_MAX_AEN_TEXT_LEN                               256

#define AEN_EVENT_SEVERITY_ERROR                          0x1
#define AEN_EVENT_SEVERITY_WARNING                        0x2
#define AEN_EVENT_SEVERITY_INFO                           0x3
#define AEN_EVENT_SEVERITY_DEBUG                          0x4


#define TW_NULL_ID                                       0xFF

#define TW_MAX_CONTROLLERS                                255
#define TW_FIRST_CONTROLLER_INDEX                           0
#define TW_LAST_CONTROLLER_INDEX                       (TW_MAX_CONTROLLERS - 1)
#define TW_NULL_CONTROLLER_INDEX                       TW_NULL_ID
#define TW_NULL_CONTROLLER                             TW_NULL_ID


#define TW_MAX_PORTS                                       128
#define TW_FIRST_PORT                                       0
#define TW_LAST_PORT_INDEX                             (TW_MAX_PORTS - 1)
#define TW_NULL_PORT                                   TW_NULL_ID


#define TW_MAX_PHYS                                       128
#define TW_FIRST_PHY                                       0
#define TW_LAST_PHY_INDEX                             (TW_MAX_PHYS - 1)
#define TW_NULL_PHY                                   TW_NULL_ID


#define TW_MAX_UNITS                                        64  //32
#define TW_FIRST_UNIT                                  TW_FIRST_PORT
#define TW_LAST_UNIT_INDEX                             (TW_MAX_UNITS - 1)
#define TW_NULL_UNIT                                   TW_NULL_PORT

#define TW_MAX_SUBUNITS                                TW_MAX_UNITS
#define TW_FIRST_SUBUNIT                               TW_FIRST_UNIT
#define TW_LAST_SUBUNIT_INDEX                          (TW_MAX_SUBUNITS - 1)
#define TW_NULL_SUBUNIT                                TW_NULL_UNIT

#define TW_MAX_DRIVES                                  TW_MAX_PORTS
#define TW_FIRST_DRIVE                                 TW_FIRST_PORT
#define TW_LAST_DRIVE_INDEX                            (TW_MAX_DRIVES - 1)
#define TW_NULL_DRIVE                                  TW_NULL_PORT

//These defines applies to 8000 series controller only
#define TW_8000_MAX_PORTS                                  16
#define TW_8000_LAST_PORT_INDEX                        (TW_8000_MAX_PORTS - 1)

#define TW_8000_MAX_UNITS                                  16
#define TW_8000_LAST_UNIT_INDEX                        (TW_8000_MAX_UNITS - 1)

#define TW_8000_MAX_SUBUNITS                           TW_8000_MAX_UNITS
#define TW_8000_LAST_SUBUNIT_INDEX                     (TW_8000_MAX_SUBUNITS - 1)

#define TW_8000_MAX_DRIVES                             TW_8000_MAX_PORTS
#define TW_8000_LAST_DRIVE_INDEX                       (TW_8000_MAX_DRIVES - 1)


#define TW_MAX_ENCLOSURES                                  16
#define TW_FIRST_ENCLOSURE                                  0
#define TW_LAST_ENCLOSURE_INDEX                        (TW_MAX_ENCLOSURES - 1)
#define TW_NULL_ENCLOSURE                              TW_NULL_ID

#define TW_MAX_FANS                                        16
#define TW_FIRST_FAN                                        0
#define TW_LAST_FAN_INDEX                              (TW_MAX_FANS - 1)
#define TW_NULL_FAN                                    TW_NULL_ID

#define TW_MAX_DOOR_LOCKS                                  16
#define TW_FIRST_DOOR_LOCK                                  0
#define TW_LAST_DOOR_LOCK_INDEX                        (TW_MAX_DOOR_LOCKS - 1)
#define TW_NULL_DOOR_LOCK                              TW_NULL_ID

#define TW_MAX_ALARMS                                      16
#define TW_FIRST_ALARM                                      0
#define TW_LAST_ALARM_INDEX                            (TW_MAX_ALARMS - 1)
#define TW_NULL_ALARM                                  TW_NULL_ID


#define TW_MAX_DRIVE_SLOTS                             TW_MAX_DRIVES
#define TW_FIRST_DRIVE_SLOT                                 0
#define TW_LAST_DRIVE_SLOT_INDEX                       (TW_MAX_DRIVE_SLOTS - 1)
#define TW_NULL_DRIVE_SLOT                             TW_NULL_ID

#define TW_MAX_TEMP_SENSORS                                32
#define TW_FIRST_TEMP_SENSOR                                0
#define TW_LAST_TEMP_SENSOR_INDEX                      (TW_MAX_TEMP_SENSORS - 1)
#define TW_NULL_TEMP_SENSOR                            TW_NULL_ID

#define TW_MAX_POWER_SUPPLYS                               16
#define TW_FIRST_POWER_SUPPLY                               0
#define TW_LAST_POWER_SUPPLY_INDEX                      (TW_MAX_POWER_SUPPLYS - 1)
#define TW_NULL_POWER_SUPPLY                            TW_NULL_ID

#define TW_ENCLOSURE_MAX_CDB_LEN                           16

#define TW_MAX_LUNS                                        64
#define TW_FIRST_LUN                                        0
#define TW_LAST_LUN_INDEX                              (TW_MAX_LUNS - 1)
#define TW_NULL_LUN                                    TW_NULL_ID

  
#define TW_MAX_PARITIES                                     2
#define TW_FIRST_PARITY                                TW_FIRST_UNIT
#define TW_SECOND_PARITY                               (TW_FIRST_PARITY +1)
#define TW_LAST_PARITY_INDEX                           (TW_MAX_PARITIES - 1)
#define TW_NULL_PARITY                                 TW_NULL_UNIT
  
#define TW_API_ID_SIZE                                      8
#define TW_MAX_API_ID_LIST_LENGTH                      TW_MAX_PORTS

#define TW_MAX_A_CHIP_VERSION_LEN                          32    
#define TW_MAX_ARCHITECTURE_ID_LEN                         32    
#define TW_MAX_BIOS_VERSION_LEN                            32    
#define TW_MAX_FIRMWARE_VERSION_LEN                        32    
#define TW_MAX_MODEL_NUMBER_LEN                            64   
#define TW_MAX_MONITOR_VERSION_LEN                         32    
#define TW_MAX_P_CHIP_VERSION_LEN                          32    
#define TW_MAX_PCB_REVISION_LEN                            32    
#define TW_MAX_SERIAL_NUMBER_LEN                           32    

#define TW_MAX_SEPS                                         4

#define TW_BACKGROUND_TASK_RATE_UNKNOWN                    -1
#define TW_BACKGROUND_TASK_RATE_LOW                         1
#define TW_BACKGROUND_TASK_RATE_MED_LOW                     2 
#define TW_BACKGROUND_TASK_RATE_MEDIUM                      3 
#define TW_BACKGROUND_TASK_RATE_MED_HI                      4
#define TW_BACKGROUND_TASK_RATE_HIGH                        5
#define TW_MIN_BACKGROUND_TASK_RATE      TW_BACKGROUND_TASK_RATE_LOW
#define TW_MAX_BACKGROUND_TASK_RATE      TW_BACKGROUND_TASK_RATE_HIGH

#define TW_TASK_CONTROL_UNKNOWN                            -1
#define TW_TASK_CONTROL_DISABLED                            0
#define TW_TASK_CONTROL_SCHEDULE                            1
#define TW_TASK_CONTROL_DRIVER_LOAD                         2

#define TW_TASK_0                                           0
#define TW_TASK_1                                           1
#define TW_TASK_2                                           2
#define TW_TASK_3                                           3
#define TW_TASK_4                                           4
#define TW_TASK_5                                           5
#define TW_TASK_6                                           6
#define TW_TOTAL_NUMBER_OF_TASKS                            7
#define TW_TEST_FIRST_TASK                          TW_TASK_0
#define TW_TEST_LAST_TASK                           TW_TASK_6

#define TW_SUNDAY                                   TW_TASK_0
#define TW_MONDAY                                   TW_TASK_1
#define TW_TUESDAY                                  TW_TASK_2
#define TW_WEDNESDAY                                TW_TASK_3
#define TW_THURSDAY                                 TW_TASK_4
#define TW_FRIDAY                                   TW_TASK_5
#define TW_SATURDAY                                 TW_TASK_6
#define TW_DAYS_IN_WEEK              TW_TOTAL_NUMBER_OF_TASKS
#define TW_FIRST_DAY_OF_WEEK               TW_TEST_FIRST_TASK
#define TW_LAST_DAY_OF_WEEK                TW_TEST_LAST_TASK


#define TW_SELFTEST_ENABLE_UPGRADE_DMA_MODE            0x0001
#define TW_SELFTEST_DISABLE_UPGRADE_DMA_MODE           0xFFFE
#define TW_SELFTEST_ENABLE_CHECK_SMART_THRESHOLD       0x0002
#define TW_SELFTEST_DISABLE_CHECK_SMART_THRESHOLD      0xFFFD

#define TW_MIN_TEST_START_TIME                              0
#define TW_MAX_TEST_START_TIME                          10065
#define TW_MIN_TEST_DURATION                                1
#define TW_MAX_TEST_DURATION                            10080

#define TW_MIN_PREFERRED_START_TIME                         0
#define TW_MAX_PREFERRED_START_TIME                     10080

#define TW_MIN_SPINUPS_ALLOWED                              1
#define TW_MAX_SPINUPS_ALLOWED                         TW_MAX_PORTS
#define TW_ALL_SPINUPS_ALLOWED                  TW_MAX_SPINUPS_ALLOWED

#define TW_MIN_STAGGER_TIME                                 0 /* Seconds */
#define TW_MAX_STAGGER_TIME                                60 /* Seconds */
#define TW_NO_STAGGER_TIME                TW_MIN_STAGGER_TIME /* Seconds */

#define TW_MIN_ROLLCALL_PAUSE_TIME                          2 /* Seconds */
#define TW_MAX_ROLLCALL_PAUSE_TIME                         30 /* Seconds */

#define TW_MIN_N_HOUR_CHECK                                 1 /* Hours        */
#define TW_MAX_N_HOUR_CHECK                              8760 /* Hours  (1 yr)*/
#define TW_NO_N_HOUR_CHECK                                  0

#define TW_MIN_DIAGNOSTIC_LOG_LEN                           1
#define TW_MAX_DIAGNOSTIC_LOG_LEN                        32768      //Changed to 32K prev 8192    

#define TW_HALT_ON_ECC_ERRORS                            0x00
#define TW_CONTINUE_ON_ECC_ERRORS                        0x01

#define TW_UNIT_NOT_INITIALIZED                             0
#define TW_UNIT_IS_INITIALIZED                              1

#define TW_BGTASK_HALTED                                    0
#define TW_BGTASK_ACTIVE                                    1

#define TW_POLICY_DISABLED                                  0
#define TW_POLICY_ENABLED                                   1
#define TW_POLICY_AUTOMATIC                                 2

#define TW_STORSAVE_UNKNOWN                                ~0
#define TW_STORSAVE_PROTECTION                              0
#define TW_STORSAVE_BALANCE                                 1
#define TW_STORSAVE_PERFORMANCE                             2

#define TW_POLICY_AUTO_REBUILD_OFF                       0x00
#define TW_POLICY_AUTO_REBUILD_CBOD                      0x01
#define TW_POLICY_AUTO_REBUILD_JBOD                      0x02
#define TW_POLICY_AUTO_REBUILD_FAILED_DISK               0x04
#define TW_POLICY_AUTO_REBUILD_ALL                       0xFF

#define TW_LBA_REORDER_DISABLED                     TW_POLICY_DISABLED
#define TW_LBA_REORDER_ENABLED                      TW_POLICY_ENABLED

#define TW_SSC_DISABLED                     TW_POLICY_DISABLED
#define TW_SSC_ENABLED                      TW_POLICY_ENABLED

#define TW_AUTO_SPINUP_ENABLED                              0
#define TW_AUTO_SPINUP_DISABLED                             1

#define TW_MIN_COERCION_FACTOR                              1
#define TW_MAX_COERCION_FACTOR                            100

#define TW_WRITE_DPO_IGNORE                                 0
#define TW_WRITE_DPO_FORCE                                  1
#define TW_WRITE_DPO_HONOR_HOST                             2

#define TW_RAPID_RECOVERY_DISABLE                           0
#define TW_RAPID_RECOVERY_REBUILD                           1
#define TW_RAPID_RECOVERY_REBUILD_AND_UNCLEAN_SHUTDOWN      3


#define TW_AUTO_EXPORT_POLICY_AT_POWER_ON                 0x01
#define TW_FORCE_HALT_TIMER_MIN                           0x00
#define TW_FORCE_HALT_TIMER_MAX                           0xFFFE
#define TW_PCI_RESET_POLICY_AS_HARD_RESET                   0
#define TW_PCI_RESET_POLICY_AS_SOFT_RESET                   1
#define TW_PCI_RESET_POLICY_USE_DEFAULT                   0xFF
#define TW_AUTO_CLEAN_TIMER_MIN                             1
#define TW_AUTO_CLEAN_TIMER_MAX                            255




#define TW_ENCLOSURE_PROTOCOL_SAFTE                         0
#define TW_ENCLOSURE_PROTOCOL_SES                           1
#define TW_MAX_EPCT_LENGTH                               1024

#define TW_MAX_ENCL_VENDOR_NAME_LEN                         8
#define TW_MAX_ENCL_PRODUCT_NAME_LEN                       16
#define TW_MAX_ENCL_PRODUCT_REVISION_LEN                    4
#define TW_MAX_ENCL_LOGICAL_ID_LEN                          8


#define TW_MAX_BBU_SERIAL_NUMBER_LEN                       32    
#define TW_MAX_BBU_FIRMWARE_VERSION_LEN                    32    
#define TW_MAX_BBU_BOOT_CODE_VERSION_LEN                   32    
#define TW_MAX_BBU_FACTORY_DATA_LEN                        64    
#define TW_MAX_BBU_DATE_LEN                                16    
#define TW_MAX_BBU_STATUS_SUMMARY_LEN                      32    

#define TW_BBU_IS_ENABLED                            TW_POLICY_ENABLED
#define TW_BBU_IS_DISABLED                           TW_POLICY_DISABLED
#define TW_NOT_PRESENT                                      0
#define TW_PRESENT                                          1
#define TW_BBU_IS_NOT_PRESENT                        TW_NOT_PRESENT
#define TW_BBU_IS_PRESENT                            TW_PRESENT
#define TW_BBU_BATTERY_IS_NOT_PRESENT                TW_NOT_PRESENT
#define TW_BBU_BATTERY_IS_PRESENT                    TW_PRESENT

#define TW_BBU_STATUS_UNKNOWN                              -1
#define TW_BBU_STATUS_NOT_READY                           0x0
#define TW_BBU_STATUS_READY                               0x8

#define TW_BBU_SUMMARY_UNKNOWN                             -1
#define TW_BBU_SUMMARY_OK                                   0
#define TW_BBU_SUMMARY_BBU_NOT_PRESENT                      1
#define TW_BBU_SUMMARY_NO_BATTERY                           2
#define TW_BBU_SUMMARY_TESTING                              3
#define TW_BBU_SUMMARY_CHARGING                             4
#define TW_BBU_SUMMARY_FAULT                                5
#define TW_BBU_SUMMARY_ERROR                                6
#define TW_BBU_SUMMARY_WEAK_BATTERY                         7
#define TW_BBU_SUMMARY_FAILED_BATTERY                       8
#define TW_BBU_SUMMARY_DISABLED                             9
#define TW_MIN_BBU_SUMMARY                      TW_BBU_SUMMARY_OK
#define TW_MAX_BBU_SUMMARY                      TW_BBU_SUMMARY_DISABLED

#define TW_BBU_BATTERY_IS_NOT_TESTING                       0
#define TW_BBU_BATTERY_IS_TESTING                           1
#define TW_BBU_BATTERY_IS_NOT_CHARGING                      0
#define TW_BBU_BATTERY_IS_CHARGING                          1

#define TW_BBU_BATTERY_TEMPERATURE_UNKNOWN                 -1
#define TW_BBU_BATTERY_TEMPERATURE_NORMAL                   0
#define TW_BBU_BATTERY_TEMPERATURE_LOW                      1 
#define TW_BBU_BATTERY_TEMPERATURE_HIGH                     2 
#define TW_BBU_BATTERY_TEMPERATURE_TOO_LOW                  3
#define TW_BBU_BATTERY_TEMPERATURE_TOO_HIGH                 4
#define TW_MIN_BBU_BATTERY_TEMPERATURE      TW_BBU_BATTERY_TEMPERATURE_NORMAL
#define TW_MAX_BBU_BATTERY_TEMPERATURE      TW_BBU_BATTERY_TEMPERATURE_TOO_HIGH

#define TW_BBU_BATTERY_VOLTAGE_UNKNOWN                     -1
#define TW_BBU_BATTERY_VOLTAGE_NORMAL                       0
#define TW_BBU_BATTERY_VOLTAGE_LOW                          1 
#define TW_BBU_BATTERY_VOLTAGE_HIGH                         2 
#define TW_BBU_BATTERY_VOLTAGE_TOO_LOW                      3
#define TW_BBU_BATTERY_VOLTAGE_TOO_HIGH                     4
#define TW_MIN_BBU_BATTERY_VOLTAGE          TW_BBU_BATTERY_VOLTAGE_NORMAL
#define TW_MAX_BBU_BATTERY_VOLTAGE          TW_BBU_BATTERY_VOLTAGE_TOO_HIGH

#define TW_BBU_BATTERY_VOLTAGE_LOW_CHARGE                   0
#define TW_BBU_BATTERY_VOLTAGE_FULL_CHARGE                  1


#define TW_MAX_UNIT_STATUS_LEN                             64
#define TW_MAX_UNIT_CONFIG_LEN                             32
#define TW_MAX_UNIT_NAME_LEN                               21
#define TW_MAX_UNIT_SERIAL_NUMBER_LEN                      21

#define TW_UNIT_MODE_OK                                  0xFF
#define TW_UNIT_MODE_VERIFYING                           0xFB
#define TW_UNIT_MODE_INITIALIZING                        0xF7
#define TW_UNIT_MODE_DEGRADED                            0xEF
#define TW_UNIT_MODE_REBUILDING                          0xDF
#define TW_UNIT_MODE_RECOVERY                            0xBF
#define TW_UNIT_MODE_MIGRATING                           0xAF
#define TW_UNIT_MODE_INOPERABLE                          0x00
#define TW_UNIT_MODE_UNKNOWN                             0x11

#define TW_UNIT_STATUS_NOT_PRESENT                       0x00
#define TW_UNIT_STATUS_FLAG_ONLINE                       0x01
#define TW_UNIT_STATUS_FLAG_COMPLETE                     0x02
#define TW_UNIT_STATUS_FLAG_DRIVES_OK                    0x04
#define TW_UNIT_STATUS_FLAG_UNUSED_BIT_3                 0x08
#define TW_UNIT_STATUS_FLAG_UNUSED_BIT_4                 0x10
#define TW_UNIT_STATUS_FLAG_UNUSED_BIT_5                 0x20
#define TW_UNIT_STATUS_FLAG_UNUSED_BIT_6                 0x40
#define TW_UNIT_STATUS_FLAG_ALIVE                        0x80
#define TW_UNIT_STATUS_OK          ( TW_UNIT_STATUS_FLAG_ONLINE   | \
                                     TW_UNIT_STATUS_FLAG_COMPLETE | \
                                     TW_UNIT_STATUS_FLAG_ALIVE        )
#define TW_UNIT_STATUS_UNKNOWN                           0x7F

#define TW_MIN_UNIT_PERCENTAGE_COMPLETE                     1  /* tenths of % */
#define TW_MAX_UNIT_PERCENTAGE_COMPLETE                  1000  /* tenths of % */
#define TW_MAX_UNIT_DONE                                    0  /* tenths of % */

/**
 * The RAID Subunit configuration below is obsolete.
 * Use CBOD in its place.
 */
#define TW_UNIT_CONFIG_UNKNOWN                             -1
#define TW_UNIT_CONFIG_RAID_0                            0x00
#define TW_UNIT_CONFIG_RAID_1                            0x01
#define TW_UNIT_CONFIG_TWINSTOR                          0x02
#define TW_UNIT_CONFIG_RAID_5                            0x05
#define TW_UNIT_CONFIG_RAID_10                           0x06
#define TW_UNIT_CONFIG_RAID_6                            0x07
#define TW_UNIT_CONFIG_RAID_50                           0x08
#define TW_UNIT_CONFIG_ADP                               0x0A
#define TW_UNIT_CONFIG_IBOD                              0x0B
#define TW_UNIT_CONFIG_CBOD                              0x0C
#define TW_UNIT_CONFIG_SPARE                             0x0D
#define TW_UNIT_CONFIG_SBOD              TW_UNIT_CONFIG_SPARE
#define TW_UNIT_CONFIG_RAID_SUBUNIT                      0x0E /* Obsolete */
#define TW_UNIT_CONFIG_JBOD                              0x0F
#define TW_UNIT_CONFIG_MIGRATOR                          0x20
#define TW_UNIT_CONFIG_RAID_6_EXTN                       0x30
#define TW_UNIT_CONFIG_DEAD                              0x40
#define TW_UNIT_CONFIG_LUN                               0x80

/* 16 drive RAID 50 grouping choices: */
#define TW_UNIT_CONFIG_RAID_50_G_UNUSED                     0
#define TW_UNIT_CONFIG_RAID_50_G0                           0
#define TW_UNIT_CONFIG_RAID_50_G3                           3
#define TW_UNIT_CONFIG_RAID_50_G4                           4
#define TW_UNIT_CONFIG_RAID_50_G5                           5
#define TW_UNIT_CONFIG_RAID_50_G6                           6
#define TW_UNIT_CONFIG_RAID_50_G7                           7
#define TW_UNIT_CONFIG_RAID_50_G8                           8
#define TW_UNIT_CONFIG_RAID_50_G9                           9
#define TW_UNIT_CONFIG_RAID_50_G10                          10
#define TW_UNIT_CONFIG_RAID_50_G11                          11
#define TW_UNIT_CONFIG_RAID_50_G12                          12
#define TW_UNIT_CONFIG_RAID_50_G13                          13
#define TW_UNIT_CONFIG_RAID_50_G14                          14
#define TW_UNIT_CONFIG_RAID_50_G15                          15
#define TW_UNIT_CONFIG_RAID_50_G16                          16


#define TW_UNIT_STRIPE_SIZE_UNKNOWN                        -1
#define TW_UNIT_STRIPE_SIZE_UNUSED                          0
#define TW_UNIT_STRIPE_SIZE_8K                              4
#define TW_UNIT_STRIPE_SIZE_16K                             5
#define TW_UNIT_STRIPE_SIZE_32K                             6
#define TW_UNIT_STRIPE_SIZE_64K                             7
#define TW_UNIT_STRIPE_SIZE_128K                            8
#define TW_UNIT_STRIPE_SIZE_256K                            9
#define TW_UNIT_STRIPE_SIZE_512K                           10
#define TW_UNIT_STRIPE_SIZE_1024K                          11

#define TW_UNIT_STRIPE_SIZE_FIRST                   TW_UNIT_STRIPE_SIZE_8K
#define TW_UNIT_STRIPE_SIZE_LAST                    TW_UNIT_STRIPE_SIZE_1024K
#define TW_UNIT_STRIPE_SIZE_DEFAULT                 TW_UNIT_STRIPE_SIZE_64K 
#define TW_UNIT_STRIPE_SIZE_NONE                    TW_UNIT_STRIPE_SIZE_UNUSED

#define TW_UNIT_CACHE_STATE_DISABLED                        0
#define TW_UNIT_CACHE_STATE_ENABLED                         1
#define TW_UNIT_CACHE_ENABLED_WITH_READ_AHEAD               2
#define TW_UNIT_CACHE_STATE_UNKNOWN                        -1
#define TW_UNIT_CACHE_DISABLED                      TW_UNIT_CACHE_STATE_DISABLED
#define TW_UNIT_CACHE_ENABLED                       TW_UNIT_CACHE_STATE_ENABLED

#define TW_FOLLOW_BBU_NOT_READY                             0
#define TW_IGNORE_BBU_NOT_READY                             1


#define TW_UNIT_READ_CACHE_DISABLED                         0
#define TW_UNIT_READ_CACHE_BASIC                            1
#define TW_UNIT_READ_CACHE_INTELLIGENT                      2


#define TW_UNIT_STANDBY_TIME_OFF                            0
#define TW_UNIT_STANDBY_TIME_MINIMUM                        5
#define TW_UNIT_STANDBY_TIME_MAXIMUM                      999


#define TW_DRIVE_STATUS_OK                               0xFF
#define TW_DRIVE_STATUS_UNSUPPORTED                      0xFE
#define TW_DRIVE_STATUS_ECC_ERROR                        0xF9
#define TW_DRIVE_STATUS_POR_ERROR                        0xFB
#define TW_DRIVE_STATUS_SMART_FAILURE                    0xF5
#define TW_DRIVE_STATUS_DEVICE_ERROR                     0xF1
#define TW_DRIVE_STATUS_DCB_READ_TIMEOUT                 0xEE
#define TW_DRIVE_STATUS_DCB_READ_FAILURE                 0xEC
#define TW_DRIVE_STATUS_CONFIG_OPERATION_FAILURE         0xE8
#define TW_DRIVE_STATUS_DCB_ORPHAN                       0xDF
#define TW_DRIVE_STATUS_DCB_DATA_CHECK                   0xDD
#define TW_DRIVE_STATUS_UNSUPPORTED_DCB                  0xDB
#define TW_DRIVE_STATUS_UNCONVERTED_DCB                  0xD9
#define TW_DRIVE_STATUS_REMOVED                          0x02
#define TW_DRIVE_STATUS_OFFLINE_JBOD                     0x01
#define TW_DRIVE_STATUS_OFFLINE                          0x00
#define TW_DRIVE_STATUS_UNKNOWN                          0x10
#define TW_DRIVE_DETECTED_FLAG                           0x80
#define TW_DRIVE_FUNCTIONAL_FLAG                         0x01

#define TW_DRIVE_CONFIG_CBOD                    TW_UNIT_CONFIG_CBOD
#define TW_DRIVE_CONFIG_IBOD                    TW_UNIT_CONFIG_IBOD
#define TW_DRIVE_CONFIG_JBOD                    TW_UNIT_CONFIG_JBOD
#define TW_DRIVE_CONFIG_SBOD                    TW_UNIT_CONFIG_SBOD
#define TW_DRIVE_CONFIG_SPARE                   TW_UNIT_CONFIG_SPARE
#define TW_DRIVE_CONFIG_UNKNOWN                            -1

#define TW_DRIVE_FLAGS_PART_OF_EXPORTABLE_UNIT           0x01

#define TW_FEATRUE_SUPPORTED_RAID5                       0x0000000000000001
#define TW_FEATRUE_SUPPORTED_RAID6                       0x0000000000000002

#define TW_DO_NOT_NOTIFY_OS                                 0
#define TW_NOTIFY_OS                                        1
#define TW_DO_NOT_NOTIFY_OS_OF_NEW_UNIT         TW_DO_NOT_NOTIFY_OS
#define TW_NOTIFY_OS_OF_NEW_UNIT                TW_NOTIFY_OS
#define TW_DO_NOT_NOTIFY_OS_OF_DESTROYED_UNIT   TW_DO_NOT_NOTIFY_OS
#define TW_NOTIFY_OS_OF_DESTROYED_UNIT          TW_NOTIFY_OS
#define TW_DO_NOT_NOTIFY_OS_OF_IMPORTED_UNIT    TW_DO_NOT_NOTIFY_OS
#define TW_NOTIFY_OS_OF_IMPORTED_UNIT           TW_NOTIFY_OS
#define TW_DO_NOT_NOTIFY_OS_OF_EXPORTED_UNIT    TW_DO_NOT_NOTIFY_OS
#define TW_NOTIFY_OS_OF_EXPORTED_UNIT           TW_NOTIFY_OS

#define TW_MAX_DRIVE_STATUS_LEN                            64
#define TW_MAX_DRIVE_MODEL_LEN                             64
#define TW_MAX_DRIVE_SERIAL_LEN                            32
#define TW_MAX_DRIVE_FIRMWARE_LEN                          32
#define TW_MAX_DRIVE_SMART_STATUS_LEN                     256 
#define TW_MAX_DRIVE_DATA_LEN                             512
#define TW_MAX_DRIVE_SMART_DATA_LEN             TW_MAX_DRIVE_DATA_LEN
#define TW_MAX_DRIVE_IDENT_DATA_LEN             TW_MAX_DRIVE_DATA_LEN
#define TW_ATA_READ                                         0
#define TW_ATA_WRITE                                        1

//Enclosure firmware download defines
#define TW_DOWNLOAD_MICROCODE_NOT_COMPATIBLE             0x00
#define TW_DOWNLOAD_MICROCODE_VERSION_NEW                0x01
#define TW_DOWNLOAD_MICROCODE_VERSION_OLD                0x02
#define TW_DOWNLOAD_MICROCODE_VERSION_SAME               0x03
#define TW_DOWNLOAD_MICROCODE_VERSION_NOT_KNOWN          0x80


// ATA passthrough flags
#define TW_ATA_FLAGS_WRITE_THRU                          0x01
#define TW_ATA_FLAGS_EXT_ATA_CMD                         0x02

// ATA passthrough transfer modes
#define TW_ATA_PASSTHRU_NO_DATA                          0x08
#define TW_ATA_PASSTHRU_DMA_READ                         0x09
#define TW_ATA_PASSTHRU_DMA_WRITE                        0x0B
#define TW_ATA_PASSTHRU_PIO_READ                         0x0D
#define TW_ATA_PASSTHRU_PIO_WRITE                        0x0F


/*
 *  Supported ATA passthrough commands
 *
 *  a) Limited to 256 sectors.
 */
#define TW_ATA_NOP                               0x00    //
#define TW_ATA_READ_SECTOR                       0x20    //
#define TW_ATA_READ_SECTOR_EXT                   0x24    // a
#define TW_ATA_READ_DMA_EXT                      0x25    // a
#define TW_ATA_READ_NATIVE_MAX_ADDRESS_EXT       0x27    //
#define TW_ATA_READ_STREAM_DMA                   0x2A    //
#define TW_ATA_READ_STREAM_PIO                   0x2B    //
#define TW_ATA_READ_LOG_EXT                      0x2F    //
#define TW_ATA_SET_MAX_ADDRESS_EXT               0x37    //
#define TW_ATA_WRITE_LOG_EXT                     0x3F    //
#define TW_ATA_READ_VERIFY_SECTOR                0x40    //
#define TW_ATA_READ_VERIFY_SECTOR_EXT            0x42    // 
#define TW_ATA_SEEK                              0x70    //
#define TW_ATA_DOWNLOAD_MICROCOE                 0x92    //
#define TW_ATA_SMART                             0xB0    //
#define TW_ATA_READ_DMA                          0xC8    //
#define TW_ATA_IDLE_IMMEDIATE                    0xE1    //
#define TW_ATA_IDLE                              0xE3    //
#define TW_ATA_READ_BUFFER                       0xE4    //
#define TW_ATA_CHECK_POWER_MODE                  0xE5    //
#define TW_ATA_FLUSH_CACHE                       0xE7    //
#define TW_ATA_WRITE_BUFFER                      0xE8    //
#define TW_ATA_FLUSH_CACHE_EXT                   0xEA    //
#define TW_ATA_IDENTIFY_DEVICE                   0xEC    //
#define TW_ATA_SECURITY_FREEZE_LOCK              0xF5    //
#define TW_ATA_READ_NATIVE_MAX_ADDRESS           0xF8    //
/*
 *  Supported ATA SMART passthrough features
 *
 */
#define TW_ATA_SMART_READ_DATA                   0xD0    //
#define TW_ATA_SMART_GET_THRESHOLD               0xD1    //
#define TW_ATA_SMART_ENABLE_DISABLE_AUTOSAVE     0xD2    //
#define TW_ATA_SMART_EXECUTE_OFFLINE_IMMEDIATE   0xD4    //
#define TW_ATA_SMART_READ_LOG                    0xD5    //
#define TW_ATA_SMART_WRITE_LOG                   0xD6    //
#define TW_ATA_SMART_ENABLE_OPERATIONS           0xD8    //
#define TW_ATA_SMART_RETURN_STATUS               0xDA    //


// SCSI passthrough transfer modes
#define TW_SCSI_PASSTHRU_NON_DATA                0x80
#define TW_SCSI_PASSTHRU_WRITE_TO_DRIVE          0x81
#define TW_SCSI_PASSTHRU_READ_FROM_DRIVE         0x82
#define TW_SCSI_PASSTHRU_BIDIRECTIONAL_DATA      0x83

// SCSI passthrough flags
#define TW_SCSI_FLAGS_NONE                       0x00


/* Supported SCSI passthrough opcodes */
#define TW_SCSI_INQUIRY                             0x12
#define TW_SCSI_LOG_SENSE                           0x4D
#define TW_SCSI_MODE_SENSE_6                        0x1A 
#define TW_SCSI_MODE_SENSE_10                       0x5A
#define TW_SCSI_READ_6                              0x08
#define TW_SCSI_READ_10                             0x28
#define TW_SCSI_READ_12                             0xA8
#define TW_SCSI_READ_16                             0x88
#define TW_SCSI_READ_BUFFER                         0x3C
#define TW_SCSI_READ_CAPACITY_10                    0x25
#define TW_SCSI_READ_CAPACITY_16                    0x9E  //opcode(9Eh), service action(10h) 
#define TW_SCSI_READ_DEFECT_DATA_10                 0x37 
#define TW_SCSI_READ_DEFECT_DATA_12                 0xB7 
#define TW_SCSI_READ_MEDIA_SERIAL_NUMBER            0xAB  //opcode(ABh), service action(01h)
#define TW_SCSI_RECEIVE_DIAGNOSTIC_RESULTS          0x1C
#define TW_SCSI_REPORT_ALIASES                      0xA3  //opcode(A3h), service action(0Bh)
#define TW_SCSI_REPORT_DEVICE_IDENTIFIER            0xA3  //opcode(A3h), service action(05h)
#define TW_SCSI_REPORT_LUNS                         0xA0
#define TW_SCSI_REPORT_PRIORITY                     0xA3  //opcode(A3h), service action(0Bh)
#define TW_SCSI_REPORT_SUPPORTED_OPERATION_CODES    0xA3  //opcode(A3h), service action(0Ch)
#define TW_SCSI_REPORT_SUPPORTED_TASK_MANAGEMENT_FUNCTIONS  0xA3  //opcode(A3h), service action(0Dh)
#define TW_SCSI_REPORT_TARGET_PORT_GROUPS           0xA3  //opcode(A3h), service action(0Ah)
#define TW_SCSI_REPORT_TIMESTAMP                    0xA3  //opcode(A3h), service action(0Fh)
#define TW_SCSI_TEST_UNIT_READY                     0x00
#define TW_SCSI_VERIFY_10                           0x2F
#define TW_SCSI_VERIFY_12                           0xAF
#define TW_SCSI_VERIFY_16                           0x8F
#define TW_SCSI_REQUEST_SENSE                       0x03
#define TW_SCSI_SEND_DIAGNOSTIC                     0x1D
#define TW_SCSI_WRITE_BUFFER                        0x3B


#define TW_CDB_SIZE_6                         6
#define TW_CDB_SIZE_10                       10
#define TW_CDB_SIZE_12                       12
#define TW_CDB_SIZE_16                       16

#define TW_RET_SUCCESS                                      0
#define TW_RET_FAILED                                      -1

#define TW_FALSE                                            0
#define TW_TRUE                                             1

#define TW_OFF                                              0
#define TW_ON                                               1
#define TW_UNKNOWN                                       0x80

#define TW_NOT_VALID                                        0
#define TW_VALID                                            1

#define TW_ERROR_SOURCE_NONE                              0x0
#define TW_ERROR_SOURCE_API                               0x1
#define TW_ERROR_SOURCE_OS                                0x2
#define TW_ERROR_SOURCE_DRIVER                            0x4
#define TW_ERROR_SOURCE_CONTROLLER                        0x8

#define TW_ERROR_SOURCE_FIRST             TW_ERROR_SOURCE_API
#define TW_ERROR_SOURCE_LAST              TW_ERROR_SOURCE_CONTROLLER

#define TW_ERROR_SOURCE_7000                          0x10000
#define TW_ERROR_SOURCE_9000                          0x20000

#define TW_ERROR_SOURCE_7000_DRIVER      ( TW_ERROR_SOURCE_7000 | \
                                           TW_ERROR_SOURCE_DRIVER   ) 
#define TW_ERROR_SOURCE_7000_CONTROLLER  ( TW_ERROR_SOURCE_7000 |     \
                                           TW_ERROR_SOURCE_CONTROLLER   ) 
#define TW_ERROR_SOURCE_9000_DRIVER      ( TW_ERROR_SOURCE_9000 | \
                                           TW_ERROR_SOURCE_DRIVER   ) 
#define TW_ERROR_SOURCE_9000_CONTROLLER  ( TW_ERROR_SOURCE_9000 |     \
                                           TW_ERROR_SOURCE_CONTROLLER   ) 

#define TW_MAX_ERROR_TEXT_LEN                            1024
#define TW_MAX_ERROR_DATA_LEN                             128

#define TW_ERROR_NONE                                       0
#define TW_ERROR_NO_CONTROLLER                              1
#define TW_ERROR_FEATURE_NOT_SUPPORTED                      2
#define TW_ERROR_CONTROLLER_NOT_SUPPORTED                   3
#define TW_ERROR_UNUSED_4                                   4
#define TW_ERROR_INVALID_DRIVER_CONTROL_CODE                5
#define TW_ERROR_INVALID_FIRMWARE_OPCODE                    6
#define TW_ERROR_MEMORY_ALLOC_FAILED                        7
#define TW_ERROR_INVALID_CONTROLLER_ID                      8
#define TW_ERROR_INVALID_UNIT_ID                            9
#define TW_ERROR_INVALID_DRIVE_ID                          10
#define TW_ERROR_INVALID_PORT_ID                           11
#define TW_ERROR_DRIVER_NOT_LOADED                         12
#define TW_ERROR_RETRIEVING_UNIT_DESCRIPTOR                13
#define TW_ERROR_UNUSED_14                                 14
#define TW_ERROR_UNUSED_15                                 15
#define TW_ERROR_FAILED_TO_GET_UNIT_STATUS                 16
#define TW_ERROR_FAILED_TO_GET_PERCENTAGE_COMPLETE         17
#define TW_ERROR_FAILED_TO_GET_DRIVE_FROM_UNIT_ID          18
#define TW_ERROR_FAILED_TO_GET_DRIVE_LIST_FROM_UNIT_ID     19
#define TW_ERROR_FAILED_TO_REMOVE_DRIVE                    20
#define TW_ERROR_FAILED_TO_NOTIFY_OS_OF_UNIT_CHANGE        21
#define TW_ERROR_FAILED_TO_GET_UNIT_CAPACITY               22
#define TW_ERROR_ADD_PORT_IN_USE                           23
#define TW_ERROR_SCAN_INVALID_PORT                         24
#define TW_ERROR_CREATE_CONFIG_NOT_SUPPORTED               25
#define TW_ERROR_CREATE_INVALID_NUM_DRIVES                 26
#define TW_ERROR_CREATE_INVALID_STRIPE                     27
#define TW_ERROR_CREATE_DRIVES_NOT_FROM_SAME_CONTROLLER    28
#define TW_ERROR_CREATE_DUPLICATE_DRIVE                    29
#define TW_ERROR_CREATE_PORT_EMPTY                         30
#define TW_ERROR_CREATE_DRIVE_IN_USE                       31
#define TW_ERROR_CREATE_DRIVE_ERROR                        32
#define TW_ERROR_CREATE_INVALID_CONFIG                     33
#define TW_ERROR_CREATE_FAILED_TO_GET_UNIT_ID              34
#define TW_ERROR_CREATE_INVALID_GROUPING                   35
#define TW_ERROR_IMPORT_NO_DRIVES_SPECIFIED                36
#define TW_ERROR_IMPORT_CANT_IMPORT_MULTIPLE_DRIVES        37
#define TW_ERROR_IMPORT_DRIVES_NOT_FROM_SAME_CONTROLLER    38
#define TW_ERROR_IMPORT_PORT_EMPTY                         39
#define TW_ERROR_IMPORT_DRIVE_ERROR                        40
#define TW_ERROR_IMPORT_DRIVE_IN_USE                       41
#define TW_ERROR_WRITE_CACHE_NOT_SUPPORTED                 42
#define TW_ERROR_UNIT_STATUS_NO_UNIT                       43
#define TW_ERROR_UNIT_HAS_NO_SUBUNITS                      44
#define TW_ERROR_INVALID_TASK_RATE                         45
#define TW_ERROR_REMOVE_PORT_EMPTY                         46
#define TW_ERROR_REMOVE_PORT_BUSY                          47
#define TW_ERROR_REMOVE_NOT_REDUNDANT                      48
#define TW_ERROR_REBUILD_UNIT_OK                           49
#define TW_ERROR_REBUILD_UNIT_REBUILDING                   50
#define TW_ERROR_REBUILD_UNIT_BUSY                         51
#define TW_ERROR_REBUILD_NOT_REDUNDANT                     52
#define TW_ERROR_REBUILD_REPLACEMENT_NOT_CBOD_SPARE        53
#define TW_ERROR_REBUILD_REPLACEMENT_TOO_SMALL             54
#define TW_ERROR_PERCENTAGE_INVALID_CONFIG                 55
#define TW_ERROR_REMOVE_UNIT_VERIFYING                     56
#define TW_ERROR_SPINUPS_INVALID_PARAM                     57
#define TW_ERROR_STAGGER_INVALID_PARAM                     58
#define TW_ERROR_DELETE_NO_UNIT                            59
#define TW_ERROR_STRIPE_INVALID_CONFIG                     60
#define TW_ERROR_DRIVE_ID_LIST_NO_UNIT                     61
#define TW_ERROR_UNIT_ID_PORT_EMPTY                        62
#define TW_ERROR_UNIT_DESCRIPTOR_NO_UNIT                   63
#define TW_ERROR_REBUILD_UNIT_NOT_REBUILDING               64
#define TW_ERROR_IMPORT_DUPLICATE_DRIVE                    65
#define TW_ERROR_COMPATABILITY_TOO_LOW                     66
#define TW_ERROR_COMPATABILITY_TOO_HIGH                    67
#define TW_ERROR_VERIFY_UNIT_BUSY                          68
#define TW_ERROR_FAILED_TO_REMOVE_DEVICE_NODE_FROM_OS      69
#define TW_ERROR_FAILED_TO_ADD_DEVICE_NODE_TO_OS           70
#define TW_ERROR_DRIVER_FIRMWARE_INCOMPATABLE              71
#define TW_ERROR_VERIFY_INVALID_OP_FOR_UNIT_MODE           72
#define TW_ERROR_VERIFY_NOT_REDUNDANT                      73
#define TW_ERROR_BBU_NOT_PRESENT                           74
#define TW_ERROR_BBU_BATTERY_NOT_PRESENT                   75
#define TW_ERROR_FAILED_TO_GET_IS_INITIALIZED              76
#define TW_ERROR_IS_INITIALIZED_INVALID_CONFIG             77
#define TW_ERROR_FAILED_TO_GET_TASK_ACTIVE                 78
#define TW_ERROR_TASK_ACTIVE_INVALID_CONFIG                79
#define TW_ERROR_SPLIT_INVALID_OP_FOR_UNIT_CONFIG          80
#define TW_ERROR_DISABLED_CONTROLLER                       81
#define TW_ERROR_ECC_POLICY_INVALID_CONFIG                 82
#define TW_ERROR_ATA_COMMAND_NOT_SUPPORTED                 83
#define TW_ERROR_ATA_SMART_FEATURE_NOT_SUPPORTED           84
#define TW_ERROR_REMOVE_UNIT_RECOVERY                      85
#define TW_ERROR_FW_IMAGE_INVALID_PROM_IMAGE               86
#define TW_ERROR_FW_IMAGE_INCORRECT_PROM_IMAGE             87
#define TW_ERROR_FW_IMAGE_DRIVER_SRL_RANGE                 88
#define TW_ERROR_FW_IMAGE_API_SRL_RANGE                    89
#define TW_ERROR_BBU_IS_DISABLED                           90
#define TW_ERROR_MIGRATE_UNIT_NO_CHANGE                    91
#define TW_ERROR_INVALID_ENCLOSURE_ID                      92
#define TW_ERROR_INVALID_FAN_ID                            93
#define TW_ERROR_INVALID_DOOR_LOCK_ID                      94
#define TW_ERROR_INVALID_ALARM_ID                          95
#define TW_ERROR_INVALID_DRIVE_SLOT_ID                     96
#define TW_ERROR_FAN_DOES_NOT_EXIST                        97
#define TW_ERROR_DOOR_LOCK_DOES_NOT_EXIST                  98
#define TW_ERROR_ALARM_DOES_NOT_EXIST                      99
#define TW_ERROR_DRIVE_SLOT_DOES_NOT_EXIST                100
#define TW_ERROR_PORT_IS_NOT_CONNECTED                    101

#define TW_ERROR_INVALID_FIRMWARE_IMAGE_SIZE              102
#define TW_ERROR_INVALID_FIRMWARE_MAGIC                   103
#define TW_ERROR_INCOMPATIBLE_FIRMWARE_ARCHITECTURE       104

#define TW_ERROR_MIGRATE_WITH_LESS_DRIVES                 105
#define TW_ERROR_INVALID_BINARY_ARCH                      106
#define TW_ERROR_UNIT_HAS_NO_LUNS                         107
#define TW_ERROR_INVALID_LUN_ID                           108
#define TW_ERROR_FAIL_SPLIT_MIRROR_WHILE_IO_PENDING       109
#define TW_ERROR_INVALID_TEMP_SENSOR_ID                   110
#define TW_ERROR_TEMP_SENSOR_DOES_NOT_EXIST               111
#define TW_ERROR_UNIT_NOT_REBUILDING                      112
#define TW_ERROR_INCOMPATIBLE_FIRMWARE_SRL                113
#define TW_ERROR_UNIT_NOT_FOUND                           114
#define TW_ERROR_DRIVE_CMD_QUEUE_NOT_SUPPORTED            115
#define TW_ERROR_USER_BUFFER_TOO_SMALL                    116
#define TW_ERROR_INVALID_POWER_SUPPLY_ID                  117
#define TW_ERROR_POWER_SUPPLY_DOES_NOT_EXIST              118
#define TW_ERROR_INVALID_ID                               119
#define TW_ERROR_INVALID_PHY_ID                           120
#define TW_ERROR_TEMP_SENSOR_INVALID_TEMPERATURE          121
#define TW_ERROR_INVALID_ARGUMENTS                        122
#define TW_ERROR_SCSI_COMMAND_NOT_SUPPORTED               123
#define TW_ERROR_INVALID_FAN_SPEED_PARAMETER              124
#define TW_ERROR_TEMP_SENSOR_STATUS_UNKNOWN               125
#define TW_ERROR_INVALID_PAGE_CODE_IN_PARAMETER_DATA      126
#define TW_ERROR_ENCLOSURE_ELEMENT_DOES_NOT_EXIST         127
#define TW_ERROR_FAN_SPEED_SET_NOT_SUPPORTED              128
#define TW_ERROR_INVALID_EVENT_CAPTURE_SIZE               129
#define TW_ERROR_ENCLOSURE_DOES_NOT_EXIST                 130
#define TW_ERROR_AEN_SOURCE_UNKNOWN                       131
#define TW_ERROR_ALARM_TONE_NOT_SUPPORTED                 132
#define TW_ERROR_JBOD_NOT_SUPPORTED                       133
#define TW_ERROR_LINK_SPEED_6_0_GBPS_NOT_SUPPORTED        134
#define TW_ERROR_EXP_DOWNLOAD_INCORRECT_IMAGE             135
#define TW_ERROR_EXP_DOWNLOAD_UNKNOWN_EXP_TYPE            136
#define TW_ERROR_EXP_DOWNLOAD_EXP_ERROR                   137
#define TW_ERROR_EXP_DOWNLOAD_NOT_SUPPORTED               138
#define TW_ERROR_EXP_DOWNLOAD_UNKNOWN_IMAGE               139
#define TW_ERROR_EXP_DOWNLOAD_INVALID_IMAGE               140
#define TW_ERROR_DRIVE_DOWNLOAD_NOT_SUPPORTED             141
#define TW_ERROR_DRIVE_DOWNLOAD_INVALID_IMAGE             142
#define TW_ERROR_DRIVE_NOT_PRESENT                        143
#define TW_ERROR_DRIVE_INTERFACE_NOT_SUPPORTED            144
#define TW_ERROR_PORT_IS_NOT_DIRECT_CONNECTED                    145


#define TW_ERROR_FIRST                TW_ERROR_NONE
#define TW_ERROR_LAST                 TW_ERROR_PORT_IS_NOT_DIRECT_CONNECTED
#define TW_ERROR_MEM_ALLOC_FAILED     TW_ERROR_MEMORY_ALLOC_FAILED


#define TW_IOCTL_ERROR_NO_MORE_EVENTS                       0
#define TW_IOCTL_SYNC_TIME_WITH_HOST                   0x0031

#define tw_format_message(x)                  ((char*)(x.text))
#define tw_api_init(x)                         TW_RET_SUCCESS
#define tw_api_done()
#define tw_celsius_to_fahrenheit(celsius)     ( ((9 * celsius) / 5) + 32 )
#define tw_fahrenheit_to_celsius(fahrenheit)  ( ((fahrenheit - 32) * 5) / 9 )


#define TW_FW_PASSTHRU_CMD_UNKNOWN          0xFF
#define TW_FW_PASSTHRU_CMD_NO_DATA          1
#define TW_FW_PASSTHRU_CMD_DATA_IN          2
#define TW_FW_PASSTHRU_CMD_DATA_OUT         3


#define TW_TARGET_COMMAND_QUEUE_SIZE                      256

#define TW_HBA_BUS_UNKNOWN_0                              0x01
#define TW_HBA_BUS_UNKNOWN_1                        (0x01 << 1)
#define TW_HBA_BUS_PCI_MODE64                       (0x01 << 2)                            
#define TW_HBA_BUS_PCI_33                           (0x01 << 3)
#define TW_HBA_BUS_PCI_66                           (0x01 << 4)
#define TW_HBA_BUS_PCIX_66                          (0x01 << 5)
#define TW_HBA_BUS_PCIX_100                         (0x01 << 6)
#define TW_HBA_BUS_PCIX_133                         (0x01 << 7)
#define TW_HBA_BUS_PCIE_G1                          (0x01 << 8)
#define TW_HBA_BUS_PCIE_G2                          (0x01 << 9)
#define TW_HBA_BUS_PCIE_LANE_1X                     (0x01 << 11)
#define TW_HBA_BUS_PCIE_LANE_4X                     (0x01 << 12)
#define TW_HBA_BUS_PCIE_LANE_8X                     (0x01 << 13)
#define TW_HBA_BUS_PCIE_LANE_16X                    (0x01 << 14)


// The following defines are temporary to make R3 cli compile

#define TW_SATA_LINK_SPEED_GEN1_SUPPORTED                0x01
#define TW_SATA_LINK_SPEED_GEN2_SUPPORTED                0x02
#define TW_SATA_LINK_SPEED_GEN1_ENABLED                  0x01
#define TW_SATA_LINK_SPEED_GEN2_ENABLED                  0x02

#define TW_UNEXPORTED_UNIT                               0xFE

// end of temporary defines


// Drive SMART data defines
#define TW_DRIVE_POWER_ON_HOURS_NULL_VALUE               0xFFFF

// PHY link speed defines

#define TW_DRIVE_LINK_1_5_GBPS                           0x01
#define TW_DRIVE_LINK_3_0_GBPS                           0x02
#define TW_DRIVE_LINK_6_0_GBPS                           0x04


#define TW_PHY_NO_LINK                                   0x00
#define TW_PHY_LINK_1_5_GBPS                             0x01
#define TW_PHY_LINK_3_0_GBPS                             0x02
#define TW_PHY_LINK_6_0_GBPS                             0x04


#define TW_PHY_LINK_SPEED_AUTO                           0x00
#define TW_PHY_LINK_SPEED_LIMIT_1_5_GBPS                 0x01
#define TW_PHY_LINK_SPEED_LIMIT_3_0_GBPS                 0x02
#define TW_PHY_LINK_SPEED_LIMIT_6_0_GBPS                 0x04


#define TW_DRIVE_LINK_SPEED_AUTO                         0x00
#define TW_DRIVE_LINK_SPEED_LIMIT_1_5_GBPS               0x01
#define TW_DRIVE_LINK_SPEED_LIMIT_3_0_GBPS               0x02


// drive interface defines
#define TW_DRIVE_INTERFACE_NO_DRIVE                      0x00
#define TW_DRIVE_INTERFACE_SATA_DIRECT_ATTACHED          0x01
#define TW_DRIVE_INTERFACE_SAS_DIRECT_ATTACHED           0x03
#define TW_DRIVE_INTERFACE_SATA_EXPANDER_ATTACHED        0x05
#define TW_DRIVE_INTERFACE_SAS_EXPANDER_ATTACHED         0x07

// device type connected to phy
#define TW_DEVICE_TYPE_NO_DEVICE                         0x00
#define TW_DEVICE_TYPE_SATA                              0x01
#define TW_DEVICE_TYPE_SAS                               0x02
#define TW_DEVICE_TYPE_ENCLOSURE                         0x03
#define TW_DEVICE_TYPE_ENCLOSURE_WITH_DISCOVERY_ERROR    0x04


#define TW_PORT_SETTING_FOR_SAS                          0x01
#define TW_PORT_SETTING_FOR_SATA                         0x02

// Mode page control values
#define TW_SCSI_MODEPAGE_CONTROL_HONOR                   0x00
#define TW_SCSI_MODEPAGE_CONTROL_IGNORE_WCE              0x01


// Background Task Activity Policy(BTAP) setting values
#define TW_BTAP_POLICY_ADAPTIVE                          0x00
#define TW_BTAP_POLICY_LOW_LATENCY                       0x01



//Enclosure drive slot status defines
#define TW_ENCL_DRIVE_SLOT_STATUS_NONE                              0x00
#define TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_INSERTED                   0x01
#define TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_READY_TO_INSERT            0x02
#define TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_READY_TO_REMOVE            0x04
#define TW_ENCL_DRIVE_SLOT_STATUS_DEVICE_ACTIVATED                  0x08


//Enclosure fan status defines
#define TW_ENCL_FAN_STATUS_OK                  0x00
#define TW_ENCL_FAN_STATUS_FAILURE             0x01
#define TW_ENCL_FAN_STATUS_NOT_INSTALLED       0x02
#define TW_ENCL_FAN_STATUS_OFF                 0x03
#define TW_ENCL_FAN_STATUS_UNKNOWN             0x80

//Enclosure fan speed code defines
#define TW_ENCL_FAN_STOPPED                    0x00
#define TW_ENCL_FAN_SPEED_LOWEST               0x01
#define TW_ENCL_FAN_SPEED_SECOND_LOWEST        0x02
#define TW_ENCL_FAN_SPEED_THIRD_LOWEST         0x03
#define TW_ENCL_FAN_SPEED_INTERMEDIATE         0x04
#define TW_ENCL_FAN_SPEED_THIRD_HIGHEST        0x05
#define TW_ENCL_FAN_SPEED_SECOND_HIGHEST       0x06
#define TW_ENCL_FAN_SPEED_HIGHEST              0x07
#define TW_ENCL_FAN_SPEED_UNKNOWN              0x80


//Enclosure door lock status defines
#define TW_ENCL_DOOR_LOCK_STATUS_LOCKED        0x00
#define TW_ENCL_DOOR_LOCK_STATUS_UNLOCKED      0x01
#define TW_ENCL_DOOR_LOCK_STATUS_NOT_INSTALLED 0x02
#define TW_ENCL_DOOR_LOCK_STATUS_UNKNOWN       0x80

//Enclosure alram status defines
#define TW_ENCL_ALARM_STATUS_OFF                0x00
#define TW_ENCL_ALARM_STATUS_ON                 0x01
#define TW_ENCL_ALARM_STATUS_NOT_INSTALLED      0x02
#define TW_ENCL_ALARM_STATUS_MUTED_STATE        0x03
#define TW_ENCL_ALARM_STATUS_FAILURE            0x04
#define TW_ENCL_ALARM_STATUS_MUTED_ENCLOSURE_ERROR_DETECTED     0x05
#define TW_ENCL_ALARM_STATUS_UNKNOWN            0x80

//Enclosure audible alarm tone urgency defines
#define TW_ALARM_TONE_URGENCY_UNRECOVERABLE    0x01
#define TW_ALARM_TONE_URGENCY_CRITICAL         0x02
#define TW_ALARM_TONE_URGENCY_NON_CRITICAL     0x04
#define TW_ALARM_TONE_URGENCY_INFO             0x08

//Enclosure alarm mute states
#define TW_ALARM_NOT_MUTED                     0x00
#define TW_ALARM_MUTED                         0x01

//Enclosure alarm on-off states
#define TW_ALARM_OFF                           0x00
#define TW_ALARM_ON                            0x01

//Enclosure temperature sensor status defines
#define TW_ENCL_TEMP_SENSOR_STATUS_OK                    0x00
#define TW_ENCL_TEMP_SENSOR_STATUS_CRITICAL              0x01
#define TW_ENCL_TEMP_SENSOR_STATUS_NOT_INSTALLED         0x02
#define TW_ENCL_TEMP_SENSOR_STATUS_UNKNOWN               0x80

//Enclosure temperature sensor threshold status defines
#define TW_ENCL_TEMP_THRESHOLD_UNDER_TEMP_WARNING        0x01
#define TW_ENCL_TEMP_THRESHOLD_UNDER_TEMP_FAILURE        0x02
#define TW_ENCL_TEMP_THRESHOLD_OVER_TEMP_WARNING         0x04
#define TW_ENCL_TEMP_THRESHOLD_OVER_TEMP_FAILURE         0x08

//Enclosure power supply status defines
#define TW_ENCL_POWER_SUPPLY_STATUS_OK                   0x00
#define TW_ENCL_POWER_SUPPLY_STATUS_FAILURE              0x01
#define TW_ENCL_POWER_SUPPLY_STATUS_NOT_INSTALLED        0x02
#define TW_ENCL_POWER_SUPPLY_STATUS_OFF                  0x03
#define TW_ENCL_POWER_SUPPLY_STATUS_UNKNOWN              0x80

//Enclosure power supply threshold status defines
#define TW_ENCL_POWER_THRESHOLD_DC_CURRENT_NORMAL        0x00
#define TW_ENCL_POWER_THRESHOLD_DC_VOLTAGE_NORMAL        0x00
//threshold status is the bitwise OR of the following
#define TW_ENCL_POWER_THRESHOLD_DC_OVER_CURRENT          0x01
#define TW_ENCL_POWER_THRESHOLD_DC_UNDER_VOLTAGE         0x02
#define TW_ENCL_POWER_THRESHOLD_DC_OVER_VOLTAGE          0x04

//Enclosure status defines
//Change the values to reflect the common status code
#define TW_ENCL_STATUS_NOT_SUPPORTED      0x00 //0x80
#define TW_ENCL_STATUS_OK                 0x01 //0x00
#define TW_ENCL_STATUS_FAILURE            0x02 //0x01
#define TW_ENCL_STATUS_NOT_INSTALLED      0x05 //0x02
#define TW_ENCL_STATUS_WARNING            0x03 //0x03
#define TW_ENCL_STATUS_UNKNOWN            0x06 //0x80


//-------------------------------------------------------
// Software event logging defines
//-------------------------------------------------------

#define TW_AEN_SCSI_SENSE_DATA_SIZE          18

// for AMCC application use
#define TW_AEN_SOURCE_3DM                   0x00
#define TW_AEN_SOURCE_ENCLOSURE_MONITOR     0x01
#define TW_AEN_SOURCE_CLI                   0x02
#define TW_AEN_SOURCE_SCOUT                 0x03
// for 3rd party application use
#define TW_AEN_SOURCE_3RD_PARTY             0x80

#define TW_AEN_SEVERITY_ERROR            1
#define TW_AEN_SEVERITY_WARNING          2
#define TW_AEN_SEVERITY_INFORMATION      3
#define TW_AEN_SEVERITY_DEBUG            4



//==============================================================================
// Performance Monitoring and statistics defines
//==============================================================================
#define TW_PMSTAT_VERSION              1       // current version

#define TW_MAX_PMSTAT_COMMANDS                 1000

#define TW_PMSTAT_IS_DISABLED                  0x00
#define TW_PMSTAT_IS_ENABLED                   0x01

#define TW_PMSTAT_OF_DRIVE                     0x00
#define TW_PMSTAT_OF_UNIT                      0x01

#define TW_PMSTAT_VOLATILE                     0x00
#define TW_PMSTAT_PERSISTENT                   0x01

#define TW_PMSTAT_RUN_AVERAGE_SUMMARY          0x00
#define TW_PMSTAT_HIGHER_RESPONSE_TIME_CMDS    0x01
#define TW_PMSTAT_EXTD_DRIVE_STATISTICS        0x02

#define TW_RESPONSE_TIME_0_TO_1_MS             0x00
#define TW_RESPONSE_TIME_1_TO_2_MS             0x01
#define TW_RESPONSE_TIME_2_TO_3_MS             0x02
#define TW_RESPONSE_TIME_3_TO_4_MS             0x03
#define TW_RESPONSE_TIME_4_TO_5_MS             0x04
#define TW_RESPONSE_TIME_5_TO_6_MS             0x05
#define TW_RESPONSE_TIME_6_TO_7_MS             0x06
#define TW_RESPONSE_TIME_7_TO_8_MS             0x07
#define TW_RESPONSE_TIME_8_TO_9_MS             0x08
#define TW_RESPONSE_TIME_9_TO_10_MS            0x09
#define TW_RESPONSE_TIME_10_TO_20_MS           0x10
#define TW_RESPONSE_TIME_20_TO_30_MS           0x11
#define TW_RESPONSE_TIME_30_TO_40_MS           0x12
#define TW_RESPONSE_TIME_40_TO_50_MS           0x13
#define TW_RESPONSE_TIME_50_TO_60_MS           0x14
#define TW_RESPONSE_TIME_60_TO_70_MS           0x15
#define TW_RESPONSE_TIME_70_TO_80_MS           0x16
#define TW_RESPONSE_TIME_80_TO_90_MS           0x17
#define TW_RESPONSE_TIME_90_TO_100_MS          0x18
#define TW_RESPONSE_TIME_100_TO_200_MS         0x19
#define TW_RESPONSE_TIME_200_TO_300_MS         0x20
#define TW_RESPONSE_TIME_300_TO_400_MS         0x21
#define TW_RESPONSE_TIME_400_TO_500_MS         0x22
#define TW_RESPONSE_TIME_500_OR_MORE_MS        0x23


// Optional capabilities that are supported by the data source:
#define TW_PMSTAT_CAPMASK_TARGET_LUN   0x0001  // can specify target/lun filter
#define TW_PMSTAT_CAPMASK_CTYPE_SCSI   0x0002  // capturing SCSI commands
#define TW_PMSTAT_CAPMASK_CTYPE_ATA    0x0004  // capturing (S)ATA commands
#define TW_PMSTAT_CAPMASK_CTYPE_INT    0x0008  // capturing internal commands
#define TW_PMSTAT_CAPMASK_REQUEST      0x0010  // triggering on request
#define TW_PMSTAT_CAPMASK_MAXTIME      0x0020  // triggering on response time
#define TW_PMSTAT_CAPMASK_ERROR        0x0040  // triggering on error
#define TW_PMSTAT_CAPMASK_OPCODE       0x0080  // triggering on opcode
#define TW_PMSTAT_CAPMASK_OPTYPE_ONE   0x0100  // capturing specific command
#define TW_PMSTAT_CAPMASK_OPTYPE_RD    0x0200  // capturing just read commands
#define TW_PMSTAT_CAPMASK_OPTYPE_WT    0x0400  // capturing just write commands
#define TW_PMSTAT_CAPMASK_OPTYPE_NORW  0x0800  // capturing all but r/w commands
#define TW_PMSTAT_CAPMASK_CMODE_BEGIN  0x1000  // capturing data after trigger
#define TW_PMSTAT_CAPMASK_CMODE_MID    0x2000  // capturing data around trigger
#define TW_PMSTAT_CAPMASK_CMODE_END    0x4000  // capturing data before trigger

// indicates the status of data capture (allows combining values)
#define TW_PMSTAT_STATE_RUNNING        0x01    // 1 if data capture is currently active
#define TW_PMSTAT_STATE_DATAREAD       0x02    // 1 if data has already been read from API

// specifies a type of command (allows combining values)
#define TW_PMSTAT_CMDTYPE_DEFAULT      0x00    // default (REQ)
#define TW_PMSTAT_CMDTYPE_SCSI         0x01    // SCSI command
#define TW_PMSTAT_CMDTYPE_ATA          0x02    // (S)ATA command
#define TW_PMSTAT_CMDTYPE_INT          0x04    // internal command

// specifies types of opcodes (allows combining values)
#define TW_PMSTAT_OPTYPE_ALL           0x00    // all commands (REQ)
#define TW_PMSTAT_OPTYPE_ONE           0x01    // one specific command
#define TW_PMSTAT_OPTYPE_READ          0x02    // read commands
#define TW_PMSTAT_OPTYPE_WRITE         0x04    // write commands
#define TW_PMSTAT_OPTYPE_NON_RW        0x08    // non read/write commands

// specifies current execution mode (only one can be specified)
#define TW_PMSTAT_RUNMODE_DISABLED     0x00    // data capture disabled (REQ)
#define TW_PMSTAT_RUNMODE_CONTINUOUS   0x01    // continuous data capture (REQ)
#define TW_PMSTAT_RUNMODE_TRIGGER      0x02    // trigger data capture

// specifies event that triggers data capture (allows combining values)
#define TW_PMSTAT_TRIGGER_REQUEST      0x00    // explicit request
#define TW_PMSTAT_TRIGGER_ERROR        0x01    // upon command error
#define TW_PMSTAT_TRIGGER_OPCODE       0x02    // when match opcode
#define TW_PMSTAT_TRIGGER_RTIME        0x04    // response time exceeds value

// specifies where trigger point is in data (not valid for _TRIGGER_START)
#define TW_PMSTAT_CAPMODE_START        0       // starting from trigger
#define TW_PMSTAT_CAPMODE_MIDDLE       1       // trigger point in middle
#define TW_PMSTAT_CAPMODE_END          2       // trigger point is at end

// specifies status for every captured command:
#define TW_PMSTAT_STATUS_VALID         0x01    // command data is valid (REQ)
#define TW_PMSTAT_STATUS_TRIGGER       0x10    // this command triggered capture
#define TW_PMSTAT_STATUS_ERROR         0x80    // command completed with error


// performance monitoring running response histogram defines
 
#define TW_PMSTAT_HIST_NUM_BINS_1MS            10
#define TW_PMSTAT_HIST_NUM_BINS_10MS            9
#define TW_PMSTAT_HIST_NUM_BINS_100MS           9
#define TW_PMSTAT_HIST_NUM_BINS_1000MS          9
#define TW_PMSTAT_HIST_NUM_BINS_EXCESS          1
#define TW_PMSTAT_HIST_NUM_BINS_TOTAL          38

// command details for commands with higher response time 
#define TW_PMSTAT_CMD_STATUS_ERROR         0x01    // command completed with error


//==============================================================================
// Event Capture defines
//==============================================================================
#define TW_EVENT_CAPTURE_DISABLED          0
#define TW_EVENT_CAPTURE_ENABLED           1

#define TW_EVENT_CAPTURE_UNDEFINED         0
#define TW_EVENT_CAPTURE_UNTIL_FULL        1
#define TW_EVENT_CAPTURE_UNTIL_STOPPED     2

#define TW_EVENT_CAPTURE_STOP              0
#define TW_EVENT_CAPTURE_START             1

#define TW_EVENT_CAPTURE_SEGMENT_HEADER_LEN     64

#define TW_EVENT_CAPTURE_MIN_EVENTS        1024   //controller default is 1000;
                                                  // if num_of_events=1, specified in tw_enable_event_capture 
#define TW_EVENT_CAPTURE_MAX_EVENTS        1000000


//==============================================================================
// Event Trigger defines
//==============================================================================
#define TW_DIAGNOSTIC_EVENT_RESET           0x0001  //bit  0
#define TW_DIAGNOSTIC_EVENT_ASSERT          0x0002  //bit  1
#define TW_DIAGNOSTIC_EVENT_DRIVE_ERROR     0x0004  //bit  2
#define TW_DIAGNOSTIC_EVENT_SAVE_ONCE       0x8000  //bit 15
#define TW_DIAGNOSTIC_LOG_SAVE_LAST              0
#define TW_DIAGNOSTIC_LOG_SAVE_FIRST             1
#define TW_DIAGNOSTIC_LOG_TIGGER_DISABLED        0
#define TW_DIAGNOSTIC_LOG_TIGGER_ENABLED         1


#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif  /* _TW_API_DEFINES_H */
