#ifndef CTRL_H
#define CTRL_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                   C O N T R O L

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file ctrl.h
 *
 * @brief This file contains declarations and data types implemented by control system.
 *
 * @details This file contains the definitions, types and APIs
 * of control system.
 *
 * <b>Edit History For File</b>\n
 *  This section contains comments describing changes made to this file.\n
 *  Notice that changes are listed in reverse chronological order.\n
 * <table border>
 * <tr>
 *   <td><b> when </b></td>
 *   <td><b> who </b></td>
 *   <td><b> what, where, why </b></td>
 * </tr>
 * <tr>
 *   <td> 10/02/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify low power mode handling to decrease current consumption in power down.
 *        Use active halt with two wakeup one for switch debouncing and second for LCD disabling.
 *        Solve buzzer bug with power fail, Modify error screen to display latched tampers and alarms also. </td>
 * </tr>
 * <tr>
 *   <td> 27/03/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Change low power switches handling to avoid halt while SW_STAT pin is low and add commands to force relay open. </td>
 * </tr>
 * <tr>
 *   <td> 13/01/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Make overload sequence configurable. </td>
 * </tr>
 * <tr>
 *   <td> 09/01/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Resolve battery percentage variables duplicate. </td>
 * </tr>
 * <tr>
 *   <td> 13/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Move tampers counter and operator ID to system variables. </td>
 * </tr>
 * <tr>
 *   <td> 09/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add template ID to configure meter record. </td>
 * </tr>
 * <tr>
 *   <td> 29/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add read meter status function. </td>
 * </tr>
 * <tr>
 *   <td> 21/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Solve bug of event log and  GPRS events codes. </td>
 * </tr>
 * <tr>
 *   <td> 05/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup CTRL_SYS Control System
 * @ingroup METER
 * Control system responsible for managemnet of tampers, external board components
 * power down detection etc….\n
 * This module has four interfaces each one of these interfaces has number of APIs.\n
 *   -# Configuration Interface.
 *   -# Event Interface.
 *   -# Command Interface.
 *   -# Read System Variables Interface.\n
 * @brief Control module manages tampers, events log and board components.
 */
/*!
 * @defgroup CTRL_CORE Application Core APIs
 * @ingroup CTRL_SYS
 * @brief Control system entry and initialization functions.
 */
/*!
 * @defgroup CTRL_CFG Configuration APIs
 * @ingroup CTRL_SYS
 * @brief Control system confiuration functions and data types.
 */
/*!
 * @defgroup CTRL_READ Read System Variables APIs
 * @ingroup CTRL_SYS
 * @brief Used to read calculated variables by control system.
 */
/*!
 * @defgroup CTRL_EVENT Events APIs
 * @ingroup CTRL_SYS
 * @brief Control system events call back APIs.
 */
/*!
 * @defgroup CTRL_CMD Commands APIs
 * @ingroup CTRL_SYS
 * @brief Control system command interface APIs.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
/* Included directly, NOT through dependencies_layer.h. ctrl.h is mutually
   dependent with trf_pymt.h and Meter_Config.h; that cycle only resolves when
   it is entered from here. Going via the aggregate makes its include guard
   swallow the re-entry, leaving acc_reg_t / trf_pymt_mny_rec_t undefined at
   the point this file needs them. */
#include "dependencies_layer.h"
#include "GPIO_Interface.h"
#include "Timer_Interface.h"
#ifdef ANSI_STACK
/* #include "Table33.h" */ /* module removed from the DRU build */
/* #include "ANSI_Utility_interface.h" */ /* module removed from the DRU build */
#endif
#ifdef G2_DM
/* #include "G2_DM.h" */ /* module removed from the DRU build */
#endif
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/

/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/
#ifdef ANSI_STACK
#define TABLE01_MFG_SERIAL_NUMBER_SIZE    (16)  
#endif
#define NORMAL_OPERATING_VOLTAGE    100UL // 100V
#define SAG_LVL                     1500000 //1586241UL  // @80V when nominal voltage = 232V
//#define SAG_LVL                     2762720UL  // @150V when nominal voltage = 232V


#define SIZE_OF_METER_ID    20
#define STANDARD_LOG_MAX_NUMBER_OF_RECORDS 400
#define STANDARD_LOG_XDR_RECORD_SIZE 21 /* "2+14+3+2" 2 for structure formating, 14 for date & time, 3 for meter status and 2 for event code */
#define STANDARD_LOG_MAX_RECORDS_PER_XDR   ((TU16)((Dlms_mXdrMaxBufferSize - 3) / STANDARD_LOG_XDR_RECORD_SIZE))
#define STANDARD_LOG_NUMBER_OF_CAPTURE_OBJECTS      3

#define TAMPERING_LOG_MAX_NUMBER_OF_RECORDS 200
#define TAMPERING_LOG_XDR_RECORD_SIZE 21 /* "2+14+3+2" 2 for structure formating, 14 for date & time, 3 for meter status and 2 for event code*/
#define TAMPERING_LOG_MAX_RECORDS_PER_XDR   ((TU16)((Dlms_mXdrMaxBufferSize - 3) / TAMPERING_LOG_XDR_RECORD_SIZE))
#define TAMPERING_LOG_NUMBER_OF_CAPTURE_OBJECTS      3

#define DISCONNECT_LOG_MAX_NUMBER_OF_RECORDS 10
#define DISCONNECT_LOG_XDR_RECORD_SIZE 20 /* "2+14+2+2" 2 for structure formating, 14 for date & time, 2 for disconnector output status and 2 for event code*/
#define DISCONNECT_LOG_MAX_RECORDS_PER_XDR   ((TU16)((Dlms_mXdrMaxBufferSize - 3) / DISCONNECT_LOG_XDR_RECORD_SIZE))
#define DISCONNECT_LOG_NUMBER_OF_CAPTURE_OBJECTS      3
#define MAX_SIZE_MATCH_ACTION                  255

#define LOAD_PROFILE_SIZE			sizeof(TstLoadProfileRecord)
#define LOAD_PROFILE_MAX_RECORD 	1000  		/* 1000 for load profile */
#define LOAD_PROFILE_RECORDS		11//10//12	  		/* Number of fields in the Load Profile Record*/
#define LOAD_PROFILE_RECORD_PERIOD	900			/* Store record every 15 Min*/
#define METER_CFG_PROFILE             4
#ifdef DLMS_ENABLED
extern const TCapture_Object CaptureObjectsMeterCfg[METER_CFG_PROFILE];

extern const TCapture_Object CaptureObjectsLoadProfile[LOAD_PROFILE_RECORDS];
extern const uint8_t CaptureObjectsLoadProfile_XDR_Size[LOAD_PROFILE_RECORDS];
extern const TCapture_Object Standard_Log_CaptureObjects[STANDARD_LOG_NUMBER_OF_CAPTURE_OBJECTS];
extern const uint8_t Standard_Log_CaptureObjects_XDR_Size[STANDARD_LOG_NUMBER_OF_CAPTURE_OBJECTS];

#define MONEY_TRANS_LOG_NUMBER_OF_CAPTURE_OBJECTS 6
#define MONEY_TRANS_LOG_XDR_RECORD_SIZE 31//14+5+3+2+2+5
extern const uint8_t Money_Trans_CaptureObjects_XDR_Size[MONEY_TRANS_LOG_NUMBER_OF_CAPTURE_OBJECTS];
extern const TCapture_Object Money_Trans_CaptureObjects[MONEY_TRANS_LOG_NUMBER_OF_CAPTURE_OBJECTS];
#endif
/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
#ifdef ANSI_STACK

typedef enum 
{
  METERING_MODE_BIT,
  TEST_MODE_BIT,
  METER_SHOP_MODE_BIT,
  METER_FACTORY_MODE_BIT,
  UNI_BIDIRECTIONAL_BIT
    
}metering_mode_t;

typedef enum
{
    ALARMS_SELF_DIAGNOSTIC_FAIL    = (uint8_t)  0 ,   
    ALARMS_REVERSE                 = (uint8_t)  1 ,
    ALARMS_EXCEEDING_LMT           = (uint8_t)  2 ,  /**not used**/
    ALARMS_OVER_VOLTAGE            = (uint8_t)  3 ,
    ALARMS_UNDER_VOLTAGE           = (uint8_t)  4 ,
    ALARMS_TST_RELAY               = (uint8_t)  5 ,  /**not used**/
    /* defines electricity company open relay command */
    ALARMS_USER_OPEN_RELAY         = (uint8_t)  6 ,
    /* Defines meters'owner < Client > open relay command */
    ALARMS_CUSTOMER_OPEN_RELAY         = (uint8_t) 7 ,
    ALARMS_INCORRECT_PASSWORD         = (uint8_t)  8 ,
    ALARMS_NUM_OF_CODE             = (uint8_t)  9 ,
    ALARMS_NO_ALARM_EXIST          = (uint8_t) 200 ,
    ALARMS_ALL_ALARMS              = (uint8_t) 255
}AlarmsCodes_t;

typedef enum
{
  ALARMS_MISS_NUETRAL              = (uint8_t) 0,
  ALARMS_ADE_FAIL                 = (uint8_t)  1,
  ALARMS_EEPROM_FAIL               = (uint8_t) 2,
  ALARMS_RELAY_FAIL                = (uint8_t) 3,
  ALARMS_NUM_OF_ALARM_NEW           = (uint8_t) 4,
  /** RFU Bytes From 4 to 100 **/  
  ALARMS_NUM_OF_CODE_FATAL         = (uint8_t) 100,
  
}New_AlarmsCodes_t;

__packed typedef struct
{
 uint8_t  rly_disc_reason;
 uint16_t rly_disc_count;
 uint32_t alarm_registered;
 uint16_t alarm_action_latched;
 uint8_t crc_ctrl;
}alarm_ctrl_rcd_t;

__packed typedef struct
{
 uint8_t alarm_code;
 uint16_t alarm_action;
}alarm_Confg_Rcd_t;

__packed typedef struct
{
 uint16_t up_side_down_counter;
 uint8_t  meter_is_upside_down;
 uint8_t  crc;
}up_side_down_parameters_t;

__packed typedef struct{
  uint8_t primarySection;
  uint8_t secondarySection;
  uint8_t externalSection;
}failLoadDataCounters_t;

__packed typedef struct
{
  uint8_t init_flag;
  alarm_Confg_Rcd_t alarm_Confg_Rcd[ALARMS_NUM_OF_CODE];
  uint8_t CRC_value;
  alarm_ctrl_rcd_t alarm_ctrl_rcd;
//  alarm_Buzzer_Confg_t alarm_Buzzer_Confg;
  up_side_down_parameters_t up_side_down_parameters ;
  failLoadDataCounters_t failLoadDataCounters; 
  uint8_t CRC;
}Alarms_File_t;

__packed typedef struct
{
  uint8_t init_flag;
  alarm_Confg_Rcd_t alarm_Confg_Rcd[ALARMS_NUM_OF_CODE_FATAL];
  uint8_t CRC_value;
  alarm_ctrl_rcd_t alarm_ctrl_rcd;
//  alarm_Buzzer_Confg_t alarm_Buzzer_Confg;
  up_side_down_parameters_t up_side_down_parameters ;
  failLoadDataCounters_t failLoadDataCounters; 
  uint8_t CRC;
}Alarms_File_New_t;
#endif

typedef enum
{
  LPM_NONE_STATUS                  = (uint8_t)0x00,
  LPM_PF_OCCURRED_STATUS           = (uint8_t)0x01,
  LPM_VOLT_EXCEED_THRESHOLD_STATUS = (uint8_t)0x02,
}LPM_MODES;
typedef enum {
  RESET_BATTERY_TAMPER = (uint8_t)0,
  RESET_OVERLOAD_A     = (uint8_t)5,
  RESET_OVERLOAD_B     = (uint8_t)6,
  RESET_OVERLOAD_C     = (uint8_t)7,
  RESET_COVER_OPEN     = (uint8_t)20, 
  RESET_TERMINAL_OPEN  = (uint8_t)21, 
  RESET_MAGNETIC       = (uint8_t)22, 
  RESET_REVERSE_A      = (uint8_t)23, 
  RESET_REVERSE_B      = (uint8_t)24, 
  RESET_REVERSE_C      = (uint8_t)25, 
  RESET_EARTH_A        = (uint8_t)26, 
  RESET_EARTH_B        = (uint8_t)27, 
  RESET_EARTH_C        = (uint8_t)28, 
  RESET_MISSING_A      = (uint8_t)29, 
  RESET_MISSING_B      = (uint8_t)30, 
  RESET_MISSING_C      = (uint8_t)31, 
  
}RESET_SPECIFIC_TAMPER_enum;
/*!
 * @ingroup CTRL_READ
 * @par Description:
 *   This enumerator contains flags masks of control system.
 */
typedef enum
{
  CTRL_PWR_DWN_MASK = (uint8_t)0x01,
  CTRL_RTC_INT_MASK = (uint8_t)0x02,
  CTRL_RELAY_HINT_MASK = (uint8_t)0x04,
  CTRL_GO_LPM  = (uint8_t)0x08,
  CTRL_FIRST_INIT_LPM  = (uint8_t)0x10,
} ctrl_flag_mask_notSaved_t;
typedef enum
{
  CTRL_MAXIMUM_CONSUMPTION = (uint8_t)0x08,
  CTRL_COMMISSIONING_PROCESS1 = (uint8_t)0x10,
  CTRL_COMMISSIONING_PROCESS2 = (uint8_t)0x20,
  CTRL_COMMISSIONING_PROCESS = (uint8_t)0x30,
} ctrl_flag_mask_t;

/*!
 * @ingroup CTRL_cfg
 * @par Description:
 *   This enumerator contains flags masks of enable_send_flag_tamper.
bit0 to enable sending data corrupt event .
bit1 to enable sending Hardware error event.
bit2 to enable sending low battery event.
bit3 to enable sending 1st low credit alarm event.
bit4 to enable sending 2nd low credit alarm event.
bit5 to enable sending no freindly credit event.
bit6 to enable sending  no credit event.
bit7 to enable sending over load event.
bit8 to enable sending over volt event.
bit9 to enable sending under volt event.
bit10 to enable sending open cover tamper event.
bit11 to enable sending open terminal tamper event.
bit12 to enable sending high magntic tamper event. 
bit13 to enable sending reverse tamper event.
bit14 to enable sending falt energy tamper event.
bit15 to enable sending relay error tamper event.
bit16 to enable sending GPRS cover open start tamper event.
bit17 to bit31 are reserved fault energy tamper event.
 */
typedef enum{
    /* Byte 1 */
  CTRL_DATA_CRPT_SEND_ENABLE_FLAG=(uint8_t)0x1,        
  CTRL_HARDWARE_ERR_SEND_ENABLE_FLAG=(uint8_t)0x2,
  CTRL_BTRY_LOW_SEND_ENABLE_FLAG=(uint8_t)0x4,
  CTRL_LOW_CRDT_1ST_ALRM_SEND_ENABLE_FLAG=(uint8_t)0x8,
  CTRL_LOW_CRDT_2ND_ALRM_SEND_ENABLE_FLAG=(uint8_t)0x10,
  CTRL_NO_CRDT_FRNDLY_SEND_ENABLE_FLAG=(uint8_t)0x20,
  CTRL_NO_CRDT_SEND_ENABLE_FLAG=(uint8_t)0x40,
  CTRL_OVER_LOAD_SEND_ENABLE_FLAG=(uint8_t)0x80,
  /* Byte 2 */
  CTRL_OVER_VOLT_SEND_ENABLE_FLAG=(uint8_t)0x1,
  CTRL_UNDER_VOLT_SEND_ENABLE_FLAG=(uint8_t)0x2,
  CTRL_CVR_OPN_SEND_ENABLE_FLAG=(uint8_t)0x4,
  CTRL_TRMNL_OPN_SEND_ENABLE_FLAG=(uint8_t)0x8,
  CTRL_MGNT_HIGH_SEND_ENABLE_FLAG=(uint8_t)0x10,
  CTRL_RVRS_CRNT_SEND_ENABLE_FLAG=(uint8_t)0x20,
  CTRL_FLT_ENRG_SEND_ENABLE_FLAG=(uint8_t)0x40,
  CTRL_RELAY_ERR_SEND_ENABLE_FLAG=(uint8_t)0x80,
    /* Byte 3 */
  CTRL_GPRS_MODULE_CVR_SEND_ENABLE_FLAG=(uint8_t)0x1,
  /* remaining are reserved */
} ctrl_event_enable_send_flag_t; 


/*!
 * @ingroup CTRL_READ
 * @par Description:
 *   This enumerator contains flags masks of control system.
 */
typedef enum
{
  CTRL_OPTICAL = (uint8_t)0x00,
  CTRL_GPRS = (uint8_t)0x01,
  CTRL_RFID = (uint8_t)0x02,
} ctrl_interface_t;


typedef enum{
  ANSI_OPTICAL_POWER_UP = 0,
  ANSI_OPTICAL_NEW_SEC,
  ANSI_OPTICAL_TO,
  ANSI_RESET_COUNTER
  
}OPTICAL_STATE;

typedef enum
{
  SEQ_NORMAL = (uint8_t)0, /*!< Cover open record code in internal memory.*/
  PER_DAILY = (uint8_t)1, /*!< Terminal open record code in internal memory.*/
  PER_MONTH = (uint8_t)2, /*!< Terminal open record code in internal memory.*/
  PER_YEAR = (uint8_t)3, /*!< Terminal open record code in internal memory.*/
} flags_ovr_load_times_cntr_t;

/*!
 * @ingroup CTRL_READ
 * @par Description:
 *   This enumerator contains flags masks of control system.
 */
typedef enum
{
  CTRL_GPRS_NEW_TMPR_FLAG = (uint8_t)0x01,
  CTRL_RFID_NEW_TMPR_FLAG = (uint8_t)0x02,
  NEW_HINT_RLY_ACT_FLAG = (uint8_t)0x10,
} ctrl_new_tmpr_masks_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains all events having non latched actions.
 */
typedef enum
{
  CTRL_DATA_CRPT,
  CTRL_HARDWARE_ERR,
#if (CTRL_BTRY_NON_CHRG == 1)
  CTRL_BTRY_LOW,
#endif
#ifdef PAYMENT_SYS
  CTRL_LOW_CRDT_1ST_ALRM,
#endif
#ifdef PYMT_LOW_TWO_LVL
  CTRL_LOW_CRDT_2ND_ALRM,
#endif
#ifdef PYMT_FRIENDLY
  CTRL_NO_CRDT_FRNDLY,
#endif
#ifdef PAYMENT_SYS
  CTRL_NO_CRDT,
#endif
#ifdef MTR_ENABLE_LMT
  CTRL_OVER_LOAD,
#endif
#ifdef MTR_ENABLE_LMT
  CTRL_OVER_VOLT,
#endif
#ifdef MTR_ENABLE_LMT
  CTRL_UNDER_VOLT,
#endif
#ifdef MTR_THREE_PH
  CTRL_PHS_SEQ,
#endif
  CTRL_NON_LTCH_ACT_EVNT_NUM
} ctrl_event_with_non_latched_actions_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains all events having latched actions.
 */
