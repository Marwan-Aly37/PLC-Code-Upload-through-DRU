#ifndef COMM_H
#define COMM_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             C O M M U N I C A T I O N

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file comm.h
 *
 * @brief This file contains declarations of all global functions of communication interfaces.
 *
 * @details This file is the interface between apllication layer and all communication interfaces..
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
 *   <td> 28/02/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Make a shared buffer between RF LINK and IEC. </td>
 * </tr>
 * <tr>
 *   <td> 14/04/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Integrate with RF Link. </td>
 * </tr>
 * <tr>
 *   <td> 30/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add scratch card status to instanteneous data. </td>
 * </tr>
 * <tr>
 *   <td> 29/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify instantaneous data and add scratch card command and status. </td>
 * </tr>
 * <tr>
 *   <td> 26/07/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup COMM_SYS Communication System
 * @ingroup METER
 * Communication file is responsible for managing all communication interfaces tasks.
 *
 * <b>References</b>\n
 * -  Design document of communication system.
 * -  Communication Data Sheet.
 *
 * @brief Collection of all communication interfaces.
 */
/*!
 * @defgroup COMM_CORE Application Core APIs
 * @ingroup COMM_SYS
 * @brief Core containing functions handling all communication interfaces.
 */
/*!
 * @defgroup COMM_CMD Commands APIs
 * @ingroup COMM_SYS
 * @brief Command interface of communication layer.
 */
/*!
 * @defgroup COMM_READ Read System Variables APIs
 * @ingroup COMM_SYS
 * @brief Used to read meter data by any communication port.
 */
/*!
 * @defgroup COMM_INTFC Communication Interfaces
 * @ingroup COMM_SYS
 * @brief Used to read meter data by any communication port.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"

#ifdef ANSI_STACK
/* #include "ANSI_APP_OpticalTables.h" */ /* module removed from the DRU build */
#endif
/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/

/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/
/*! Argument passed with tariff and payment reset command #reset_cmd_t.*/
#define RST_SYS_VAR_TRF_PYMT 0
#define ACK_RX_PACKET_NUM    5 
#define BOOT_LOADER_TIMEOUT          240ul
#ifdef DRU_SW_UP
#define MAX_LED_TOGGLE_COUNTER  5000
#else
#define MAX_LED_TOGGLE_COUNTER  1
#endif
#define MAX_LED_FLASHING_COUNTER  5000

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/

typedef enum
{
  LED_START,
  LED_OFF,
  LED_FLASHING,
  LED_FLASHING_ACCPT,
  LED_ON
  
}led_t;
  
typedef enum
{
 NO_BOOTLOADER_DATA = 0 ,
 RECIEVE_BOOTLOADER_DATA = 1

}boot_loader_flag_state;


