#ifndef IEC_SLAVE_H
#define IEC_SLAVE_H
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                               I E C     S L A V E

                              H E A D E R     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
 * @file iec62056_21_slave.h
 *
 * @brief This file contains definitions related to the IEC 62056-21 protocol.
 *
 * @details This file contains the required definitions, types for packets and
 * communication sequences of IEC62056-21 protocol.
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
 *   <td> 31/07/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Optimize and add comments. </td>
 * </tr>
 * <tr>
 *   <td> 30/12/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Wait 0.5 second after changing baud rate and move scratch card status to instantaneous data and remove scratch card mode.</td>
 * </tr>
 * <tr>
 *   <td> 30/07/12 </td>
 *   <td> islam.elshahat </td>
 *   <td> Created </td>
 * </tr>
 * </table>\n
 */
/*!
 * @defgroup IEC_SLAVE IEC 62056-21 Slave
 * @ingroup COMM_INTFC
 * IEC 62056-21 protocol is responsible for:
 *   -# Mange communication between meter and device connected to S_UART2
    (may be HHU through optical communication or the user interface through RS 485).
 *   -# Transfer data between meter and device connected to S_UART2.
 *   -# Receive meter configuration and commands from device connected to S_UART2.
 *
 * <b>References</b>\n
 * -  Manual of IEC 62056-21.
 * -  Design document of the protocol.
 * -  Reference document of the standard.
 *
 * @brief File system is responsible for saving and loading in storage media for
 * different modules and all logging.
 */
/*!
 * @defgroup IEC_CORE Core APIs
 * @ingroup IEC_SLAVE
 * @brief IEC protocol core functions.
 */
/*!
 * @defgroup IEC_EVENT Event APIs
 * @ingroup IEC_SLAVE
 * @brief IEC protocol event interface.
 */
/*!
 * @defgroup IEC_READ Read System Variables APIs
 * @ingroup IEC_SLAVE
 * @brief IEC protocol read system variables interface.
 */
/*==========================================================================================

                                 INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "config.h"
#include "UART_Config.h"
/*==========================================================================================

                          DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/

/*------------------------------------------------------------------------------------------
                                  Constants and Definitions
------------------------------------------------------------------------------------------*/
/*! Meter ID length.*/
#define IEC_METER_ID_LENGTH 9
/*! Identification packet length.*/
#ifdef IEC_62056_21_SLAVE
#define IDNT_PCKT_LENGTH (IEC_METER_ID_LENGTH + 7)
#else
#define IDNT_PCKT_LENGTH (IEC_METER_ID_LENGTH + 8)
#endif
/*! RS 485 timeout by minutes. After timeout RS485 icon is removed from LCD.*/
#ifdef GROUP_METER_FEATURE
#define RS_485_TIMEOUT 5
#else
#define RS_485_TIMEOUT 20 
#endif
#ifdef TRF_BP_HISTORY
/*! Maximum number of BPH records.*/
#define BPH_REC_MAX 30
#endif
#ifdef CTRL_EVNT_LOG
/*! Maximum number of event records.*/
#define EVNT_REC_MAX 25
#endif
#ifdef PYMT_MONY_TRANS
/*! Maximum number of money records.*/
#define MNY_REC_MAX 20
#endif
#ifdef MTR_LOAD_PROFILE
/*! Maximum number of profile records.*/
#define PRFILE_REC_MAX 35
#endif
#ifdef CTRL_CFG_METER_LOG
/*! Maximum number of configure meter records.*/
#define CFG_MTR_REC_MAX 30
#endif
/*! Period to wait master to change the baud rate in 40msec.*/
#define WAIT_BAUD_RATE_PERIOD 25     // 25 new value for test - 12 old value /*edit by A_K*/ because rtc is fake//25 in 1ph

#define NUL  0x00
#define SOH  0x01
#define STX  0x02
#define ETX  0x03
#define EOT  0x04
#define ACK  0x06
#define LF   0x0A
#define CR   0x0D
#define NAK  0x15
#define ESC  0x1B

