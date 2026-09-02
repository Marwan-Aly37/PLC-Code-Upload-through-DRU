/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

C O M M U N I C A T I O N

S O U R C E     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
* @file comm.c
*
* @brief This file calls all functions of communication interfaces of the meter.
*
* @details This files collects the functions of all communication interfaces.
*
* <b>References</b>\n
* -  Design document of IEC 62056-21 protocol.
* -  Design document of RF ID.
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
*   <td> 27/02/12 </td>
*   <td> islam.elshahat </td>
*   <td> Add two commands to enter and exit open relay mode. </td>
* </tr>
* <tr>
*   <td> 28/02/12 </td>
*   <td> islam.elshahat </td>
*   <td> Make a shared buffer between RF LINK and IEC. </td>
* </tr>
* <tr>
*   <td> 10/01/12 </td>
*   <td> islam.elshahat </td>
*   <td> Tell tariff with a new customer with general reset command. </td>
* </tr>
* <tr>
*   <td> 10/02/12 </td>
*   <td> islam.elshahat </td>
*   <td> Tell tariff with a new customer with general reset command,
*        add overload configuration and change template ID. </td>
* </tr>
* <tr>
*   <td> 30/12/12 </td>
*   <td> islam.elshahat </td>
*   <td> Add scratch card status to instanteneous data. </td>
* </tr>
* <tr>
*   <td> 16/12/12 </td>
*   <td> mohamed.essa </td>
*   <td> Integrate with GPRS application. </td>
* </tr>
* <tr>
*   <td> 09/12/12 </td>
*   <td> islam.elshahat </td>
*   <td> Add template ID to main meter data. </td>
* </tr>
* <tr>
*   <td> 29/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Modify instantaneous data and add scratch card command and status. </td>
* </tr>
* <tr>
*   <td> 21/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Solve bug of magnetic feature and GPRS. </td>
* </tr>
* <tr>
*   <td> 04/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Integrate with control. </td>
* </tr>
* <tr>
*   <td> 2/10/12 </td>
*   <td> mohamed.essa </td>
*   <td> Add SIM900 and GPRS </td>
* </tr>
* <tr>
*   <td> 25/09/12 </td>
*   <td> aibraheem </td>
*   <td> Integrate with metering (not complete). </td>
* </tr>
* <tr>
*   <td> 20/10/12 </td>
*   <td> aibraheem </td>
*   <td> Integrate with metering completed. </td>
* </tr>
* <tr>
*   <td> 24/09/12 </td>
*   <td> aibraheem </td>
*   <td> Integrate with metering (not complete). </td>
* </tr>
* <tr>
*   <td> 30/07/12 </td>
*   <td> islam.elshahat </td>
*   <td> Add IEC62056-21. </td>
* </tr>
* <tr>
*   <td> 26/07/12 </td>
*   <td> islam.elshahat </td>
*   <td> Created </td>
* </tr>
* <tr>
*   <td> 05/08/13 </td>
*   <td> saber.ragab </td>
*   <td> replace mtr_cmd with cal_cmd in two cases start auto cal and nxt auto cal </td>
* </tr>
* </table>\n
*/

/*==========================================================================================

INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "v85xx_flash.h"  
#include "comm.h"
#include "dependencies_layer.h"
#include "ctrl.h"
#include "config.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#include "iec62056_21_slave.h"
#include "string.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
//#include "contact_card.h"
/* #include "gprs_sys.h" */ /* module removed from the DRU build */
/* #include "modem.h" */ /* module removed from the DRU build */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "RF_Link.h" */ /* module removed from the DRU build */
#ifdef RF_LINK_FEATURE
/* #include "TI_CC1120.h" */ /* module removed from the DRU build */
#endif
//#include "Calibration.h"
//#include "EEPROM_AT24C1024.h"
//#include "EEPROM_M24M02.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "task_priority.h" -- see dependencies_layer.h */

#if (MicroController == Micro_V94XX)
#include "V94XX_WDT.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#else
#include "v85xx_wdt.h"
/* #include "V9360.h" */ /* module removed from the DRU build */


#endif
/* #include "Dlms_Itf.h" */ /* module removed from the DRU build */
/* #include "Cal_Itf.h" */ /* module removed from the DRU build */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
//#include "Flash_app.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "Dlms_PhyLp.h" */ /* module removed from the DRU build */
/* #include "STS_Module.h" */ /* module removed from the DRU build */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "Keypad.h" */ /* module removed from the DRU build */
#ifdef MTR_TWO_PH
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#endif
#ifdef STS_ENABLE
/* #include "STS_Key_Store.h" */ /* module removed from the DRU build */
uint8_t g_Token[] = {"06565357798943368613"},STS_flag = 0;
#endif
#ifdef DLMS_UI
/* #include "UI.h" */ /* module removed from the DRU build */
#endif
#ifdef RF_CC1120
/* #include "cc1120_MAC.h" */ /* module removed from the DRU build */
#endif
/* #include "iec62056_21_master.h" */ /* module removed from the DRU build */
#ifdef CONSOL_FEATURE
/* #include "Consol_Interface.h" */ /* module removed from the DRU build */
#endif
#ifdef ANSI_STACK
/* #include    "ANSI_APP_RF.h" */ /* module removed from the DRU build */
#endif
/*==========================================================================================

DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
Constants and Definitions
------------------------------------------------------------------------------------------*/
#define ReadFromEEPROM(X,Y,Z)	  	          	         EEPROM_read(Data_eeprom_address,X,Y,Z)
#define	WriteInEEPROM(X,Y,Z)     	             	     EEPROM_write(Data_eeprom_address,X,Y,Z)
/*! Argument passed with tariff and payment reset command #reset_cmd_t.*/
#define RST_SYS_VAR_TRF_PYMT 0
/*! Main meter data packet size.*/
#define MAIN_DATA_SIZE 44

#define MAX_BOOTLOADER_PCKT_SIZE     256ul 
#define MAX_BOOTLOADER_PCKT_NUM      1000ul 
#define   INCREMENT_COUNTER_OF_RESET_METER()     ctrl_sys_var_second_optional_feature.reset_meter_counter++
#define BOOTLOADER_EEPROM_BEGIN_ADD  (uint32_T)0 
#define SECTOR_SIZE  4096
#define TOTAL_SCREEN_SECTORS  75
#define EXTERNAL_FLASH_SIZE   4000
#define COMM_BUZZER_ON(TONE)     ctrl_cmd(TONE) 
#define COMM_TAMPER_LED_ON()     S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
#define COMM_TAMPER_LED_OFF()    S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
#define COMM_TAMPER_LED_TOGGLE()  S_GPIO_Toggle_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
#define COMM_LOW_CRDT_LED_TOGGLE()  S_GPIO_Toggle_Bits(CRDT_LED_PORT, CRDT_LED_PIN)
#define COMM_LOW_CRDT_ON()      S_GPIO_Clear_Bits(CRDT_LED_PORT, CRDT_LED_PIN)
#define COMM_LOW_CRDT_OFF()     S_GPIO_Set_Bits(CRDT_LED_PORT, CRDT_LED_PIN)
#define EXTERNAL_FLASH_SAVE
#define TIME_OUT_DRU_MAX  20

/*------------------------------------------------------------------------------------------
Data Types
------------------------------------------------------------------------------------------*/
/*!
* @par Description:
*   This enumerator contains all flags masks of instant data used by user interface.
*/
typedef enum
{
  UNIQUE_ID_MASK = (uint8_t)0x01,
  TRF_CFG_MASK = (uint8_t)0x02,
  PYMT_CFG_MASK = (uint8_t)0x04,
  TAX_CFG_MASK = (uint8_t)0x08,
  GPRS_ICON_MASK = (uint8_t)0x10,
  HINT_STAT_MASK = (uint8_t)0x20,
  RFID_ICON_MASK = (uint8_t)0x40,
  PYMT_DATA_MASK = (uint8_t)0x80,
} inst_flags_masks;
extern uint8_t DotmatrixSend;
#ifdef CRC_INTEGRITY_ENABLE 
extern uint8_t Signature_val;
#endif
uint16_t current_pckt_ID = 0 ;
uint16_t prev_pckt_ID  = 0;
uint16_t Boot_loader_timeout =0 ;
uint8_t tmp_crc = 0;
uint8_t comm_disable_zero_command = 0;
uint16_t replace_charge_counter_max;
uint8_t DotMatrixOpticalFlag = 0;
uint16_t DruCounterTimeOut = 0;
uint8_t FinishedDotMatrixFlag = 0;
uint8_t  LedFlag = LED_START;
#ifdef DEBUG_DRU_CRC
uint8_t flagtessst = 0;
#endif
#ifdef RF_CC1120
extern uint8_t RF_flags_arr[10];
#endif
#ifdef MTR_THREE_PH
const uint8_t size_of_comm[256] =
{
  0,0,7,7,1,48,3,14,119,9,//9
  2,149,0,0,0,0,0,0,0,0,//19
  0,2,7,5,4,0,0,0,0,0,//29
  0,6,18,3,4,2,0,10,132,2,//39
  3,7,0,0,0,0,0,0,0,0,//49s
  0,0,0,0,0,0,0,0,0,0,//59
  2,0,0,0,0,0,0,0,0,0,//69
  0,0,0,0,0,0,0,0,0,0,//79
  0,0,0,0,0,0,0,0,0,0,//89
  0,0,0,0,0,0,0,0,0,0,//99
  
  0,0,4,0,0,0,0,0,1,4,//109
  0,0,0,1,0,1,1,4,4,4,//119
  3,0,0,0,0,0,0,4,1,1,//129
  4,2,1,40,5,0,0,2,2,0,//139
  4,0,0,0,4,0,0,1,0,0,//149
  2,4,4,1,2,2,1,4,0,0,//159
  0,4,89,82,0,0,0,4,2,2,//169
  2,2,1,2,0,0,4,4,0,0,//179
  0,0,5,2,0,0,2,255,2,0,//189
  0,255,1,1,4,4,2,1,1,2,//199
  4,1,1,1,1,1,4,0,1,2,//209
  4,0,0,0,0,0,0,0,0,0,//219
  0,0,0,0,0,0,0,0,0,0,//229
  0,0,0,0,0,0,0,0,0,0,//239
  0,0,0,0,0,0,0,0,0,0,//249
  0,0,0,0,5,0//255
};
#else
#ifdef ASHNTTI_PROJECT
const uint8_t size_of_comm[256] =
{
  0,0,7,7,1,48,3,14,119,9,//9
  2,23,0,0,0,0,0,0,0,0,//19
  0,2,7,3,4,0,0,0,0,0,//29
  0,6,16,3,4,2,2,4,132,2,//39
  3,0,0,0,0,0,0,0,0,0,//49
  0,0,0,0,0,0,0,0,0,0,//59
  0,0,0,0,0,0,0,0,0,0,//69
  0,0,0,0,0,0,0,0,0,0,//79
  0,0,0,0,0,0,0,0,0,0,//89
  0,0,0,0,0,0,0,0,0,0,//99
  
  0,0,4,0,0,0,0,0,1,4,//109
  0,0,0,1,0,1,1,4,4,4,//119
  3,0,0,0,0,0,0,4,1,1,//129
  4,2,0,0,5,0,0,2,2,0,//139
  4,0,0,0,4,0,0,1,0,0,//149
  2,4,4,1,2,2,0,4,0,0,//159
  0,0,0,0,0,0,0,4,2,2,//169
  2,2,1,2,0,0,4,4,0,0,//179
  0,0,2,2,0,0,2,255,2,0,//189
  0,255,1,1,4,4,2,1,1,2,//199    /// TODO 199 should be 1 instead 2
  
  4,1,1,1,1,1,4,0,1,2,//209
  4,0,0,0,0,0,0,0,0,0,//219
  0,0,0,0,0,0,0,0,0,0,//229
  0,0,0,0,0,0,0,0,0,0,//239
  0,0,0,0,0,0,0,0,0,0,//249
  0,0,0,0,5,0//255
};
#else
const uint8_t size_of_comm[256] =
{
  0,0,7,7,1,48,3,14,160,9,//9
  3,149,0,0,0,0,0,0,0,0,//19
  0,2,7,3,4,0,0,0,0,0,//29
  0,6,16,3,4,2,2,10,132,2,//39
  3,0,0,0,0,0,0,0,0,0,//49
  0,0,0,0,0,0,0,0,0,0,//59
  0,0,0,0,0,0,0,0,0,0,//69
  0,0,0,0,0,0,0,0,0,0,//79
  0,0,0,0,0,0,0,0,0,0,//89
  0,0,0,0,0,0,0,0,0,0,//99
  
  0,0,4,0,0,0,0,0,1,4,//109
  0,0,0,1,0,1,1,4,4,4,//119
  3,0,0,0,0,0,0,4,1,1,//129
  4,2,0,0,5,0,0,2,2,0,//139
  4,0,0,0,4,0,0,1,0,0,//149
  2,4,4,1,2,2,0,4,0,0,//159
  0,0,0,0,0,0,0,4,2,2,//169
  2,2,1,2,0,0,4,4,0,0,//179
  0,0,2,2,0,0,2,255,2,0,//189
  0,255,1,1,4,4,2,1,1,2,//199    /// TODO 199 should be 1 instead 2
  
  4,1,1,1,1,1,4,0,1,2,//209
  4,0,0,0,0,0,0,0,0,0,//219
  0,0,0,0,0,0,0,0,0,0,//229
  0,0,0,0,0,0,0,0,0,0,//239
  0,0,0,0,0,0,0,0,0,0,//249
  0,0,0,0,5,0//255
};
#endif
#endif

