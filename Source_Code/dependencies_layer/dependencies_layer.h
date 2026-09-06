/*==========================================================================================

  D E P E N D E N C I E S   L A Y E R

  Self-contained declarations for the DRU build.

  The individual headers this replaced (Meter_Interface.h, file_sys.h, trf_pymt.h,
  display.h, EEPROM.h, Power_modes.h, V9203.h, ...) are no longer part of the tree.
  Only the declarations the surviving code actually references were kept, copied
  verbatim from those headers so the values are unchanged.

  This file deliberately includes nothing from the application. The old headers
  included ctrl.h, which included them back; inlining here breaks that cycle.

==========================================================================================*/
#ifndef __DEPENDENCIES_LAYER_H
#define __DEPENDENCIES_LAYER_H

#include "config.h"
#include "basic_srv.h"
#include "Clock.h"

/*==========================================================================================
  Declarations recovered from the removed headers
==========================================================================================*/



/*------------------------------------------------------------------------------------
  from EEPROM_Addresses.h
------------------------------------------------------------------------------------*/
// Created with Excel Table By Mohamed Hamed			
// Date:  11/04/2015        Time:  02:14:49			

#ifndef EEPROM_PAGE_256
#else
#endif
/*!
 * @addtogroup EEPROM_CMD
 * @{
 */
/*! EEPROM page size in Bytes.*/
#define EEPROM_PAGE_SIZE 256L
#define Data_eeprom_address  0
/*! EEPROM page 0 address.*/
/*! EEPROM page 1 address.*/
/*! EEPROM page 2 address.*/
    /*! EEPROM page 4 address.*/
   /*! EEPROM page 5 address.*/
/*! EEPROM page 6 address.*/
/*! EEPROM page 7 address.*/
/*! EEPROM page 8 address.*/
#define EEPROM_PAGE_8 (uint32_t)(8*EEPROM_PAGE_SIZE)
/*! EEPROM page 11 address.*/
#define EEPROM_PAGE_12 (uint32_t)(12*EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_13 (uint32_t)(13*EEPROM_PAGE_SIZE)
/*! EEPROM page 14 address.*/
#define EEPROM_PAGE_14 (uint32_t)(14*EEPROM_PAGE_SIZE)
/*! EEPROM page 15 address.*/
#define EEPROM_PAGE_15 (uint32_t)(15*EEPROM_PAGE_SIZE)
/*! EEPROM page 17 address.*/
#define EEPROM_PAGE_17 (uint32_t)(17*EEPROM_PAGE_SIZE)
/*! EEPROM page 18 address.*/
/*! EEPROM page 19 address.*/
#define EEPROM_PAGE_19 (uint32_t)(19*EEPROM_PAGE_SIZE)
/*! EEPROM page 20 address.*/
#define EEPROM_PAGE_20 (uint32_t)(20*EEPROM_PAGE_SIZE)
/*! EEPROM page 21 address.*/
#define EEPROM_PAGE_21 (uint32_t)(21*EEPROM_PAGE_SIZE)
/*! EEPROM page 22 address.*/
#define EEPROM_PAGE_22 (uint32_t)(22*EEPROM_PAGE_SIZE)
#ifdef FM24C128D_2_Wire_Serial_EEPROM
/*! EEPROM page 23 address.*/
/*! EEPROM page 24 address.*/
/*! EEPROM page 25 address.*/
/*! EEPROM page 26 address.*/
/*! EEPROM page 27 address.*/
/*! EEPROM page 28 address.*/
/*! EEPROM page 29 address.*/
#define EEPROM_PAGE_29 (uint32_t)(29*EEPROM_PAGE_SIZE)
/*! EEPROM page 30 address.*/
#define EEPROM_PAGE_30 (uint32_t)(30*EEPROM_PAGE_SIZE)
/*! EEPROM page 32 address.*/
/*! EEPROM page 33 address.*/
/*! EEPROM page 34 address.*/
#define EEPROM_PAGE_34 (uint32_t)(34*EEPROM_PAGE_SIZE)
/*! EEPROM page 35 address.*/
/*! EEPROM page 36 address.*/
/*! EEPROM page 37 address.*/
#define EEPROM_PAGE_37 (uint32_t)(37*EEPROM_PAGE_SIZE)
/*! EEPROM page 38 address.*/
/*! EEPROM page 39 address.*/
/*! EEPROM page 40 address.*/
#define EEPROM_PAGE_40 (uint32_t)(40*EEPROM_PAGE_SIZE)
/*! EEPROM page 41 address.*/
/*! EEPROM page 42 address.*/
/*! EEPROM page 43 address.*/
/*! EEPROM page 44 address.*/
/*! EEPROM page 45 address.*/
#define EEPROM_PAGE_45 (uint32_t)(45*EEPROM_PAGE_SIZE) //free dlms
/*! EEPROM page 46 address.*/
/*! EEPROM page 47 address.*/
/*! EEPROM page 48 address.*/
/*! EEPROM page 49 address.*/
/*! EEPROM page 50 address.*/
/*! EEPROM page 51 address.*/
/*! EEPROM page 52 address.*/
/*! EEPROM page 53 address.*/
/*! EEPROM page 54 address.*/
/*! EEPROM page 55 address.*/
/*! EEPROM page 56 address.*/
/*! EEPROM page 57 address.*/
/*! EEPROM page 58 address.*/
/*! EEPROM page 59 address.*/
/*! EEPROM page 60 address.*/
/*! EEPROM page 61 address.*/
/*! EEPROM page 62 address.*/
/*! EEPROM page 63 address.*/

// these addresses are for DLMS ,there is No place in EEPROM


#define FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS        EEPROM_PAGE_12

#define  FILE_SYS_GET_BACKUP_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS        EEPROM_PAGE_13
#define  EEPROM_RTC_INIT_FLAG_ADD                  EEPROM_PAGE_45
#define RTC_SAVE_TIME_ADDR                        (EEPROM_RTC_INIT_FLAG_ADD+2)
#define RTC_SAVE_DATE_ADDR                        (RTC_SAVE_TIME_ADDR+4)
#define  INT_ELAPSED_SAVE_TIME_ADDR               (RTC_SAVE_DATE_ADDR+4)
   
#define  EEPROM_TEST_PAGE               (uint32_t)(46*EEPROM_PAGE_SIZE)
/*! @}*/
#define   FILE_SYS_ERR_LOAD_FLAG                0x01
#define   CTRL_ERR_LOAD_FLAG                    0x02
#define   COMM_ERR_LOAD_FLAG                    0x20
#define   CTRL_OPTIONAL_ERR_LOAD_FLAG           0x40
#define   CTRL_SECOND_OPTIONAL_ERR_LOAD_FLAG    0x80
#define   CTRL_NEW_SYS_ERR_LOAD_FLAG            0x100
#define   CTRL_VAR_ERR_LOAD_FLAG                0x400
#define   STS_CFG_ERR_LOAD_FLAG                 0x2000
#define   CTRL_LAST_TAMPERS_LOAD_FLAG           0x4000
#ifdef ANSI_STACK
#endif

#ifdef GPRS_SEND_NOTIFICATION_FEATURE
//3 bytes (struct gprs_Notification_st)
#endif
#else
/*! EEPROM page 30 address.*/
#define EEPROM_PAGE_30 (uint32_t)(30*EEPROM_PAGE_SIZE)
/*! EEPROM page 32 address.*/
/*! EEPROM page 52 address.*/
/*! EEPROM page 62 address.*/
/*! EEPROM page 55 address.*/
/*! EEPROM page 56 address.*/
/*! EEPROM page 58 address.*/
/*! EEPROM page 60 address.*/
/*! EEPROM page 23 address.*/
/*! EEPROM page 24 address.*/
/*! EEPROM page 25 address.*/
/*! EEPROM page 26 address.*/
/*! EEPROM page 29 address.*/
#define EEPROM_PAGE_29 (uint32_t)(29*EEPROM_PAGE_SIZE)




/*! EEPROM page 508 address.*/
/*! EEPROM page 511 address.*/

#define EEPROM_PAGE_397 (uint32_t)((uint16_t)397* (uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_401 (uint32_t)((uint16_t)401* (uint16_t)EEPROM_PAGE_SIZE)
#define EEPROM_PAGE_402 (uint32_t)((uint16_t)402* (uint16_t)EEPROM_PAGE_SIZE)
/*! EEPROM page 199 address.*/
/*! EEPROM page 200 address.*/
/*! EEPROM page 201 address.*/
/*! EEPROM page 202 address.*/
/*! EEPROM page 203 address.*/
/*! EEPROM page 204 address.*/
/*! EEPROM page 205 address.*/
/*! EEPROM page 206 address.*/
/*! EEPROM page 206 address.*/
#define EEPROM_PAGE_425 (uint32_t)(425*EEPROM_PAGE_SIZE)//new page 

#define FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS        EEPROM_PAGE_12

#define  FILE_SYS_GET_BACKUP_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS        EEPROM_PAGE_13
#define  EEPROM_RTC_INIT_FLAG_ADD                  EEPROM_PAGE_425
#define RTC_SAVE_TIME_ADDR                        (EEPROM_RTC_INIT_FLAG_ADD+2)
#define RTC_SAVE_DATE_ADDR                        (RTC_SAVE_TIME_ADDR+4)
#define  INT_ELAPSED_SAVE_TIME_ADDR               (RTC_SAVE_DATE_ADDR+4)
   
#define  EEPROM_TEST_PAGE               (uint32_t)(426*EEPROM_PAGE_SIZE)
/*! @}*/
#define   FILE_SYS_ERR_LOAD_FLAG                0x01
#define   CTRL_ERR_LOAD_FLAG                    0x02
#define   COMM_ERR_LOAD_FLAG                    0x20
#define   CTRL_OPTIONAL_ERR_LOAD_FLAG           0x40
#define   CTRL_SECOND_OPTIONAL_ERR_LOAD_FLAG    0x80
#define   CTRL_NEW_SYS_ERR_LOAD_FLAG            0x100
#define   CTRL_VAR_ERR_LOAD_FLAG                0x400
#define   CTRL_LAST_TAMPERS_LOAD_FLAG           0x4000
   
#ifdef GPRS_SEND_NOTIFICATION_FEATURE
//3 bytes (struct gprs_Notification_st)
#endif
#endif


/*------------------------------------------------------------------------------------
  from EEPROM.h
------------------------------------------------------------------------------------*/

#include "config.h"
/* #include "EEPROM_Addresses.h" -- inlined in this file */

#if (MicroController == Micro_V94XX)
#define EEPROM_AT24C256_I2C_ADDRESS   0xa4    //0x50
#elif (defined(EM110_BOARD) || defined(EM130_BOARD) || defined(EM210_BOARD) || defined(EM122U_BOARD)|| defined(EM330_BOARD))
#define EEPROM_AT24C256_I2C_ADDRESS   0xa4    //0x50
#else
#define EEPROM_AT24C256_I2C_ADDRESS   0xa0    //0x50
#endif

extern volatile uint16_t eeprom_load_errors_register; // indicate if we load wrong data from the eeprom 
__packed struct Eeprom_check_t
{
  uint8_t eeprom_check_arr[30];
  uint8_t eeprom_crc;

};

void EEPROM_Initialize(void);
void EEPROM_PowerDown(void);
TBOOL EEPROM_write(uint8_t EEPROM_Add ,  uint8_t* TxData,uint32_t Address, uint32_t numBytes);
TBOOL EEPROM_read(uint8_t EEPROM_Add ,  uint8_t* RxData,uint32_t Address, uint32_t numBytes);
void EEPROM_Failure_Handler(void);
uint8_t EEPROM_Erease(uint8_t EEPROM_Add);
uint8_t EEPROM_test(uint8_t EEPROM_Add);
void eeprom_check_init(void);
void check_Eeprom_Fail(void);




/*------------------------------------------------------------------------------------
  from file_sys.h
------------------------------------------------------------------------------------*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               F I L E    S Y S T E M

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file file_sys.h
 *
 * @brief This file contains Addresses of all files in storage media.
 *
 * @details This file contains the required definitions, types for saving
 * and loading in storage media.
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
 *   <td> 27/03/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Save control data only with power down and not with any log update.</td>
 * </tr>
 * <tr>
 *   <td> 24/09/12 </td>
 *   <td> aibraheem </td>
 *   <td> Integrate with metering. </td>
 * </tr>
 * <tr>
 *   <td> 13/09/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify comments and Doxygen groups. </td>
 * </tr>
 * <tr>
 *   <td> 26/08/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add support to use internal MCU memory in file system. </td>
 * </tr>
 * <tr>
 *   <td> 03/07/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add get multiple records function and config meter log. </td>
 * </tr>
 * <tr>
 *   <td> 28/05/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup FILE_SYS File System
 * @ingroup METER
 * File system is responsible for:
 *   -# Read and write in files of internal modules.
 *   -# Billing period history records.
 *   -# Money transaction records.
 *   -# Event log records.
 *   -# Profile records.
 *
 * \n<b>References</b>\n
 * - Analysis document of file system.
 * - Design document of file system.
 *
 * @brief File system is responsible for saving and loading in storage media for
 * different modules and all logging.
 */
/*!
 * @defgroup FILE_SYS_CORE Application Core APIs
 * @ingroup FILE_SYS
 * @brief File system initialization functions.
 */
/*!
 * @defgroup FILE_SYS_READ Read System Variables APIs
 * @ingroup FILE_SYS
 * @brief Used to read data from file system module.
 */
/*!
 * @defgroup FILE_SYS_CMD Commands APIs
 * @ingroup FILE_SYS
 * @brief File system command interface APIs.
 */
/*!
 * @defgroup FILE_SYS_EVENT Events APIs
 * @ingroup FILE_SYS
 * @brief File system event interface APIs.
 */

/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
/* #include "EEPROM.h" -- inlined in this file */
/* #include "basic_srv.h" -- inlined in this file */
/* #include "EEPROM_Addresses.h" -- inlined in this file */
#include "FLASH_Addresses.h"
/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------------------
                                    External Variables
------------------------------------------------------------------------------------------*/
extern uint8_t first_init;
extern uint8_t trf_recalculation_prv_month;
extern volatile uint16_t eeprom_load_errors_register; // indicate if we load wrong data from the eeprom
extern uint8 is_Flash_work ; 
/*!
 * @addtogroup FILE_SYS_READ
 * @{
 */
#ifdef FILE_SYS_USE_INT
/*! The address of first byte in internal data EEPROM.*/
#define INTERNAL_DATA_EEPROM_ADRS FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS
/*! Address of section 0 in internal EEPROM.*/
#define FILE_SYS_GET_INTRNL_MEM_SEC0_ADRS INTERNAL_DATA_EEPROM_ADRS
#endif
/*! Address of first byte in the space reserved for tariff and payment configuration.*/
/*! Address of first byte in the space reserved for tariff and payment waiting tariff configuration.*/
/*! Address of first byte in the space reserved for tariff and payment system variables.*/
/*! Address of first byte in the space reserved for STS module.*/
/*! Address of first byte in the space reserved for metering application configuration.*/
/*! Address of first byte in the space reserved for metering calibration configuration.*/
/*! Address of first byte in the space reserved for metering system variables.*/
#define FILE_SYS_GET_METERING_SYS_VAR          EEPROM_PAGE_8
/*! Address of first byte in the space reserved for display configuration.*/
/*! Address of first byte in the space reserved for control configuration.*/
#define FILE_SYS_GET_CONTROL_CFG_ADRS          EEPROM_PAGE_14
/*! Address of first byte in the space reserved for control system variables.*/
#define FILE_SYS_GET_CONTROL_SYS_VAR_ADRS      EEPROM_PAGE_15
/*! Address of first byte in the space reserved for GPRS communication data.*/
#define FILE_SYS_GET_COM_GPRS_DATA_ADRS        EEPROM_PAGE_17
/*! Address of first byte in the space reserved for RF communication data.*/
/*! Address of first byte in the space reserved for RFID communication data.*/
#define FILE_SYS_GET_COM_RFID_DATA_ADRS        EEPROM_PAGE_19
/*! Address of first byte in the space reserved for RFID communication data.*/
#define FILE_SYS_GET_CTRL_VARIABLE_OPTIONAL_DATA_ADRS        EEPROM_PAGE_20
#define FILE_SYS_GET_BACKUP_CTRL_VARIABLE_OPTIONAL_DATA_ADRS        EEPROM_PAGE_21
/*! Address of first byte in the space reserved for file system control.*/
#define FILE_SYS_GET_FILE_SYS_CTRL_ADRS        EEPROM_PAGE_22
/*! Address of first byte in the space reserved for metering application configuration backup address .*/
#define FILE_SYS_GET_CONTROL_CFG_BACKUP_ADRS           EEPROM_PAGE_29
#ifdef FM24C128D_2_Wire_Serial_EEPROM
#define FILE_SYS_GET_DISPLAY_SHOW                      EEPROM_PAGE_30  ///display take 3 Pages to save default configuration 
#define FILE_SYS_CONTROL_NEW_VAR_ADRS_BACKUP           EEPROM_PAGE_34 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_NEW_VAR_ADRS              EEPROM_PAGE_37 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_VAR_BACKUP_ADRS           EEPROM_PAGE_40// 2pages

// these addresses are for DLMS
//reserved 5 pages for DLMS CONFIG








#else
#define FILE_SYS_GET_DISPLAY_SHOW                      EEPROM_PAGE_504  ///display take 3 Pages to save default configuration 

#define FILE_SYS_CONTROL_NEW_VAR_ADRS_BACKUP           EEPROM_PAGE_402 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_NEW_VAR_ADRS              EEPROM_PAGE_397 // 519B , 3pages
#define FILE_SYS_GET_CONTROL_VAR_BACKUP_ADRS           EEPROM_PAGE_401
#ifdef FILE_SYS_LOG
#ifdef TRF_BP_HISTORY
/*! Address of first byte in the space reserved for BPH records.*/
#endif
#ifdef PYMT_MONY_TRANS
/*! Address of first byte in the space reserved for money transaction records.*/
#endif
#ifdef CTRL_CFG_METER_LOG
/*! Address of first byte in the space reserved for profile records.*/
#endif
//#ifdef MTR_LOAD_PROFILE
/*! Address of first byte in the space reserved for configure meter records.*/
//#endif
#endif









#endif
/*! @} */

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
#ifdef FILE_SYS_LOG
/*!
 * @par Description:
 *   This enumerator contains all records types saved in EEPROM.
 *
 * @ingroup FILE_SYS
 */
typedef enum
{
#ifdef TRF_BP_HISTORY
  FILE_SYS_BPH_RECORD=0,       /*!< BPH record.*/
#endif
#ifdef CTRL_EVNT_LOG
  FILE_SYS_EVENT_RECORD,     /*!< Event record.*/
#endif
#ifdef PYMT_MONY_TRANS
  FILE_SYS_MONEY_RECORD,     /*!< Money record.*/
#endif
//#ifdef MTR_LOAD_PROFILE
  FILE_SYS_PROFILE_RECORD,   /*!< Profile record.*/
//#endif
#ifdef CTRL_CFG_METER_LOG
  FILE_SYS_CFG_METER_RECORD, /*!< Configure meter log.*/
#endif

#ifdef DLMS_ENABLED
  FILE_SYS_MD_RECORD, /*!< Configure meter log.*/
  DISCONNECT_CONTROL_EVENTS_LOG_ID,
  METER_ERRORS_EVENTS_LOG_ID,
  CUSTOME_WITH_PARAMETERS_EVENTS_LOG_ID,
  FIRMWARE_SELF_CHECK_LOG_ID,
  STANDARD_EVENTS_LOG_ID, //(+ event log)
  POWER_EVENTS_LOG_COC_ID, //(+ event log)
  FILE_SYS_LP_LIST1_ID,
  FILE_SYS_LP_LIST2_ID,
  FILE_SYS_LP_LIST4_ID,
  FILE_SYS_LP_LIST5_ID,
  PROFILE_CONUMPTION_MONTHLY_ID,  
#endif
#ifdef G2_DM
  FILE_SYS_TAMPER_RECORD,              /*!< Tamper record.*/
  FILE_SYS_REMOVED_TAMPER_RECORD,     /*!< Removed Tamper record.*/
  FILE_SYS_YEARLY_CURRENT_MD,     /*!< Removed Tamper record.*/
  FILE_SYS_YEARLY_POWER_MD,     /*!< Removed Tamper record.*/
#endif
    FILE_SYS_MAX_LOGS,/*!< Number of record types.*/

} file_sys_record_type_t;

/*!
 * @addtogroup FILE_SYS_EVENT
 * @{
 */
typedef enum
{
  FILE_SYS_PWR_DN = (uint8_t)0,
  FILE_SYS_NEW_HR = (uint8_t)1,
  FILE_SYS_NEW_SEC,
  FILE_SYS_NEW_DAY,
  FILE_SYS_PWR_UP,
} file_sys_event_type_t;
/*! @} */

__packed typedef struct
{
  uint16_t num_saved_record[FILE_SYS_MAX_LOGS];  /*!< Number of saved records in each log.*/
  uint16_t record_index[FILE_SYS_MAX_LOGS];      /*!< Index of each log.*/
  uint8_t crc;                                   /*!< CRC of this structure*/
} record_control_t;
extern const uint16_t max_num_record[FILE_SYS_MAX_LOGS];
#endif

/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
extern const uint8_t size_of_record[];

/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/

/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/*! @addtogroup FILE_SYS_READ
 * @{
 */
/*!
 * @brief Read non volatile data.
 *
 * @par Description:
 * Read data from non voltaile storage media with max 65535 Bytes.
 *
 * @param [in] data_ptr   Address of buffer to get data in.
 * @param [in] address    Address of first byte of non volatile memory to be read.
 * @param [in] bytes_num  Number of bytes to read.
 *
 * @return
 * - TRUE if success and FALSE if error.
 */
uint8_t file_sys_read(uint8_t* data_ptr, uint32_t address, uint32_t bytes_num);
/*!@}*/

/*! @addtogroup FILE_SYS_CMD
 * @{
 */
/*!
 * @brief Write non volatile data.
 *
 * @par Description:
 * Write data in non voltaile storage media with max 65535 Bytes.
 *
 * @param [in] data_ptr   Address of buffer to get data from.
 * @param [in] address    Address of first byte of non volatile memory to be written.
 * @param [in] bytes_num  Number of bytes to write.
 *
 * @return
 * - TRUE if success and FALSE if error.
 */
uint8_t file_sys_write(uint8_t* data_ptr, uint32_t address, uint32_t bytes_num);
/*!@}*/
void test_flash_internal();

#ifdef FILE_SYS_USE_INT
/*! @addtogroup FILE_SYS_READ
 * @{
 */
/*!
 * @brief Read from internal EEPROM.
 *
 * @par Description:
 * This function is used to read from internal EEPROM with maximum 255 bytes.
 *
 * @param [in] mem_adr Address of internal EEPROM you want to read data from.
 * @param [out] data_ptr    Address of buffer to get the data in
 * @param [in] size Size of data in Bytes to read from internal EEPROM.
 *
 * @return
 * - None.
*/
void file_sys_read_int(uint32_t mem_adr, uint8_t *data_ptr, uint8_t size);
/*!@}*/

/*! @addtogroup FILE_SYS_CMD
 * @{
 */
/*!
 * @brief Write in internal EEPROM.
 *
 * @par Description:
 * This function is used to write in internal EEPROM with maximum 255 bytes.
 *
 * @param [in] mem_adr Address of internal EEPROM you want to write data at.
 * @param [in] data_ptr    Address of buffer to get the data from.
 * @param [in] size Size of data in Bytes to write in internal EEPROM.
 *
 * @return
 * - None.
*/
void file_sys_write_int(uint32_t mem_adr, uint8_t *data_ptr, uint8_t size);
/*! @} */
#endif
/*!
 * @brief Get the index of the wanted record of certain type .
 *
 * @par Description:
 * Get the index of the wanted record of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 *
 * @return
 * - 16 bit: the number of saved records.
 */
uint16_t file_sys_get_index_record(uint8_t record_type);

/*!
 * @brief Get the max number of records .
 *
 * @par Description:
 * Get the max number of records of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 *
 * @return
 * - 16 bit: the number of saved records.
 */
uint32_t file_sys_get_the_max_No_record(uint8_t record_type);
#ifdef FILE_SYS_LOG
/*! @addtogroup FILE_SYS_CMD
 * @{
 */
/*!
 * @brief Clear records of certain type.
 *
 * @par Description:
 * Clear all record saved in storage media of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t that will be cleared
 *
 * @return
 * - None.
 *
 * @note
 * - Clearing storage media is not done by reset pages in storage media but, only by reset index
 *   and number of saved records to be overwritten by coming records.
 */
void file_sys_clear_log(uint8_t record_type);
/*! @} */

/*! @addtogroup FILE_SYS_READ
 * @{
 */
/*!
 * @brief Get number of saved records of certain type.
 *
 * @par Description:
 * Get number of saved in storage media of certain type like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 *
 * @return
 * - 16 bit: the number of saved records.
 */
uint32_t file_sys_get_num_record(uint8_t record_type);

/*!
 * @brief Get a certain record from records saved in storage media.
 *
 * @par Description:
 * Get a record data saved in storage media of certain type like:
 * - Profile record
 * - Money transaction record
 * - Billing period history record
 * - Event record
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 * @param [in] record_num  Number of record to be read (0 -> max-1)
 * @param [out] data_ptr   Address of buffer to get the data in
 *
 * @return
 * - None.
 *
 * @note
 * If we can't access the storage media, a communication error event is published.
 */
void file_sys_get_record(uint8_t record_type, uint16_t record_num, uint8_t *data_ptr);

/*!
 * @brief Get number of records saved in storage media.
 *
 * @par Description:
 * Get certain number of records saved in storage media of certain type like:
 * - Profile record
 * - Money transaction record
 * - Billing period history record
 * - Event record
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 * @param [in] from        First record number you want to read (0 -> max-1)
 * @param [in] num_rec  Number of record to be read (1 -> max)
 * @param [out] data_ptr   Address of buffer to get the data in
 *
 * @return
 * - Size of data written in buffer if read is done successfully, 0 if no data read.
 */
uint16_t file_sys_get_mlti_rec(uint8_t record_type, uint16_t from, uint16_t num_rec, uint8_t *data_ptr);
/*! @} */

/*! @addtogroup FILE_SYS_CMD
 * @{
 */
/*!
 * @brief Save a certain record in storage media.
 *
 * @par Description:
 * Save a record data in storage media of certain type like:
 * - Profile record
 * - Money transaction record
 * - Billing period history record
 * - Event record
 * - Configure meter records
 *
 * @param [in] record_type Type of records in #file_sys_record_type_t
 * @param [in] data_ptr    Address of buffer to get the data in
 *
 * @return
 * - None.
 *
 * @note
 * If we can't access the storage media, a communication error event is published.
 */
void file_sys_save_record(uint8_t record_type, uint8_t *data_ptr);
/*! @} */

/*! @addtogroup FILE_SYS_EVENT
 * @{
 */
/*!
 * @brief Tell file system module with a new event.
 *
 * @par Description:
 * Tell file system with new event.
 *
 * @param [in] event_code Code of event #file_sys_event_type_t
 *
 * @return
 * - None.
 */
void file_sys_event(uint8_t event_code);
/*! @} */
#endif /* end of FILE_SYS_LOG */

/*!
 * @ingroup FILE_SYS_CORE
 *
 * @brief Initialize logs containing records of different types.
 *
 * @par Description:
 * Initialize all logs containing records of different types like:
 * - Profile records
 * - Money transaction records
 * - Billing period history records
 * - Event records
 * - Configure meter records
 *
 * @return
 * - None.
 *
 * @note
 * If storage media can't be read, a communication error event is published.
 * If the CRC check failed, a data corrupt event is published.
 */
void file_sys_init(void);




#define BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS      3

#ifdef DLMS_ENABLED
extern const TCapture_Object BillingProfile_CaptureObjects[BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS];
extern const uint8_t BillingProfile_CaptureObjects_XDR_Size[BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS];
#endif

#ifdef TEST_BILLING_RECORD
void test_filling_billing_records(uint8_t number_of_records);
#endif

TU08 FileSystem_gStoreItemList(TU08 recordID, TU08* ptrData);
TU08 FileSystem_gGetMaxNumRecord(TU08 recordID, TU32* max_records_num);
TU08 FileSystem_gClearList (uint8_t record_type);
TU08 FileSystem_gInitList ( TU08 recordID , TU16 record_size );
TU08 FileSystem_gGetItemList(uint8_t recordID, TU08* ptrData, TU32 index);
TU08 FileSystem_gGetEntriesInList(TU08 recordID, TU32* entries_number);
TU08 FileSystem_gGet_ListLastIndex_Inuse(TU08 recordID, TU32* index , TU32* max_num_entries, TU32* current_num_of_entries);
TU08 FileSystem_gGetLast_ActualIndex_InUse_InList(TU08 recordID, TU32* actual_index);
uint16_t file_Sys_Get_Latest_Records(uint8_t record_type, uint16_t num_rec, uint8_t *data_ptr);


/*------------------------------------------------------------------------------------
  from meter_init.h
------------------------------------------------------------------------------------*/

/*******************************************************************************
**                                                                            **
**  PROJECT   :                                                               **
**                                                                            **
**  File Name : HW_Init.h                                                     **
**                                                                            **
**  File Type : header file for the HW_Init                                   **
**                                                                            **
**  AUTHOR    : Mohamed Mahmoud Zakaria                                       **
**                                                                            **
**  PURPOSE   : It contains the prototypes of HW_Init                         **
**                                                                            **
**  History   : May 30, 2011                                                  **
**                                                                            **
**  Version   : 1.0                                                           **
*******************************************************************************/


/* To eliminate including the file more than one time */
#ifndef METER_INIT_H
#define METER_INIT_H


/********************************** Headers ***********************************/

#include "config.h"      /* Header file contains the standard types used */
#include "CLK_Interface.h"
#include "GPIO_Interface.h"
#include "SPI_Interface.h"
#include "I2C_Interface.h"
#include "ADC_Interface.h"
#include "RTC_Interface.h"
#include "UART_Interface.h"
/* #include "file_sys.h" -- inlined in this file */

#if (MicroController == Micro_V94XX)
#include "V94XX_PMU.h"
#include "V94XX_ANA.h"
#include "V94XX_CORTEX.h"
#else
#include "v85xx_pmu.h"
#include "v85xx_ana.h"
#include "v85xx_cortex.h"
#endif


/* #include "basic_srv.h" -- inlined in this file */
#include "core_cm0.h"
/******************************************************************************/



//#define GAIN_CALIB_CONST  ((double)2147483648)
//#define STD_MTR_CURR      ((float)50000000) /* in micro ampere*/

/*******************************************************************************
                           Functions prototypes
 ******************************************************************************/

/*------This is a list of functions provide for upper layer modules.----------*/

void vMC_DriversInit(void);               //PINS
void vMeter_Init(void);
void IRQ_init(void);
void detatch_sensor_init(void);

/*------------------------------------------------------------------------------
              End function prototypes
 -----------------------------------------------------------------------------*/





#endif  /*end 0f if METER_INIT_H defined */
/*******************************************************************************
                                 End of file
*******************************************************************************/


/*------------------------------------------------------------------------------------
  from V9340.h
------------------------------------------------------------------------------------*/

#include "config.h"
/* #include "meter_init.h" -- inlined in this file */

#ifdef V9340_ENABLE




//extern uint8_t V9340_comm_err_flag ;

#ifdef ANSI_FEATURE

                                            

#elif defined (EM_DL130_BOARD)





#else


#endif




#define V9340_UART_COMM_ERR          0x01
#define V9340_CKSM_COMM_ERR         0x02
//#define V9340_COMM_ERR_STRT     ((V9340_comm_err_flag & V9340_UART_COMM_ERR) || (V9340_comm_err_flag & V9340_CKSM_COMM_ERR))


#if  !defined(ANSI_FEATURE) &&  !defined(EM_DL130_BOARD)

#ifdef CONSTANT_3200 /// shunt 250U


#endif
#endif   //ANSI_FEATURE



/*!
* @par Description:
*   This enumerator contains all available calibration commands
*/
typedef enum
{
  V9340_STRT_MAN_CAL_CMD ,     /*!< Start manual calibration command.*/
  V9340_NXT_MAN_CAL_CMD ,     /*!< Next calibration command.*/
  V9340_STRT_AUTO_CAL_CMD,     /*!< Start auto calibration command.*/
  V9340_END_AUTO_CAL_CMD

} V9340_cal_cmd_t;
/*for test only*/

__packed typedef struct {
  uint32_t V9340_prev_ActpulseCount;
  uint32_t V9340_curr_ActpulseCount ;
  
  uint32_t V9340_prev_ReactpulseCount;
  uint32_t V9340_curr_ReactpulseCount ;
  uint32_t V9340_prev_AppPulseCount;
  uint32_t V9340_curr_AppPulseCount;
} V9340_pulseCount_t;


__packed typedef struct {
  uint64_t V9340_rms_curr;
  uint64_t V9340_rms_vol;
  uint64_t V9340_act_pow;
  uint64_t V9340_react_pow;
}V9340_mtr_avg_readings_t;


__packed typedef struct {

  uint32_t V9340_calib_rms_curr[MTR_NUM_OF_CH];
  uint32_t V9340_calib_rms_vol[MTR_NUM_OF_CH];
  uint32_t V9340_calib_act_pow[MTR_NUM_OF_CH];
  uint8_t   V9340_cal_flag;          /*!< Contains CRC of all data in this structure.*/
}V9340_calib_t;
__packed typedef struct {

  uint32_t V9340_ANCTRL0_Netural_Reg;
  uint8_t  V9340_Reserved[28];
  uint8_t  V9340_is_Calib_happened;
  uint8_t  V9340_crc;          /*!< Contains CRC of all data in this structure.*/
}V9340_calib_registers_t;
__packed typedef struct {
  uint64_t V9340_rms_curr[MTR_NUM_OF_CH];
  uint64_t V9340_rms_vol[MTR_NUM_OF_CH];
  uint64_t V9340_ph_angle;
  uint64_t V9340_pf[MTR_NUM_OF_CH];
  uint64_t V9340_line_freq;
  uint64_t V9340_act_pow[MTR_NUM_OF_CH];
  uint64_t V9340_react_pow[MTR_NUM_OF_CH];
  uint64_t V9340_act_energy;
}V9340_mtr_readings_t;


 typedef struct 
{ uint16_t V9340_addr;
  uint32_t V9340_value;
}V9340_reg_values_t;


typedef enum {
  V9340_RESET_NONE_MODE           = (uint8_t) 0,
  V9340_RESET_DATA_CRPT           = (uint8_t) 1<<0,
  V9340_RESET_UART_PROBLEM        = (uint8_t) 1<<1,
  V9340_RESET_SWITCH_LINE_NETURAL = (uint8_t) 1<<2,
  V9340_RESET_WRONG_FREQUENCY     = (uint8_t) 1<<3,
  V9340_RESET_CALIBRATE           = (uint8_t) 1<<4,
  V9340_RESET_POWER_UP            = (uint8_t) 1<<5,
  V9340_RESET_MAIN_INIT           = (uint8_t) 1<<6,
  V9340_RESET_NEW_ENG_FAIL        = (uint8_t) 1<<7,
}V9340_RESET_MODULE_REASON_enumt;

extern uint8_t V9340_init_numbers ;
extern uint8_t V9340_flag_get_volt ; 
extern V9340_mtr_readings_t V9340_mtr_inst_readings;
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/
void V9340_test_uart();
void V9340_task(void);
void V9340_cal_handler_task(void);
void V9340_cal_cmd(uint8_t arg1);
uint8_t V9340_cal_state(void);
void V9340_inst_read(uint8_t phase);
uint8_t V9340_Init_LoadcalibParam( uint32_t* Cksum_ptr);
uint8_t V9340_Init(uint8_t init_reason);
uint8_t V9340_chkFailure(void);
void V9340_read_initial_PulseCount(void);
void V9340_calib(void);
double V9340_calc_act_pow_error(uint64_t V9340_avg_reading);
int64_t V9340_Pow_Gain_Calib(double V9340_pow_error, uint64_t V9340_avg_reading);
int32_t V9340_RMS_Gain_Calib(uint32_t V9340_avg_reading, float V9340_std_mtr_value);
void V9340_chk_active_phases(void);
uint8_t V9340_get_ins_reverse(void);
uint64_t V9340_get_ActEnergy_Accum( uint8_t* Negative_flag_ptr);
void V9340_clr_ins_reverse(uint8_t arg1) ;
uint64_t V9340_instant_active_power_read_microW();
void V9340_check_line_or_Netural(uint32_t Ia , uint32_t Im);
uint8 V9340_get_channel_I ();
void V9340_rx_Isr(uint8_t rx_byte);
void V9340_read_inst_task();
void V9340_Energy_Flag();
void V9340_Volt_Flag();
uint8_t V9340_Get_Negative_Flag();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_get_Reset_Flag
// Description      : This function is used to get the reason of reseting the module
// Arguments        : NONE
// Return           : one of this enum V9340_RESET_MODULE_REASON_enumt
// Output           : NONE
//-----------------------------------------------------------------------------
// Author           : Ibrahem Mostafa
// Date             : 30/03/2020
//=============================================================================
uint8_t V9340_get_Reset_Reason (void);
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_get_ReactEnergy_Accum
// Description      : This function is used to get the reactive energy accmulation for each phase
// Arguments        : phase : the phase(0 for phase A, 1 for phase B, 2 for phase C)
// Return           : the reactive energy accmulation for each phase
// Output           : NONE
//-----------------------------------------------------------------------------  
uint64_t V9340_get_ReactEnergy_Accum();
uint32_t V9340_Reactpulse_counter_read();