typedef enum
{
#ifdef CTRL_CVR_SW
  CTRL_CVR_OPN,
#endif
#ifdef CTRL_TRMNL_SW
  CTRL_TRMNL_OPN,
#endif
#ifdef LOCAL_WITH_DLMS
#ifdef MTR_SINGLE_PH
  CTRL_MGNT_HIGH,
#endif
#else
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
  CTRL_MGNT_HIGH,
#endif
#endif
#ifdef MTR_RVS_TMPR
  CTRL_RVRS_CRNT,
#endif
#ifdef MTR_ERTH_TMPR
  CTRL_FLT_ENRG,
#endif
#ifdef MTR_MIS_POT_TMPR
  CTRL_MIS_POT,
#endif
#if defined CTRL_GPRS_MODULE_CVR_SW && defined MTR_THREE_PH
  CTRL_GPRS_MODULE_CVR_OPN,
#endif
  CTRL_LTCH_ACT_EVNT_NUM
} ctrl_event_with_latched_actions_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 0 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_DATA_CRPT_TRF_MASK = (uint8_t)0x01, /*!< Tariff data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_FILE_SYS_MASK = (uint8_t)0x02, /*!< File system data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_MTR_MASK = (uint8_t)0x04, /*!< Metering and ADE data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_DIS_MASK = (uint8_t)0x08, /*!< Display and LCD data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_RF_MASK = (uint8_t)0x10, /*!< RF driver and application data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_GPRS_MASK = (uint8_t)0x20, /*!< GPRS driver and application data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_RFID_MASK = (uint8_t)0x40, /*!< RFID data corrupt tamper status mask.*/
  CTRL_DATA_CRPT_CTRL_MASK = (uint8_t)0x80, /*!< Control system data corrupt tamper status mask.*/
} meter_status0_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 1 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_HRDWRE_ERR_ADE_MASK = (uint8_t)0x02, /*!< ADE communication error tamper status mask.*/
  CTRL_HRDWRE_ERR_ADE_NO_INT_MASK = (uint8_t)0x04, /*!< ADE no interrupt error tamper status mask.*/
  CTRL_HRDWRE_ERR_RF_MASK = (uint8_t)0x08, /*!< RF chip error tamper status mask.*/
  CTRL_HRDWRE_ERR_RFID_MASK = (uint8_t)0x10, /*!< RFID chip error tamper status mask.*/
  CTRL_HRDWRE_ERR_GPRS_MASK = (uint8_t)0x20, /*!< GPRS chip error tamper status mask.*/
  CTRL_HRDWRE_ERR_RELAY_MASK = (uint8_t)0x40, /*!< Relay error status mask.*/
} meter_status1_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 2 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_BTRY_LOW_MASK = (uint8_t)0x01, /*!< Battery low tamper status mask.*/
  CTRL_LOW_CRDT_1ST_ALRM_MASK = (uint8_t)0x02, /*!< Low credit 1st alarm tamper status mask.*/
  CTRL_LOW_CRDT_2ND_ALRM_MASK = (uint8_t)0x04, /*!< Low credit 2nd alarm tamper status mask.*/
  CTRL_NO_CRDT_FRNDLY_MASK = (uint8_t)0x08, /*!< No credit with friendly tamper status mask.*/
  CTRL_NO_CRDT_MASK = (uint8_t)0x10, /*!< No credit tamper status mask.*/
  CTRL_OVR_LOAD_CHA_MASK = (uint8_t)0x20, /*!< Over load Channel A tamper status mask.*/
  CTRL_OVR_LOAD_CHB_MASK = (uint8_t)0x40, /*!< Over load Channel B tamper status mask.*/
  CTRL_OVR_LOAD_CHC_MASK = (uint8_t)0x80, /*!< Over load Channel C tamper status mask.*/
} meter_status2_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 3 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_OVR_VOLT_CHA_MASK = (uint8_t)0x01, /*!< Over volt Channel A tamper status mask.*/
  CTRL_OVR_VOLT_CHB_MASK = (uint8_t)0x02, /*!< Over volt Channel B tamper status mask.*/
  CTRL_OVR_VOLT_CHC_MASK = (uint8_t)0x04, /*!< Over volt Channel C tamper status mask.*/
  CTRL_UNDR_VOLT_CHA_MASK = (uint8_t)0x08, /*!< Under volt Channel A tamper status mask.*/
  CTRL_UNDR_VOLT_CHB_MASK = (uint8_t)0x10, /*!< Under volt Channel B tamper status mask.*/
  CTRL_UNDR_VOLT_CHC_MASK = (uint8_t)0x20, /*!< Under volt Channel C tamper status mask.*/
  CTRL_PHS_SEQ_CHA_MASK = (uint8_t)0x40, /*!< Phase sequence Channel A tamper status mask.*/
  CTRL_PHS_SEQ_CHB_MASK = (uint8_t)0x80, /*!< Phase sequence Channel B tamper status mask.*/
} meter_status3_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 4 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_PHS_SEQ_CHC_MASK = (uint8_t)0x01, /*!< Phase sequence Channel C tamper status mask.*/
  CTRL_CVR_OPN_MASK = (uint8_t)0x02, /*!< Cover open tamper status mask.*/
  CTRL_TRMNL_OPN_MASK = (uint8_t)0x04, /*!< Terminal open tamper status mask.*/
  CTRL_MGNT_HGH_MASK = (uint8_t)0x08, /*!< Magnetic interference high tamper status mask.*/
  CTRL_RVRS_CRNT_CHA_MASK = (uint8_t)0x10, /*!< Reverse current Channel A tamper status mask.*/
  CTRL_RVRS_CRNT_CHB_MASK = (uint8_t)0x20, /*!< Reverse current Channel B tamper status mask.*/
  CTRL_RVRS_CRNT_CHC_MASK = (uint8_t)0x40, /*!< Reverse current Channel C tamper status mask.*/
  CTRL_FLT_ENRG_CHA_MASK = (uint8_t)0x80, /*!< Fault energy Channel A tamper status mask.*/
} meter_status4_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 5 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_FLT_ENRG_CHB_MASK = (uint8_t)0x01, /*!< Fault energy Channel B tamper status mask.*/
  CTRL_FLT_ENRG_CHC_MASK = (uint8_t)0x02, /*!< Fault energy Channel C tamper status mask.*/
  CTRL_MIS_POT_CHA_MASK = (uint8_t)0x04, /*!< Phase sequence Channel A tamper status mask.*/
  CTRL_MIS_POT_CHB_MASK = (uint8_t)0x08, /*!< Phase sequence Channel B tamper status mask.*/
  CTRL_MIS_POT_CHC_MASK = (uint8_t)0x10, /*!< Phase sequence Channel C tamper status mask.*/
  CTRL_GPRS_MODULE_CVR_MASK = (uint8_t)0x20, /*!< GPRS module cover open tamper.*/
  CTRL_MAXIMUM_CONSUMPTION_MASK = (uint8_t)0x40, /*!< GPRS module cover open tamper.*/
  CTRL_CHANGED_TEMPLATE_ID = (uint8_t)0x80, /*!< GPRS module cover open tamper.*/
} meter_status5_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 6 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_RLY_MASK = (uint8_t)0x01, /*!< Relay status mask.*/
  CTRL_BZR_STAT_MASK = (uint8_t)0x02, /*!< Buzzer status mask.*/
  CTRL_ALRM_ICN_MASK = (uint8_t)0x04, /*!< Alarm icon status mask.*/
  CTRL_LEDS_MASK = (uint8_t)0x78, /*!< All LEDs mask.*/
  CTRL_TMPR_LED_MASK = (uint8_t)0x18, /*!< Tamper LED status mask.*/   /* not used*/
  CTRL_TMPR_LED_ON = (uint8_t)0x08, /*!< Tamper LED status is On.*/  
  CTRL_TMPR_LED_FLASHING = (uint8_t)0x10, /*!< Tamper LED status is Flashing.*/
  CTRL_LOW_CRDT_LED_MASK = (uint8_t)0x60, /*!< Low credit LED status mask.*/ /* not used*/
  CTRL_CRDT_LED_ON = (uint8_t)0x20, /*!< Low credit LED status is ON.*/
  CTRL_CRDT_LED_FLASHING = (uint8_t)0x40,  /*!< Low credit LED status is Flashing.*/
  CTRL_PENALTY = (uint8_t)0x80,  /*!< Low credit LED status is Flashing.*/
} meter_status6_set_mask_t;

/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 6 elements' masks. Details in the design document
 */
typedef enum
{      //we need to implement remaining actions in appply and deduce actions //NOUR TO DO****/
  CTRL_DISPLAY_CONTROL = (uint8_t)0, /*!< Display Alarm screen.*/   
  CTRL_RELAY_DISCONNECT_DISPLAY_CONTROL  = (uint8_t)1   /*!< Alarm relay disconnect reason screen.*/ 
}ansi_remaining_actions_t;

typedef enum
{ 
//***** ansi remaining alarms will be flaged in this variable (ansi_remaining_alarms)****/
CTRL_SELF_DIAGNOSTIC_FAIL    = (uint8_t)0,
CTRL_REVERSE                 = (uint8_t)1,
CTRL_EXCEEDING_LMT           = (uint8_t)2,
CTRL_OVER_VOLTAGE            = (uint8_t)3,
CTRL_UNDER_VOLTAGE           = (uint8_t)4,
CTRL_USER_OPEN_RELAY         = (uint8_t)6,
CTRL_CUSTOMER_OPEN_RELAY     = (uint8_t)7,
CTRL_INCORRECT_PASSWORD      = (uint8_t)8,
CTRL_ANSI_ALARMS_NUM
}ctrl_event_ansi_alarms;

typedef enum
{ 
//***** ansi remaining alarms will be flaged in this variable (ansi_remaining_alarms_fatal)****/
  CTRL_MISS_NUETRAL         = (uint8_t) 0, 
  CTRL_ADE_FAIL                 = (uint8_t)  1,
  CTRL_EEPROM_FAIL               = (uint8_t) 2,
  CTRL_RELAY_FAIL                = (uint8_t) 3,
  CTRL_NUM_OF_CODE_NEW           = (uint8_t) 4,
}ctrl_event_ansi_alarms_fatal;

typedef enum
{ 
  RLY_REASON_SELF_DIAGNOSTIC_FAIL      = (uint8_t) 0,
  RLY_REASON_ALARMS_REVERSE            = (uint8_t) 1,
  RLY_REASON_EXCEEDING_LMT             = (uint8_t) 2,
  RLY_REASON_OVER_VOLTAGE              = (uint8_t) 3,
  RLY_REASON_UNDER_VOLTAGE             = (uint8_t) 4,
  RLY_REASON_TST_RELAY                 = (uint8_t) 5, 
  RLY_REASON_USER_OPEN_RELAY           = (uint8_t) 6,
  RLY_REASON_CUSTOMER_OPEN_RELAY       = (uint8_t) 7,
  RLY_REASON_INCORRECT_PASSWORD        = (uint8_t) 8,
  RLY_REASON_MISS_NUETRAL              = (uint8_t) 9, 
  RLY_REASON_ADE_FAIL                  = (uint8_t) 10,
  RLY_REASON_EEPROM_FAIL               = (uint8_t) 11,
  RLY_REASON_RELAY_FAIL                = (uint8_t) 12,
  RLY_REASON_NUM_CODE
}ctrl_relay_open_reason_t;

#ifdef  ANSI_STACK
typedef enum
{
    ALARMS_NO_ACTION                         = (uint16_t)  0x00 ,
    ALARMS_RELAY_DISCONNECT                  = (uint16_t)  0x01 ,
    ALARMS_RELAY_DISCONNECT_LATCH            = (uint16_t)  0x02 ,    //will be replaced by shadow algorithm
    ALARMS_ERR_LED                           = (uint16_t)  0x04 ,
    ALARMS_ERR_LED_LATCH                     = (uint16_t)  0x08 ,   //will be replaced by shadow algorithm
    ALARMS_DISPLAY_CONTROL                   = (uint16_t)  0x10 ,
    ALARMS_RELAY_DISCONNECT_DISPLAY_CONTROL  = (uint16_t)  0x20 ,
    ALARMS_BUZZER                            = (uint16_t)  0x40 ,
    
}AlarmsActions_t;

typedef enum
{
    BIT_RELAY_DISCONNECT,
    BIT_RELAY_DISCONNECT_LATCH,    
    BIT_ERR_LED,
    BIT_ERR_LED_LATCH,
    BIT_DISPLAY_CONTROL,
    BIT_RELAY_DISCONNECT_DISPLAY_CONTROL,
    BIT_BUZZER
    
}AlarmsActionsbit_t;
#endif

/*!
 * @ingroup CTRL_CFG
 * @par Description:
 *   This enumerator contains all available configuration commands used to get
 *   or set configuration data.
 */
typedef enum
{
  CTRL_CFG_SERIAL = (uint8_t)0, /*Configuration serial number of the meter.*/
  CTRL_CFG_ID     = (uint8_t)1, /*!< Configuration ID data.*/
  CTRL_CFG_ACTION = (uint8_t)2, /*!< Configuration actions data.*/
  CTRL_CFG_TIME = (uint8_t)3, /*!< Configuration RTC-time data.*/
  CTRL_CFG_DATE = (uint8_t)4, /*!< Configuration RTC-date data.*/
  CTRL_CFG_BTRY = (uint8_t)5, /*!< Configuration of battery data.*/
  CTRL_CFG_MGNT = (uint8_t)6, /*!< Configuration of magnetic data.*/
  CTRL_CFG_OVR_LOAD = (uint8_t)7, /*!< Configuration of overload periods.*/
  CTRL_CFG_SET_TAMPER_SEND_ENABLE_FLAG=(uint8_t)8,/*!< Configuration of send_enable_flag.*/
  CTRL_CFG_MAX = (uint8_t)8, /*!< Maximum value that config command may take.*/
} ctrl_cfg_cmd_t;

/*!
 * @ingroup CTRL_EVENT
 * @par Description:
 *   This enumerator contains all available event codes used to inform control system
 * by new events.
 */