#define FRAME_START_BYTE    0x40
#define MAX_FRAME_LENGTH    33
#define MAX_TEST_BUFF_SIZE  254
#define FRAME_END_BYTE      0xC0

#ifdef optical_DMA_Enable
#define IEC_DMA_ENABLE() uart_DMA_enable(OPTICAL_UART_PORT_NUMBER, 2, iec_62056_21_isr_buffer, 4)
#else
#define IEC_DMA_ENABLE() // nothing
#endif

#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
/*! - Enable listening to both RS485 and Optical.*/
#define IEC_62056_21_LISTEN_BOTH()    {S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);iec62056_21_lcd_icon_flg &= (~IEC_OPTICAL_RS);}
/*! - Enable listening to RS485 and disable listening to Optical.*/
#define IEC_62056_21_LISTEN_RS_SEND_OPT()    S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN)
/*! - Enable listening to optical and disable listening to RS485.*/
#define IEC_62056_21_LISTEN_OPT_SEND_RS()    S_GPIO_Set_Bits(RS485_PORT_OUT, RS485_PIN)
#else
/*! - Enable listening to both RS485 and Optical.*/
#define IEC_62056_21_LISTEN_BOTH()    {S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);iec62056_21_lcd_icon_flg &= (~IEC_OPTICAL_RS);}
/*! - Enable listening to RS485 and disable listening to Optical.*/
#define IEC_62056_21_LISTEN_RS_SEND_OPT()    S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN)
/*! - Enable listening to optical and disable listening to RS485.*/
#define IEC_62056_21_LISTEN_OPT_SEND_RS()    S_GPIO_Set_Bits(RS485_PORT_OUT, RS485_PIN)
#endif
/*! Clear receiving data from UART flag.*/
#define IEC_62056_21_CLR_INT_FLG()
/*! Read received character.*/
#define IEC_62056_21_GET_RX_CHAR() ((iec62056_21_lcd_icon_flg & IEC_OPTICAL) ? OPTRXBUF : RSRXBUF)

#ifdef GPRS_DEBUG
#define IEC_62056_21_UART_INIT(parity, baud_rate)
#else
#ifdef CURRNET_DEBUG 
#define IEC_62056_21_UART_INIT(parity, baud_rate)
#else
#ifdef GROUP_METER_FEATURE
    #ifdef MTR_THREE_PH
#define IEC_62056_21_UART_INIT(parity, baud_rate)  {if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) { S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);} else if(iec62056_21_lcd_icon_flg & IEC_RS) { S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,'4',UART_WORDLEN_8B,Dlms_NoParity); } else if((iec62056_21_lcd_icon_flg & IEC_OPTICAL_RS)==0){ S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,'4',UART_WORDLEN_8B,Dlms_NoParity);S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);}} IEC_DMA_ENABLE(); 
    #else
#define IEC_62056_21_UART_INIT(parity, baud_rate)  {if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) { S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,'4',UART_WORDLEN_8B,Dlms_NoParity);} else if(iec62056_21_lcd_icon_flg & IEC_RS) { S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,'4',UART_WORDLEN_8B,Dlms_NoParity); } else if((iec62056_21_lcd_icon_flg & IEC_OPTICAL_RS)==0){ S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,'4',UART_WORDLEN_8B,Dlms_NoParity);S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,'4',UART_WORDLEN_8B,Dlms_NoParity);}} IEC_DMA_ENABLE(); 
    #endif