//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_load_Param
// Description      : This function is used to load from eeprom the calibration parameters
// Arguments        : None.
// Return           : true if load from eeprom ok.
// Output           : NONE
//===========================================================================
uint8_t V9340_load_Param ();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_get_rms_voltage
// Description      : This function is used to get the instantaneous total rms voltage value for each phase
// Arguments      : arg1 : the phase (0 for ph A, 1 for ph B, 2 for ph C)
// Return           : The instantaneous rms voltage value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9340_get_rms_voltage();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_Set_Default_Param
// Description      : This function is used to set and save the default of the calibration parameters
// Arguments        : None.
// Return           : NONE
// Output           : NONE
//===========================================================================
void V9340_Set_Default_Param();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_get_rms_current
// Description      : This function is used to get the instantaneous total rms current value in Micro Amperes
// Arguments      : arg1 : the phase (0 for ph A, 1 for ph B, 2 for ph C)
// Return           : The instantaneous total rms current value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9340_get_rms_current(uint8_t V9340_arg1);
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_instant_total_irms_read
// Description      : This function is used to read the instantaneous total rms current value for each phase
// Arguments      : LocalPhase: (0 for ph A, 1 for ph B)
// Return           : The instantaneous total rms current value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9340_instant_total_irms_read(uint8_t LocalPhase) ;

// FUNCTION
//=============================================================================
// Name             : V9340_instant_total_vrms_read
// Description      : This function is used to read the instantaneous total rms voltage value
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C)..
// Return           :the instantaneous total rms voltage value
// Output           : NONE
//=============================================================================
uint32_t V9340_instant_total_vrms_read() ;

//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9340_instant_tot_active_power_read
// Description      : This function is used to get the total instantaneous active power  
// Arguments         : NONE
// Return           :the total instantaneous active power
// Output           : NONE
//-----------------------------------------------------------------------------

uint64_t V9340_instant_active_power_read_microW();

uint32_t V9340_instant_reactive_power_read_milliVAR(void);
//=============================================================================
// Name             : V9340_line_freq_read
// Description      : This function is used to get the line frquency for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the line frequency
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9340_line_freq_read() ;
//=============================================================================
// Name             : V9340_instant_total_PF_read
// Description      : This function is used to get the instantaneous power factor for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the instantaneous power factor
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9340_instant_total_PF_read(uint64_t power , uint32_t volt , uint32_t current) ;
//=============================================================================
// Name             : V9340_Actpulse_counter_read\
// Description      : This function is used to get the pulse counter for active power for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the pulse counter for active power
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9340_Actpulse_counter_read();
uint32_t V9340_Apperantpulse_counter_read();
//=============================================================================
// Name             : V9340_INT_flagReg_read
// Description      : This function is used to read the interrupt flag register (V9340_REG_IRQFLAG)
// Arguments         : NONE
// Return           :the interrupt flag register (V9340_REG_IRQFLAG)
// Output           : NONE
//------------------------------------------------------------------------
uint8_t V9340_get_reg ( uint16_t Address, uint32_t* reg_value_ptr);
uint8_t V9340_set_reg ( uint16_t Address, uint32_t reg_value); 

uint8_t V9340_INT_flagReg_read(uint32_t * localFlagReg);
void V9340_Recive_DATA(uint8_t byte);
void V9340_Switch_line_or_Netural(uint8_t is_Switch_Line);
uint8_t V9340_testing ();
void V9340_SelfTest_SetV9340Param(void);


uint32_t V9340_instant_apparent_power_read_milliVA(void); 
uint64_t V9340_instant_apparent_power_read_microVA(void);


uint32_t V9340_instant_active_power_read_milliW(void);
uint64_t V9340_get_ApperantEnergy_Accum();

//extern 

extern uint8_t V9340_down_volt_flag;/* flag to tell mtr.c the volt was down to harry going to ctrl to save */
extern uint8_t V9340_data_crpt_flag ;
extern uint8_t first_init_pulses;
extern volatile uint8_t V9340_Rx_ByteChkSum;
extern uint8_t V9340_Rx_I_RecivedByte;
extern uint8_t V9340_RX_Buffer_ISR_Counter;
extern uint8_t V9340_RX_Buffer[10];
extern uint64_t V9340_CounterValueFromTheEndOfInitV9340;

volatile extern  V9340_calib_registers_t V9340_Calib_Reg_Values;
uint8_t V93xx_CalRcClk(uint8_t Freq);

#endif






/*------------------------------------------------------------------------------------
  from V9381.h
------------------------------------------------------------------------------------*/

#include "config.h"
/* #include "meter_init.h" -- inlined in this file */

#ifdef V9381_ENABLE











//extern uint8_t V9381_comm_err_flag ;

#ifdef ANSI_FEATURE

#define GAIN_CALIB_CONST              ((double)2147483648)
#define STD_MTR_CURR                  ((float)15000000)     /*  A in micro ampere*/

#define VOL_RATIO_FACTOR              ((uint32_t)167375)      //306855)        //((uint32_t)1650)
#ifdef Freq_60HZ
#else
#endif


#elif defined (EM_DL130_BOARD)


#define GAIN_CALIB_CONST  ((double)2147483648)
#define STD_MTR_CURR      ((float)5000000) /*  A in micro ampere*/

#define VOL_RATIO_FACTOR              ((uint32_t)167375)        //((uint32_t)1650)

#else

#define GAIN_CALIB_CONST  ((double)2147483648)
#define STD_MTR_CURR      ((float)5000000) /*  A in micro ampere*/

#endif



#define V9381_UART_COMM_ERR          0x01 
#define V9381_CKSM_COMM_ERR         0x02 
//#define V9381_COMM_ERR_STRT     ((V9381_comm_err_flag & V9381_UART_COMM_ERR) || (V9381_comm_err_flag & V9381_CKSM_COMM_ERR))
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if  !defined(ANSI_FEATURE) &&  !defined(EM_DL130_BOARD)


#ifdef CONSTANT_3200 /// shunt 250U
#define VOL_RATIO_FACTOR              ((uint32_t)167375)        //((uint32_t)1650)


#endif


#endif     //ANSI_FEATUE
/*!
* @par Description:
*   This enumerator contains all available calibration commands
*/
typedef enum
{
  STRT_MAN_CAL_CMD ,     /*!< Start manual calibration command.*/
  NXT_MAN_CAL_CMD ,     /*!< Next calibration command.*/
  STRT_AUTO_CAL_CMD,     /*!< Start auto calibration command.*/
  END_AUTO_CAL_CMD

} cal_cmd_t;
/*for test only*/

__packed typedef struct {
  uint32_t prev_ActpulseCount;
  uint32_t curr_ActpulseCount ;
  
  uint32_t prev_ReactpulseCount;
  uint32_t curr_ReactpulseCount ;

  uint32_t prev_AppPulseCount;
	uint32_t curr_AppPulseCount;
} pulseCount_t;


__packed typedef struct {
  uint64_t rms_curr;
  uint64_t rms_vol;
  uint64_t act_pow;
  uint64_t react_pow;
}mtr_avg_readings_t;


__packed typedef struct {

  uint32_t calib_rms_curr[MTR_NUM_OF_CH];
  uint32_t calib_rms_vol[MTR_NUM_OF_CH];
  uint32_t calib_act_pow[MTR_NUM_OF_CH];
  uint8_t   cal_flag;          /*!< Contains CRC of all data in this structure.*/
}V9381_calib_t;
__packed typedef struct {

  uint32_t ANCTRL0_Netural_Reg;
  uint8_t  Reserved[28];
  uint8_t  is_Calib_happened;
  uint8_t  crc;          /*!< Contains CRC of all data in this structure.*/
}V9381_calib_registers_t;
__packed typedef struct {
  uint64_t rms_curr[MTR_NUM_OF_CH];
  uint64_t rms_vol[MTR_NUM_OF_CH];
  uint64_t ph_angle;
  uint64_t pf[MTR_NUM_OF_CH];
  uint64_t line_freq;
  uint64_t act_pow[MTR_NUM_OF_CH];
  uint64_t react_pow[MTR_NUM_OF_CH];
  uint64_t act_energy;
}mtr_readings_t;


 typedef struct 
{ uint16_t addr;
  uint32_t value;
}V9381_reg_values_t;


typedef enum {
  RESET_NONE_MODE           = (uint8_t) 0,
  RESET_DATA_CRPT           = (uint8_t) 1<<0,
  RESET_UART_PROBLEM        = (uint8_t) 1<<1,
  RESET_SWITCH_LINE_NETURAL = (uint8_t) 1<<2,
  RESET_WRONG_FREQUENCY     = (uint8_t) 1<<3,
  RESET_CALIBRATE           = (uint8_t) 1<<4,
  RESET_POWER_UP            = (uint8_t) 1<<5,
  RESET_MAIN_INIT           = (uint8_t) 1<<6,
  RESET_NEW_ENG_FAIL        = (uint8_t) 1<<7,
}V9381_RESET_MODULE_REASON_enumt;

#define LINEPERIOD 1
extern uint8_t V9381_init_numbers ;
extern uint8_t flag_get_volt ; 
extern  mtr_readings_t mtr_inst_readings;
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/
void V9381_test_uart();
void V9381_task(void);   
void cal_handler_task(void);
void cal_cmd(uint8_t arg1);
uint8_t cal_state(void);
void V9381_inst_read(uint8_t phase);
uint8_t Init_LoadcalibParam( uint32_t* Cksum_ptr);
uint8_t V9381_Init(uint8_t init_reason);
uint8_t V9381_chkFailure(void);
void V9381_read_initial_PulseCount(void);
void V9381_calib(void);
double V9381_calc_act_pow_error(uint64_t avg_reading);
int64_t V9381_Pow_Gain_Calib(double pow_error, uint64_t avg_reading);
int32_t V9381_RMS_Gain_Calib(uint32_t avg_reading, float std_mtr_value);
void V9381_chk_active_phases(void);
uint8_t V9381_get_ins_reverse(void);
uint64_t V9381_get_ActEnergy_Accum( uint8_t* Negative_flag_ptr);
uint64_t V9381_get_ActEnergy_Accum_ph1( uint8_t* Negative_flag_ptr);
void V9381_clr_ins_reverse(uint8_t arg1) ;
uint64_t V9381_instant_active_power_read_microW();
void V9381_check_line_or_Netural(uint32_t Ia , uint32_t Im);
uint8 V9381_get_channel_I ();
void V9381_rx_Isr(uint8_t rx_byte);
void V9381_read_inst_task();
void V9381_Energy_Flag();
void V9381_Volt_Flag();
uint8_t V9381_Get_Negative_Flag();
uint32_t V9381_Actpulse_counter_read_ph1();
uint32_t V9381_Reactpulse_counter_read_ph1();
uint64_t V9381_get_ReactEnergy_Accum_ph1();
//void V9381_Reactive_Power(void);

//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_get_Reset_Flag
// Description      : This function is used to get the reason of reseting the module
// Arguments        : NONE
// Return           : one of this enum V9381_RESET_MODULE_REASON_enumt
// Output           : NONE
//-----------------------------------------------------------------------------
// Author           : Ibrahem Mostafa
// Date             : 30/03/2020
//=============================================================================
uint8_t V9381_get_Reset_Reason (void);
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_get_ReactEnergy_Accum
// Description      : This function is used to get the reactive energy accmulation for each phase
// Arguments        : phase : the phase(0 for phase A, 1 for phase B, 2 for phase C)
// Return           : the reactive energy accmulation for each phase
// Output           : NONE
//-----------------------------------------------------------------------------  
uint64_t V9381_get_ReactEnergy_Accum();
uint32_t V9381_Reactpulse_counter_read();


uint64_t V9381_get_ApperantEnergy_Accum();
uint32_t V9381_Apperantpulse_counter_read();

//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_load_Param
// Description      : This function is used to load from eeprom the calibration parameters
// Arguments        : None.
// Return           : true if load from eeprom ok.
// Output           : NONE
//===========================================================================
uint8_t V9381_load_Param ();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_get_rms_voltage
// Description      : This function is used to get the instantaneous total rms voltage value for each phase
// Arguments      : arg1 : the phase (0 for ph A, 1 for ph B, 2 for ph C)
// Return           : The instantaneous rms voltage value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9381_get_rms_voltage();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_Set_Default_Param
// Description      : This function is used to set and save the default of the calibration parameters
// Arguments        : None.
// Return           : NONE
// Output           : NONE
//===========================================================================
void V9381_Set_Default_Param();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_get_rms_current
// Description      : This function is used to get the instantaneous total rms current value in Micro Amperes
// Arguments      : arg1 : the phase (0 for ph A, 1 for ph B, 2 for ph C)
// Return           : The instantaneous total rms current value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9381_get_rms_current(uint8_t arg1);
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_instant_total_irms_read
// Description      : This function is used to read the instantaneous total rms current value for each phase
// Arguments      : LocalPhase: (0 for ph A, 1 for ph B)
// Return           : The instantaneous total rms current value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9381_instant_total_irms_read(uint8_t LocalPhase) ;

// FUNCTION
//=============================================================================
// Name             : V9381_instant_total_vrms_read
// Description      : This function is used to read the instantaneous total rms voltage value
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C)..
// Return           :the instantaneous total rms voltage value
// Output           : NONE
//=============================================================================
uint32_t V9381_instant_total_vrms_read() ;

//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9381_instant_tot_active_power_read
// Description      : This function is used to get the total instantaneous active power  
// Arguments         : NONE
// Return           :the total instantaneous active power
// Output           : NONE
//-----------------------------------------------------------------------------

uint64_t V9381_instant_active_power_read_microW();

uint64_t V9381_instant_reactive_power_read_milliVAR(void);
//=============================================================================
// Name             : V9381_line_freq_read
// Description      : This function is used to get the line frquency for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the line frequency
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9381_line_freq_read() ;
//=============================================================================
// Name             : V9381_instant_total_PF_read
// Description      : This function is used to get the instantaneous power factor for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the instantaneous power factor
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9381_instant_total_PF_read(uint64_t power , uint32_t volt , uint32_t current) ;
//=============================================================================
// Name             : V9381_Actpulse_counter_read\
// Description      : This function is used to get the pulse counter for active power for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the pulse counter for active power
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9381_Actpulse_counter_read();
//=============================================================================
// Name             : V9381_INT_flagReg_read
// Description      : This function is used to read the interrupt flag register (V9381_REG_IRQFLAG)
// Arguments         : NONE
// Return           :the interrupt flag register (V9381_REG_IRQFLAG)
// Output           : NONE
//------------------------------------------------------------------------
uint8_t V9381_get_reg ( uint16_t Address, uint32_t* reg_value_ptr);
uint8_t V9381_set_reg ( uint16_t Address, uint32_t reg_value); 


uint8_t V9381_INT_flagReg_read(uint32_t * localFlagReg);

void V9381_Switch_line_or_Netural(uint8_t is_Switch_Line);
uint8_t V9381_testing ();
void SelfTest_SetV9381Param(void);

uint32_t V9381_instant_apparent_power_read_milliVA(void); 
uint64_t V9381_instant_apparent_power_read_microVA(void);

 
uint32_t V9381_instant_active_power_read_milliW(void);

//extern 

extern uint8_t down_volt_flag;/* flag to tell mtr.c the volt was down to harry going to ctrl to save */
extern uint8_t V9381_data_crpt_flag ;
extern uint8_t first_init_pulses;
extern V9381_calib_registers_t Calib_Reg_Values;


#endif /* V9381*/






/*------------------------------------------------------------------------------------
  from DINRail_Metering.h
------------------------------------------------------------------------------------*/




/* #include "V9340.h" -- inlined in this file */
/* #include "V9381.h" -- inlined in this file */




//////////////////////////////////////////////V9381///////////////////////////////////////////////////////////////////////////








//////////////////////////////////////////////V9340///////////////////////////////////////////////////////////////////////////









extern uint8_t DL_Netural_flage;

uint8_t DINRail_Init(uint8_t reason_init);
uint32_t DINRail_get_rms_voltage(void);                           
uint32_t DINRail_get_rms_current(uint8_t channel);                
uint64_t DINRail_instant_active_power_read_MicroW(void);          
uint32_t DINRail_instant_active_power_read_milliW(void);          
uint32_t DINRail_instant_reactive_power_read_milliVAR(void);      
uint64_t DINRail_get_ActEnergy_Accum(uint8_t *Negative_flag_ptr); 
uint64_t DINRail_get_ReactEnergy_Accum(void);
uint64_t DINRail_get_AppEnergy_Accum(void);
uint32_t DINRail_instant_tot_apparent_power_read_milliVA(void);
uint32_t DINRail_instant_total_PF_read(uint64_t power,uint32_t volt,uint32_t current);                     
uint32_t DINRail_line_freq_read();                             
void DINRail_calib(void); 
void DINRail_cal_handler_task(void);
void DINRail_Switch_Line_or_Neutral(uint8_t LineOrNeutral);
void DINRail_task(void);
uint8_t DINRail_cal_state(void);                                         
void DINRail_clr_ins_reverse(uint8_t arg1);                                
uint8_t DINRail_get_ins_reverse(void);                                   

uint8_t DINRail_get_channel_I(void);






/*------------------------------------------------------------------------------------
  from Delta_Algorithm.h
------------------------------------------------------------------------------------*/

#include "config.h"
#include "FLASH_Addresses.h"
extern uint32_t __vector_table;
//#define orgin_in_ext_flash 1
#undef orgin_in_ext_flash 
#if 0

#else
#ifdef orgin_in_ext_flash
#else
#endif
#endif

extern uint32_t SizeofCompressionFile;
extern uint8_t prev_crc,nxt_crc;
extern uint8_t delta_run;
extern uint8_t New_BootLoaderFlage;
extern uint8_t LcdBootLoader;
extern uint16_t BootLoaderFirmwareLocation;
extern uint8_t file_crc;
extern uint32_t file_length;
extern uint32_t EXT_FlashDestination;
/************************Delta****************************/
uint8_t Image_Gen_task(void );
void FireDelta(void);
void timeout_bootloader_sec(void);
void cal_crc_image(uint8_t* ptr_buf,uint16_t length );
void UpdateBootLoaderImage_task(void);
void FireBootLoaderImageFlage(void);
uint8_t Is_UpdateCode_run(void);
void ResetUpdateBootloader(void);
void reset_delta_state(void);
void ansi_cal_crc_image_last_packet(uint8_t* ptr_buf,uint16_t length );


/*------------------------------------------------------------------------------------
  from Dot_Matrix_Commands.h
------------------------------------------------------------------------------------*/
/**
 * @author Ahmed Elhossiny
 * @create date 2022-11-1 
 * @desc REGISTERS.h
 */
#include"stdint.h"


            /************************************common Commands**************************************/


//for set EV you must set the volume and put the level
//used for contrast
//also put electronic volume level from 0 -> 255 for LCD_ST7525
//also put electronic volume level from 0 -> 63 for LCD_ST7567

            /***************************************LCD ST7567****************************************/


//#define READ_STATUS
#ifdef READ_STATUS
#endif
//write data from 0x00 -> 0xFF
//read data from 0x00 -> 0xFF





/*****  data for PWR contol    *****/
// Note: we have three parameters here : voltage regulator & voltage follower & Voltage booster

//for LCD contrast


//you must set booster in command after that choose the level



  /*************************************** LCD ST7525 ****************************************/


// 1-used AC0=1 ,Column address will stop increasing by 1 while reaching each boundary.
// 2-used AC2=1 ,Page address decrease by 1 (PA -1, upward).












/*------------------------------------------------------------------------------------
  from Dot_Matrix_MCAL.h
------------------------------------------------------------------------------------*/
/*
 * @author Ahmed Elhossiny
 * @create date 2022-11-1 
 * @desc LCD_7567.h
 */


/* #include "Dot_Matrix_Commands.h" -- inlined in this file */
#include "config.h"

#if (MicroController == Micro_V94XX)
#include "V94XX.h"
#else
#include "v85xx.h"
#endif

/*choose the LCD 1- LCD_7567 OR 2- LCD_7525 */
#define LCD_ST7525 


/************************Functions Prototypes For Initialiazion*************************/
void Dot_LCD_Init(uint8_t low_pwr_flag);
void Disable_Dot_Matrix_LCD_PWR(void);
void Enable_LCD_PWR(void);
void ENTERING_PWR_SAVE(void);
void ENTERING_PWR_SAVE_WITH_DIS_VCC(void);
void EXIT_PWR_SAVE(void);
void SET_ALL_PIX(void);

/************************Functions Prototypes for control*************************/
void goto_row(uint8_t y);
void goto_col(uint8_t x);
void goto_xy(uint8_t x, uint8_t y);
void putdata(uint8_t x, uint8_t y, const uint8_t *arr);
void Dot_Clear_Screen(void);
void write_com(uint16_t para);
void cursor(uint8_t x, uint8_t y ,uint8_t val);
void write_data(uint16_t para);
void Dot_LCD_OFF(void);
void Dot_LCD_ON(void);
void Dot_Set_Screen(void);

/*------------------------------------------------------------------------------------
  from Dot_Matrix_Interfacing.h
------------------------------------------------------------------------------------*/
/**
 * @author Ahmed Elhossiny
 * @create date 2022-12-1
 * @desc LCD_7567_INTERFACING.h
 */

#include "config.h"
#include "Flash.h"
/* #include "Dot_Matrix_MCAL.h" -- inlined in this file */
#include "string.h"
/* #include "basic_srv.h" -- inlined in this file */
#include "SPI_Interface.h"
#include "SystemTick_Interface.h"
#include "ctrl.h"

/******************** MACROS FOR INTERNAL FLASH ******************/
#define NUM_SECTORS_DOTMATRIX_INTERNAL  199   //this number of sector for one language

/******************** MACROS FOR INTERNAL FLASH ******************/

#define CHECK_BITT(Reg,bit)         ((Reg>>bit)&1)
#define DOT_MATRIX_FRAME_SIZE 261
#define DIFFERENCE_OFFSET_BETWEEN_AR_EN 25
#define DIFFERENCE_OFFSET_BETWEEN_AR_FR 50
#define DOT_MATRIX_LANGUAGE1_OFFSET                (0)
#define DOT_MATRIX_LANGUAGE2_OFFSET                (DIFFERENCE_OFFSET_BETWEEN_AR_EN * 4096)   
#define DOT_MATRIX_LANGUAGE3_OFFSET                (DIFFERENCE_OFFSET_BETWEEN_AR_FR * 4096)   
#define DOT_MATRIX_CTRL_CHANGE_LANG_FLAG           ctrl_sys_new_var.ctrl_change_dot_matrix_language_flag
#define DOT_MATRIX_SAVE_CRITICAL_DATA()             ctrl_event(CTRL_SAVE_CRITICAL_DATA)





/*********************************LCD MAPPING PAGES*********************************/
typedef enum
{
  V_NUM0 = 1,
  V_NUM1,
  V_NUM2,
  V_NUM3,
  V_NUM4,
  V_NUM5,
  V_NUM6,
  V_NUM7,
  V_NUM8,
  V_NUM9,
  V_DOT,
  I_DOT,
  V_DASH,
  V_space,
  ddot,
} EN_DOT_MATRIX_VALUE_NUMBERS;

typedef enum
{
  WIFI = 16,
  OPTICAL,
  GPRS_SIGNAL,
  BLUETOOTH,
  RS_458,
  MESSAGE,
  BATT,
  L1,
  L2,
  L3,
  UP_ARROW,
  DOWN_ARROW,
  RIGHT_ARROW,
  LEFT_ARROW,
  SMILE_FACE,
  SAD_FACE,
} EN_DOT_MATRIX_SYMBOLS1;

typedef enum
{
  MAGNET = 32,
  REVERSE,
  OPEN_RELAY,
  GROUND,
  HANDCHECK,
  RF,
  LOAD,
  CARD, // elkart
  IN,   // est5dm
  ALARM,
#ifdef ANSI_STACK
  PRUEBA =46,
#endif  
  LAST_SYMBOL,
} EN_DOT_MATRIX_SYMBOLS2;

typedef enum
{ 
  GPRS_WORD = 48,
  ERROR_RELAY,
  _8ER,
  MA2BOL,
  reserved1,
  reserved2,
  _7ML, //a2sy 7ml
  ZA2D,
  ELRASEED,
  ELBA2Y,
  GNEH,
  YOOM,
  EL7ALY,
  ELSABEQ,
  EGMALY,
  MOTAWSET,
} EN_DOT_MATRIX_WORDS1;

typedef enum
{
  MO3AML_ELQUDRA = 64,
  ESTHLAK,
  ELSHARE7A,
  CODE,
  DUON,
  ELTARE5,
  TLA3OB,
  ELWA2T,
  MO8L2,
  MAFTO7,
  STS_CODE,
  ELMOSHTAREK,
  EL3DAD,
  KOLI,
  ZERO_DASH,
  INF,
} EN_DOT_MATRIX_WORDS2;

typedef enum
{
  KILO = 80,
  KWH,
  MEGA,
  GIGA,
  VOLT,
  KVA,
  MVA,
  GVA,
  HZ,
  AMP,
  KW,
  KVAR,
  KVARH,
} EN_DOT_MATRIX_UNITS;

typedef enum
{
  a = 96,
  b,
  c,
  d,
  e,
  f,
  g,
  h,
  i,
  j,
  k,
  l,
  m,
  n,
  o,
  p,
} EN_DOT_MATRIX_ALPHA1;
typedef enum
{
  q = 112,
  r,
  s,
  t,
  u,
  v,
  w,
  x,
  y,
  z,
  
} EN_DOT_MATRIX_ALPHA2;

typedef enum
{
  I_NUM0 = 129,
  I_NUM1,
  I_NUM2,
  I_NUM3,
  I_NUM4,
  I_NUM5,
  I_NUM6,
  I_NUM7,
  I_NUM8,
  I_NUM9,
  I_DASH,
  I_SPACE,
} EN_DOT_MATRIX_INDEX_NUMBERS;
typedef enum
{
  T_OPEN = 144,
  C_OPEN,
  LOW_BATT,
  NO_BATT,
  AM,
  PM,
  ACTIVITY,
  IP,
  MANUAL,
  AUTOMATIC,
  PT,
  CT,
  APN_SYMBOL,
  PHSEQ,
  PH1,
} EN_DOT_MATRIX_WORDS3;
typedef enum
{
  PH2 = 160,
  PH3 ,
  SCREEN_BATT , //inside screen 26 
  METER_TYPE_1S,
  METER_TYPE_12S,
  METER_TYPE_16S,

  
} EN_DOT_MATRIX_WORDS4;

enum
{
  WRITE_SCREEN = 0,
  WRITE_SYMBOL,
};

enum
{
  DATA_READ = 0,
  DATA_WRITE,
};
enum
{
  SYM_OR_SCR=0,
  ERASE_ALL_SECT,
  ONE_OR_TWO_SCR,
  CHANGE_SYM_OR_SCR,
};

enum 
{
  READING=0,
  WRITING,
};

enum 
{
  LANGUAGE1=0,
  LANGUAGE2,
  LANGUAGE3,
};

/*********************************** LCD SCREENS Num of Array Elements  **************************************/
#define Test_Screen 1
#define Remaining 8
#define Total_consumption 2
#define Active_Energy 4
#define Reverse_Energy 2
#define Earth_Energy_Fault_Energy 1
#define Charges 6
#define Date_Time 2
#define Alarm_Limit 12
#define Voltage_Instantaneous 3
#define Current_Instantaneous 6
#define DM_SCR_FREQUENCY 1  /* renamed: "Frequency" shadowed CLK_Struct.PLLL.Frequency */
#define Power_Factor_Instantaneous 6
#define Active_Power_Instantaneous 4
#define ID 7
#define Tamper_Count 7
#define Tampering_indication_in_the_register 5    //last 1
#define MDA_Billing_Period 6
#define MDA_Billing_Period_for_tariff 1
#define MD_KW_Billing_Period 6
#define MD_KW_Billing_Period_for_tariff 1
#define MDA_meter_life 11
#define MD_KW_meter_life 11
#define MD_reset_count 1
#define Level 3
#define Meter_reading 2
#define Pulse_indication_test 1
#define Load_Cost_per_Hours 2
#define Tariff_prices 1
#define Tariff_steps 1
#define Taxes 3
#define MD_KVA_Billing_Period 6
#define MD_KVA_Billing_Period_for_tariff 1
#define MD_KVA_meter_life 4
#define Tariff_Active_Energy_KWh 7
#define Power_Interrupt 1
#define Total_Reactive_energy 5
#define Total_apparent_energy 3
#define Reactive_power 4
#define Apparent_power 4
#define Step_remaining 1
#define Billing_period_consumption 1
#define Billing_period_bill 1
#define Friendly_consumption 1
#define start_of_time_and_date_tamper_event 1        // using STS_test0_screens instead of that screens
#define stop_of_time_and_date_tamper_event 1          // not using so we can remove it , last 36
#define Test_Accuracy 2
#define Last_12Consumption_In_KWH_And_Money 1       //last 24 but we make it as a special screen ,all 24 screens as one screen
#define Total_Debit 3
#define Last_12Customer_Services_Taxes 12
#define State_of_relay 1
#define Current_Level_KWH_per_Current_Step_Tariff 1
#define Sum_of_charge_interfaces 1
#define Tamper_counter 9
#define Error_of_contactor 1
#define Date_of_Last_charge 1
#define Department_ID 1
#define Overload_Counter 1
#define Battery_Screen 1
#define Last_terminal_cover_tamper 2
#define Selected_screens 19
#define Reserved1 1 
#define Active_4Q_consumption 4
#define Reactive_4Q_consumption 4
#define Indirect_param 3
#define Reserved2 1
#define BPH_reactive_consumption_in_KVAR_for_each_Billing_Record 1
#define BPH_MDW_consumption_in_KW_for_each_Billing 1
#define BPH_Power_factor_for_each_Billing_Record 1
#define Total_Export_Energy 1
#define Total_import_and_export_Energy 1
#define Total_import_Energy 1
#define sts_code 1
#define disp_message 1
#define STS_TEST0_Screens 9
#define GPRS_Screens 7
#define Reserved3 1
#define TOKENS_SCREENS 10
/*********************************** LCD SCREENS SUB INDEX **************************************/

/*********************** 0-Test Screen **********************/

/************************* 1-Remaining ********************/

/************************* 2-Total consumption ********************/

/************************* 3-Active Energy ********************/

/************************* 4-Reverse Energy ********************/

/*************** 5-Earth Energy(Fault Energy) ******************/

/*************************** 6-Charges *************************/

/************************ 7-Date / Time *************************/

/************************ 8-Alarm Limit *************************/

/****************** 9-Voltage Instantaneous *******************/

/****************** 10-Current Instantaneous *******************/

/****************** 11-Frequency *******************/

/****************** 12-Power Factor Instantaneous *******************/

/*************************** 13-Active Power ************************/

/**************************** 14-ID  ******************************/

/************************* 15-Tamper Count  **********************/

/***********  16-Tampering indication in the register  **********/

/********************  17-MDA Billing Period  ******************/


/**************  18-MDA Billing Period for tariff  ***************/

/********************  19-MD KW Billing Period  ******************/

/*************  20-MD KW Billing Period for tariff  **************/

/**********************  21-MDA meter life  **********************/

/********************  22-MDA KW meter life  ********************/

/********************  23-MD reset count  ********************/

/************************ 24-Level  **************************/

/********************  25-Meter Reading  ********************/

/********************  26-Pulse indication test  ********************/

/********************  27-Load Cost per Hours  ********************/

/********************  28-Tariff prices  ********************/

/********************  29-Tariff steps  ********************/

/*************************  30-Taxes  ***************************/

/*****************  31-MD KVA Billing Period  *******************/

/************  32-MD KVA Billing Period for tariff  *************/

/*****************  33-MD KVA meter life  *******************/

/************** 34-Tariff Active Energy KWh  *************/

/******************  35-Power Interrupt  ******************/

/**************** 36-Total Reactive energy ***************/

/****************  37-Total apparent energy ***************/

/******************  38-Reactive power ********************/

/****************** 39-Apparent power ********************/

/****************** 40-Step remaining ********************/

/************ 41-Billing period consumption **************/

/*************** 42-Billing period bill ******************/

/*************** 43-Friendly consumption *****************/

/************* 44-start of time and date tamper event ************/
/********* Date & Time of Terminal openStart ********/
/********* Date & Time of Cover open Start ********/
/********* Date & Time of Reverse Ch A Start ********/
/********* Date & Time of Fault Energy Start ********/
/********* Date & Time of Overload Start ********/
/********* Date & Time of Reverse Ch B Start ********/

/************* 45-stop of time and date tamper event************/
/********* Date & Time of Terminal open stop ********/
/********* Date & Time of Cover open stop ********/
/********* Date & Time of Reverse Ch A stop ********/
/********* Date & Time of Fault Energy stop ********/
/********* Date & Time of Overload stop ********/
/********* Date & Time of Reverse Ch B stop ********/

/****************** 46-Test Accuracy ********************/

/******* 47-Last 12 Consumption In KWH And Money ********/

/****************** 48-Total Debit ********************/

/******** 49-Last 12 Customer Services Taxes **********/

/****************** 50-State of relay ********************/

/****************** 51-Current Level KWH per Current Step Tariff ********************/

/****************** 52-Sum of charge interfaces ********************/

/****************** 53-Tamper counter ********************/

/****************** 54-Error of contactor ********************/

/****************** 55-Date of Last charge ********************/

/****************** 56-department ID ********************/

/****************** 57-Overload Counter ********************/

/****************** 58-Battery Screen ********************/

/*************  59-Last terminal cover tamper *************/

/****************** 60-Reserved1 ********************/

/****************** 61-Selected Screen ********************/

/****************** 62-Active 4Q consumption ********************/

/****************** 63-Reactive 4Q consumption ********************/

/****************** 64-Indirect parameter ********************/

/****************** 65-Reserved2 ********************/

/******** 66-BPH reactive consumption in KVAR for each Billing Record **********/

/******** 67-BPH reactive consumption in KW for each Billing Record **********/

/******** 68-BPH Power factor in KVAR for each Billing Record **********/

/****************** 69-Total Export Energy ********************/

/****************** 70-Total import and export Energy ********************/

/****************** 71-Total import Energy ********************/

/*********************************** LCD SCREENS Index **************************************/
#define INDEX_Test_Screen 0

/************functions prototyping************/
void bitmap(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t* input);
void Draw_All_Screen(void);
void Dot_Matrix_Draw_Screen(uint8_t screen_index, uint8_t screen_sub_index, uint64_t value, uint8_t *index_array);
void Draw_STS_Screen(uint8_t screen_index, uint8_t screen_sub_index, uint8_t *charging_ptr);
uint32_t Get_Screen_Address(uint8_t index, uint8_t sub_index);
uint32_t Get_Sym_Sector(uint8_t num);
uint16_t Get_Symbol_Size_R_W(uint32_t address, uint8_t page, uint8_t *data, uint8_t mode);
uint16_t Writing_On_Flash(uint8_t *data_ptr);
void Dot_matrix_handling_disp_message(uint8_t *main,uint8_t *index);
uint32_t get_sector_offset(uint8_t offset_sector);
uint32_t Get_Scr_Sector(uint32_t add);
uint8_t Dot_Matrix_set_indepndant_icons(uint8_t symbol_id, uint32_t check_sym_flag);
uint8_t Dot_Matrix_special_screen_words(uint8_t symbol_id, uint32_t check_words);
void Dot_Matrix_handling_decimal_point_value(void);
void erase_flash_sectors_from_to(uint16_t start,uint16_t end);
uint16_t Writing_On_Internal_Flash(uint8_t *data_arr);
void DotMatrixSaveInternalFlash(void);
uint32_t Get_Sym_Sector_internal(uint8_t num);
void erase_dot_matrix_language_section(uint8_t lang_num);
void calc_DM_template_integraity(uint8_t lang_num, uint8_t *data_ptr);

/*------------------------------------------------------------------------------------
  from Two_Phase_Interface.h
------------------------------------------------------------------------------------*/
#include "config.h"

#ifdef MTR_TWO_PH


/* #include "V9340.h" -- inlined in this file */
/* #include "V9381.h" -- inlined in this file */

extern uint8_t Two_Phase_data_crpt_flag ;
extern uint8_t Two_Phase_comm_err_flag ;


//////////////////////////////////////////////V9381///////////////////////////////////////////////////////////////////////////








//////////////////////////////////////////////V9340///////////////////////////////////////////////////////////////////////////



















typedef enum
{
 Two_Phase_INIT_ROM = 0,    /*!< Means run initialization with loading from ROM.*/
  Two_Phase_INIT_RAM = 1    /*!< Means run initialization without loading from ROM.*/
}  Two_Phase_init_t;


__packed typedef struct {
  //int32_t calib_rms_curr[MTR_NUM_OF_CH];
  //int32_t calib_rms_vol[MTR_NUM_OF_CH];
  //int32_t calib_act_pow[MTR_NUM_OF_CH];
#ifdef NEUTRAL_FEATURE
    uint32_t calib_rms_curr[MTR_NUM_OF_CH+1];//+1 fo nuteral
#else
    uint32_t calib_rms_curr[MTR_NUM_OF_CH];
#endif
  uint32_t calib_rms_vol[MTR_NUM_OF_CH];
  uint32_t calib_act_pow[MTR_NUM_OF_CH];
  //uint64_t calib_react_pow[MTR_NUM_OF_CH];
  //int64_t calib_ph_compens[MTR_NUM_OF_CH];
  uint8_t   cal_flag;          /*!< Contains CRC of all data in this structure.*/
}Two_Phase_mtr_calib_t;