#ifdef MTR_SINGLE_PH
  uint8_t charge_data_ar_size[3]={0x80,0x01,0xc0};
  uint8_t trf_pymt_t_ar_size[]={0x04,0xa4,0x02};
#ifdef ASHNTTI_PROJECT
  uint8_t  trf_t_ar_size[]={0x06,0xe9,0xea,0xe9,0x01};
#else
  uint8_t  trf_t_ar_size[]={0x07,0xe9,0xea,0xea,0xe9,0x01};
#endif
  uint8_t tax_t_ar_size[]={0xe2,0x01};
  uint8_t lmt_cfg_ar_size[]={0x80,0x05};
#ifdef LOAD_LIMIT_PLAN
  uint8_t load_limit_cfg_ar_size[]={,0x02,0xa9,0xea};
#endif
  uint8_t mtr_lmt_cfg_ar_size[]={0x80,0x05};
  uint8_t id_ar_size[2]={0xc0,0x80};
  uint8_t pymt_data_ar_size[]={0xf0,0xa9};
  uint8_t trf_pymt_md_par_ar_size[]={0xc0,0x05,0xc0,0x05,0xea};
#ifdef MTR_REACTIVE
  uint8_t acc_en_reg_t_ar_size[]={0xc0,0x80,0xc0,0x80,0xc0,0x80,0xc0,0x80,0xc0,0x80,0xc0,0x80};
#else
  uint8_t acc_en_reg_t_ar_size[]={0xc0,0x80,0xc0,0x80,0xc0,0x80};
#endif
//  uint8_t basic_ar_size[]={/*(uint8_t)(CTRL_NON_LTCH_ACT_EVNT_NUM+CTRL_LTCH_ACT_EVNT_NUM+9)*/0x18,0xa0,0x0a,0xc0,0x02,0xa0};
#ifdef ASHNTTI_PROJECT
  uint8_t basic_ar_size[]={0x18,0x80,0x04,0xc0,0x02,0x80};
#else
  uint8_t basic_ar_size[]={0x18,0x80,0x0a,0xc0,0x02,0x80};
#endif
#ifdef MTR_REACTIVE
  uint8_t md_reg_t_ar_size[]={0xc0,0x05,0x80,0xc0,0x05,0x80,0xc0,0x05,0x80};
#else
  uint8_t md_reg_t_ar_size[]={0xc0,0x05,0x80,0xc0,0x05,0x80};
#endif
  uint8_t tariff_data_extnsion[3]= { 0xc0,0x01,0xca};
  uint8_t var_temp_size =0;
  uint8_t display_ctrl_data_t_ar_size[]={0xa2,0x03};
  uint8_t Corruption_reading_ar_size[]={0x80,0x80};
#else
  #ifdef ASHNTTI_PROJECT
    uint8_t var_temp_size =0;
#endif
#endif
/*------------------------------------------------------------------------------------------
External Variables
------------------------------------------------------------------------------------------*/
#ifdef IEC_62056_21_SLAVE
extern uint8_t iec_comm_buffer[IEC_BUFFER_SIZE]; /*!< Buffer of IEC protocol communication to receive packets in.*/
#endif
extern uint8_t RFID_idle_fla;
 uint8_t EchoBuffer[260];
/*------------------------------------------------------------------------------------------
Local Variables
------------------------------------------------------------------------------------------*/
/* PLC Firmware Variables */

/* This is 1 until the first PLC packet is received. */
uint8_t plc_first_packet = 1u;
/* Address where the next received data will be written. */
uint32_t plc_write_address = PLC_FIRMWARE_START_ADDRESS;
/*=====================*/
/* RF Firmware Variables */

/* This is 1 until the first RF packet is received. */
 uint8_t rf_first_packet = 1u;
/* Address where the next received data will be written. */
uint32_t rf_write_address = RF_FIRMWARE_START_ADDRESS;
/*=====================*/

static  comm_source_t comm_source_flag =  LOCAL_SOURCE;
uint8_t CommSendDataFlag = 0;
uint8_t customer_flag;
uint16_t replace_charge_counter=0;
uint8_t  CommDruBuffer[PAGESIZE]  = {0};
extern  uint8_t CoverClosedFlag;
extern uint8_t DM_integraty_template_crc[20];
/*------------------------------------------------------------------------------------------
Macros
------------------------------------------------------------------------------------------*/
/*! Get current minute from RTC.*/
#define COMM_MIN CTRL_READ_MIN()
/*! Get current hour from RTC.*/
#define COMM_HR CTRL_READ_HR()
/*! Get current day from RTC.*/
#define COMM_DAY CTRL_READ_DAY()
/*! Get current month from RTC.*/
#define COMM_MONTH CTRL_READ_MONTH()
/*! Get current year from RTC.*/
#define COMM_YEAR CTRL_READ_YEAR()
/*! Get firmware version.*/
#define COMM_GET_FIRMWARE_VERSION() FIRMWARE_VERSION
/*! Get tariff defined features.*/
#define COMM_GET_TRF_FETUR(adrs)  trf_pymt_config_get(TRF_PYMT_CFG_FETUR, adrs)
/*! Get metering defined features.*/
#define COMM_GET_MTR_FETUR(adrs) mtr_get_feature(adrs)
/*! Get Control defined features.*/
#define COMM_GET_CTRL_FETUR(adrs) ctrl_get_fetur(adrs)
/*! Get display defined features.*/
#define COMM_GET_DSPLY_FETUR() display_get_feature()
/*! Return byte of GPRS defined features.*/
#ifdef GPRS_FEATURE
#define COMM_GET_GPRS_FETUR() gprs_get_feature()
#else
#define COMM_GET_GPRS_FETUR() 0
#endif
/*! For future use.*/
#define COMM_GET_IEC_FETUR() 0
/*! Return byte of RF defined features.*/
#define COMM_GET_RF_FETUR() RF_Link_get_feature();
/*! Return byte of RFID defined features. RFID is not built for the DRU, so no
    RFID feature bits are reported. */
#define COMM_GET_RFID_FETUR()   0
/*! Get new tampers flag.*/
#define GET_NEW_TMPR_FLG() CTRL_GPRS_NEW_TMP_STAT
#define GET_SERIAL_NUMBER(PTR)  ctrl_cfg_get(CTRL_CFG_SERIAL,PTR)
/*! Get number of saved BPH records.*/
#ifdef  TRF_BP_HISTORY
#define GET_NO_OF_SAVED_BPH_REC() 0 /* tariff/payment not built for the DRU */
#else
#define GET_NO_OF_SAVED_BPH_REC() 0
#endif
/*! Get number of event records.*/
#ifdef CTRL_EVNT_LOG
#define GET_NO_OF_SAVED_EVNT_REC() ctrl_get_evnt_num()
#else
#define GET_NO_OF_SAVED_EVNT_REC() 0
#endif
/*! Get number of money records.*/
#ifdef PYMT_MONY_TRANS
#define GET_NO_OF_SAVED_MNY_REC() 0 /* tariff/payment not built for the DRU */
#else
#define GET_NO_OF_SAVED_MNY_REC() 0
#endif
/*! Get number of saved profile records.*/
#ifdef MTR_LOAD_PROFILE
#define GET_NO_OF_SAVED_PRFLE_REC() mtr_get_lp_num()
#else
#define GET_NO_OF_SAVED_PRFLE_REC() 0
#endif
/*! Get the meter mode now. 1 calibration mode, 0 normal mode.*/
#define COMM_CAL_MODE_STAT() mtr_cal_state()
/*! Get number of saved config meter records.*/
#ifdef CTRL_CFG_METER_LOG
#define GET_NO_OF_SAVED_CFG_METER_REC() ctrl_get_cfg_meter_num()
#else
#define GET_NO_OF_SAVED_CFG_METER_REC() 0
#endif
#ifdef GPRS_SRT_CARD
/*! Read scratch card status.*/
#define COMM_READ_SCRTCH_CARD_STAT(data_ptr) gprs_get_sc_key_status(data_ptr)
#endif
#define COMM_RESET_RFID_TAMPER()                  ctrl_cmd(CTRL_CLR_NEW_TMPR_RFID) /*!< This macro is used to reset the new tamper variable.*/
#define COMM_SEND_OPTICAL_FRAME(data_ptr,length)  send_frame(data_ptr,length)

#define SAVE_CRITICAL_DATA_TARF_CTRL_FILSYS_MTR      0xffff      // new self test
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/

void CommSetSendDataFlag(uint8_t Val)
{
  CommSendDataFlag = Val;
}

uint8_t CommGetSendDataFlag(void)
{
  return CommSendDataFlag;
}


/* CommEraseAllSectorOfDotMatrixSpi2() removed: not reachable in the DRU build. */

void CommEraseAllSectorExtFlashOfDotMatrix(void)
{ 
  uint32_t addr = 0; 
  uint8_t modifyshiftbyte = 0;
  
  memcpy((uint8_t*)&modifyshiftbyte,(uint8_t*)dotmatrixdata,1); 
   
  addr=(modifyshiftbyte * 0x20000)+LN1_LCD_SECTOR_ADDRESS_SCREEN_0;
  Flash_erase_block_by_address(addr); // this address is first address in block
  Delay_ms(400);
  if(modifyshiftbyte==0)
  {
    //erase_flash_sectors_from_to(LN1_LCD_SECTOR_NUM_AR_WORDS4,LN1_LCD_SECTOR_NUM_ALPHA2);
    Flash_erase_block_by_address(LN1_LCD_SECTOR_ADDRESS_AR_WORDS4);
    Delay_ms(400);
  }
  else if(modifyshiftbyte==1)
  {
    erase_flash_sectors_from_to(LN2_LCD_SECTOR_NUM_SCREEN_0,LN2_LCD_SECTOR_NUM_SCREEN_6);
    erase_flash_sectors_from_to(LN2_LCD_SECTOR_NUM_ALPHA1,LN2_LCD_SECTOR_NUM_ALPHA2);
  }
  else if(modifyshiftbyte==2)
  {
    erase_flash_sectors_from_to(LN3_LCD_SECTOR_NUM_SCREEN_0,LN3_LCD_SECTOR_NUM_AR_WORDS1);
  }  
}

void CommIncrementDruCounterTimeOut(void)
{
  DruCounterTimeOut++;
}

void CommResetDruCounterTimeOut(void)
{
  DruCounterTimeOut=0;
}

#ifdef CRC_INTEGRITY_ENABLE 
void IntegrityCrcCheckState(void)
{
  uint8_t CrcVal = 0;
  
  memset(DM_integraty_template_crc,0,sizeof(DM_integraty_template_crc));
  memcpy(DM_integraty_template_crc,(uint8_t*)dotmatrixcrc,sizeof(DM_integraty_template_crc));
  CrcVal = calc_crc(DM_integraty_template_crc, sizeof(DM_integraty_template_crc)-1 , CRC);  
  if(CrcVal != DM_integraty_template_crc[19])
  {
    /*** this is mean that Sceens of Dot matrix Not completed in the case of Optical .***/
    LedFlag = LED_FLASHING_ACCPT;
  }
  
}
#endif

void CommResetLedDruStatus(void)
{
  COMM_LOW_CRDT_OFF();
  COMM_TAMPER_LED_OFF();
}

uint8_t CommDruWriteSerialNumSector(uint32_t Add)
{
  uint16_t OffsetAdd = 0;
  uint8_t i = 0;
  uint8_t flash_addr[4] = {0};
  uint32_t Address = 0;
  uint8_t Offset = NUM_PAGES_PER_SECTOR-2;
  uint8_t ScreenSize = 128;
  
  OffsetAdd = LN1_LCD_SECTOR_ADDRESS_SCREEN_1 - LN1_LCD_SECTOR_ADDRESS_SCREEN_0;
  
  for(i=0; i < NUM_PAGES_PER_SECTOR;i++)  
  {
    memset(CommDruBuffer,0,sizeof(CommDruBuffer));
    memcpy((uint8_t*)CommDruBuffer,(uint8_t*)dotmatrixpage+(OffsetAdd+(i*FLASH_PAGE_SIZE)),FLASH_PAGE_SIZE);          
    memset(flash_addr, 0, 4);
    if(i == NUM_PAGES_PER_SECTOR-2)  //Skip Page of Serial Number
      i++;
    Address = Add+OffsetAdd+(i*FLASH_PAGE_SIZE);
    ConvertUint32ToBytes(flash_addr, Address);  
    
    if(FLASH_gWrite(flash_addr,CommDruBuffer,FLASH_PAGE_SIZE) != NO_ERROR)
    {       
      LedFlag = LED_ON;
      COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone          
      CommSetSendDataFlag(0);
      CoverClosedFlag = 0;
      return 0;
    }
    
  }
 
  memset(CommDruBuffer,0,sizeof(CommDruBuffer));   //Read page of Serial number from internal FLASH
  memcpy((uint8_t*)CommDruBuffer,(uint8_t*)dotmatrixpage+(OffsetAdd+(Offset*FLASH_PAGE_SIZE)),FLASH_PAGE_SIZE); 
  for(i=0;i<2;i++)//this for writing screen of serial number at the last
  {              
    memset(flash_addr, 0, 4);
    Address = (Add+OffsetAdd+(Offset*FLASH_PAGE_SIZE))+(i*ScreenSize);
    ConvertUint32ToBytes(flash_addr, Address);  
    
    if(FLASH_gWrite(flash_addr,CommDruBuffer+(i*ScreenSize),ScreenSize) != NO_ERROR)
    {       
      LedFlag = LED_ON;
      COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone          
      CommSetSendDataFlag(0);
      CoverClosedFlag = 0;
      return 0;
    }
  }
  return 1;
}