#else
#ifdef DLMS_UI
#define IEC_62056_21_UART_INIT(parity, baud_rate)  {if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) { S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);} else if(iec62056_21_lcd_icon_flg & IEC_RS) { S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity); } else if((iec62056_21_lcd_icon_flg & IEC_OPTICAL_RS)==0){/* S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);*/S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);}} IEC_DMA_ENABLE(); 
#else
#define IEC_62056_21_UART_INIT(parity, baud_rate)  {if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) { S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);} else if(iec62056_21_lcd_icon_flg & IEC_RS) { S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity); } else if((iec62056_21_lcd_icon_flg & IEC_OPTICAL_RS)==0){ S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity); S_UART_INIT((UART_TypeDef*)OPTICAL_UART_PORT_NUMBER,baud_rate,UART_WORDLEN_8B,parity);}} IEC_DMA_ENABLE();                                                 
#endif
#endif
#endif
#endif
/*! Send a character.*/
#define IEC_62056_21_SEND_CHAR(value)  {if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) S_UART_SEND_BYTE(OPTICAL_UART_PORT_NUMBER, value);}
/*! Send a frame. Returns FALSE if power is down during sending.*/
#define IEC_62056_21_SEND_FRAME(data_ptr, length) {if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) {Delay_ms(30); S_UART_SEND_FRAME(OPTICAL_UART_PORT_NUMBER ,data_ptr,length);}}//{S_UART_DISABLE_TX(UART_PORT_NUMBER);vDelay_SW(12000); if(iec62056_21_lcd_icon_flg & IEC_OPTICAL) S_UART_SEND_FRAME(UART_PORT_NUMBER ,data_ptr,length);S_UART_ENABLE_RX(UART_PORT_NUMBER);}
/*! Get active energy low value.*/
#define IEC_62056_21_GET_ACTV_ENRGY_LOW() MTR_GET_TOT_ACT_EN_LOW()
/*! Get active energy high value.*/
#define IEC_62056_21_GET_ACTV_ENRGY_HIGH() MTR_GET_TOT_ACT_EN_HIGH()
/*! Get reactive energy low value.*/
#define IEC_62056_21_GET_RACTV_ENRGY_LOW() MTR_GET_TOT_REACT_EN_LOW()
/*! Get reactive energy high value.*/
#define IEC_62056_21_GET_RACTV_ENRGY_HIGH() MTR_GET_TOT_REACT_EN_HIGH()
/*! Get apparent energy low value.*/
#define IEC_62056_21_GET_APRNT_ENRGY_LOW() MTR_GET_TOT_APP_EN_LOW()
/*! Get apparent energy high value.*/
#define IEC_62056_21_GET_APRNT_ENRGY_HIGH() MTR_GET_TOT_APP_EN_HIGH()