void Two_Phase_inst_read(uint8_t phase);
void Two_Phase_ade7758_earth_tmp(void);
void Two_Phase_clr_ins_reverse(uint8_t );
uint8_t Two_Phase_get_ins_reverse(void);
uint8_t Two_Phase_chk_reverse_ch(void);
void Two_Phase_Reset_SPI(void);
uint8_t Two_Phase_CheckSum(uint32_t dat,uint8_t cmd,uint8_t type);
uint32_t Two_Phase_ReceiveFrame(uint8_t cmd);
uint8_t Two_Phase_Read_Data(uint16_t addr,uint8_t DataLen);
void Two_Phase_Send_Frame(uint32_t CmdData);
void Two_Phase_Write_Data(uint16_t addr,uint32_t data);
void Two_Phase_set_reg(uint16_t V9203_REG_ADDR, uint32_t reg_value);
uint32_t Two_Phase_get_reg(uint16_t V9203_REG_ADDR) ;
uint32_t Two_Phase_Init_calibParam(Two_Phase_init_t init_type);
void Two_Phase_Init(uint8_t reason_init);
uint32_t Two_Phase_get_rms_voltage(uint8_t arg1);
uint32_t Two_Phase_get_rms_current(uint8_t arg1);
uint32_t Two_Phase_instant_total_irms_read(uint8_t LocalPhase);
uint32_t Two_Phase_avg_total_irms_read(uint8_t LocalPhase);
uint32_t Two_Phase_avg_fundamental_irms_read(uint8_t LocalPhase);
uint32_t Two_Phase_instant_total_vrms_read(uint8_t LocalPhase) ;
uint32_t Two_Phase_avg_total_vrms_read(uint8_t LocalPhase);
uint32_t Two_Phase_avg_fundamental_vrms_read(uint8_t LocalPhase) ;
uint32_t Two_Phase_avg_active_power_read(uint8_t LocalPhase);
//uint64_t V9203_instant_active_power_read(uint8_t LocalPhase) ;
uint32_t Two_Phase_avg_reactive_power_read(uint8_t LocalPhase) ;
//uint64_t V9203_instant_reactive_power_read(uint8_t LocalPhase);
uint32_t Two_Phase_avg_apparent_power_read(uint8_t LocalPhase) ;
//uint64_t V9203_instant_apparent_power_read(uint8_t LocalPhase);
uint32_t Two_Phase_line_freq_read(uint8_t LocalPhase);
uint32_t Two_Phase_vol_phase_angle_read(uint8_t LocalPhase);
uint32_t Two_Phase_avg_total_PF_read(uint8_t LocalPhase);
uint32_t Two_Phase_instant_total_PF_read(uint8_t LocalPhase);


uint64_t Two_Phase_total_apparent_Energy_read(uint8_t LocalPhase);
uint64_t Two_Phase_total_active_Energy_read_PH(uint8_t LocalPhase);
uint64_t Two_Phase_total_Reactive_Energy_read(uint8_t LocalPhase);
uint32_t Two_Phase_INT_flagReg_read(void);
uint32_t Two_Phase_Actpulse_counter_read(uint8_t LocalPhase);
uint32_t Two_Phase_Reactpulse_counter_read(uint8_t LocalPhase);
uint32_t Two_Phase_AppPulse_counter_read(uint8_t LocalPhase);
uint32_t Two_Phase_tot_Actpulse_counter_read(void);
uint64_t Two_Phase_total_active_Energy_read(void);
void Two_Phase_CF_read(uint32_t* ptr_pulseCounter);
uint64_t Two_Phase_get_ActEnergy_Accum(uint8_t phase);
uint64_t Two_Phase_get_ReactEnergy_Accum(uint8_t phase);
uint64_t Two_Phase_get_AppEnergy_Accum(uint8_t phase);
void Two_Phase_read_initial_PulseCount(void);
uint32_t Two_Phase_tot_AppPulse_counter_read(void);
uint32_t Two_Phase_tot_Reactpulse_counter_read(void);
uint64_t Two_Phase_get_tot_ActEnergy_Accum(void);
uint64_t Two_Phase_get_tot_ReactEnergy_Accum(void);
uint64_t Two_Phase_get_tot_AppEnergy_Accum(void);
void Two_Phase_calib(void);

uint32_t Two_Phase_instant_tot_active_power_read_milliW(void);
uint32_t Two_Phase_instant_tot_reactive_power_read_milliVAR(void);
uint32_t Two_Phase_instant_tot_apparent_power_read_milliVA(void );
uint32_t Two_Phase_instant_active_power_read_milliW(uint8_t LocalPhase) ;
uint64_t Two_Phase_instant_active_power_read_microW(uint8_t LocalPhase);
uint32_t Two_Phase_instant_reactive_power_read_milliVAR(uint8_t LocalPhase);
uint64_t Two_Phase_instant_reactive_power_read_microVAR(uint8_t LocalPhase);
uint32_t Two_Phase_instant_apparent_power_read_milliVA(uint8_t LocalPhase);
uint64_t Two_Phase_instant_apparent_power_read_microVA(uint8_t LocalPhase);
double Two_Phase_calc_act_pow_error(uint64_t avg_reading);
int32_t Two_Phase_RMS_Gain_Calib(uint32_t avg_reading, float std_mtr_value);
//int64_t Act_phase_compens(double pow_error);
int64_t Two_Phase_Pow_Gain_Calib(double pow_error, uint64_t avg_reading);
void Two_Phase_Energy_task(void);
//void V9203_chk_active_phases(void);
void Two_Phase_Pow_Gain_Adjust(uint8_t ind, uint32_t pow_value);
void Two_Phase_RMS_CURR_Adjust(uint8_t ind, uint32_t rms_curr);
void Two_Phase_RMS_VOL_Adjust(uint8_t ind, uint32_t rms_vol);
void Two_Phase_task(void);
void Two_Phase_ph_seq_tmp(void);
//uint8_t V9203_get_active_phases(void);
uint8_t Two_Phase_self_test(void);

uint8_t Two_Phase_cal_state(void);
void Two_Phase_cal_cmd(uint8_t arg1);
void Two_Phase_cal_handler_task(void);
void Two_Phase_manual_cal_handler_task(void);
void Two_Phase_new_sec(void);
void Two_Phase_SendFilterFrame(uint8_t filter_type);
void Two_Phase_IRQ_isr(void);
uint8_t Two_Phase_chkFailure(void);
void Two_Phase_IRQ_config(void);

uint32_t Two_Phase_curr_phase_angle_read(uint8_t LocalPhase);
void Two_Phase_calib_init(void);
/*!
* @brief used to clear previous Active phases.
*
* @par Description:
* This function is responsible to clear previous active phases.
*
* @param[out] void
*
*/
void Two_Phase_clr_prv_no_active_phases(void);
/* @brief This function used to read active phases status.
*
* @par Description:
* Used to get the status for active phases.
*
* @return
* - uint8_t active phases flags.
*/
uint8_t Two_Phase_get_active_phases(void);
/*!
* @brief used to check the Active phases.
*
* @par Description:
* This function is responsible to check active phases and select the zero crossing and frequancy source .
*
* @param[out] void
*
*/
void Two_Phase_chk_active_phases(void);

void Two_Phase_SelfTest_Set_Two_Phase_Param(void);
#ifdef NEUTRAL_FEATURE
uint32_t Two_Phase_Check_Neutral_Mismatch(void);
#endif




#endif     // TWO_PHASE_INTERFACE_H





/*------------------------------------------------------------------------------------
  from V9203.h
------------------------------------------------------------------------------------*/

#include "config.h"
/* #include "meter_init.h" -- inlined in this file */
#ifdef V9203_ENABLE
#define LINEPERIOD                2
#undef CAL_50A
#undef ANSI_CLIBRATION_POINT
#define GAIN_CALIB_CONST  ((double)2147483648)
#ifndef ANSI_CLIBRATION_POINT
#else
#endif
#ifdef MTR_INDIRECT
#define STD_MTR_CURR      ((float)1000000) /*  A in micro ampere*/
#else
#define STD_MTR_CURR      ((float)5000000) /*  A in micro ampere*/
#endif






#ifdef MTR_THREE_PH
#else
#endif
/* IRQ BITS OF V9203*/
#define CF0      ((uint32_t)0x00000080) /*To indicate a pulse output interrupt on pin CF0 occurs.*/
#define CF1      ((uint32_t)0x00000100) /*To indicate a pulse output interrupt on pin CF1 occurs.*/
#define CF2      ((uint32_t)0x00000200) /*To indicate a pulse output interrupt on pin CF2 occurs.*/
#define CF3      ((uint32_t)0x00000400) /*To indicate a pulse output interrupt on pinCF3 occurs.*/
//#ifdef V9203_PH_SEQ_TMPR
//#endif
#define PD       ((uint32_t)0x00010000) /*To indicate a power-down interrupt occurs.*/


#if defined (EM330_BOARD)
#define V9203_SPI_ReceiveData(data,len)       SPI2_rx_frame(data,len)
#define V9203_SPI_SendData(data,len)          S_SPI2_TX_FRAME(data,len)

//#define V9203_SPI_ReceiveData(data,len)                   bSPI_InfSyncRXBuf(data,len)
//#define V9203_SPI_SendData(data,len)                      bSPI_InfSyncTxBuf(data,len)

#else
#define V9203_SPI_ReceiveData(data,len)       SPI1_rx_frame(data,len)
#define V9203_SPI_SendData(data,len)          S_SPI1_TX_FRAME(data,len)
#endif

/*Met ctrl register 0 */


/* mtr CTRL reg 1*/












/* MTR CTRL REG 2*/































/*Analog ctrl reg bits*/











/* analog ctrl reg 2*/








/*not used configuration for ANCTL2 register*/


/* ANALOG CTRL REG 3*/











//#define V9203_IRQEN0_MASK         (((uint32_t)(1<<0)) | ((uint32_t)(1<<15)) |((uint32_t)(1 << 16)))        /* disable interrrupts*/


#ifdef MTR_DIRECT
#define LOW_ENERGY_THRESHOLD    ((uint32_t)35697115)

#define VOL_RATIO_FACTOR        ((uint32_t)1001)

#define CURR_RATIO_FACTOR       ((uint32_t)13200)
#define POW_RATIO_FACTOR    ((uint64_t)630303)
#else

//#define LOW_ENERGY_THRESHOLD    ((uint32_t)43300762)	// 0X2A9F619
//#define LOW_ENERGY_THRESHOLD    ((uint32_t)0x294B79A) // 43300762
#define LOW_ENERGY_THRESHOLD    ((uint32_t)29747596)
//#define LOW_ENERGY_THRESHOLD    ((uint32_t)43338684)


#define VOL_RATIO_FACTOR        ((uint32_t)1001)

//#define CURR_RATIO_FACTOR       ((uint32_t)12789 )
#define CURR_RATIO_FACTOR       ((uint32_t)110000)

//#define POW_RATIO_FACTOR    ((uint64_t)324763)
//#define POW_RATIO_FACTOR    ((uint64_t)324465)
#define POW_RATIO_FACTOR    ((uint64_t)75636)
#endif

/*list of V9203 cmd*/






//#define VOL_RATIO_FACTOR        ((uint32_t)2000)
//#define CURR_RATIO_FACTOR       ((uint32_t)13200)
//#define POW_RATIO_FACTOR    ((uint64_t)324)
//#define POW_RATIO_FACTOR    ((uint64_t)314)



#if MTR_NUM_OF_CH==1
#elif MTR_NUM_OF_CH==2
#elif MTR_NUM_OF_CH==3
#endif

/*!
* @par Description:
*   This enumerator contains all available calibration commands
*/
typedef enum
{
  STRT_MAN_CAL_CMD ,     /*!< Start manual calibration command.*/
  NXT_MAN_CAL_CMD ,     /*!< Next calibration command.*/
  STRT_AUTO_CAL_CMD,     /*!< Start auto calibration command.*/
  END_AUTO_CAL_CMD

} cal_cmd_t;

typedef enum
{
  V9203_INIT_ROM = 0,    /*!< Means run initialization with loading from ROM.*/
  V9203_INIT_RAM = 1    /*!< Means run initialization without loading from ROM.*/
}  V9203_init_t;

/*for test only*/

__packed typedef struct {
	uint32_t prev_ActpulseCount[MTR_NUM_OF_CH];
	uint32_t curr_ActpulseCount[MTR_NUM_OF_CH] ;
	uint32_t prev_TotActpulseCount ;
	uint32_t curr_TotActpulseCount ;
        
	uint32_t prev_ReactpulseCount[MTR_NUM_OF_CH];
	uint32_t curr_ReactpulseCount[MTR_NUM_OF_CH] ;
	uint32_t prev_TotReactpulseCount ;
	uint32_t curr_TotReactpulseCount ;
        
	uint32_t prev_AppPulseCount[MTR_NUM_OF_CH];
	uint32_t curr_AppPulseCount[MTR_NUM_OF_CH] ;
	uint32_t prev_TotAppPulseCount ;
	uint32_t curr_TotAppPulseCount ;
} pulseCount_t;


__packed typedef struct {
#ifdef NEUTRAL_FEATURE
  uint64_t rms_curr[MTR_NUM_OF_CH+1];
#else
  uint64_t rms_curr[MTR_NUM_OF_CH];
#endif
  uint64_t rms_vol[MTR_NUM_OF_CH];
  uint64_t act_pow[MTR_NUM_OF_CH];
  uint64_t react_pow[MTR_NUM_OF_CH];
  uint64_t app_pow[MTR_NUM_OF_CH];
}mtr_avg_readings_t;


/*!
* @par Description:
*   This enumerator contains all Calibration Steps.
*/
typedef enum
{
  ADE7758_CALIB_IDLE = (uint8_t)0x00,
  ADE7758_CALIB_START,
  ADE7758_CALIB_STEP_1,
  ADE7758_CALIB_STEP_2,
  ADE7758_CALIB_STEP_3,
  ADE7758_CALIB_STEP_4,
  ADE7758_CALIB_STEP_5,
  ADE7758_CALIB_STEP_6,
  ADE7758_CALIB_SAVE_DATA,
  ADE7758_CALIB_STEP_8,
  ADE7758_CALIB_STEP_9
} ade7758_calib_step;

__packed typedef struct {
  //int32_t calib_rms_curr[MTR_NUM_OF_CH];
  //int32_t calib_rms_vol[MTR_NUM_OF_CH];
  //int32_t calib_act_pow[MTR_NUM_OF_CH];
#ifdef NEUTRAL_FEATURE
    uint32_t calib_rms_curr[MTR_NUM_OF_CH+1];//+1 fo nuteral
#else
    uint32_t calib_rms_curr[MTR_NUM_OF_CH];
#endif
  uint32_t calib_rms_vol[MTR_NUM_OF_CH];
  uint32_t calib_act_pow[MTR_NUM_OF_CH];
  //uint64_t calib_react_pow[MTR_NUM_OF_CH];
  //int64_t calib_ph_compens[MTR_NUM_OF_CH];
  uint8_t   cal_flag;          /*!< Contains CRC of all data in this structure.*/
}mtr_calib_t;

__packed typedef struct {
#ifdef NEUTRAL_FEATURE
  uint64_t rms_curr[MTR_NUM_OF_CH+1];//for nuteral
#else
  uint64_t rms_curr[MTR_NUM_OF_CH];
#endif
  uint64_t rms_vol[MTR_NUM_OF_CH];
  uint64_t ph_angle[MTR_NUM_OF_CH];
  uint64_t pf[MTR_NUM_OF_CH];
  uint64_t line_freq[MTR_NUM_OF_CH];
  uint64_t act_pow[MTR_NUM_OF_CH];
  uint64_t react_pow[MTR_NUM_OF_CH];
  uint64_t app_pow[MTR_NUM_OF_CH];
}mtr_readings_t;


__packed typedef struct 
{ uint16_t addr;
  uint32_t value;
}V9203_reg_values_t;


typedef enum {
  V9203_REG_MTPARA0 = 0xC000, /*Metering Control Register 0*/ 
  V9203_REG_MTPARA1 = 0xC001, /*Metering Control Register 1*/ 
  V9203_REG_MTPARA2 = 0xC002, /*Metering Control Register 2*/
  V9203_REG_MTPARA3 = 0xC003, /*Metering Control Register 3*/
  V9203_REG_ZZCPSEL = 0xEC15, /*No-Load Detection Mode Register*/
  V9203_REG_ZZPA0 = 0xEC23, /*Total/Fundamental Active Power on The Overall System Calculation Mode 0*/
  V9203_REG_ZZPA1 = 0xEC24, /*Total/Fundamental Active Power On The Overall System Calculation Mode 1*/
  V9203_REG_ZZQA0 = 0xEC47, /*Total/Fundamental Reactive Power on the overall system Calculation Mode 0*/
  V9203_REG_ZZQA1 = 0xEC48, /*Total/Fundamental Reactive Power On The Overall System Calculation Mode 1*/
  V9203_REG_ZZAPPA = 0xEC05, /*Total/Fundamental Apparent Power over All System Calculation Mode*/
  V9203_REG_ZZPCF0A = 0xEC34 ,/*CF Pulse Source Selection*/
  V9203_REG_DCUA = 0xF02C, /*instantaneous direct voltage of Phase A*/
  V9203_REG_DCIA = 0xF02D, /*The instantaneous direct current of Phase A.*/
  V9203_REG_DCUB = 0xF02E, /*The instantaneous direct voltage of Phase B*/
  V9203_REG_DCIB = 0xF02F, /* instantaneous direct current of Phase B*/
  V9203_REG_DCUC = 0xF030, /*The instantaneous direct voltage of Phase C*/
  V9203_REG_DCIC = 0xF031, /*instantaneous direct current of Phase C*/
  V9203_REG_DCIN = 0xF032,/*The instantaneous direct current of Channel IN*/
  V9203_REG_FA = 0xC008 ,/* The line frequency of Phase A*/
  V9203_REG_FB = 0xC009,/*The line frequency of Phase B.*/
  V9203_REG_FC = 0xC00A, /* The line frequency of Phase C.*/
  V9203_REG_PHSUB = 0xC00B, /* The voltage phase angle of Phase B*/
  V9203_REG_PHSUC = 0xC00C,/*The voltage phase angle of Phase C*/
  V9203_REG_PHSIA = 0xC00D,/*The current phase angle of Phase A.*/
  V9203_REG_PHSIB = 0xC00E,/*The current phase angle of Phase B*/
  V9203_REG_PHSIC = 0xC00F,/*The current phase angle of Phase C*/
   V9203_REG_AFA = 0xE811 ,/*The instantaneous total power factor of Phase A*/
  V9203_REG_AFB = 0xE812, /* The instantaneous total power factor of Phase B*/
  V9203_REG_AFC = 0xE813 ,/*The instantaneous total power factor of Phase C*/
  V9203_REG_AFS = 0xE814, /*The instantaneous total power factor on the overall system*/
  V9203_REG_MAFA = 0xE8E4 ,/* The average total power factor of Phase A.*/
  V9203_REG_MAFB = 0xE8E5, /*The average total power factor of Phase B*/
  V9203_REG_MAFC = 0xE8E6, /*The average total power factor of Phase C*/
  V9203_REG_MAFS =  0xE8E7, /*The average total power factor on the overall system*/
  V9203_REG_BFA = 0xE857, /*The instantaneous fundamental power factor of Phase A*/
  V9203_REG_BFB = 0xE858,/*The instantaneous fundamental power factor of Phase B*/
  V9203_REG_BFC = 0xE859 ,/*The instantaneous fundamental power factor of Phase C*/
  V9203_REG_BFS = 0xE85A, /* The instantaneous fundamental power factor on the overall system.*/
  V9203_REG_MBFA =  0xE8F9,/*The average fundamental power factor of Phase A*/
  V9203_REG_MBFB = 0xE8FA ,/*The average fundamentalpower factor of Phase B*/
  V9203_REG_MBFC = 0xE8FB, /*The average fundamental power factor of Phase C*/
  V9203_REG_MBFS = 0xE8FC, /*The average fundamental power factor on the overall system*/
  V9203_REG_ARTIA = 0xE83E, /* The instantaneous total current RMS of Phase A*/
  V9203_REG_ARTIB = 0xE83F,  /* The instantaneous total current RMS of Phase B*/
  V9203_REG_ARTIC = 0xE840,  /*The instantaneous total current RMS of Phase C.*/
  V9203_REG_ARTIN = 0xE841 , /*The instantaneous total current RMS of Channel IN*/
  V9203_REG_ARTUA = 0xE842 , /*The instantaneous total voltage RMS of Phase A*/
  V9203_REG_ARTUB = 0xE843 , /*The instantaneous total voltage RMS of Phase B.*/
  V9203_REG_ARTUC = 0xE844 , /*The instantaneous total voltage RMS of Phase C*/
  V9203_REG_MUM = 0xE94B,  /*The algebra sum of the total current RMS on the overall system*/
#if 0
  V9203_REG_MUA = 0xE944 , /*The average total voltage RMS of Phase A*/
  V9203_REG_MUB = 0xE945 , /*The average total voltage RMS of Phase B*/
  V9203_REG_MUC = 0xE946,  /*The average total voltage RMS of Phase C*/
 V9203_REG_MIA = 0xE90E,  /*The average total current RMS of Phase A*/
 V9203_REG_MIB = 0xE90F,  /*The average total current RMS of Phase B*/
 V9203_REG_MIC = 0xE910,  /*The average total current RMS of Phase C.*/
 V9203_REG_MIN = 0xE911,  /*The average total current RMS of Channel IN*/
 V9203_REG_BRTIA = 0xE880  , /*The instantaneous fundamental current RMS ofPhase A*/
 V9203_REG_BRTIB = 0xE881,  /*The instantaneous fundamental current RMS of Phase B*/
 V9203_REG_BRTIC = 0xE882 , /*The instantaneous fundamental current RMS of phase C*/
 V9203_REG_BRTUA = 0xE883 , /*The instantaneous fundamental voltage RMS of Phase A*/
 V9203_REG_BRTUB = 0xE884 , /*The instantaneous fundamental voltage RMS of Phase B*/
 V9203_REG_BRTUC = 0xE885 , /*The instantaneous fundamental voltage RMS of Phase C*/
 V9203_REG_MBIA = 0xE8FD , /*The average fundamental current RMS of Phase A.*/
 V9203_REG_MBIB =  0xE8FE , /*The average fundamental current RMS of Phase B*/
 V9203_REG_MBIC = 0xE8FF,  /*The average fundamental current RMS of Phase C*/
 V9203_REG_MBUA = 0xE90B , /*The average fundamental voltage RMS of Phase A*/
 V9203_REG_MBUB = 0xE90C , /*The average fundamental voltage RMS of Phase B*/
 V9203_REG_MBUC = 0xE90D , /*The average fundamental voltage RMS of Phase C*/
#endif
 V9203_REG_AABSPSUM0 = 0xE801 , /*The absolute value of the instantaneous total active power on the overall system in calculation mode 0 */
 V9203_REG_AABSPSUM1 = 0xE802 , /* The absolute value of the instantaneous total active power on the overall system in Calculation Mode 1*/
 V9203_REG_AABSQSUM0 = 0xE803 , /*The absolute value of the instantaneous total reactive power on the overall system in Calculation Mode 0*/
 V9203_REG_AABSQSUM1 = 0xE804 , /*The absolute value of the instantaneous total reactive power on the overall system in Calculation Mode 1*/
 V9203_REG_ABSAPA = 0xE805 , /*The absolute value of the instantaneous total active power of Phase A*/
 V9203_REG_ABSAPB = 0xE806 , /* The absolute value of the instantaneous total active power of Phase B*/
 V9203_REG_ABSAPC = 0xE807 , /*The absolute value of the instantaneous total active power of Phase C*/
 V9203_REG_ABSAQA = 0xE808 , /*The absolute value of the instantaneous total reactive power of Phase A*/
 V9203_REG_ABSAQB = 0xE809 , /*The absolute value of the instantaneous total reactive power of Phase B*/
 V9203_REG_ABSAQC = 0xE80A , /*The absolute value of the instantaneous total reactive power of Phase C*/
 V9203_REG_APPA = 0xE81F , /*The instantaneous total apparent power of Phase A*/
 V9203_REG_APPB = 0xE820 , /*The instantaneous total apparent power of Phase B*/
 V9203_REG_APPC = 0xE821 , /*The instantaneous total apparent power of Phase C*/
 V9203_REG_APSUM0 = 0xE822 , /*The algebra value of the instantaneous total active power on the overall system in Calculation Mode 0*/
 V9203_REG_APSUM1 = 0xE823 ,  /*The algebra value of the instantaneous total active power on the overall system in Calculation Mode 1.*/
 V9203_REG_APTLA = 0xE824 , /*The algebra value of the instantaneous total active power of Phase A.*/
 V9203_REG_APTLB = 0xE825 , /*The algebra value of the instantaneous total active power of Phase B.*/
 V9203_REG_APTLC = 0xE826 , /*The algebra value of the instantaneous total active power of Phase C.*/
 V9203_REG_AQSUM0 = 0xE831 , /*The algebra value of the instantaneous total reactive power on the overall system in Calculation Mode 0.*/
 V9203_REG_AQSUM1 = 0xE832 , /*The algebra value of the instantaneous total reactive power on the overall system in Calculation Mode 1.*/
 V9203_REG_AQTLA = 0xE833 , /*The algebra value of the instantaneous total reactive power of Phase A.*/
 V9203_REG_AQTLB = 0xE834 , /*The algebra value of the instantaneous total reactive power of Phase B*/
 V9203_REG_AQTLC = 0xE835 , /*The algebra value of the instantaneous total reactive power of Phase C.*/
 V9203_REG_ASSUM = 0xE84E , /*The instantaneous total apparent power on the overall system.*/
#if 0
 V9203_REG_MAPA = 0xE8E8 , /*The average total active power of Phase A.*/
 V9203_REG_MAPB = 0xE8E9 , /*The average total active power of Phase B*/
 V9203_REG_MAPC  = 0xE8EA ,/*The average total active power of Phase C.*/
 V9203_REG_MAPPA = 0xE8EB , /*The average total apparent power of Phase A.*/
 V9203_REG_MAPPB = 0xE8EC , /*The average total apparent power of Phase B.*/
 V9203_REG_MAPPC = 0xE8ED , /*The average total apparent power of Phase C.*/
 V9203_REG_MAPSUM0 = 0xE8EE , /*The average total active power on the overall system 0.*/
 V9203_REG_MAPSUM1 = 0xE8EF , /*The average total active power on the overall system 1*/
 V9203_REG_MAQA = 0xE8F0 , /*The average total reactive power of Phase A.*/
 V9203_REG_MAQB = 0xE8F1,  /*The average total reactive power of Phase B*/
 V9203_REG_MAQC = 0xE8F2 , /*The average total reactive power of Phase C.*/
 V9203_REG_MAQSUM0 = 0xE8F3 , /*The average total reactive power on the overall system 0.*/
 V9203_REG_MAQSUM1 = 0xE8F4 , /*The average total reactive power on the overall system 1.*/
 V9203_REG_MASSUM = 0xE8F5 , /*The average total apparent power on the overall system.*/
#endif
 V9203_REG_ABSBPA = 0xE80B , /*The absolute value of the instantaneous fundamental active power of Phase A*/
 V9203_REG_ABSBPB = 0xE80C , /*The absolute value of the instantaneous fundamental active power of Phase B.*/
 V9203_REG_ABSBPC = 0xE80D , /*The absolute value of the instantaneous fundamental active power of Phase C.*/
 V9203_REG_ABSBQA = 0xE80E , /*The absolute value of the instantaneous fundamental reactive power of Phase A.*/
 V9203_REG_ABSBQB = 0xE80F , /*The absolute value of the instantaneous fundamental reactive power of Phase B.*/
 V9203_REG_ABSBQC = 0xE810 , /*The absolute value of the instantaneous fundamental reactive power of Phase C.*/
 V9203_REG_BABSPSUM0 = 0xE850 , /*The absolute value of the instantaneous fundamental active power on the overall system in Calculation Mode 0*/
 V9203_REG_BABSPSUM1 = 0xE851 , /*The absolute value of the instantaneous fundamental active power on the overall system in calculation mode 1*/
 V9203_REG_BABSQSUM0 = 0xE852 , /*The absolute value of the instantaneous fundamental reactive power on the overall system in Calculation Mode 0.*/
 V9203_REG_BABSQSUM1 = 0xE853 , /*The absolute value of the instantaneous fundamental reactive power on the overall system in Calculation Mode 1.*/
 V9203_REG_BAPPA  = 0xE854 , /*The instantaneous fundamental apparent power of Phase A.*/
 V9203_REG_BAPPB = 0xE855,  /*The instantaneous fundamental apparent power of Phase B.*/
 V9203_REG_BAPPC = 0xE856 , /*The instantaneous fundamental apparent power of Phase C.*/
 V9203_REG_BPSUM0 =  0xE864 , /*The algebra value of the instantaneous fundamental active power on the overall systemin Calculation Mode 0.*/
  V9203_REG_BPSUM1 = 0xE865 , /*The algebra value of the instantaneous fundamental active power on the overall system in Calculation Mode 1.*/
  V9203_REG_BPTLA = 0xE866 , /*The algebra value of the instantaneous fundamental active power of Phase A.*/
 V9203_REG_BPTLB = 0xE867 , /*The algebra value of the instantaneousfundamental active power of Phase B.*/
 V9203_REG_BPTLC = 0xE868 , /*The algebra value of the instantaneous fundamental active power of Phase C.*/
 V9203_REG_BQSUM0 = 0xE875 , /*The algebra value of the instantaneous fundamental reactive power on the overall system in Calculation Mode 0*/
 V9203_REG_BQSUM1 = 0xE876 , /*The algebra value of the instantaneous fundamental reactive power on the overall system in Calculation Mode 1.*/
 V9203_REG_BQTLA = 0xE877 , /*The algebra value of the instantaneous fundamental reactive power of Phase A*/
 V9203_REG_BQTLB = 0xE878 , /*The algebra value of the instantaneous fundamental reactive power of Phase B.*/
 V9203_REG_BQTLC = 0xE879 , /*The algebra value of the instantaneous fundamental reactive power of Phase C.*/
 V9203_REG_BSSUM = 0xE88C , /*The instantaneous fundamental apparent power on the overall system.*/
#if 0
 V9203_REG_MBAPPA = 0xE8F6 , /*The average fundamental apparent power of Phase A.*/
 V9203_REG_MBAPPB = 0xE8F7 , /*The average fundamental apparent power of Phase B.*/
 V9203_REG_MBAPPC = 0xE8F8 , /*The average fundamental apparent power of Phase C.*/
 V9203_REG_MBPA = 0xE900 , /*The average fundamental active power of Phase A.*/
 V9203_REG_MBPB = 0xE901 , /*The average fundamental active power of Phase B*/
 V9203_REG_MBPC = 0xE902 , /*The average fundamental active power of Phase C*/
 V9203_REG_MBPSUM0 = 0xE903 , /*The average fundamental active power on the overall system 0.*/
 V9203_REG_MBPSUM1 = 0xE904 , /*The average fundamental active power on the overall system 1.*/
 V9203_REG_MBQA = 0xE905 , /*The average fundamental reactive power of Phase A.*/
 V9203_REG_MBQB = 0xE906 , /*The average fundamental reactive power of Phase B.*/
 V9203_REG_MBQC = 0xE907 , /*The average fundamental reactive power of Phase C.*/
 V9203_REG_MBQSUM0 = 0xE908 , /*The average fundamental reactive power on the overall system 0.*/
 V9203_REG_MBQSUM1 = 0xE909,  /*The average fundamental reactive power on the overall system 1.*/
 V9203_REG_MBSSUM = 0xE90A , /*The average fundamental apparent power on the overall system.*/
#endif
 V9203_REG_EGYAPPAH = 0xE8AC , /*Higher 32 bits of the register for the total apparent energy of Phase A.*/
 V9203_REG_EGYAPPAL = 0xE8AD , /*Lower 32 bits of the register for the total apparent energy of Phase A*/
 V9203_REG_EGYAPPBH = 0xE8AE , /*Higher 32 bits of the register for the total apparent energy of Phase B.*/
  V9203_REG_EGYAPPBL = 0xE8AF , /*Lower 32 bits of the register for the total apparent energy of Phase B.*/
  V9203_REG_EGYAPPCH = 0xE8B0 , /*Higher 32 bits of the register for the total apparent energy of Phase C.*/
  V9203_REG_EGYAPPCL = 0xE8B1 , /*Lower 32 bits of the register for the total apparent energy of Phase C.*/
  V9203_REG_EGYAPPSH = 0xE8B2 , /*Higher 32 bits of the register for the total apparent energy on the overall system.*/
  V9203_REG_EGYAPPSL = 0xE8B3 , /*Lower 32 bits of the register for the total apparent energy on the overall system.*/
  V9203_REG_EGYPAH = 0xE8D0 , /*Higher 32 bits of the register for the total active energy of Phase A.*/
  V9203_REG_EGYPAL = 0xE8D1 , /* Lower 32 bits of the register for the total active energy of Phase A.*/
  V9203_REG_EGYPBH = 0xE8D2 , /* Higher 32 bits of the register for the total active energy of Phase B*/
   V9203_REG_EGYPBL = 0xE8D3 , /*Lower 32 bits of the register for the total active energy of Phase B.*/
  V9203_REG_EGYPCH = 0xE8D4 , /* Higher 32 bits of the register for the total active energy of Phase C.*/
  V9203_REG_EGYPCL = 0xE8D5 , /*Lower 32 bits of the register for the total active energy of Phase C.*/
  V9203_REG_EGYPS0H = 0xE8D6 , /*Higher 32 bits of the register for the total active energy 0, accumulated by the total active power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYPS0L  = 0xE8D7 ,/*Lower 32 bits of the register for the total active energy 0, accumulated by the total active power  on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYPS1H = 0xE8D8 , /*Higher 32 bits of the register for the total active energy 1, accumulated by the total active power on the overall system in Calculation Mode 1*/
  V9203_REG_EGYPS1L = 0xE8D9 , /*Lower 32 bits of the register for the total active energy 1, accumulated by the total active power on the overall system in Calculation Mode 1.*/
  V9203_REG_EGYQAH = 0xE8DA , /*Higher 32 bits of the register for the total reactive energy of Phase A.*/
  V9203_REG_EGYQAL = 0xE8DB , /* Lower 32 bits of the register for the total reactive energy of Phase A.*/
  V9203_REG_EGYQBH = 0xE8DC , /*Higher 32 bits of the register for the total reactive energy of Phase B.*/
  V9203_REG_EGYQBL = 0xE8DD , /*Lower 32 bits of the register for the total reactive energy of Phase B.*/
  V9203_REG_EGYQCH = 0xE8DE , /*Higher 32 bits of the register for the total reactive energy of Phase C.*/
  V9203_REG_EGYQCL = 0xE8DF , /*Lower 32 bits of the register for the total reactive energy of Phase C.*/
  V9203_REG_EGYQS0H = 0xE8E0 , /*Higher 32 bits of the register for the total reactive energy 0, accumulated by the total reactive power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYQS0L = 0xE8E1 , /*Lower 32 bits of the register for the total reactive energy 0, accumulated by the total reactive power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYQS1H = 0xE8E2 , /*Higher 32 bits of the register for the total reactive energy 1, accumulated by the total reactive power on the overall system in Calculation Mode 1.*/
  V9203_REG_EGYQS1L = 0xE8E3 , /*Lower 32 bits of the register for the total reactive energy 1, accumulated by the total reactive power on the overall system in Calculation Mode 1.*/
  V9203_REG_CFAPPA = 0xE88D, /* Pulse counter of the total apparent energy of Phase A. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFAPPB = 0xE88E , /*Pulse counter of the total apparent energy of Phase B. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFAPPC =0xE88F , /*Pulse counter of the total apparent energy of Phase C. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFAPPS = 0xE890 , /*Pulse counter of the total apparent energy on the overall system. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFPA = 0xE89F  ,/*Pulse counter of the total active energy of Phase A. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFPB = 0xE8A0 , /*Pulse counter of the total active energy of Phase B. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFPC = 0xE8A1 , /*Pulse counter of the total active energy of Phase C. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFPS0 = 0xE8A2 , /*Pulse counter of the total active energy 0 on the overall system. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFPS1 = 0xE8A3 , /*Pulse counter of the total active energy 1 on the overall system. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFQA = 0xE8A4 , /*Pulse counter of the total reactive energy of Phase A. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFQB = 0xE8A5,  /*Pulse counter of the total reactive energy of Phase B. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFQC = 0xE8A6,  /*Pulse counter of the total reactive energy of Phase C. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFQS0 = 0xE8A7 , /*Pulse counter of the total reactive energy 0 on the overall system. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFQS1  = 0xE8A8 , /*Pulse counter of the total reactive energy 1 on the overall system. R/W 32-bit, unsigned, positive*/
#if 0
  V9203_REG_EGYBAPPAH = 0xE8B4 , /*Higher 32 bits of the register for the fundamental apparent energy of Phase A. R/W*/
  V9203_REG_EGYBAPPAL = 0xE8B5 , /*Lower 32 bits of the register for the fundamental apparent energy of Phase A.*/
  V9203_REG_EGYBAPPBH = 0xE8B6, /* Higher 32 bits of the register for the fundamental apparent energy of Phase B.*/
  V9203_REG_EGYBAPPBL = 0xE8B7,  /* Lower 32 bits of the register for the fundamental apparent energy of Phase B.*/
  V9203_REG_EGYBAPPCH = 0xE8B8 , /* Higher 32 bits of the register for the fundamental apparent energy of Phase C.*/
  V9203_REG_EGYBAPPCL = 0xE8B9,  /*Lower 32 bits of the register for the fundamental apparent energy of Phase C*/
  V9203_REG_EGYBAPPSH = 0xE8BA , /*Higher 32 bits of the register for the fundamental apparent energy on the overall system. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBAPPSL = 0xE8BB,  /*Lower 32 bits of the register for the fundamental apparent energy on the overall system. R/W*/
  V9203_REG_EGYBPAH = 0xE8BC , /*Higher 32 bits of the register for the fundamental active energy of Phase A. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBPAL = 0xE8BD,  /*Lower 32 bits of the register for the fundamental active energy of Phase A. R/W*/ 
  V9203_REG_EGYBPBH = 0xE8BE , /*Higher 32 bits of the register for the fundamental active energy of Phase B. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBPBL = 0xE8BF , /*Lower 32 bits of the register for the fundamental active energy of Phase B. R/W*/ 
  V9203_REG_EGYBPCH = 0xE8C0,  /*Higher 32 bits of the register for the fundamental active energy of Phase C. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBPCL = 0xE8C1 , /*Lower 32 bits of the register for the fundamental active energy of Phase C. R/W*/
  V9203_REG_EGYBPS0H = 0xE8C2, /*Higher 32 bits of the register for the fundamental active energy 0, accumulated by the
fundamental active power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYBPS0L = 0xE8C3, /*Lower 32 bits of the register for the fundamental active energy 0, accumulated by the
fundamental active power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYBPS1H = 0xE8C4, /*Higher 32 bits of the register for the fundamental active energy 1, accumulated by the
fundamental active power on the overall system in Calculation Mode 1.*/
  V9203_REG_EGYBPS1L = 0xE8C5, /*Lower 32 bits of the register for the fundamental active energy 1, accumulated by the
fundamental active power on the overall system in Calculation Mode 1.*/
  V9203_REG_EGYBQAH = 0xE8C6 , /*Higher 32 bits of the register for the fundamental reactive energy of Phase A. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBQAL = 0xE8C7 , /*Lower 32 bits of the register for the fundamental reactive energy of Phase A. R/W*/
  V9203_REG_EGYBQBH = 0xE8C8 , /* Higher 32 bits of the register for the fundamental reactive energy of Phase B. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBQBL = 0xE8C9 , /*Lower 32 bits of the register for the fundamental reactive energy of Phase B. R/W*/
  V9203_REG_EGYBQCH = 0xE8CA , /*Higher 32 bits of the register for the fundamental reactive energy of Phase C. R/W
64-bit, unsigned, positive*/
  V9203_REG_EGYBQCL = 0xE8CB  ,/*Lower 32 bits of the register for the fundamental reactive energy of Phase C. R/W*/
  V9203_REG_EGYBQS0H = 0xE8CC ,/*Higher 32 bits of the register for the fundamental reactive energy 0, accumulated by the
fundamental reactive power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYBQS0L = 0xE8CD ,/*Lower 32 bits of the register for the fundamental reactive energy 0, accumulated by the
fundamental reactive power on the overall system in Calculation Mode 0.*/
  V9203_REG_EGYBQS1H = 0xE8CE ,/*Higher 32 bits of the register for the fundamental reactive energy 1, accumulated by the
fundamental reactive power on the overall system in Calculation Mode 1.*/
  V9203_REG_EGYBQS1L = 0xE8CF ,/*Lower 32 bits of the register for the fundamental reactive energy 1, accumulated by the
fundamental reactive power on the overall system in Calculation Mode 1.*/
  V9203_REG_CFBAPPA = 0xE891, /*  Pulse counter of the fundamental apparent energy of Phase A. R/W 32-bit, unsigned, positive */
  V9203_REG_CFBAPPB = 0xE892, /*  Pulse counter of the fundamental apparent energy of Phase B. R/W 32-bit, unsigned, positive*/
  V9203_REG_CFBAPPC = 0xE893, /* Pulse counter of the fundamental apparent energy of Phase C. R/W*/
  V9203_REG_CFBAPPS = 0xE894 , /* pulse counter of the fundamental apparent energy on the overall system. */
  V9203_REG_CFBPA = 0xE895,  /* pulse counter of the fundamental active energy of Phase A.*/
  V9203_REG_CFBPB = 0xE896,  /* pulse counter of the fundamental active energy of Phase B.*/ 
  V9203_REG_CFBPC = 0xE897 , /* pulse counter of the fundamental active energy of Phase C. */
  V9203_REG_CFBPS0 = 0xE898,  /* pulse counter of the fundamental active energy 0 on the overall system.*/ 
  V9203_REG_CFBPS1 = 0xE899 , /* pulse counter of the fundamental active energy 1 on the overall system.  */
  V9203_REG_CFBQA = 0xE89A , /* pulse counter of the fundamental reactive energy of Phase A.  */
  V9203_REG_CFBQB = 0xE89B , /* pulse counter of the fundamental reactive energy of Phase B.  */
  V9203_REG_CFBQC = 0xE89C , /* pulse counter of the fundamental reactive energy of Phase C.  */
  V9203_REG_CFBQS0 = 0xE89D , /* pulse counter of the fundamental reactive energy 0 on the overall system. */  
  V9203_REG_CFBQS1 = 0xE89E , /* pulse counter of the fundamental reactive energy 1 on the overall system.  */
#endif
  V9203_REG_WARTIN = 0xE96B , /*To set gain calibration of the total current RMS of  Channel IN.*/
  V9203_REG_ZZPCF0 = 0xEC33 , /*The total/fundamental active power on the overall system for energy accumulation in high-speed mode.*/
  V9203_REG_ZZPCF1 = 0xEC38 , /*The total/fundamental reactive power on the overall system for energy accumulation in high-speed mode.*/
  V9203_REG_ZZPCF2 = 0xEC3D , /*The total/fundamental active power on the overall system for energy accumulation in high-speed mode.*/ 
  V9203_REG_ZZPCF3 = 0xEC42 , /*The total/fundamental apparent power or the total reactive power on the overall system for energy accumulation in high-speed mode.*/
  V9203_REG_EGY1H = 0xD003 , /*Higher 32 bits of the register for the total/fundamental reactive
  energy on the overall system in high-speed mode (CF1).*/
  V9203_REG_EGY2L = 0xD004, /*Lower 32 bits of the register for the total/fundamental active energy
  on the overall system in high-speed mode (CF2).*/ 
  V9203_REG_EGY2H = 0xD005 , /*Higher 32 bits of the register for the total/fundamental active
  energy on the overall system in high-speed mode (CF2).*/
  V9203_REG_EGY3L = 0xD006 , /*Lower 32 bits of the register for the total/fundamental apparent energy or the total reactive energy on the overall system in high-speed mode (CF3).*/
  V9203_REG_EGY3H = 0xD007 , /*Higher 32 bits of the register for the total/fundamental apparent energy or the total reactive energy on the overall system in
  high-speed mode (CF3).*/
    V9203_REG_CF0 = 0xD008 , /*Pulse counter of the total/fundamental active energy on the overall
  system in high-speed mode (CF0).*/
  V9203_REG_CF1 = 0xD009,  /*Pulse counter of the total/fundamental reactive energy on the
  overall system in high-speed mode (CF1).*/
  V9203_REG_CF2 = 0XD00A , /*Pulse counter of the total/fundamental active energy on the overall
  system in high-speed mode (CF2).*/
  V9203_REG_CF3 = 0XD00B , /*Pulse counter of the total/fundamental apparent energy or thte
  total reactive energy on the overall system in high-speed mode(CF3).*/

  /* interrupt registers*/
  V9203_REG_IRQEN0 = 0xA000 ,/* IRQ0 Interrupt Enable Register*/
  V9203_REG_IRQFLAG = 0xA002 ,/*Interrupt Flag Register */
    
   /* CTRL REGISTERS*/
   V9203_REG_ANCTRL0 = 0x8000 , /* Analog Control Register 0*/ 
   V9203_REG_ANCTRL1 = 0x8001 , /* Analog control register 1 */
   V9203_REG_ANCTRL2 = 0x8002 , /* analog control register 2 */
   V9203_REG_ANCTRL3  = 0x8003 , /* analog control register 3 */
   
     /*threshold registers*/
     V9203_REG_ZZEGYTHH= 0xEC1E , /* HIGH ENERGY THRESHOLD*/
     V9203_REG_ZZEGYTHL= 0xEC1F , /* LOW ENERGY THRESHOLD*/
}v9203_reg_t;