typedef enum
{
  CTRL_NEW_SEC = (uint8_t)0, /*!< New second event for internal use.*/
  CTRL_METER_DATA_READ = (uint8_t)1, /*!< Reading meter data.*/
  CTRL_PWR_UP = (uint8_t)2, /*!< Power up for internal use.*/
  CTRL_PWR_DN = (uint8_t)3, /*!< Power down for internal use.*/
  CTRL_MD_CLR_SW = (uint8_t)4, /*!< Clear maximum demand by switch.*/
  CTRL_TMPR_RST = (uint8_t)5, /*!< Reset tampers for internal use.*/
  CTRL_MD_RST = (uint8_t)6, /*!< Used to inform control system that tariff received MD reset command.*/
  CTRL_NEW_HR = (uint8_t)9, /*!< New hour event for internal use.*/
  CTRL_NEW_MIN = (uint8_t)10, /*!< New minute event for internal use.*/
  CTRL_DATA_CRPT_MIN = (uint8_t)20, /*!< Minimum value of data corrupt event of any type.*/
  CTRL_DATA_CRPT_TRF = (uint8_t)20, /*!< Tariff and payment data corrupt.*/
  CTRL_DATA_CRPT_FILE_SYS = (uint8_t)21, /*!< File system data corrupt.*/
  CTRL_DATA_CRPT_MTR_ADE = (uint8_t)22, /*!< Metering or ADE data corrupt.*/
  CTRL_DATA_CRPT_DSPLY_LCD = (uint8_t)23, /*!< Display or LCD data corrupt.*/
  CTRL_DATA_CRPT_RF = (uint8_t)24, /*!< RF application or driver data corrupt.*/
  CTRL_DATA_CRPT_GPRS = (uint8_t)25, /*!< GPRS application or driver data corrupt.*/
  CTRL_DATA_CRPT_RFID = (uint8_t)26, /*!< RFID application or driver data corrupt.*/
  CTRL_DATA_CRPT_CTRL = (uint8_t)27, /*!< Control system data corrupt.*/
  CTRL_DATA_CRPT_MAX = (uint8_t)27, /*!< Maximum value of data corrupt event of any type.*/
  CTRL_COMM_ERR_STRT_MIN = (uint8_t)28, /*!< Minimum value of communication error start event of any type.*/
  CTRL_ADE_COMM_ERR_STRT = (uint8_t)29, /*!< ADE communication error start.*/
  CTRL_ADE_DRIVER_ERR_STRT = (uint8_t)30, /*!< ADE driver error start (no interrupt).*/
  CTRL_RF_CHIP_COMM_ERR_STRT = (uint8_t)31, /*!< RF chip communication error start.*/
  CTRL_RFID_CHIP_COMM_ERR_STRT = (uint8_t)32, /*!< RFID chip communication error start.*/
  CTRL_GPRS_CHIP_COMM_ERR_STRT = (uint8_t)33, /*!< GPRS chip communication error start.*/
  CTRL_RELAY_ERR_STRT = (uint8_t)34, /*!< Relay error start.*/
  CTRL_HARDWARE_ERR_STRT_MAX = (uint8_t)34, /*!< Maximum value of communication error start event of any type.*/
  CTRL_NON_LATCH_EVNT_STRT_MIN = (uint8_t)36, /*!< Minimum value of non latched start event of any type.*/
  CTRL_LOW_BTRY_STRT = (uint8_t)36, /*!< Low battery start.*/
  CTRL_LOW_CRDT_1ST_ALRM_STRT = (uint8_t)37, /*!< Low credit 1st alarm start.*/
  CTRL_LOW_CRDT_2ND_ALRM_STRT = (uint8_t)38, /*!< Low credit 2nd alarm start.*/
  CTRL_NO_CRDT_FRNDLY_STRT = (uint8_t)39, /*!< No credit with friendly start.*/
  CTRL_NO_CRDT_STRT = (uint8_t)40, /*!< No credit start.*/
  CTRL_OVR_LOAD_A_STRT = (uint8_t)41, /*!< Over load on channel A start.*/
  CTRL_OVR_LOAD_B_STRT = (uint8_t)42, /*!< Over volt on channel A start.*/
  CTRL_OVR_LOAD_C_STRT = (uint8_t)43, /*!< Under volt on channel A start.*/
  CTRL_OVR_VOLT_A_STRT = (uint8_t)44, /*!< Phase sequence on channel A start.*/
  CTRL_OVR_VOLT_B_STRT = (uint8_t)45, /*!< Over load on channel B start.*/
  CTRL_OVR_VOLT_C_STRT = (uint8_t)46, /*!< Over volt on channel B start.*/
  CTRL_UNDR_VOLT_A_STRT = (uint8_t)47, /*!< Under volt on channel B start.*/
  CTRL_UNDR_VOLT_B_STRT = (uint8_t)48, /*!< Phase sequence on channel B start.*/
  CTRL_UNDR_VOLT_C_STRT = (uint8_t)49, /*!< Over load on channel C start.*/
  CTRL_PHS_SEQ_STRT = (uint8_t)50, /*!< Over volt on channel C start.*/
  CTRL_PHS_SEQ_A_STRT = (uint8_t)50,
  CTRL_NON_LATCH_EVNT_STRT_MAX = (uint8_t)52, /*!< Maximum value of non latched start event of any type.*/
  CTRL_LATCH_EVNT_STRT_MIN = (uint8_t)53, /*!< Minimum value of latched start event of any type.*/
  CTRL_CVR_OPN_STRT = (uint8_t)53, /*!< Cover open start.*/
  CTRL_TRMNL_OPN_STRT = (uint8_t)54, /*!< Terminal open start.*/
  CTRL_MGNT_HIGH_STRT = (uint8_t)55, /*!< Magnetic high start.*/
  CTRL_RVRS_CRNT_A_STRT = (uint8_t)56, /*!< Reverse current on channel A start.*/
  CTRL_RVRS_CRNT_B_STRT = (uint8_t)57, /*!< Fault energy on channel A start.*/
  CTRL_RVRS_CRNT_C_STRT = (uint8_t)58, /*!< Missing potential on channel A start.*/
  CTRL_FLT_ENRG_A_STRT = (uint8_t)59, /*!< Reverse current on channel B start.*/
  CTRL_FLT_ENRG_B_STRT = (uint8_t)59, /*!< Fault energy on channel B start.*/    
  CTRL_FLT_ENRG_STRT = (uint8_t)59, /*!< Fault energy on channel B start.*/
  CTRL_MIS_POT_A_STRT = (uint8_t)62, /*!< Reverse current on channel C start.*/
  CTRL_MIS_POT_B_STRT = (uint8_t)63, /*!< Fault energy on channel C start.*/
  CTRL_MIS_POT_C_STRT = (uint8_t)64, /*!< Missing potential on channel C start.*/
  CTRL_GPRS_MODULE_CVR_STRT = (uint8_t)65, /*!< GPRS module cover open start.*/
  CTRL_TMPR_OVR_LD_STRT= (uint8_t)66,
  CTRL_LATCH_EVNT_STRT_MAX = (uint8_t)66, /*!< Maximum value of latched start event of any type.*/
  CTRL_COMM_ERR_STOP_MIN = (uint8_t)128, /*!< Minimum value of communication error stop event of any type.*/
  CTRL_ADE_COMM_ERR_STOP = (uint8_t)129, /*!< ADE communication error stop.*/
  CTRL_ADE_DRIVER_ERR_STOP = (uint8_t)130, /*!< ADE driver error stop (no interrupt).*/
  CTRL_RF_CHIP_COMM_ERR_STOP = (uint8_t)131, /*!< RF chip communication error stop.*/
  CTRL_RFID_CHIP_COMM_ERR_STOP = (uint8_t)132, /*!< RFID chip communication error stop.*/
  CTRL_GPRS_CHIP_COMM_ERR_STOP = (uint8_t)133, /*!< GPRS chip communication error stop.*/
  CTRL_RELAY_ERR_STOP = (uint8_t)134, /*!< Relay error stop.*/
  CTRL_HARDWARE_ERR_STOP_MAX = (uint8_t)134, /*!< Maximum value of communication error stop event of any type.*/
  CTRL_NON_LATCH_EVNT_STOP_MIN = (uint8_t)136, /*!< Minimum value of non latched stop event of any type.*/
  CTRL_LOW_BTRY_STOP = (uint8_t)136, /*!< Low battery stop.*/
  CTRL_LOW_CRDT_1ST_ALRM_STOP = (uint8_t)137, /*!< Low credit 1st alarm stop.*/
  CTRL_LOW_CRDT_2ND_ALRM_STOP = (uint8_t)138, /*!< Low credit 2nd alarm stop.*/
  CTRL_NO_CRDT_FRNDLY_STOP = (uint8_t)139, /*!< No credit with friendly stop.*/
  CTRL_NO_CRDT_STOP = (uint8_t)140, /*!< No credit stop.*/
  CTRL_OVR_LOAD_A_STOP = (uint8_t)141, /*!< Over load on channel A stop.*/
  CTRL_OVR_LOAD_B_STOP = (uint8_t)142, /*!< Over volt on channel A stop.*/
  CTRL_OVR_LOAD_C_STOP  = (uint8_t)143, /*!< Under volt on channel A stop.*/
  CTRL_OVR_VOLT_A_STOP  = (uint8_t)144, /*!< Phase sequence on channel A stop.*/
  CTRL_OVR_VOLT_B_STOP = (uint8_t)145, /*!< Over load on channel B stop.*/
  CTRL_OVR_VOLT_C_STOP = (uint8_t)146, /*!< Over volt on channel B stop.*/
  CTRL_UNDR_VOLT_A_STOP = (uint8_t)147, /*!< Under volt on channel B stop.*/
  CTRL_UNDR_VOLT_B_STOP  = (uint8_t)148, /*!< Phase sequence on channel B stop.*/
  CTRL_UNDR_VOLT_C_STOP = (uint8_t)149, /*!< Over load on channel C stop.*/
  CTRL_PHS_SEQ_STOP = (uint8_t)150, /*!< Over volt on channel C stop.*/
  CTRL_PHS_SEQ_A_STOP = (uint8_t)150, /*!< Over volt on channel C stop.*/
  CTRL_PHS_SEQ_B_STOP = (uint8_t)151, /*!< Under volt on channel C stop.*/
  CTRL_PHS_SEQ_C_STOP = (uint8_t)152, /*!< Phase sequence on channel C stop.*/
  CTRL_NON_LATCH_EVNT_STOP_MAX = (uint8_t)152, /*!< Maximum value of non latched stop event of any type.*/
  CTRL_LATCH_EVNT_STOP_MIN = (uint8_t)153, /*!< Minimum value of latched stop event of any type.*/
  CTRL_CVR_OPN_STOP = (uint8_t)153, /*!< Cover open stop.*/
  CTRL_TRMNL_OPN_STOP = (uint8_t)154, /*!< Terminal open stop.*/
  CTRL_MGNT_HIGH_STOP = (uint8_t)155, /*!< Magnetic high stop.*/
  CTRL_RVRS_CRNT_A_STOP = (uint8_t)156, /*!< Reverse current on channel A stop.*/
  CTRL_RVRS_CRNT_B_STOP = (uint8_t)157, /*!< Reverse current on channel B stop.*/
  CTRL_RVRS_CRNT_C_STOP  = (uint8_t)158,/*!< Reverse current on channel C stop.*/
  CTRL_FLT_ENRG_STOP = (uint8_t)159, /*!< Fault energy on channel A stop.*/
  CTRL_FLT_ENRG_A_STOP = (uint8_t)159, /*!< Fault energy on channel A stop.*/
  CTRL_FLT_ENRG_B_STOP = (uint8_t)160, /*!< Fault energy on channel B stop.*/
  CTRL_FLT_ENRG_C_STOP  = (uint8_t)161,/*!< Fault energy on channel C stop.*/
  CTRL_MIS_POT_A_STOP = (uint8_t)162, /*!< Missing potential on channel A stop.*/
  CTRL_MIS_POT_B_STOP = (uint8_t)163, /*!< Missing potential on channel B stop.*/
  CTRL_MIS_POT_C_STOP = (uint8_t)164, /*!< Missing potential on channel C stop.*/
  CTRL_GPRS_MODULE_CVR_STOP = (uint8_t)165,/*!< GPRS module cover stop.*/
  CTRL_LATCH_EVNT_STOP_MAX = (uint8_t)166, /*!< Maximum value of latched stop event of any type.*/
  CTRL_SAVE_CRITICAL_DATA = (uint8_t)167, /*!< Save control data.*/
  CTRL_TMPR_OVR_LD_STOP  =   (uint8_t)168,
  CTRL_LOG2_LoadProfile_Event = (uint8_t)172, /*!< load profile need to be read.*/
  CTRL_RELAY_DISCONNECTED = (uint8_t)173,
  CTRL_RELAY_CONNECTED = (uint8_t)174,
  CTRL_NEW_TARIFF_CONFIG = (uint8_t)175,
  CTRL_EVNT_ENTER_OPEN_RELAY_MODE = (uint8_t)176,
  CTRL_EVNT_EXIT_OPEN_RELAY_MODE = (uint8_t)177,
  CTRL_LOAD_WAITING_TARIFF = (uint8_t)178,
  CTRL_ENTER_PENALETY_MODE = (uint8_t)179,
  CTRL_EXIT_PENALETY_MODE = (uint8_t)180,
  CTRL_SET_CT_PT = (uint8_t)181, 
  CTRL_GRACE_HOURS_START= (uint8_t)182, 
  CTRL_WARNING_EXCEED_CONSUMPTION_LIMIT = (uint8_t)183, 
  CTRL_GRACE_HOURS_STOP= (uint8_t)184,
  CTRL_CHANGED_TARIFF_CONFIG = (uint8_t)185,
  DLMS_COMM_FAILURE_METERDISCONNECTED_TIMEOUT = (uint8_t)222,//make a gap in numbers to be able to include anther events when merging from 1ph bluetooth
  DLMS_END_COMMUNICATION_OPTICAL_PORT = (uint8_t)223,
  DLMS_BEGIN_COMMUNICATION_OPTICAL_PORT = (uint8_t)224,
  CTRL_DATA_CRPT_DLMS = (uint8_t)225,
#ifdef ANSI_STACK
  CTRL_SELF_DIAGNOSTIC_STRT         = (uint8_t)226,
  CTRL_USER_OPEN_RELAY_STRT         = (uint8_t)227,
  CTRL_CUSTOMER_OPEN_RELAY_STRT     = (uint8_t)228,
  CTRL_INCORRECT_PASSWORD_STRT      = (uint8_t)229,
  CTRL_SELF_DIAGNOSTIC_STOP         = (uint8_t)230,
  CTRL_USER_OPEN_RELAY_STOP         = (uint8_t)231,
  CTRL_CUSTOMER_OPEN_RELAY_STOP     = (uint8_t)232,
  CTRL_INCORRECT_PASSWORD_STOP      = (uint8_t)233,
  CTRL_MISS_NUETRAL_STRT            = (uint8_t)234,
  CTRL_ADE_FAIL_STRT                = (uint8_t)235,
  CTRL_RELAY_FAIL_STRT              = (uint8_t)236,
  CTRL_MISS_NUETRAL_STOP            = (uint8_t)237,
  CTRL_ADE_FAIL_STOP                = (uint8_t)238,
  CTRL_RELAY_FAIL_STOP              = (uint8_t)239,
#else
#ifdef G2_DM
  CTRL_LOW_CRDT_3RD_ALRM_STRT = (uint8_t)226,
  CTRL_LOW_CRDT_3RD_ALRM_STOP = (uint8_t)227,
  /*** Removed Events and Tampers***/  
  CTRL_CVR_OPN_REMOVED = (uint8_t)228, /*!< Cover open removed.*/
  CTRL_TRMNL_OPN_REMOVED = (uint8_t)229, /*!< Terminal open removed.*/
  CTRL_MGNT_HIGH_REMOVED = (uint8_t)230, /*!< Magnetic high removed.*/
  CTRL_RVRS_CRNT_A_REMOVED = (uint8_t)231, /*!< Reverse current on channel A removed.*/
  CTRL_RVRS_CRNT_B_REMOVED = (uint8_t)232, /*!< Fault energy on channel A removed.*/
  CTRL_RVRS_CRNT_C_REMOVED = (uint8_t)233, /*!< Missing potential on channel A removed.*/
  CTRL_FLT_ENRG_A_REMOVED = (uint8_t)234, /*!< Reverse current on channel B removed.*/
  CTRL_FLT_ENRG_B_REMOVED = (uint8_t)235, /*!< Fault energy on channel B removed.*/    
  CTRL_FLT_ENRG_REMOVED = (uint8_t)236, /*!< Fault energy on channel B removed.*/
  CTRL_GPRS_MODULE_CVR_REMOVED  = (uint8_t)237,
#endif  
#endif
  CTRL_LAST_EVENT,
}ctrl_evnt_code_t;

/*!
 * @ingroup CTRL_EVENT_COUNTER
 * @par Description:
 *   This enumerator contains all available event codes used to inform control system
 * by new events.
 */
typedef enum
{
  OPEN_MODULE_COVER_COUNTER = (uint8_t)0, 
  OPEN_METER_COVER_COUNTER =  (uint8_t)1, 
  OPEN_TERMINAL_COVER_COUNTER = (uint8_t)2, 
  BYPASS = (uint8_t)3, 
  CURRENT_REVERSE_L1_COUNTER = (uint8_t)4, 
  CURRENT_REVERSE_L2_COUNTER = (uint8_t)5, 
  CURRENT_REVERSE_L3_COUNTER = (uint8_t)6, 
  OPEN_BATTERY_COVER_COUNTER = (uint8_t)7, 
  CTRL_LAST_EVENT_COUNTER,
}ctrl_evnt_counter_t;

typedef enum
{
  UNIDIRECTION = (uint8_t)0,
  BIDIRECTION = (uint8_t)1
} meter_mode;
/*!
 * @ingroup CTRL_CMD
 * @par Description:
 *   This enumerator contains all command codes available.
 */
typedef enum
{
  CTRL_TST_RLY = (uint8_t)0, /*!< Test relay.*/
  CTRL_VSUAL_SELF_TEST_START = (uint8_t)1, /*!< Start visual test.*/
  CTRL_VSUAL_SELF_TEST_STOP = (uint8_t)2, /*!< Stop visual test.*/
  CTRL_TST_MD_SW = (uint8_t)3, /*!< Test MD switch.*/
  CTRL_TST_CVR_SW = (uint8_t)4, /*!< Test cover switch.*/
  CTRL_TST_TRMNL_SW = (uint8_t)5, /*!< Test terminal switch.*/
  CTRL_TST_UP_SW = (uint8_t)6, /*!< Test UP switch.*/
  CTRL_TST_DN_SW = (uint8_t)7, /*!< Test DN switch.*/
  CTRL_RST = (uint8_t)8, /*!< Reset control module.*/
  CTRL_RST_TMPR = (uint8_t)9, /*!< Reset tampers.*/
  CTRL_RST_EVNT_LOG = (uint8_t)10, /*!< Reset event log.*/
  CTRL_RST_CFG_METER_LOG = (uint8_t)11, /*!< Reset cofigure meter log.*/
  CTRL_CLR_NEW_TMPR_GPRS = (uint8_t)12, /*!< Clear new tamper flag for GPRS interface.*/
  CTRL_CLR_NEW_TMPR_RFID = (uint8_t)13, /*!< Clear new tamper flag for RFID interface.*/
  CTRL_END_HINT = (uint8_t)14, /*!< End the hint made by buzzer or relay or both.*/
  CTRL_BEEP_TONE0 = (uint8_t)15, /*!< Generate beep with buzzer.*/
  CTRL_BEEP_TONE1 = (uint8_t)16, /*!< Generate beep with buzzer.*/
  CTRL_BEEP_TONE2 = (uint8_t)17, /*!< Generate beep with buzzer.*/
  CTRL_ENTER_OPEN_RELAY_MODE = (uint8_t)18, /*!< Enter open relay mode.*/
  CTRL_EXIT_OPEN_RELAY_MODE = (uint8_t)19, /*!< Enter open relay mode.*/
  CTRL_SET_SERIAL_NUM = (uint8_t)20, /*!< Set the serial number command*/
  CTRL_RESET_UP_SIDE_DOWN_COUNTER= (uint8_t)21, /*!< Reset Up_side_down_counter */
  CTRL_BEEP_TONE3 = (uint8_t)22, /*!< Generate beep with buzzer.*/
} ctrl_cmd_code_t;

typedef enum
{
  CTRL_LOG1_COVER_OPEN = (uint8_t)1, 
  CTRL_LOG1_TERMINAL_OPEN = (uint8_t)2,
  CTRL_LOG1_RELAY_ERR   = (uint8_t)3,
  CTRL_LOG1_RVRS_CRNT_A = (uint8_t)4,
  CTRL_LOG1_RVRS_CRNT_B = (uint8_t)5,
  CTRL_LOG1_RVRS_CRNT_C = (uint8_t)6,
  CTRL_LOG1_FLT_ENRG   = (uint8_t)7,
  CTRL_LOG1_OVER_LOAD_A = (uint8_t)8,
  CTRL_LOG1_OVER_LOAD_B = (uint8_t)9,
  CTRL_LOG1_OVER_LOAD_C = (uint8_t)10,
  CTRL_LOG1_GPRS_COVER = (uint8_t)11,
  CTRL_LOG1_MIS_POT_A = (uint8_t)12,
  CTRL_LOG1_MIS_POT_B = (uint8_t)13,
  CTRL_LOG1_MIS_POT_C = (uint8_t)14,
  CTRL_LOG1_OVR_VOLT_A = (uint8_t)15,
  CTRL_LOG1_OVR_VOLT_B = (uint8_t)16,
  CTRL_LOG1_OVR_VOLT_C = (uint8_t)17,
  CTRL_LOG1_UNDR_VOLT_A = (uint8_t)18,
  CTRL_LOG1_UNDR_VOLT_B = (uint8_t)19,
  CTRL_LOG1_UNDR_VOLT_C = (uint8_t)20,
  CTRL_LOG1_POWER_UP = (uint8_t)21,
  CTRL_LOG1_NUMBER_OF_EVENTS = (uint8_t)22,
  CTRL_LOG1_EVENT_LOG_CLEARED = (uint8_t)255,
} ctrl_evnt_log1_code_t;

typedef enum
{
  CTRL_LOG2_LoadProfile_Ready = (uint8_t)1, 
  CTRL_LOG2_NUMBER_OF_EVENTS = (uint8_t)2,
  CTRL_LOG2_EVENT_LOG_CLEARED = (uint8_t)255,
} ctrl_evnt_log2_code_t;


typedef enum
{
  LOG_ID_1 = 0, // Standards Event Log
  LOG_ID_2,     // Tampering Event Log
  // TODO: Add new log IDs here
  LOG_NUMBER_OF_LOGS,
} TLOG_ID;