/*! Update LCD.*/
#define IEC_62056_21_UPDATE_LCD() display_update()
/*! Check if power is going down.*/
#define IEC_62056_21_IS_PWR_DWN() 0/*CTRL_PWR_DN_STAT*/
/*! Get meter ID as 4 byte integer.*/
#define IEC_62056_21_GET_CUSTOMER_ID() 100
/*! Get department ID as 2 byte integer.*/
#define IEC_62056_21_GET_DEPART_ID() CTRL_DPRT_ID
/*! Write new data list beginning from adrs and return size of data written.*/
#define IEC_62056_21_WRITE_MAIN_DATA_LIST(adrs) comm_read_raw_data(adrs, COMM_MAIN_DATA)
/*! Save a configure meter record.*/
#define IEC_62056_21_SAVE_CFG_METER_REC(op_id,INTERFACE) ctrl_save_cfg_meter_rec(op_id, INTERFACE)
/*! Handle a received command packet and return TRUE or FALSE according to achievement.*/
#define IEC_62056_21_HANDLE_CMD(adrs, size)  comm_handle_rx_cmd(adrs, size)
/*! Handle a received Boot Loader packet and return TRUE or FALSE according to achievement.*/
#define IEC_62056_21_HANDLE_BOOTLOADER_DATA(adrs, size) comm_handle_rx_bootloader_data(adrs, size)
/*! Fill communication buffer with tariff data and returns size of data written.*/
#define IEC_62056_21_GET_TRF_DATA(adrs) comm_read_raw_data(adrs, COMM_TRF_DATA)
/*! Fill communication buffer with metering data and returns size of data written.*/
#define IEC_62056_21_GET_METERING_DATA(adrs) comm_read_raw_data(adrs, COMM_METERING_DATA)
/*! Fill communication buffer with control data and returns size of data written.*/
#define IEC_62056_21_GET_CTRL_DATA(adrs) comm_read_raw_data(adrs, COMM_CTRL_DATA)
/*! Fill communication buffer with instantaneous data and returns size of data written.*/
#define IEC_62056_21_GET_INST_DATA(adrs) comm_read_raw_data(adrs, COMM_METERING_INST_DATA)
/*! Fill communication buffer with instantaneous data and returns size of data written.*/
#define IEC_62056_21_GET_TRF_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_TRF_CFG)
/*! Fill communication buffer with instantaneous data and returns size of data written.*/
#define IEC_62056_21_GET_PYMT_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_PYMT_CFG)
/*! Fill communication buffer with instantaneous data and returns size of data written.*/
#define IEC_62056_21_GET_TAX_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_TAX_CFG)
/*! Get multiple BPH records and return size of data written.*/
#define IEC_62056_21_GET_BPH_RECS(from, num, adrs) trf_pymt_get_bph_mlti_rec(adrs, from, num)
/*! Get multiple event records and return size of data written.*/
#define IEC_62056_21_GET_EVNT_RECS(from, num, adrs) ctrl_get_evnt_mlti_rec(from, num, adrs)
/*! Get multiple event records and return size of data written.*/
#define IEC_62056_21_GET_MNY_RECS(from, num, adrs) trf_pymt_get_mlti_mny_rec(adrs, from, num)
/*! Get multiple event records and return size of data written.*/
#define IEC_62056_21_GET_PRFL_RECS(from, num, adrs) mtr_get_lp(from,adrs,num)
/*! Get multiple event records and return size of data written.*/
#define IEC_62056_21_GET_CFG_RECS(from, num, adrs) ctrl_get_cfg_meter_mlti_rec(from, num, adrs)
/*! Get current day from RTC.*/
#define IEC_62056_21_DAY CTRL_READ_DAY()
/*! Get current month from RTC.*/
#define IEC_62056_21_MONTH CTRL_READ_MONTH()
/*! Get current year from RTC.*/
#define IEC_62056_21_YEAR CTRL_READ_YEAR()
/*! Increment data read counter.*/
#define IEC_62056_21_EVE_DATA_READ() ctrl_event(CTRL_METER_DATA_READ)
/*! Save data in internal eeprom.*/
#define IEC_62056_21_INT_DATA_WRITE(eeprom_adrs, data_adrs, size) file_sys_write_int(eeprom_adrs, data_adrs, size)
/*! Save critical data in external eeprom.*/
#define IEC_62056_21_SAVE_METER_DATA() ctrl_event(CTRL_SAVE_CRITICAL_DATA)

// ADDED BY AMR
#define IEC_62056_21_GET_VACATION_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_VACATION_CFG_DATA)
#define IEC_62056_21_GET_FRIENDLY_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_FRIENDLY_CFG_DATA)
#define IEC_62056_21_GET_PYMENT_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_PYMENT_CFG_DATA)
#define IEC_62056_21_GET_TARIFF_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_TARIFF_CFG_DATA)
#define IEC_62056_21_GET_TAXES_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_TAX_CFG_DATA)
#define IEC_62056_21_GET_BP_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_BP_CFG_DATA)
#define IEC_62056_21_GET_CONTROL_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_CONTROL_CFG_DATA)
#define IEC_62056_21_GET_MAXIMUM_DEMAND_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_MAXIMUM_DEMAND_CFG_DATA)
#define IEC_62056_21_GET_LIMITER_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_LIMITER_CFG_DATA)
#define IEC_62056_21_GET_TAMPERS_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_TAMPERS_CFG_DATA)
#define IEC_62056_21_GET_OPERATING_POINT_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_OPERATING_POINT_CFG_DATA)
#define IEC_62056_21_GET_ID_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_ID_CFG_DATA)
#define IEC_62056_21_GET_ACTIONS_ALARMS_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_ACTIONS_ALARMS_CFG_DATA)
#define IEC_62056_21_GET_TIME_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_TIME_CFG_DATA)
#define IEC_62056_21_GET_DATE_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_DATE_CFG_DATA)
#define IEC_62056_21_GET_BATTERY_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_BATTERY_CFG_DATA)
#define IEC_62056_21_GET_MAGNETIC_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_MAGNETIC_CFG_DATA)
#define IEC_62056_21_GET_RFID_UNIQUE_ID_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_RFID_UNIQUE_ID_CFG_DATA)
#define IEC_62056_21_GET_GPRS_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_GPRS_CFG_DATA)
#define IEC_62056_21_GET_OVERLOAD_ALARMS_CFG_DATA(adrs) comm_read_raw_data(adrs, COMM_OVERLOAD_ALARMS_CFG_DATA)
#define IEC_62056_21_GET_NEW_CTRL_DATA(adrs) comm_read_raw_data(adrs, COMM_RETURN_NEW_CTRL_DATA)
#define IEC_62056_21_GET_RETURN_CORRUPTION(adrs) comm_read_raw_data(adrs, COMM_RETURN_CORRUPTION)
#define IEC_62056_21_GET_EEPROM_PAGE(adrs) comm_read_raw_data(adrs, (uint8_t)*(adrs))