/*!
 * @ingroup COMM_READ
 * @par Description:
 *   This enumerator contains all raw data options.
 */typedef enum
{
  COMM_MAIN_DATA = (uint8_t)0,                /*!< Common data read by any communication interface to get meter features.*/
  COMM_TRF_DATA = (uint8_t)1,                 /*!< Tariff and payment system variables.*/
  COMM_METERING_DATA = (uint8_t)2,            /*!< Metering system variables.*/
  COMM_CTRL_DATA = (uint8_t)3,                /*!< Control system variables.*/
  COMM_METERING_INST_DATA = (uint8_t)4,       /*!< Metering system instantaneous variables.*/
  COMM_PB_RECORD = (uint8_t)5,                /*!< Billing Period History Record	.*/
  COMM_EVENT_RECORD = (uint8_t)6,             /*!< Event Record.*/
  COMM_MONEY_RECORD = (uint8_t)7,             /*!< Money Transaction Record.*/
  COMM_PROFILE_RECORD = (uint8_t)8,           /*!< Profile Record.*/
  COMM_MTR_CFG_RECORD = (uint8_t)9,           /*!< Configure Meter Record.*/
  COMM_TRF_CFG = (uint8_t)10,                  /*!< Tariff Configuration Data.*/
  COMM_PYMT_CFG = (uint8_t)11,                 /*!< Payment Configuration Data.*/
  COMM_TAX_CFG = (uint8_t)12,                  /*!< Tax Configuration Data.*/
  COMM_RETURN_CORRUPTION = (uint8_t)13,
  COMM_READ_DATA_EEPROM_PAGES = (uint8_t)0x80,/*!< The MSB refered that the command for reading data EEPROM.*/
  // ADDED by AMR
  COMM_VACATION_CFG_DATA = (uint8_t)14,
  COMM_FRIENDLY_CFG_DATA = (uint8_t)15,
  COMM_PYMENT_CFG_DATA   = (uint8_t)16,
  COMM_TARIFF_CFG_DATA   = (uint8_t)17,
  COMM_TAX_CFG_DATA      = (uint8_t)18,
  COMM_BP_CFG_DATA       = (uint8_t)19,
  COMM_CONTROL_CFG_DATA  = (uint8_t)20,
  COMM_MAXIMUM_DEMAND_CFG_DATA = (uint8_t)21,
  COMM_LIMITER_CFG_DATA         = (uint8_t)22,
  COMM_TAMPERS_CFG_DATA         = (uint8_t)23,
  COMM_OPERATING_POINT_CFG_DATA = (uint8_t)24,
  COMM_ID_CFG_DATA              = (uint8_t)25,
  COMM_ACTIONS_ALARMS_CFG_DATA  = (uint8_t)26,
  COMM_TIME_CFG_DATA            = (uint8_t)27,
  COMM_DATE_CFG_DATA            = (uint8_t)28,
  COMM_BATTERY_CFG_DATA         = (uint8_t)29,
  COMM_MAGNETIC_CFG_DATA        = (uint8_t)30,
  COMM_RFID_UNIQUE_ID_CFG_DATA  = (uint8_t)31,
  COMM_GPRS_CFG_DATA            = (uint8_t)32,
  COMM_OVERLOAD_ALARMS_CFG_DATA = (uint8_t)33,    
  COMM_RETURN_NEW_CTRL_DATA = (uint8_t)34,
  COMM_MAX_VALUE = (uint8_t)35,                /*!< The maximum of data types.*/
} comm_raw_data_t;

/*!
 * @ingroup COMM_READ
 * @par Description:
 *   This enumerator contains the status of shared buffer0 semaphore.
 */
typedef enum
{
  BUFFER_IS_FREE = (uint8_t)0,         /*!< Buffer is free.*/
  BUFFER_USED_BY_IEC = (uint8_t)1,     /*!< Buffer is currently used by IEC.*/
  BUFFER_USED_BY_RFLINK = (uint8_t)2,  /*!< Buffer is currently used by RFLINK.*/
} comm_buffer0_semaphore_status_t;

/*!
 * @par Description:
 *   This enumerator contains all communication commands codes.
 */