typedef enum
{
  COVER_OPEN_EVENT_MASK     = (uint32_t)0x00000001,
  TERMINAL_OPEN_EVENT_MASK  = (uint32_t)0x00000002,
  RELAY_ERR_EVENT_MASK      = (uint32_t)0x00000004,
  RVRS_CRNT_A_EVENT_MASK    = (uint32_t)0x00000008,
  RVRS_CRNT_B_EVENT_MASK    = (uint32_t)0x00000010, 
  RVRS_CRNT_C_EVENT_MASK    = (uint32_t)0x00000020, 
  FLT_ENRG_EVENT_MASK       = (uint32_t)0x00000040,
  OVER_LOAD_A_EVENT_MASK    = (uint32_t)0x00000080,
  OVER_LOAD_B_EVENT_MASK    = (uint32_t)0x00000100,
  OVER_LOAD_C_EVENT_MASK    = (uint32_t)0x00000200,
  GPRS_COVER_MASK           = (uint32_t)0x00000400,
  MIS_POT_A_MASK            = (uint32_t)0x00000800,
  MIS_POT_B_MASK            = (uint32_t)0x00001000,
  MIS_POT_C_MASK            = (uint32_t)0x00002000,
  OVR_VOLT_A_MASK           = (uint32_t)0x00004000,
  OVR_VOLT_B_MASK           = (uint32_t)0x00008000,
  OVR_VOLT_C_MASK           = (uint32_t)0x00010000,
  UNDR_VOLT_A_MASK          = (uint32_t)0x00020000,
  UNDR_VOLT_B_MASK          = (uint32_t)0x00040000,
  UNDR_VOLT_C_MASK          = (uint32_t)0x00080000,
  POWER_UP_MASK             = (uint32_t)0x00100000,
}Event_Notification_log1_masks;

typedef enum
{
  KEYPAD_UP   = (uint8_t)0x01,
  KEYPAD_DOWN = (uint8_t)0x02,
} keypad_sw_t;
/*!
 * @par Description:
 *   This enumerator contains flags masks of control system.
 */
typedef enum
{
  UP_SW_STAT = (uint8_t)0x01, /*!< Up switch state.*/
  DN_SW_STAT = (uint8_t)0x02, /*!< Down switch state.*/
  MD_STAT_MASK = (uint8_t)0x04, /*!< MD switch status mask.*/
  CONT_PRESS_FLAG = (uint8_t)0x08, /*!< Continous press period elapsed flag.*/
  UP_DN_SW_STAT = (uint8_t)(UP_SW_STAT | DN_SW_STAT | CONT_PRESS_FLAG), /*!< STAT of both UP and DOWN switches.*/
  OPEN_RELAY_MODE = (uint8_t)0x20, /*!< Open relay mode mask.*/
  OVER_LOAD_MODE = (uint8_t)0x40,  /*!< Overload mode.*/
} flags_byte1_masks_t;

#ifdef MTR_INDIRECT
typedef enum
{
  CTRL_SWITCHE_CONNECTION_MANUAL = (uint8_t)0x01,
  CTRL_SWITCHE_CONNECTION_AUTO  = (uint8_t)0x02,
} ctrl_connect_sw_type_t;
#endif

#ifdef RECORD_NEW_CUSTOMER_DATE
__packed typedef struct
{
  uint8_t new_customer_inserted_flag;
  uint8_t day;
  uint8_t month;
  uint8_t year;
  uint8_t day_week;
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  //redatodo uint8_t RFU[7];
  uint8_t crc;
} new_customer_t;
#endif

/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for ID configuration.
 * @brief ID configuration.
 */
__packed typedef struct
{
  uint32_t Customer; /*!< Meter ID as an integer ranges from 0 to 999,999,99.*/
  uint16_t dprmnt; /*!< Department ID as an integer.*/
} id_t;

#ifdef CTRL_ACTIONS
/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for tampers configuration.
 * @brief Tampers configuration.
 */
__packed typedef struct
{
  /*! Non latched action taken with each type of tampers. Non latched means that
   the action will be removed when the tamper stops.*/
  uint8_t non_latch_action[CTRL_NON_LTCH_ACT_EVNT_NUM];
  /*! Latched action taken with each type of tampers. Latched means that the action
   won't be removed if the tamper stops.*/
  uint8_t latch_action[CTRL_LTCH_ACT_EVNT_NUM];
#ifdef CTRL_BZR
  uint8_t bzr_silent_period; /*!< Buzzer idle time in minutes if exit hint is called while
  buzzer is ON. @note If time is 0, buzzer will be totally off after only first press. */
#endif
} tmpr_t;
#endif

/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for RTC-time configuration.
 * @brief RTC-time configuration.
 */
__packed typedef struct
{
  uint8_t second; /*!< Hold RTC seconds value now.*/
  uint8_t minute; /*!< Hold RTC minutes value now.*/
  uint8_t hour; /*!< Hold RTC hours value now. @note 24 hour working system.*/
} rtc_time_t;

/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for RTC-date configuration.
 * @brief RTC-date configuration.
 */
__packed typedef struct
{
  uint8_t day; /*!< Hold RTC day value now.*/
  uint8_t month; /*!< Hold RTC month value now.*/
  uint8_t year; /*!< Hold RTC year value now. @note 0 represents year 2000.*/
  uint8_t day_of_week; /*!< Hold RTC day of week value now. @note 0 represents Sunday.*/
} rtc_date_t;


__packed typedef struct
{
rtc_time_t time; /*!< Structure containing RTC-time variables.*/
rtc_date_t date; /*!< Structure containing RTC-date variables.*/
}last_success_comm_date_t;

#if (CTRL_BTRY_NON_CHRG == 1)
/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for battery voltage configuration.
 * @brief Battery configuration.
 */
__packed typedef struct
{
  uint8_t low_lvl; /*!< Battery trigger low level in 10^-1 V.*/
  uint8_t high_lvl; /*!< Battery trigger high level in 10^-1 V.*/
} btry_t;
#endif

#if ((CTRL_MGNT_SENSOR == 1) || (CTRL_MGNT_SW == 1))
/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for magnetic interference configuration.
 * @brief Magnetic interference configuration.
 */
__packed typedef struct
{
#if (CTRL_MGNT_SENSOR == 1)
  uint8_t low_lvl; /*!< Magnetic field intensity low level in 10^-2 Tesla.*/
  uint8_t high_lvl; /*!< Magnetic field intensity high level in 10^-2 Tesla.*/
#endif
  uint8_t detect_time; /*!< Presistance time in minutes of magnetic field
                        * interference above high level to consider as tamper start.*/
  uint8_t restore_time; /*!< Presistance time in minutes of magnetic field
                         * interference below low level to consider as tamper end.*/
} mgnt_t;
#endif

#if (defined(CTRL_RLY) && defined(MTR_ENABLE_LMT))
/*!
 * @ingroup CTRL_CFG
 * One of configuration structures for overload alarm in case of the configured
 * action of overload is to open the relay.
 * @brief Magnetic interference configuration.
 */
__packed typedef struct
{
  uint8_t persistence_times; /*!< Number of times to close the relay before
                              * opening it permanently. If this number is 0,
                              * it will be equivalent  to infinity.*/
  uint8_t open_relay_time; /*!< Time to open the relay in minutes when an
                            * overload alarm starts, ranges (1 - 250).*/
  uint8_t wait_time; /*!< Time to wait a new overload alarm after closing the relay
                      * in minutes, ranges (1 - 250). If no overload alarm happened within
                      * this period after closing the relay, sequence is initiated.*/
} overload_t;
#endif
__packed typedef struct
{
  uint8_t tamper_min;
  uint8_t tamper_hour;
  uint8_t tamper_day;
  uint8_t tamper_month;
  uint8_t tamper_year;
}time_date_t ;

/*!
 * @ingroup CTRL_CFG
 * Structure containing all configuration data.
 * @brief Big configuration structure.
 */
__packed typedef struct
{
  ser_t serial; /*!< Serial number of the meter.*/
  id_t id; /*!< Structure containing meter and department IDs.*/
#ifdef CTRL_ACTIONS
  tmpr_t tmpr; /*!< Structure containing configuration of tampers and actions.*/
#endif
  rtc_time_t reserved1; /*!< Structure containing RTC-time variables.*/
  rtc_date_t reserved2; /*!< Structure containing RTC-date variables.*/
#if (CTRL_BTRY_NON_CHRG == 1)
  btry_t btry; /*!< Structure containing battery configuration variables.*/
#endif
#if ((CTRL_MGNT_SW ==1) || (CTRL_MGNT_SENSOR == 1))
  mgnt_t mgnt; /*!< Structure containing magnetic sensor configuration variables.*/
#endif
#if (defined(CTRL_RLY) && defined(MTR_ENABLE_LMT))
  overload_t overload;
#endif
  uint8_t crc; /*!< Character redundancy check (XOR) of all previous bytes in structure.*/
}ctrl_cfg_t;

/*!
 * @ingroup CTRL_READ
 * Structure containing basic system variables data.
 * @brief Basic system variables structure.
 */
__packed typedef struct
{
  /*! Count of number of tampers happened with non latched actions of each
   * tamper type #ctrl_event_with_non_latched_actions_t.*/
  uint8_t non_latch_evnt_count[CTRL_NON_LTCH_ACT_EVNT_NUM];
  /*! Count of number of tampers with latched actions happened of each
   * tamper type #ctrl_event_with_latched_actions_t.*/
  uint8_t latch_evnt_count[CTRL_LTCH_ACT_EVNT_NUM];
  uint8_t meter_status[8]; /*!< State flags of each tamper and board components.*/
  uint8_t read_data_count; /*!< Number of times meter data was read.*/
  uint16_t md_reset_count; /*!< Number of times MD has been reset.*/
#ifdef RFID_FEATURE
#ifdef ASHNTTI_PROJECT
  uint8_t rfid_unique[4]; /*!< Unique identification used with RFID cards.*/
#else
  uint8_t rfid_unique[10]; /*!< Unique identification used with RFID cards.*/
#endif
#endif
  uint32_t power_on_hour; /*!< Hours that meter is ON by the power line.*/
#ifdef MTR_SINGLE_PH
  uint8_t gprs_signal_strength; /*!< GPRS signal strength.*/
#else
#ifdef GPRS_FEATURE
  uint8_t gprs_signal_strength; /*!< GPRS signal strength.*/
#endif
#endif
#if ((CTRL_BTRY_NON_CHRG == 1)||(CTRL_BTRY_CHRG == 1))
  uint8_t battery_level; /*!< Battery level as percentage.*/
#endif
#if (CTRL_MGNT_SENSOR == 1)
  uint8_t mgnt_level; /*!< Magnetic interference level in multiple of 10 Gauss.*/
#endif
  uint16_t power_int_counter; /*!< Number of power interruptions.*/
} basic_t;

/*!
 * @ingroup CTRL_READ
 * Structure containing all system variables data.
 * @brief Big system variables structure.
*/
__packed typedef struct
{
  basic_t basic; /*!< Basic system variables of control which are copied to software.*/
#ifdef CTRL_BZR
  /*! Seconds counter used to detect idle time.*/
  uint16_t bzr_silent_sec_cntr;
#endif
#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW))
  uint8_t switch_state[4]; /*!< Switches state that are scanned by shift register.*/
#endif
  uint8_t template_id[2]; /*!< ID of the current template that meter is working with.*/

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_MGNT_SW) ||\
  defined(CTRL_MGNT_SENSOR) || defined(MTR_RVS_TMPR) || defined(MTR_ERTH_TMPR)\
    || defined(MTR_MIS_POT_TMPR) || defined(MTR_THREE_PH) || defined(CTRL_GPRS_MODULE_CVR_SW))
  uint16_t tmpr_count; /*!< Total count of tampers (Phase sequence,
  * cover open, terminal open, magnetic high,
  * reverse current, fault energy, missing potential).*/
#endif
#if (defined(RFID_FEATURE) || defined(IEC_62056_21_SLAVE) || defined(GPRS_FEATURE) || defined(RF_LINK_FEATURE))
  uint16_t operator_id; /*!< Last operator ID that configured the meter.*/
#endif
  uint8_t shadow_status[6]; /*!< Shadow status is the meter status combined
                             * with counter of latched tampers and alarms.*/
  uint16_t new_evnt_counter; /*!< Counter of new events since last read.*/
  uint8_t flags; /*!< Flags of control #ctrl_new_tmpr_masks_t.*/
  uint8_t crc; /*!< Character redundancy check (XOR) of all previous bytes in structure.*/
}ctrl_sys_var_t;


/*!
 * @ingroup CTRL_SYS
 * @par Description:
 *   This enumerator contains meter status byte 6 elements' masks. Details in the design document
 */
typedef enum
{
  CTRL_BLOCK_STATE = (uint8_t)0x01, /*!< Relay status mask.*/
  BLOCK_STATE_SC_ERROR = (uint8_t)0x02, /*!< error status mask.*/
  CTRL_BLOCK_CFG = (uint8_t)0x80,
} meter_block_set_mask_t;

/*!
* @par Description:
*   This enumerator contains flags masks of action byte corresponding to each event #tmpr_t.
*/
typedef enum
{
  SR_SWITCHES_MASK = (uint8_t)0x0F, /*!< Switches mask.*/
  SR_CVR_SW = (uint8_t)0x01, /*!< Terminal switch.*/
  SR_TRMNL_SW = (uint8_t)0x02, /*!< Cover switch.*/
  SR_DN_SW = (uint8_t)0x04, /*!< Down switch.*/
  SR_UP_SW = (uint8_t)0x08, /*!< Up switch.*/
  SR_TURN_TMPR_LED_ON = (uint8_t)0x10, /*!< Turn tamper LED ON pattern.*/
  SR_TURN_CRDT_LED_ON = (uint8_t)0x20, /*!< Turn credit LED ON pattern.*/
  SR_TURN_LEDS_ON = (uint8_t)0x30, /*!< Turn LEDs ON pattern.*/
  SR_OPEN_RELAY = (uint8_t)0x80, /*!< Open relay pattern.*/
  SR_CLOSE_RELAY = (uint8_t)0x40, /*!< Close relay pattern.*/
  SR_RELAY_MASK = (uint8_t)0xC0,
} shift_register_pattern_mask_t;


#if 1//def ARABIC_SCREEN_ENABLE


/*!
 * @ingroup CTRL_READ
 * Structure containing all system variables data.
 * @brief Big system variables structure.
*/
__packed typedef struct
{
  uint16_t num_saved_record[FILE_SYS_MAX_LOGS];  /*!< Number of saved records in each log.*/
  uint16_t record_index[FILE_SYS_MAX_LOGS];      /*!< Index of each log.*/
  uint8_t flags_byte; 
  /*!< Bit 0 flag of scan shift register.
  * Bit 1 flag of power up
  * Bit 2 Battery low start
  * Bit 3 Battery high start
  * Bit 4 Magnetic low start
  * Bit 5 Magnetic high start
  * Bit 6 Check battery and magnetic flag
  * Bit 7 We are waiting end hint of relay flag.*/
  uint8_t flags_byte1; /*!< Bit 0 Up switch state
  * Bit 1 Down switch state
  * Bit 2 MD switch status
  * Bit 3 Continous press period elapsed flag
  * Bit 4 Display processing flag while power down.
  * Bit 5 New hint by relay action flag
  * Bit 6 Open relay mode flag.*/
  uint8_t flags_byte2;
  /*!< Bit 0 New second flag   
  * Bit 1 New minute flag
  * Bit 2 New hour flag
  * Bit 3 LCD timeout flag
  * Bit 4 Save in events in internal memory flag
  * Bit 5 Relay error flag
  * Bit 6 Don't enable interrupt with SR scanning flag
  * Bit 7 Ignore result of scanning scrolling switches.*/
  uint8_t ctrl_flags; /*!< Flags of control system.*/
  uint8_t Last_tamper_code;
  uint8_t Last_tamper_min;
  uint8_t Last_tamper_hour;
  uint8_t Last_tamper_day;
  uint8_t Last_tamper_month;
  uint8_t Last_tamper_year;
  uint8_t usage_type;
  uint32_t debit_money;
  uint8_t meter_calcualtion;
  uint8_t tariff_changed;
  uint8_t tariff_prev_num;
  uint32_t zero_consumbtion_tax;
  uint8_t tariff_tax_deduction_mode;
  uint32_t consumed_daily_tax;
  uint32_t acc_nkwh_consumbtion;
  uint8_t flag1;
  uint16_t ctrl_last_operator_id[3];
  int32_t   postpaid_bill;
  uint8_t config_bp_grace_period;
  uint8_t var_grace_period;
  uint32_t friendly_amount_consumed_kwh;
  uint32_t  prv_actv_trf_step_wh[10]; /*!< Consumption of previous each tariff (Wh).*/
  uint32_t  maximum_consumption_value; /*!< Consumption of previous each tariff (Wh).*/
  uint8_t ctrl_maximum_consumptions_action;
  uint16_t ovr_load_in_power_tout;
  uint32_t ovr_load_in_power_value;
  #ifndef G2_DM
  uint32_t vac_prices[10];  /*!< The price of each step in vacation days in 10^-4 of currency / kWh.*/
  #else
  uint8_t prv_month_pf;
  uint8_t RFU1[23];  //it must be 39 but it decrease because FILE_SYS_MAX_LOGS is increase to 9.
  #endif
  uint8_t vac_time;       /*!< Decide if vacation time includes vacation days and weekend days.*/
  uint32_t tax_1k_value;
  uint32_t tax_nk_value;
  uint8_t meter_type; // 0 for ashanti and for amer group
  uint16_t load_profile_configuration_time; // 0 for ashanti and for amer group
  uint16_t maximum_current_value;
  uint8_t serial_prefix[4];
  uint8_t comm_day;
  uint8_t comm_month;
  uint8_t comm_year;
  uint8_t tax_limit;
  uint32_t curr_bp_reactv_varh;
  uint32_t prev_bp_reactv_varh;
  uint8_t tariff_prev_num_2;
  uint8_t meter_calcualtion_2;
  uint8_t ctrl_consumption_alarm_status;
  uint32_t ctrl_consumption_alarm_value;
  uint8_t ctrl_consumption_alarm_action;
  uint32_t ctrl_grace_consumption;
  uint16_t ctrl_remaining_consumption_limit;
  uint16_t ctrl_grace_days_after_recalc_2_var;
  uint16_t ctrl_grace_days_after_recalc_2_cfg;
  uint8_t ctrl_missing_potential_tamper_counter_phase_B;
  uint8_t ctrl_missing_potential_tamper_counter_phase_C;
 // acc_reg_t   tot_current_year_inductive_react_en;            /*!< Used to get current year accumulative inductive reactive in Varh for all channels.*/
//  acc_reg_t   tot_prv_year_inductive_react_en;            /*!< Used to get previous year accumulative inductive reactive energy in Varh for all channels.*/
  uint32_t   mic_rem_tot_current_inductive_react_en;            /*!< Used to get current year accumulative apparent energy in VAH for all channels.*/
  
  uint8_t waiting_meter_calcualtion;
  uint8_t waiting_meter_calcualtion_2;
  uint8_t induction_pf;
  uint16_t tax_nk_value2;
  uint8_t tax_limit2;
  uint16_t acc_nkwh_consumbtion2;
  rtc_time_t rtc_time; /*!< Structure containing RTC-time variables.*/
  rtc_date_t rtc_date; /*!< Structure containing RTC-date variables.*/
#ifdef G2_DM
  uint32_t curr_bp_apparent_va;
  uint32_t prev_bp_apparent_va;
  uint16_t unbalance_current_value;
  uint8_t meter_status_3rd_alarm;
#else
#ifdef ANSI_STD
  uint8_t ansi_prev_day;
  uint8_t RFU2[10];
#else
  uint8_t RFU2[11];
#endif
#endif
  uint8_t iec62056_21_Master_Slave_flag_mode;
  uint8_t No_Of_Norm_Mode_Scr;
  uint8_t No_Of_Test_Mode_Scr;
  uint8_t RFU[1];
  uint8_t crc; /*!< Character redundancy check (XOR) of all previous bytes in structure.*/
}ctrl_sys_var_optional_feature_t;