typedef enum
{
  WAIT_PCKT_STRT = (uint8_t)0, /*!< Waiting a packet start character.*/
  WAIT_CR = (uint8_t)1, /*!< Waiting CR charachter.*/
  WAIT_LF = (uint8_t)2, /*!< Waiting LF charachter.*/
  DATA_COLLECT = (uint8_t)3, /*!< Receive data block.*/
  WAIT_EXCLAMATION_MARK = (uint8_t)4, /*!< Waiting ! charachter.*/
  WAIT_ETX = (uint8_t)5, /*!< Waiting ETX charachter.*/
  WAIT_BCC = (uint8_t)6, /*!< Waiting BCC charachter.*/
  rcv_scan_mode = (uint8_t)7,
  WAIT_3_BYTES_ACK = (uint8_t)8,
  WAIT_3F_BYTE_IN_REQ = (uint8_t)9,
  WAIT_METER_ID = (uint8_t)10
} rx_char_state_t;

/*!
 * @par Description:
 *   This enumerator contains options of the mask applied on received charachter.
 */
typedef enum
{
  STANDARD_MASK = (uint8_t)0x7F,
  SPECIFIC_MASK = (uint8_t)0xFF,
} char_mask_t;

/*!
 * @par Description:
 *   This enumerator contains options of a received packet and will be the return
 * of #check_pckt_valid.
 */
typedef enum
{
  INVALID_PCKT = (uint8_t)0, /*!< Invalid packet type.*/
  RQST_PCKT = (uint8_t)1, /*!< Request packet.*/
  ACK_PCKT = (uint8_t)2, /*!< Acknowledgment packet.*/
  DATA_PCKT = (uint8_t)3, /*!< Data packet.*/
  DATA_END_COMM_PCKT = (uint8_t)4, /*!< Data packet - End communication type.*/
  DATA_RQST_PCKT = (uint8_t)5, /*!< Data packet - Data request packet.*/
  DATA_CMD_PCKT = (uint8_t)6, /*!< Data packet - Command type.*/
  DATA_AUTHNC_PCKT = (uint8_t)7, /*!< Data packet - Authentication type.*/
  DATA_BOOTLOADER_PCKT= (uint8_t)8,/*!< Data packet - Boot Loader type.*/
  SCAN_PCKT=9,
  SAVE_READOUT_DATA_PCKT=10,
  LAST_ACK_BOOTLOADER_PCKT=11,
}pckt_type_t;

/*!
 * @par Description:
 *   This enumerator contains options of communication modes.
 */