uint8_t data_reading_from_external[512] = {0};
void CommDruTask(void)
{  
  int16_t i = 0;
  uint8_t  AddressByte = 0;
  uint32_t Address,Add = 0;
  uint16_t FlashSize = 0;
  uint8_t flash_addr[4];
  static uint16_t  counter = 0; 

  
  if(DruCounterTimeOut >= TIME_OUT_DRU_MAX)
  {
    CommResetDruCounterTimeOut();  
    plc_reset_var(); /* 23ml reset ll var bta3t el plc firmware */
    rf_reset_var();
    DotMatrixOpticalFlag = 0;
    CommSetSendDataFlag(0);
    CoverClosedFlag = 0;
  }  
#ifndef SAVING_DIRECT_INTERNAL
  DotMatrixSaveInternalFlash(); // 5 sec
#endif
  // hn3ml detect ll flash size
  // hn3ml hwar 2no y3ml flashing
#ifdef DRU_SW_UP
  if(LedFlag == LED_FLASHING_ACCPT)
    CommSetSendDataFlag(0);
  if((LedFlag != LED_FLASHING_ACCPT)&&(!DotMatrixOpticalFlag)&&((CommGetSendDataFlag())/* || (CoverClosedFlag)*/)) // hn7tag de
#else
  if(LedFlag != LED_FLASHING_ACCPT)
#endif
  {
    Delay_ms(100);
    LedFlag = LED_START;
    FlashSize = FlashSizeDetection_U8(); //170 ms // de mfrod function bt3ml detect ll size bta3 l flash
   if(FlashSize < EXTERNAL_FLASH_SIZE)
    {
      /****DO ALARM **/
      LedFlag = LED_FLASHING;  
#ifdef DRU_SW_UP // lama 2dos 3la l button kda hbd2 2n2l l firmware ll external
      CommSetSendDataFlag(0);
      CoverClosedFlag = 0;
      COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone     
#endif
    }
    else // kda na hktb
    {
      COMM_TAMPER_LED_OFF();
      COMM_LOW_CRDT_OFF();
      
#ifndef EXTERNAL_FLASH_SAVE 
    CommEraseAllSectorOfDotMatrixSpi2();
    for(i=0;i<=(TOTAL_SCREEN_SECTORS*16);i++)
    {
      memset(CommDruBuffer,0,sizeof(CommDruBuffer));
      FlashRead(LN1_LCD_SECTOR_ADDRESS_SCREEN_0 + (i*FLASH_PAGE_SIZE),CommDruBuffer,FLASH_PAGE_SIZE);
      /***** write Data of screens to another flash *****/
      FlashWriteSpi2(LN1_LCD_SECTOR_ADDRESS_SCREEN_0 + (i*FLASH_PAGE_SIZE),CommDruBuffer,FLASH_PAGE_SIZE); 
    }
#else
    memcpy((uint8_t*)&AddressByte,(uint8_t*)dotmatrixdata,1); 
    if(AddressByte == 2)
      Add = LN3_LCD_SECTOR_ADDRESS_SCREEN_0; 
    else if(AddressByte == 1) 
      Add = LN2_LCD_SECTOR_ADDRESS_SCREEN_0; 
    else 
      Add = LN1_LCD_SECTOR_ADDRESS_SCREEN_0;

    IntegrityCrcCheckState();  // LedFlag will be set in this function if All screens not send.
    if(LedFlag == LED_FLASHING_ACCPT)
    {
      COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone  
      return;
    }
    CommEraseAllSectorExtFlashOfDotMatrix(); //830 ms for one language

    for(i=NUM_SECTORS_DOTMATRIX_INTERNAL; i>= 0;i--)  //1000 ms
    {
      memset(CommDruBuffer,0,sizeof(CommDruBuffer));
      memset(flash_addr, 0, 4);
      Address = Add+(i*PAGESIZE);
      if(Address == (LN1_LCD_SECTOR_ADDRESS_SCREEN_1+(7*PAGESIZE)))
      {
        // Skip this screen for now, it will be written at the end
        //COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone
        continue;
      }
      memcpy((uint8_t*)CommDruBuffer,(uint8_t*)dotmatrixpage+(i*PAGESIZE),PAGESIZE);
      ConvertUint32ToBytes(flash_addr, Address);

      if(FLASH_gWrite(flash_addr,CommDruBuffer,PAGESIZE) != NO_ERROR)
      {
        LedFlag = LED_ON;
        COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone
        CommSetSendDataFlag(0);
        CoverClosedFlag = 0;
        return;
      }
    }
    
    // Now write the special screen with swapped halves: last 256 bytes first, then first 256 bytes ,total (512 bytes)
    memset(CommDruBuffer,0,sizeof(CommDruBuffer));
    memcpy((uint8_t*)CommDruBuffer,(uint8_t*)dotmatrixpage+(15*PAGESIZE),PAGESIZE); //address of last 4 screen (serial screen,...etc)
    uint8_t chunk_indx=1;
    for(uint8_t chunk=0;chunk<=1;chunk++)
    {
      // Write second 256 bytes of page 7 to first 256 bytes of last page when j=0
      // Write first 256 bytes of page 7 to second 256 bytes of last page when j=1
      memset(flash_addr, 0, 4);
      Address = (LN1_LCD_SECTOR_ADDRESS_SCREEN_1+(7*PAGESIZE)+(256*chunk_indx));
      ConvertUint32ToBytes(flash_addr, Address);
      if(FLASH_gWrite(flash_addr,CommDruBuffer+(256*chunk_indx),256) != NO_ERROR)
      {
        LedFlag = LED_ON;
        COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone
        CommSetSendDataFlag(0);
        CoverClosedFlag = 0;
        return;
      }
      chunk_indx--;
    }
      /**** Write Last scetor that including serial number screen  ***/
    //if(CommDruWriteSerialNumSector(Add) == 0)
      //return;
    if(!UnitTestReadingFunction(Add))
    {
      LedFlag = LED_ON;
      COMM_BUZZER_ON(CTRL_BEEP_TONE1);//Bad Tone
      CommSetSendDataFlag(0);
      CoverClosedFlag = 0;
      return;
    }

#endif
    CoverClosedFlag = 0;
    CommSetSendDataFlag(0);

#ifndef CRC_INTEGRITY_ENABLE
    memset(DM_integraty_template_crc,0,sizeof(DM_integraty_template_crc));
    DM_integraty_template_crc[0] = 242;
    DM_integraty_template_crc[19] = calc_crc(DM_integraty_template_crc, sizeof(DM_integraty_template_crc)-1 , CRC);     
#endif
    COMM_BUZZER_ON(CTRL_BEEP_TONE0);//Good Tone
    LedFlag = LED_OFF;     
    /** write in the external flash for meter **/
    memset(flash_addr, 0, 4);
    Address = INTEGRITY_CRC_ADDRESS;    
    ConvertUint32ToBytes(flash_addr, Address);  
    FLASH_gSectorErase(flash_addr);
    FLASH_gWrite(flash_addr,DM_integraty_template_crc,sizeof(DM_integraty_template_crc)); 
  }
 }
  
  if(LedFlag == LED_START)
  {
    CommResetLedDruStatus();
  }
  else if(LedFlag == LED_ON)
  {
    COMM_LOW_CRDT_OFF();
    COMM_TAMPER_LED_ON();
  }
  else if(LedFlag == LED_FLASHING)
  {
    COMM_LOW_CRDT_OFF();
    if(counter++ >= MAX_LED_TOGGLE_COUNTER)
    {
      COMM_TAMPER_LED_TOGGLE();
      counter = 0;
    }
  }
  else if(LedFlag == LED_OFF)
  {
    COMM_TAMPER_LED_OFF(); // de hynwr el lamp el red
    COMM_LOW_CRDT_ON(); // lamp el blue htnwr
  }
#ifdef CRC_INTEGRITY_ENABLE   
  else if(LedFlag == LED_FLASHING_ACCPT)
  {
    if(counter++ >= MAX_LED_FLASHING_COUNTER)
    {
      COMM_LOW_CRDT_LED_TOGGLE() ;
      COMM_TAMPER_LED_TOGGLE();
      counter = 0;
    }
  }
#endif
  else
  {
     //DO NO Thing 
  }
  
#ifdef CRC_INTEGRITY_ENABLE  
  if(FinishedDotMatrixFlag)
  {
    LedFlag = LED_START;
    FinishedDotMatrixFlag = 0;    
    DM_integraty_template_crc[19] = calc_crc(DM_integraty_template_crc, sizeof(DM_integraty_template_crc)-1 , CRC);
    FLASH_SectorErase(dotmatrixcrc);
    Delay_ms(100);
    FLASH_ProgramByte(dotmatrixcrc,(uint8_t*)DM_integraty_template_crc,sizeof(DM_integraty_template_crc)); 
  }
#endif 
#ifdef DEBUG_DRU_CRC
  if(flagtessst)
  {
    memset(flash_addr, 0, 4);
    ConvertUint32ToBytes(flash_addr, INTEGRITY_CRC_ADDRESS);  
    FLASH_gRead(flash_addr,DM_integraty_template_crc,sizeof(DM_integraty_template_crc));  
  }
#endif
}

uint8_t UnitTestReadingFunction(uint32_t Add)
{
  uint8_t ret=1;
  uint32_t Address= 0;
  uint8_t flash_addr[4];
  for(int16_t i=NUM_SECTORS_DOTMATRIX_INTERNAL; i>= 0;i--)  //1000 ms
  {
    memset(CommDruBuffer,0,sizeof(CommDruBuffer)); //intenal flash
    memset(data_reading_from_external,0,sizeof(data_reading_from_external)); //external flash
    memset(flash_addr, 0, 4);
    Address = Add+(i*PAGESIZE);
    memcpy((uint8_t*)CommDruBuffer,(uint8_t*)dotmatrixpage+(i*PAGESIZE),PAGESIZE);
    ConvertUint32ToBytes(flash_addr, Address);
    FLASH_gRead(flash_addr,data_reading_from_external,PAGESIZE);
    if(memcmp(data_reading_from_external,CommDruBuffer,PAGESIZE))
    {
      return 0;
    }
  }
  return ret;
}
/* @internship Program
 * This function is called we want to reset all variables used by the PLC
 */
void plc_reset_var(void)
{
  
   plc_write_address = PLC_FIRMWARE_START_ADDRESS;
   plc_first_packet = 1;
  return;
}
/* @internship Program
 * This function is called we want to reset all variables used by RF
 */

void rf_reset_var(void)
{
  
   rf_write_address = RF_FIRMWARE_START_ADDRESS;
   rf_first_packet = 1;
  return;
}
/*
 * Save the data bytes received after command D4.
 * The first six and last five frame bytes were already removed.
 */

uint8_t save_plc_firmware_data(uint8_t *data, uint16_t data_length) // zawd haga t3ml detect l 2khr packet 2nna khlsna 3shan n3ml reset l kol haga
{
  uint16_t sector;
  uint32_t sector_address;

  /* Receiving D4 without data is invalid. */
  if(data_length == 0u)
  {
    plc_reset_var();
    return FALSE;
  }

  /* Erase the reserved area before writing the first packet. */
  if(plc_first_packet == 1u)
  {
    /* Erase sectors */
    for(sector = 0u; sector < PLC_FIRMWARE_SECTOR_COUNT; sector++)
    {
      sector_address = PLC_FIRMWARE_START_ADDRESS +((uint32_t)sector * PLC_FLASH_SECTOR_SIZE);

      FLASH_SectorErase(sector_address);
      Delay_ms(4);
    }
    plc_write_address = PLC_FIRMWARE_START_ADDRESS;
    plc_first_packet = 0u;
  }

  /* Check that this packet will not exceed the reserved area. */
  if((uint32_t)data_length >(PLC_FIRMWARE_END_ADDRESS - plc_write_address))
  {
    plc_reset_var();
    return FALSE;
  }

  /* Write the received data into the internal flash. */
  FLASH_ProgramByte(plc_write_address, data, data_length);
//memcpy(data_test,(uint8_t*)plc_write_address , 512);

  /* Read the written bytes directly and compare them with the packet. */
  if(memcmp((uint8_t*)plc_write_address, data, data_length) != 0)
  {
    plc_reset_var();
    return FALSE ;
  }

  /* Move the address by the actual packet data length. */
  plc_write_address += (uint32_t)data_length;

  return TRUE;
}
/* @internship Program
 * This function is called when we receive RF firmware command
 */