__packed typedef struct
{
  uint16_t gprs_notifyTamperRepeateTime;  //in minutes
  uint16_t gprs_resetTimeOut;             //in minutes
  uint16_t gprs_waitAckTimeout;           //in minutes
#ifdef GPRS_SEND_NOTIFICATION_FEATURE
#endif
  uint8_t No_of_MnyNotification;  
  uint8_t No_of_BillNotification;
  uint8_t enable_send_flag_tamper[4];
  uint8_t Enable_Send_Notification;/* 0x01 : enable sending new charging notification feature.
                                  0x02 : enable sending new billing notification feature.
                                  0x20 : enable sending load profile notification feature.
                                  0x40 : enable sending new Configuration notification feature.
                                  0x80 : enable sending ct changed notification feature.*/
  uint8_t gprs_flag_notify;    /* 0x01 : for new tamper  //new
                                  0x02 : for new charging
                                  0x04 : for new billing 
                                  0x08 : for new scratch key 
                                  0x10 : differnt meaning : commission or key happened and 60 seconds passed in running gprs state in switch case of gprs_task() 
                                  0x20 : load profile notification 
                                  0x40 : Configuration notification 
                                  0x80 : CT changed notification   */
  uint16_t gprs_notify_time;  //in minutes //new
  uint16_t No_of_LP_Notification;
  uint8_t No_of_Cfg_Notification;
  uint8_t modem_ip[16];
  uint16_t recalc_flags;/*recalc step flag*/
  uint16_t future_recalc_flags;/*new recalc step flags to the next month */
  
  uint8_t  tech_hour[3];
  uint8_t  tech_day[3];
  uint8_t  tech_month[3];
  uint8_t  tech_year[3];
  uint16_t sub_department_id;
  uint16_t new_activity_id;
  uint32_t test_reset_counter[3];
  uint8_t test_interface;
#ifdef G2_DM
  uint32_t G2_DM_ErrCode;
  uint8_t  G2_DM_ErrCodeDateTime[6];
  uint32_t G2_DM_LIMT_CRDT_THRD_ALRM;
  uint8_t G2_DM_LOW_CRDT_3RD_ALRM_STAT;
  uint32_t curr_actv_energy_yearly;
  uint32_t curr_mic_rem_actv_energy_yearly;
  uint32_t curr_react_energy_yearly;
  uint32_t curr_mic_rem_react_energy_yearly;
  uint32_t curr_apparent_energy_yearly;
  uint32_t curr_mic_rem_apparent_energy_yearly;
  uint32_t curr_reverse_actv_energy_yearly;
  uint32_t curr_mic_rem_reverse_actv_energy_yearly;
  uint32_t curr_rev_actv_energy_monthly;
  uint32_t curr_mic_rem_rev_actv_energy_monthly;
  uint16_t prv_3_years[3];
  uint32_t prv_3_years_actv_energy_value[3];
  uint32_t prv_3_years_reactive_energy_value[3];
  uint32_t prv_3_years_apparent_energy_value[3];
  uint32_t prv_3_years_reverse_energy_value[3];
  uint32_t prv_3_years_pf_value[3];
  uint8_t prv_3_years_idx;
  uint8_t curr_month_pf;
  uint32_t curr_year_avg_pf;
  uint32_t max_power_demand_quarter_value;
  mtr_date_time_t mdw_time_quarter;
  uint32_t max_current_demand_quarter_value;
  mtr_date_time_t mda_time_quarter;
  uint8_t frndly_start_time_min;
  uint8_t frndly_end_time_min;
  uint8_t Low_Crdt_3rd_alarm_start;
#else
  
#ifdef ANSI_STACK 
  uint32_t alarm_registerd_value ;
  uint16_t ansi_remaining_alarms;     //NOUR TO DO will implement ansi alarms and actions in case of start of ansi business 
  acc_reg_t tot_gen_act_en; //6 bytes done
  acc_reg_t tot_fwd_act_en; //6 bytes done
  ansi_mic_rem_reg_t mic_rem_reg ; //8b if 1S , 32b if 12s    
#ifdef MTR_THREE_PH   
  acc_reg_t gen_act_en[MTR_NUM_OF_PHASE]; //18 bytes
  acc_reg_t fwd_act_en[MTR_NUM_OF_PHASE]; //18 bytes
  uint8_t RFU1[60];
#else
   uint8_t RFU1[120];
#endif  
#else  
  uint8_t RFU1[150];
#endif//ANSI_STACK
  
#ifdef ANSI_STD
  uint8_t  Table76_over_flow_flg;  
  uint16_t   Table76_last_entry_element;
  uint32_t   Table76_last_entry_seq_num;
  uint16_t   Table76_num_unread_entries;
  uint16_t   Table76_num_valid_entries;
  uint8_t    Table74_over_flow_flg;  
  uint16_t   Table74_last_entry_element;
  uint32_t   Table74_last_entry_seq_num;
  uint16_t   Table74_num_unread_entries;
  uint16_t   Table74_num_valid_entries;
  uint16_t   last_interval_set1;
  uint16_t   last_block_set1;
  uint16_t   last_block_seq_num_set1;
  uint16_t   num_valid_block_set1;
  uint16_t   num_read_block_set1;
  uint16_t   end_reading_flag_volt;
  uint16_t   end_reading_flag_current;
  uint16_t   calibration_counter_std;
  uint16_t   program_counter_std;
    
  uint16_t coin_act_power_index ;
  uint16_t coin_inst_current_index ; 
  uint16_t dem_act_power_index ;
  uint16_t dem_inst_current_index; 
  uint32_t demand_acc_act_power;
  uint32_t demand_acc_inst_current;
   
  uint8_t    RFU0[92];//148              
#endif //ANSI_STD  
#endif //G2_DM
  int32_t STS_using_calc_credit_kwh;
#ifdef G2_DM
  uint8_t start_bzr_silent_period_hr;
  uint8_t end_bzr_silent_period_hr;
  uint8_t start_bzr_silent_period_min;
  uint8_t end_bzr_silent_period_min;
#else
  uint32_t new_indirect_mw_overload_value;
#endif
  uint8_t GM_bootloader_file_CRC;
  uint32_t GM_bootloader_file_length;
#ifdef G2_DM
  uint8_t future_template_id[2];
  uint8_t G2_events_counters1[24];
#else
#ifdef ANSI_STACK
  uint8_t ansi_remaining_fatal_alarms;
  uint8_t ansi_remaining_actions[ALARMS_NUM_OF_CODE]; /** all actions for 4 remaining alarms and 2 remainig action disc relay disp - alarm disp for all alarm*/
  uint8_t ansi_remaining_fatal_actions[CTRL_NUM_OF_CODE_NEW];
  uint8_t meter_mode_byte;                            /**  will be implement in code to set mode of meter**/ 
  uint8_t ade_fail_counter;
  uint8_t miss_neutral_counter;
  uint8_t self_test_counter;
  uint8_t incorrect_password_counter;
  uint8_t reset_meter_counter;
  uint16_t relay_disconnect_reason;
  uint8_t RFU3[3]; //26
#else
 uint8_t RFU3[24]; //24
#endif
#endif
  // uint8_t RFU[187];
  uint8_t crc; /*!< Character redundancy check (CRC) of all previous bytes in structure.*/
} ctrl_sys_var_second_optional_feature_t;

typedef enum {
CTRL_SET_DATE_TIME =(uint8_t)0x01,
CTRL_RESET_ALL_TMPR =(uint8_t)0x02,
CTRL_RESET_SPECIFIC_TMPR = (uint8_t)0x03
}tech_action_flag_t;

/*!
 * @ingroup CTRL_READ
 * Structure containing format of event record.
 * @brief Event record.
 */
__packed typedef struct
{
  uint16_t tech_id; /*!< Event code of this record listed in #ctrl_evnt_code_t.*/
  uint8_t transaction_id[8]; /*!< Current meter status of tampers and board components.*/
  uint8_t minute; /*!< Minute of this record.*/
  uint8_t hour; /*!< Hour of this record.*/
  uint8_t day; /*!< Day of this record.*/
  uint8_t month; /*!< Month of this record.*/
  uint8_t year; /*!< Year ?of this record.*/
} ctrl_removed_tamper_rec_t;

__packed typedef struct
{
  time_date_t  start_event_tamper ;
  time_date_t   End_event_tamper ;
}start_end_event_tamper_t  ;
__packed typedef struct 
{
uint8_t tariff_changed;
uint32_t grace_init_time ;
uint8_t trf_not_make_new_billing_with_new_date_cfg ;
uint8_t trf_make_previous_12_month_recalc_cfg;
uint8_t usage_type;
uint8_t BPH_index;
start_end_event_tamper_t  tamper_event[6][3] ;        /*define start of time and date tamper event */
start_end_event_tamper_t  first_tamper_event[6][3] ;   /*define stop of time and date tamper event */
uint32_t number_of_put_card ;   /* define number of times put card on the meter*/
uint32_t frndly_debit ;       /*value of debit in friendly state*/
time_date_t date_last_charge ; /* date and time of last charge on meter*/
int32_t diff_debit_recalc ;   /*difference debits result due to change recalculations accounts*/
uint32_t tax_customer_serv ;  /*difference debits result due to customer service*/
uint8_t nk_limit_maximum ; /*value of maximum programming load on meter */
uint8_t nk_limit_maximum2 ; /*value of maximum programming load on meter */
uint16_t grace_cfg_monthly;
uint8_t overload_current_value ;
uint8_t block_state;
uint16_t ovr_load_sec_cntr;
//trf_pymt_mny_rec_t trf_chrg_mny_rec; /* Money transaction record structure.*/
#ifdef RECORD_NEW_CUSTOMER_DATE
new_customer_t new_customer; /*!< Structure containing all configuration data.*/
#endif
uint16_t tech_ids[3];
uint8_t tech_ids_index;
uint16_t new_activity;/*recalc step flag*/
uint16_t boot_ver;/*new recalc step flags to the next month */
uint8_t  grace_state;
uint16_t grace_hours_cfg;
uint32_t grace_time_snapshot;
uint32_t grace_time_elapsed;
uint16_t error_contactor;
uint32_t consumption_error_contactor;
//redatodo uint8_t RFU1[1];
uint8_t  Enable_one_time_connect_relay;
uint8_t first_or_last_trf;/*1 ----for first ,0-----for last as old*/
uint8_t tarrif_0_flag;
uint8_t ovr_load_times_cntr_flag;
uint32_t Last_replace_card_seconds;//adjusted_money_with_previous_12_month_calculation;
uint8_t adjusted_money_trf_month;
uint8_t adjusted_money_trf_year;
uint16_t tech_id_time_reset_all_specific[9];  //0---2 tech_id_who set time ,tech_id_who 3---5 reset all temper ,tech_id_who 6--8 reset specific tamper
/* 0--CTRL_RESET_OVER_LOAD_TMPR_TIME_DATE_INDEX , 1--CTRL_RESET_BTRY_LOW_TMPR_TIME_DATE_INDEX ,2--CTRL_RESET_RVRS_CRNT_TMPR_TIME_DATE_INDEX ,3--CTRL_RESET_CVR_OPN_TMPR_TIME_DATE_INDEX ,4--CTRL_RESET_TRMNL_OPN_TMPR_TIME_DATE_INDEX ,5--CTRL_RESET_FLT_ENRG_TMPR_TIME_DATE_INDEX */
time_date_t  last_reset_date_time_tamper[6] ;
uint8_t ovr_load_times_cntr;
uint16_t tech_id_cover_terminal[2];
uint8_t type_operation_last_tech_id;
uint8_t cyshield_only_both;
#ifndef G2_DM
ctrl_removed_tamper_rec_t ctrl_removed_tamper[10];
#else
uint8_t G2_events_counters2[15];
uint8_t G2_tampers_counters[5];
uint32_t G2_tarrif_fees;
uint8_t  tamper_counter_max;
uint8_t  event_counter_max;
uint8_t open_account_flag;
uint8_t sector_code;
uint16_t public_administration;
uint16_t recharge_center_code;
uint8_t phase_type;
uint8_t meter_version;
uint16_t PhaseType;
uint8_t manufacture_code;
uint8_t installation_mode;
uint8_t Noinstall_without_btry;
uint8_t install_with_clear_tampers;
uint64_t max_crdt_limit;
uint64_t max_charg_value_limit;
uint8_t date_time_mode;
uint8_t lab_test_type;
uint8_t saving_energy_days[48];
uint16_t saving_energy_cutoff_time_per_min;
uint16_t saving_energy_hours_from_to;
uint32_t saving_energy_lmt_value;
uint32_t saving_energy_lmt_mode;
uint32_t saving_energy_elapsed_time;
uint32_t saving_energy_snapshot_time;
uint16_t saving_energy_init_time;
uint8_t saving_enrgy_state;
uint8_t prv_meter_status[5];
uint32_t first_tech_code;
uint32_t installation_date_time;
uint8_t RFU1[8];
#endif
uint8_t ctrl_select_which_card_type;
time_date_t last_battery_low_occurance_date_time;

//////////////////////////////////////////////////////////////////
  uint32_t old_trf_cosumption_1 ;
  uint32_t old_trf_bill_1;
  uint16_t new_recalc_flags_1;
  uint8_t trf_mode_1;
  /////////////////////////////////////////////////
  uint8_t ctr_no_bat_flage;
  time_date_t SW_date_last_charge ; /* date and time of last charge on meter*/
  uint8_t MDW_reset_number_month_cfg;
  uint32_t curr_mda2;
  uint32_t curr_mda3;
  uint8_t ctrl_start_open_relay_test;
  uint16_t ctrl_start_open_relay_test_min_cfg;
  uint8_t ctrl_change_dot_matrix_language_flag;
#if (defined G2_DM || defined CAMERON_TENDER)
  uint32_t set_overdraft_frndly_threshold;
#else
  uint8_t RFU1[4];
#endif
#ifdef G2_DM
  uint32_t consumption_error_contactor_money;
  uint8_t start_limit1; 
  uint32_t special_fee_history1;
  uint8_t start_limit2;
  uint32_t special_fee_history2; 
  uint16_t current_tarrif_activity_code;
  uint32_t test_accuracy_config_nw_consumption_cfg;
  uint8_t  test_accuracy_aftr_finsh_flag_cfg;
  uint8_t  remove_tampers_log_count_cfg;   //TODO in code make limit for removing tampers
  uint8_t  remove_tampers_log_count;
  uint8_t  array_tamper_log_flags[28];
  uint8_t RFU[9]; //44
#endif
#ifndef G2_DM
 
#ifdef ANSI_STACK
  uint8_t ansi_meter_id[SIZE_OF_METER_ID];
  uint16_t relay_disconnect_counter;
  uint8_t MFG_serial_number_PCB[TABLE01_MFG_SERIAL_NUMBER_SIZE];
  uint8_t RFU[24];//40
#else
  uint8_t indirect_enclosure_connection_sw_state;
  time_date_t indirect_enclosure_connection_sw_date_time;
  uint16_t SuperCap_Value;
  uint8_t RFU[50];
#endif 
#endif
  uint8_t crc;
}ctrl_sys_new_var_t;
#ifdef MTR_SINGLE_PH
__packed typedef struct
{
  uint8_t Last_tamper_min[10];
  uint8_t Last_tamper_hour[10];
  uint8_t Last_tamper_day[10];
  uint8_t Last_tamper_month[10];
  uint8_t Last_tamper_year[10];
  uint8_t crc;
} ctrl_last_tamper_t;
#else
__packed typedef struct
{
  uint8_t Last_tamper_min[25];
  uint8_t Last_tamper_hour[25];
  uint8_t Last_tamper_day[25];
  uint8_t Last_tamper_month[25];
  uint8_t Last_tamper_year[25];
  uint8_t crc;
} ctrl_last_tamper_t;
#endif
/*!
 * @ingroup CTRL_READ
 * Structure containing format of event record.
 * @brief Event record.
 */
__packed typedef struct
{
  uint8_t evnt_code; /*!< Event code of this record listed in #ctrl_evnt_code_t.*/
  uint8_t meter_status[8]; /*!< Current meter status of tampers and board components.*/
  uint8_t minute; /*!< Minute of this record.*/
  uint8_t hour; /*!< Hour of this record.*/
  uint8_t day; /*!< Day of this record.*/
  uint8_t month; /*!< Month of this record.*/
  uint8_t year; /*!< Year of this record.*/
} ctrl_evnt_rec_t;
#define BTRY_LOG
#ifdef BTRY_LOG
/*!
 * @ingroup CTRL_READ
 * Structure containing format of Battery record.
 * @brief Event record.
 */

__packed typedef struct
{
  uint16_t Battery; /*!< Battery value of this record.*/
  uint8_t Second; /*!< second of this record.*/
  uint8_t minute; /*!< Minute of this record.*/
  uint8_t hour; /*!< Hour of this record.*/
  uint8_t day; /*!< Day of this record.*/
  uint8_t month; /*!< Month of this record.*/
  uint8_t year; /*!< Year of this record.*/
} ctrl_BTRY_rec_t;
#endif



/*!
* @par Description:
*   The structure of a record to be saved in internal EEPROM.
*/
__packed typedef struct
{
  uint8_t write_flag; /*!< Flag that there is data written in memory.*/
  ctrl_evnt_rec_t rec; /*!< Record saved like an event record.*/
} int_mem_rec_t;
/*!
 * @ingroup CTRL_READ
 * Structure containing format of configure meter record.
 * @brief Configure meter record.
 */
__packed typedef struct
{
  uint16_t operator_id; /*!< The ID of the operator who configured the meter.*/
  uint8_t interface; /*!< Interface of communication from which configuration was done.*/
  uint8_t template_id[2]; /*!< Template ID after configuration.*/
  uint8_t minute; /*!< Minute of this record.*/
  uint8_t hour; /*!< Hour of this record.*/
  uint8_t day; /*!< Day of this record.*/
  uint8_t month; /*!< Month of this record.*/
  uint8_t year; /*!< Year of this record.*/
} ctrl_cfg_rec_t;
/*!
 * @par Description:
 *   This enumerator contains flags masks of action byte corresponding to each event #tmpr_t.
 */