typedef enum
{
  READOUT_STND_MODE = (uint8_t)'0', /*!< Readout data - standard mode.*/
  READ_DATA_SPCFC_MODE = (uint8_t)'6', /*!< Read data - manufacturer specific mode.*/
  CFG_MODE = (uint8_t)'7', /*!< Configure mode.*/
  BOOTLOADER_DATA_MODE = (uint8_t)'8', /*!< Bootloader data mode.*/
  BOOTLOADER_MODE = (uint8_t)'9', /*!< Bootloader mode.*/  
  TOOLING_MODE = (uint8_t)'A', /*!< Bootloader mode.*/  
  WRITE_EEPROM_MODE=0x6A,
} comm_modes_t;

/*!
 * @par Description:
 *   This enumerator contains options of type character received in data packets.
 *
 * @note
 * This enum doesn't contain data request type as it is range not a certain value.
 */
typedef enum
{
  DATA_END_COMM_TYPE = (uint8_t)0, /*!< End communication type charchter.*/
  DATA_CMD_TYPE = (uint8_t)1, /*!< Command type charchter.*/
  DATA_AUTHNC_TYPE = (uint8_t)2, /*!< Authentication type charchter.*/
  DATA_BOOTLOADER_TYPE = (uint8_t)4 /*!< Boot Loader type character.*/
} data_pckt_type_t;

/*!
 * @par Description:
 *   This enumerator contains general protocol states. Check state machine figure.
 */
typedef enum
{
  IDLE = (uint8_t)0, /*!< Idle state (default state).*/
  WAIT_ACK = (uint8_t)1, /*!< Waiting Acknowledgment state (Identifed state).*/
  WAIT_CMD = (uint8_t)2, /*!< Waiting command state.*/
  WAIT_DATA_RQST = (uint8_t)3, /*!< Waiting data request state.*/
  WAIT_AUTHNC = (uint8_t)4, /*!< Waiting authentication state.*/
  CHANGE_BAUD_RATE = (uint8_t)5, /*!< Waiting for the master to change the baud rate.*/
  WAIT_BOOTLOADER_DATA = (uint8_t)6, /*!< Waiting Boot loading state.*/
  WAIT_IDENT_PCKT_DONE = (uint8_t)7,
  SEND_BOOTLOADER_DATA = (uint8_t)8,
  SCAN = (uint8_t)9,
  COLLECT_READOUT_DATA=10
} protocol_states_t;

/*!
 * @par Description:
 *   This enumerator contains options of data request packet types.
 */
typedef enum
{
  READOUT_DATA_STND_SEND = (uint8_t)0, /*!< Readout data of standard mode.*/
  MAIN_METER_DATA = (uint8_t)30, /*!< Main meter data.*/
  MIN_RAW_DATA = (uint8_t)31, /*!< Lower boundary of data request type.*/
  TRF_DATA = (uint8_t)31, /*!< Request tariff and payment data.*/
  METERING_DATA = (uint8_t)32, /*!< Request metering data.*/
  CTRL_DATA = (uint8_t)33, /*!< Request control data.*/
  TRF_CFG_DATA = (uint8_t)36, /*!< Request tariff configuration data.*/
  PYMT_CFG_DATA = (uint8_t)37, /*!< Request payment configuration data.*/
  TAX_CFG_DATA = (uint8_t)38, /*!< Request tax configuration data.*/
  INST_DATA = (uint8_t)39, /*!< Request instantaneous data.*/
  
   // Added by AMR
  VACATION_CFG_GET = (uint8_t)40,
  FRIENDLY_CFG_GET = (uint8_t)41,
  PYMENT_CFG_GET   = (uint8_t)42,
  TARIFF_CFG_GET   = (uint8_t)43,
  TAX_CFG_GET      = (uint8_t)44,
  BP_CFG_GET       = (uint8_t)45,
  CONTROL_CFG_GET  = (uint8_t)46,
  MAXIMUM_DEMAND_CFG_GET  = (uint8_t)47,
  LIMITER_CFG_GET         = (uint8_t)48,
  TAMPERS_CFG_GET         = (uint8_t)49,
  OPERATING_POINT_CFG_GET = (uint8_t)50,
  ID_CFG_GET              = (uint8_t)51,
  ACTIONS_ALARMS_CFG_GET  = (uint8_t)52,
  TIME_CFG_GET            = (uint8_t)53,
  DATE_CFG_GET            = (uint8_t)54,
  BATTERY_CFG_GET         = (uint8_t)55,
  MAGNETIC_CFG_GET        = (uint8_t)56,
  RFID_UNIQUE_ID_CFG_GET  = (uint8_t)57,
  GPRS_CFG_GET            = (uint8_t)58,
  OVERLOAD_ALARMS_CFG_GET = (uint8_t)59,
  NEW_CTRL_DATA_GET       = (uint8_t)60,
  COMM_RETURN_CORRUPTION_GET  = (uint8_t)61,
  COMM_RETURN_EEPROM_PAGE  = (uint8_t)62,
  MAX_RAW_DATA = (uint8_t)62, /*!< Upper boundary of data request type.*/
  
  MIN_REC_DATA = (uint8_t)10, /*!< Lower boundary of records request type.*/
  BPH_REC = (uint8_t)10, /*!< Request BPH records.*/
  EVNT_REC = (uint8_t)11, /*!< Request event records.*/
  MNY_REC = (uint8_t)12, /*!< Request money transaction records.*/
  PRFL_REC = (uint8_t)13, /*!< Request profile records.*/
  CFG_REC = (uint8_t)14, /*!< Request configure meter data.*/
   
  MAX_REC_DATA = (uint8_t)14, /*!< Upper boundary of records request type.*/
} data_rqst_type_t;