uint8_t save_rf_firmware_data(uint8_t *data, uint16_t data_length)
{
  uint16_t sector;
  uint32_t sector_address;

  /* Receiving D4 without data is invalid. */
  if(data_length == 0u)
  {
    rf_reset_var();
    return FALSE;
  }

  /* Erase the reserved area before writing the first packet. */
  if(rf_first_packet == 1u)
  {
    /* Erase sectors */
    for(sector = 0u; sector < RF_FIRMWARE_SECTOR_COUNT; sector++)
    {
      sector_address = RF_FIRMWARE_START_ADDRESS +((uint32_t)sector * RF_FLASH_SECTOR_SIZE);

      FLASH_SectorErase(sector_address);
      Delay_ms(4);
    }
    rf_write_address = RF_FIRMWARE_START_ADDRESS;
    rf_first_packet = 0u;
  }

  /* Check that this packet will not exceed the reserved area. */
  if((uint32_t)data_length >(RF_FIRMWARE_END_ADDRESS - rf_write_address))
  {
    rf_reset_var();
    return FALSE;
  }

  /* Write the received data into the internal flash. */
  FLASH_ProgramByte(rf_write_address, data, data_length);
//memcpy(data_test,(uint8_t*)plc_write_address , 512);

  /* Read the written bytes directly and compare them with the packet. */
  if(memcmp((uint8_t*)rf_write_address, data, data_length) != 0)
  {
    rf_reset_var();
    return FALSE ;
  }

  /* Move the address by the actual packet data length. */
  rf_write_address += (uint32_t)data_length;

  return TRUE; 
}
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
uint8_t comm_handle_rx_cmd(uint8_t *data_ptr, uint16_t data_size)
{
  // TODO: remove this function
  /*!@par Pseudo Code:*/
  uint16_t elapsed_cmds_data_size = 0;
  uint16_t fnshd_cmd_data_size;
  uint8_t tmp_status;
  uint8_t tmp_block,tmp_card_type;


  
  /* trf_old_activity bookkeeping removed: tariff/payment is not built. */

    
/* was: if(SelfTest_gGetSelfTest() == 0) -- self-test is not built for the DRU,
   so this is always true. Kept as if(1) to leave the block structure alone. */
if(1)
 {
  tmp_status =com_check_size_commands( data_ptr ,  data_size);

  if(tmp_status == 0 || tmp_status == 9 || tmp_status == 7 || tmp_status == 71 || tmp_status == 72 || tmp_status == 73 || tmp_status == 74)
  {
    return tmp_status;
  }
  
/* Replacement-card charge-counter check removed: tariff/payment is not built
   for the DRU, so there is no charge counter to compare against. */
  if (ctrl_get_block_state() == 2)
  {
    if (*data_ptr != CONFIGURE_BLOCK_STATE)
      return FALSE;
  }
  }
  if (pwr_up_sec_cntr < PWR_UP_NO_REC_TIME)
  {
    if (*data_ptr != MTR_STRT_CAL_CMD)
    {
      return FALSE;
    }
  }

  if(customer_flag == 1)
    reset_all_meter_data_cmd();

  while(elapsed_cmds_data_size < data_size)
  {
#ifdef DEBUG_HANG_LOOPING
    uart_debug_byte ('C',0);
#endif
    /*! - Check command code.*/
    switch(*data_ptr)
    {
    case BEGIN_METER_COPY_CMD:
      customer_flag = 1;
      fnshd_cmd_data_size = 0;
      break;
      
    case END_METER_COPY_CMD:
      customer_flag = 0;
      fnshd_cmd_data_size = 0;
      break;      
      
    case METER_RST:
    case COMM_RESET_METER_WITHOT_RST_GPRS:

      reset_all_meter_data_cmd();
      fnshd_cmd_data_size = 0;
      break;
#ifdef PAYMENT_SYS
        /* case CHRG_MNY: removed: tariff/payment and EEPROM are not built for the DRU. */
        /* case DISCHRG_MNY: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
#ifdef TARIFF_SYS
        /* case RST_TRF_PYMT: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
#if (defined(PYMT_VACATION_TRF) || defined(PYMT_FRIENDLY))
      
        /* case VAC_CFG_SET: removed: tariff/payment and EEPROM are not built for the DRU. */
      
#endif
#ifdef PYMT_FRIENDLY
      
        /* case FRDLY_CFG_SET: removed: tariff/payment and EEPROM are not built for the DRU. */
      
#endif
#ifdef PAYMENT_SYS
      
      
        /* case PYMT_CFG_SET: removed: tariff/payment and EEPROM are not built for the DRU. */
      
#endif
#ifdef TARIFF_SYS
      
        /* case TRF_CFG_SET: removed: tariff/payment and EEPROM are not built for the DRU. */
      
#endif
#ifdef PYMT_TAX
      
        /* case TAX_CFG_SET: removed: tariff/payment and EEPROM are not built for the DRU. */
      
#endif
#ifdef TARIFF_SYS
        /* case BP_CFG_SET: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
    case DSPLY_CTRL_CFG_SET:
#ifdef ASHNTTI_PROJECT
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      if(!(fnshd_cmd_data_size = display_set_ctrl_data(data_ptr + 1)))
        return FALSE;
#else
      /*! - Case: Reset tariff and payment, call tariff and payment reset command.*/
#if(DISPLAY_SCREEN_ORDER == CONSTANT_SCREEN_ORDER)
      fnshd_cmd_data_size = 23;
#else
      fnshd_cmd_data_size = 149;
#endif
#endif
      break;
#if 1
    case DSPLY_GO_OP_MODE:
#ifdef ANSI_STACK
      if (mtr_get_mtr_start_accuracy_flag())
      { /*! - Case: Reset tariff and payment, call tariff and payment reset command.*/
        display_operator_mode();
        fnshd_cmd_data_size = 0;
      }
#else    
      /*! - Case: Reset tariff and payment, call tariff and payment reset command.*/
      /* display_operator_mode(); */  /* display module not built for the DRU */
      fnshd_cmd_data_size = 0;
#endif
      break;
#endif
        /* case MTR_RST_ACC_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#ifdef MTR_RVS_TMPR
        /* case MTR_RST_REV_ACC_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
#ifdef MTR_ERTH_TMPR
        /* case MTR_RST_FLT_ACC_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
        /* case MTR_RST_TMP_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
        /* case MTR_RST_LD_PRF_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
        /* case MTR_RST_MD_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
        /* case MTR_STRT_CAL_CMD: removed: metering (Meter_Program/V9203) is not built for the DRU. */
      /*case MTR_NXT_CAL_CMD:
      ! - Case: Next calibration process, call next calibration process command.
      cal_cmd(MTR_NXT_AUTO_CAL);
      fnshd_cmd_data_size = 0;
      break;*/
        /* case MTR_SET_MD_CFG: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#ifdef MTR_ENABLE_LMT
        /* case MTR_SET_LMT_CFG: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
#ifdef LOAD_LIMIT_PLAN
        /* case MTR_SET_LOAD_LMT_CFG: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
        /* case MTR_SET_TMP_CFG: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
#if defined(MTR_INDIRECT)
        /* case INDIRECT_PARAM_SET: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
        /* case MTR_SET_OPR_CFG: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#if 0
    case BEGIN_SELF_TEST:
      /*! - Case: call self_test_event.*/
      self_test_event(SELF_TEST_READY_EVE);
      fnshd_cmd_data_size = 0;
      break;
#endif
        /* case BEGIN_ACCURACY_TEST: removed: metering (Meter_Program/V9203) is not built for the DRU. */
        /* case CONFIG_SET_TIME_CONSUM_TEST_ACCURACY : removed: metering (Meter_Program/V9203) is not built for the DRU. */
    case RESET_CONTROL:
      tmp_block = ctrl_sys_new_var.block_state; 
      tmp_card_type = ctrl_sys_new_var.ctrl_select_which_card_type;
#ifndef ASHNTTI_PROJECT
      if (!customer_flag)
#endif
      {
        ctrl_cmd(CTRL_RST);
      }
      ctrl_sys_new_var.block_state = tmp_block; 
      ctrl_sys_new_var.ctrl_select_which_card_type = tmp_card_type;
      fnshd_cmd_data_size = 0;
      break;
    case RESET_TMPR:
#ifndef ASHNTTI_PROJECT
      if (!customer_flag)
#endif
      {
      ctrl_cmd(CTRL_RST_TMPR);
      update_technician_function(CTRL_RESET_ALL_TMPR );
      ctrl_sys_new_var.consumption_error_contactor = 0;
#ifdef G2_DM
      ctrl_sys_new_var.consumption_error_contactor_money=0;
#endif
      }
      if (ctrl_sys_var_second_optional_feature.test_interface < 3)
        ctrl_sys_var_second_optional_feature.test_reset_counter[ctrl_sys_var_second_optional_feature.test_interface]++;
      fnshd_cmd_data_size = 0;
      break;
#ifdef CTRL_EVNT_LOG
    case RST_EVNT_LOG:
      ctrl_cmd(CTRL_RST_EVNT_LOG);
      fnshd_cmd_data_size = 0;
      break;
#endif
#ifdef CTRL_CFG_METER_LOG
    case RST_CFG_LOG:
      ctrl_cmd(CTRL_RST_CFG_METER_LOG);
      fnshd_cmd_data_size = 0;
      break;
#endif
#ifdef CTRL_RLY
#ifndef ETHIOPIA_METER
    case TST_RLY:
      ctrl_cmd(CTRL_TST_RLY);
      fnshd_cmd_data_size = 0;
      
      break;
#endif
#endif
    case ID_CFG_SET:
#ifdef MTR_SINGLE_PH
      big_endian_memcpy(data_ptr + 1, data_ptr + 1 ,id_ar_size,6);
#endif
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_ID, data_ptr + 1)))
        return FALSE;
#ifdef TARIFF_SYS
      else
      {
#ifdef RF_LINK_FEATURE
        RF_Link_set_id();
#endif
#ifdef TARIFF_SYS
        /* trf_pymt_event(TRF_PYMT_NEW_CUSTOMER, &rst_data); */  /* tariff / EEPROM not built for the DRU */
#endif
      }
#endif
      break;
#ifdef CTRL_ACTIONS
    case ACT_CFG_SET:
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_ACTION, data_ptr + 1)))
        return FALSE;
      break;
#endif
#ifdef CTRL_RTC
    case TIME_CFG_SET:
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_TIME, data_ptr + 1)))
        return FALSE;
      break;
    case DATE_CFG_SET:
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_DATE, data_ptr + 1)))
        return FALSE;
      if(ctrl_sys_var_optional_feature.rtc_date.year > 18)
      {
        //if (ctrl_sys_new_var.count_date_time_change != 0xff )
         // ctrl_sys_new_var.count_date_time_change++;
      }
      update_technician_function(CTRL_SET_DATE_TIME);
      break;
#endif
#if (CTRL_BTRY_NON_CHRG == 1)
    case BTRY_CFG_SET:
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_BTRY, data_ptr + 1)))
        return FALSE;
      break;
#endif
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
    case MAG_CFG_SET:
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_MGNT, data_ptr + 1)))
        return FALSE;
      break;
#endif
#ifdef RFID_FEATURE
    case RFID_UNIQUE_SET:
      if(!(fnshd_cmd_data_size = ctrl_set_unique_rfid(data_ptr + 1)))
        return FALSE;
      break;
#endif
#ifdef GPRS_FEATURE
    case GPRS_CFG_SET:
      if(!(fnshd_cmd_data_size = gprs_store_setting(data_ptr + 1)))
        return FALSE;
      break;
#endif
    case CHNG_TMPLT_ID:
      if(!(fnshd_cmd_data_size = ctrl_change_tmplt_id(data_ptr + 1)))
        return FALSE;
      break;
#if (defined(CTRL_RLY) && defined(MTR_ENABLE_LMT))
    case OVRLOAD_CFG_SET:
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_OVR_LOAD, data_ptr + 1)))
        return FALSE;
      break;
#endif
#ifdef CTRL_RLY
    case ENTER_OPEN_RELAY_MODE:
      ctrl_sys_new_var.ctrl_start_open_relay_test = 0;
      ctrl_cmd(CTRL_ENTER_OPEN_RELAY_MODE);
      fnshd_cmd_data_size = 0;
      
      break;
    case EXIT_OPEN_RELAY_MODE:
      ctrl_cmd(CTRL_EXIT_OPEN_RELAY_MODE);
      fnshd_cmd_data_size = 0;
      
      break;
#endif

    case GPRS_SCRTCH_CARD_SEND:
#ifdef GPRS_SRT_CARD
      *data_ptr = (uint8_t)data_size - 1;
      if(!(fnshd_cmd_data_size = gprs_send_scratch_key(data_ptr)))
        return FALSE;