typedef enum
{
  METER_RST = (uint8_t)1,
  CHRG_MNY = (uint8_t)2,
  DISCHRG_MNY = (uint8_t)3,
  RST_TRF_PYMT = (uint8_t)4,
  VAC_CFG_SET = (uint8_t)5,
  FRDLY_CFG_SET = (uint8_t)6,
  PYMT_CFG_SET = (uint8_t)7,
  TRF_CFG_SET = (uint8_t)8,
  TAX_CFG_SET = (uint8_t)9,
  BP_CFG_SET = (uint8_t)10,
  DSPLY_CTRL_CFG_SET = (uint8_t)11,
  DSPLY_GO_OP_MODE = (uint8_t)12,
  MTR_RST_ACC_CMD = (uint8_t)13,
#ifdef MTR_RVS_TMPR
  MTR_RST_REV_ACC_CMD = (uint8_t)14,
#endif
#ifdef MTR_ERTH_TMPR
  MTR_RST_FLT_ACC_CMD = (uint8_t)15,
#endif
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
  MTR_RST_TMP_CMD = (uint8_t)16,
#endif
//#ifdef MTR_LOAD_PROFILE
  MTR_RST_LD_PRF_CMD = (uint8_t)17,
//#endif
  MTR_RST_MD_CMD   = (uint8_t)18,
  MTR_STRT_CAL_CMD   = (uint8_t)19,
  MTR_NXT_CAL_CMD  = (uint8_t)20,
  MTR_SET_MD_CFG   = (uint8_t)21,
#ifdef MTR_ENABLE_LMT
  MTR_SET_LMT_CFG  = (uint8_t)22,
#endif
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
  MTR_SET_TMP_CFG  = (uint8_t)23,
#endif
  MTR_SET_OPR_CFG  = (uint8_t)24,
  BEGIN_SELF_TEST = (uint8_t)25,
  RESET_CONTROL = (uint8_t)26,
  RESET_TMPR = (uint8_t)27,
#ifdef CTRL_EVNT_LOG
  RST_EVNT_LOG = (uint8_t)28,
#endif
#ifdef CTRL_CFG_METER_LOG
  RST_CFG_LOG = (uint8_t)29,
#endif
#ifdef CTRL_RLY
  TST_RLY = (uint8_t)30,
#endif
  ID_CFG_SET = (uint8_t)31,
#ifdef CTRL_ACTIONS
  ACT_CFG_SET = (uint8_t)32,
#endif
#ifdef CTRL_RTC
  TIME_CFG_SET = (uint8_t)33,
  DATE_CFG_SET = (uint8_t)34,
#endif
#if (CTRL_BTRY_NON_CHRG == 1)
  BTRY_CFG_SET = (uint8_t)35,
#endif
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
  MAG_CFG_SET = (uint8_t)36,
#endif
#ifdef RFID_FEATURE
  RFID_UNIQUE_SET = (uint8_t)37,
#endif
#ifdef GPRS_FEATURE
  GPRS_CFG_SET = (uint8_t)38,
#endif
  CHNG_TMPLT_ID = (uint8_t)39,
#if (defined(CTRL_RLY) && defined(MTR_ENABLE_LMT))
  OVRLOAD_CFG_SET = (uint8_t)40,
#endif
#ifdef MTR_INDIRECT
  INDIRECT_PARAM_SET = (uint8_t)41,
#endif
#ifdef CTRL_RLY
  ENTER_OPEN_RELAY_MODE = (uint8_t)42,
  EXIT_OPEN_RELAY_MODE = (uint8_t)43,
#endif

  GPRS_SCRTCH_CARD_SEND = (uint8_t)100,

#if (defined(CTRL_RLY) || defined(CTRL_BZR))
  END_HINT_CMD = (uint8_t)101,
#endif
  SET_SERIAL_NUM_CMD = (uint8_t)102,
  RESET_SERIAL_NUM_CMD = (uint8_t)103,
#ifdef EDIT_TIME_DATE_MANUALLY
  MANUALY_SET_DATE_TIME = (uint8_t)104,
#endif
   RESTART_METER_CMD = (uint8_t)105,
   Write_All_EEPROM=(uint8_t)106,
   Reset_Meter=(uint8_t)107,
   SET_USAGE_TYPE_CMD = (uint8_t)108,
   SET_DEBIT_MONEY_CMD = (uint8_t)109,   
   GPRS_BOOTLOADER_MODE_CMD = (uint8_t)110,
   WRITE_DATA_EXTERNAL_EEPROM_CMD = (uint8_t)111,
   RESET_SPECIFIC_TAMPER = (uint8_t)113,
   READ_ALL_EEPROM =(uint8_t)114,
   SET_TARIFF_CALC_W_KW=(uint8_t)115,
   SET_TARIFF_TAX_DEDUCTION_MODE = (uint8_t)116,
   SET_ZERO_CONSUMBTION_TAX_DEDUCTION_VALUE = (uint8_t)117,
   SET_CURRENT_CONSUMPTION = (uint8_t)118,
   SET_TOTAL_CONSUMPTION = (uint8_t)119,
   SET_CHARGE_NUM  = (uint8_t)120,
#ifdef LOAD_LIMIT_PLAN
   MTR_SET_LOAD_LMT_CFG= (uint8_t)121,
#endif
LP_EVENT_NOTIFICATION_TIME_CFG_CMD = (uint8_t)122,
OPEN_CLOSE_RELAY_MODE = (uint8_t)123,
ACTIVATE_REMOTE_PROGRAMMING = (uint8_t)124,
Write_All_EEPROM2 = (uint8_t)125,
COMM_CONFIGURE_PBH_GRACE_PERIOD = (uint8_t)126,
COMM_CONFIGURE_MAXIMIM_CONSUMPTION = (uint8_t)127,
COMM_CONFIGURE_MAXIMIM_CONSUMPTION_ACTION = (uint8_t)128,
COMM_SET_COMMISSIONING_STATE = (uint8_t)129,
COMM_SET_OVERLOAD_IN_POWER_VALUE = (uint8_t)130,
COMM_SET_OVERLOAD_IN_POWER_TIMEOUT = (uint8_t)131,
COMM_SET_VACATION_TIME = (uint8_t)132,
COMM_SET_VACATION_PRICES = (uint8_t)133,
COMM_SET_NK_TAX = (uint8_t)134,
COMM_SET_1K_TAX = (uint8_t)135,
COMM_METER_TYPE  = (uint8_t)136,
COMM_SET_LOAD_PROFILE_TIMEOUT  = (uint8_t)137,
COMM_CONFIGURE_MAXIMIM_CURRENT = (uint8_t)138,
COMM_RESET_METER_WITHOT_RST_GPRS =  (uint8_t)139,
COMM_SET_TOT_CONSUMPTION_ACTIVE =  (uint8_t)140,
COMM_SET_TOT_CONSUMPTION_REACTIVE =  (uint8_t)141,
COMM_SET_TOT_CONSUMPTION_APPARENT =  (uint8_t)142,
COMM_RESET_CURRENT_MD_VALUES =  (uint8_t)143,
COMM_SET_SERIAL_PREFIX_CHAR =  (uint8_t)144,
COMM_CHECK_REMOTE_EEPROM =  (uint8_t)145,
BEGIN_ACCURACY_TEST =  (uint8_t)146,
SET_TARIFF_RECALC_CALC_2 = (uint8_t)147,
BEGIN_METER_COPY_CMD =  (uint8_t)148,
END_METER_COPY_CMD =  (uint8_t)149,
SET_TOTAL_CONSUMPTION_HIGH = (uint8_t)150,
CTRL_SET_CONSUMPTION_ALARM_VALUE =  (uint8_t)151,
CTRL_SET_CONSUMPTION_GRACE_AFTER_RECALC =  (uint8_t)152,
CTRL_SET_CONSUMPTION_ALARM_ACTION =  (uint8_t)153,
CTRL_SET_DAYS_GRACE_AFTER_RECALC = (uint8_t)154,

SET_WAITING_TARIFF_CALC_W_KW = (uint8_t)155,
SET_INDUCTION_PF_FEATURE = (uint8_t)156,
TRF_SET_MONEY_BALANCE = (uint8_t)157,
TRF_SET_CURRENT_BP_BILL = (uint8_t)158,
TRF_SET_CURRENT_REACTIVE     = (uint8_t)161,
TRF_SET_TARIFF_DATA  = (uint8_t)162,
TRF_SET_PAYMENT_DATA  =  (uint8_t)163,
MTR_SET_MD_REGISTERS  =    (uint8_t)164,
CTRL_SET_SENT_TAMPER_FLAG_CMD=(uint8_t)167,
CTRL_SET_TECH_ID =(uint8_t)168,
SET_GPRS_SERVER_RESTART_TIMEOUT = (uint8_t)169,
SET_GPRS_NOTIFY_TAMPER_REPEAT_TIMEOUT = (uint8_t)170,
SET_GPRS_WAIT_ACK_TIMEOUT  = (uint8_t)171,
CTRL_ENABLE_GPRS_SEND_NOTIFICATION_CMD = (uint8_t)172,
SET_GPRS_NOTIFY_BILLING_CHARGING_REPEAT_TIMEOUT = (uint8_t)173,
SET_GPRS_NOTIFY_KEY_COMMISSIONING_REPEAT_TIMEOUT = (uint8_t)174,
SET_MAX_MDA =(uint8_t)176,
SET_MAX_TAMPER_DEBIT=(uint8_t)177,
SET_TARRIF_EXTENSION = (uint8_t)180,
SET_GPRS_SMS_PHONE_NUMBER = (uint8_t)181,
SET_RECLAC_FLAGS_CMD = (uint8_t)182,
SET_FUTURE_RECLAC_FLAGS_CMD = (uint8_t)183,
CHANGE_48_HOUR_AFTER_SELLING = (uint8_t)186,
CONFIG_SET_DISPLAY_SCREEN  =(uint8_t)187, // new self test
CONFIG_SET_DURATION_MINUTES_TEST_RELAY =(uint8_t)188,
CONFIG_RESET_CONSUMPTION_ERR_CONTACTOR =(uint8_t)189,
CONFIG_RESET_COUNT_TIME_DATE_CHANGE =(uint8_t)190,
SET_12_BH_RECORD  =(uint8_t)191,
CONFIG_CUST_SERV_CAL_TYP  =(uint8_t)192, //type 0 or 1
CONFIG_STATE_OF_RESET_OVR_LOAD_COUNTER=(uint8_t)193,
SET_FRIENDLY_CONSUMPTION =(uint8_t)194,
SET_CURRENT_BILL = (uint8_t)195,
SET_CREENT_BPH_INDEX = (uint8_t)196,
SET_NOT_MAKE_NEW_BILLING_WITH_NEW_DATE_CFG = (uint8_t)197,
SET_MAKE_PREV_12MONTH_RECALC_CFG = (uint8_t)198,
SET_48_HOUR_MONTHLY = (uint8_t)199,
COMM_SET_NK_TAX2 = (uint8_t)200,
COMM_SET_MAXIMUM_CONSUMPTION_NK_TAX1 = (uint8_t)201,
CONFIGURE_BLOCK_STATE = (uint8_t)202,
TFF_CFG_TYPE = (uint8_t)203,
CTRL_CFG_ONE_RELAY_CMD_CONNECT = (uint8_t)204,
CTRL_CHANGE_RFID_CARD_TYPE = (uint8_t)205,
TRF_TOTAL_CHARGES_AMOUNT = (uint8_t)206,
DOT_MATRIX_LCD_FLASH_WR = (uint8_t)207,
DOT_MATRIX_CHANGE_LANGUAGE = (uint8_t)208,
SET_ACITVITY_ID = (uint8_t)209,
CTRL_SET_DAILY_TAXES_REGISTER = (uint8_t)210,
RF_FIRMWARE_SAVING_CMD = (uint8_t)211,
PLC_FIRMWARE_SAVING_CMD   = (uint8_t)212, /* Added new command for PLC firmware receive - Moustafa */
LAST_COMMAND = PLC_FIRMWARE_SAVING_CMD, // Do not Forget to add its validation in the array size_of_comm /* Incremented it to 213 instead of 211 - Moustafa */
CONFIG_SET_TIME_CONSUM_TEST_ACCURACY  =(uint8_t)254, // new self test
} comm_cmd_code_t;