extern uint8_t V9203_data_crpt_flag ;
extern uint8_t V9203_comm_err_flag ;
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/
void V9203_inst_read(uint8_t phase);
void ade7758_earth_tmp(void);
void V9203_clr_ins_reverse(uint8_t );
uint8_t V9203_get_ins_reverse(void);
uint8_t chk_reverse_ch(void);
void V9203_Reset_SPI(void);
uint8_t V9203_CheckSum(uint32_t dat,uint8_t cmd,uint8_t type);
uint32_t V9203_ReceiveFrame(uint8_t cmd);
uint8_t V9203_Read_Data(uint16_t addr,uint8_t DataLen);
void V9203_Send_Frame(uint32_t CmdData);
void V9203_Write_Data(uint16_t addr,uint32_t data);
void V9203_set_reg(uint16_t V9203_REG_ADDR, uint32_t reg_value);
uint32_t V9203_get_reg(uint16_t V9203_REG_ADDR) ;
uint32_t Init_calibParam(V9203_init_t init_type);
void V9203_Init(V9203_init_t init_type);
uint32_t V9203_get_rms_voltage(uint8_t arg1);
uint32_t V9203_get_rms_current(uint8_t arg1);
uint32_t V9203_instant_total_irms_read(uint8_t LocalPhase);
uint32_t V9203_avg_total_irms_read(uint8_t LocalPhase);
uint32_t V9203_avg_fundamental_irms_read(uint8_t LocalPhase);
uint32_t V9203_instant_total_vrms_read(uint8_t LocalPhase) ;
uint32_t V9203_avg_total_vrms_read(uint8_t LocalPhase);
uint32_t V9203_avg_fundamental_vrms_read(uint8_t LocalPhase) ;
uint32_t V9203_avg_active_power_read(uint8_t LocalPhase);
//uint64_t V9203_instant_active_power_read(uint8_t LocalPhase) ;
uint32_t V9203_avg_reactive_power_read(uint8_t LocalPhase) ;
//uint64_t V9203_instant_reactive_power_read(uint8_t LocalPhase);
uint32_t V9203_avg_apparent_power_read(uint8_t LocalPhase) ;
//uint64_t V9203_instant_apparent_power_read(uint8_t LocalPhase);
uint32_t V9203_line_freq_read(uint8_t LocalPhase);
uint32_t V9203_vol_phase_angle_read(uint8_t LocalPhase);
uint32_t V9203_avg_total_PF_read(uint8_t LocalPhase);
uint32_t V9203_instant_total_PF_read(uint8_t LocalPhase);


uint64_t V9203_total_apparent_Energy_read(uint8_t LocalPhase);
uint64_t V9203_total_active_Energy_read_PH(uint8_t LocalPhase);
uint64_t V9203_total_Reactive_Energy_read(uint8_t LocalPhase);
uint32_t V9203_INT_flagReg_read(void);
uint32_t V9203_Actpulse_counter_read(uint8_t LocalPhase);
uint32_t V9203_Reactpulse_counter_read(uint8_t LocalPhase);
uint32_t V9203_AppPulse_counter_read(uint8_t LocalPhase);
uint32_t V9203_tot_Actpulse_counter_read(void);
uint64_t V9203_total_active_Energy_read(void);
void CF_read(uint32_t* ptr_pulseCounter);
uint64_t V9203_get_ActEnergy_Accum(uint8_t phase);
uint64_t V9203_get_ReactEnergy_Accum(uint8_t phase);
uint64_t V9203_get_AppEnergy_Accum(uint8_t phase);
void V9203_read_initial_PulseCount(void);
uint32_t V9203_tot_AppPulse_counter_read(void);
uint32_t V9203_tot_Reactpulse_counter_read(void);
uint64_t V9203_get_tot_ActEnergy_Accum(void);
uint64_t V9203_get_tot_ReactEnergy_Accum(void);
uint64_t V9203_get_tot_AppEnergy_Accum(void);
void V9203_calib(void);

uint32_t V9203_instant_tot_active_power_read_milliW(void);
uint32_t V9203_instant_tot_reactive_power_read_milliVAR(void);
uint32_t V9203_instant_tot_apparent_power_read_milliVA(void );
uint32_t V9203_instant_active_power_read_milliW(uint8_t LocalPhase) ;
uint64_t V9203_instant_active_power_read_microW(uint8_t LocalPhase);
uint32_t V9203_instant_reactive_power_read_milliVAR(uint8_t LocalPhase);
uint64_t V9203_instant_reactive_power_read_microVAR(uint8_t LocalPhase);
uint32_t V9203_instant_apparent_power_read_milliVA(uint8_t LocalPhase);
uint64_t V9203_instant_apparent_power_read_microVA(uint8_t LocalPhase);
double V9203_calc_act_pow_error(uint64_t avg_reading);
int32_t V9203_RMS_Gain_Calib(uint32_t avg_reading, float std_mtr_value);
//int64_t Act_phase_compens(double pow_error);
int64_t V9203_Pow_Gain_Calib(double pow_error, uint64_t avg_reading);
void V9203_Energy_task(void);
//void V9203_chk_active_phases(void);
void Pow_Gain_Adjust(uint8_t ind, uint32_t pow_value);
void RMS_CURR_Adjust(uint8_t ind, uint32_t rms_curr);
void RMS_VOL_Adjust(uint8_t ind, uint32_t rms_vol);
void V9203_task(void);
void V9203_ph_seq_tmp(void);
//uint8_t V9203_get_active_phases(void);
uint8_t V9203_self_test(void);

uint8_t cal_state(void);
void cal_cmd(uint8_t arg1);
void cal_handler_task(void);
void manual_cal_handler_task(void);
void V9203_new_sec(void);
void V9203_SendFilterFrame(uint8_t filter_type);
void V9203_IRQ_isr(void);
uint8_t V9203_chkFailure(void);
void V9203_IRQ_config(void);

uint32_t V9203_curr_phase_angle_read(uint8_t LocalPhase);
void V9203_calib_init(void);
/*!
* @brief used to clear previous Active phases.
*
* @par Description:
* This function is responsible to clear previous active phases.
*
* @param[out] void
*
*/
void V9203_clr_prv_no_active_phases(void);
/* @brief This function used to read active phases status.
*
* @par Description:
* Used to get the status for active phases.
*
* @return
* - uint8_t active phases flags.
*/
uint8_t V9203_get_active_phases(void);
/*!
* @brief used to check the Active phases.
*
* @par Description:
* This function is responsible to check active phases and select the zero crossing and frequancy source .
*
* @param[out] void
*
*/
void V9203_chk_active_phases(void);

void SelfTest_SetV9203Param(void);
#ifdef NEUTRAL_FEATURE
uint32 V9203_Check_Neutral_Mismatch ();
#endif
#endif /* V9203_ENABLE */








/*------------------------------------------------------------------------------------
  from Meter_Config.h
------------------------------------------------------------------------------------*/

/*==========================================================================================

INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
////////////#include "ctrl.h"       //REMOVE_METER_INTERFACE
/* #include "Two_Phase_Interface.h" -- inlined in this file */
#ifdef V9203_ENABLE
/* #include "V9203.h" -- inlined in this file */
#elif defined(V9261f_ENABLE)
// #include "V9261f.h"
#endif
#ifdef EM_DL130_BOARD
/* #include "DINRail_Metering.h" -- inlined in this file */
#endif
/*==========================================================================================

						  DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
								  Constants and Definitions
------------------------------------------------------------------------------------------*/
#ifdef ADE7953_ENABLE

#endif

/*------------------------------------------------------------------------------------------
Macros
------------------------------------------------------------------------------------------*/
#ifndef GHANA_PILOT_CONFIG
/*!
 * @brief rms gain calibration
 */
/*!
 * @brief rms gain calibration
 */
/*!
 * @brief rms offset calibration
 */
#endif

#ifdef MTR_SINGLE_PH
/*! Used to get high segment of total accumulative reactive energy in VARH for all channels.*/
#define MTR_GET_TOT_REACT_EN_HIGH() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_react_en.high_reg
/*! Used to get low segment of total accumulative reactive energy in VARH for all channels.*/
#define MTR_GET_TOT_REACT_EN_LOW() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_react_en.low_reg
#endif




/*!
 * @addtogroup MTR_READ
 * @{
 */
#ifdef MTR_THREE_PH
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
#endif // MTR_THREE_PH




/*! Used to get high segment of total accumulative active energy in WH for all channels.*/
#define MTR_GET_TOT_ACT_EN_HIGH() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_act_en.high_reg
/*! Used to get low segment of total accumulative active energy in WH for all channels.*/
#define MTR_GET_TOT_ACT_EN_LOW() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_act_en.low_reg
/*! Used to get low segment of total accumulative generated active energy in WH for all channels.*/
// TODO: check it later
#ifdef DLMS_ENABLED
/*! Used to get high segment of total accumulative generated active energy in WH for all channels.*/
#else
/*! Used to get high segment of total accumulative generated active energy in WH for all channels.*/
#endif
// TODO: check it later
/*! Used to get high segment of total accumulative generated active energy in WH for all channels.*/
/*! Used to get values and years of last 3 years.*/



#ifdef MTR_THREE_PH
/*! Used to get high segment of accumulative reverse active energy in WH for any channel.*/
/*! Used to get low segment of accumulative reverse active energy in WH for any channel.*/
#endif//MTR_THREE_PH
/*! Used to get low segment of total accumulative forward active energy in WH for all channels.*/
/*! Used to get high segment of total accumulative forward active energy in WH for all channels.*/
#ifdef MTR_THREE_PH
    /*! Used to get high segment of accumulative forward active energy in WH for any channel.*/
    /*! Used to get low segment of accumulative forward active energy in WH for any channel.*/
#endif//MTR_THREE_PH


/*! Used to get high segment of accumulative reactive energy in VARH for any channel.*/
/*! Used to get low segment of accumulative reactive energy in VARH for any channel.*/
                                                
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/

/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/

/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/

/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/



#ifdef ANSI_STACK

//---------------------------------------------------------------------------------------------------//
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/

#endif//ANSI_STACK


/*! Used to get high segment of total accumulative reactive energy in VARH for all channels.*/
#define    MTR_GET_TOT_REACT_EN_HIGH()       mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_react_en.high_reg
/*! Used to get low segment of total accumulative reactive energy in VARH for all channels.*/
#define    MTR_GET_TOT_REACT_EN_LOW()        mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_react_en.low_reg

/*! Used to get high segment of accumulative apparent energy in VAH for any channel.*/
/*! Used to get low segment of accumulative apparent energy in VAH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/
/*! Used to get high segment of accumulative active energy in WH for any channel.*/
/*! Used to get low segment of accumulative active energy in WH for any channel.*/





/*! Used to get high segment of total accumulative apparent energy in VAH for all channels.*/
#define MTR_GET_TOT_APP_EN_HIGH() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_app_en.high_reg
/*! Used to get low segment of total accumulative apparent energy in VAH for all channels.*/
#define MTR_GET_TOT_APP_EN_LOW() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_app_en.low_reg

#ifdef MTR_RVS_TMPR

#ifdef MTR_THREE_PH
/*! Used to get high segment of accumulative reverse active energy in WH for any channel.*/
/*! Used to get low segment of accumulative reverse active energy in WH for any channel.*/
#endif // MTR_THREE_PH

#ifdef MTR_THREE_PH
/*! Used to get high segment of accumulative reverse reactive energy in VARH for any channel.*/
/*! Used to get low segment of accumulative reverse reactive energy in VARH for any channel.*/
#endif // MTR_THREE_PH

/*! Used to get high segment of total accumulative reverse active energy in WH for all channels.*/
#define MTR_GET_TOT_REV_ACT_EN_HIGH() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_rev_act_en.high_reg
/*! Used to get low segment of total accumulative reverse active energy in WH for all channels.*/
#define MTR_GET_TOT_REV_ACT_EN_LOW() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_rev_act_en.low_reg
/*! Used to get high segment of total accumulative reverse reactive energy in VARH for all channels.*/
#define MTR_GET_TOT_REV_REACT_EN_HIGH() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_rev_react_en.high_reg
/*! Used to get low segment of total accumulative reverse reactive energy in VARH for all channels.*/
#define MTR_GET_TOT_REV_REACT_EN_LOW() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_rev_react_en.low_reg
#endif // MTR_RVS_TMPR



#ifdef MTR_ERTH_TMPR
/*! Used to get high segment of total accumulative fault energy in WH for all channels.*/
#define MTR_GET_TOT_FLT_EN_HIGH() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_flt_en.high_reg
/*! Used to get low segment of total accumulative fault energy in WH for all channels.*/
#define MTR_GET_TOT_FLT_EN_LOW() mtr_data.mtr_data_mem.mtr_acc_md.acc_mic_en_reg.acc_en_reg.tot_flt_en.low_reg
#endif


/*! Used to get value of calculate maximum demand Reverse.*/
/*! Used to get value of calculate maximum demand froward.*/
/*! Used to get value of calculate maximum demand W.*/
/*! Used to get value of calculated maximum demand Watts minutes.*/
/*! Used to get value of calculated maximum demand Watts hours.*/
/*! Used to get value of calculated maximum demand Watts days.*/
/*! Used to get value of calculated maximum demand Watts month.*/
/*! Used to get value of calculated maximum demand Watts years.*/
/*! Used to get value of period minutes maximum demand W.*/
/*! Used to get value of calculate maximum demand W.*/
/*! Used to get value of calculated maximum demand Watts minutes.*/
/*! Used to get value of calculated maximum demand Watts hours.*/
/*! Used to get value of calculated maximum demand Watts days.*/
/*! Used to get value of calculated maximum demand Watts month.*/
/*! Used to get value of calculated maximum demand Watts years.*/
/*! Used to get value of period minutes maximum demand W.*/
/*! Used to get value of calculate maximum demand W.*/
/*! Used to get value of calculated maximum demand Watts minutes.*/
/*! Used to get value of calculated maximum demand Watts hours.*/
/*! Used to get value of calculated maximum demand Watts days.*/
/*! Used to get value of calculated maximum demand Watts month.*/
/*! Used to get value of calculated maximum demand Watts years.*/
/*! Used to get value of period minutes maximum demand W.*/
/*! Used to get value of calculate maximum demand Reverse.*/
/*! Used to get value of calculate maximum demand VAs.*/
/*! Used to get value of calculated maximum demand VAs minutes.*/
/*! Used to get value of calculated maximum demand VAs hours.*/
/*! Used to get value of calculated maximum demand VAs days.*/
/*! Used to get value of calculated maximum demand VAs month.*/
/*! Used to get value of calculated maximum demand VAs years.*/
/*! Used to get value of period minutes maximum demand VAs.*/
/*! Used to get value of calculate maximum demand VAs.*/
/*! Used to get value of calculated maximum demand VAs minutes.*/
/*! Used to get value of calculated maximum demand VAs hours.*/
/*! Used to get value of calculated maximum demand VAs days.*/
/*! Used to get value of calculated maximum demand VAs month.*/
/*! Used to get value of calculated maximum demand VAs years.*/
/*! Used to get value of period minutes maximum demand VAs.*/
/*! Used to get value of calculate maximum demand VAs.*/
/*! Used to get value of calculated maximum demand VAs minutes.*/
/*! Used to get value of calculated maximum demand VAs hours.*/
/*! Used to get value of calculated maximum demand VAs days.*/
/*! Used to get value of calculated maximum demand VAs month.*/
/*! Used to get value of calculated maximum demand VAs years.*/
/*! Used to get value of period minutes maximum demand VAs.*/
/*! Used to get value of calculate maximum demand amperes.*/
/*! Used to get value of calculated maximum demand amperes minutes.*/
/*! Used to get value of calculated maximum demand amperes hours.*/
/*! Used to get value of calculated maximum demand amperes days.*/
/*! Used to get value of calculated maximum demand amperes month.*/
/*! Used to get value of calculated maximum demand amperes years.*/
/*! Used to get value of period minutes maximum demand amperes.*/
/*! Used to get voltage in volts for 3 phase.*/
/*! Used to get value of calculate maximum demand amperes.*/
/*! Used to get value of calculated maximum demand amperes minutes.*/
/*! Used to get value of calculated maximum demand amperes hours.*/
/*! Used to get value of calculated maximum demand amperes days.*/
/*! Used to get value of calculated maximum demand amperes month.*/
/*! Used to get value of calculated maximum demand amperes years.*/
/*! Used to get value of period minutes maximum demand amperes.*/
/*! Used to get value of calculate maximum demand amperes.*/
/*! Used to get value of calculated maximum demand amperes minutes.*/
/*! Used to get value of calculated maximum demand amperes hours.*/
/*! Used to get value of calculated maximum demand amperes days.*/
/*! Used to get value of calculated maximum demand amperes month.*/
/*! Used to get value of calculated maximum demand amperes years.*/
/*! Used to get value of period minutes maximum demand amperes.*/
/*! Used to get voltage in volts for 3 phase.*/
/*! Used to get current in amperes for 3 phase.*/
#define MTR_GET_CUR(ch_id) mtr_data.inst_reg.current[ch_id]


#ifdef MTR_THREE_PH
/*! Used to get instant active power in watts.*/
#endif


/*! Used to get instant total active power in watts.*/


#ifdef MTR_THREE_PH
/*! Used to get instant reactive power in VARs.*/
#endif


/*! Used to get instant total reactive power in VARs.*/


#ifdef MTR_THREE_PH
/*! Used to get instant apparent power in VAs.*/
#endif


#if defined(MTR_THREE_PH) && defined(MTR_INDIRECT)
/*! Used to get the Resolution Factor*/
#endif


/*! Used to get instant total apparent power in VAs.*/
/*! Used to get instant power factor for 3 phase.*/
/*! Used to get instant total apparent power in VAs.*/
/*! Used to get instant total apparent power in VAs.*/
/*! Used to get instant frequency.*/


#ifdef MTR_THREE_PH
#else
#endif


/*! Flag of Right Arrow*/
/*! Flag of Left Arrow*/
/*! Flag of UP Arrow*/
/*! Flag of Down Arrow*/
/*! Flag to clear All Arrows*/
/*! Used to get left energy arrow status.*/
/*! Used to get right energy arrow status.*/
/*! Used to get up energy arrow status.*/
/*! Used to get down energy arrow status.*/


#ifdef MTR_THREE_PH
/*! Used to get phase 1 status.*/
/*! Used to get phase 2 status.*/
/*! Used to get phase 3 status.*/
#endif // MTR_THREE_PH


/*! Used to get active pulse counter.*/


#ifdef MTR_LOAD_PROFILE
/*! Used to get load profile record size.*/
#endif // MTR_LOAD_PROFILE


#ifdef MTR_ENABLE_LMT
/*! Used to get over voltage configuration.*/
/*! Used to get under voltage configuration.*/
/*! Used to get over load configuration.*/
#endif // MTR_ENABLE_LMT


/*!- Used to check the minimum threshold energy */
/*!- Used to check the Maximum threshold energy */



#ifdef MTR_SINGLE_PH
#else

#ifdef MTR_DIRECT
#else
#endif

#endif





// #ifdef MTR_DIRECT 
#define OVERFLOW_THERSHOLD (uint32_t)999999999
// #else
// #define OVERFLOW_THERSHOLD (uint32_t)99999999
// #endif
#ifdef ANSI_STACK
#define OVERFLOW_THERSHOLD (uint32_t)999999999
#endif



/*! - Get RMS current.*/
#ifdef ADE7953_ENABLE
#elif defined(V9261f_ENABLE)
#elif defined(V9360_ENABLE)
#elif defined(MTR_TWO_PH)
#elif defined(V9203_ENABLE)
#elif defined(EM_DL130_BOARD)
#elif defined(V9381_ENABLE)
#endif



#ifdef ANSI_FEATURE
#else
#endif

#ifdef MTR_INDIRECT
#else
#if defined(ANSI_FEATURE) && defined(MTR_TWO_PH)
#else
#endif
#endif


///////////////////////////////////////////////////////////////////////////////////////////
/*==========================================================================================

DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
Constants and Definitions
------------------------------------------------------------------------------------------*/

#ifdef TARIFF_SYS
/*! - Publish metering new consumption data*/
#endif
/* Preprocessors used to calculate the two feature bytes in the configuration*/
#ifdef  MTR_SINGLE_PH
#elif (defined MTR_THREE_PH )
#endif

#ifdef  MTR_REACTIVE
#else
#endif

#ifdef  MTR_RVS_TMPR
#else
#endif

#ifdef  MTR_ERTH_TMPR
#else
#endif

#ifdef  MTR_MIS_POT_TMPR
#else
#endif

#ifdef  MTR_LOAD_PROFILE
#else
#endif

#ifdef  MTR_ENABLE_LMT
#else
#endif

#ifdef  MTR_PH_SEQ_TMPR
#else
#endif


#if     defined(ADE7953_ENABLE)
#elif   defined(V9203_ENABLE)
#elif   defined(V9261f_ENABLE)
#ifdef MTR_SINGLE_PH
#else
#endif
#else
#endif

#if (MTR_NUM_OF_CH == 1)
#elif (MTR_NUM_OF_CH == 2)
#elif (MTR_NUM_OF_CH == 3)
#elif (MTR_NUM_OF_CH == 4)
#endif

#ifdef  MTR_INDIRECT
#else
#endif



#if ( defined (MTR_THREE_PH) && defined (EM330_BOARD) )
#else
#endif
#ifdef MTR_UNIT_TEST
#define MTR_DRV_CF_CFG                drv_CF_cfg_stub
#ifdef ADE7953_ENABLE
typedef enum
{
  ADE7953_CH_A = 0,
  ADE7953_CH_B = 1,
} ade7953_current_channel_t;
typedef enum
{
  ADE7953_ACTIVE_CAL = 0,
  ADE7953_REACTIVE_CAL = 1,
  ADE7953_APPARENT_CAL = 2,
  ADE7953_RMS_CAL = 3,
  ADE7953_PHASE_CAL = 4,
} ade7953_cal_cmd_t;
typedef enum
{
  ADE7953_GAIN_CAL = 0,
  ADE7953_OFFSET_CAL = 1,
} ade7953_cal_opt_t;
typedef enum
{
  ADE7953_INIT_ROM = 0,
  ADE7953_INIT_RAM = 1,
} ade7953_init_t;


#endif

#define  TRF_PYMT_EVNT_MD                        MD_EVNT_STUB
#else
#ifdef ADE7953_ENABLE
                
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get RMS current.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/
/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
#ifdef ADE7953_ENABLE
/*! - Get power factor.*/
#elif
#error "No power factor getter defined."
#endif
/*! - Get frequency.*/
/*! - Change CF pulse for a different channel.*/
#define MTR_DRV_CF_CFG(channel,scale)                ade7953_CF_cfg(channel,scale)
/*! - Calibrate the metering driver.*/
#if defined(ADE7953_GAIN_CAL_EN) || defined(ADE7953_OFST_CAL_EN)|| defined(ADE7953_RMS_CAL_EN) || defined(ADE7953_PHASE_CAL_EN)
#else
#endif
/*! - Metering driver main task.*/
#elif defined (V9261f_ENABLE)
                
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get RMS current.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/
/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
/*! - Get power factor.*/
/*! - Get frequency.*/
/*! - Change CF pulse for a different channel.*/
/*! - Calibrate the metering driver.*/

/*! - Metering driver main task.*/


#elif defined (V9360_ENABLE)
                
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get RMS current.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/
/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
/*! - Get power factor.*/
/*! - Get frequency.*/
/*! - Change CF pulse for a different channel.*/
/*! - Calibrate the metering driver.*/

/*! - Metering driver main task.*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#elif defined (MTR_TWO_PH)
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/

/*added section*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/

/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
/*! - Get power factor.*/
/*! - Get frequency.*/
/*! - Get frequency.*/
/*! - Get ADE calibration state.*/
/*! - Metering driver main task.*/
/*! check the phase sequance Tamper Status*/
/*! check the frequency variation*/
/*! check the earth Tamper Status*/
//#define MTR_ERTH_TMP()              ade7758_earth_tmp();
/*!- check active phases*/

/*!- check active phases*/
////#define MTR_DRV_CF_CFG(channel)                //ade7758_CF_cfg(channel)







//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#elif defined (V9203_ENABLE)
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/

/*added section*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/

/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
/*! - Get power factor.*/
/*! - Get frequency.*/
/*! - Get frequency.*/
/*! - Get ADE calibration state.*/
/*! - Metering driver main task.*/
/*! check the phase sequance Tamper Status*/
/*! check the frequency variation*/
/*! check the earth Tamper Status*/
//#define MTR_ERTH_TMP()              ade7758_earth_tmp();
/*!- check active phases*/

/*!- check active phases*/
////#define MTR_DRV_CF_CFG(channel)                //ade7758_CF_cfg(channel)





/* Configure_ Internal measuring chip V94XX*/




#elif defined (EM_DL130_BOARD)
                
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get RMS current.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/
/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
/*! - Get power factor.*/
/*! - Get frequency.*/
/*! - Change CF pulse for a different channel.*/
/*! - Calibrate the metering driver.*/

/*! - Metering driver main task.*/




#elif defined (V9381_ENABLE)
                
/*! - Initialize metering driver.*/
/*! - Get RMS voltage.*/
/*! - Get RMS current.*/
/*! - Get active power.*/
/*! - Get reactive power.*/
/*! - Get apparent power.*/
/*! - Get active energy.*/
/*! - Get reactive energy.*/
/*! - Get apparent energy.*/
/*! - Get power factor.*/
/*! - Get frequency.*/
/*! - Change CF pulse for a different channel.*/
/*! - Calibrate the metering driver.*/

/*! - Metering driver main task.*/


#endif









/*! - Publish metering MD data*/
#ifdef TRF_BP_HISTORY

/*! - Publish metering new consumption data*/
#endif

#if defined (V9203_ENABLE) || defined (ADE7953_ENABLE) || defined (V9261f_ENABLE) ||defined (V9360_ENABLE) || defined (V9381_ENABLE) /*edit by A_K*/
#else
#error "you didn't define v9203 and ade7953!, define one of this."
#endif
#if defined (V9203_ENABLE) && defined (ADE7953_ENABLE) && defined (V9261f_ENABLE)
#error "you define v9203 and ade7953!, just define one of this."
#endif

#ifndef MTR_TWO_PH

#ifdef ADE7953_ENABLE
/*! - Used to read number of half linecycles for accumulation period.*/
#elif defined (V9261f_ENABLE)
#elif defined (V9360_ENABLE)
#elif defined (V9381_ENABLE)
#endif

#endif

#ifdef TRF_USE_MD_KW
#endif
#ifdef  TRF_USE_MD_KVA
#endif
     
//////#define    MTR_GET_PRV_MD_A()         TRF_PYMT_GET_PRV_MD_A()   
//////////////#define    MTR_READ_PWR_FAIL_PIN()  (ctrl_flags & CTRL_PWR_DWN_MASK)

#ifdef V9203_ENABLE
/*! - Used to read number of half linecycles for accumulation period.*/
#endif
#endif
#ifdef MTR_ENABLE_LMT
/*!< Get over load status for a certain current channel.*/
/*!< Get over volt status for a certain voltage channel.*/
/*!< Get under volt status for a certain voltage channel.*/
#endif
#ifdef MTR_RVS_TMPR
/*!< Get reverse current status for a certain current channel.*/
#endif
#ifdef MTR_MIS_POT_TMPR
/*!< Get missing potential tamper status for a certain phase.*/
#endif
/*!< Calculate CRC for MTR data structure.*/
/*!< Calculate CRC for MTR configuration structure.*/
/*! - Save configuration in non volatile storage media.*/
/*! - Load configuration from non volatile storage media.*/
/*! - Save data in non volatile storage media.*/
#define MTR_SAVE_DATA()  file_sys_write((uint8_t*)&mtr_data.mtr_data_mem,FILE_SYS_GET_METERING_SYS_VAR, sizeof(mtr_data_mem_t))
// TODO: check where we need to store these var
/*! - Load Data from non volatile storage media.*/
// TODO: check where we need to load these var
/*! - Used to clear load profile log.*/
/*! - Used to read number of records in load profile log.*/
/*! - Used to save record in load profile log.*/
/*! - Used to read selected records from load profile log.*/
/*! - Used to read latest records from load profile log.*/
/*! - Used to read latest event records from event log.*/
/*! - Used to read latest event records from event log.*/
/*! - Check power status.*/
/*! - Check ADE communication error status.*/
/*! - Publish metering event.*/
/*! - Store event in EEPROM .*/
/*! - Set event Notification Flag.*/
/*!Check pulse to increment pulse counter*/
#ifdef PULSE_COUNT_ENABLE
#endif
/*! clear of event notification log1.*/
#ifdef GPRS_FEATURE
/*! - Set event Notification Flag for load profile.*/
#else
#endif




#ifdef ADE7953_ENABLE
#elif defined (V9261f_ENABLE)
#if !defined(EM110_BOARD) && !defined (EM330_BOARD)&& !defined(EM130_BOARD)  && !defined(EM210_BOARD)
#else
#endif
#endif
#ifdef ANSI_STACK
#else
#endif

#ifdef MTR_THREE_PH
#ifdef MTR_DIRECT
#else
#endif
#else
#endif

#ifndef ASHNTTI_PROJECT
#else
#endif
#ifdef MTR_MIS_POT_TMPR
#endif
#ifdef MTR_PH_SEQ_TMPR
#endif
#ifdef MTR_THREE_PH
#endif//_MTR_THREE_PH





/*------------------------------------------------------------------------------------
  from Meter_Interface.h
------------------------------------------------------------------------------------*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           M E T E R I N G    S Y S T E M

                              H E A D E R     F I L E

 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file mtr.h
 * @brief This file contains metering and limiter APIs prototype.
 * @details The header file for metering and limiter module\n
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
 *   <td> 27/12/2012 </td>
 *   <td> aibraheem  </td>
 *   <td> modify limiter calculation methods. </td>
 * </tr>
 * <tr>
 *   <td> 16/12/2012 </td>
 *   <td> aibraheem  </td>
 *   <td> modify metering data structure </td>
 * </tr>
 *   <td> 09/12/2012 </td>
 *   <td> aibraheem  </td>
 *   <td> Solve calibration bugs.</td>
 * </tr>
 * <tr>
 *   <td> 05/11/2012 </td>
 *   <td> aibraheem  </td>
 *   <td> Modify calibration mode state machine.</td>
 * </tr>
 * <tr>
 *   <td> 25/09/2012 </td>
 *   <td> aibraheem  </td>
 *   <td> Add load profile interfaces.</td>
 * </tr>
 * <tr>
 *   <td> 30/07/2012 </td>
 *   <td> aibraheem  </td>
 *   <td> Created  </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup METERING_SYS Metering System
 * @ingroup METER
 * Metering module provides functions for meter measurement and limits handling.\n
 * \n<b>References</b>\n
 * - metering_system.docx inside App folder in Metering folder.
 * @brief Responsible for managing metering driver and measurement limits.
 */
/*!
 * @defgroup MTR_CORE Application Core APIs
 * @ingroup METERING_SYS
 * @brief Metering system entry and initialization functions.
 */
/*!
 * @defgroup MTR_CFG Configuration APIs
 * @ingroup METERING_SYS
 * @brief Metering system configuration functions and data types.
 */
/*!
 * @defgroup MTR_READ Read System Variables APIs
 * @ingroup METERING_SYS
 * @brief Used to read recorded variables by metering system.
 */
/*!
 * @defgroup MTR_EVENT Events APIs
 * @ingroup METERING_SYS
 * @brief Metering events call back APIs.
 */