#else
#ifdef STS_ENABLE
      STS_push_Token_Data(data_ptr+1);
      Reset_UI_STS_token_Status();
      fnshd_cmd_data_size = 0;
#endif
#endif
      break;

#if (defined(CTRL_RLY) || defined(CTRL_BZR))
    case END_HINT_CMD:
      ctrl_cmd(CTRL_END_HINT);
      fnshd_cmd_data_size = 0;
      break;
#endif
    case SET_SERIAL_NUM_CMD:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      if(!(fnshd_cmd_data_size = ctrl_cfg_set(CTRL_CFG_SERIAL, data_ptr + 1)))
        return FALSE;
      break;
      
    case RESET_SERIAL_NUM_CMD:
      ctrl_cfg.serial = 0;
      fnshd_cmd_data_size = 0;
      /* disable RTC isr to avoid time change after calculating CRC*/
      save_ctrl_cfg();
      break;
#ifdef EDIT_TIME_DATE_MANUALLY
#ifndef REMOVE_MANUALLY_SET_DATE // TODO
    case MANUALY_SET_DATE_TIME:
      fnshd_cmd_data_size = 0;      
      /* display_set_edit_mode(); */  /* display module not built for the DRU */
      break;
#endif
#endif
      
    case RESTART_METER_CMD:
      fnshd_cmd_data_size = 0;
      ctrl_event(CTRL_SAVE_CRITICAL_DATA);
      vDelay_HW(1);
      // Reset microcontroller
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('X',2);
#endif
      CORTEX_NVIC_SystemReset(1);      /* system soft reset */
      break;
    case SET_USAGE_TYPE_CMD:
#ifndef MTR_THREE_PH
      /*take usage type only after reset meter or when the same usage type*/
      if((ctrl_sys_var_optional_feature.usage_type != *(data_ptr + 1))&& (ctrl_sys_var_optional_feature.usage_type != 0)&&( *(data_ptr + 1) !=0 ) )
      {
        return FALSE;
      }
#endif
      ctrl_sys_var_optional_feature.usage_type = *(data_ptr + 1);
      ctrl_sys_var_optional_feature.usage_type = ctrl_sys_var_optional_feature.usage_type;
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.usage_type);
      break;
      
    case SET_DEBIT_MONEY_CMD:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      memcpy(&(ctrl_sys_var_optional_feature.debit_money), data_ptr + 1, sizeof(ctrl_sys_var_optional_feature.debit_money));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.debit_money);
      break;
      
/* case Write_All_EEPROM / Write_All_EEPROM2 removed: EEPROM is not built for
   the DRU. */
    case Reset_Meter:
      // Reset microcontroller
#ifdef DEBUG_BOOTLOADER
      uart_debug_byte ('X',0);
#endif
#ifdef ANSI_STACK
INCREMENT_COUNTER_OF_RESET_METER();     
#endif
      CORTEX_NVIC_SystemReset(2);    // jump directly to BSL
      fnshd_cmd_data_size = 0;
      return TRUE;
      break;
    case RESET_SPECIFIC_TAMPER:
      reset_specific_tamper(*(data_ptr + 1));
      update_technician_function(CTRL_RESET_SPECIFIC_TMPR );
      fnshd_cmd_data_size = 1;
      break;
      
    case SET_TARIFF_CALC_W_KW:
#if 0
      if (ctrl_sys_var_optional_feature.tariff_changed)
      {
        ctrl_sys_var_optional_feature.tariff_prev_num = ctrl_sys_var_optional_feature.meter_calcualtion;
      }
      else
        ctrl_sys_var_optional_feature.tariff_prev_num = *(data_ptr + 1);
      
      ctrl_sys_var_optional_feature.meter_calcualtion = *(data_ptr + 1);
#endif
      if (*(data_ptr + 1)&0x01)
        ctrl_sys_var_optional_feature.tariff_prev_num |= 0x01;
      else
        ctrl_sys_var_optional_feature.tariff_prev_num &= 0xfe;
      
      //ctrl_sys_var_second_optional_feature.recalc_flags |= 1<<(ctrl_sys_var_optional_feature.tariff_prev_num >> 1);
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.meter_calcualtion);
      break;
      
      
    case SET_TARIFF_RECALC_CALC_2:
#if 0
      if (ctrl_sys_var_optional_feature.tariff_changed)
      {
        ctrl_sys_var_optional_feature.tariff_prev_num_2 = ctrl_sys_var_optional_feature.meter_calcualtion_2;
      }
      else
        ctrl_sys_var_optional_feature.tariff_prev_num_2 = *(data_ptr + 1);
      
      ctrl_sys_var_second_optional_feature.recalc_flags |= 1<<ctrl_sys_var_optional_feature.tariff_prev_num_2;
      ctrl_sys_var_optional_feature.meter_calcualtion_2 = *(data_ptr + 1);
      
#endif
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.meter_calcualtion_2);
      break;
      
    case SET_WAITING_TARIFF_CALC_W_KW:
#if 0
      ctrl_sys_var_optional_feature.waiting_meter_calcualtion = *(data_ptr + 1);
      ctrl_sys_var_optional_feature.waiting_meter_calcualtion_2 = *(data_ptr + 2);
      ctrl_sys_var_second_optional_feature.future_recalc_flags |= 1<<ctrl_sys_var_optional_feature.waiting_meter_calcualtion | 1<<ctrl_sys_var_optional_feature.waiting_meter_calcualtion_2;
#endif
      fnshd_cmd_data_size = 2;
      break;
      
    case SET_INDUCTION_PF_FEATURE:
      ctrl_sys_var_optional_feature.induction_pf = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;  
      break;
    case SET_TARIFF_TAX_DEDUCTION_MODE:
      ctrl_sys_var_optional_feature.tariff_tax_deduction_mode = *(data_ptr + 1);
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.tariff_tax_deduction_mode);
      break;
    case SET_ZERO_CONSUMBTION_TAX_DEDUCTION_VALUE:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      ctrl_sys_var_optional_feature.zero_consumbtion_tax = *(data_ptr + 1);
      
      memcpy(&( ctrl_sys_var_optional_feature.zero_consumbtion_tax), data_ptr + 1, sizeof( ctrl_sys_var_optional_feature.zero_consumbtion_tax));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.zero_consumbtion_tax);
      break;
      
        /* case SET_CURRENT_CONSUMPTION: removed: tariff/payment and EEPROM are not built for the DRU. */
        /* case SET_TOTAL_CONSUMPTION: removed: tariff/payment and EEPROM are not built for the DRU. */
        /* case SET_CHARGE_NUM: removed: tariff/payment and EEPROM are not built for the DRU. */
#ifdef MTR_LOAD_PROFILE
    case LP_EVENT_NOTIFICATION_TIME_CFG_CMD:
#ifdef DLMS_ENABLED
      
      if(!(fnshd_cmd_data_size = Cal_Evt_cfg_set(CAL_EVT_LOAD_PROFILE_TIME_CFG, data_ptr + 1)))
        return FALSE;
#endif
      break;
#endif 
    case OPEN_CLOSE_RELAY_MODE:
      if (ctrl_sys_var_optional_feature.flag1 & OPEN_RELAY_MODE)
      {
        ctrl_cmd(CTRL_EXIT_OPEN_RELAY_MODE);
        ctrl_sys_new_var.ctrl_start_open_relay_test = 0;
      } 
      else
      {
        ctrl_sys_new_var.ctrl_start_open_relay_test = 1;
        ctrl_start_open_relay_test_sec_counter = 0;
        ctrl_cmd(CTRL_ENTER_OPEN_RELAY_MODE);
      }
      fnshd_cmd_data_size = 0;
      break;
    case ACTIVATE_REMOTE_PROGRAMMING:
      Boot_loader_flag = RECIEVE_BOOTLOADER_DATA;
      fnshd_cmd_data_size = 0;
      break;
    case COMM_CONFIGURE_PBH_GRACE_PERIOD:
      memcpy(&ctrl_sys_var_optional_feature.config_bp_grace_period,data_ptr + 1,sizeof(ctrl_sys_var_optional_feature.config_bp_grace_period));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.config_bp_grace_period);
      break;
    case COMM_CONFIGURE_MAXIMIM_CONSUMPTION://with maximum demand
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      memcpy(&ctrl_sys_var_optional_feature.maximum_consumption_value,data_ptr + 1,sizeof(ctrl_sys_var_optional_feature.maximum_consumption_value) );
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.maximum_consumption_value);
      break;
    case COMM_CONFIGURE_MAXIMIM_CONSUMPTION_ACTION:
      memcpy(&ctrl_sys_var_optional_feature.ctrl_maximum_consumptions_action,data_ptr + 1,sizeof(ctrl_sys_var_optional_feature.ctrl_maximum_consumptions_action) );
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ctrl_maximum_consumptions_action);
      break ;
    case COMM_SET_COMMISSIONING_STATE:
      if (*(data_ptr + 1)==1)
        ctrl_sys_var_optional_feature.ctrl_flags |= CTRL_COMMISSIONING_PROCESS;
      else
        ctrl_sys_var_optional_feature.ctrl_flags &= (~CTRL_COMMISSIONING_PROCESS);
#ifndef GROUP_METER_FEATURE
      ctrl_sys_var_optional_feature.ctrl_flags &= ~CTRL_COMMISSIONING_PROCESS2;
#endif
      fnshd_cmd_data_size = 1;
      break ;
    case COMM_SET_OVERLOAD_IN_POWER_VALUE:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      memcpy((&ctrl_sys_var_optional_feature.ovr_load_in_power_value),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.ovr_load_in_power_value));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ovr_load_in_power_value);
      break;
    case COMM_SET_OVERLOAD_IN_POWER_TIMEOUT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      memcpy((&ctrl_sys_var_optional_feature.ovr_load_in_power_tout),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.ovr_load_in_power_tout));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ovr_load_in_power_tout);
      break;
    case COMM_SET_VACATION_TIME: 
      memcpy((&ctrl_sys_var_optional_feature.vac_time),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.vac_time));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.vac_time);
      break;
    case COMM_SET_VACATION_PRICES:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xea;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 40);
#endif
#ifndef G2_DM
      memcpy((&ctrl_sys_var_optional_feature.vac_prices),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.vac_prices));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.vac_prices);
#endif
      break;
    case COMM_METER_TYPE:
      memcpy((&ctrl_sys_var_optional_feature.meter_type),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.meter_type));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.meter_type);
      break;
    case COMM_SET_LOAD_PROFILE_TIMEOUT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      memcpy((&ctrl_sys_var_optional_feature.load_profile_configuration_time),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.load_profile_configuration_time));
      if (ctrl_sys_var_optional_feature.load_profile_configuration_time < 60)
        ctrl_sys_var_optional_feature.load_profile_configuration_time = 15*60;
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.load_profile_configuration_time);
      break;
    case COMM_CONFIGURE_MAXIMIM_CURRENT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      memcpy((&ctrl_sys_var_optional_feature.maximum_current_value),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.maximum_current_value));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.maximum_current_value);
      break;
        /* case COMM_SET_TOT_CONSUMPTION_ACTIVE: removed: tariff/payment and EEPROM are not built for the DRU. */
#ifdef MTR_THREE_PH
        /* case COMM_SET_TOT_CONSUMPTION_REACTIVE: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
 #ifdef MTR_REACTIVE
        /* case COMM_SET_TOT_CONSUMPTION_APPARENT: removed: metering (Meter_Program/V9203) is not built for the DRU. */
#endif
#if 0
        /* case COMM_RESET_CURRENT_MD_VALUES: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
    case COMM_SET_SERIAL_PREFIX_CHAR:
      memcpy((&ctrl_sys_var_optional_feature.serial_prefix),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.serial_prefix));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.serial_prefix);
      break;
#ifdef IEC_62056_21_SLAVE
        /* case COMM_CHECK_REMOTE_EEPROM: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
        /* case SET_TOTAL_CONSUMPTION_HIGH removed: metering is not built for the DRU. */
        /* case COMM_SET_NK_TAX: removed: tariff/payment and EEPROM are not built for the DRU. */
    case CTRL_SET_CONSUMPTION_ALARM_VALUE:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      memcpy((&ctrl_sys_var_optional_feature.ctrl_consumption_alarm_value),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.ctrl_consumption_alarm_value));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ctrl_consumption_alarm_value);
      break;
    case CTRL_SET_CONSUMPTION_GRACE_AFTER_RECALC:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xc0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      memcpy((&ctrl_sys_var_optional_feature.ctrl_grace_consumption),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.ctrl_grace_consumption));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ctrl_grace_consumption);
      break;
    case CTRL_SET_CONSUMPTION_ALARM_ACTION:
      memcpy((&ctrl_sys_var_optional_feature.ctrl_consumption_alarm_action),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.ctrl_consumption_alarm_action));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ctrl_consumption_alarm_action);
      break;
    case CTRL_SET_DAYS_GRACE_AFTER_RECALC:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      memcpy((&ctrl_sys_var_optional_feature.ctrl_grace_days_after_recalc_2_cfg),(data_ptr + 1), sizeof(ctrl_sys_var_optional_feature.ctrl_grace_days_after_recalc_2_cfg));
      if (ctrl_sys_var_optional_feature.ctrl_grace_days_after_recalc_2_cfg== 255)
        ctrl_sys_var_optional_feature.ctrl_grace_days_after_recalc_2_cfg = 0;
      
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.ctrl_grace_days_after_recalc_2_cfg);
      break;
        /* case TRF_SET_MONEY_BALANCE: removed: tariff/payment and EEPROM are not built for the DRU. */