typedef enum
{
  ACT_TMPR_LED_MASK = (uint8_t)0x07, /*!< Tamper LED mask.*/
  ACT_TMPR_LED_ON = (uint8_t)0x01, /*!< Turn Tamper LED ON mask.*/
  ACT_TMPR_LED_FLASH = (uint8_t)0x02, /*!< Flash Tamper LED mask.*/
  ACT_CRDT_LED_MASK = (uint8_t)0x07, /*!< Credit LED mask.*/
  ACT_CRDT_LED_ON = (uint8_t)0x03, /*!< Turn credit LED on.*/
  ACT_CRDT_LED_FLASH = (uint8_t)0x04,/*!< Flash credit LED.*/
  ACT_BZR_ON = (uint8_t)0x40, /*!< Buzzer mask.*/
  ACT_ALRM_ICON = (uint8_t)0x08, /*!< Alarm icon mask.*/
  ACT_RLY_MASK = (uint8_t)0x30, /*!< Relay mask.*/
  ACT_OPN_RLY_MASK = (uint8_t)0x10, /*!< Open relay mask.*/
  ACT_HINT_BY_RLY = (uint8_t)0x20, /*!< Hint by relay mask.*/
  ACT_GO_PNLTY_MODE = (uint8_t)0x80, /*!< Go to penalty mode.*/
} action_byte_events_masks;

typedef enum
{
  ACT_TMPR_LED_ON_BIT = (uint8_t)0, /*!< Turn Tamper LED ON mask.*/
  ACT_TMPR_LED_FLASH_BIT = (uint8_t)1, /*!< Flash Tamper LED mask.*/
  ACT_CRDT_LED_FLASH_BIT = (uint8_t)2,/*!< Flash credit LED.*/
  ACT_ALRM_ICON_BIT = (uint8_t)3, /*!< Alarm icon mask.*/
  ACT_OPN_RLY_MASK_BIT = (uint8_t)4, /*!< Open relay mask.*/
  ACT_HINT_BY_RLY_BIT = (uint8_t)5, /*!< Hint by relay mask.*/
  ACT_BZR_ON_BIT = (uint8_t)6, /*!< Buzzer mask.*/
  ACT_GO_PNLTY_MODE_BIT = (uint8_t)7, /*!< Go to penalty mode.*/
} action_prepaid_bit_t;

typedef enum
{
  CVR_OPN_MASK = (uint8_t)0x01,
  TRMNAL_OPN_MASK= (uint8_t)0x02,
  GPRS_MODULE_CVR_MASK = (uint8_t)0x04,
} ctrl_switches_mask_t;

typedef enum
{
  G2_CHECK_SERIAL =0x00,
  G2_SET_SERIAL =0x01,
  G2_GET_SERIAL =0x02,
} open_acc_flag_t;

typedef enum
{
  CHECH_NO_TIME_BACK =0x01,
  G2_CHECH_NO_TIME_BACK =0x02,
} no_time_back_t;


/*!
 * @ingroup CTRL_READ
 * struct to save the date and the time of last successful RF communication
 * @brief save the date and the time of last successful RF communication
 */

/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
extern volatile uint8_t ctrl_flags; /*!< Flags of control system.*/
extern uint8_t LPM_status;
#ifdef G2_DM
extern uint32_t tech_id_temp;
#else
extern uint16_t tech_id_temp;
#endif
extern uint8_t transaction_id_temp[8];
extern uint8_t channel_cpy;
extern  ctrl_sys_new_var_t ctrl_sys_new_var;
extern ctrl_cfg_t ctrl_cfg; /*!< Structure containing all configuration data.*/
extern ctrl_sys_var_t ctrl_sys_var; /*!< Structure containing all system variables.*/
//volatile extern  ctrl_sys_var_optional_feature_t ctrl_sys_var_optional_feature;
extern uint8_t ctrl_tst_mode;
//extern volatile uint8_t ctrl_flags; /*!< Flags of control system.*/
/*! Counter of number of times overload tamper has been repeated during one
* detection period.*/
//extern uint8_t ovr_load_times_cntr;
extern volatile uint16_t ctrl_10ms_timer;
extern uint8_t pwr_up_sec_cntr; /*!< Seconds counter since last power up.*/
extern ctrl_last_tamper_t last_tampers;
extern uint32_t ctrl_Event_Notification_flag;
extern volatile uint8_t HDLC_Activity_Counter;
extern ctrl_sys_var_optional_feature_t ctrl_sys_var_optional_feature;
extern uint8_t ctr_no_bat_flage;
extern uint32_t ctrl_test_relay_second;
extern uint8_t ctrl_test_relay_flag;
extern uint16_t ctrl_test_relay_minutes ;
extern uint8_t ctrl_power_down_event;
extern ctrl_sys_var_second_optional_feature_t ctrl_sys_var_second_optional_feature;
extern unsigned char CurrentTask ;
extern     uint16_t adc_result ;
extern uint8_t uC_speed_high;
#ifdef AC_BUZZER_FEATURE 
extern volatile uint8_t buzzer_enable ;
#endif
extern uint8_t battery_percentage;
extern uint8_t ctrl_high_freq;
extern uint32_t ctrl_start_open_relay_test_sec_counter;
extern uint16_t battery_percentage_fraction;
#ifdef KEYPAD_FEATURE
extern uint8_t keypad_reinit_counter;
#endif
#ifdef GROUP_METERING
extern uint8_t GM_readout_data_timing;
extern uint8_t GM_rise_readout_data_flag;
#endif
#ifdef DOT_MATRIX_LCD_ENABLE
extern uint8_t DOT_MATRIX_reinit_counter;
#ifdef DOT_MATRIX_LOW_PWR_FEATURE
extern uint8_t DM_low_pwr_mode_entering_flag;
#endif
#endif
#ifdef NEW_ASHANTI_PROJECT
extern uint8_t data_is_loaded;
#endif
extern uint8_t prv_sw_state;
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
#ifdef CTRL_EVNT_LOG
	/*! - Get number of saved event records.*/
#define CTRL_GET_NUM_EVNT_REC() file_sys_get_num_record(FILE_SYS_EVENT_RECORD)
	/*! - Clear event log.*/
#define CTRL_CLEAR_EVNT_LOG() {file_sys_clear_log(FILE_SYS_EVENT_RECORD);\
		ctrl_sys_var.new_evnt_counter = 0;}

#ifdef G2_DM
#define CTRL_SAVE_EVNT_REC(record_address,data_ptr)  {ctrl_save_record_DM_G2(record_address, data_ptr);\
		if(~ctrl_sys_var.new_evnt_counter) ctrl_sys_var.new_evnt_counter++;}
#else
	/*! - Save an event log.*/
#define CTRL_SAVE_EVNT_REC(data_ptr) {file_sys_save_record(FILE_SYS_EVENT_RECORD, data_ptr);\
		if(~ctrl_sys_var.new_evnt_counter) ctrl_sys_var.new_evnt_counter++;}
#endif
	/*! - Get an event record.*/
#define CTRL_GET_EVNT_REC(index, data_ptr) file_sys_get_record(FILE_SYS_EVENT_RECORD, index, data_ptr)
	/*! - Get multiple event records.*/
#define CTRL_GET_EVNT_MLTI_REC(from, num, data_ptr) file_sys_get_mlti_rec(\
                                                                          FILE_SYS_EVENT_RECORD, from, num, data_ptr)

#endif
#define CTRL_SW_STATE_READ(DATA_PTR)     file_sys_read(DATA_PTR, FILE_SYS_GET_COM_GPRS_DATA_ADRS+254, 1)
#define CTRL_SW_STATE_RESET(DATA_PTR)    file_sys_write(DATA_PTR, FILE_SYS_GET_COM_GPRS_DATA_ADRS+254, 1)
#define CTRL_SW_STATE_STORE(DATA_PTR)    prv_sw_state=0; CTRL_SW_STATE_READ(&prv_sw_state); (*DATA_PTR)|=prv_sw_state; file_sys_write(DATA_PTR, FILE_SYS_GET_COM_GPRS_DATA_ADRS+254, 1);

/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
/*!
 * @addtogroup CTRL_READ
 * @{
 */
/*! Read hint by relay status.*/
#define CTRL_READ_HINT_BY_RLY_STAT (ctrl_sys_var_optional_feature.ctrl_flags & CTRL_RELAY_HINT_MASK)
/*! Read Template ID.*/
#define CTRL_READ_TMPLT_ID(data_ptr) memcpy(data_ptr, ctrl_sys_var.template_id, sizeof(ctrl_sys_var.template_id))
/*! Read current seconds from RTC (0-59).*/
#define CTRL_READ_SEC()  ctrl_sys_var_optional_feature.rtc_time.second
/*! Read current minute from RTC (0-59).*/
#define CTRL_READ_MIN()  ctrl_sys_var_optional_feature.rtc_time.minute
/*! Read current minute from RTC (0-23).*/
#define CTRL_READ_HR()   ctrl_sys_var_optional_feature.rtc_time.hour
/*! Read current minute from RTC (1-31).*/
#define CTRL_READ_DAY() ctrl_sys_var_optional_feature.rtc_date.day
/*! Read current month from RTC (1-12).*/
#define CTRL_READ_MONTH() ctrl_sys_var_optional_feature.rtc_date.month
/*! Read current year from RTC (0 - 255).*/
#define CTRL_READ_YEAR() ctrl_sys_var_optional_feature.rtc_date.year
/*! Read current day of week from RTC (0 Sunday - 6 Saturday).*/
#define CTRL_READ_DAYOW() ctrl_sys_var_optional_feature.rtc_date.day_of_week

#ifdef RECORD_NEW_CUSTOMER_DATE
#define CTRL_READ_NEW_CUSTOMER_SEC()      ctrl_sys_new_var.new_customer.sec
#define CTRL_READ_NEW_CUSTOMER_MIN()      ctrl_sys_new_var.new_customer.min
#define CTRL_READ_NEW_CUSTOMER_HOUR()     ctrl_sys_new_var.new_customer.hour
#define CTRL_READ_NEW_CUSTOMER_DAY()      ctrl_sys_new_var.new_customer.day
#define CTRL_READ_NEW_CUSTOMER_MONTH()    ctrl_sys_new_var.new_customer.month
#define CTRL_READ_NEW_CUSTOMER_YEAR()     ctrl_sys_new_var.new_customer.year
#define CTRL_READ_NEW_CUSTOMER_DAY_WEEK() ctrl_sys_new_var.new_customer.day_week
#endif

#define CTRL_DATA_CRPT_STAT   (ctrl_sys_var.basic.meter_status[0])
/*! Read the Boolean status of data corrupt of tariff.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_TRF_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_TRF_MASK)
/*! Read the Boolean status of data corrupt of file system.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_FILE_SYS_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_FILE_SYS_MASK)
/*! Read the Boolean status of data corrupt of metering.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_MTR_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_MTR_MASK)
/*! Read the Boolean status of data corrupt of display.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_DIS_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_DIS_MASK)
/*! Read the Boolean status of data corrupt of RF MBUS.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_RF_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_RF_MASK)
/*! Read the Boolean status of data corrupt of GPRS.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_GPRS_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_GPRS_MASK)
/*! Read the Boolean status of data corrupt of control system.0, no data corrupt. Other, there is data corrupt.*/
#define CTRL_DATA_CRPT_CTRL_STAT (ctrl_sys_var.basic.meter_status[0] & CTRL_DATA_CRPT_CTRL_MASK)
/*! Read the Boolean status of ADE communication error.0, no error . Other, there is an error.*/
#define CTRL_HRDWARE_ERR_ADE_STAT (ctrl_sys_var.basic.meter_status[1] & CTRL_HRDWRE_ERR_ADE_MASK)
/*! Read the Boolean status of ADE no interrupt error.0, no error . Other, there is an error.*/
#define CTRL_HRDWARE_ERR_ADE_NO_INT_STAT (ctrl_sys_var.basic.meter_status[1] & CTRL_HRDWRE_ERR_ADE_NO_INT_MASK)
/*! Read the Boolean status of hardware error of RF MBUS.0, no error . Other, there is an error.*/
#define CTRL_HRDWARE_ERR_RF_STAT (ctrl_sys_var.basic.meter_status[1] & CTRL_HRDWRE_ERR_RF_MASK)
/*! Read the Boolean status of hardware error of RFID.0, no error . Other, there is an error.*/
#define CTRL_HRDWARE_ERR_RFID_STAT (ctrl_sys_var.basic.meter_status[1] & CTRL_HRDWRE_ERR_RFID_MASK)
/*! Read the Boolean status of hardware error of GPRS.0, no error . Other, there is an error.*/
#define CTRL_HRDWARE_ERR_GPRS_STAT (ctrl_sys_var.basic.meter_status[1] & CTRL_HRDWRE_ERR_GPRS_MASK)
/*! Read the Boolean status of hardware error of relay.0, no error . Other, there is an error.*/
#define CTRL_HRDWARE_ERR_RELAY_STAT (ctrl_sys_var.basic.meter_status[1] & CTRL_HRDWRE_ERR_RELAY_MASK)
#if((CTRL_BTRY_NON_CHRG == 1)||(CTRL_BTRY_CHRG == 1))
/*! Read the Boolean status of battery.0, low level. Other, high level*/
#define CTRL_BTRY_STAT (!(ctrl_sys_var.basic.meter_status[2] & CTRL_BTRY_LOW_MASK))
#define CTRL_BTRY_OCCURED (!(ctrl_sys_var.shadow_status[2] & CTRL_BTRY_LOW_MASK))
/*! Read the battery value as percentage from the full voltage value. (0 - 100).*/
#ifdef ASHNTTI_PROJECT
#define CTRL_BTRY_VALUE battery_percentage
#else
#define CTRL_BTRY_VALUE ctrl_sys_var.basic.battery_level
#endif
#endif
#ifdef PAYMENT_SYS
/*! Read the Boolean status of 1st alarm of low credit.0, no alarm. Other, there is an alarm.*/
#define CTRL_LOW_CRDT_1ST_ALRM_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_LOW_CRDT_1ST_ALRM_MASK)
#define CTRL_LOW_CRDT_1ST_ALRM_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_LOW_CRDT_1ST_ALRM_MASK)
#endif
#ifdef PYMT_LOW_TWO_LVL
/*! Read the Boolean status of 2nd alarm of low credit.0, no alarm. Other, there is an alarm.*/
#define CTRL_LOW_CRDT_2ND_ALRM_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_LOW_CRDT_2ND_ALRM_MASK)
#define CTRL_LOW_CRDT_2ND_ALRM_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_LOW_CRDT_2ND_ALRM_MASK)
#endif
#ifdef G2_DM
#define CTRL_LOW_CRDT_3RD_ALRM_STAT     (ctrl_sys_var_optional_feature.meter_status_3rd_alarm & (1<<LOW_CRDT_3RD_ALRM_BIT) )//(ctrl_sys_var_second_optional_feature.G2_DM_LOW_CRDT_3RD_ALRM_STAT = !(trf_pymt_var.pymt_data.money_balance <= ctrl_sys_var_second_optional_feature.G2_DM_LIMT_CRDT_THRD_ALRM))
#define Apply_Action_LOW_CRDT_3RD_ALRM_START        ctrl_sys_var_second_optional_feature.Low_Crdt_3rd_alarm_start            
#define Apply_Action_LOW_CRDT_3RD_ALRM_STOP         0
#define LOW_CRDT_3RD_ALRM_BIT                       3
#endif