/*!
 * @defgroup MTR_CMD Commands APIs
 * @ingroup METERING_SYS
 * @brief Metering command interface APIs.
 */
/*!
 * @defgroup MTR_DRV Drivers
 * @ingroup METERING_SYS
 * @brief List of all supported metering drivers in this system.
 */
/*==========================================================================================

INCLUDE FILES FOR MODULE

==========================================================================================*/
/* #include "Meter_Config.h" -- inlined in this file */

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
/*!
 * @addtogroup MTR_CMD
 * @{
 */

/*!
 * @par Description:
 *   This enumerator contains all available commands.
 */
typedef enum
{
	MTR_RST_ACC      = 0,          /*!< Used to reset accumulation registers.*/
#ifdef MTR_RVS_TMPR
	MTR_RST_REV_ACC  = 1,          /*!< Used to reset reverse accumulation registers.*/
#endif//
#ifdef MTR_ERTH_TMPR
	MTR_RST_FLT_ACC  = 2,          /*!< Used to reset fault accumulation registers.*/
#endif//
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
	MTR_RST_TMP      = 3,          /*!< Used to reset tamper registers.*/
#endif//
#ifdef MTR_LOAD_PROFILE
	MTR_RST_LD_PRF   = 4,          /*!< Used to reset load profile.*/
#endif//
	MTR_RST_MD       = 5,          /*!< Used to reset maximum demand.*/
	MTR_STRT_MAN_CAL = 6,          /*!< Used to begin manual calibration application.*/
	MTR_NXT_MAN_CAL  = 7,          /*!< Used to go to next manual calibration step.*/
	MTR_STRT_CAL = 8,         /*!< Used to begin automatic calibration application.*/
	MTR_NXT_AUTO_CAL = 9,          /*!< Used to go to next automatic calibration step.*/
	MTR_STRT_PEN     = 10,         /*!< Used to begin penalty calculations.*/
	MTR_END_PEN      = 11,         /*!< Used to end penalty calculations.*/
	MTR_RST          = 12,         /*!< Used to reset all metering application.*/
	MTR_CFG_RST      = 13,          /*!< Used to reset metering application configuration.*/
	MTR_SAVE_DATA    = 14,         /*!< Used to save metering data.*/
	MTR_RST_MD_WITHOUT_LIFE_ACC = 15,
	MTR_RST_OVR_LD_COUNTERS=16,

	MTR_RST_IMP_ACTIVE_ACC=17,
	MTR_RST_EXP_ACTIVE_ACC=18,
	MTR_RST_IMP_REACTIVE_ACC=19,
	MTR_RST_EXP_REACTIVE_ACC=20,
	MTR_CFG_OVER_LOAD=21,
    MTR_RST_REV_TMP =22,
    MTR_NEW_MONTH=23,
} mtr_cmd_t;

/*!
 * @par Description:
 *    Structure used to save time stamp for measurements.
 */

__packed typedef struct
{
	uint8_t min;          /*!< Minutes(0-59).*/
	uint8_t hr;           /*!< Hours, 24 hour system(0-23).*/
	uint8_t day;          /*!< Days(1-31)*/
	uint8_t month;        /*!< Months(1-31)*/
	uint8_t year;         /*!< Years(0-99)*/
} mtr_date_time_t;

/*!
 * @par Description:
 *   This enumerator contains all available self test results.
 */
typedef enum
{
	MTR_TST_PASS = 0,   /*!< Pass self test result.*/
	MTR_TST_FAIL_DRV = 1,   /*!< Failure self test result due to driver.*/
	MTR_TST_FAIL_COMM = 2,   /*!< Failure self test result due to communication.*/
	MTR_TST_FAIL_VOLTAGE = 4, /*!< Failure self test result due to Voltage .*/
	MTR_TST_FAIL_CURRENT = 8, /*!< Failure self test result due to Current .*/
	MTR_TST_FAIL_PF = 16,        /*!< Failure self test result due to power factor .*/
        MTR_TST_FAIL_REV = 32,        /*!< Failure result due TO REVESE CH .*/
        //MTR_TEST_FAIL_SPI_COMM = 32; /*  Failure due to spi comm error*/
        //MTR_TEST_FAIL_PH_SEQ =64; /* Failure due to phase sequence error*/
	} tst_rslt;
/*!@}*/

/*!
 * @addtogroup MTR_READ
 * @{
 */
/*!
 * @par Description:
 *   This enumerator contains all available current channels in meter.
 */
typedef enum
{
	MTR_CH_A = 0,     /*!< Current channel A*/
#if(MTR_NUM_OF_CH >= 2)
	MTR_CH_B = 1,     /*!< Current channel B*/
#endif
#if(MTR_NUM_OF_CH >= 3)
	MTR_CH_C = 2,     /*!< Current channel C*/
#endif
#if(MTR_NUM_OF_CH ==4)
	MTR_CH_D = 3,     /*!< Current channel D*/
#endif
	MTR_CH_SIZE
} mtr_current_channel_t;
/*!@}*/

/*!
 * @addtogroup MTR_EVENT
 * @{
 */
/*!
 * @par Description:
 *   This enumerator contains all available current channels in meter.
 */
typedef enum
{
	MTR_NEW_ENERGY = 0,     /*!< New energy event.*/
#if defined (ADE7953_ENABLE) ||defined(V9261f_ENABLE) || defined (V9360_ENABLE) || defined (V9381_ENABLE)
	MTR_DATA_CRPT = 1,      /*!< Data corrupted event.*/
	MTR_COMM_ERR = 2,       /*!< Communication error event.*/
	MTR_COMM_GOOD = 3,      /*!< Communication success event.*/
#endif
	MTR_NEW_PLS = 4,        /*!< Driver new pulse event.*/
#ifdef V9203_ENABLE
	MTR_SEQ_ERR = 5,        /*!< Driver sequence error event.*/
	MTR_IDLE_ERR = 6,       /*!< Driver idle driver event.*/
	MTR_EARTH_TAMP = 7      /*!< Driver earth detection event.*/
#endif
} mtr_drv_event_t;


/*!
 * @par Description:
 *   This enumerator contains all available power down options.
 */
typedef enum
{
	MTR_WITH_SAVE      = 0,     /*!< Used to save in power down command.*/
	MTR_WITHOUT_SAVE   = 1    /*!< Used to enter power down without saving.*/
} mtr_save_opt_t;

/*!@}*/

/*!
 * @addtogroup MTR_READ
 * @{
 */
/*!
 * @par Description:
 *   This structure used to save accumulative value in tow higher and lower registers.
 */

__packed typedef struct
{
	uint32_t  low_reg;       /*!< Used to define the lower value of register.*/
	uint16_t  high_reg;      /*!< Used to define the higher value of register.*/
} acc_reg_t;



/*!
 * @par Description:
 *   This structure contains new consumption energy remainder registers.
 */

__packed typedef struct
{
	uint32_t mic_act_en;
	uint32_t mic_react_en;
	uint32_t mic_app_en;
} mtr_consum_mic_reg_t;

#if defined(MTR_THREE_PH) && defined(MTR_INDIRECT)
__packed typedef struct
{
	uint32 active[3];
	uint32 reactive[3];
	uint32 apparent[3];
} mtr_cons_resolution_reg_t;
#endif
typedef enum
{
	QUADRANT_1,
	QUADRANT_2,
	QUADRANT_3,
	QUADRANT_4,
	NO_OF_QUADRANTS=4,
}quadrant_t;
enum
{
	QUADRANT_1_INDEX=0X00,
	QUADRANT_2_INDEX=0X01,
	QUADRANT_3_INDEX=0X09,
	QUADRANT_4_INDEX=0X08,
};



__packed typedef struct
{
	acc_reg_t 	quadrant_act_energy[NO_OF_QUADRANTS][3];		/*!< Used to get accumulative active energy in WH for every channel per quadrant .*/
	acc_reg_t 	quadrant_react_energy[NO_OF_QUADRANTS][3];	/*!< Used to get accumulative reactive energy in VARH for every channel per quadrant*/
	acc_reg_t 	quadrant_app_energy[NO_OF_QUADRANTS][3];		/*!< Used to get accumulative apparent energy in VAH for every channel per quadrant .*/
} mtr_quadrant_energy_data_t;
/*!
 * @par Description:
 *   This structure contains accumulative energy registers.
 */

__packed typedef struct
{
#ifdef MTR_THREE_PH
	acc_reg_t   act_energy[3];         /*!< Used to get accumulative active energy in WH for every channel.*/
#endif//MTR_THREE_PH
	acc_reg_t   tot_act_en;            /*!< Used to get accumulative active energy in WH for all channels.*/
#ifdef MTR_REACTIVE
#if (defined (MTR_THREE_PH))      
	acc_reg_t   react_energy[3];       /*!< Used to get accumulative reactive energy in VARH for every channel.*/
#endif//MTR_THREE_PH
	acc_reg_t   tot_react_en;          /*!< Used to get accumulative reactive energy in VARH for all channels.*/
  //  acc_reg_t	react_quad_en[4];		   /*!< Used to get accumulative reactive energy in VARH for all channels.*/
#if (defined (MTR_THREE_PH)) 
	acc_reg_t   app_energy[3];         /*!< Used to get accumulative apparent energy in VAH for every channel.*/
#endif//MTR_THREE_PH
	acc_reg_t   tot_app_en;            /*!< Used to get accumulative apparent energy in VAH for all channels.*/
#endif//MTR_REACTIVE
#ifdef MTR_RVS_TMPR
#ifdef MTR_THREE_PH
	acc_reg_t   rev_act_en[3];         /*!< Used to get accumulative reverse active energy in WH for every channel.*/
#endif//MTR_THREE_PH
	acc_reg_t	tot_rev_act_en; 	   /*!< Used to get accumulative reverse active energy in WH for all channels.*/
#ifdef MTR_REACTIVE
#ifdef MTR_THREE_PH
	acc_reg_t   rev_react_en[3];       /*!< Used to get accumulative reverse reactive energy in VARH for every channel.*/
#endif//
	acc_reg_t   tot_rev_react_en;      /*!< Used to get accumulative reverse reactive energy in VARH for all channels.*/
#endif//MTR_REACTIVE
#endif//MTR_RVS_TMPR
#ifdef  MTR_ERTH_TMPR
	acc_reg_t   tot_flt_en;            /*!< Used to get accumulative fault energy in WH for all channels.*/
#endif//MTR_RVS_TMPR
#if defined(MTR_THREE_PH) && defined(MTR_INDIRECT)
	mtr_cons_resolution_reg_t resolution_fraction  ;         /*!< Used to get accumulative active energy in WH for every channel.*/
#endif//MTR_THREE_PH
#ifdef MTR_REACTIVE
	mtr_quadrant_energy_data_t   quadrant_data;
#endif
#if defined(MTR_THREE_PH)||defined(ANSI_STACK)||defined(Reactive_PWR_V94XX_Enable)
        
	acc_reg_t   tot_current_year_act_en;            /*!< Used to get current year accumulative active energy in WH for all channels.*/
	acc_reg_t   tot_current_year_app_en;            /*!< Used to get current year accumulative apparent energy in VAH for all channels.*/
        
	acc_reg_t   tot_prv_year_act_en;            /*!< Used to get previous year accumulative active energy in WH for all channels.*/
	acc_reg_t   tot_prv_year_app_en;            /*!< Used to get previous year accumulative apparent energy in VAH for all channels.*/

#endif 
#if (defined (MTR_SINGLE_PH) && Reactive_PWR_V94XX_Enable) 
        acc_reg_t   react_energy[3]; 
        acc_reg_t   app_energy[3]; 
#endif
} acc_en_reg_t;
  
__packed typedef struct

{
	acc_reg_t   tot_act_en;            /*!< Used to get accumulative active energy in WH for all channels.*/
	acc_reg_t   tot_react_en;          /*!< Used to get accumulative reactive energy in VARH for all channels.*/
	acc_reg_t   tot_app_en;            /*!< Used to get accumulative apparent energy in VAH for all channels.*/
	acc_reg_t   tot_rev_act_en;        /*!< Used to get accumulative reverse active energy in WH for all channels.*/
	acc_reg_t   tot_rev_react_en;      /*!< Used to get accumulative reverse reactive energy in VARH for all channels.*/
	acc_reg_t   tot_flt_en;            /*!< Used to get accumulative fault energy in WH for all channels.*/
} sp_acc_en_reg_t;

/*!
 * @par Description:
 *   This structure contains the remainder micro of accumulative energy registers.
 */
 
__packed typedef struct
{
#if 1//def MTR_THREE_PH
	uint32_t   mic_rem_act_energy[MTR_NUM_OF_PHASE];    /*!< Used to define the remainder micro of accumulative active energy in WH for every channel.*/
#endif//
	uint32_t   mic_rem_tot_act_en;       /*!< Used to define the remainder micro of accumulative active energy in WH for all channels.*/
#if 1//def MTR_THREE_PH
	uint32_t   mic_rem_react_energy[MTR_NUM_OF_PHASE];    /*!< Used to define the remainder micro of accumulative reactive energy in VARH for every channel.*/
#endif//
	uint32_t   mic_rem_tot_react_en;     /*!< Used to define the remainder micro of accumulative reactive energy in VARH for all channels.*/

#if 1//def MTR_THREE_PH
	uint32_t   mic_rem_app_energy[MTR_NUM_OF_PHASE];    /*!< Used to define the remainder micro of accumulative apparent energy in VAH for every channel.*/
#endif//
	uint32_t   mic_rem_tot_app_en;       /*!< Used to define the remainder micro of accumulative apparent energy in VAH for all channels.*/
#ifdef MTR_RVS_TMPR
#if 1//def MTR_THREE_PH
	uint32_t   mic_rem_rev_act_en[MTR_NUM_OF_PHASE];    /*!< Used to define the remainder micro of accumulative reverse reactive energy in VARH for every channel.*/
#endif//
	uint32_t   mic_rem_tot_rev_act_en;     /*!< Used to define the remainder micro of accumulative reverse active energy in WH for all channels.*/
#if 1//def MTR_THREE_PH
    uint32_t   mic_rem_rev_react_en[MTR_NUM_OF_PHASE];    /*!< Used to define the remainder micro of accumulative reverse active energy in WH for every channel.*/
#endif//
	uint32_t   mic_rem_tot_rev_react_en;   /*!< Used to define the remainder micro of accumulative reverse reactive energy in VARH for all channels.*/
#ifdef MTR_ERTH_TMPR
	uint32_t   mic_rem_tot_flt_en;       /*!< Used to define the remainder micro of accumulative fault energy in WH for all channels.*/
#endif//
#endif//
#if 1//def MTR_THREE_PH
	uint32_t 	mic_rem_quadrant_act_energy[NO_OF_QUADRANTS][MTR_NUM_OF_PHASE];		/*!< Used to get accumulative active energy in WH for every channel per quadrant .*/
	uint32_t 	mic_rem_quadrant_react_energy[NO_OF_QUADRANTS][MTR_NUM_OF_PHASE];	/*!< Used to get accumulative reactive energy in VARH for every channel per quadrant*/
	uint32_t 	mic_rem_quadrant_app_energy[NO_OF_QUADRANTS][MTR_NUM_OF_PHASE];		/*!< Used to get accumulative apparent energy in VAH for every channel per quadrant .*/
#endif//MTR_THREE_PH
    uint32_t   mic_rem_tot_current_year_act_en;            /*!< Used to get current year accumulative active energy in WH for all channels.*/
	uint32_t   mic_rem_tot_current_year_app_en;            /*!< Used to get current year accumulative apparent energy in VAH for all channels.*/
#ifdef DLMS_ENABLED//redatodo
	uint32_t   mic_rem_tot_mda;            
	uint32_t   mic_rem_tot_mdw;
#endif

} mic_rem_reg_t;

__packed typedef struct
{
        uint32_t   mic_rem_tot_fwd_act_en;     /*!< Used to define the remainder micro of accumulative reverse active energy in WH for all channels.*/
	uint32_t   mic_rem_tot_gen_act_en;     /*!< Used to define the remainder micro of accumulative reverse active energy in WH for all channels.*/	
#ifdef MTR_THREE_PH       
        uint32_t   mic_rem_fwd_act_en[MTR_NUM_OF_PHASE];     /*!< Used to define the remainder micro of accumulative reverse active energy in WH for all channels.*/
        uint32_t   mic_rem_gen_act_en[MTR_NUM_OF_PHASE];    /*!< Used to define the remainder micro of accumulative reverse active energy in WH for every channel.*/
#endif//MTR_THREE_PH 
} ansi_mic_rem_reg_t;

/*!
 * @par Description:
 *   This structure contains accumulative energy registers and its remainder micro.
 */
 
__packed typedef struct
{
	acc_en_reg_t  acc_en_reg;
	mic_rem_reg_t mic_rem_reg;
} acc_mic_en_reg_t;

/*!
 * @par Description:
 *   This structure contains maximum demand registers.
 */ 
__packed typedef struct
{
	uint32_t        mdw_value;           /*!< Used to save value maximum demand watts.*/
#if 0
	uint32_t		mdw_fwd_value;			 /*!< Used to save value maximum demand watts.*/
	uint32_t		mdw_rev_value;			 /*!< Used to save value maximum demand watts.*/
#endif
	mtr_date_time_t mdw_tim;             /*!< Used to save time stamp for maximum demand watts.*/
	uint16_t        mdw_period;          /*!< Used to save period minutes for maximum demand watts.*/
#if 0
	uint32_t        mdvar_value;          /*!< Used to save maximum demand vas.*/
	mtr_date_time_t mdvar_tim;            /*!< Used to save time stamp for maximum demand vas.*/
	uint16_t        mdvar_period;         /*!< Used to save period minutes for maximum demand vas.*/
#endif
#ifdef MTR_REACTIVE
    uint32_t        mdva_value;          /*!< Used to save maximum demand vas.*/
    mtr_date_time_t mdva_tim;            /*!< Used to save time stamp for maximum demand vas.*/
    uint16_t        mdva_period;         /*!< Used to save period minutes for maximum demand vas.*/
#endif//
    uint32_t        mda_value;           /*!< Used to save maximum demand amperes.*/
    mtr_date_time_t mda_tim;             /*!< Used to save time stamp for maximum demand amperes.*/
    uint16_t        mda_period;          /*!< Used to save period minutes for maximum demand amperes.*/
#ifdef DLMS_ENABLED//redatodo
    acc_reg_t    tot_mda;  
    acc_reg_t    tot_mdw; 
    uint8_t      silding_num;         // start time of silding window
#endif
} md_reg_t;

/*!
 * @par Description:
 *   This structure contains temporary maximum demand registers without times.
 */
 
__packed typedef struct
{
	uint32_t        mdw_val;           /*!< Used to save value maximum demand watts.*/
	uint32_t        mdva_val;          /*!< Used to save maximum demand vas.*/
	uint32_t        mda_val;           /*!< Used to save maximum demand amperes.*/
} md_reg_temp;


/*!
 * @par Description:
 *   This structure contains instantaneous measurements registers.
 */  
__packed typedef struct
{
	uint32_t   voltage[MTR_NUM_OF_PHASE];             /*!< Used to save voltage for 3 phase.*/
#ifdef NEUTRAL_FEATURE
	uint32_t   current[MTR_NUM_OF_PHASE+1];             /*!< Used to save current for 3 phase.*/
#else
#ifdef ANSI_STACK
  	uint32_t   current[MTR_NUM_OF_PHASE];             /*!< Used to save current for 3 phase.*/      
#else
  	uint32_t   current[MTR_NUM_OF_PHASE+1];             /*!< Used to save current for 3 phase.*/  
#endif // ANSI_STACK
#endif
	uint32_t     power_factor[MTR_NUM_OF_PHASE];        /*!< Used to save power factor for 3 phase .*/
#ifdef MTR_THREE_PH
	uint32_t   inst_act_power[MTR_NUM_OF_PHASE];      /*!< Used to define instantaneous active power.*/
#endif
	uint32_t   inst_tot_act_power;                    /*!< Used to define total instantaneous active power.*/
#if defined (MTR_THREE_PH) || (Reactive_PWR_V94XX_Enable == 1 )
	uint32_t   inst_react_power[MTR_NUM_OF_PHASE];    /*!< Used to define instantaneous reactive power.*/
	uint32_t   inst_tot_react_power;                  /*!< Used to define total instantaneous reactive power.*/
#endif
#ifdef MTR_THREE_PH
	uint32_t   inst_app_power[MTR_NUM_OF_PHASE];      /*!< Used to define instantaneous apparent power.*/
#endif
	uint32_t   inst_tot_app_power;                    /*!< Used to define total instantaneous apparent power.*/

	uint32_t   inst_freq[MTR_NUM_OF_PHASE];                             /*!< Used to define instantaneous voltage channel frequency.*/
	uint32_t   act_pls_cntr;                          /*!< Used to count number of active pulses.*/
	uint8_t    load_qrt;                              /*!< Used to describe the quarter between current and voltage.*/
#if (defined (MTR_THREE_PH) || Reactive_PWR_V94XX_Enable)      
	uint8_t    active_phases;                         /*!< Used to show the active phases.*/
	uint8_t     avg_current_power_factor;        	/*!< Used to save current year average power factor power factor for 3 phase  .*/
	uint8_t     avg_prv_power_factor;        		/*!< Used to save previous year average power factor for 3 phase .*/
#endif
       
} inst_reg_t;

/*!
 * @par Description:
 *   This structure contains all available metering accumulation and maximum demand registers.
 */  
__packed typedef struct
{
	acc_mic_en_reg_t acc_mic_en_reg ;          /*!< Used to save accumulative energy registers.*/
	md_reg_t md_reg;                           /*!< Used to save maximum demand registers.*/
	md_reg_t md_reg_cur_year;                           /*!< Used to save maximum demand registers.*/
	md_reg_t md_reg_prv_year;                           /*!< Used to save maximum demand registers.*/

} mtr_acc_md_t;

/*!
 * @par Description:
 *   This structure contains all available metering data variables that need to be saved into none volatile memory.
 */
  
__packed typedef struct
{
  mtr_acc_md_t  mtr_acc_md;                  /*!< Used to get accumulation and maximum demand registers.*/
#ifdef TRF_BP_HISTORY
  mtr_consum_mic_reg_t mtr_consum_mic_reg;   /*!< Used to save the remainder micro consumption registers for tarrife modules.*/
#endif
  uint16_t event_reg;                         /*! Used to save a copy from metering application event flag register.*/
  uint8_t  curr_ch;                          /*! Used to save the current channel which work on.*/
  uint8_t  mtr_data_crc;                     /*!< Used to save a crc for saved data.*/
} mtr_data_mem_t;

/*!
 * @par Description:
 *   This structure contains all available metering and limiter data variables.
 */ 
__packed typedef struct
{
	mtr_data_mem_t mtr_data_mem;               /*!< Used to save into None volatile memory.*/
	inst_reg_t inst_reg;                       /*!< Used to get instantaneous measurements registers.*/
} mtr_data_t;
#ifdef  MTR_LOAD_PROFILE
/*!
 * @par Description:
 *   This structure contains load profile record.
 */
#ifndef ASHNTTI_PROJECT 
__packed typedef struct
{
#ifdef MTR_SINGLE_PH
  uint32_t   tot_cons;  
#else
  uint32_t   total_cons_active_l; // todo
  uint16_t   total_cons_active_h; // todo
  uint32_t   total_cons_reactive_l;// todo
  uint16_t   total_cons_reactive_h; // todo
#endif
  uint16_t   current[MTR_NUM_OF_PHASE];               /*!< Used to save current per phase in A*10^-2.*/
  uint16_t   volt[MTR_NUM_OF_PHASE];                  /*!< Used to save voltage per phase in V.*/
  int8_t     pwr_fact[MTR_NUM_OF_PHASE];              /*!< Used to save power factor per phase * 10^-2.*/
  mtr_date_time_t time_stamp;       /*!< Used to save time stamp for load profile record.*/
} lp_rec_t;

#else // Ashanti

#ifdef MTR_SINGLE_PH
__packed typedef struct
{
#ifdef GHANA_NEW_MODIFICATION
  uint8_t   current;               /*!< Used to save current of phase in A*10^-2.*/
#else
  uint16_t   current;               /*!< Used to save current of phase in A*10^-2.*/
#endif
  uint16_t   volt;                  /*!< Used to save voltage of phase channel by V.*/
  int8_t     pwr_fact;              /*!< Used to save power factor of phase channel * 10^-2.*/
#ifdef GHANA_NEW_MODIFICATION
  uint16_t   total_cons_active_l;
  uint8_t   total_cons_active_h;
  uint8_t   MD_current;
#endif
  mtr_date_time_t time_stamp;       /*!< Used to save time stamp for load profile record.*/
} lp_rec_t;

#else

__packed typedef struct
{
	uint16_t   current[MTR_NUM_OF_PHASE];               /*!< Used to save current per phase in A*10^-2.*/
	uint16_t   volt[MTR_NUM_OF_PHASE];                  /*!< Used to save voltage per phase in V.*/
	int8_t     pwr_fact[MTR_NUM_OF_PHASE];              /*!< Used to save power factor per phase * 10^-2.*/
#ifndef MTR_INDIRECT       
        uint32_t   total_cons_active_l;
        uint16_t   total_cons_active_h;
        uint32_t   total_cons_reactive_l;
        uint16_t   total_cons_reactive_h;
#endif
	mtr_date_time_t time_stamp;       /*!< Used to save time stamp for load profile record.*/
} lp_rec_t;
#endif
#endif

#endif
/*!@}*/

/*!
 * @addtogroup MTR_CFG
 * @{
 */
#if defined(MTR_INDIRECT) || defined(ANSI_STACK)
/*!
 * @par Description:
 * This structure contains all available limiter configurations.
 */ 
__packed typedef struct
{
	uint16_t ct_numerator;            /*!< Used to define CT numerator ratio.*/
	uint8_t  ct_denominator;            /*!< Used to define CT denominator ratio.*/
	uint16_t pt_numerator;            /*!< Used to define PT numerator ratio.*/
	uint8_t  pt_denominator;            /*!< Used to define PT denominator ratio.*/
	uint8_t registers_resolution;   /*<Used to define all registers resolution >*/
} mtr_ct_pt_cfg_t;
#endif
/*!
 * @par Description:
 * This structure contains all available metering and limiter configurations.
 */
 
__packed typedef struct
{
	uint8_t  mda_time;                          /*!< Used to define time in minutes to calculate maximum demand amperes.*/
	uint8_t  mdw_time;                          /*!< Used to define time in minutes to calculate maximum demand Kwatts.*/
	//uint16_t mdw_max_lmt;                       /*!< Used to define time in maximum demand limit which if this limit is reached certin action will be taken.*/
	//uint8_t  mdw_subinterval_num;               /*!< Used to define number of sub intervals in maximum demand period.*/
} mtr_md_cfg_t;

#ifdef MTR_ENABLE_LMT
/*!
 * @par Description:
 * This structure contains all available limiter configurations.
 */
 
__packed typedef struct
{
	uint16_t over_voltage;                      /*!< Used to define maximum of over voltage.*/
#ifdef ANSI_STACK
          uint16_t over_voltage_offset;               /*!< Used to define maximum of over voltage offset.*/
#endif    
	uint8_t  under_voltage;                     /*!< Used to define maximum under voltage.*/
#ifdef ANSI_STACK          
          uint8_t  under_voltage_offset;              /*!< Used to define maximum under voltage.*/
#endif    
	uint8_t  over_load;                         /*!< Used to define maximum of over load current.*/
	uint8_t  ovr_cur_time;                      /*!< Used to define persistence time in minutes to guarantee that over current limit is persisting.*/
	uint8_t  ovr_vlt_time;                      /*!< Used to define persistence time in minutes to guarantee that over volt limit is persisting.*/
	uint8_t  und_vlt_time;                      /*!< Used to define persistence time in minutes to guarantee that under volt limit is persisting.*/
} mtr_lmt_cfg_t;
#endif
#if defined(MTR_RVS_TMPR) || defined(V9203_ENABLE) || defined(MTR_MIS_POT_TMPR) || defined(MTR_PH_SEQ_TMPR)
/*!
 * @par Description:
 * This structure contains all available metering tampers configurations.
 */
 
__packed typedef struct
{
#ifdef MTR_RVS_TMPR
	uint8_t  rev_tmp_time;            /*!< Used to define persistence time in seconds to guarantee that reverse tamper is persisting.*/
#endif
#ifdef MTR_ERTH_TMPR
	uint8_t  lmt_mrgn_ratio;          /*!< Used to define ratio of allowable current threshold between two current channels as a ratio from measured current.*/
	uint8_t  flt_tmp_time;            /*!< Used to define persistence time in seconds to guarantee that fault tamper is persisting.*/
#endif
#ifdef MTR_MIS_POT_TMPR
	uint8_t  pot_tmp_time;            /*!< Used to define persistence time in seconds to guarantee that missing potential tamper is persisting.*/
#endif
#ifdef MTR_PH_SEQ_TMPR
	uint8_t  seq_tmp_time;            /*!< Used to define persistence time in seconds to guarantee that phase sequence tamper is persisting.*/
#endif
} mtr_tmp_cfg_t;
#endif
/*!
 * @par Description:
 * This structure contains all available metering and limiter configurations.
 */
  
__packed typedef struct
{
	uint16_t nominal_volt ;             /*!< Used to define meter operating voltage in volts * 10.*/
	uint16_t pnlty_current;             /*!< Used to define the value of current for penalty mode calculation in amperes.*/
} mtr_opr_cfg_t;
/*!
 * @brief Load limit configuration.
 */ 
__packed typedef struct
{
	uint8_t num;            /*!< Number of used periods valid values 1 to 10.*/
	uint8_t type;           /*!< #unused .*/

	uint16_t steps_min[9]; /*!< Used to define the upper boundary of the steps.
	 * Value is by minutes.
	 * @note
	 * No of defined boundaries = No of used steps-1.
	 */
	uint32_t threshold[10];      /*!< The threshold of each step in milliampar.*/
}load_limit_t;
/*!
 * @brief time of daily update for load profile .
 */

/*!
 * @par Description:
 *   This structure contains all available metering and limiter configurations.
 */
  
__packed typedef struct
{
	mtr_md_cfg_t mtr_md_cfg;//2
#ifdef MTR_ENABLE_LMT
	mtr_lmt_cfg_t mtr_lmt_cfg;//10
#endif
#if defined(MTR_RVS_TMPR) || defined(V9203_ENABLE) || defined(MTR_MIS_POT_TMPR) || defined(MTR_PH_SEQ_TMPR)
	mtr_tmp_cfg_t mtr_tmp_cfg;//5
#endif
	mtr_opr_cfg_t mtr_opr_cfg;//4
#if defined(MTR_INDIRECT) || defined(ANSI_STACK)
	mtr_ct_pt_cfg_t mtr_ct_pt_cfg;
#endif
#ifdef LOAD_LIMIT_PLAN
	load_limit_t load_limit;
#endif
	uint8_t mtr_lp_timeout;
	uint8_t  mtr_cfg_crc;                     /*!< Used to save a crc for saved configuration.*/
} mtr_cfg_t;

/*!
 * @par Description:
 *   This enumerator contains all available configuration types.
 */
typedef enum
{
	MTR_MD_CFG      = 0,     /*!< Used to configure MD.*/
#ifdef MTR_ENABLE_LMT
	MTR_LMT_CFG     = 1,     /*!< Used to configure limiter.*/
#endif
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
	MTR_TMP_CFG     = 2,     /*!< Used to configure tampers.*/
#endif
	MTR_OPR_CFG     = 3,     /*!< Used to configure operating point.*/
#ifdef MTR_INDIRECT
	MTR_CT_PT_CFG = 4,       /*!< Used to configure CT and PT ratio.*/
#endif
#ifdef LOAD_LIMIT_PLAN
   MTR_LOAD_LIMIT_CFG   =5,

#endif
	MTR_LP_CFG      = 6,     /*!< Used to configure load profile.*/

	MTR_OVER_LOAD_CFG      = 7,     /*!< Used to configure load profile.*/

	MTR_CFG_SIZE
} mtr_cfg_type_t;
typedef enum
{
	INDIRECT_CURRENT_PARAMETER,
	INDIRECT_VOLTAGE_PARAMETER,
	INDIRECT_POWER_ENERGY_PARAMETER,

}indirect_parameter_t;
enum relay_state
{
  MTR_CLOSE_RELAY , 
  MTR_OPEN_RELAY , 
};

/////////////////////////////////////////////////////////////////////////////////////////////


/*!@}*/
/*------------------------------------------------------------------------------------------
External Variables
------------------------------------------------------------------------------------------*/

extern mtr_data_t mtr_data; /*! Used to save metering data, only accessible using available macros.*/
extern mtr_cfg_t mtr_cfg; /*! Used to save metering configurations.*/
extern volatile uint8_t sample_ready;
extern volatile uint16_t seconds_mdw; 
extern volatile uint16_t seconds_wasSaved_2;
extern uint8_t is_saved   ;
extern uint8_t is_saved_2   ;
//extern int32_t act_power1[3];//saber


extern uint16_t cal_reg_flag;
extern uint8_t Phase_Flag;
extern uint8_t PHDOWN_chk_event;
extern uint8_t mtr_init_flag;
extern uint32_t  test_accuracy_variable_second ,test_accuracy_variable_nw_consumption ;  // new self test
#ifdef MTR_TWO_PH
extern uint32_t Two_Phase_Neutral_Mismatch ;
#else
extern uint32_t V9203_Neutral_Mismatch ;
#endif
extern volatile uint16_t seconds_wasSaved ; 
extern uint8_t go_down_grater100v_flag;
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/


uint8_t mtr_task(void);
void mtr_init(void);
uint8_t mtr_config_set(uint8_t arg1, uint8_t * arg2);
uint8_t mtr_config_get(uint8_t arg1, uint8_t * arg2);
uint8_t mtr_get_feature(uint8_t * arg1);
uint16_t mtr_get_acc_reg(uint8_t* arg1);
uint8_t mtr_get_md_reg(uint8_t* arg1);
uint8_t mtr_get_inst_reg(uint8_t* arg1);
uint8_t mtr_get_qrt(void);
uint8_t mtr_get_act_ph(void);

#ifdef MTR_LOAD_PROFILE
uint32_t mtr_get_lp_num(void);
uint16_t mtr_get_lp(uint16_t arg1, uint8_t* arg2, uint16_t arg3);
#endif

uint8_t mtr_cal_state(void);
void mtr_drv_event(uint8_t arg1);
void mtr_new_sec(void);
void mtr_pwr_up(void);
void mtr_pwr_dwn(uint8_t arg1);
void mtr_cal_rx(void);
void mtr_cmd(uint8_t arg1);
uint8_t mtr_self_test(void);
void mtr_chk_tmp(void);
void mtr_apply_resolution_factor(uint32_t* , uint32_t*);
uint32_t mtr_get_fault_energy();
void mtr_set_resolution_factor(void);
void mtr_add_Resolution_Factor(void);
uint32_t mtr_get_resolution_factor(void);
uint32_t mtr_add_indirect_parameter(uint32_t ,indirect_parameter_t);
void mtr_pulse_count(void);


#ifdef LOAD_LIMIT_PLAN
void mtr_load_limit_plan(void);
#endif


uint8_t mtr_self_test(void);
void mtr_clear_total_accuracy_test_active_cons(void);
void mtr_start_accuracy_test(void);
uint8_t mtr_config_set_test_time_consumption(uint8_t * arg ); // new self test
void mtr_reset_overload(void);
void mtr_set_TestMode();
void mtr_cf_enable();
void mtr_cf_disable();
void Meter_Reset_ADE_Driver_Timeout(void);
void mtr_read_en_reg(void);
void mtr_read_inst(void);
uint32_t mtr_get_total_accuracy_test_active_cons(void);
uint32_t mtr_get_total_accuracy_test_time_minute(void);
uint8_t Check_Channels_reverse(void);
uint8_t mtr_get_current_channel(void);
uint8_t mtr_get_mtr_start_accuracy_flag(void);  // new self test

#ifdef V9360_ENABLE
void mtr_V9360_chip_check(void);
#elif defined (V9261f_ENABLE)
void mtr_V9261f_chip_check(void);
#elif defined (MTR_TWO_PH)
void mtr_Two_Phase_chip_check(void);
#elif defined (V9381_ENABLE)
void mtr_V9381_chip_check(void);
#endif

void mtr_load_backup(void);
void mtr_set_first_variables (void);
void mtr_rst_mtr_start_accuracy_flag(void);


void set_accuracy_test_flag(void);
uint16_t Get_calib_Flag(void);

#ifdef ANSI_STACK
void  MTR_cfg_adjustframefrom_SW(uint8_t * arg2);
#endif
void mtr_cfg_save_power_up (void);
void reset_all_trf_md_val(void);


/*------------------------------------------------------------------------------------
  from Power_modes.h
------------------------------------------------------------------------------------*/


#include "config.h"
#if (MicroController == Micro_V94XX)

#include "V94XX_PMU.h"
#include "V94XX_ANA.h"
#include "V94XX_CORTEX.h"

#else

#include "v85xx_pmu.h"
#include "v85xx_ana.h"
#include "v85xx_cortex.h"

#endif

/* #include "basic_srv.h" -- inlined in this file */
#include "core_cm0.h"


#define VDCIN_PWR_DOWN          ((uint8_t)0x01)
#define VDDALARM_PWR_DOWN       ((uint8_t)0x02)

#define EnterLPM(init_struct,MODE)   PMU_EnterSleep_LowPower(init_struct , MODE)

extern uint8_t PWR_down_detect;

#if 0
void VDDALARM_Config(void);
void VDCIN_Config(void);
void AUTOSWITCH_Config(void);
void AUTO_DeepSleep_WAKEUP_config(void);
void VDDAlarm_init(void);
#endif

void Pwr_ctrl_config(void);
void LPM_Config(PMU_LowPWRTypeDef* LowPower_InitStruct,uint8_t lcd_enabled);
void TELT_WakeUpConfig(void);

/*------------------------------------------------------------------------------------
  from Special_Days.h
------------------------------------------------------------------------------------*/
/*
 * Special_Days.h
 *
 *  Created on: Dec 14, 2016
 *      Author: mahmoud.mohsen
 */