#if 0
    case SET_MAX_MDA:
      memcpy(((uint8_t *)&ctrl_sys_new_var.configure_Max_mda),(data_ptr + 1), sizeof(ctrl_sys_new_var.configure_Max_mda));
      fnshd_cmd_data_size = 4;//sizeof(ctrl_sys_new_var.configure_Max_mda); TODO Should returned back
      break;
#else
        /* case COMM_SET_NK_TAX2: removed: tariff/payment and EEPROM are not built for the DRU. */
    case COMM_SET_MAXIMUM_CONSUMPTION_NK_TAX1:
      ctrl_sys_new_var.nk_limit_maximum = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;
      break;
#endif
#if 0
    case SET_MAX_TAMPER_DEBIT:
      memcpy(((uint8_t *)&ctrl_sys_new_var.all_tamper_debit),(data_ptr + 1), sizeof(ctrl_sys_new_var.all_tamper_debit));
      fnshd_cmd_data_size = sizeof(ctrl_sys_new_var.all_tamper_debit);
      break;
#endif
/* case TRF_SET_CURRENT_BP_BILL removed: tariff/payment is not built for the DRU. */
    case TRF_SET_CURRENT_REACTIVE:
   //   memcpy(((uint8_t *)&TRF_PYMT_CURR_BP_REACTV_VARH),(data_ptr + 1), sizeof(TRF_PYMT_CURR_BP_REACTV_VARH));
     // fnshd_cmd_data_size = sizeof(TRF_PYMT_CURR_BP_REACTV_VARH);
      break;
        /* case MTR_SET_MD_REGISTERS: removed: metering (Meter_Program/V9203) is not built for the DRU. */
        /* case TRF_SET_TARIFF_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
        /* case TRF_SET_PAYMENT_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    case SET_GPRS_NOTIFY_TAMPER_REPEAT_TIMEOUT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.gprs_notifyTamperRepeateTime), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.gprs_notifyTamperRepeateTime));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_second_optional_feature.gprs_notifyTamperRepeateTime);
      break;
    case SET_GPRS_NOTIFY_BILLING_CHARGING_REPEAT_TIMEOUT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.gprs_notify_time), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.gprs_notify_time));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_second_optional_feature.gprs_notify_time);
      break;
#if 0
    case SET_GPRS_NOTIFY_KEY_COMMISSIONING_REPEAT_TIMEOUT:
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.gprs_notify_key_commissioning_time), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.gprs_notify_key_commissioning_time));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_second_optional_feature.gprs_notify_key_commissioning_time);
      break;
#endif
    case CHANGE_48_HOUR_AFTER_SELLING:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_new_var.grace_hours_cfg), data_ptr + 1, sizeof(ctrl_sys_new_var.grace_hours_cfg));
      fnshd_cmd_data_size=sizeof(ctrl_sys_new_var.grace_hours_cfg);
      break;
    case SET_GPRS_SERVER_RESTART_TIMEOUT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.gprs_resetTimeOut), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.gprs_resetTimeOut));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_second_optional_feature.gprs_resetTimeOut);
      break;
    case SET_GPRS_WAIT_ACK_TIMEOUT:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.gprs_waitAckTimeout), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.gprs_waitAckTimeout));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_second_optional_feature.gprs_waitAckTimeout);
      break;
    case CTRL_SET_SENT_TAMPER_FLAG_CMD :
      memcpy(ctrl_sys_var_second_optional_feature.enable_send_flag_tamper,(data_ptr+1) ,sizeof(ctrl_sys_var_second_optional_feature.enable_send_flag_tamper));
      fnshd_cmd_data_size=sizeof(ctrl_sys_var_second_optional_feature.enable_send_flag_tamper);
      break;
    case CTRL_ENABLE_GPRS_SEND_NOTIFICATION_CMD :
      memcpy((uint8_t*)&ctrl_sys_var_second_optional_feature.Enable_Send_Notification,(data_ptr+1) ,sizeof(ctrl_sys_var_second_optional_feature.Enable_Send_Notification));
      fnshd_cmd_data_size=sizeof(ctrl_sys_var_second_optional_feature.Enable_Send_Notification);
      break;
    case SET_RECLAC_FLAGS_CMD:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.recalc_flags), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.recalc_flags));
      fnshd_cmd_data_size=sizeof(ctrl_sys_var_second_optional_feature.recalc_flags);
      break;
    case SET_FUTURE_RECLAC_FLAGS_CMD:
#if 0
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&(ctrl_sys_var_second_optional_feature.future_recalc_flags), data_ptr + 1, sizeof( ctrl_sys_var_second_optional_feature.future_recalc_flags));
      fnshd_cmd_data_size=sizeof(ctrl_sys_var_second_optional_feature.future_recalc_flags);
#endif
      break;
    case CONFIG_SET_DISPLAY_SCREEN:
#if 0
#ifdef MTR_SINGLE_PH
     fnshd_cmd_data_size = (data_ptr[1]*256+ data_ptr[2])*4 + sizeof(display_ctrl_data_t);
#else
     fnshd_cmd_data_size = (data_ptr[2]*256+ data_ptr[1])*4 + sizeof(display_ctrl_data_t);
#endif
#ifdef LOCAL_WITH_DLMS 
      if (data_ptr[8] == (calc_crc((uint8_t*)&data_ptr[1], sizeof(display_ctrl_data) - 1,CRC)))
      {
#ifdef MTR_SINGLE_PH
        var_temp_size =0xa2;
        big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 4);
#endif
        data_ptr[8] = calc_crc((uint8_t*)&data_ptr[1], sizeof(display_ctrl_data) - 1,CRC);
         if(fnshd_cmd_data_size <=( (DISPLAY_MAXIMUM_SCREENS*4) + sizeof(display_ctrl_data_t)))
         {
           /* file_sys_write(data_ptr + 1, FILE_SYS_GET_DISPLAY_SHOW,fnshd_cmd_data_size);//512ul); */  /* file system not built for the DRU */
         }
      /*set configuration display data*/
      /* display_set_ctrl_data(); */  /* display module not built for the DRU */
      }
#endif
#endif
      break;
    case CONFIG_SET_DURATION_MINUTES_TEST_RELAY:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, (data_ptr  + 1),&var_temp_size, 2);
#endif
      ctrl_test_relay_flag ^= 0x01;
      memcpy((uint8_t *)&ctrl_test_relay_minutes , (data_ptr + 1),sizeof(ctrl_test_relay_minutes));
      ctrl_test_relay_second =0;
      fnshd_cmd_data_size = sizeof(ctrl_test_relay_minutes);
      break;
#if 0
    case CONFIG_RESET_CONSUMPTION_ERR_CONTACTOR:
      ctrl_sys_new_var.consumption_error_contactor = 0;
      fnshd_cmd_data_size = 0;
      break;
#endif
#if 0
    case CONFIG_RESET_COUNT_TIME_DATE_CHANGE:
      ctrl_sys_new_var.count_date_time_change = 0;
      fnshd_cmd_data_size = 0;
      break;
#endif
    case SET_12_BH_RECORD:
      {
#if 0      
        uint8_t i;
        uint8_t *tmp_ptr = data_ptr;
        trf_pymt_bph_rec_t bph_record_temp;
        /* file_sys_clear_log(FILE_SYS_BPH_RECORD); */  /* file system not built for the DRU */
        tmp_ptr++;
        for(i=0;i<12;i++)
        {
          //check year
          memcpy((uint8_t*)&bph_record_temp,tmp_ptr,sizeof(trf_pymt_bph_rec_t));
          if(/*bph_record_temp.day==1 &&*/(bph_record_temp.month >0 && bph_record_temp.month <=12) )
          {
            /* file_sys_save_record(FILE_SYS_BPH_RECORD, tmp_ptr); */  /* file system not built for the DRU */
          }
          else
          {
            break;
          }
          tmp_ptr += sizeof(trf_pymt_bph_rec_t);
        }
        fnshd_cmd_data_size =(sizeof(trf_pymt_bph_rec_t)*12);
#endif
        break;
      }
    case CONFIG_CUST_SERV_CAL_TYP:
      ctrl_sys_new_var.first_or_last_trf = data_ptr[1];
      /*1 ----for deduction in the beginning tariff ,0-----for deduction in the last tariff */
      fnshd_cmd_data_size = sizeof(ctrl_sys_new_var.first_or_last_trf );
      break;
    case CONFIG_STATE_OF_RESET_OVR_LOAD_COUNTER:
      ctrl_sys_new_var.ovr_load_times_cntr_flag = data_ptr[1];
      fnshd_cmd_data_size = sizeof(ctrl_sys_new_var.ovr_load_times_cntr_flag );
      break;
    case SET_FRIENDLY_CONSUMPTION:
      {
#ifdef MTR_SINGLE_PH
        var_temp_size =0xc0;
        big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
        memcpy((uint8_t *)&ctrl_sys_var_optional_feature.friendly_amount_consumed_kwh,(data_ptr + 1),sizeof(ctrl_sys_var_optional_feature.friendly_amount_consumed_kwh));
        fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.friendly_amount_consumed_kwh);
        break;
      }
        /* case SET_CURRENT_BILL: removed: tariff/payment and EEPROM are not built for the DRU. */
    case SET_CREENT_BPH_INDEX:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
   //   memcpy((uint8_t*)&ctrl_sys_var_optional_feature.record_index[FILE_SYS_BPH_RECORD],(data_ptr + 1),sizeof(ctrl_sys_var_optional_feature.record_index[FILE_SYS_BPH_RECORD]));
   //   fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.record_index[FILE_SYS_BPH_RECORD]);
      break;
    case SET_NOT_MAKE_NEW_BILLING_WITH_NEW_DATE_CFG:
#ifdef G2_DM
    if(*(data_ptr + 1) & CHECH_NO_TIME_BACK)
#endif
    {
      ctrl_sys_new_var.trf_not_make_new_billing_with_new_date_cfg = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;
    }
      break;
    case SET_MAKE_PREV_12MONTH_RECALC_CFG:
      ctrl_sys_new_var.trf_make_previous_12_month_recalc_cfg = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;
      break;  
    case SET_48_HOUR_MONTHLY:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&ctrl_sys_new_var.grace_cfg_monthly,(data_ptr + 1),sizeof(ctrl_sys_new_var.grace_cfg_monthly));
      fnshd_cmd_data_size = sizeof(ctrl_sys_new_var.grace_cfg_monthly);
      break;
    case CONFIGURE_BLOCK_STATE:
      ctrl_sys_new_var.block_state = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;
      break;
    case CTRL_CFG_ONE_RELAY_CMD_CONNECT:
      ctrl_sys_new_var.Enable_one_time_connect_relay = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;
      break;
    case CTRL_CHANGE_RFID_CARD_TYPE:
      ctrl_sys_new_var.ctrl_select_which_card_type = *(data_ptr + 1);
      fnshd_cmd_data_size = 1;
      break;   
    case PLC_FIRMWARE_SAVING_CMD:
      {
  uint16_t plc_data_size = 0;

  /*
   * Calculate the number of bytes after D4.
   * The final five frame bytes are already excluded.
   */
  plc_data_size = data_size - 1u;
  /*
   * data_ptr points to D4.
   * data_ptr + 1 points to the first byte that must be saved.
   */
  if(save_plc_firmware_data(data_ptr + 1, plc_data_size) == FALSE)
  {
    return FALSE; // Something went wrong 23mlha trace
  }

  /*
   * Tell the command loop how many bytes were handled.
   * The loop itself will add one for D4.
   */
  fnshd_cmd_data_size = plc_data_size;
  /* Reset the timer */
  CommResetDruCounterTimeOut();   
  break;
    }
      case RF_FIRMWARE_SAVING_CMD:
      {
  uint16_t rf_data_size = 0;

  /*
   * Calculate the number of bytes after D4.
   * The final five frame bytes are already excluded.
   */
  rf_data_size = data_size - 1u;
  /*
   * data_ptr points to D4.
   * data_ptr + 1 points to the first byte that must be saved.
   */
  if(save_rf_firmware_data(data_ptr + 1, rf_data_size) == FALSE)
  {
    return FALSE; // Something went wrong 23mlha trace
  }

  /*
   * Tell the command loop how many bytes were handled.
   * The loop itself will add one for D4.
   */
  fnshd_cmd_data_size = rf_data_size;
  /* Reset the timer */
  CommResetDruCounterTimeOut();   
  break;
    }