typedef enum
{
   DLMS_SOURCE = (uint8_t)0x01 ,
   RFID_SOURCE = (uint8_t)0x02 ,
   GPRS_SOURCE = (uint8_t)0x04 ,
   OPTICAL_SOURCE = (uint8_t)0x08 ,
   RF_LINK_SOURCE = (uint8_t)0x10,
   LOCAL_SOURCE = (uint8_t)0x20,
   STS_SOURCE = (uint8_t)0x40 ,

}comm_source_t;
/*------------------------------------------------------------------------------------------
                                     External Variables

------------------------------------------------------------------------------------------*/
extern uint16_t current_pckt_ID ;
extern uint16_t RX_PacketID_arr[ACK_RX_PACKET_NUM];
extern uint8_t Rx_packet_num;
extern uint16_t Boot_loader_timeout;
extern uint8_t tmp_crc;
extern uint8_t customer_flag;
extern uint8_t comm_disable_zero_command;
extern uint16_t replace_charge_counter;
extern uint8_t var_temp_size;
extern uint8_t cyshield_card;
extern uint8_t EchoBuffer[260];
extern uint8_t g_jig_start;

extern uint8_t id_ar_size[2];

extern uint8_t plc_first_packet;
extern uint8_t rf_first_packet;

/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
#define MAX_BOOTLOADER_PCKT_SIZE_IN_FLASH     257ul 
#define MAX_BOOTLOADER_PCKT_SIZE     256ul 
#define MAX_BOOTLOADER_PCKT_NUM      1000ul 
#define CURRENT_PACKET_ADDRESS (MAX_BOOTLOADER_PCKT_SIZE_IN_FLASH *(current_pckt_ID-1))
/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/* @internship Program
 * This function is called we want to reset all variables used by the PLC
 */