#ifdef PYMT_FRIENDLY
/*! Read the Boolean status of no credit with friendly.0, no alarm. Other, there is an alarm.*/
#define CTRL_NO_CRDT_FRNDLY_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_NO_CRDT_FRNDLY_MASK)
#define CTRL_NO_CRDT_FRNDLY_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_NO_CRDT_FRNDLY_MASK)
#endif
#ifdef PAYMENT_SYS
/*! Read the Boolean status of no credit.0, no alarm. Other, there is an alarm.*/
#define CTRL_NO_CRDT_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_NO_CRDT_MASK)
#define CTRL_NO_CRDT_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_NO_CRDT_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of over load channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_OVR_LOAD_CHA_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_OVR_LOAD_CHA_MASK)
#define CTRL_OVR_LOAD_CHA_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_OVR_LOAD_CHA_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of over load channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_OVR_LOAD_CHB_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_OVR_LOAD_CHB_MASK)
#define CTRL_OVR_LOAD_CHB_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_OVR_LOAD_CHB_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of over load channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_OVR_LOAD_CHC_STAT (ctrl_sys_var.basic.meter_status[2] & CTRL_OVR_LOAD_CHC_MASK)
#define CTRL_OVR_LOAD_CHC_OCCURED (ctrl_sys_var.shadow_status[2] & CTRL_OVR_LOAD_CHC_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of over volt channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_OVR_VOLT_CHA_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_OVR_VOLT_CHA_MASK)
#define CTRL_OVR_VOLT_CHA_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_OVR_VOLT_CHA_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of over volt channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_OVR_VOLT_CHB_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_OVR_VOLT_CHB_MASK)
#define CTRL_OVR_VOLT_CHB_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_OVR_VOLT_CHB_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of over volt channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_OVR_VOLT_CHC_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_OVR_VOLT_CHC_MASK)
#define CTRL_OVR_VOLT_CHC_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_OVR_VOLT_CHC_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of under volt channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_UNDR_VOLT_CHA_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_UNDR_VOLT_CHA_MASK)
#define CTRL_UNDR_VOLT_CHA_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_UNDR_VOLT_CHA_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of under volt channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_UNDR_VOLT_CHB_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_UNDR_VOLT_CHB_MASK)
#define CTRL_UNDR_VOLT_CHB_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_UNDR_VOLT_CHB_MASK)
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the Boolean status of under volt channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_UNDR_VOLT_CHC_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_UNDR_VOLT_CHC_MASK)
#define CTRL_UNDR_VOLT_CHC_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_UNDR_VOLT_CHC_MASK)
#endif
#ifdef MTR_THREE_PH
/*! Read the Boolean status of phase sequence channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_PHASE_SEQ_CHA_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_PHS_SEQ_CHA_MASK)
#define CTRL_PHASE_SEQ_CHA_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_PHS_SEQ_CHA_MASK)
#endif
#ifdef MTR_THREE_PH
/*! Read the Boolean status of phase sequence channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_PHASE_SEQ_CHB_STAT (ctrl_sys_var.basic.meter_status[3] & CTRL_PHS_SEQ_CHB_MASK)
#define CTRL_PHASE_SEQ_CHB_OCCURED (ctrl_sys_var.shadow_status[3] & CTRL_PHS_SEQ_CHB_MASK)
#endif
#ifdef MTR_THREE_PH
/*! Read the Boolean status of phase sequence channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_PHASE_SEQ_CHC_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_PHS_SEQ_CHC_MASK)
#define CTRL_PHASE_SEQ_CHC_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_PHS_SEQ_CHC_MASK)
#endif
#ifdef CTRL_CVR_SW
/*! Read the Boolean status of cover open.0, cover closed. Other, cover is open.*/
#define CTRL_CVR_OPN_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_CVR_OPN_MASK)
#define CTRL_CVR_OPN_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_CVR_OPN_MASK)
#endif
#ifdef CTRL_TRMNL_SW
/*! Read the Boolean status of termonal open.0, terminal closed. Other, terminal is open.*/
#define CTRL_TRMNL_OPN_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_TRMNL_OPN_MASK)
#define CTRL_TRMNL_OPN_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_TRMNL_OPN_MASK)
#endif
#if ((CTRL_MGNT_SW ==1) || (CTRL_MGNT_SENSOR == 1))
/*! Read the Boolean status of alarms. 0, low magnetic interference level. Other, high magnetic level.*/
#define CTRL_MGNT_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_MGNT_HGH_MASK)
#define CTRL_MGNT_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_MGNT_HGH_MASK)
#endif
#if (CTRL_MGNT_SENSOR == 1)
/*! Read the magnetic interference applied on meter value in in 10-2 Tesla. (0 - 255).*/
#define CTRL_MGNT_VALUE ctr_sys_var.basic.mgnt_value
#endif
#ifdef MTR_RVS_TMPR
/*! Read the Boolean status of reverse current channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_RVRS_CRNT_CHA_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_RVRS_CRNT_CHA_MASK)
#define CTRL_RVRS_CRNT_CHA_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_RVRS_CRNT_CHA_MASK)
/*! Read the Boolean status of latched reverse current channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_RVRS_CRNT_CHA_LATCH_STAT (ctrl_sys_var.shadow_status[4] & CTRL_RVRS_CRNT_CHA_MASK)
#endif
#ifdef MTR_RVS_TMPR
/*! Read the Boolean status of reverse current channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_RVRS_CRNT_CHB_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_RVRS_CRNT_CHB_MASK)
#define CTRL_RVRS_CRNT_CHB_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_RVRS_CRNT_CHB_MASK)
/*! Read the Boolean status of latched reverse current channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_RVRS_CRNT_CHB_LATCH_STAT (ctrl_sys_var.shadow_status[4] & CTRL_RVRS_CRNT_CHB_MASK)
#endif
#ifdef MTR_RVS_TMPR
/*! Read the Boolean status of reverse current channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_RVRS_CRNT_CHC_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_RVRS_CRNT_CHC_MASK)
#define CTRL_RVRS_CRNT_CHC_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_RVRS_CRNT_CHC_MASK)
/*! Read the Boolean status of latched reverse current channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_RVRS_CRNT_CHC_LATCH_STAT (ctrl_sys_var.shadow_status[4] & CTRL_RVRS_CRNT_CHC_MASK)
#endif
#ifdef MTR_ERTH_TMPR
/*! Read the Boolean status of fault energy channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_FLT_ENRG_CHA_STAT (ctrl_sys_var.basic.meter_status[4] & CTRL_FLT_ENRG_CHA_MASK)
#define CTRL_FLT_ENRG_CHA_OCCURED (ctrl_sys_var.shadow_status[4] & CTRL_FLT_ENRG_CHA_MASK)
#endif
#ifdef MTR_ERTH_TMPR
/*! Read the Boolean status of fault energy channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_FLT_ENRG_CHB_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_FLT_ENRG_CHB_MASK)
#define CTRL_FLT_ENRG_CHB_OCCURED (ctrl_sys_var.shadow_status[5] & CTRL_FLT_ENRG_CHB_MASK)
#endif
#ifdef MTR_ERTH_TMPR
/*! Read the Boolean status of fault energy channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_FLT_ENRG_CHC_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_FLT_ENRG_CHC_MASK)
#define CTRL_FLT_ENRG_CHC_OCCURED (ctrl_sys_var.shadow_status[5] & CTRL_FLT_ENRG_CHC_MASK)
#endif
#if 1//def MTR_MIS_POT_TMPR
/*! Read the Boolean status of missing potential channel A.0, no alarm. Other, there is an alarm.*/
#define CTRL_MIS_POT_CHA_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_MIS_POT_CHA_MASK)
#define CTRL_MIS_POT_CHA_OCCURED (ctrl_sys_var.shadow_status[5] & CTRL_MIS_POT_CHA_MASK)
#endif
#if 1//def MTR_MIS_POT_TMPR
/*! Read the Boolean status of missing potential channel B.0, no alarm. Other, there is an alarm.*/
#define CTRL_MIS_POT_CHB_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_MIS_POT_CHB_MASK)
#define CTRL_MIS_POT_CHB_OCCURED (ctrl_sys_var.shadow_status[5] & CTRL_MIS_POT_CHB_MASK)
#endif
#if 1//def MTR_MIS_POT_TMPR
/*! Read the Boolean status of missing potential channel C.0, no alarm. Other, there is an alarm.*/
#define CTRL_MIS_POT_CHC_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_MIS_POT_CHC_MASK)
#define CTRL_MIS_POT_CHC_OCCURED (ctrl_sys_var.shadow_status[5] & CTRL_MIS_POT_CHC_MASK)
#endif
#if 1//def CTRL_GPRS_MODULE_CVR_SW
/*! Read the Boolean status of GPRS cover open.0, cover closed. Other, GPRS cover is open.*/
#define CTRL_GPRS_MODULE_CVR_OPN_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_GPRS_MODULE_CVR_MASK)
#define CTRL_GPRS_MODULE_CVR_OPN_OCCURED (ctrl_sys_var.shadow_status[5] & CTRL_GPRS_MODULE_CVR_MASK)

#endif
#define CTRL_GENERATION_MODE_STAT (ctrl_sys_var.basic.meter_status[5] & CTRL_GENERATION_MODE_MASK)


#define CTRL_CHANGED_TEMPLATE_ID_OCCURED (ctrl_sys_var.basic.meter_status[5] & CTRL_CHANGED_TEMPLATE_ID)

#define CTRL_CHANGED_TEMPLATE_ID_START {ctrl_sys_var.basic.meter_status[5] |= CTRL_CHANGED_TEMPLATE_ID ; ctrl_sys_var.shadow_status[5] |= CTRL_CHANGED_TEMPLATE_ID;}
#define CTRL_CHANGED_TEMPLATE_ID_RESET {ctrl_sys_var.basic.meter_status[5] &= ~CTRL_CHANGED_TEMPLATE_ID ; ctrl_sys_var.shadow_status[5] &= ~CTRL_CHANGED_TEMPLATE_ID;}


#ifdef CTRL_RLY
/*! Read the Boolean status of relay. 0, relay is closed. Other, relay is open.*/
#define CTRL_RLY_STAT (ctrl_sys_var.basic.meter_status[6] & CTRL_RLY_MASK)
#else 
#define CTRL_RLY_STAT  0
#endif
#define CTRL_TEST_MODE              TEST //TODO Remove this and put the right handling function 
#ifdef CTRL_BZR
/*! Read the Boolean status of buzzer. 0, OFF. Other, ON.*/
#define CTRL_BZR_STAT (ctrl_sys_var.basic.meter_status[6] & CTRL_BZR_STAT_MASK)
#endif
#ifdef CTRL_ACTIONS
/*! Read the Boolean status of alarm icon. 0, no alarms. Other, There is an alarm at least.*/
#define CTRL_ALRM_STAT (ctrl_sys_var.basic.meter_status[6] & CTRL_ALRM_ICN_MASK)
#endif


#ifdef ANSI_STACK
/*! Read meter ID as an integer.*/
#define ANSI_CTRL_CUSTOMER_ID  ctrl_sys_new_var.ansi_meter_id
#endif

/*! Read meter ID as an integer.*/
#define CTRL_CUSTOMER_ID ctrl_cfg.id.Customer
/*! Read department ID as an integer.*/
#define CTRL_DPRT_ID ctrl_cfg.id.dprmnt
/*! Read the number of times meter data has been read.*/
#define CTRL_DATA_READ_COUNT() ctrl_sys_var.basic.read_data_count
/*! Read the power status. 0, if the power is up. Other, if the power is down.*/
#ifdef MTR_THREE_PH
#define CTRL_PWR_DN_STAT  0/*(ctrl_flags & CTRL_PWR_DWN_MASK) */
#else
#define CTRL_PWR_DN_STAT ((ctrl_flags & CTRL_PWR_DWN_MASK) || (LPM_status))
#endif
/*! Read the number of times data corrupt occurred.*/
#define CTRL_DATA_CRPT_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_DATA_CRPT]
/*! Read the number of times that hardware errors occurred.*/
#define CTRL_HRDWARE_ERR_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_HARDWARE_ERR]
/*! Read the number of times that cover was open.*/
#define CTRL_CVR_OPN_COUNT ctrl_sys_var.basic.latch_evnt_count[CTRL_CVR_OPN]
/*! Read the number of times that terminal was open.*/
#define CTRL_TRMNL_OPN_COUNT ctrl_sys_var.basic.latch_evnt_count[CTRL_TRMNL_OPN]
#if (CTRL_BTRY_NON_CHRG == 1)
/*! Read the number of times that battery went low.*/
#define CTRL_BTRY_LOW_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_BTRY_LOW]
#endif
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
/*! Read the number of times of high magnetic interference.*/
#define CTRL_MGNT_HIGH_COUNT ctrl_sys_var.basic.latch_evnt_count[CTRL_MGNT_HIGH]
#endif
#ifdef PAYMENT_SYS
/*! Read the number of times that low credit first alarm happened.*/
#define CTRL_LOW_CRDT_COUNT0 ctrl_sys_var.basic.non_latch_evnt_count[CTRL_LOW_CRDT_1ST_ALRM]
#endif
#ifdef PYMT_LOW_TWO_LVL
/*! Read the number of times that low credit second alarm happened.*/
#define CTRL_LOW_CRDT_COUNT1 ctrl_sys_var.basic.non_latch_evnt_count[CTRL_LOW_CRDT_2ND_ALRM]
#endif
#ifdef PYMT_FRIENDLY
/*! Read the number of times that no credit with friendly event happened.*/
#define CTRL_NO_CRDT_FRN_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_NO_CRDT_FRNDLY]
#endif
#ifdef PAYMENT_SYS
/*! Read the number of times that no credit event happened.*/
#define CTRL_NO_CRDT_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_NO_CRDT]
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the number of times that over load tamper happened.*/
#define CTRL_OVR_LOAD_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_OVER_LOAD]
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the number of times that over volt tamper happened.*/
#define CTRL_OVR_VLT_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_OVER_VOLT]
#endif
#ifdef MTR_ENABLE_LMT
/*! Read the number of times that under volt tamper happened.*/
#define CTRL_UNDR_VLT_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_UNDER_VOLT]
#endif
#ifdef MTR_RVS_TMPR
/*! Read the number of times that reverse current tamper happened.*/
#define CTRL_RVRS_CRNT_COUNT ctrl_sys_var.basic.latch_evnt_count[CTRL_RVRS_CRNT]
#endif
#ifdef MTR_MIS_POT_TMPR
/*! Read the number of times that fault energy tamper happened.*/
#define CTRL_FLT_ENRG_COUNT ctrl_sys_var.basic.latch_evnt_count[CTRL_FLT_ENRG]
#endif
#ifdef MTR_MIS_POT_TMPR
/*! Read the number of times that missing potential tamper happened.*/
#define CTRL_MISS_POT_COUNT ctrl_sys_var.basic.latch_evnt_count[CTRL_MIS_POT]
#endif
#ifdef MTR_THREE_PH
/*! Read the number of times that phase sequence tamper happened.*/
#define CTRL_PHS_SEQ_COUNT ctrl_sys_var.basic.non_latch_evnt_count[CTRL_PHS_SEQ]
#endif
/*! Read the new tamper status of meter since last read for data.
 * 0, if no new tampers. Other, if there is new tampers*/
#define CTRL_GPRS_NEW_TMP_STAT (ctrl_sys_var.flags & CTRL_GPRS_NEW_TMPR_FLAG)
/*! Read the new tamper status of meter since last read for data.
 * 0, if no new tampers. Other, if there is new tampers*/
#define CTRL_RFID_NEW_TMP_STAT (ctrl_sys_var.flags & CTRL_RFID_NEW_TMPR_FLAG)
#ifdef CTRL_EVNT_LOG
/*! Read number of unread records.*/
#define CTRL_READ_NUM_UNREAD_REC() ctrl_sys_var.new_evnt_counter
/*! Read the size of an event record in Bytes.*/
#ifdef G2_DM
#define CTRL_READ_SIZE_EVNT_REC() sizeof(G2_DM_Events_Log_type)
#define CTRL_READ_SIZE_TAMPER_REC()  sizeof(G2_DM_Tampers_Log_type)
#define CTRL_READ_SIZE_REMOVED_TAMPER_REC()  sizeof(G2_DM_Removed_Tampers_Log_type)
#define CTRL_READ_SIZE_YEARLY_CURRENT_MD_REC()  sizeof(G2_yearly_current_demand_Log_type)
#define CTRL_READ_SIZE_YEARLY_POWER_MD_REC()  sizeof(G2_yearly_power_demand_Log_type)
#else
#define CTRL_READ_SIZE_EVNT_REC() sizeof(ctrl_evnt_rec_t)
#endif
#endif
#ifdef CTRL_CFG_METER_LOG
/*! Read the size of a configure meter record in bytes.*/
#define CTRL_READ_SIZE_CFG_REC() sizeof(ctrl_cfg_rec_t)
#endif
#ifdef RFID_FEATURE
/*! Read the address of the string of unique ID of RFID cards used with this meter.*/
#define CTRL_READ_UNIQUE_RFID(data_ptr) \
  memcpy(data_ptr, ctrl_sys_var.basic.rfid_unique, sizeof(ctrl_sys_var.basic.rfid_unique))
#endif
#ifdef GPRS_FEATURE
/*! Read GPRS signal strength.*/
#define CTRL_READ_GPRS_SIGNAL_STRENGTH() ctrl_sys_var.basic.gprs_signal_strength
#endif
#if (defined(RFID_FEATURE) || defined(IEC_62056_21_SLAVE) || defined(GPRS_FEATURE) || defined(RF_LINK_FEATURE))
/*! Read ID of last operator configured the meter.*/
#define CTRL_READ_OPERATOR_ID() ctrl_sys_var.operator_id
#endif
#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_MGNT_SW) ||\
    defined(CTRL_MGNT_SENSOR) || defined(MTR_RVS_TMPR) || defined(MTR_ERTH_TMPR)\
    || defined(MTR_MIS_POT_TMPR) || defined(MTR_THREE_PH) || defined(CTRL_GPRS_MODULE_CVR_SW))
/*! Total count of tampers (Phase sequence,cover open, terminal open, magnetic
 * high,reverse current, fault energy, missing potential).*/
#define CTRL_TMPR_COUNT() ctrl_sys_var.tmpr_count
#endif
#define CTRL_READ_LAST_TAMPER_EVENT() ctrl_sys_var_optional_feature.Last_tamper_code
#define CTRL_READ_LAST_TAMPER_MIN()   ctrl_sys_var_optional_feature.Last_tamper_min
#define CTRL_READ_LAST_TAMPER_HOUR()  ctrl_sys_var_optional_feature.Last_tamper_hour
#define CTRL_READ_LAST_TAMPER_DAY()   ctrl_sys_var_optional_feature.Last_tamper_day
#define CTRL_READ_LAST_TAMPER_MONTH() ctrl_sys_var_optional_feature.Last_tamper_month
#define CTRL_READ_LAST_TAMPER_YEAR()  ctrl_sys_var_optional_feature.Last_tamper_year
#define CTRL_GET_DEBIT_MONEY()        ctrl_sys_var_optional_feature.debit_money    
#define CTRL_GET_USAGE_TYPE()         ctrl_sys_var_optional_feature.usage_type    
/*! Number of bytes in meter status for alarms and tampers status.*/
#define CTRL_NO_BYTE_ALRMS_TMPR 6
/*! Read tampers and alarms status.*/
#define READ_TAMPER_STATUS(data_ptr) memcpy(data_ptr, ctrl_sys_var.shadow_status, CTRL_NO_BYTE_ALRMS_TMPR)
/*! Read power on hours.*/
#define READ_POWER_ON_HOURS() ctrl_sys_var.basic.power_on_hour
/*! Read flag of LCD timeout.*/
#define CTRL_READ_RTC_INT_LOW_PWR_FLAG() (ctrl_flags & CTRL_RTC_INT_MASK)
/*! Read number of power interruptions.*/
#define CTRL_READ_PWR_INT_COUNT() ctrl_sys_var.basic.power_int_counter
/*!@}*/
/*! Set the flag of LCD timeout.*/
#define CTRL_SET_RTC_INT_LOW_PWR_FLAG() (ctrl_flags |= CTRL_RTC_INT_MASK)
/*! Reset the flag of LCD timeout.*/
#define CTRL_RESET_RTC_INT_LOW_PWR_FLAG() (ctrl_flags &= (~CTRL_RTC_INT_MASK))
/*! Set the hint by relay flag.*/
#define CTRL_SET_HINT_RLY() (ctrl_sys_var_optional_feature.ctrl_flags |= CTRL_RELAY_HINT_MASK)
/*! Clear the hint by relay flag.*/
#define CTRL_CLEAR_HINT_RLY() {(ctrl_sys_var_optional_feature.ctrl_flags &= (~CTRL_RELAY_HINT_MASK));ctrl_sys_var.flags &= (~NEW_HINT_RLY_ACT_FLAG);}
/*! Read the up switch state.*/
#ifdef EM_DL130_BOARD
#define CTRL_READ_UP_SW()  1//S_GPIO_Get_Bits(SW_PORT,SW_SCROLL_UP_PIN)
#else
#define CTRL_READ_UP_SW()  S_GPIO_Get_Bits(SW_PORT,SW_SCROLL_UP_PIN)
#endif
/*! set Event Notification flag.*/
#define CTRL_SET_EVENT_NOTIFICATION_FLAG(MASK)  ctrl_Event_Notification_flag |= MASK
/*! set Event Notification flag.*/
#define CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(MASK)  ctrl_Event_Notification_flag &= ~MASK
/*! Calculate day of week.*/
#define CTRL_CALC_DAY_OF_WEEK() bser_calc_day_of_week(ctrl_sys_var_optional_feature.rtc_date.day, \
                                                      ctrl_sys_var_optional_feature.rtc_date.month, ctrl_sys_var_optional_feature.rtc_date.year)