/* case TRF_TOTAL_CHARGES_AMOUNT removed: tariff/payment is not built for the DRU. */
#if defined(DOT_MATRIX_LCD_ENABLE) 
      case DOT_MATRIX_LCD_FLASH_WR:
        if(ctrl_sys_new_var.ctrl_change_dot_matrix_language_flag > 2)
          ctrl_sys_new_var.ctrl_change_dot_matrix_language_flag = 0;
      fnshd_cmd_data_size = Writing_On_Flash(data_ptr);
      CommResetDruCounterTimeOut();     
      DotmatrixSend = 1;
#ifndef SAVING_DIRECT_INTERNAL
      DotMatrixOpticalFlag = 1;
#endif
      break;
      case DOT_MATRIX_CHANGE_LANGUAGE:
      ctrl_sys_new_var.ctrl_change_dot_matrix_language_flag= *(data_ptr+1);
      fnshd_cmd_data_size =1;  
      break;
      #endif  
      case SET_ACITVITY_ID:
#ifdef MTR_SINGLE_PH
      var_temp_size =0x80;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 2);
#endif
      memcpy((uint8_t*)&ctrl_sys_var_second_optional_feature.new_activity_id,(data_ptr + 1),sizeof(ctrl_sys_var_second_optional_feature.new_activity_id));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_second_optional_feature.new_activity_id);
      break;
    case CTRL_SET_DAILY_TAXES_REGISTER:
#ifdef MTR_SINGLE_PH
      var_temp_size =0xC0;
      big_endian_memcpy(data_ptr + 1, data_ptr + 1,&var_temp_size, 4);
#endif
      memcpy((uint8_t*)&ctrl_sys_var_optional_feature.consumed_daily_tax,(data_ptr + 1),sizeof(ctrl_sys_var_optional_feature.consumed_daily_tax));
      fnshd_cmd_data_size = sizeof(ctrl_sys_var_optional_feature.consumed_daily_tax);
      break; 
    default:
      return FALSE;
    }/* end of switch(cmd_code)*/
    elapsed_cmds_data_size += (fnshd_cmd_data_size + 1);
    data_ptr += (fnshd_cmd_data_size + 1);
  }
  if(elapsed_cmds_data_size != data_size)
    return FALSE;
  return TRUE;
}

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
//ismail
/* comm_handle_rx_bootloader_data() removed: not reachable in the DRU build. */

#if 0
void read_bootloader_data ()
{
  uint32_t i = 0  , j=0;
  uint8_t arr2[300] , arr[300];
  for( i = 0 ;i < 300 ; i++)
  {
    arr2[i]= 8 ;
  }
  current_pckt_ID = 514;
  while(WriteInEEPROM_2M(CURRENT_PACKET_ADDRESS ,  arr2 , (uint32_t)256)== false);
  while(ReadFromEEPROM_2M(CURRENT_PACKET_ADDRESS,  arr , (uint32_t)256)== false);
  while(ReadFromEEPROM_2M(0,  arr , (uint32_t)256)== false);
  while(ReadFromEEPROM_2M(256,  arr , (uint32_t)256)== false);
  while(ReadFromEEPROM_2M(512,  arr , (uint32_t)256)== false); 
  SendFrameUSCI_A3(arr2, 256);
  
  for(i = 0 ; i<830 ; i++)
  {
    j = i*256;
    while(ReadFromEEPROM_2M(j ,  arr2 , (uint32_t)256)== false);
    SendFrameUSCI_A3(arr2, 256);
  }
  
  
}
#endif
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
#ifdef TEST_ENDIANNESS_RAW_DATA
void fill_data(uint8_t* ptr_data ,uint16_t length_var)
{
  uint16_t index=0,index1=0;
  uint8_t *ptr_struct;
  ptr_struct = ptr_data;
  for(index=0;index<length_var;index++)
  {
    ptr_struct[index] =index1;
    index1++;
    if(index1>10)
    {
      index1 =0;
    }
  }
}
#endif
#define  FIRMWARE_VERSION     "4.526"
uint16_t comm_read_raw_data(uint8_t *buffer_ptr, uint8_t type)
{
  /*!@par Pseudo Code:*/
  uint8_t *buffer_index = buffer_ptr;
#if defined (MTR_SINGLE_PH) ||defined(ASHNTTI_PROJECT)
  uint8_t*buffer_ptr_endian=buffer_ptr;
#endif
  uint8_t len = 0;
  memset(buffer_ptr, 0, MAIN_DATA_SIZE);
  uint16_t cal=0;
  switch(type)
  {
    /*! - Check type of data required.*/
  case COMM_MAIN_DATA:
    /*! If the type is main meter data.*/
    /*! - Write endian format and time now.*/
#ifdef BIG_ENDIAN
    buffer_ptr[0] = 1;
#else
    buffer_ptr[0] = 0;
#endif
    buffer_ptr[1] = COMM_MIN;
    buffer_ptr[2] = COMM_HR;
    buffer_ptr[3] = COMM_DAY;
    buffer_ptr[4] = COMM_MONTH;
    buffer_ptr[5] = COMM_YEAR;
    /*! - Get firmware version and defined feature.*/
    memcpy(buffer_ptr + 6, FIRMWARE_VERSION, 6);
    CTRL_READ_TMPLT_ID(buffer_ptr + 12);
#ifdef MTR_SINGLE_PH
    var_temp_size =0x80;
    big_endian_memcpy(buffer_ptr + 12, buffer_ptr + 12,&var_temp_size, 2);
#endif
    /* Tariff feature bytes: the tariff module is not built for the DRU, but
       these four bytes sit at a fixed offset in the packet and send_data_pckt()
       does not clear iec_comm_buffer, so they must be zeroed explicitly --
       otherwise they carry stale bytes from the previously received packet. */
    memset(buffer_ptr + 14, 0, 4);
    /* Metering feature bytes: metering is not built for the DRU. Same fixed-offset
       rule as above -- zero them rather than leaving them unwritten. */
    memset(buffer_ptr + 18, 0, 4);
    COMM_GET_CTRL_FETUR(buffer_ptr + 22);
#ifdef ASHNTTI_PROJECT
    buffer_ptr[26] = COMM_GET_DSPLY_FETUR();
#else
#ifdef MTR_SINGLE_PH
    uint16_t ver;
    ver = SwapEndian16(ctrl_sys_new_var.boot_ver);
    memcpy(&buffer_ptr[26],(uint8_t*)&ver,2);
#else
    memcpy(&buffer_ptr[26],&ctrl_sys_new_var.boot_ver,2);
#endif
#endif
    buffer_ptr[28] = COMM_GET_RFID_FETUR();
    buffer_ptr[29] = COMM_GET_GPRS_FETUR();
#ifdef RF_LINK_FEATURE
    buffer_ptr[30] = COMM_GET_RF_FETUR();
#else
    buffer_ptr[30] = 0;
#endif    
    buffer_ptr[31] = COMM_GET_IEC_FETUR();
    /*! - Get new tamper status and number of records saved in all logs.*/
    buffer_ptr[32] = GET_NEW_TMPR_FLG();
    if (GET_NO_OF_SAVED_BPH_REC() > 255)
      buffer_ptr[33] = 255;
    else
      buffer_ptr[33] = GET_NO_OF_SAVED_BPH_REC();
    if (GET_NO_OF_SAVED_EVNT_REC() > 0xffff)
    {
      buffer_ptr[34] = 0xff;
      buffer_ptr[35] = 0xff;
    }
    else
    {
#ifdef MTR_SINGLE_PH
      buffer_ptr[35] = (uint8_t)(GET_NO_OF_SAVED_EVNT_REC() & 0x00ff);
      buffer_ptr[34] = (uint8_t)((GET_NO_OF_SAVED_EVNT_REC() >> 8) & 0x00ff);
#else
      buffer_ptr[34] = (uint8_t)(GET_NO_OF_SAVED_EVNT_REC() & 0x00ff);
      buffer_ptr[35] = (uint8_t)((GET_NO_OF_SAVED_EVNT_REC() >> 8) & 0x00ff);
#endif
    }
    if (GET_NO_OF_SAVED_MNY_REC()>255)
    {
      buffer_ptr[36] = 0xff;
    }
    else
      buffer_ptr[36] = GET_NO_OF_SAVED_MNY_REC();
    /* Load-profile record count: metering is not built for the DRU, so there is
       no profile log. Fixed-offset field -- write zeros explicitly. */
    buffer_ptr[37] = 0;
    buffer_ptr[38] = 0;
    if (GET_NO_OF_SAVED_CFG_METER_REC() > 255)
      buffer_ptr[39] = 255;
    else
      buffer_ptr[39] = (uint8_t) GET_NO_OF_SAVED_CFG_METER_REC();
    GET_SERIAL_NUMBER(&buffer_ptr[40]);
#ifdef MTR_SINGLE_PH
 //   var_temp_size =0xc0;
 //   big_endian_memcpy(&buffer_ptr[40], &buffer_ptr[40],&var_temp_size, 4); already done in get serial function
#endif
    /*! - Return size of main meter data packet #MAIN_DATA_SIZE .*/
    return MAIN_DATA_SIZE;
        /* case COMM_TRF_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
        /* case COMM_METERING_DATA: removed: metering (Meter_Program/V9203) is not built for the DRU. */
  case COMM_CTRL_DATA:
    /*! If the type is control data, copy data and return its size.*/
    buffer_ptr += ctrl_read_data(buffer_ptr);
#ifdef MTR_SINGLE_PH
#ifdef ASHNTTI_PROJECT
    big_endian_memcpy(buffer_ptr_endian,buffer_ptr_endian,basic_ar_size,38);
#else
    big_endian_memcpy(buffer_ptr_endian,buffer_ptr_endian,basic_ar_size,44);
#endif
#endif
    COMM_RESET_RFID_TAMPER();
    return (buffer_ptr - buffer_index);
// #ifdef SYSLINK_ENABLE
        /* case COMM_METERING_INST_DATA: removed: metering (Meter_Program/V9203) is not built for the DRU. */
// #endif
#ifdef TARIFF_SYS
        /* case COMM_TRF_CFG: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
#ifdef PAYMENT_SYS
        /* case COMM_PYMT_CFG: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
#ifdef PYMT_TAX
        /* case COMM_TAX_CFG: removed: tariff/payment and EEPROM are not built for the DRU. */
#endif
    
    // ADDED BY AMR
        /* case COMM_VACATION_CFG_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    
        /* case COMM_FRIENDLY_CFG_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    
        /* case COMM_PYMENT_CFG_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    
        /* case COMM_TARIFF_CFG_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    
        /* case COMM_TAX_CFG_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    
        /* case COMM_BP_CFG_DATA: removed: tariff/payment and EEPROM are not built for the DRU. */
    
    
  case COMM_CONTROL_CFG_DATA:

    /* No display control data to report: display is not built for the DRU. */
    len = 0;
#ifdef MTR_SINGLE_PH
    big_endian_memcpy(buffer_ptr_endian,buffer_ptr_endian,display_ctrl_data_t_ar_size, 7);
#endif
    buffer_ptr += len;
    return (buffer_ptr - buffer_index);
    
    break;
    
        /* case COMM_MAXIMUM_DEMAND_CFG_DATA: removed: metering (Meter_Program/V9203) is not built for the DRU. */
    
        /* case COMM_LIMITER_CFG_DATA: removed: metering (Meter_Program/V9203) is not built for the DRU. */
    
    
        /* case COMM_TAMPERS_CFG_DATA: removed: metering (Meter_Program/V9203) is not built for the DRU. */
    
        /* case COMM_OPERATING_POINT_CFG_DATA: removed: metering (Meter_Program/V9203) is not built for the DRU. */
    
  case COMM_ID_CFG_DATA:

    memcpy(buffer_ptr, &(ctrl_cfg.id), sizeof(ctrl_cfg.id));
#ifdef MTR_SINGLE_PH
    big_endian_memcpy(buffer_ptr_endian,buffer_ptr_endian,id_ar_size,6);
#endif
    buffer_ptr += sizeof(ctrl_cfg.id);
    return (buffer_ptr - buffer_index);
    
    break;
    
  case COMM_ACTIONS_ALARMS_CFG_DATA:
    
    memcpy(buffer_ptr, &(ctrl_cfg.tmpr), sizeof(ctrl_cfg.tmpr));
    buffer_ptr += sizeof(ctrl_cfg.tmpr);
    return (buffer_ptr - buffer_index);
    
    break;
    
  case COMM_TIME_CFG_DATA:
    
    memcpy(buffer_ptr, &(ctrl_sys_var_optional_feature.rtc_time), sizeof(ctrl_sys_var_optional_feature.rtc_time));
    buffer_ptr += sizeof(ctrl_sys_var_optional_feature.rtc_time);
    return (buffer_ptr - buffer_index);
    
    break;
    
  case COMM_DATE_CFG_DATA:
    
    memcpy(buffer_ptr, &(ctrl_sys_var_optional_feature.rtc_date), sizeof(ctrl_sys_var_optional_feature.rtc_date));
    buffer_ptr += sizeof(ctrl_sys_var_optional_feature.rtc_date);
    return (buffer_ptr - buffer_index);
    
    break;
    
  case COMM_BATTERY_CFG_DATA:
    
    memcpy(buffer_ptr, &(ctrl_cfg.btry), sizeof(ctrl_cfg.btry));
    buffer_ptr += sizeof(ctrl_cfg.btry);
    return (buffer_ptr - buffer_index);
    
    break;
#if (CTRL_MGNT_SW ==1 && CTRL_MGNT_SENSOR == 1 )
  case COMM_MAGNETIC_CFG_DATA:
    memcpy(buffer_ptr, &(ctrl_cfg.mgnt), sizeof(ctrl_cfg.mgnt));
    buffer_ptr += sizeof(ctrl_cfg.mgnt);
    return (buffer_ptr - buffer_index); 
    break;
#endif
#ifdef RFID_FEATURE    
  case COMM_RFID_UNIQUE_ID_CFG_DATA:
    memcpy(buffer_ptr, &(ctrl_sys_var.basic.rfid_unique), sizeof(ctrl_sys_var.basic.rfid_unique));
    buffer_ptr += sizeof(ctrl_sys_var.basic.rfid_unique);
    return (buffer_ptr - buffer_index);  
    break;
#endif
#ifdef GPRS_FEATURE
  case COMM_GPRS_CFG_DATA:
    
    memcpy(buffer_ptr, &(gprs_ctl), sizeof(gprs_ctl));
    buffer_ptr += sizeof(gprs_ctl);
    return (buffer_ptr - buffer_index);
#endif
    break;
#if (defined(CTRL_RLY) && defined(MTR_ENABLE_LMT))    
  case COMM_OVERLOAD_ALARMS_CFG_DATA:
    memcpy(buffer_ptr, &(ctrl_cfg.overload), sizeof(ctrl_cfg.overload));
    buffer_ptr += sizeof(ctrl_cfg.overload);
    return (buffer_ptr - buffer_index); 
    break;
#endif   

  case COMM_RETURN_CORRUPTION:
  //  memcpy(buffer_ptr,(uint8_t*)&eeprom_load_errors_register, sizeof(eeprom_load_errors_register));
 //   buffer_ptr += sizeof(eeprom_load_errors_register);
    
    cal = (uint16_t)RTC->CAL;
    memcpy(buffer_ptr,(uint8_t*)&cal, sizeof(cal));
    buffer_ptr += sizeof(cal);
#ifdef MTR_SINGLE_PH
    big_endian_memcpy(buffer_ptr_endian,buffer_ptr_endian,Corruption_reading_ar_size,sizeof(cal)+sizeof(eeprom_load_errors_register));
#endif
#ifdef STS_ENABLE
    memcpy(buffer_ptr,(uint8_t*)&STS_Master.STS_PRIMARY_KEY, sizeof(STS_Master.STS_PRIMARY_KEY));
    buffer_ptr += sizeof(STS_Master.STS_PRIMARY_KEY);
#endif
#ifdef RF_CC1120
    CC1120_Get_Important_Flags();
    memcpy(buffer_ptr,(uint8_t*)RF_flags_arr, sizeof(RF_flags_arr));
    buffer_ptr += sizeof(RF_flags_arr);
#endif
    return (buffer_ptr - buffer_index);
    break;

  case COMM_RETURN_NEW_CTRL_DATA://need test
    memcpy(buffer_ptr, (uint8_t*)&ctrl_sys_new_var, sizeof(ctrl_sys_new_var));    
#ifdef MTR_SINGLE_PH
    big_endian_memcpy(buffer_ptr,buffer_ptr,ctrl_sys_new_var_t_ar_size,sizeof(ctrl_sys_new_var));
#endif
    return (sizeof(ctrl_sys_new_var));
  default:
    /*! - The MSB refered that the command for reading data EEPROM.
    The other 7bit represents the page number to be read.
    */
#if 1
    if (type >= COMM_READ_DATA_EEPROM_PAGES)
    {
      /* EEPROM_read(0xa4, buffer_ptr, ((type&(~COMM_READ_DATA_EEPROM_PAGES))*EEPROM_PAGE_SIZE), EEPROM_PAGE_SIZE); */  /* tariff / EEPROM not built for the DRU */
   //   return EEPROM_PAGE_SIZE;
    }
#endif
    return 0;
  }
}