#include "config.h"
/* #include "file_sys.h" -- inlined in this file */
//#include "flash_app.h"
#ifdef SPECIAL_DAYS_ENABLE
#ifdef NEW_DESIGN_OPTIMIZE
#define SPECIAL_DAYS_MAX_RECORDS            (1)             /* The maximum special days configuration number */
#else
#define SPECIAL_DAYS_MAX_RECORDS            (20)             /* The maximum special days configuration number */
#endif


typedef struct
__attribute__((__packed__))
{
  TU16    index;      /* Special day index */
  TstDate date;       /* Special day date format as in 4.1.6.1 in Blue Book*/
  TU08    day_id;     /* Day_Id, 1--9*/
} TstSDaysRcd;

typedef struct
__attribute__((__packed__))
{
  TstSDaysRcd RecordTable [SPECIAL_DAYS_MAX_RECORDS];//20*10
  TU16 SpecialDaysRecords;
} TstSpecialDays;

TU08 SpecialDays_Int(void);
TU08 SpecialDays_delete(TU16 index);
TU08 SpecialDays_insert(TstSDaysRcd* SDRecord);
TS08 SpecialDay_checkThisDayIsSpecialDay(TU08 Event_Flag);
TU08 SpecialDay_gSetNumOfSpecialDayRecords(TU16 num);
TU08 SpecialDay_gSetSpecialDay(TstSDaysRcd* stSDaysRcd, TU16 index);
TU16 SpecialDay_gGetNumOfSpecialDayRecords(void);
TU08 SpecialDay_gGetSpecialDay(TstSDaysRcd* SDRecord, TU16 index);
#endif /*SPECIAL_DAYS_H_ */


/*------------------------------------------------------------------------------------
  from Task_priority.h
------------------------------------------------------------------------------------*/
/*
 * task_priority.h
 *
 *  Created on: May 21, 2018
 *      Author: islam haggag
 */


#include "config.h"

typedef enum
{
  PRIORITY_FOR_METER_TASK         = (uint16_t)0x2000,
  PRIORITY_FOR_COMM_TASK          = (uint16_t)/*0x10*/0x1000,
  PRIORITY_FOR_CONTROL_TASK       = (uint16_t)/*0x08*/0x800,
  PRIORITY_FOR_Test_Mode_TASK     = (uint16_t)/*0x07*/0x400,
  PRIORITY_FOR_lpm_task           = (uint16_t)/*0x06*/0x200,
  PRIORITY_FOR_Optical_task       = (uint16_t)0x100,
  PRIORITY_FOR_SCALLING_FRAME_TASK = (uint16_t)0x100,
  PRIORITY_FOR_meter_modes_task   = (uint16_t)0x80,
  PRIORITY_FOR_alarms_task        = (uint16_t)0x40,
  PRIORITY_FOR_calender_task      = (uint16_t)0x20,
  PRIORITY_FOR_TARRIF_TASK        = (uint16_t)0x10,
  PRIORITY_FOR_BillingProfile_task = (uint16_t)0x08,
  PRIORITY_FOR_DISPLAY_TASK       = (uint16_t)0x04,
  PRIORITY_FOR_SELF_TEST_TASK     = (uint16_t)0x01,
  PRIORITY_FOR_NO_TASK            = (uint16_t)0x00,
} TaskPriorityLevels_t;



uint8_t task_priority_get(TaskPriorityLevels_t TaskPriorityLevels);


void task_priority_set(TaskPriorityLevels_t TaskPriorityLevels);


void task_priority_release(TaskPriorityLevels_t TaskPriorityLevels);





/*------------------------------------------------------------------------------------
  from V9261f.h
------------------------------------------------------------------------------------*/

#include "config.h"
/* #include "meter_init.h" -- inlined in this file */
#ifdef V9261f_ENABLE
extern uint8_t down_volt_flag;/* flag to tell mtr.c the volt was down to harry going to ctrl to save */
extern uint8_t V9261f_data_crpt_flag ;
extern uint8_t first_init_pulses;
//extern uint8_t V9261f_comm_err_flag ;

#define GAIN_CALIB_CONST  ((double)2147483648)
#define STD_MTR_CURR      ((float)5000000) /*  A in micro ampere*/




#define V9261f_UART_COMM_ERR          0x01
#define V9261f_CKSM_COMM_ERR         0x02
//#define V9261f_COMM_ERR_STRT     ((V9261f_comm_err_flag & V9261f_UART_COMM_ERR) || (V9261f_comm_err_flag & V9261f_CKSM_COMM_ERR))


/* IRQ BITS OF V9261f*/
/*In  V9261F ,  �bit[8:0]�  of register  �SysCtrl�  (0x0180) are used for key configuration for the operation 
mode,  and  �bit[24:16]�  are  designed  as  the  backup  of  their  configurations.  Invert  the  values  of
�bit[8:0]�, and write them into �bit[24:16]� sequentially. The internal self-checking circuit compares 
the  content  of  both  parts  all  the  time.  If  they  are  opposite  to  each  other  bit  by  bit,  it  indicates  the 
configurations are right; otherwise, an interrupt signal will be triggered,  and  the  flag bit  �SYSERR�  (bit20 
of SysCtrl) will be set to �1�.*/




/*Met ctrl register 0 */


/* mtr CTRL reg 1*/
configuration verification and current detection only; clear this bit to execute all computations.*/
When  this  bit  is  cleared,  all  functions  of  the  Vango  metering architecture are activated. */


//bit 16 ,25 and 17 enable : enable Ia , Im , V channels

/*Analog ctrl reg bits*/
#if 0
//bits 6,30 for proper operation, +20ppm
#endif

//bits 6,30 for proper operation, +60ppm

//+60ppm
//+10ppm




/* analog ctrl reg 2*/


/*not used configuration for ANCTL2 register*/

#define CONSTANT_1000

#ifdef CONSTANT_5000
#elif defined(CONSTANT_1000)
#elif defined(CONSTANT_10000)
#endif
#define VOL_RATIO_FACTOR              1750//((uint32_t)1650)

#ifdef CONSTANT_5000
#elif defined(CONSTANT_1000)
#elif defined(CONSTANT_10000)
#endif


/*!
* @par Description:
*   This enumerator contains all available calibration commands
*/
typedef enum
{
  STRT_MAN_CAL_CMD ,     /*!< Start manual calibration command.*/
  NXT_MAN_CAL_CMD ,     /*!< Next calibration command.*/
  STRT_AUTO_CAL_CMD,     /*!< Start auto calibration command.*/
  END_AUTO_CAL_CMD

} cal_cmd_t;
/*for test only*/

__packed typedef struct {
  uint32_t prev_ActpulseCount;
  uint32_t curr_ActpulseCount ;
  
  uint32_t prev_ReactpulseCount;
  uint32_t curr_ReactpulseCount ;
} pulseCount_t;


__packed typedef struct {
  uint64_t rms_curr;
  uint64_t rms_vol;
  uint64_t act_pow;
  uint64_t react_pow;
}mtr_avg_readings_t;


__packed typedef struct {

  uint32_t calib_rms_curr[MTR_NUM_OF_CH];
  uint32_t calib_rms_vol[MTR_NUM_OF_CH];
  uint32_t calib_act_pow[MTR_NUM_OF_CH];
  uint8_t   cal_flag;          /*!< Contains CRC of all data in this structure.*/
}v9261_calib_t;
__packed typedef struct {

  uint32_t ANCTRL0_Netural_Reg;
  uint8_t  Reserved[28];
  uint8_t  is_Calib_happened;
  uint8_t  crc;          /*!< Contains CRC of all data in this structure.*/
}v9261_calib_registers_t;
__packed typedef struct {
  uint64_t rms_curr[MTR_NUM_OF_CH];
  uint64_t rms_vol[MTR_NUM_OF_CH];
  uint64_t ph_angle;
  uint64_t pf[MTR_NUM_OF_CH];
  uint64_t line_freq;
  uint64_t act_pow[MTR_NUM_OF_CH];
  uint64_t react_pow[MTR_NUM_OF_CH];
  uint64_t act_energy;
}mtr_readings_t;


__packed typedef struct 
{ uint16_t addr;
  uint32_t value;
}V9261f_reg_values_t;


typedef enum {
    /* interrupt registers*/
  V9261f_REG_SysCtrl = 0x0180 ,/* IRQ0 Interrupt Enable Register*/ 
  
  /* offset and gain fundamental not used , just zeroing them as they are in the 24 registers for checksum register*/
  V9261f_REG_ZZDCI = 0x0123 , 
  V9261f_REG_ZZDCU = 0x0124 , 
  V9261f_REG_WBRTI = 0x0126 , 
  V9261f_REG_WBPT  = 0x0127 , 
  V9261f_REG_WWBPT = 0x0128 , 
  V9261f_REG_WBQT  = 0x0129 , 
  V9261f_REG_WWBQT = 0x012A , 
  V9261f_REG_WBRTU = 0x012B , 
  V9261f_REG_WWAPT = 0x012F , 
  V9261f_REG_WWAQT = 0x0131 , 

   /* CTRL REGISTERS*/
  V9261f_REG_ANCTRL0 = 0x0185 , /* Analog Control Register 0*/ 
  V9261f_REG_ANCTRL1 = 0x0186 , /* Analog control register 1 */
  V9261f_REG_ANCTRL2 = 0x0187 , /* analog control register 2 */
  
   /* Metering Control Register */
  V9261f_REG_MTPARA0 = 0x0183, /*Metering Control Register 0*/ 
  V9261f_REG_MTPARA1 = 0x0184, /*Metering Control Register 1*/

  /*------- Registers for Calibration -------*/
  /* Registers for Calibration */
  V9261f_REG_WARTI  = 0x012C ,/* Set a value to gain calibrate the total current RMS. */
  V9261f_REG_WARTU  = 0x0132 ,/* Set a value to gain calibrate the total voltage RMS. */
  V9261f_REG_WARTM  = 0x012D ,/* Set a value to gain calibrate the total M channel RMS. */
  V9261f_REG_WAPT   = 0x012E ,/* Set a value to gain calibrate the total active power. */
  V9261f_REG_WAQT   = 0x0130 ,/* Set a value to gain calibrate the total reactive power. */

  /* Registers for Calibrating Active/Reactive Power*/
  V9261f_REG_EGYTH  = 0x0181 ,/* Set  a  threshold  for  energy-to-pulse  conversion */
  V9261f_REG_CTH    = 0x0182 ,/* Set a threshold for no-load detection. */
  V9261f_REG_IDETTH = 0x0134 ,/* Set a threshold for current detection.  */

  /* Register for Bandpass Filter Coefficient Configuration*/
  V9261f_REG_BPFPARA= 0x0125 ,/*   it  is  set  to  �0x811D2BA7�  for  the best performance. */
  
  /* Checksum Register*/
  V9261f_REG_CKSUM = 0x0133 ,/*  to compute  the  checksum  for  configuration verification  to  ensure  the  configuration  of  all  the registers  are  in  the  desired  states.  If  the  sum  is �0xFFFFFFFF�, the verification will pass.  */

  
  /* Registers for Line Frequency */
  V9261f_REG_FA      = 0x019A ,/* The line frequency of Phase A updated in 20 ms */
  V9261f_REG_SAFREQ  = 0x011D ,/* The line frequency per second updated in 1.28 s */
  V9261f_REG_AFREQ   = 0x011E ,/* The average line frequency updated in 10.24 s */
  
  /* Registers for RMS Values of Total/Fundamental Signals (R/W) */
  V9261f_REG_ARRTI   = 0x0105, /*The raw total current RMS ,depend on Current Channel is used to process the current signal from IA or IB ,will be updated in 160 ms and settled in 500 ms*/
  V9261f_REG_ARRTU   = 0x0104 ,/* The raw total voltage RMS ,updated in 160 ms and settled in 500 ms. */
  V9261f_REG_ARRTM   = 0x0106 ,/* The raw RMS value of the various signal of Measurement ,updated in 160 ms and settled in 500 ms */
  V9261f_REG_ARTI    = 0x010E ,/* The instantaneous total current RMS ,,depend on Current Channel is used to process the current signal from IA or IB ,updated in 160 ms and settled in 500 ms */
  V9261f_REG_ARTU    = 0x010D ,/* The instantaneous total v RMS updated in 160 ms and settled in 500 ms */
  V9261f_REG_ARTM    = 0x010F ,/* The  instantaneous  RMS  value  of  the  various  signal  of Measurement Channel updated in 160 ms and settled in 500 ms */
  V9261f_REG_ARTIM   = 0x00F8 ,/* this register is the backup of the content of register  �ARTM�  (0x010F),  and  the  master  MCU  should read this register for the measurement. */
  
  /* Total/Fundamental Active/Reactive Power Registers (R/W)*/
  V9261f_REG_RAP    = 0x0102 ,/* The raw total active power , updated  in  160  ms  and  settled  in  500  ms.  */
  V9261f_REG_IAP    = 0x010B ,/* The instantaneous total active power , updated  in  160  ms  and  settled  in  500  ms.  */
  V9261f_REG_IAQ    = 0x010C ,/* The instantaneous total reactive power, updated  in  160  ms  and  settled  in  500  ms.  */

  /* Active/Reactive Energy Accumulators (R/W) */
  V9261f_REG_PEGY   = 0x01A1 ,/* Accumulating  positive  average  power:  T otal/Fundamental  and active/reactive power ,the accumulation  frequency  will  be  204.8  kHz. 
                                 This register is physically 46-bit, but only the most significant 32 bits  */
  V9261f_REG_NEGY   = 0x01A2 ,/* Accumulating  negative  average  power:  T otal/Fundamental  and active/reactive power ,This register is physically 46-bit, but only the most significant 32 bits  are  active.   */

  /* Active/Reactive CF Pulse Counters */
  V9261f_REG_PCFCNT = 0x01A3 ,/* Positive CF pulse counter  */
  V9261f_REG_NCFCNT = 0x01A4 ,/* Negative CF pulse counter */

  /* Power Register */
  V9261f_REG_DATACP = 0x0189 ,/* ??? */

}V9261f_reg_t;

typedef enum {
  RESET_NONE_MODE           = (uint8_t) 0,
  RESET_DATA_CRPT           = (uint8_t) 1<<0,
  RESET_UART_PROBLEM        = (uint8_t) 1<<1,
  RESET_SWITCH_LINE_NETURAL = (uint8_t) 1<<2,
  RESET_WRONG_FREQUENCY     = (uint8_t) 1<<3,
  RESET_CALIBRATE           = (uint8_t) 1<<4,
  RESET_POWER_UP            = (uint8_t) 1<<5,
  RESET_MAIN_INIT           = (uint8_t) 1<<6,
  RESET_NEW_ENG_FAIL        = (uint8_t) 1<<7,
}v9261_RESET_MODULE_REASON_enumt;

#define LINEPERIOD 1
extern uint8_t v9261f_init_numbers ;
extern uint8_t flag_get_volt ; 
extern mtr_readings_t mtr_inst_readings;
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/
void V9261f_test_uart();
void V9261f_task(void);
void cal_handler_task(void);
void cal_cmd(uint8_t arg1);
uint8_t cal_state(void);
void V9261f_inst_read(uint8_t phase);
uint8_t Init_LoadcalibParam( uint32_t* Cksum_ptr);
uint8_t V9261f_Init(uint8_t init_reason);
uint8_t V9261f_chkFailure(void);
void V9261f_read_initial_PulseCount(void);
void V9261f_calib(void);
double V9261f_calc_act_pow_error(uint64_t avg_reading);
int64_t V9261f_Pow_Gain_Calib(double pow_error, uint64_t avg_reading);
int32_t V9261f_RMS_Gain_Calib(uint32_t avg_reading, float std_mtr_value);
void V9261f_chk_active_phases(void);
uint8_t V9261f_get_ins_reverse(void);
uint64_t V9261f_get_ActEnergy_Accum( uint8_t* Negative_flag_ptr);
void V9261f_clr_ins_reverse(uint8_t arg1) ;
uint64_t V9261f_instant_active_power_read_microW();
void v9261f_check_line_or_Netural(uint32_t Ia , uint32_t Im);
uint8 V9261f_get_channel_I ();
void v9261f_rx_Isr(uint8_t rx_byte);
void v9261f_read_inst_task();
void v9261f_Energy_Flag();
void v9261f_Volt_Flag();
uint8_t v9261f_Get_Negative_Flag();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_get_Reset_Flag
// Description      : This function is used to get the reason of reseting the module
// Arguments        : NONE
// Return           : one of this enum v9261_RESET_MODULE_REASON_enumt
// Output           : NONE
//-----------------------------------------------------------------------------
// Author           : Ibrahem Mostafa
// Date             : 30/03/2020
//=============================================================================
uint8_t V9261f_get_Reset_Reason (void);
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_get_ReactEnergy_Accum
// Description      : This function is used to get the reactive energy accmulation for each phase
// Arguments        : phase : the phase(0 for phase A, 1 for phase B, 2 for phase C)
// Return           : the reactive energy accmulation for each phase
// Output           : NONE
//-----------------------------------------------------------------------------  
uint64_t V9261f_get_ReactEnergy_Accum();
uint32_t V9261f_Reactpulse_counter_read();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_load_Param
// Description      : This function is used to load from eeprom the calibration parameters
// Arguments        : None.
// Return           : true if load from eeprom ok.
// Output           : NONE
//===========================================================================
uint8_t V9261f_load_Param ();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_get_rms_voltage
// Description      : This function is used to get the instantaneous total rms voltage value for each phase
// Arguments      : arg1 : the phase (0 for ph A, 1 for ph B, 2 for ph C)
// Return           : The instantaneous rms voltage value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9261f_get_rms_voltage();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_Set_Default_Param
// Description      : This function is used to set and save the default of the calibration parameters
// Arguments        : None.
// Return           : NONE
// Output           : NONE
//===========================================================================
void V9261f_Set_Default_Param();
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_get_rms_current
// Description      : This function is used to get the instantaneous total rms current value in Micro Amperes
// Arguments      : arg1 : the phase (0 for ph A, 1 for ph B, 2 for ph C)
// Return           : The instantaneous total rms current value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9261f_get_rms_current(uint8_t arg1);
//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_instant_total_irms_read
// Description      : This function is used to read the instantaneous total rms current value for each phase
// Arguments      : LocalPhase: (0 for ph A, 1 for ph B)
// Return           : The instantaneous total rms current value
// Output           : NONE
//-----------------------------------------------------------------------------
uint32_t V9261f_instant_total_irms_read(uint8_t LocalPhase) ;

// FUNCTION
//=============================================================================
// Name             : V9261f_instant_total_vrms_read
// Description      : This function is used to read the instantaneous total rms voltage value
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C)..
// Return           :the instantaneous total rms voltage value
// Output           : NONE
//=============================================================================
uint32_t V9261f_instant_total_vrms_read() ;

//=============================================================================
// FUNCTION
//=============================================================================
// Name             : V9261f_instant_tot_active_power_read
// Description      : This function is used to get the total instantaneous active power  
// Arguments         : NONE
// Return           :the total instantaneous active power
// Output           : NONE
//-----------------------------------------------------------------------------

uint64_t V9261f_instant_active_power_read_microW();

uint32_t V9261f_instant_tot_reactive_power_read_milliVAR(void);
//=============================================================================
// Name             : V9261f_line_freq_read
// Description      : This function is used to get the line frquency for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the line frequency
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9261f_line_freq_read() ;
//=============================================================================
// Name             : V9261f_instant_total_PF_read
// Description      : This function is used to get the instantaneous power factor for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the instantaneous power factor
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9261f_instant_total_PF_read(uint64_t power , uint32_t volt , uint32_t current) ;
//=============================================================================
// Name             : V9261f_Actpulse_counter_read\
// Description      : This function is used to get the pulse counter for active power for each phase
// Arguments         : LocalPhase  (0 for ph A, 1 for ph B, 2 for ph C
// Return           :the pulse counter for active power
// Output           : NONE
//------------------------------------------------------------------------
uint32_t V9261f_Actpulse_counter_read();
//=============================================================================
// Name             : V9261f_INT_flagReg_read
// Description      : This function is used to read the interrupt flag register (V9261f_REG_IRQFLAG)
// Arguments         : NONE
// Return           :the interrupt flag register (V9261f_REG_IRQFLAG)
// Output           : NONE
//------------------------------------------------------------------------
uint8_t V9261f_INT_flagReg_read(uint32_t * localFlagReg);

void V9261f_Switch_line_or_Netural(uint8_t is_Switch_Line);

#endif






/*------------------------------------------------------------------------------------
  from display.h
------------------------------------------------------------------------------------*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                    D I S P L A Y

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file display.h
 * @brief This file contains display APIs prototype.
 * @details This file contains the required definitions, types and APIs
 * for display system.
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
 *   <td> 10/01/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Handling of power down moved to control. calculating varying index is done every 1 sec not in the task. </td>
 * </tr>
 * <tr>
 *   <td> 26/08/2012 </td>
 *   <td> mohamed.zakaria  </td>
 *   <td> Add new message "ver x.x.xx" to be displayed in meter start up.</td>
 * </tr>
 * <tr>
 *   <td> 08/07/2012 </td>
 *   <td> mohamed.zakaria  </td>
 *   <td> Update comments in file. Also add API for display test.</td>
 * </tr>
 * <tr>
 *   <td> 08/07/2012 </td>
 *   <td> mohamed.zakaria  </td>
 *   <td> Created  </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup DISPLAY_SYS Display System
 * @ingroup METER
 * Display system provides functions for screens handling.\n
 * This module can be accessed through only APIs.
 * This module has three interfaces each one of these interfaces has number of APIs.\n
 *   -# Configuration Interface.
 *   -# Event Interface.
 *   -# Command Interface.
 *
 * \n<b>References</b>\n
 * - Ref folder in App inside Display folder.
 * - display_design.docx in App inside Display folder.
 * @brief Responsible to manage display on LCD.
 */
/*!
 * @defgroup DISPLAY_CORE_APIS Core APIs
 * @ingroup DISPLAY_SYS
 * @brief Display system entry and initialization functions.
 */
/*!
 * @defgroup DISPLAY_CONFIG_APIS Configuartion APIs
 * @ingroup DISPLAY_SYS
 * @brief Display system confiuration functions and data types.
 */
/*!
 * @defgroup DISPLAY_EVENT_APIS Event APIs
 * @ingroup DISPLAY_SYS
 * @brief Display events call back APIs.
 */
/*!
 * @defgroup DISPLAY_CMD_APIS Commands APIs
 * @ingroup DISPLAY_SYS
 * @brief display command interface APIs.
 */
/*!
 * @defgroup DISPLAY_DRV Drivers
 * @ingroup DISPLAY_SYS
 * @brief List of all supported LCD drivers in this system.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"

/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/***************Firmware Version***************/
#ifdef MTR_THREE_PH
#ifdef MTR_DIRECT
#ifdef ASHNTTI_PROJECT
#define  FIRMWARE_VERSION     "h.491"  //3ph direct
#elif defined (GPRS_FEATURE)
#define  FIRMWARE_VERSION     "3.519"  //3ph direct
#elif defined (SPAIN_BOARD)
#define  FIRMWARE_VERSION     "d.531"  //3ph direct
        #else
#define  FIRMWARE_VERSION     "u.009"  //3ph direct  //DRU FIRMWARE_VERSION
        #endif

#else//indirect
        #ifdef SPAIN_BOARD
#define  FIRMWARE_VERSION     "i.487"  //3ph indirect
        #else
#define  FIRMWARE_VERSION     "i.507"  //3ph indirect
        #endif
#endif//MTR_DIRECT
#else//Single
#ifdef ANSI_STACK
#define  FIRMWARE_VERSION     "2.539"
#else
#ifdef ASHNTTI_PROJECT
#define  FIRMWARE_VERSION     "e.529"  //1ph EM122  Ashanti
#elif defined (GPRS_FEATURE) || defined(PLC_MODEM) || defined(PLC_MODEM_IEC_TEST)
#ifdef GROUP_METER_FEATURE
#define  FIRMWARE_VERSION     "g.527"  //1ph 5mer EM122 with GPRS
#else
#define  FIRMWARE_VERSION     "b.594"  //1ph Local EM122 with GPRS
#endif
#elif defined (GROUP_METER_FEATURE)
#define  FIRMWARE_VERSION     "4.526"  //1ph Amer EM120 without GPRS 
#elif defined (SPAIN_BOARD)
#define  FIRMWARE_VERSION     "s.535"  //1ph
#else
#define  FIRMWARE_VERSION     "2.553"
        #endif
#endif
#endif//MTR_THREE_PH
//#include "V9203.h"
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
/*!
* @par Description:
*   This enumerator contains display modes.
*/
typedef enum
{
  NORMAL_MODE   = (uint8_t)0, /*!< LCD cleard */
  AUTO_MODE     = (uint8_t)1, /*!< When meter up or after operator or manual out time*/
  OPERATOR_MODE = (uint8_t)2, /*!< When operator connects optical*/
  MANUAL_MODE   = (uint8_t)3, /*!< When customer is pressed on any button of LCD*/
#ifdef EDIT_TIME_DATE_MANUALLY
  TIME_DATE_EDIT_MODE =(uint8_t)4,   /*!< when recieve the edit time commend*/
#endif
} display_mode_t;

/*!
 * @ingroup DISPLAY_EVENT_APIS
 * @par Description:
 * This enumerator contains display events.
 */
typedef enum
{
  DISPLAY_EVE_SEC     = (uint8_t)1,   /*!< Used to inform the system by new second elapsed.*/
  DISPLAY_EVE_KEY_UP  = (uint8_t)2,   /*!< Used to inform the system by up key pressed.*/
  DISPLAY_EVE_KEY_DOWN  = (uint8_t)3, /*!< Used to inform the system by down key pressed.*/
  DISPLAY_EVE_PWR_DWN   = (uint8_t)4, /*!< Used to inform the system by power down*/
  DISPLAY_EVE_PWR_UP    = (uint8_t)5, /*!< Used to inform the system by power up*/
  DISPLAY_EVE_UP_KEY_RELEASE		= (uint8_t)6,
  DISPLAY_EVE_DOWN_KEY_RELEASE		= (uint8_t)7,
} display_event_t;

typedef enum
{
  FIRST= (uint8_t)0,
  HOUR=(uint8_t)0,
  MIN=(uint8_t)1,
  SEC=(uint8_t)2,
  YEAR=(uint8_t)3,
  MON=(uint8_t)4,
  DAY=(uint8_t)5,
  LAST= (uint8_t)5,
}current_adjested_seg_t;
/*!
 * @ingroup DISPLAY_CONFIG_APIS
 * configuration structure for defining the main variables used by display saved in non-volatile area.
 * @brief Display configurations.
 */
#ifndef DISPLAY_MAP_SCREEN
__packed typedef struct
{
  uint16_t scrn_saver_duration;/*!< Time to leave screens scrolling and clear LCD and go to sleep.*/
  uint8_t scroll_time;         /*!< Time of Change from screen to next order, it sets by seconds*/
  uint8_t manual_duration;     /*!< Time to leave manual screens */
  uint8_t msg_duration;        /*!< Time of displayed message */
#if(DISPLAY_SCREEN_ORDER == CONSTANT_SCREEN_ORDER)
  uint8_t scrn_auto_mode[6];       /*!< It represents screen statues in automatic*/ /* 6 means 48 bit for 48 screens*/
  uint8_t scrn_manual_mode[6];     /*!< It represents screen statues in manual*/ /* 6 means 48 bit for 48 screens*/
  uint8_t scrn_operator_mode[6];   /*!< It represents screen statues in operatore*/ /* 6 means 48 bit for 48 screens*/
#else /*CHANGEABLE_SCREEN_ORDER*/
  uint8_t scrn_auto_mode[48];         /*!< It represents screen statues in automatic*/
  uint8_t scrn_manual_mode[48];       /*!< It represents screen statues in manual*/
  uint8_t scrn_operator_mode[48];     /*!< It represents screen statues in operator*/
#endif
  uint8_t display_crc;         /*!< crc byte check.*/
} display_ctrl_data_t;
#else
__packed typedef struct
{
  uint16_t screen_numbers;      /*number of screens*/  
  uint16_t scrn_saver_duration;/*!< Time to leave screens scrolling and clear LCD and go to sleep.*/
  uint8_t scroll_time;         /*!< Time of Change from screen to next order, it sets by seconds*/
  uint8_t manual_duration;     /*!< Time to leave manual screens */
  uint8_t msg_duration;        /*!< Time of displayed message */
  uint8_t display_crc;         /*!< crc byte check.*/
} display_ctrl_data_t;
#endif
__packed typedef struct
{
  uint8_t start_screen_num;    /*!< It represents screen number on display*/
  uint8_t start_index_screen_num; /*!< It represents screen index on display*/
  uint8_t draw_screen_num;        /*!< It represents real  screen number searching in draw_screen*/
  uint8_t draw_index_screen_num;   /*!< It represents real  screen index searching in draw_screen*/
}display_show_t;
#if DISPLAY_OBIS
#endif
/*!
 * @par Description:
 *   This enumerator contains name of screens.
 */
typedef enum
{
  SCREEN_TEST                     = (uint8_t)0, /*!< screen 0 is SCREEN_TEST */
  SCREEN_REMAINING                = (uint8_t)1, /*!< screen 1 */
  SCREEN_TOTAL_KWH                = (uint8_t)2, /*!< screen 2 is SCREEN_TOTAL_KWH */
  SCREEN_ACTIVE_ENG_KWH           = (uint8_t)3, /*!< screen 3 is SCREEN_ACTIVE_ENG_KWH */
  SCREEN_REVERSE_ENG              = (uint8_t)4, /*!< screen 4 */
  SCREEN_EARTH_ENG                = (uint8_t)5, /*!< screen 5 */
  SCREEN_CHARGES                  = (uint8_t)6, /*!< screen 6 */
  SCREEN_DATE_TIME                = (uint8_t)7, /*!< screen 7 is SCREEN_DATE_TIME */
  SCREEN_ALARM_LIMIT              = (uint8_t)8, /*!< screen 8 */
  SCREEN_VOLTAGE                  = (uint8_t)9, /*!< screen 9 is SCREEN_VOLTAGE */
  SCREEN_CURRENT                  = (uint8_t)10, /*!< screen 10 is SCREEN_CURRENT */
  SCREEN_FREQEUANCY               = (uint8_t)11, /*!< screen 11 is SCREEN_FREQEUANCY */
  SCREEN_PF                       = (uint8_t)12, /*!< screen 12 */
  SCREEN_ACTV_PWR_KW              = (uint8_t)13, /*!< screen 13 */
  SCREEN_ID                       = (uint8_t)14, /*!< screen 14 is SCREEN_ID */
  SCREEN_TAMPER_COUNT             = (uint8_t)15, /*!< screen 15 */
  SCREEN_TAMPERING_INDICATION     = (uint8_t)16, /*!< screen 16 */
  SCREEN_MD_A_BILLING             = (uint8_t)17, /*!< screen 17 */
  SCREEN_MD_A_BILLING_TRF         = (uint8_t)18, /*!< screen 18 */
  SCREEN_MD_KW_BILLING            = (uint8_t)19, /*!< screen 19 */
  SCREEN_MD_KW_BILLING_TRF        = (uint8_t)20, /*!< screen 20*/
  SCREEN_MDA_MTER_LIFE            = (uint8_t)21, /*!< screen 17 */
  SCREEN_MD_KW_MTER_LIFE          = (uint8_t)22, /*!< screen 22 */
  SCREEN_MD_RESET_COUNT           = (uint8_t)23, /*!< screen 23 */
  SCREEN_LEVEL                    = (uint8_t)24, /*!< screen 24 */
  SCREEN_METER_READING            = (uint8_t)25, /*!< screen 25 is SCREEN_METER_READING */
  SCREEN_PUSLE_INDICATION_TEST    = (uint8_t)26, /*!< screen 26 is SCREEN_PUSLE_INDICATION_TEST */
  SCREEN_LOAD_COST                = (uint8_t)27, /*!< screen 27 */
  SCREEN_TRF_PRICES               = (uint8_t)28, /*!< screen 28 */
  SCREEN_TRF_STEPS                = (uint8_t)29, /*!< screen 29 */
  SCREEN_TAXES                    = (uint8_t)30, /*!< screen 30 */
  SCREEN_MD_KVA_BILLING           = (uint8_t)31, /*!< screen 31 */
  SCREEN_MD_KVA_BILLING_TRF       = (uint8_t)32, /*!< screen 32 */
  SCREEN_MD_KVA_MTER_LIFE         = (uint8_t)33, /*!< screen 33 */
  SCREEN_TRF_ACTV_ENG_KWH         = (uint8_t)34, /*!< screen 34 is SCREEN_TRF_ACTV_ENG_KWH */
  SCREEN_POWER_INT                = (uint8_t)35, /*!< screen 35 is SCREEN_TRF_ACTV_ENG_KWH */
  SCREEN_TOTAL_KVARH              = (uint8_t)36, /*!< screen 36 */
  SCREEN_TOTAL_KVAH               = (uint8_t)37, /*!< screen 37 */
  SCREEN_REACTIVE_PWR_KVAR        = (uint8_t)38, /*!< screen 38 */
  SCREEN_APPARENT_PWR_KVA         = (uint8_t)39, /*!< screen 39 */
  SCREEN_STEP_REMAINING           = (uint8_t)40, /*!< screen 40 */
  SCREEN_BPH                      = (uint8_t)41, /*!< screen 41 */
  SCREEN_BPH2                     = (uint8_t)42, /*!< screen 41 */
  SCREEN_FRIENDLY   		  = (uint8_t)43, /*!< screen 42 */
  SCREEN_START_TAMPER_EVENT       = (uint8_t)44, /*!< screen 47 */
  SCREEN_STOP_TAMPER_EVENT        = (uint8_t)45, /*!< screen 48 */
  SCREEN_ACCURACY_TEST            = (uint8_t)46, /*!< screen 46 */
  SCREEN_LAST_12_CONSUM_MONEY_KWH = (uint8_t)47, /*!< screen 47 */
  SCREEN_TOTAL_DEBIT              =(uint8_t)48 ,/*!< screen 48 */
  SCREEN_CUSTOMER_SERVICE         =(uint8_t)49 ,/*!< screen 49 */
  SCREEN_STATE_RELAY              =(uint8_t)50 ,/*!< screen 50 */
  SCREEN_MAX_CURRENT_LEVEL_KWH     =(uint8_t)51 ,/*!< screen 51 */
  SCREEN_SUM_CHARGE_NUM_INTERFACES =(uint8_t)52,/*!< screen 52 */
  SCREEN_TAMPER_COUNTER            =(uint8_t)53,/*!< screen 53 */
  SCREEN_ERROR_OF_CONTACTOR        =(uint8_t)54,/*!< screen 54 */
  SCREEN_LAST_CHARGE                       = (uint8_t)55, /*!< screen 55 */
  SCREEN_DEPARTMENT_ID             =(uint8_t)56,/*!< screen 56 */
  SCREEN_OVERLOAD_COUNTER          =(uint8_t)57,/*!< screen 57 */
  SCREEN_BATTERY        = (uint8_t)58,/*!< screen 58*/
  SCREEN_LAST_DATE_OPEN_TERMNL = (uint8_t)59,/*!< screen 59*/
  SCREEN_SELECTED_REMAINING_SCREENS = (uint8_t)60,/*!< screen 60*/
  SCREEN_REVERSE_REMAINING_GRACE_MONEY         = (uint8_t)61,/*!< screen 61*/
  
  SCREEN_ACTIVE_4Q_CONSUMPTION    = (uint8_t)62,  /*!< screen 62 */
  SCREEN_REACTIVE_4Q_CONSUMPTION  = (uint8_t)63, /*!< screen 63 */
  SCREEN_INDIRECT_PARAMETER       = (uint8_t)64, /*!< screen 64 */
  SCREEN_47   = (uint8_t)64, /*!< screen 47 */
#ifdef SCREEN_GPRS_FEATURE 
 // SCREEN_GPRS_STATUS              = (uint8_t)65, /*!< screen 46 */
#endif
  SCREEN_BPH3            = (uint8_t)66, /*!< screen 66 */
  SCREEN_BPH4            = (uint8_t)67, /*!< screen 66 */
  SCREEN_STS_TEST0            = (uint8_t)68, /*!< screen 68 */
#ifdef ANSI_STACK  
  SCREEN_RELAY_DISC_REASON   = (uint8_t)69, /*!< screen 69 */
#else
  STS_TENDER_SCREENS      = (uint8_t)69, /*!< screen 69 */
#endif 
#ifdef ANSI_STACK
  SCREEN_NEW_ANSI               = (uint8_t)71,
 /*! SCREEN_ANSI_NEW 
  SCREEN_METER_CONSTANT	  0	
  SCREEN_METERING_MODE	  1     
  SCREEN_NET_ENERGY	  2       
  SCREEN_TOT_FRD	  3      
  SCREEN_TOT_GEN	  4     
  SCREEN_NEUTRAL_STATUS   5      
  SCREEN_RELAY_ERR_STATUS 6
  SCREEN_ALARM_REASON     7
  SCREEN_EEPROM_FAIL      8
  SCREEN_ADE_FAIL         9     
*/    
#endif  
#ifdef SCREEN_GPRS_FEATURE 
  SCREEN_GPRS_STATUS              = (uint8_t)75, /*!< screen 46 */
#endif
#if (DISPLAY_SCREEN_ORDER  == CONSTANT_SCREEN_ORDER)
  SCREEN_LAST  = (uint8_t) 67    //old 44
#else
  SCREEN_LAST = 80   
#endif 
}current_screen_t;


typedef enum 
{
  token_Accept=0,
  first_key,
  second_key,
  key_type_error,
  token_error,
  function_error,
  over_flow_error,
  first_accept,
  second_accept
}token_state;