void plc_reset_var(void);
/* @internship Program
 * This function is called when we receive PLC firmware command
 */
uint8_t save_plc_firmware_data(uint8_t *data, uint16_t data_length);
/* @internship Program
 * This function is called we want to reset all variables used by the PLC
 */
void rf_reset_var(void);
/* @internship Program
 * This function is called when we receive RF firmware command
 */
uint8_t save_rf_firmware_data(uint8_t *data, uint16_t data_length);
/*!
 * @ingroup COMM_CMD
 */
/*!
 * @brief Handle a received command packet from any communication interface.
 *
 * @par Description:
 * Handle commands coming in a command packet by fetching command code then calling the
 * proper module API to handle this command and do these for all commands in the packet.
 *
 * @param [in] data_ptr Address of first byte in the buffer containing commands.
 * @param [in] data_size Size of data to be handled.
 *
 * @return TRUE or FALSE return. TRUE means that the all commands where achieved
 * successfully. FALSE means that one of commands at least failed or the data size
 * didn't match with coming commands.
 *
 * @note The first byte in the buffer should be a command code byte. Commands
 * should be written as (command code then command data) with any number of commands.
 */
uint8_t comm_handle_rx_cmd(uint8_t *data_ptr, uint16_t data_size);
/*!
 * @ingroup COMM_BOOTLOADER
 */