/* comm_remote_programming() removed: not reachable in the DRU build. */
/*!
* @brief Initializtion of all communication interfaces.
*
* @par Description:
* Initailize different communication interfaces.
*
* @return
* - None.
*/
/* comm_init() removed: not reachable in the DRU build. */

/*!
* @brief Task of all communication interfaces.
*
* @par Description:
* Calls all task functions of communication interfaces.
*
* @return
* - None.
*/

void comm_task(void)
{  
   CommDruTask();  
   iec_62056_21_sub_task();   
   iec_62056_21_slave_task();
}

void bootloader_newsec_event()
{
  Boot_loader_timeout++;
}

/* get_comm_source() removed: not reachable in the DRU build. */

/* set_comm_source() removed: not reachable in the DRU build. */
uint8_t com_check_size_commands(uint8_t *data_ptr, uint16_t data_size)
{
  /*!@par Pseudo Code:*/
  uint16_t elapsed_cmds_data_size = 0;
  uint16_t fnshd_cmd_data_size = 0;
  uint8_t time_out = 0;
  uint8_t get_general_zero_flag=0;
  uint8_t get_initial_chrg=0;
  uint32_t rcv_customer_id=0;
  
  /* Contactless (cyshield) card-type gate dropped: the MFRC522 reader is not
     built for the DRU, so there is no card type to check here. */
  while((elapsed_cmds_data_size < data_size) && (time_out++ < 0xff))
  {    
    if(( *data_ptr == 0)||(( *data_ptr >43)&&( *data_ptr <100))||( *data_ptr == 112)||( *data_ptr == 121)||\
      ( *data_ptr == 175)||(( *data_ptr >177)&&( *data_ptr <180))||\
        (( *data_ptr >183)&&( *data_ptr <186))||(( *data_ptr >LAST_COMMAND)&&( *data_ptr <254))||(*data_ptr ==255)) 
    {
      return FALSE;
    }
    else if(*data_ptr == SET_12_BH_RECORD )
    {
  //    fnshd_cmd_data_size = sizeof(trf_pymt_bph_rec_t)*12;
    }
    else if(*data_ptr ==CONFIG_SET_DISPLAY_SCREEN)
    {
#ifdef MTR_SINGLE_PH
      fnshd_cmd_data_size = (data_ptr[1]*256+ data_ptr[2])*4 + sizeof(display_ctrl_data_t);
#else
    //  fnshd_cmd_data_size =(data_ptr[2]*256+ data_ptr[1])*4 + sizeof(display_ctrl_data_t);
#endif
    }
    else if (*data_ptr == Write_All_EEPROM2)
    {
      fnshd_cmd_data_size = 260;
    }
   /*
   * D4 owns every remaining byte in this command packet.
   * Subtract one byte because data_ptr[0] is D4.
   */
    else if(*data_ptr == PLC_FIRMWARE_SAVING_CMD)
    {
      /* Save el size bta3 el command bta3tk fl finished command data */
      /* elapsed cmds de l bytes el already 7aslha handle 2bl kda bs kda kda mfysh */
      fnshd_cmd_data_size = data_size - elapsed_cmds_data_size - 1u;
    }
       /*
   * D3 owns every remaining byte in this command packet.
   * Subtract one byte because data_ptr[0] is D3.
   */
    else if(*data_ptr == RF_FIRMWARE_SAVING_CMD)
    {
      /* Save el size bta3 el command bta3tk fl finished command data */
      /* elapsed cmds de l bytes el already 7aslha handle 2bl kda bs kda kda mfysh */
      fnshd_cmd_data_size = data_size - elapsed_cmds_data_size - 1u;
    }

    /* MTR_SET_MD_REGISTERS size branch removed along with its command case:
       metering is not built for the DRU. */

    else if(*data_ptr == DOT_MATRIX_LCD_FLASH_WR)
    {
      fnshd_cmd_data_size = 261;
    }
//#ifdef GPRS_SRT_CARD
    else if(*data_ptr == GPRS_SCRTCH_CARD_SEND)
    {
      fnshd_cmd_data_size = data_size -1;
    }
//#endif
    else
    {
#ifndef STS_ENABLE   
     if ((*data_ptr == METER_RST) && !CTRL_TRMNL_OPN_STAT)
     {
       return 73;
     }

     if((*data_ptr ==  EXIT_OPEN_RELAY_MODE)  &&  (ctrl_sys_new_var.new_customer.new_customer_inserted_flag == 0) )
     {
       if (!(CTRL_BTRY_STAT))
         return 71;
       if (CTRL_TRMNL_OPN_STAT)
         return 72;
     }

     if ( (*data_ptr ==  EXIT_OPEN_RELAY_MODE) && (ctrl_sys_new_var.Enable_one_time_connect_relay == 0x01) &&  ctrl_sys_new_var.new_customer.new_customer_inserted_flag == 1 )
     {
       return 7;
     }
#endif
#if 1
     if(*data_ptr == 2)
     {
         memcpy((uint8_t*)&replace_charge_counter_max,&data_ptr[1],sizeof(replace_charge_counter_max));
#ifdef MTR_SINGLE_PH
        replace_charge_counter_max = SwapEndian16(replace_charge_counter_max);
#endif
     }
#endif
     
     if(*data_ptr == CHRG_MNY)
     {
       uint16_t chrg_num=0;
       memcpy((uint8_t*)&chrg_num , &data_ptr[1] ,sizeof(chrg_num));
       if(chrg_num==0)
         get_initial_chrg=1;
     }
     if(*data_ptr == METER_RST)
     {
       get_general_zero_flag=1;
     }
     if(*data_ptr ==  ID_CFG_SET)
     {
       memcpy((uint8_t*)&rcv_customer_id,&data_ptr[1],sizeof(rcv_customer_id));
#ifdef MTR_SINGLE_PH
       rcv_customer_id=SwapEndian32(rcv_customer_id);
#endif
     }
      fnshd_cmd_data_size = size_of_comm[*data_ptr];
    }
    elapsed_cmds_data_size += (fnshd_cmd_data_size + 1);
    data_ptr += (fnshd_cmd_data_size + 1);
#ifdef DEBUG_HANG_LOOPING
   uart_debug_byte ('q',0);
#endif
  }
  if(elapsed_cmds_data_size != data_size)
  {
    return FALSE;
  }
  else
  {
 //  if (ctrl_sys_var_second_optional_feature.test_interface==TRF_PYMT_INT_OPTICAL || ctrl_sys_var_second_optional_feature.test_interface==TRF_PYMT_INT_GPRS)
     {
    if(get_general_zero_flag==1 && get_initial_chrg==1)  //for customer
    {
      customer_flag = 1;
      if((rcv_customer_id == 0)|| ctrl_cfg.id.Customer != 0)
        return 74;
    }
    if(customer_flag==0 && get_general_zero_flag==1 && get_initial_chrg==0) //for zero
    {
      if((rcv_customer_id != 0))
        return 74;
    } 
     }
    return TRUE;
     }

}

void reset_all_meter_data_cmd(void)
{
  uint8_t tmp_block, tmp_card_type;
  
  tmp_block = ctrl_sys_new_var.block_state;
  tmp_card_type = ctrl_sys_new_var.ctrl_select_which_card_type;
  /*! - Case: Meter Reset.*/
  if (comm_disable_zero_command == 1)
  {
    comm_disable_zero_command = 0;
  }
  /*! - Case: Meter Reset.*/
/* Tariff/payment reset removed: the module is not built for the DRU. */
  /*! - Reset control data.*/
  ctrl_cmd(CTRL_RST);
  //ctrl_reset_cfg(1);
  /*! - Reset metering data.*/
  /* mtr_cmd(MTR_RST); */  /* metering not built for the DRU */
  /* mtr_cmd(MTR_CFG_RST); */  /* metering not built for the DRU */
  /*! - Reset GPRS configuration.*/
#ifdef GPRS_FEATURE
    gprs_reset_config();
#endif
#ifdef RF_LINK_FEATURE
  RF_Link_event(RF_LINK_EVE_PWR_DWN);
#endif
#ifndef ANSI_STACK
#if defined(MTR_INDIRECT)
  if (customer_flag == 0)
  {
    display_set_default_screen();
  }
#endif
#endif                          // ANSI_STACK
#ifdef RECORD_NEW_CUSTOMER_DATE // TODO
  //      memset((uint8_t*)&ctrl_sys_new_var.new_customer, 0, sizeof(ctrl_sys_new_var.new_customer));
  ctrl_sys_new_var.new_customer.crc = calc_crc((uint8_t *)&ctrl_sys_new_var.new_customer, sizeof(ctrl_sys_new_var.new_customer) - 1, CRC);
#endif
  ctrl_sys_new_var.block_state = tmp_block;
  ctrl_sys_new_var.ctrl_select_which_card_type = tmp_card_type;

 // Flash_app_Set_Default_All_Records_ID();

#ifdef ASHNTTI_PROJECT
  display_load_default_data();
#endif
#ifdef STS_ENABLE
  if (customer_flag == 0)
  {
    STS_mtr_reset();
  }
#endif
#ifdef ANSI_STACK
  ANSI_DISPLAY_cfg_RST();
#endif
}