#define CTRL_SAVE_ADE_FAIL_COUNT(PTR,SIZE)        file_sys_write_int(FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS-0xF, PTR, SIZE) /*! - todo call when save a ADE faile counter to internal EEPROM.*/
#define CTRL_LOAD_ADE_FAIL_COUNT(PTR,SIZE)        file_sys_read_int(FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS-0xF, PTR, SIZE) /*! - todo call when save a ADE faile counter to internal EEPROM.*/
#define CTRL_IS_TEST_MODE()                       Test_Mode_GetStatus()
/*! If the first byte in internal memory takes this value, so it contains new data.*/
#define INT_MEM_NEW_DATA_FLAG 0x66

#define CTRL_PUBLISH_EVENT(type)       ctrl_save_evnt_record(type)

#define CTRL_GET_SERIAL_NUM() ctrl_cfg.serial

/*! Disable switch state pin interrupt.*/
#define CTRL_SW_STAT_DISABLE_INT()    {SWITCHE_Terminal_IE_REG &= ~ SWITCHE_Terminal;  SWITCHE_UP_IE_REG &= ~SWITCHE_UP; SWITCHE_DOWN_IE_REG &= ~ SWITCHE_DOWN; SWITCHE_COVER_IE_REG &= ~ SWITCHE_COVER ; SWITCHE_MODULE_OPEN_IE_REG &= ~ TELT_SW_PIN;}

/*! Time in seconds after power up to save power up record.*/
#define PWR_UP_NO_REC_TIME 10
#define PWR_UP_NO_REC_CITICAL_TIME (PWR_UP_NO_REC_TIME - 2)

#define CTRL_TURN_ON_LED()                    S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
#define CTRL_TURN_OFF_LED()                   S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
#ifdef CTRL_BZR
/*! Initialize Buzzer. DC Buzzer doesn't have intialization.*/
#define CTRL_BUZZER_INIT(tone)
#ifdef AC_BUZZER_FEATURE
#define CTRL_BUZZER_ON()   buzzer_enable  = 1   
#define CTRL_BUZZER_OFF()  buzzer_enable  = 0
#else
/*! Turn buzzer ON.*/
#define CTRL_BUZZER_ON()  S_GPIO_Set_Bits(BUZZER_PORT,BUZZER_PIN)
/*! Turn buzzer OFF.*/
#define CTRL_BUZZER_OFF() S_GPIO_Clear_Bits(BUZZER_PORT,BUZZER_PIN)
#endif
#endif

#ifdef AC_BUZZER_FEATURE 
#define CTRL_INIT_TIMER0()   {S_Timer_Internal_INIT(TMR0 , TMR_EXTGT_DISABLE, 100); S_ENABLE_TIMER_0_INT; S_ENABLE_TIMER(TMR0);}
#else
#define CTRL_INIT_TIMER0()   {S_Timer_Internal_INIT(TMR0 , TMR_EXTGT_DISABLE, 10000); S_ENABLE_TIMER_0_INT; S_ENABLE_TIMER(TMR0);}
#endif
/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/*!
 * @addtogroup CTRL_CFG
 * @{
 */
/*!
 * @brief This function is used to set new configuration for control module.
 *
 * @par Description:
 * According to a certain configuration command, part of configuration data is
 * moved to the configuration structure in RAM and also saved in its
 * corresponding part in EEPROM.
 *
 * @param [in] cfg_cmd This is an unsigned char that takes one of
 * #ctrl_cfg_cmd_t values to specify which part of configuration data will be written.
 * @param [in] data_ptr The address of the first byte in the buffer to read the data from.
 *
 * @return
 * - An 8 bit positive number representing the number of bytes moved from the buffer.
 * - Zero in case of failure.
 */
uint8_t ctrl_cfg_set(uint8_t cfg_cmd, uint8_t *data_ptr);

/*!
 * @brief This function is used to get configuration data from control module.
 *
 * @par Description:
 * Read part of configuration data according to a certain configuration command.
 *
 * @param [in] cfg_cmd This is an unsigned char that takes one of
 * #ctrl_cfg_cmd_t values to specify which part of configuration data will be written.
 * @param [out] data_ptr The address of the first byte in the buffer to read the data from.
 *
 * @return
 * - An 8 bit positive number representing the number of bytes moved to the buffer.
 * - Zero in case of failure.
 */
uint8_t ctrl_cfg_get(uint8_t cfg_cmd, uint8_t *data_ptr);

/*!
 * @brief This function is used to get defined features of control system.
 *
 * @par Description:
 * Copy defined features array to the buffer. Check the design document.
 *
 * @return
 * - Number of used control features bytes.
 */
uint8_t ctrl_get_fetur(uint8_t *data_ptr);
/*!@}*/

/*!
 * @addtogroup CTRL_READ
 * @{
 */
/*!
 * @brief This function is used to read system variables of control system.
 *
 * @par Description:
 * Read system variables of control system and return data size.
 *
 * @param [out] data_ptr The address of the first byte in the buffer to write data in.
 *
 * @return
 * - 16 bit positive number representing the number of bytes moved to the buffer.
 */
uint16_t ctrl_read_data(uint8_t *data_ptr);

/*!
 * @brief This function is used to read meter status.
 *
 * @par Description:
 * Read meter status including hardware components status and tampers status.
 *
 * @param [out] data_ptr The address of the first byte in the buffer to write data in.
 *
 * @return
 * - An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t ctrl_read_meter_status(uint8_t *data_ptr);

#ifdef CTRL_EVNT_LOG
/*!
 * @brief Get number of saved event records.
 *
 * @par Description:
 * Read the number of saved event records in non volatile memory.
 *
 * @return
 * - A 16 bit positive number representing the number of saved records.
 */
uint32_t ctrl_get_evnt_num(void);

/*!
 * @brief Read an event record.
 *
 * @par Description:
 * Read a certain event record from non volatile memory to a buffer.
 *
 * @param [in] index The index of the required record ranging from 0 to #EVENT_LOG_RECORD_NUM-1
 * @param [out] data_ptr The address of the first byte in the buffer to write data in.
 *
 * @return
 * - None.
 */
void ctrl_get_evnt_rec(uint16_t index, uint8_t *data_ptr);

/*!
 * @brief Read multiple event records.
 *
 * @par Description:
 * Read number of event records starting from a certain record.
 *
 * @param [in] from The index of the start record ranging from 0 to #EVENT_LOG_RECORD_NUM-1
 * @param [in] num_rec The number of the required records ranging from 1 to #EVENT_LOG_RECORD_NUM
 * @param [out] data_ptr The address of the first byte in the buffer to write data in.
 *
 * @return
 * - A 16 bit positive number representing the number of bytes moved to the buffer.
 */
uint16_t ctrl_get_evnt_mlti_rec(uint16_t from, uint16_t num_rec, uint8_t *data_ptr);
#endif

#ifdef CTRL_CFG_METER_LOG
/*!
 * @brief Get number of saved configure meter records.
 *
 * @par Description:
 * Read the number of saved configure meter records in non volatile memory.
 *
 * @return
 * - An 8 bit positive number representing the number of saved records.
 */
uint32_t ctrl_get_cfg_meter_num(void);

/*!
 * @brief Read a configure meter record.
 *
 * @par Description:
 * Read a certain configure meter record from non volatile memory to a buffer.
 *
 * @param [in] index The index of the required record ranging from 0 to #CFG_METER_RECORD_NUM-1
 * @param [out] data_ptr The address of the first byte in the buffer to write data in.
 *
 * @return
 * - None.
 */
void ctrl_get_cfg_meter_rec(uint8_t index, uint8_t *data_ptr);


/*!
* @brief Operations before power down.
*
* @par Description:
* Check if the power was up since enough period and do all power down operations.
* If the power is not up since enough periods no data is saved in non vloatile
* memory to avoid time consuming.
*
* @return
* - None.
*/
void power_going_down(void);

/*!
 * @brief Update status of switches connected to the shift registers(Up Scroll, Down Scroll,
 * Cover, Terminal).
 *
 * @par Description:
 * Scan the four switches connected to the shift register (Up Scroll, Down Scroll,
 * Cover, Terminal).
 *
 * @return
 * - None.
 *
 * @note:
 * - To avoid noise and bounce effect, we check the switches for different
 *   number of times #NUMBER_SWITCH_CHECK.
 * - Cover and terminal switches are normally closed.
 * - This function records the state of switches in the variable scan_result.
 */
void update_sr_switches_state(uint8_t type);

/*!
 * @brief Initialize scanning switches results.
 *
 * @par Description:
 * Initialize scanning switches results.
 *
 * @return
 * - None.
 */
void init_sw_scan_res(void);

/*!
 * @brief Read multiple configure meter records.
 *
 * @par Description:
 * Read number of configure meter records starting from a certain record.
 *
 * @param [in] from The index of the start record ranging from 0 to #CFG_METER_RECORD_NUM-1
 * @param [in] num_rec The number of the required records ranging from 1 to #CFG_METER_RECORD_NUM
 * @param [out] data_ptr The address of the first byte in the buffer to write data in.
 *
 * @return
 * - A 16 bit positive number representing the number of bytes moved to the buffer.
 */
uint16_t ctrl_get_cfg_meter_mlti_rec(uint8_t from, uint8_t num_rec, uint8_t *data_ptr);
#endif
/*!@}*/

#ifdef SCREEN_RELAY_STATUS_FEATURE
uint8_t GetRelayOpenStatus ();
#endif

void ctrl_default_secondryOption();
/*!
 * @brief get the screen content of relay status 
 *
 * @par Description:
 * This function is getting the screen content of relay status 
 *
* @param[in] Screensptr : pointer to array of pointers have the relay screens
 *
 * @return
 * number of screens
 */
#ifdef SCREEN_RELAY_STATUS_FEATURE
uint8_t GetRelayScreens ( uint8_t ** Screensptr);
#endif
/*!
 * @addtogroup CTRL_EVENT
 * @{
 */
/*!
 * @brief Inform control system with events.
 *
 * @par Description:
 * This function is used to inform control system with new events.
 *
 * @param [in] event Code of the event to inform control system with and its values
 * is listed in #ctrl_evnt_code_t.
 *
 * @return
 * - None.
 */
void ctrl_event(uint8_t event);

/*!
 * @brief Power status ISR.
 *
 * @par Description:
 * Interrupt service routine called when power line status is changed to up or down.
 *
 * @return
 * - None.
 */
void ctrl_pwr_isr(void);

/*!
 * @brief Timer ISR.
 *
 * @par Description:
 * Interrupt service routine called when timer counter overflows used for
 * communication with shift register and continous press detection.
 *
 * @return
 * - None.
 */
void ctrl_tmr_isr();

/*!
 * @brief Switches ISR.
 *
 * @par Description:
 * Interrupt service routine called when one of the four switches (Up or Down or
 * Cover or Terminal) interrupts the halt state while power is down.
 *
 * @return
 * - None.
 */
void ctrl_switch_isr(void);

/*!
 * @brief RTC ISR.
 *
 * @par Description:
 * Interrupt service routine called each second to update meter clock from RTC.
 *
 * @return
 * - None.
 */
void ctrl_rtc_isr(void);
void restore_second_optional_ctrl_data_to_default (void);
/*!@}*/

/*!
 * @addtogroup CTRL_CMD
 * @{
 */
/*!
 * @brief Command control system.
 *
 * @par Description:
 * This function is used to send command to control system.
 *
 * @param [in] cmd_code Code of the command sent listed in #ctrl_cmd_code_t.
 *
 * @return
 * - Test result, If return value is 0 then test passes,
 * Other, test fails, the value of the return integer represents the state of
 * each switch as in the design document.
 */
uint8_t ctrl_cmd(uint8_t cmd_code);

/*!
 * @brief Change template ID.
 *
 * @par Description:
 * Change the template ID to a new value.
 *
 * @param [in] data_ptr Address of the new template ID.
 *
 * @return
 * - Size of template ID.
 */
uint8_t ctrl_change_tmplt_id(uint8_t *data_ptr);

#ifdef CTRL_CFG_METER_LOG
/*!
 * @brief Save a configure meter record.
 *
 * @par Description:
 * Save a configure meter record in non volatile memory.
 *
 * @param [in] operator_id ID of the operator who configured meter.
 * @param [in] interface interface from which meter was configured #ctrl_interface_t.
 *
 * @return
 * - None.
 */
void ctrl_save_cfg_meter_rec(uint16_t operator_id, uint8_t interface);
#endif

#ifdef GPRS_FEATURE
/*!
 * @brief Set the srength of GPRS signal.
 *
 * @par Description:
 * Set the srength of GPRS signal.
 *
 * @param [in] new_value The new value to set the variable with.
 *
 * @return
 * - None.
 */
void ctrl_set_gprs_signal_strength(uint8_t new_value);
#endif

#ifdef RFID_FEATURE
/*!
 * @brief Set RFID uinque identification.
 *
 * @par Description:
 * Set RFID uinque identification.
 *
 * @param [in] data_ptr The address of the new ID.
 *
 * @return
 * - Size of ID.
 */
uint8_t ctrl_set_unique_rfid(uint8_t* data_ptr);
#endif

/*!
 * @brief This function is used to retrieve last three operator ids.
 *
 * @par Description:
 * Pass an event record data to get last three operator ids from config records.
 *
 * @return
 * - None.
 */
void update_last_operator_ids(uint16_t operator_name);

/*!@}*/

/*!
 * @addtogroup CTRL_CORE
 * @{
 */
/*!
 * @brief Reset control configuration
 *
 * @par Description:
 * Return the configuration to its defaults and save it in EEPROM.
 *
 * @return
 * - None.
 */
void ctrl_reset_cfg(uint8_t save);
/*!
 * @brief Check if the meter is powered by power line or battery.
 *
 * @par Description:
 * Check if the meter is first powered by the power line or by battery. If it is
 * powered by battery, halt the processor.
 *
 * @return
 * - None.
 */
void ctrl_first_init(void);

/*!
 * @brief Initialize all variables and load data from EEPROM
 *
 * @par Description:
 * Load configuration and system variables from EEPROM and assign to system variables
 *
 * @return
 * - None.
 */
void ctrl_init(void);

void ctrl_task(void);

void set_power_up_flag(void);
/*!@}*/
#endif
void ctrl_get_selected_meter_status(uint8_t *ptr);

uint8_t ctrl_check_overload(void);
void ctrl_rset_overload(void);
void reset_specific_tamper(uint8_t ptr);
void restore_optional_ctrl_data_to_default(void);
/*!
 * @brief used to know the switch status
 *
 * @par Description:
 * @return
 * - True if sw is pressed.
 * - False if sw isn't pressed.
 */
uint8 get_cont_flag(void);

/*!
 * @brief used to latch state of up switch
 *
 * @par Description:
 *no return
 */
uint8 get_up_flag(void);
/*!
 * @brief used to reset latch_long_press_flag
 *
 * @par Description:
 *no return
 */
void reset_cont_flag(void);
/*!
 * @brief used to reset up switch state
 *
 * @par Description:
 *no return
 */
void reset_up_flag(void);;

void set_power_up_flag(void);
void set_power_down_flag(void);
uint16_t ctrl_get_up_side_down_counter(void);

void ctrl_get_selected_meter_status(uint8_t *ptr);
#ifdef MTR_THREE_PH
void ctrl_overload_state (uint8_t ol_state,uint8_t channel);
#else
void ctrl_overload_state (uint8_t ol_state);
#endif
///////////#ifdef ARABIC_SCREEN_ENABLE
uint8_t ctrl_check_overload(void);
/////////#endif
void reset_specific_tamper(uint8_t ptr);
void close_relay(void);
void open_relay(void);
#ifdef CTRL_AUX_LED
void AuxLedOn(void);
void AuxLedOff(void);
#endif
void AlarmLedOn(void);
void AlarmLedOff(void);
/*!
 * @brief Toggle the relay.
 *
 * @par Description:
 * Toggle the relay.
 *
 * @return
 * - None.
 */
void toggle_relay(void);


void DRU_Activate(uint8_t  * Ptr);
void Bootloader_Activate(uint8_t  * Ptr);
void bootloader_newsec_event();
uint8_t ctrl_check_status(uint8_t latched_tamper_index);
void restore_optional_ctrl_data_to_default (void);
void calc_init_rem_day_wh (void);
void ctrl_set_upsideDown_flag(void);
void ctrl_save_evnt_record(uint8_t evnt_code);
void ctrl_save_cfg();
void power_going_down(void);
void lpm_first_init(void);
void dispaly_half_sec_timer();
void power_is_up(void);
void new_sec(void);
void new_min(void);
void new_hr(void);
uint8_t is_power_down(void);
void save_rtc_flag(void);
void check_btry(void)  ;
uint16_t CTRL_getBatteryLevel(void);
void CTRL_startBatteryRead(void);
void Save_Important_Data(void);
void MCU_disable_all(uint8_t I2c_Flag);
void Power_up(void);
void ctrl_set_openCoverFlagPWD();
void test_toggle_relay();
void ctrl_load_backup(void);
uint8_t ctrl_no_cdt_no_tmpr (void);
void check_update_times_event(uint8_t event );
#ifdef KEYPAD_FEATURE
void ctrl_keypad_down_up_update_flag(uint8_t sw_type);
#endif
void check_and_reset_ov_load_cntr(uint8_t state);
void update_technician_function(uint8_t tech_action_flag);
uint8_t get_last_date_tamper(uint8_t type_tamper , time_date_t* tamper_date);
void ctrl_reset_last_tamper (uint8_t tmpr);
void save_ctrl_cfg (void);
uint8_t get_start_date_tamper(uint8_t type_tamper , time_date_t* tamper_date);
void ctrl_rtc_monitor_task(void);
#ifdef ANSI_STACK
void optical_slave_event(uint8_t type);
uint32_t alarm_get_reason(void);
#endif
/*!
 * @brief Check if if there is new event noification need to be sent .
 *
 * @par Description:
 * Check if if there is new event noification need to be sent. If it is
 * the flag is set.
 *
 * @return
 * - None.
 */
uint8_t check_value(uint8_t value);
void ctrl_Event_Notification_Send();
void CTRL_set_technicial_id_value (void);
uint8_t ctrl_get_block_state(void);
void ctrl_set_block_state (void);
uint8_t ctrl_is_power_down (void);
void ctrl_set_ctrl_battery_magnet_flag_check(void);
void construct_new_tech_ids (uint8_t * buff);
void ctrl_save_power_up_cfg_pages (void);
void Our_reset(void);
void ctrl_reset_watania_tampers (uint8_t ii,uint8_t num);
void Force_Open_Relay(void);
void ctrl_down_pressed_isr(void);
void ctrl_up_pressed_isr(void);

void ctrl_exit_lpm (void);
void Force_Close_Relay_FOR_Test_Calb_(void);
TBOOL ctrl_check_tampers_flag(uint8_t event);
void ctrl_save_record_DM_G2(uint8_t record_type, uint8_t *data_ptr);
uint8_t test_sw(uint8_t sw_mask);
void check_switches_state_pwrdwn(void);
void Ctrl_Set_Date_Time_Fixed(void);
/*!@}*/
#endif