/*!
 * @par Description:
 *   This enumerator contains options of data responses.
 */
typedef enum
{
  REJECTED = (uint8_t)'R', /*!< Reject charachter.*/
  ACKNLDG = (uint8_t)'A', /*!< Acknowledgment character.*/
} data_response_t;

/*!
 * @par Description:
 *   This enumerator contains options of energy types used as index of
 * standard mode readout data.
 */
typedef enum
{
  ACTV_ENRG = (uint8_t)0, /*!< Active energy.*/
  RCTV_ENRG = (uint8_t)1, /*!< Reactive energy.*/
  APRNT_ENRG = (uint8_t)2, /*!< Apparent energy.*/
  MAX_NUM_ENRG_TYPE = (uint8_t)3, /*!< Number of energy types used.*/
} energy_type_t;


/*!
 * Request data packet format used to detect from and number of records.
 *
 * @brief Structure containg request data packet format.
 */
__packed typedef struct
{
  uint8_t start_byte; /*!< Start byte (STX).*/
  uint16_t blck_size; /*!< Data block size in Bytes.*/
  uint8_t type; /*!< Type of data packet.*/
  uint16_t from; /*!< Record number index to read from.*/
  uint16_t num; /*!< Number of required records.*/
} data_rqst_pckt_t;

/*!
 * Obis code and units used to send data with data reqdout standard mode.
 *
 * @brief Structure containg readout mode data.
 */
__packed typedef struct
{
  uint8_t obis_code[7]; /*!< OBIS code of data sent in readout mode.*/
  uint8_t unit[10]; /*!< Unit od data sent.*/
  uint8_t unit_size; /*!< Size of unit in bytes.*/
} readout_mode_object_t;


/*------------------------------------------------------------------------------------------
                                         Data Types
------------------------------------------------------------------------------------------*/
#if defined (IEC_62056_21_SLAVE) || defined (HDLC_PROTOCOL) || defined (DLMS_ENABLED)
/*!
 * @ingroup IEC_EVENT
 * @par Description:
 *   This enumerator contains events types.
 */
typedef enum
{
  IEC_62056_21_PWR_UP = (uint8_t)0, /*!< Power up event.*/
  IEC_62056_21_NEW_SEC = (uint8_t)1, /*!< New second event.*/
  IEC_62056_21_NEW_TIMER_TICK = (uint8_t)2 /*!< New timer tick event.*/
} iec_62056_21_event_type_t;

/*!
 * @par Description:
 *   This enumerator contains options of comunication.
 *
 */