/*!
 * @brief Handle a received boot loader packet packet from any communication interface.
 *
 * @par Description:
 * Handle boot loader data packet and save it in EEPROM with CRC
 *
 *
 * @param [in] data_ptr Address of first byte in the buffer containing commands.
 * @param [in] data_size Size of data to be handled.
 *
 * @return TRUE or FALSE return. TRUE means that the all commands where achieved
 * successfully. FALSE means that one of data packet with specific ID is failed or  
 * didn't match with coming commands.
 *
 
 */
uint8_t comm_handle_rx_bootloader_data(uint8_t *data_ptr, uint16_t data_size);

/*!
 * @ingroup COMM_READ
 */
/*!
 * @brief Write raw meter data to a buffer.
 *
 * @par Description:
 * This function writes meter data to a buffer. Meter data is divided into parts
 * each part belongs to a separate module to reduce overhead of transfering data
 * as one chunk.
 *
 * @param [out] buffer_ptr Address of first byte in the buffer to get data.
 * @param [in] type Type of data required #comm_raw_data_t .
 *
 * @return Size of data written in buffer.
 *
 * @note Main meter data size is fixed regardless of defined features as example,
 * if the BPH feature is not defined the sent number of BPH records is zero.
 */
uint16_t comm_read_raw_data(uint8_t *buffer_ptr, uint8_t type);

/*!
 * @ingroup COMM_CORE
 */
/*!
 * @brief Initializtion of all communication interfaces.
 *
 * @par Description:
 * Initailize different communication interfaces.
 *
 * @return
 * - None.
 */
void comm_init(void);

/*!
 * @ingroup COMM_CORE
 *
 * @brief Task of all communication interfaces.
 *
 * @par Description:
 * Calls all task functions of communication interfaces.
 *
 * @return
 * - None.
 */
void comm_task(void);
void read_bootloader_data ();
uint8_t comm_remote_programming (uint8_t *buff,uint16_t size);
void set_comm_source (comm_source_t  comm_source);
extern comm_source_t get_comm_source ();
uint8_t com_check_size_commands(uint8_t *data_ptr, uint16_t data_size);
void reset_all_meter_data_cmd(void);
void CommSetSendDataFlag(uint8_t Val);
void CommDruTask(void);
void CommEraseAllSectorOfDotMatrixSpi2(void);
uint8_t CommGetSendDataFlag(void);
void CommResetDruCounterTimeOut(void);
void CommIncrementDruCounterTimeOut(void);
void CommEraseAllSectorExtFlashOfDotMatrix(void);
void CommResetLedDruStatus(void);
void IntegrityCrcCheckState(void);
uint8_t CommDruWriteSerialNumSector(uint32_t Add);
uint8_t UnitTestReadingFunction(uint32_t Add);
#endif