typedef enum 
{
  WIFI_SYMBOL_FLAG = 0x00000001,
  OPTICAL_SYMBOL_FLAG = 0x00000002,
  GPRS_SYMBOL_FLAG = 0x00000004,
  BLUETOOTH_SYMBOL_FLAG = 0x00000008,
  RS485_SYMBOL_FLAG = 0x00000010, 
  MESSAGE_SYMBOL_FLAG = 0x00000020, 
  BATTERY_SYMBOL_FLAG = 0x00000040,
  L1_SYMBOL_FLAG = 0x00000080,
  L2_SYMBOL_FLAG = 0x00000100,
  L3_SYMBOL_FLAG = 0x00000200,
  UP_SYMBOL_FLAG = 0x00000400,
  DOWN_SYMBOL_FLAG = 0x00000800,
  RIGHT_SYMBOL_FLAG = 0x00001000,
  LEFT_SYMBOL_FLAG = 0x00002000,
  SMILE_SYMBOL_FLAG = 0x00004000,
  SAD_SYMBOL_FLAG = 0x00008000,
  MAGNETIC_SYMBOL_FLAG = 0x00010000,
  REVERSE_SYMBOL_FLAG = 0x00020000,
  RELAY_SYMBOL_FLAG = 0x00040000,
  EARTH_SYMBOL_FLAG = 0x00080000,
  HANDCHECK_SYMBOL_FLAG=0x00100000,
  RF_SYMBOL_FLAG = 0x00200000,     
  LOAD_SYMBOL_FLAG = 0x00400000, 
  CARD_SYMBOL_FLAG = 0x00800000,   
  IN_SYMBOL_FLAG = 0x01000000,   
  ALARM_SYMBOL_FLAG = 0x02000000,
#ifdef ANSI_STACK  
  PRUEBA_FLAG = 0x40000000,
#endif
}symbols_flage_t;
typedef enum 
{
  EL7ALY_FLAG = 0x00000001,
  ELSABEQ_FLAG = 0x00000002,
  KWH_FLAG = 0x00000004,
  GNEH_FLAG = 0x00000008,
  MO8L2_FLAG = 0x00000010,
  MAFTO7_FLAG = 0x00000020,
  ZERO_DASH_FLAG = 0x00000040,
  INFINITY_FLAG = 0x00000080,
  _8ER_FLAG = 0x00000100,
  TLA3OB_FLAG = 0x00000200,
  T_OPEN_FLAG = 0x00000400,
  C_OPEN_FLAG = 0x00000800,
  LOW_BAT_FALG=0x00001000,
  NO_BAT_FALG=0x00002000,
  AM_FALG=0x00004000,
  PM_FALG=0x00008000,
  ERROR_RELAY_FALG=0x00010000,
  MA2BOL_FLAG=0x00020000,
  MANUAL_FLAG=0x00040000,
  AUTOMATIC_FLAG=0x00080000,
  ZA2D_FLAG=0x00100000,
  VAR_FLAG=0x00200000,
  KW_FLAG=0x00400000,
  AMP_FLAG=0x00800000,  
  VOLT_FLAG=0x01000000,  
  PASHSEQ_FLAG=0x02000000,  
}special_screens;

/*!
* @par Description:
*   This enumerator Indicates accessing Digits.
*/
typedef enum
{
  MAIN_DIGITS  = (uint8_t)0,  /*!< accessing Main digits.*/
  INDEX_DIGITS = (uint8_t)1   /*!< accessing Index digits.*/
} digit_type_t;

#ifdef ANSI_STACK
typedef enum {
            DP1 = (uint8_t)0,
            DP2 = (uint8_t)1,
            DP3 = (uint8_t)2,
            NO_DP = (uint8_t)3,
}Display_DP_STD;

typedef enum {
          KILO_W = (uint8_t)0,
          UNITS =(uint8_t)1,
}Display_UnitsSTD;

__packed typedef struct
{
  uint8_t Unit;
  uint8_t DP;
  uint8_t Crc;
}Display_UnitsCommaConfigSTD;

#endif

typedef enum
{
  TOTAL_CONSUMPTION_MONEY=0,
  LAST_PURCHSE_TOKEN,
  PURCHASE_TOKNES,
  FRENDLY_TIME,
  REASON_FOR_LAST_RELAY_DISC,
  TOTAL_CHARGES_KWH_STS,
  METER_FAULT_STATUS,
  OVER_DRAFT_LIMIT,
  ACTIVE_ENERGY_CONSUM_LAST_DAY,
  LAST_CHARGE_TIME,
  LAST_CHARGE_AMOUNT,

} cameron_screens_t;

typedef enum
{
  ACTIVATE_EMERGENCY_OVERDRAFT=811,  //frendly
  CANCEL_AUDIABLE_ALARM,
} cameron_actions_t;
#ifdef NEW_ASHANTI_PROJECT
typedef enum {
    EEPROM_ERROR = 100,
    RTC_ERROR,
    MEASUREMENT_CHIP_ERROR,
    LOW_BATTERY,
    REPLACE_BATTERY,
    RELAY_MALFUNCTION,
    OPEN_METER_COVER,
    OPEN_TERMINAL_COVER,
    MAGNETIC_INTERFERENCE,
    UNDERVOLTAGE,
    OVER_VOLTAGE,
    L1_PHASE_OVER_CURRENT,
    L2_PHASE_OVER_CURRENT,
    L3_PHASE_OVER_CURRENT,
    OVERLOAD,
    FIRMWARE_READY,
    ERROR_REGISTER_CLEARED,
    AUTHEN_FAILURE,
    CURRENT_FIRMWARE_VERSION,
    RELAY_TEST,
    OPEN_COMMUNICATION_COVER
} GhanaErrorCode;
#endif
typedef struct {
    uint8_t message[20];
    uint8_t value;
} ScreenResult;

typedef enum {
    RTC_MASK = 0x01,
    BOOTLOADER_MASK = 0x02,
}INDEX_MASK;
/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
extern uint8_t  enabled_index[48];
extern uint8_t eeprom_test_flag;
extern display_ctrl_data_t  display_ctrl_data;
extern uint8_t latch_long_press_flag ;
//extern uint8_t init_task_number;
extern uint8_t reinit_lcd;
extern uint32_t symbol_flag;
extern uint32_t dot_matrix_special_screen_reg;
extern uint8_t dot_matrix_special_screen_flag;
extern uint8_t dot_matrix_handle_dash_in_last12scr_flag;
extern uint64_t UI_indep_icons_flag;
extern display_mode_t display_prev_mode;
extern display_mode_t display_mode;
extern uint8_t dm_enter_manually_dateTime;
extern current_adjested_seg_t current_adjested_seg;
extern uint8_t cont_byte_screen;
extern uint16_t dis_eep_index;
#ifdef DLMS_UI
extern uint8_t UI_index_ptr[8];
#endif
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
#ifdef ARABIC_SCREEN
#ifdef DOT_MATRIX_LCD_ENABLE
#else
#endif
#else
#ifdef DOT_MATRIX_LCD_ENABLE
#else
#endif
#endif
#ifndef DOT_MATRIX_LCD_ENABLE
#else
#endif
#ifdef ASHNTTI_PROJECT
#else
#endif
#define DISPLAY_MAXIMUM_SCREENS     148//592
#ifdef DOT_MATRIX_LCD_ENABLE
#else
#endif
#ifdef CD0066_MH6531AHSP_ENGLISH
#ifdef DOT_MATRIX_LCD_ENABLE
#else
#endif
#else
#ifdef DOT_MATRIX_LCD_ENABLE
#else
#endif
#ifdef ASHNTTI_PROJECT
#else
#endif
#endif
#ifndef DLMS_UI
#else
#endif


#ifdef ANSI_STACK
#endif

/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/*!
 * @addtogroup DISPLAY_CONFIG_APIS
 * @{
 */
/*!
 * @brief Storing control data
 *
 * @par Description:
 * This function is responsible to configure display configuration structure and update it
 * in non-volatile area.
 *
 * @param[in] data_ptr is uint8_t * argument which represents the pointer of new structure.
 *
 * @return
 *  - Positive value on success to indicate size of data that was set.
 *  - Zero value in case of failure.
 */
#ifndef DISPLAY_MAP_SCREEN
uint8_t display_set_ctrl_data(uint8_t * data_ptr);
#else
uint8_t display_set_ctrl_data(void);
#endif

#define TEST_MTR_VALUES_ON_LCD
#ifdef TEST_MTR_VALUES_ON_LCD
void display_test_metering_values();
#endif
/*!
 * @brief Getting control data
 *
 * @par Description:
 * This function is responsible to copy display configuration structure to given buffer.
 *
 * @param[in] data_ptr is uint8_t * argument which represents the pointer of buffer to be fill.
 *
 * @return
 *  - Positive value on success to indicate size of data that was gotten.
 *  - Zero value in case of failure.
 */
uint8_t display_get_ctrl_data(uint8_t * data_ptr);

/*!
 * @brief Getting system feature
 *
 * @par Description:
 * This function is responsible to return enabled display features
 *
 * @return
 * -  uint8_t value represents the enabled features
 */
uint8_t display_get_feature(void);
/*! @}*/

/*!
 * @addtogroup DISPLAY_CMD_APIS
 * @{
 */
/*!
 * @brief Switch to operator mode
 *
 * @par Description:
 * It changes display mode to operator mode OPERATOR_MODE, it is called from other modules
 * to change display mode i.e. when operator connects optical to meter and send command.
 *
 * @return
 * None.
 */
void display_operator_mode(void);

/*!
 * @brief Force screen update
 *
 * @par Description:
 * This function is responsible for update screen with new main, index digits and independent
 * icons after clearing screen.
 *
 * @return
 * None.
 */
void display_update(void);

/*!
 * @brief Display test
 *
 * @par Description:
 * This function is responsible for setting all segments of LCD.
 *
 * @return
 * None.
 *
 */
void display_test(void);

/*!
 * @brief Display message event
 *
 * @par Description:
 * This function is responsible for display specific system massegs. If system in sleep mode
 * then system will up and display it after that switch to automatic mode.
 *
 * @param[in] main_string contains string displayed on main digits
 * @param[in] index_string contains string displayed on index digits
 *
 * @return
 * None.
 *
 * @note
 * This API must be called in power up state only.
 */
void display_message(uint8_t * main_string, uint8_t * index_string);
/*! @}*/
#ifdef DOT_MATRIX_LCD_ENABLE
void Dot_Matrix_display_message(uint8_t *str);

#endif
/*!
 * @addtogroup DISPLAY_EVENT_APIS
 * @{
 */
/*!
 * @brief Display event callback
 *
 * @par Description:
 * It is used by other modules to inform display system by new events.
 *
 * @param[in] event_type is #display_event_t which represents events.
 *
 * @return
 * None.
 */
void display_event(uint8_t event_type);
/*! @}*/

/*!
 * @addtogroup DISPLAY_CORE_APIS
 * @{
 */

/*!
 * @brief Initialize display
 *
 * @par Description:
 * Initialize LCD and load display configuration.
 *
 * @return
 * None.
 */
void display_init(void);

/*!
 * @brief Main entry point for display system
 *
 * @par Description:
 * It is main function called by main loop of the system periodically
 *
 * @return
 * None.
 */
void display_task(void);
void display_number_generic(uint32_t displayed_value,uint8_t display_index);
#ifdef EDIT_TIME_DATE_MANUALLY
/*!
 * @brief Start the time and date edit mode
 *
 * @par Description:
 * it is used to start the edit mode
 *
 * @return
 * None.
 */
void display_set_edit_mode(void);
uint8_t Display_check_edit_mode(void);
/*!
 * @brief time and date edit task
 *
 * @par Description:
 * it is used to edit time and date manually
 *
 * @return
 * None.
 */
void display_edit_time_date_manually(void);
#endif
void Display_clearScreen() ;
/*!
 * @brief control the scale of consumption
 *
 * @par Description:
 * It is used to display the required decimal point according to the resolution factor
 *
 * @return
 * None.
 */
void display_resolution_factor(void);
/*!
 * @brief read display configuration
 *
 * @par Description:
 * used to read display configuration
 *
 * @return
 * None.
 */
display_ctrl_data_t  *  display_get_config();
/*! @}*/
void save_default_data(void);
void display_load_default_data(void);
#ifdef EDIT_TIME_DATE_MANUALLY
void display_set_edit_mode(void);
void display_edit_time_date_manually(void);
uint8_t BS_check_valid_date (uint8_t day , uint8_t month , uint8_t year);
uint8_t Display_check_edit_mode(void);
#endif 
void display_save_data_config (void);
void display_accuracy_test_init(void);
void display_reset_manual (void); // new self test
uint16_t sumofindexscreen(void);
uint16_t fill_eeprom_disp(void);
void display_set_default_screen(void);
void get_billing_record(uint8_t subscreen_index,uint8_t *data_ptr);
void reset_scr_svr_timeout(void);
uint8_t update_sts_screen(void);
void Seg_STS_Screen(void);
void process_and_draw_dot_matrix_screen(uint64_t DM_value);
void reset_sts_diplay_buf(void);
void set_current_and_index_screen(uint8_t tmp_current_screen,uint8_t tmp_index_screen,uint8_t tmp_new_current_screen,uint8_t tmp_new_index_screen);
uint32_t get_abis(uint8_t current_screen,uint8_t current_screen_index);
uint8_t Get_UI_scr_feature_flag(void);
uint32_t Get_OBIS_for_UI(void);
uint8_t *Get_Value_for_UI(void);
uint64_t Get_UI_indep_icons(void);
uint8_t Get_date_time_UI_flag(void);
void reset_date_time_UI_flag(void);
uint8_t Get_hour_day_value(void);
void process_and_handling_value_in_UI(uint32_t val);
void set_current_screen_from_eeprom(uint16_t screen_no);
uint8_t get_new_current_screen(void);
uint8_t get_new_current_screen_index(void);
void reset_UI_indep_flag(void);
void display_set_comm_icon_state(void);
#ifdef ANSI_STACK
uint8_t display_get_dp_unit(uint8_t * buffer);
static void display_Set_ANSI_Table_interface(uint8_t table_num, uint8_t* data_ptr,const uint8_t* dis_mode_buff );
uint8_t display_get_ANSI_Table_interface(uint8_t table_num,uint8_t* data_ptr,const uint8_t* dis_buf );
void display_reset_DP_to_default(void);
void display_apply_default_screens(void);
uint8_t display_check_valid_table(uint8_t* data_ptr);
uint8_t display_check_valid_data_scroll_time(uint8_t* data_ptr);
void display_set_scroll_time(uint8_t table_num,uint8_t* data_ptr);
uint32_t Display_ScaleToValue(uint8_t dp, uint32_t value);
void ANSI_display_go_to_op_mode(void);
void ANSI_display_go_back_auto_mode(void);
void DispLay_Set_DP(uint8_t Number);
void display_load_screen_scroll_time(void);
#endif
uint64_t Dot_matrix_set_date_time(uint8_t hour_day,uint8_t min_month,uint8_t sec_year,uint8_t mode);
void modify_UI_string_frame(uint8_t *data_ptr,uint8_t data_len);
uint8_t get_current_screen(void);
uint8_t get_current_screen_index(void);
ScreenResult handle_Sc_Error_Screen(uint8_t code);



/*------------------------------------------------------------------------------------
  from rfid_sys.h
------------------------------------------------------------------------------------*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                   R F I D  S Y S T E M

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file rfid_sys.h
 * @brief This file contains RFID system APIs prototypes.
 * @details This file contains the required definitions, types and APIs
 * for RFID system.
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
 *   <td> Move the RFID valid period to header file. </td>
 * </tr>
 * <tr>
 *   <td> 04/11/2012 </td>
 *   <td> islam.elshahat  </td>
 *   <td> Solve LCD icon bug. </td>
 * </tr>
 * <tr>
 *   <td> 17/06/12 </td>
 *   <td> mohamed.essa </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup RFID_APP RFID System
 * @ingroup COMM_INTFC
 * RFID system provides interface with RFID cards.\n
 * This module can be accessed through only APIs.\n
 *
 * \n<b>References</b>\n
 * - Ref_Documents folder in RFID folder.
 * - RFID_system.docx in RFID folder.
 * @brief RFID system handling.
 */
/*!
 * @defgroup RFID_CORE Core APIs
 * @ingroup RFID_APP
 * @brief RFID core functions.
 */
/*!
 * @defgroup RFID_CONFIG Configuraton APIs
 * @ingroup RFID_APP
 * @brief RFID configuration interface.
 */
/*!
 * @defgroup RFID_EVENT Event APIs
 * @ingroup RFID_APP
 * @brief RFID event interface.
 */
/*!
 * @defgroup RFID_CMD Command API
 * @ingroup RFID_APP
 * @brief RFID Service APIs.
 */
/*!
 * @defgroup RFID_READ Read APIs
 * @ingroup RFID_APP
 * @brief List of all read APIs.
 */
/*!
 * @defgroup RFID_DRV Drivers
 * @ingroup RFID_APP
 * @brief List of all supported RFID drivers in this system.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"

/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/
#define RFID_CARD_VALIDATION_PERIOD 0               /*!< Validation period in days. */

#ifdef RFID_FEATURE
/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
typedef enum
{
  RFID_COMMUNICATION_ERROR = 1 ,
  RFID_NO_CARD = 2 ,
  RFID_READ_WRITE_ERROR = 3,
} rfid_test_error_t;
/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
extern uint8_t rfid_lcd_icon;                 /*!< RFID icon on LCD.*/
extern uint8_t customer_flag;
extern uint8_t display_configuration_flag;
#ifdef RFID_DEBUG_MEASUR_SPEED
extern uint16_t rfid_speed_index,rfid_speed_sec;
#endif
extern uint8_t file_crc;
extern uint32_t file_length;
extern uint32_t EXT_FlashDestination;
extern uint8_t ctrl_sys_new_var_t_ar_size[29];
extern uint8_t rfid_donot_handle_charge_cmd;
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
/*!
 * @addtogroup TRF_PYMT_READ
 * @{
 */
/*! Used to show that the data was written in the card.*/
/*!@}*/
/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/

/*!
 @ingroup RFID_CORE
 *
 * @brief System task.
 *
 * @par Description:
 * This function is the entry point for RFID task module..
 *
 * @return
 * None.
 */
void rfid_task(void);

/*!
 * @addtogroup RFID_CONFIG
 * @{
 */
/*!
 * @brief get unique id.
 *
 * @par Description:
 * This function used to get the unique id of the RFID card.
 *
 * @param[in] uid  argument which will be used to read four bytes unique id.
 * @return
 * None.
 */
void rfid_get_uid(uint8_t* uid);

/*!
 * @brief set unique id
 *
 * @par Description:
 * This function used to set the unique id of the RFID card.
 *
 * @param[in] uid  argument which will be filled inside this API with the four bytes
 *                      received by the response of the request in case of success.
 * @return
 * None.
 */
void rfid_set_uid(uint8_t* uid);

/*!@}*/

/*!
 * @addtogroup RFID_EVENT
 * @{
 */
/*!
 * @brief Show that there is a second left.
 *
 * @par Description:
 * This function used to show that there is a second left.
 *
 * @return
 * None.
 */
void rfid_sec_event(void);

/*!
 * @brief power down event.
 *
 * @par Description:
 * This function called when the meter enters power down mode in order to reset the state.
 *
 * @return
 * None.
 */
void rfid_pow_down_event(void);

/*!@}*/

/*!
 * @addtogroup RFID_CMD
 * @{
 */
/*!
 * @brief self test for rfid system
 *
 * @par Description:
 * This function used to write zeros to three different blocks then read blocks if true then write ones to blocks then read blocks then compare values.
 *
 * @return
 * None.
 */
uint8_t rfid_self_test(void);
/*!@}*/
#endif  /*rfid enable*/

/*!
 * @addtogroup RFID_CMD
 * @{
 */
/*!
 * @brief get rfid enabled feature system
 *
 * @par Description:
 * This function used to return the enable features as folowing bit 0  for rfid system enable (1) or not.
 *                                                                  1  for predefined parameter card feature if defined (1) if not (0).
 *                                                                  2  for postdefined parameter card feature if defined (1) if not (0).
 *                                                                  3  for card type feature if 4k card (1) if 1k card (0).
 *
 *
 * @return
 * uint8_t represents the features enabled.
 */
uint8_t rfid_get_feature(void);
/*!@}*/
/*!
 * @brief generate the encryption key from the input random key.
 *
 * @par Description:
 * This function generate the encryption key from the input random key.
 *
 * @param[in] key: represent the input random key as input/ and the generated encryption key as output.
 *
 * @return None.
 */
/*******************************************************************************************************************/
void rfid_generate_key(uint8_t* key);
uint8_t rfid_check_idle_state (void);

uint8_t rfid_write_state (void);
void rfid_measure_speed_card(void);
uint8_t rfid_bootloader_task(void);
void BootLoader_Clr_Flag(void);
uint16_t get_other_selling_data_after_one_block(uint16_t addr1);

/*------------------------------------------------------------------------------------
  from self_test.h
------------------------------------------------------------------------------------*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                  S E L F - T E S T

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file self_test.h
 * @brief This file contains Self-Test APIs prototype.
 * @details This file contains the required definitions, types and APIs
 * for self test.
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
 *   <td> 09/07/2012 </td>
 *   <td> mohamed.zakaria  </td>
 *   <td> Created  </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup SELF_TEST_SYS Self-Test System
 * @ingroup METER
 * Self-Test system provides functions for testing all moules of meter.\n
 * This module can be accessed through only APIs.
 * This module has one interface has API.\n
 *   -# Event Interface.
 *
 * @brief Responsible to testing all enabled moules of meter.
 */
/*!
 * @defgroup SELF_TEST_CORE_APIS Core APIs
 * @ingroup SELF_TEST_SYS
 * @brief SELF-TEST system entry function.
 */
/*!
 * @defgroup SELF_TEST_EVENT_APIS Event APIs
 * @ingroup SELF_TEST_SYS
 * @brief SELF-TEST events call back APIs.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
#include "ADC_Interface.h"

/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/
#ifdef V9360_ENABLE
#elif defined(V9261f_ENABLE)
#elif defined(V9203_ENABLE)
#elif defined(V9381_ENABLE)                      
#endif

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
/*!
 * @ingroup DISPLAY_EVENT_APIS
 * @par Description:
 * This enumerator contains display events.
 */
typedef enum
{
  SELF_TEST_READY_EVE = (uint8_t)1   /*!< Used to inform the system by apply self-test.*/
} self_test_event_t;

typedef enum{
  HW_PIN,
  ANSI_CMD,
  JIG_CMD,
}Self_test_source_t;


typedef enum
{
  ST_ADE_DRIVER,
  ST_ADE_COMM,
  ST_ADE_VOLTAGE,
  ST_ADE_CURRENT,
  ST_ADE_PF,
  ST_ADE_REV,
  ST_DISPLAY_EN,
  ST_RELAY,
  ST_VISUAL_EN,
  ST_EEPROM,  
  ST_NEUTRAL,//Reserved_1,
  BATTERY_EN,
  CT_WIRES,
  ST_JIG_SOURCE,
  Reserved_2,
  ST_UPDATED = 15,  
  
}ST_error_bit_t;

typedef enum
{
  SELFTEST_NO_TEST             = (uint8_t)0,  /*!< communication */
  SELFTEST_COMM_MODULE         = (uint8_t)1,  /*!< communication */
  SELFTEST_VISUAL_MODULE       = (uint8_t)2,  /*!< visual */
  SELFTEST_BUZZER_MODULE       = (uint8_t)3,  /*!< visual */
  SELFTEST_PHASEA_MODULE       = (uint8_t)4,  /*!< metering */
  SELFTEST_PHASEB_MODULE       = (uint8_t)5,  /*!< metering */
  SELFTEST_PHASEC_MODULE       = (uint8_t)6,  /*!< metering */
  SELFTEST_VOLT_PHASEA         = (uint8_t)7,  /*!< metering */
  SELFTEST_VOLT_PHASEB         = (uint8_t)8,  /*!< metering */
  SELFTEST_VOLT_PHASEC         = (uint8_t)9,  /*!< metering */
  SELFTEST_FREQ_MODULE         = (uint8_t)10,  /*!< metering */
  SELFTEST_RELAY_CLOSE_MODULE   = (uint8_t)11,  /*!< Relay */
  SELFTEST_RELAY_OPEN_MODULE    = (uint8_t)12,  /*!< Relay */
  SELFTEST_EEPROM_MODULE       = (uint8_t)13,  /*!< EEPROM */
  SELFTEST_FLASH_MODULE        = (uint8_t)14,  /*!< FLASH */
  SELFTEST_RFID_MODULE         = (uint8_t)15,  /*!< RFID */
  SELFTEST_BATTERY_MODULE      = (uint8_t)16,  /*!< Battery*/
  SELFTEST_OPTICAL_MODULE      = (uint8_t)17,  /*!< OPTICAL*/
  SELFTEST_SW_MODULE           = (uint8_t)18,  /*!< SW*/
  SELFTEST_MAG_MODULE          = (uint8_t)19,  /*!< mag SW*/
  SELFTEST_RS485_MODULE        = (uint8_t)20,  /*!< RS485 */
  SELFTEST_GPRS_MODULE         = (uint8_t)21,  /*!< GPRS */
  SELFTEST_ZERO_CUSTMER_MODULE = (uint8_t)22,  /*!< ZERO CUSTMER */
  SELFTEST_END                 = (uint8_t)23,  /*!< RS485*/
} SelfTest_Stats;
/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
extern volatile uint16_t Tooling_Counter;
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/

/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/*!
 * @addtogroup SELF_TEST_EVENT_APIS
 * @{
 */

/*!
 * @brief self-test event
 *
 * @par Description:
 * It is called from other modules to applying self-test by force event flag.
 *
 * @param[in] event_type the range is defined #self_test_event_t
 *
 * @return
 * None.
 */
void self_test_event(uint8_t event_type);
/*! @}*/

/*!
 * @addtogroup SELF_TEST_CORE_APIS
 * @{
 */
/*!
 * @brief Main entry point for SELF_TEST system
 *
 * @par Description:
 * It is main function called by main loop of the system periodically
 *
 * @return
 * - Returns true after testing finished.
 */
uint8_t self_test_task(void);
/*! @}*/

/*!
 * @brief Self_Test_Get_Error_Flag
 *
 * @par Description:
 * to return value of self test error.
 *
 * @return
 * - Returns self_test_error_mask.
 */

uint16_t Self_Test_Get_Error_Flag(void);
void SelfTest_gTask(void);
uint8_t SelfTest_gGetSelfTest(void);
void SelfTest_gStartSelfTest(void);
void SelfTest_gSetState(SelfTest_Stats Next_Module_State, uint8_t Next_State);
void SelfTest_gDecodeTestCommand(void);


/*! @}*/
#ifdef ANSI_STACK
void selfTestFrame_receiver(uint8_t Rx_Byte);
uint16 construct_self_test_ansi_result(void);
#endif



/*------------------------------------------------------------------------------------
  from soft_spi_drv.h
------------------------------------------------------------------------------------*/
/*
 * soft_spi_drv.h
 *
 * Created: 03/05/2018 09:59:59 ص
 *  Author: reda.abdelhameid
 */ 
#include "config.h" 

/* default pins */


//these macros used for any SW usage like SW SPI & DOT_MATRIX LCD to expand speed
#define Set_bit_GPIO(GPIOx ,GPIO_Pin)  (GPIOx->DAT |= GPIO_Pin)
#define Reset_bit_GPIO(GPIOx ,GPIO_Pin)  (GPIOx->DAT &= ~GPIO_Pin)

void spi_soft_init_master(void);
uint8_t spi_soft_read_byte(void);
void spi_soft_write_byte(uint8_t byte);
/**
  * @}
  */

/*------------------------------------------------------------------------------------
  from soft_spi_intf.h
------------------------------------------------------------------------------------*/
/*
 * soft_spi_intf.h
 *
 * Created: 03/05/2018 09:45:07 ص
 *  Author: reda.abdelhameid
 */ 





/********************************** Headers ***********************************/
/* #include "soft_spi_drv.h" -- inlined in this file */

/******************************************************************************/





/*******************************************************************************
            SPI Interface Configuration
*******************************************************************************/
/* This container contains the configuration parameters of the SPI Interface
 */

/********************** SPI Selected Module *****************************/
/* select the SPI Module is being used to interface*/

#define  SPI_SELCTED_MODULE   SPI_MODULE_2  /*two types of modules: SPI_MODULE_1
SPI_MODULE_2*/
/*---***---*/

/********************** Time out error *****************************/
/* determine time out duration*/

/*---***---*/

/*------------------------------------------------------------------------------
                           End of Configuration
  ----------------------------------------------------------------------------*/





/*******************************************************************************
                          Configuration Parameters
*******************************************************************************/


/*SPI_SELCTED_MODULE*/
/* it contains all parameters which selected by SPI_SELCTED_MODULE */
enum
{
  SPI_MODULE_1,
  SPI_MODULE_2
};
/*-------*/
#if 0
enum
{
	FALSE,     /* false = 0, true = 1 */
	TRUE,
};
#endif
/*------------------------------------------------------------------------------
              End of Configuration Parameters
  ----------------------------------------------------------------------------*/





/*******************************************************************************
                        Types definitions
 ******************************************************************************/

/*Indicates SPI interface state*/
typedef enum
{
	SPI_INF_READY ,
  SPI_INF_UNINIT  //= FALSE,
 // SPI_INF_READY  //= TRUE

} SPI_InfStateType;
/*------*/

/*Indicates SPI interface state*/
typedef enum
{
	  SPI_SENT   =(uint8_t)0,
  SPI_NOT_SENT =(uint8_t)1, //= (bool)FALSE, /*false*/
 // SPI_SENT      //= (bool)TRUE

} SPI_SentStatusType;
/*------*/

/*Indicates SPI interface state*/
typedef enum
{
	SPI_RECEIVED,
  SPI_NOT_RECEIVED  // = (bool)FALSE, /*false*/
//  SPI_RECEIVED      //= (bool)TRUE

} SPI_ReceivingStatusType;
/*------*/

/*------------------------------------------------------------------------------
                          END Types definitions
 -----------------------------------------------------------------------------*/





/*******************************************************************************
                         Global variables Flags
 ******************************************************************************/

/*Indicates the driver state un/initialized */
extern SPI_InfStateType  gSPI_INF_STATE;

/*------------------------------------------------------------------------------
            END Global variables Flags
 -----------------------------------------------------------------------------*/





/*******************************************************************************
                           Functions prototypes
 ******************************************************************************/

/*------This is a list of functions provide for upper layer modules.----------*/

/*Initializes SPI Module.called by upper layer*/
void vSPI_InfInit(void);

/*sending one byte from SPI*/
SPI_SentStatusType bSPI_InfSyncTxByte(uint8_t u8SPIbyte);

/*transmitting buffer array from SPI*/
SPI_SentStatusType bSPI_InfSyncTxBuf(uint8_t* buffer_ptr, uint16_t buff_size);

/*receiving byte from SPI*/
SPI_ReceivingStatusType bSPI_InfSyncRX(uint8_t* u8ptrSPI);

/*receiving bytes from SPI*/
SPI_ReceivingStatusType bSPI_InfSyncRXBuf(uint8_t* buffer_ptr, uint16_t buff_size);

/*------------------------------------------------------------------------------
              End function prototypes
 -----------------------------------------------------------------------------*/









/*------------------------------------------------------------------------------------
  from trf_pymt.h
------------------------------------------------------------------------------------*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           T A R I F F   &  P A Y M E N T

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file trf_pymt.h
 * @brief This file contains tariff and payment APIs prototypes.
 * @details This file contains the required definitions, types and APIs
 * for tariff and payment.
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
 *   <td> 27/03/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add scratch card interface. </td>
 * </tr>
 * <tr>
 *   <td> 28/02/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Reply by true regardless of charge number only in replacement card case. </td>
 * </tr>
 * <tr>
 *   <td> 10/02/13 </td>
 *   <td> islam.elshahat </td>
 *   <td> Use basic function for date compare and solve crc bug of tariff configuration. </td>
 * </tr>
 * <tr>
 *   <td> 30/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Solve bug of new billing period with a new customer. </td>
 * </tr>
 * <tr>
 *   <td> 13/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Get tariff and payment alarms states from control system. </td>
 * </tr>
 * <tr>
 *   <td> 09/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Modify charge number to become 2 Bytes and ability to reset configuration. </td>
 * </tr>
 * <tr>
 *   <td> 29/11/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add functions to read tariff, payment and tax configuration. </td>
 * </tr>
 * <tr>
 *   <td> 20/10/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Move charge money structure to c file. </td>
 * </tr>
 * <tr>
 *   <td> 06/09/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add support 2nd level of low cridit. </td>
 * </tr>
 * <tr>
 *   <td> 24/07/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Add money transactions logging. </td>
 * </tr>
 * <tr>
 *   <td> 08/07/2012 </td>
 *   <td> mohamed.zakaria  </td>
 *   <td> Solve bugs in reading APIs.  </td>
 * </tr>
 * <tr>
 *   <td> 07/06/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Update after implementation of file system. </td>
 * </tr>
 * <tr>
 *   <td> 07/03/11 </td>
 *   <td> islam elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup TRF_PYMT_SYS Tariff and Payment System
 * @ingroup METER
 * Tariff and payment system responsible for money management, billing period variables,
 * tariff calculations, etc….\n
 * All these calculations are done based on the configuration saved in EEPROM.
 * This module can be accessed through only APIs.
 * This module has four interfaces each one of these interfaces has number of APIs.\n
 *   -# Configuration Interface.
 *   -# Event Interface.
 *   -# Command Interface.
 *   -# Read System Variables Interface.\n
 *
 * \image html trf_pymt_sys.jpg "Tariff and Payment System Interfaces"
 * \n<b>References</b>\n
 * - Design document of tariff and payment system.
 * @brief Tariff and payment module calculating tariff consumption and manageing all billing.
 */
/*!
 * @defgroup TRF_PYMT_CORE Application Core APIs
 * @ingroup TRF_PYMT_SYS
 * @brief Tariff and payment system entry and initialization functions.
 */
/*!
 * @defgroup TRF_PYMT_CFG Configuration APIs
 * @ingroup TRF_PYMT_SYS
 * @brief Tariff and payment system confiuration functions and data types.
 */
/*!
 * @defgroup TRF_PYMT_READ Read System Variables APIs
 * @ingroup TRF_PYMT_SYS
 * @brief Used to read calculated variables by tariff and payment system.
 */
/*!
 * @defgroup TRF_PYMT_EVENT Events APIs
 * @ingroup TRF_PYMT_SYS
 * @brief Tariff and payment events call back APIs.
 */
/*!
 * @defgroup TRF_PYMT_CMD Commands APIs
 * @ingroup TRF_PYMT_SYS
 * @brief Tariff and payment command interface APIs.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
/////////#include "ctrl.h"   // REMOVE_METER_INTERFACE
#if defined (SPECIAL_DAYS_ENABLE) && defined (NEW_DESIGN_OPTIMIZE)
/* #include "Seasonal_Tarrif/Special_Days.h" -- inlined in this file */
#endif
/*  For more info about this warning look at
 *  http://e2e.ti.com/support/microcontrollers/msp430/f/166/t/53942.aspx
 */
#ifdef __IAR_SYSTEMS_ICC__
#pragma diag_suppress=Pa039
#endif

/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
/*!
 * @par Description:
 * This enumerator contains all available events that may be published by the module of
 * tariff and payment.
 */
typedef enum
{
  TRF_PYMT_LOW_CRDT_FRST_ALRM = (uint8_t)0,        /*!< Low Credit Event first alarm.*/
#ifdef PYMT_LOW_TWO_LVL
  TRF_PYMT_LOW_CRDT_SCND_ALRM = (uint8_t)1,        /*!< Low Credit Event second alarm.*/
#endif
  TRF_PYMT_NO_CRDT  = (uint8_t)2,        /*!< No Credit Event*/
  TRF_PYMT_NO_CRDT_WITH_FRNDLY  = (uint8_t)3,  /*!< No Credit but we are at friendly time Event*/
  TRF_PYMT_DATA_CRPT = (uint8_t)4        /*!< Data Corrupt Event (CRC check failed)*/
} trf_pymt_events_pblsh_by_trf_pymt_t;

/*!
 * @ingroup TRF_PYMT_READ
 * @par Description:
 *   This enumerator contains all available configuration options for tariff types.
 */
typedef enum
{
  TRF_PYMT_STEP_TRF = (uint8_t)0, /*!< Step tariff mode.*/
  TRF_PYMT_TOD_TRF = (uint8_t)1,  /*!< Time of use tariff mode.*/
} trf_type_t;

/*!
 * @ingroup TRF_PYMT_CMD
 * @par Description:
 *   This enumerator contains all available configuration options for system.
 */
typedef enum
{
  TRF_PYMT_INT_OPTICAL = (uint8_t)0, /*!< Command through optical interface.*/
  TRF_PYMT_INT_GPRS = (uint8_t)1,    /*!< Command through GPRS interface.*/
  TRF_PYMT_INT_RFID = (uint8_t)2,    /*!< Command through RFID interface.*/
  TRF_PYMT_INT_SCRACH = (uint8_t)3,   /*!< Command through scratch card.*/
  TRF_PYMT_INT_MAX,
} trf_pymt_interfaces_t;

/*!
 * @ingroup TRF_PYMT_CFG
 * @par Description:
 *   This enumerator contains all available configuration options for system.
 */
typedef enum
{
  TRF_PYMT_CFG_VAC    = (uint8_t)0, /*!< Vacation Configuration Data.*/
  TRF_PYMT_CFG_FRNDLY = (uint8_t)1, /*!< Friendly Configuration Data.*/
  TRF_PYMT_CFG_PYMT   = (uint8_t)2, /*!< Payment Configuration Data.*/
  TRF_PYMT_CFG_TRF    = (uint8_t)3, /*!< Tariff Configuration Data.*/
  TRF_PYMT_CFG_TAX    = (uint8_t)4, /*!< Tax Configuration Data.*/
  TRF_PYMT_CFG_BP     = (uint8_t)5, /*!< Billing Period Configuration Data.*/
  TRF_PYMT_CFG_FETUR  = (uint8_t)6  /*!< Defined Features Configuration Data.*/
} trf_pymt_cfg_cmd_t;

#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_EVENT
 * @par Description:
 *   Events to inform tariff and payment module with
 */