typedef enum
{
  IEC_OPTICAL = (uint8_t)0x01, /*!< Active energy.*/
  IEC_RS = (uint8_t)0x02, /*!< Reactive energy.*/
  IEC_OPTICAL_RS = (uint8_t)0x03, /*!< Apparent energy.*/
} comm_type_t;
#endif

/*------------------------------------------------------------------------------------------
                                     External Variables
------------------------------------------------------------------------------------------*/
#ifdef IEC_62056_21_SLAVE
/*!< LCD icons connection flag (optical, RS485).
 * Bit 0 for optical icon and Bit 1 for RS485 icon.*/
extern uint8_t iec_comm_buffer[IEC_BUFFER_SIZE]; 
extern uint8_t iec62056_21_lcd_icon_flg;
extern uint8_t IEC_DRU_flag;
extern uint8_t optical_sec_counter;
extern uint8_t customer_flag;
extern uint8_t g_jig_start;
extern uint16_t pckt_size;
extern uint8_t pckt_type;
extern uint8_t state;
extern uint16_t rs485_counter;
extern uint32_t iec62056_21_wait_baud_rate_counter;
#endif
/*------------------------------------------------------------------------------------------
                                          Macros
------------------------------------------------------------------------------------------*/



#if defined (IEC_62056_21_SLAVE) || defined (HDLC_PROTOCOL)
/*!
 * @ingroup IEC_READ
 *
 * Send optical connection status. TRUE if connected.
 */
#define IEC_62056_21_SLAVE_OPTCL_CONCT_FLG (iec62056_21_lcd_icon_flg&IEC_OPTICAL)
/*!
 * @ingroup IEC_READ
 *
 * Send RS485 connection status. TRUE if connected.
 */
#define IEC_62056_21_SLAVE_RS485_CONCT_FLG (iec62056_21_lcd_icon_flg&IEC_RS)
#endif

/*==========================================================================================

                                   FUNCTION DECLARATIONS

==========================================================================================*/
#ifdef IEC_62056_21_SLAVE
/*!
 * @ingroup IEC_EVENT
 *
 * @brief Buffer the received characters to form packets.
 *
 * @par Description:
 * Detect different packets start and end and set a flag when a packet
 * is received descriped by the following state machine: \image html Rx_Char.jpg
 *
 * @return
 * - None.
 */
void iec_62056_21_rx_char_isr(uint8_t rx_byte);

/*!
 * @ingroup IEC_EVENT
 *
 * @brief Receive new events.
 *
 * @par Description:
 * When an event happens this function does the proper action.
 *
 * @param [in] type Event type one of #iec_62056_21_event_type_t elements.
 *
 * @return
 * - None.
 */
void iec_62056_21_slave_event(uint8_t type);
#endif

/*!
 * @ingroup IEC_CORE
 *
 * @brief Task of communication protocol.
 *
 * @par Description:
 * Check new packets received and reply according to the state machine in the
 * figure. \image html "protocol_state_machine.jpg"
 *
 * @return
 * - None.
 */
void iec_62056_21_slave_task(void);

/*!
 * @brief This function is used to send any frame to optical or RS485.
 *
 * @par Description:
 * Control both channels of optical and RS485 to transmit data and listen
 * to future received packet.
 *
 * @return
 * - None.
 *
 * @note: In case of optical, the communication is full duplex so when we receive
 * request without meter ID, hardware is configured to receive from optical only.
 * In case of RS485, the communication is half duplex so before transmission,
 * code should configure channel for transmission. After transmission ends, the
 * channel should configured again to receive packets from RS485.
 */
void send_frame(uint8_t *data_ptr, uint16_t length);
uint8_t iec62056_21_slave_check_is_idle (void);
void reset_state_machine(void);
void reset_buffer(void);
void send_main_data(uint8_t next_state);
void iec_62056_21_sub_task (void);
uint8_t send_data_pckt(uint8_t type);
void send_data_response_pckt(uint8_t response);
void iec62056_21_slave_goto_bootloader(void);
void handle_scan_mode_isr(uint8_t rx_byte);
#endif