typedef enum
{
  TRF_PYMT_EVNT_MIN   = (uint8_t)0x01,   /*!< New Minute Event.*/
  TRF_PYMT_EVNT_ENERGY  = (uint8_t)0x02, /*!< New Energy Event.*/
  TRF_PYMT_EVNT_PWR_UP  = (uint8_t)0x04, /*!< Power Up Event (Not used).*/
  TRF_PYMT_EVNT_PWR_DN  = (uint8_t)0x08, /*!< Power Down Event.*/
  TRF_PYMT_EVNT_MD  = (uint8_t)0x10,     /*!< New Maximum Demand Event.*/
  TRF_PYMT_NEW_CUSTOMER = (uint8_t)0x20, /*!< There is an pdate occur.*/
  TRF_PYMT_REPLACEMENT_CARD = (uint8_t)0x40, /*!< There is an pdate occur.*/
  TRF_PYMT_EVNT_SAVE_DATA = (uint8_t)0x80,  /*!< Save tariff data.*/
  TRF_PYMT_EVNT_SEC,
  TRF_PYMT_EVNT_DATE_TIME_CONFIG,
  TRF_PYMT_EVNT_PWR_DN_2,
} trf_pymt_events_t;

/*!
 * @ingroup  TRF_PYMT_CMD
 * @par Description:
 *   Commands used by command interface for tariff and payment module.
 */
typedef enum
{
#ifdef PAYMENT_SYS
  TRF_PYMT_CMD_CHRG   = (uint8_t)0,   /*!< Charge Command.*/
  TRF_PYMT_CMD_DISCHRG  = (uint8_t)1, /*!< Discharge Command.*/
#endif
  TRF_PYMT_CMD_RST  = (uint8_t)2,     /*!< Reset Command.*/
} trf_pymt_cmd_interface_t;

#endif
#if ((defined PYMT_VACATION_TRF ) || (defined PYMT_FRIENDLY))
/*!
 * @par Description:
 *   This enumerator contains the options of day status.
 */
typedef enum
{
  DAY_WORK = (uint8_t)0x00,    /*!< Working day.*/
  DAY_WEEKEND = (uint8_t)0x01, /*!< Weekend day.*/
  DAY_VAC = (uint8_t)0x02,     /*!< Vacation day.*/
} vac_day_t;
#endif
typedef enum
{
  ENERGY_SAVING_CHECK = (uint8_t)0x00,   
  ENERGY_SAVING_ACTIVE = (uint8_t)0x01,
  WAITING_CUTOFF_TIME = (uint8_t)0x02,     
} enrgy_days_t;
#if (defined(PYMT_VACATION_TRF)||defined(PYMT_FRIENDLY))
/*!
 * @ingroup  TRF_PYMT_CFG
 * One of configuration structures for vacation configuration.
 * @brief Vacation Configuration.
 */
__packed typedef struct
{
#ifdef G2_DM
  uint8_t days[48]; /*!< Vacation Days along the year. All months are considered 31 days.*/
#else
  uint8_t days[47]; /*!< Vacation Days along the year. All months are considered 31 days.*/
#endif
  uint8_t wk_end;   /*!< Each bit represents a day. Bit 0 for Sunday,...,Bit 6 for Saturday.*/
} vac_t;
#endif

#ifdef PYMT_FRIENDLY
/*!
 * @ingroup  TRF_PYMT_CFG
 * One of configuration structures for friendly configuration.
 * @brief Friendly time configuration.
 * @note I
 * - If friendly is adjusted start=1 and end=5,
 *   then 1:30 is in friendly time but 5:30 is not in friendly time.
 * - If friendly start hour is adjusted to be equal to friendly end hour
 *   then all the time is dealed as friendly.
 */
__packed typedef struct
{
  uint8_t start;   /*!< Friendly start hour. Valid 11->23.*/
  uint8_t end;     /*!< Friendly end hour. Valid 1->10.*/
  uint8_t enable;  /*!< Enable friendly feature.*/
} frndly_t;
#endif

#ifdef PAYMENT_SYS
/*!
 * @ingroup  TRF_PYMT_CFG
 * One of configuration structures for payment configuration.
 * @brief Payment configuration
 */
__packed typedef struct
{
  uint8_t type;            /*!< Prepaid or basic according to #payment_type_t */
  uint8_t lmt_avg_calc;    /*!< Period to calculate average on. 1-7 */
  uint8_t lmt_crdt_day[PMYT_LVL];    /*!< Limit of remaining days.(0, 255)*/
  uint16_t lmt_crdt_kwh[PMYT_LVL];   /*!< Limit of remaining kWh.(0, 65535)*/
  uint16_t lmt_crdt_money[PMYT_LVL]; /*!< Limit of remaining money of local currency.(0, 65535)*/
  uint8_t lmt_ctrl[PMYT_LVL];        /*!< Low credit event control.For more details see design document.*/
} pymt_t;
#endif

__packed typedef struct
{
  uint32_t  mda_value;
  uint32_t  mdw_value;
  
}trf_MD_rec_t;
#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_CFG
 * One of configuration structures for tariff configuration.
 * @brief Tariff configuration.
 */
__packed typedef struct
{
  uint8_t num;            /*!< Number of used tariffs valid values 1 to 10.*/
  uint8_t type;           /*!< #trf_type_t .*/
#ifdef MTR_SINGLE_PH
#ifdef PYMT_VACATION_TRF
#ifndef ASHNTTI_PROJECT
  uint8_t vac_time;       /*!< Decide if vacation time includes vacation
                          days and weekend days.*/
#endif
#endif
#endif
  uint8_t start_new;      /*!< Start new tariff config options #start_new_trf_t.*/
  uint8_t start_date[3];  /*!< Date to start the new tariff config.
                           * @note
                           *   start_date[0] Day.\n
                           *   start_date[1] Month.\n
                           *   start_date[2] Year. Considered that 0 stands for year 2000.\n
                           */
#ifdef G2_DM
  uint32_t steps_kwh_min[10]; /*!< Used to define the upper boundary of the steps.*/
#else
  uint32_t steps_kwh_min[9]; /*!< Used to define the upper boundary of the steps.*/
#endif
                              /* Value is by kWh for step tariff and by minutes in case of TOD.
                              * @note
                              * No of defined boundaries = No of used steps-1.
                              */
#ifdef PAYMENT_SYS
  uint32_t prices[10];      /*!< The price of each step in 10^-4 of local currency / kWh.*/
#endif
#ifdef MTR_SINGLE_PH
#ifdef PYMT_VACATION_TRF
#ifndef ASHNTTI_PROJECT
  uint32_t vac_prices[10];  /*!< The price of each step in vacation days in 10^-4 of currency / kWh.*/
#endif
#endif
#endif
#ifdef PYMT_TAX
#ifdef G2_DM
  uint32_t swtch_tax[10];   /*!< The added tax when switching between steps in case of step*/
#else
  uint32_t swtch_tax[9];   /*!< The added tax when switching between steps in case of step*/
#endif
                               /* tariff in 10^-4 of currency*/
#endif
  uint8_t crc;             /*!< CRC of tariff only. used for waiting tariff config.*/
} trf_t;
#endif


__packed typedef struct
{
  uint8_t flag;
  uint32_t LPM_elapsed;
  uint8_t crc;
} trf_grace_elapsed_time_t;
  

#ifdef PYMT_TAX
/*!
 * @ingroup  TRF_PYMT_CFG
 * One of configuration structures for tariff configuration.
 * @brief Tax configuration.
 */
__packed typedef struct
{
  uint32_t fxd_day;   /*!< Every day tax by 10^-4 of local country currency*/
  uint32_t fxd_bp;    /*!< Every BP tax by 10^-4 of local country currency*/
  uint8_t prcnt_bp;   /*!< Perctange from the final bill at end of BP. Valid values: 0-100*/
} tax_t;
#endif

#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_CFG
 * One of configuration structures for tariff configuration.
 * @brief Billing period configuration.
 */
__packed typedef struct
{
  uint8_t start;   /*!< Bill start day valid from 1 to 28*/
  uint8_t period;  /*!< Bill period by month valid from 1 to 12*/
#ifndef MTR_THREE_PH
#ifndef ASHNTTI_PROJECT
  uint8_t grace_period; //TODO: POSTPAID
#endif
#endif
} bp_t;
#endif

#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_CFG
 * The big configuration structure consisting of six smaller structures.
 * @note Configuration structures sizes depend on the defined features.
 * @brief Contains all system configuration data.
 */
__packed typedef struct
{
#if (defined(PYMT_VACATION_TRF)||defined(PYMT_FRIENDLY))
  vac_t vac;          /*!< Vacation configuration structure.*/
#endif

#ifdef PYMT_FRIENDLY
  frndly_t frndly;    /*!< Friendly configuration structure.*/
#endif

#ifdef PAYMENT_SYS
  pymt_t pymt;        /*!< Payment configuration structure.*/
#endif

#ifdef TARIFF_SYS
  trf_t trf;          /*!< Tariff configuration structure.*/
#endif

#ifdef PYMT_TAX
  tax_t tax;          /*!< Tax configuration structure.*/
#endif

#ifdef TARIFF_SYS
  bp_t bp;            /*!< Billing period configuration structure.*/
#endif
  uint8_t crc;        /*!< CRC of configuration*/
} trf_pymt_cfg_t;
#endif

#ifdef  TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_READ
 * One of system variables structures for tariff data.
 * @brief Tariff data.
 */
__packed typedef struct
{
  uint32_t  curr_bp_actv_wh;      /*!< Consumption of current BP (Wh).*/
  uint32_t  prv_hr_actv_wh;       /*!< Previous hour consumption (Wh).*/
  uint32_t  prv_days_actv_wh[7];  /*!< Previous 7 days consumption (Wh).prev_days_actv_wh[6] is the yesterday data*/
  uint32_t  prv_wk_actv_wh;       /*!< Previous week consumption (Wh).*/
  uint32_t  prv_bp_actv_wh;       /*!< Previous BP consumption (Wh).*/
  uint32_t  step_rem_wh_or_min;   /*!< Remaining for next step.
                                       @note Calculation of remaining for next
                                       step doesn't depend on money balance.*/
  uint32_t  actv_trf_step_wh[10]; /*!< Consumption of each tariff (Wh).*/
  uint8_t   curr_num;             /*!< Current used tariff number.Valid values from 0 to 9.*/
} trf_data_t;
#endif

#if  (defined(TRF_USE_MD_KW) || defined(TRF_USE_MD_KVA) || defined(TRF_USE_MD_A))
/*!
 * @ingroup  TRF_PYMT_READ
 * Maximum demand data used to keep current or previous MD data.
 * @brief Maximum demand structure.
 */
__packed typedef struct
{
  uint32_t md_value;  /*!< Maximum demand value*/
  uint8_t min;  /*!< Maximum demand minute*/
  uint8_t hr; /*!< Maximum demand hour*/
  uint8_t day; /*!< Maximum demand day*/
  uint8_t month; /*!< Maximum demand month*/
  uint8_t year; /*!< Maximum demand year*/
} trf_pymt_md_t;
#endif

#if (defined(TRF_USE_MD_KW) || defined(TRF_USE_MD_KVA) || defined(TRF_USE_MD_A))
/*!
 * @ingroup  TRF_PYMT_READ
 * One of system variables structures for maximum demand data independent of MD type.
 * @brief Maximum demand parameters.
 */
__packed typedef struct
{
  trf_pymt_md_t prv; /*!< Previous maximum demand data(Watt,VA, mA).*/
  trf_pymt_md_t curr; /*!< Current maximum demand data(Watt,VA, mA).*/
  uint32_t  curr_md_per_trf[10];  /*!< Current MD per tariff (Watt,VA, mA).*/
} trf_pymt_md_par_t;
#endif

#ifdef PAYMENT_SYS
/*!
 * @ingroup  TRF_PYMT_READ
 * One of system variables structures for payment data.
 * @brief Payment data.
 */
__packed typedef struct
{
  uint32_t  curr_bp_bill;       /*!< Current billing period bill*10^-4 local currency.*/
  int32_t  prv_hr_bill;        /*!< Previous hour bill (10^-4 local currency).*/
  int32_t  prv_days_bill[7];   /*!< Previous 7 days bill (10^-4 local currency).prev_days_bill[6] is yesterday bill*/
  int32_t  prv_wk_bill;        /*!< Previous week bill (10^-4 local currency).
                                   * @note This variable is updated each day.\n
                                   * Previous week begins from yesterday not a fixed day.*/
  uint32_t  prv_bp_bill;        /*!< Previous billing period bill*10^-4 local currency.*/
  uint32_t  total_life_chrg_e_ve_3_crncy; /*!< Total life charge*10^-3 local currency.*/
  uint32_t  curr_bp_chrg_e_ve_3_crncy;    /*!< Current billing period charge*10^-3 local currency.*/
  uint32_t  prv_bp_chrg_e_ve_3_crncy;     /*!< Previous billing period charge*10^-3 local currency.*/
  int32_t   money_balance;       /*!< Money balance in (10^-4 local currency).*/
  uint32_t  rem_wh;             /*!< Expected remaining active energy by wh.*/
  uint16_t  rem_days;            /*!< Expected remaining days.*/
  uint16_t  chrg_num[TRF_PYMT_INT_MAX];   /*!< Charge number from different interfaces. #trf_pymt_interfaces_t*/
  uint16_t  dischrg_num[TRF_PYMT_INT_MAX];/*!< Discharge number from different interfaces. #trf_pymt_interfaces_t*/
} pymt_data_t;
#endif

#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_READ
 * The big system variables structure consisting of four smaller structures.
 * @brief System variables structure
 */
__packed typedef struct
{
  uint16_t  fraction_wh;          /*!< fraction of KWH.*/
  uint32_t  curr_hr_actv_wh;      /*!< Current hour consumption (Wh).*/
  uint32_t  curr_day_actv_wh;     /*!< Current day consumption (Wh).*/
  uint8_t pblsh_evnt_flag; /*!< To save published events.to avoid publishing a published event.*/
  uint8_t prv_hr;                 /*!< Previous hour value from RTC used to check hour switching.*/
  uint8_t prv_day;                /*!< Previous day value from RTC used to check day switching.*/
  uint8_t prv_month;              /*!< Last BP date month.*/
  uint8_t prv_year;               /*!< Last BP date year.*/
  trf_data_t  trf_data;           /*!< Tariff data structure.*/
#ifdef PAYMENT_SYS
  int32_t  curr_hr_bill;     /*!< Current hour bill (10^-4 local currency).*/
  int32_t  curr_day_bill;      /*!< Current day bill (10^-4 local currency).*/
  pymt_data_t pymt_data;        /*!< Payment data structure.*/
#endif
#ifdef PYMT_FRIENDLY
  uint8_t frndly_status;        /*!< Friendly status.*/
#endif
#ifdef  TRF_USE_MD_KW
  trf_pymt_md_par_t  md_actv_pwr;   /*!< Maximum demand of active power data structure.*/
#endif
#ifdef  TRF_USE_MD_KVA
  trf_pymt_md_par_t  md_aprnt_pwr; /*!< Maximum demand of apparent power data structure.*/
#endif
#ifdef TRF_USE_MD_A
  trf_pymt_md_par_t  md_current;     /*!< Maximum demand of current data structure.*/
#endif
  uint8_t crc;          /*!< CRC os system variables.*/
} trf_pymt_var_t;
#endif



#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_DATA_TYPES
 * Structure used to read time now from RTC.
 * @brief Time structure.
 */
__packed typedef struct
{
  uint8_t day_of_week;  /*!< Day of week Sunday is represented by 0(0-6).*/
  uint8_t min;          /*!< Minutes(0-59).*/
  uint8_t hr;           /*!< Hours, 24 hour system(0-23).*/
  uint8_t day;          /*!< Day(1-31).*/
  uint8_t month;        /*!< Month(1-12).*/
  uint8_t year;       /*!< Year, 0 represents year 2000.*/
} trf_pymt_date_time_t;

#ifdef TRF_BP_HISTORY
/*!
 * @ingroup  TRF_PYMT_READ
 * Billing period history record structure.
 * @brief BPH record.
 * @note Strucure size depends on payment feature definition.
 */
#ifdef MTR_SINGLE_PH
__packed typedef struct
{

  uint32_t  consumption_wh; /*!< Billing period consumption in Wh.*/  
#ifndef ASHNTTI_PROJECT
  uint32_t  mda;
#endif
  uint32_t  bill;
#ifndef ASHNTTI_PROJECT
  int32_t  diff_debit_recalc ;   /* difference debits result due to change recalculations accounts*/
  uint32_t tax_customer_serv ;  /* the total customer service taxes */
  int32_t  money_balance ;
  uint32_t  mdw;
#endif
#ifdef G2_DM
  uint32_t mdvar;
  mtr_date_time_t mda_date_time;
  mtr_date_time_t mdw_date_time;
  mtr_date_time_t mdvar_date_time;
  uint8_t   pf;

  uint8_t   min;
  uint8_t   hour;
#endif
  uint8_t   day;          /*!< Day of bill.*/
  uint8_t   month;        /*!< Month of bill.*/
  uint8_t   year;         /*!< Year of bill.*/
} trf_pymt_bph_rec_t;


#else // three phase


__packed typedef struct
{
  uint32_t  consumption_wh; /*!< Billing period consumption in Wh.*/
  uint32_t  bill;         /*!< Billing period bill(10^-4 local currency).*/
#ifndef ASHNTTI_PROJECT  
  uint32_t  mda;         /*!< Max demand amper>*/
  uint32_t  mdw;         /*!< Max demand amper>*/
  uint32_t  consumption_varh;
#endif
#ifdef G2_DM
  int32_t  money_balance;
  uint32_t mdvar;
  mtr_date_time_t mda_date_time;
  mtr_date_time_t mdw_date_time;
  mtr_date_time_t mdvar_date_time;
  uint8_t   pf;

  uint8_t   min;
  uint8_t   hour;
#endif
  uint8_t   day;          /*!< Day of bill.*/
  uint8_t   month;        /*!< Month of bill.*/
  uint8_t   year;         /*!< Year of bill.*/
} trf_pymt_bph_rec_t;


#endif
#endif
#endif

#ifdef PYMT_MONY_TRANS
/*!
 * @ingroup TRF_PYMT_READ
 * Money transaction record structure.
 * @brief Money record.
 */
__packed typedef struct
{
  int32_t final_money_balance; /*!< Money balance after transaction in 10^-4 of currency.*/
  uint32_t chrg_value; /*!< Charge or discharge money value as positive number in 10^-4 of currency.*/
  uint16_t used_chrg_number; /*!< Used charge or discharge number.*/
  uint8_t type; /*!< Charge or discharge #money_charge_type_t.*/
  uint8_t interface; /*!< Interface from which transaction is done #trf_pymt_interfaces_t.*/
  uint8_t minute; /*!< Minute of transaction.*/
  uint8_t hour; /*!< Hour of transaction.*/
  uint8_t day; /*!< Day of transaction.*/
  uint8_t month; /*!< Month of transaction.*/
  uint8_t year; /*!< Year of transaction.*/
} trf_pymt_mny_rec_t;
#endif

#ifdef PAYMENT_SYS
/*!
 * @ingroup  TRF_PYMT_CMD
 * Charging or discharging data strucure.
 * @brief Charging or discharging data strucure.
 */
__packed typedef struct
{
  uint16_t num;         /*!< Charging number.*/
  uint8_t interface;    /*!< Interface type of #trf_pymt_interfaces_t.*/
  int32_t money;        /*!< Charging Money (10^-4 local currency).*/
} chrg_data_t;

#endif
/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
#ifdef TARIFF_SYS
extern trf_pymt_cfg_t trf_pymt_cfg;    /*!< Big configuration structure of #trf_pymt_cfg .*/
extern trf_pymt_var_t trf_pymt_var;    /*!< System variable structure of #trf_pymt_var .*/
extern uint8_t evnt_flags; /*!< Used to detect a coming events.*/
#ifdef PYMT_MONY_TRANS
extern trf_pymt_mny_rec_t trf_chrg_mny_rec;
#endif
#endif
extern uint8_t trf_old_activity;
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
extern trf_grace_elapsed_time_t trf_grace_elapsed_time;

#endif
#ifdef G2_DM
extern uint8_t saving_days_status;
#endif
extern uint8_t calc_bill_flag;
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/
/*!
 * @addtogroup TRF_PYMT_READ
 * @{
 */
#ifdef TARIFF_SYS
/*! Active energy consumption during friendly time by Wh.*/
/*! Active energy consumption during current billing period by Wh.*/
/*! Active energy consumption during previous hour by Wh.*/
/*! Active energy consumption during previous day by Wh.*/
/*! Active energy consumption during day by Wh.*/
/*! Active energy consumption during previous week by Wh.*/
/*! Active energy consumption during previous hour by Wh.*/
/*! Number of tariffs from 1-10.*/
/*! Tariff type (step, TOD).*/
/*! Current tariff number from 0 to 9. 0 mean 1st tariff*/
/*! Upper boundary of a step by kWh or minute. Step_num ranges from 0 to 8.*/
#ifdef PAYMENT_SYS
/*! Price of tariff with number step_num by 10^-4 of local currency. step_num ranges from 0 to 9.*/
#endif
#ifdef PYMT_VACATION_TRF
/*! Price of vacation tariff with number step_num by 10^-4 of local currency. step_num ranges from 0 to 9.*/
#endif
/*! Remaining for next step by Wh or minute.*/
/*! Consumption in each tariff by wh. step_num ranges from 0 to 9.*/
#endif

/* MAXIMUM DEMAND */
#ifdef TRF_USE_MD_KW
/*! Current maximum demand of active power by Watt.*/
/*! Minute at which current MD happened.*/
/*! Hour at which current MD happened.*/
/*! Day at which current MD happened.*/
/*! Month at which current MD happened.*/
/*! Year at which current MD happened.*/
/*! Previous maximum demand of active power by Watt.*/
/*! Minute at which previous MD happened.*/
/*! Hour at which previous MD happened.*/
/*! Day at which previous MD happened.*/
/*! Month at which previous MD happened.*/
/*! Year at which previous MD happened.*/
/*! Active power maximum demand of each tariff by Watt. selected_trf ranges 0-9.*/
#endif

#ifdef  TRF_USE_MD_KVA
/*! Current maximum demand of apparent power by VA.*/
/*! Minute at which current MD happened.*/
/*! Hour at which current MD happened.*/
/*! Day at which current MD happened.*/
/*! Month at which current MD happened.*/
/*! Year at which current MD happened.*/
/*! Previous maximum demand of apparent power by VA.*/
/*! Minute at which previous MD happened.*/
/*! Hour at which previous MD happened.*/
/*! Day at which previous MD happened.*/
/*! Month at which previous MD happened.*/
/*! Year at which previous MD happened.*/
/*! Apparent power maximum demand of each tariff by VA. selected_trf ranges 0-9.*/
#endif

#ifdef  TRF_USE_MD_A
/*! Current maximum demand of electrical current by mA.*/
/*! Minute at which current MD happened.*/
/*! Hour at which current MD happened.*/
/*! Day at which current MD happened.*/
/*! Month at which current MD happened.*/
/*! Year at which current MD happened.*/
/*! Previous maximum demand of electrical current by mA.*/
#define    TRF_PYMT_GET_PRV_MD_A()        trf_pymt_var.md_current.prv.md_value
/*! Minute at which previous MD happened.*/
/*! Hour at which previous MD happened.*/
/*! Day at which previous MD happened.*/
/*! Month at which previous MD happened.*/
/*! Year at which previous MD happened.*/
/*! Electrical current maximum demand of each tariff by mA. selected_trf ranges 0-9.*/
#endif

/* PAYMENT */
#ifdef  PAYMENT_SYS
/*! Current billing period bill by 10^-4 of local currency.*/
/*! Previous hour bill by 10^-4 of local currency.*/
/*! Previous day bill by 10^-4 of local currency.*/
/*! Previous week bill by 10^-4 of local currency.*/
/*! Previous billing period bill by 10^-4 of local currency.*/
/*! Total charing money during meter life by 10^-3 of local currency.
@note Discharged money is not deducted from this variable(non decreasing variable).*/
/*! Current billing period charing money by 10^-3 of local currency.
@note Discharged money is not deducted from this variable(non decreasing variable).*/
/*! Previous billing period charing money by 10^-3 of local currency.
@note Discharged money is not deducted from this variable(non decreasing variable).*/
/*! Money Balance by 10^-4 of local currency.*/
/*! Expected remaining active energy by Wh.*/
/*! Expected remaining days.*/
/*! Limit of money by in local currency. Money is considered low under this value for 1st alarm.*/
/*! Limit of active energy by kWh. Remaining active energy is consdered low under this limit for 1st alarm.*/
/*! Limit of days. Remaining days are consdered low under this limit for 1st alarm.*/
#ifdef PYMT_LOW_TWO_LVL
/*! Limit of money by in local currency. Money is considered low under this value for 2nd alarm.*/
/*! Limit of active energy by kWh. Remaining active energy is consdered low under this limit for 2nd alarm.*/
/*! Limit of days. Remaining days are consdered low under this limit for 2nd alarm.*/
#endif
#endif
/* TAX */
#ifdef  PYMT_TAX
/*! Fixed day tax by 10^-4 of local currency*/
/*! Fixed billing period tax by 10^-4 of local currency*/
/*! Percentage tax by added every billing period to the BP bill*/
/*! Switching between tariff tax. selected_trf ranges 0-8.*/
#endif

/*FRIENDLY STATUS*/
#ifdef  PYMT_FRIENDLY
/*! Boolean, If we are at friendly time or no.*/
#endif

#ifdef TRF_BP_HISTORY
/*! Get size of the BPH record structure.*/
#endif

#ifdef PYMT_MONY_TRANS
/*! Get size of a money transaction record.*/
#endif

/*! Current billing period reactve consumption*/
#define TRF_PYMT_CURR_BP_REACTV_VARH ctrl_sys_var_optional_feature.curr_bp_reactv_varh    

/*! Previous billing period reactve consumption*/  
/*!@}*/
/*! - Replacement card flag set.*/
/*! - Replacement card flag reset.*/
/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
/*!
 * @addtogroup TRF_PYMT_CORE
 * @{
 */
#ifdef TARIFF_SYS
/*!
 * @brief Reset tariff and payment configuration
 *
 * @par Description:
 * Return the configuration to its defaults and save it in EEPROM.
 *
 * @return
 * - None.
 */
void trf_pymt_reset_cfg(void);
#endif

/*!
 * @brief Initialize all variables and load data from EEPROM
 *
 * @par Description:
 * Load configuration and system variables from EEPROM and assign to system variables
 *
 * @return
 * - None.
 */
void trf_pymt_init(void);

/*!
 * @brief The basic function of tariff and payment
 *
 * @par Description:
 * Do all tasks related to tariff and payment according to the new events:
 * - New Energy
 * - New Minute
 * - New Maximum Demand
 *
 * @return
 * - None.
 *
 * @note
 * - Hour Bill doesn't include daily taxes.\n
 * - Day bill doesn't include BP taxes.\n
 * - Switching tax is included in (day, hr, BP) bills.
 */
void trf_pymt_task(void);
/*!@}*/

/*!
 * @addtogroup TRF_PYMT_CFG
 * @{
 */
#ifdef TARIFF_SYS
/*!
 * @brief This API moves part of configuration data from a buffer to one of the six
 * configuration structures and also save in the EEPROM
 *
 * @par Description:
 * According to a certain command, part of configuration data is moved to the configuration
 * structure in RAM and also saved in its corresponding part in EEPROM.\n
 * If the coming configuration is for tariff, then it won't be directly saved to its
 * corresponding part in RAM and EEPROM. As configuration of tariff has an option to be
 * saved with the next BP or at certain date in the future. Hence, we should keep the
 * current tariff config beside the new config to be applied at the required time.
 *
 * @param [in]  config_cmd  This is an unsigned char that takes one of #trf_pymt_cfg_cmd_t values
 * @param [in]  data   The address of the first byte in the buffer to read the data from
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved from the buffer.
 * If an invalid command is sent, 0 is returned and nothing is written to the structure
 *
 * @note
 * - The configuration structures' sizes differ according to the defined features.
 * - Features bytes are read only (can't be set).
 */
uint8_t trf_pymt_config_set(uint8_t config_cmd, uint8_t *data);
#endif

/*!
 * @brief This function copies one of the seven tariff and payment configuration sturctures
 * to a data buffer.
 *
 * @par Description:
 * This function copies one of the seven structures of tariff and payment configuration to
 * a data buffer. There are seven different commands used to read these structures.
 *
 * @param [in]   config_cmd  This is an unsigned char that takes one of #trf_pymt_cfg_cmd_t values
 * @param [out]  data  The address of the first byte in the buffer to get the data in
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved from the buffer.
 * If an invalid command is sent, 0 is returned and nothing is written to the structure
 *
 * @note
 * - The configuration structures' sizes differ accordning to the defined features.
 */
uint8_t trf_pymt_config_get(uint8_t config_cmd, uint8_t *data);
/*!@}*/

/*!
 * @addtogroup TRF_PYMT_READ
 * @{
 */
#ifdef TARIFF_SYS
/*!
 * @brief This function copies all running values of data related to tariff to a buffer.
 *
 * @par Description:
 * This function copies system variables related to tariff #trf_data_t to a buffer with the
 * address passed to the function.
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_tariff_data(uint8_t  *data);
#endif

#ifdef TARIFF_SYS
/*!
 * @brief This function copies all tariff configuration to a buffer.
 *
 * @par Description:
 * This function copies all configuration related to tariffs.
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_tariff_cfg(uint8_t  *data);
#endif

#ifdef PAYMENT_SYS
/*!
 * @brief This function copies all payment configuration to a buffer.
 *
 * @par Description:
 * This function copies all configuration related to tariffs.
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_payment_cfg(uint8_t  *data);
#endif

#ifdef PYMT_TAX
/*!
 * @brief This function copies all tax configuration to a buffer.
 *
 * @par Description:
 * This function copies all configuration related to tariffs.
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_tax_cfg(uint8_t  *data);
#endif

#ifdef TRF_BP_HISTORY
/*!
 * @brief Get number of billing history records.
 *
 * @par Description:
 * This function get billing period history record number saved in EEPROM.
 *
 * @return
 * - uint8_t to represent the number of saved records.
 */
#ifdef FLASH_FM25W32_ENABLE
uint16_t trf_pymt_get_bph_num(void);
#else
uint8_t trf_pymt_get_bph_num(void);
#endif

/*!
 * @brief This function copies a billing period history record to a buffer.
 *
 * @par Description:
 * This function copies a billing period history record  #trf_pymt_bph_rec_t to a buffer.
 * The number of the billing period history record required is passed to the function.
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in.
 * @param [in]  record  The number of the record required ranges from 0 to #TRF_BP_HISTORY_RECORDS - 1
 *
 * @return
 * - None.
 */
void trf_pymt_get_bph_rec(uint8_t* data, uint8 record);

/*!
 * @brief This function copies any number of billing period history records to a buffer.
 *
 * @par Description:
 * This function copies any number of billing period history records #trf_pymt_bph_rec_t to a buffer.
 *
 * @param [out] data_ptr  The address of the first byte in the buffer to get the data in.
 * @param [in] from Index of beginning record (0 to #TRF_BP_HISTORY_RECORDS-1).
 * @param [in] num_rec Number of records required (1 to #TRF_BP_HISTORY_RECORDS ).
 *
 * @return
 * 16 bit positive number representing the number of bytes moved to the buffer.
 */
uint16_t trf_pymt_get_bph_mlti_rec(uint8_t* data_ptr, uint16_t from, uint16_t num_rec);
#endif

#ifdef  TRF_USE_MD_KW
/*!
 * @brief This function copies all active power maximum demand data to a buffer.
 *
 * @par Description:
 * This function copies all active power maximum demand data #trf_pymt_md_par_t to a buffer
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in.
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_md_kw_data(uint8_t *data);
#endif

#ifdef  TRF_USE_MD_KVA
/*!
 * @brief This function copies all apparent power maximum demand data to a buffer.
 *
 * @par Description:
 * This function copies all apparent power maximum demand data #trf_pymt_md_par_t to a buffer
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in.
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_md_kva_data(uint8_t *data);
#endif

#ifdef  TRF_USE_MD_A
/*!
 * @brief This function copies all electrical current maximum demand data to a buffer.
 *
 * @par Description:
 * This function copies all electrical current maximum demand data #trf_pymt_md_par_t to a buffer
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in.
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_md_a_data(uint8_t *data);
#endif

#ifdef PAYMENT_SYS
/*!
 * @brief This function copies all running values of data related to payment to a buffer.
 *
 * @par Description:
 * This function copies system variables related to payment #pymt_data_t to a buffer with
 * the address passed to the function.
 *
 * @param [out]  data  The address of the first byte in the buffer to get the data in
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved to the buffer.
 */
uint8_t trf_pymt_get_pymt_data(uint8_t* data);
#endif

#ifdef PYMT_MONY_TRANS
/*!
 * @brief This function returns the number of saved money records.
 *
 * @par Description:
 * This function gets the number of saved money transaction records.
 *
 * @return
 * 8 bits representing the number of saved records.
 */
uint32_t trf_pymt_get_num_mny_rec(void);

/*!
 * @brief This function copies a money record to a buffer.
 *
 * @par Description:
 * This function copies a certain money record to a buffer with the address
 * passed to the function.
 *
 * @param [out]  adrs  The address of the first byte in the buffer to get the data in
 * @param [in] record_num The number of record to copy ranges from 0 to #PMYT_MNY_TRANS_REC-1
 *
 * @return
 * None.
 */
void trf_pymt_get_mny_rec(uint8_t *adrs, uint8_t record_num);

/*!
 * @brief This function copies multiple money records to a buffer.
 *
 * @par Description:
 * This function copies number of money transaction records beginning from
 * certain record to a buffer.
 *
 * @param [out]  adrs  The address of the first byte in the buffer to get the data in.
 * @param [in] from The record number to begin copy from ranges from 0 to #PMYT_MNY_TRANS_REC -1
 * @param [in] num The number of records to copy ranges from 1 to #PMYT_MNY_TRANS_REC .
 *
 * @return
 * Size of written data in the buffer.
 */
uint16_t trf_pymt_get_mlti_mny_rec(uint8_t *adrs, uint16_t from, uint16_t num);
#endif
/*!@}*/

#ifdef TARIFF_SYS
/*!
 * @ingroup TRF_PYMT_EVENT
 *
 * @brief Used to inform the tariff and payment module with new events
 *
 * @par Description:
 * Used by other modules to inform tariff and payment with new events. New Events are new
 * min, new energy, new MD, power down and power up.
 *
 * @param [in] event 8 bits may take one of #trf_pymt_events_t values
 * @param [in] data Address of first byte of array containing data corresponding to the event
 *
 * @return
 * - None.
 *
 * @note
 * When PWR_UP event occurs we do nothing
 */
void trf_pymt_event(uint8_t  event,  uint8_t *data);
void trf_time_monitoring_grace(void);
/*!
 * @ingroup TRF_PYMT_CMD
 *
 * @brief Send a command to tariff and payment module.
 *
 * @par Description:
 * This function passes a command to tariff and payment module to reset, charge or discharge.
 *
 * @param [in] cmd 8 bits may take one of #trf_pymt_cmd_interface_t values
 * @param [in] data Address of first byte of array containing charge or discharge data or options of reset
 *
 * @return
 * An 8 bit positive number representing the number of bytes moved from the buffer.
 * If an invalid command is sent, 0 is returned.
 *
 * @note
 * For #TRF_PYMT_CMD_RST, reset type is one of #reset_cmd_t.
 */
uint8_t trf_pymt_cmd(uint8_t cmd, uint8_t* data);
#endif

#ifdef TARIFF_SYS
int32_t Set_New_Tariff(void);
//uint32_t calc_curr_bp_bill(uint8_t tariff_calc);
uint32_t calc_curr_bp_bill(void);
uint32_t calc_old_bp_bill(void);


uint32_t calc_new_bp_bill(void);
void deduct_new_tariff(void);
#endif
void trf_load_backup(void);
void trf_check_future_tariff (void);
uint8_t check_frndly_now();
void trf_find_tariff_step_consumption (void);
/*!
 * @brief Reset all MD variables except previous MD
 *
 * @par Description:
 * Reset all of these variables: \n
 * -# Current MD with its date and time.
 * -# Tariff MD data.
 *
 * @return
 *  - None.
 */
void reset_md(void);
/*!
 * @brief Reset all previous MD variables except previous MD
 *
 * @par Description:
 * Reset all of these variables: \n
 * -# Previous MD with its date and time.
 * -# Tariff MD data.
 *
 * @return
 *  - None.
 */
void reset_prev_md(void);
uint8_t trf_get_current_step(void);
void TRF_provious_month_recalculation_for_new_tariff(uint8_t month , uint8_t year);

/*!
 * @brief Check day status if vacation or weekend.
 *
 * @par Description:
 * Check day status according to given configuration (WORK, WEEKEND, VACATION or WEEKEND
 * with VACATION).
 *
 * @return day status of #vac_day_t.
 */
uint8_t get_day_status(void);
void test_eeprom_TwoPages();
void calc_init_rem_day_wh (void);
void trf_check_new_trf ();
uint8_t Trf_set_or_Clear_vac_day(TstDate* date , uint8_t flag);
#if defined (SPECIAL_DAYS_ENABLE) && defined (NEW_DESIGN_OPTIMIZE)
/*!
 * @brief Get the day id and date for the given Day index.
 *
 * @par Description:
 * you give me the special day index according to the DLMS special day structrue and I will search by this index for
 * the correct corresponsing day.
 *
 * @param [in]  Special_Day_ptr: pointer to the  Special day, given in it the index
 * @param [out]  Special_Day_ptr also :D : pointer to the wanted Special day, to return the date and the id in it.
 *
 * @return
 *   - staus  : true means index in the range and argument OK.
 *            : 0xff means not found this index.
 *            : False means Argument has errors.
 *
 * @note
 *   - None.
 */
uint8_t Trf_get_Special_day_by_Index (  TstSDaysRcd* Special_Day_ptr);
#endif
int32_t Calc_bill(uint32_t Consumption_bp_wh , uint8_t is_Set_Variables);
void trf_cfg_save_power_up (void);
void check_new_md();
void trf_save_backup (uint8_t set);
void trf_restore_backup (void);
uint8_t check_saving_energy_now(void);



#endif /* __DEPENDENCIES_LAYER_H */
