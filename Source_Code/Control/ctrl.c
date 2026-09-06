/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

C O N T R O L

S O U R C E     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
* @file ctrl.c
*
* @brief This file contains the implementation of control system.
*
* @details Control system in responsible to
*  -# Manage tampers and take actions accordining to these tampers.
*  -# Manage event and configure meter logs.
*  -# Manage board components like switches, LEDs, relay, buzzer, battery and
*     magnetic sensor.
*  -# Detect power down and power up and handle tasks with each case.
*
* <b>References</b>\n
* -  Design document of control system.
* -  Communication data sheet.
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
*   <td> 14/04/13 </td>
*   <td> islam.elshahat </td>
*   <td> Integrate with RF Link. </td>
* </tr>
* <tr>
*   <td> 27/03/13 </td>
*   <td> islam.elshahat </td>
*   <td> Change low power switches handling to avoid halt while SW_STAT pin is low and add commands to force relay open. </td>
* </tr>
* <tr>
*   <td> 18/03/13 </td>
*   <td> islam.elshahat </td>
*   <td> Solve hint by relay problem with low credit. </td>
* </tr>
* <tr>
*   <td> 07/03/13 </td>
*   <td> islam.elshahat </td>
*   <td> Solve switches freezing problem in low power mode. </td>
* </tr>
* <tr>
*   <td> 28/02/13 </td>
*   <td> islam.elshahat </td>
*   <td> Solve saving in EEPROM when power goes down and hang problem after power up. </td>
* </tr>
* <tr>
*   <td> 25/02/13 </td>
*   <td> islam.elshahat </td>
*   <td> Handle excution time of timer and RTC interrupt delays, Solve instant open of cover and terminal in low power mode. </td>
* </tr>
* <tr>
*   <td> 20/02/13 </td>
*   <td> islam.elshahat </td>
*   <td> Change scanning switches to be 4 switches every 40 msec. </td>
* </tr>
* <tr>
*   <td> 13/02/13 </td>
*   <td> islam.elshahat </td>
*   <td> Solve bug of buzzer continous ON. </td>
* </tr>
* <tr>
*   <td> 10/02/13 </td>
*   <td> islam.elshahat </td>
*   <td> Modify low power mode handling to decrease current consumption in power down.
*        Use active halt with two wakeup one for switch debouncing and second for LCD disabling.
*        Solve buzzer bug with power fail, Modify error screen to display latched tampers and alarms also. </td>
* </tr>
* <tr>
*   <td> 15/01/13 </td>
*   <td> islam.elshahat </td>
*   <td> Solve bug of reset event and meter configure logs. </td>
* </tr>
* <tr>
*   <td> 13/01/13 </td>
*   <td> islam.elshahat </td>
*   <td> Make overload sequence configurable. </td>
* </tr>
* <tr>
*   <td> 10/01/13 </td>
*   <td> islam.elshahat </td>
*   <td> Don't reset no credit status. </td>
* </tr>
* <tr>
*   <td> 09/01/13 </td>
*   <td> islam.elshahat </td>
*   <td> Solve bug of cover and terminal detection in power down and self test of switches. </td>
* </tr>
* <tr>
*   <td> 30/12/12 </td>
*   <td> islam.elshahat </td>
*   <td> Reset STCP pin each 5 msec and only one switch is checked every 10 msec. </td>
* </tr>
* <tr>
*   <td> 13/12/12 </td>
*   <td> islam.elshahat </td>
*   <td> Move tampers counter and operator ID to system variables and solve bug of updating RTC with loaded time after power up. </td>
* </tr>
* <tr>
*   <td> 09/12/12 </td>
*   <td> islam.elshahat </td>
*   <td> Modify saving in configure meter log and add ability to reset configuration. </td>
* </tr>
* <tr>
*   <td> 29/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Solve beep bug and high current in power down problems. </td>
* </tr>
* <tr>
*   <td> 21/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Solve event log bug of meter status and move switch state to system variables. </td>
* </tr>
* <tr>
*   <td> 06/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Add detection of calibration jumper and solve bug of telling other modules with new second event. </td>
* </tr>
* <tr>
*   <td> 05/11/12 </td>
*   <td> islam.elshahat </td>
*   <td> Created. </td>
* </tr>
* </table>\n
*/

/*==========================================================================================

INCLUDE FILES FOR MODULE

==========================================================================================*/
#include "Flash.h"
#include "dependencies_layer.h"
#include "Timer_Interface.h"
#include "RTC_Interface.h"
#include "ctrl.h"
#include "string.h"
#include "iec62056_21_slave.h"
#include "comm.h"
#include "config.h"
#include "dependencies_layer.h"
/* #include "Dlms_PhyLp.h" */ /* module removed from the DRU build */
#if (MicroController == Micro_V94XX)
#include "V94XX_WDT.h"
#include "dependencies_layer.h"
#include "V94XX_EnergyMetering.h"
#else
#include "v85xx_wdt.h"
/* #include "V9360.h" */ /* module removed from the DRU build */

#endif

/* #include "lcd_HQ30774.h" */ /* module removed from the DRU build */
#include "GPIO_Interface.h"

//#include "v9261f.h"
//#include "Flash_app.h"
/* #include "STS_Timer.h" */ /* module removed from the DRU build */
/* #include "STS_Module.h" */ /* module removed from the DRU build */
#include "I2C_Interface.h"
/* #include "STS_Key_Store.h" */ /* module removed from the DRU build */
#include "dependencies_layer.h"
/* #include "G2_DM.h" */ /* module removed from the DRU build */
/* #include "keypad.h" */ /* module removed from the DRU build */
#ifdef MTR_TWO_PH
#include "dependencies_layer.h"
#endif
#ifdef STS_ENABLE
/* #include "STS_execute.h" */ /* module removed from the DRU build */
#endif
#ifdef DLMS_UI
/* #include "UI.h" */ /* module removed from the DRU build */
#endif
#include "ISO7816_Config.h"
#ifdef ANSI_STACK
/* #include "ANSI_Utility_Interface.h" */ /* module removed from the DRU build */
#ifdef ANSI_STD
/* #include "Table76.h" */ /* module removed from the DRU build */
#endif
#endif
#ifdef RF_CC1120
/* #include "cc1120_interface.h" */ /* module removed from the DRU build */
#endif
void GPRS_set_connection_reset(void);
uint8_t PWR_down_detect = 0;
/*==========================================================================================

DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables and other
items needed by this module.

==========================================================================================*/
/*------------------------------------------------------------------------------------------
Constants and Definitions
------------------------------------------------------------------------------------------*/
#ifdef DLMS_UI
uint8_t up_switch_hold_press=0;
#endif
#ifdef ANSI_STD
extern EVENT_LOG_RCD_t Table76_Data;
uint16_t event_argument = 0;
#endif
#ifdef ANSI_STACK
extern uint16_t cmd_cycles;
#endif
#define RTC_INIT_FLAG                  (uint8_t) 0xEE

#ifdef CTRL_UP_SW
#define UP_FETUR 1
#else
#define UP_FETUR 0
#endif
#ifdef CTRL_DN_SW
#define DN_FETUR 1
#else
#define DN_FETUR 0
#endif
/*! Initial value of period of continous press detection.*/
#define INIT_SCROLL_PERIOD 40
/*! Value of step to decrement from period of continous press to speed up scrolling
* while continous press in 10 msec.*/
#define STEP_DOWN_SCROLL_PERIOD 17
/*! Number of rates required for scrolling while the continous press of scrolling switch.*/
#define NUMBER_OF_SCRL_RATE 2
#if ((STEP_DOWN_SCROLL_PERIOD * NUMBER_OF_SCRL_RATE) > INIT_SCROLL_PERIOD)
#error "Invalid values for continous press periods."
#endif
/*! Value of minimum scrolling period while continous press.*/
#define MIN_SCROLL_PERIOD (uint8_t)(INIT_SCROLL_PERIOD - (NUMBER_OF_SCRL_RATE * STEP_DOWN_SCROLL_PERIOD))


#define DVCC_STATE       (AUXCTL0 & AUX0SW)
#define VCC1_STATE       (AUXCTL0 & AUX1SW)

/*! Test switch period in seconds.*/
#define TEST_SW_PERIOD 5
/*! Beep period 0 in 10ms.*/

#define BEEP_PERIOD0 5      // old 5
/*! Beep period 1 in 10ms.*/
#define BEEP_PERIOD1 100      //old 25
/*! Beep period 2 in 10ms.*/
#define BEEP_PERIOD2 37
/*! keypad beep.*/
#define BEEP_PERIOD3 5
/*! Number of switches to check with continous press checking.*/
#define CONT_PRESS_SW_NUM (uint8_t)(UP_FETUR + DN_FETUR)
/*! Number of times to repeat the check on switch to verify its state pressed or
* released. @note Mustn't exceed 8 (implementation algorithm limit)*/
#define NUMBER_SWITCH_CHECK 2
/*! Number of ones in this byte represents the number of times to check switch
to avoid bounce effect of switches.*/
#define SWITCH_CHECK_TIMES_FLAG (uint8_t)(0xFF >> (8 - NUMBER_SWITCH_CHECK))
/*! Number of checks on jumper pin to decide if it is connected or not.*/
#define NUMBER_JMPR_CHECK 2
/*! Mask to be applied on the state byte of the calibration pin.*/
#define CALB_PIN_MASK (uint8_t)(0xFF >> (8 - NUMBER_JMPR_CHECK))
/*! Minimum period in 10 msec to wait when meter wakes up from halt mode due to switch press
* interrupt before going to halt again to give chane for scanning switch function.*/
#define WAIT_PWR_DN_PERIOD (uint8_t)(NUMBER_SWITCH_CHECK * 4)
/*! Period in seconds that hint of buzzer or relay can't be stopped.*/
#define HINT_BLIND_PERIOD 2
/*! MD switch released state.*/
#define MD_SW_PRESS_STAT RESET
/*! Number of times of presistance for check to toggle battery state between low and high.*/
#define BTRY_DETECT_PRESISIT 2
/*! Battery channel connected to ADC.*/
// TODO: the below parameters from Moroco
#define BTRY_CH 0///ADC_Channel_27
/*! Magnetic sensor channel connected to ADC.*/
#define MGNT_CH 1////ADC_Channel_24
/*! Update battery and magnetic interference readings every certain period in seconds.
* @note Don't use value higher than 59 or implementation method should be changed.*/
#define BTRY_MGNT_UPDATE_PERIOD 3
/*! Period to update the relay status in minutes.*/
#define RELAY_UPDATE_PERIOD 2
/*! If the first byte in internal memory takes this value, so it contains new data.*/
#define INT_MEM_NEW_DATA_FLAG 0x66
/*! Check power fail pin to know if power is up or down.*/
#define PWR_DN  ((ctrl_flags & CTRL_PWR_DWN_MASK) || (PMU_GetPowerStatus(PMU_PWRSTS_VDCINDROP)) || (PMU_GetPowerStatus(PMU_PWRSTS_VDDALARM)) || PWR_down_detect!=0)
/*!< Number of tampers and alarms bits in meter status array.*/
#define NO_TMPRS_ALRMS_BITS (CTRL_NO_BYTE_ALRMS_TMPR * 8)

#define DATA_CRDT_MTCH (uint8_t)0
#define HRDWR_ERR_MTCH (uint8_t)1
#define BTRY_MTCH (uint8_t)2
#if (CTRL_BTRY_NON_CHRG == 1)
#define LOW_CRDT_1ST_MTCH (BTRY_MTCH + 1)
#else
#define LOW_CRDT_1ST_MTCH BTRY_MTCH
#endif
#ifdef PAYMENT_SYS
#define LOW_CRDT_2ND_MTCH (LOW_CRDT_1ST_MTCH + 1)
#else
#define LOW_CRDT_2ND_MTCH LOW_CRDT_1ST_MTCH
#endif
#ifdef PYMT_LOW_TWO_LVL
#define NO_CRDT_FRNDLY_MTCH (LOW_CRDT_2ND_MTCH + 1)
#else
#define NO_CRDT_FRNDLY_MTCH LOW_CRDT_2ND_MTCH
#endif
#ifdef PYMT_FRIENDLY
#define NO_CRDT_MTCH (NO_CRDT_FRNDLY_MTCH + 1)
#else
#define NO_CRDT_MTCH NO_CRDT_FRNDLY_MTCH
#endif
#ifdef PAYMENT_SYS
#define OVR_LOAD_MTCH (NO_CRDT_MTCH + 1)
#else
#define OVR_LOAD_MTCH NO_CRDT_MTCH
#endif
#ifdef MTR_ENABLE_LMT
#define OVR_VOLT_MTCH (OVR_LOAD_MTCH + 1)
#else
#define OVR_VOLT_MTCH OVR_LOAD_MTCH
#endif
#ifdef MTR_ENABLE_LMT
#define UNDR_VOLT_MTCH (OVR_VOLT_MTCH + 1)
#else
#define UNDR_VOLT_MTCH OVR_VOLT_MTCH
#endif
#ifdef MTR_ENABLE_LMT
#define PHS_SEQ_MTCH (UNDR_VOLT_MTCH + 1)
#else
#define PHS_SEQ_MTCH UNDR_VOLT_MTCH
#endif
#ifdef MTR_THREE_PH
#define CVR_OPN_MTCH (PHS_SEQ_MTCH + 1)
#else
#define CVR_OPN_MTCH PHS_SEQ_MTCH
#endif
#define TRMNL_OPN_MTCH (CVR_OPN_MTCH + 1)
#define MGNT_HIGH_MTCH (TRMNL_OPN_MTCH + 1)
#ifdef MTR_SINGLE_PH
#define RVRS_CRNT_MTCH (MGNT_HIGH_MTCH + 1)

#else
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
#define RVRS_CRNT_MTCH (MGNT_HIGH_MTCH + 1)
#else
#define RVRS_CRNT_MTCH MGNT_HIGH_MTCH
#endif
#endif
#ifdef MTR_RVS_TMPR
#define FLT_ENRG_MTCH (RVRS_CRNT_MTCH + 1)
#else
#define FLT_ENRG_MTCH RVRS_CRNT_MTCH
#endif
#ifdef MTR_ERTH_TMPR
#define MIS_POT_MTCH (FLT_ENRG_MTCH + 1)
#else
#define MIS_POT_MTCH FLT_ENRG_MTCH
#endif
#if defined (CTRL_GPRS_MODULE_CVR_SW) && defined (MTR_THREE_PH)
#define GPRS_MODULE_CVR_OPEN (MIS_POT_MTCH + 1)
#else
#define GPRS_MODULE_CVR_OPEN MIS_POT_MTCH
#endif
#ifdef MTR_THREE_PH
#define GEN_MTCH (MIS_POT_MTCH + 1)
#else
#define GEN_MTCH (MIS_POT_MTCH)
#endif
/********************************
Battery Configuration 
********************************/

#define  BAT_LOW_LEVEL_MV   2100
#define  BAT_HIGH_LEVEL_MV  2700
#define  BAT_MAX_LEVEL_MV   3080

/********************************/
/*! Initial value for high level of output voltage used for battery management.*/
#define BTRY_HIGH_LVL_INIT 22//20
/*! Initial value for low level of output voltage used for battery management.*/
#define BTRY_LOW_LVL_INIT 20//15
/*! Initial value for high level of magnetic field used for interference detection.*/
#define MGNT_HIGH_LVL_INIT 36
/*! Default buzzer tone takes one of the values of BEEP_Frequency_TypeDef.*/
#define DFLT_BUZ_TONE BEEP_Frequency_1KHz
/*! Buzzer tone 0.*/
#define BUZ_TONE0 BEEP_Frequency_1KHz
/*! Buzzer tone 1.*/
#define BUZ_TONE1 BEEP_Frequency_2KHz
/*! Buzzer tone 2.*/
#define BUZ_TONE2 BEEP_Frequency_4KHz
/*! LCD timeout in seconds, appearance time of LCD in power down after a switch press.
* Value (0 - 16).*/
#define LCD_TIMEOUT_LOW_POWER 10
/*! Period to wait switch state pin change.*/
#define SW_STAT_CHANGE_PERIOD 100
/*! Period to wait before next scanning in power down.*/
#define PWR_DN_SCAN_PERIOD 5000
/*! Delay period to avoid multiple screens scrolling due to one press in low power mode.*/
#define PWR_DN_SW_DELAY 125000
/*! Get the type of battery management in this meter.*/
#if (CTRL_SUPER_CAP == 1)
#define BTRY_SEL 0
#elif (CTRL_BTRY_NON_CHRG == 1)
#define BTRY_SEL 1
#elif (CTRL_BTRY_CHRG == 1)
#define BTRY_SEL 2
#endif
/*! Get the value of bit0 of battery management type.*/
#define BTRY_SEL0 (BTRY_SEL & 0x01)
/*! Get the value of bit1 of battery management type.*/
#define BTRY_SEL1 (BTRY_SEL & 0x02)
/*! Get the value of bit2 of battery management type.*/
#define BTRY_SEL2 (BTRY_SEL & 0x04)
/*! Get value of magnetic feature bit.*/
#if (CTRL_MGNT_SW == 1)
#define MGNT_FETUR_FETUR 0x08
#else
#define MGNT_FETUR_FETUR 0
#endif
/*! Get value of control actions feature bit.*/
#ifdef CTRL_ACTIONS
#define CTRL_ACTIONS_FETUR 0x10
#else
#define CTRL_ACTIONS_FETUR 0
#endif
/*! Get value of events log feature bit.*/
#ifdef CTRL_EVNT_LOG
#define CTRL_EVNT_LOG_FETUR 0x20
#else
#define CTRL_EVNT_LOG_FETUR 0
#endif
/*! Get value of configure meter log feature bit.*/
#ifdef CTRL_CFG_METER_LOG
#define CTRL_CFG_METER_LOG_FETUR 0x40
#else
#define CTRL_CFG_METER_LOG_FETUR 0
#endif
/*! Get value of MD switch feature bit.*/
#ifdef CTRL_MD_SW
#define CTRL_MD_SW_FETUR 0x80
#else
#define CTRL_MD_SW_FETUR 0
#endif
/*! Get value of cover switch feature bit.*/
#ifdef CTRL_CVR_SW
#define CTRL_CVR_SW_FETUR 0x01
#else
#define CTRL_CVR_SW_FETUR 0
#endif
/*! Get value of terminal switch feature bit.*/
#ifdef CTRL_TRMNL_SW
#define CTRL_TRMNL_SW_FETUR 0x02
#else
#define CTRL_TRMNL_SW_FETUR 0
#endif
/*! Get value of up switch feature bit.*/
#ifdef CTRL_UP_SW
#define CTRL_UP_SW_FETUR 0x04
#else
#define CTRL_UP_SW_FETUR 0
#endif
/*! Get value of down switch feature bit.*/
#ifdef CTRL_DN_SW
#define CTRL_DN_SW_FETUR 0x08
#else
#define CTRL_DN_SW_FETUR 0
#endif
/*! Get value of tamper LED feature bit.*/
#ifdef CTRL_TMPR_LED
#define CTRL_TMPR_LED_FETUR 0x10
#else
#define CTRL_TMPR_LED_FETUR 0
#endif
/*! Get value of low credit LED feature bit.*/
#ifdef CTRL_LOW_CRDT_LED
#define CTRL_LOW_CRDT_LED_FETUR 0x20
#else
#define CTRL_LOW_CRDT_LED_FETUR 0
#endif
/*! Get value of relay feature bit.*/
#ifdef CTRL_RLY
#define CTRL_RLY_FETUR 0x40
#else
#define CTRL_RLY_FETUR 0
#endif
/*! Get value of buzzer feature bit.*/
#ifdef CTRL_BZR
#define CTRL_BZR_FETUR 0x80
#else
#define CTRL_BZR_FETUR 0
#endif
/*! Get value of alarm icon feature bit.*/
#ifdef CTRL_ALRM_ICON
#define CTRL_ALRM_ICON_FETUR 0x01
#else
#define CTRL_ALRM_ICON_FETUR 0
#endif
/*! Get value of RTC feature bit.*/
#ifdef CTRL_RTC
#define CTRL_RTC_FETUR 0x02
#else
#define CTRL_RTC_FETUR 0
#endif
/*! Get value of magnetic interference detection with ADC bit.*/
#if (CTRL_MGNT_SENSOR == 1)
#define CTRL_MGNT_SENSOR_FETUR 0x04
#else
#define CTRL_MGNT_SENSOR_FETUR 0
#endif
/*! Get value of cover switch feature bit.*/
#if defined (CTRL_GPRS_MODULE_CVR_SW) && defined (MTR_THREE_PH)
#define CTRL_GPRS_MODULE_CVR_FETUR 0x08
#else
#define CTRL_GPRS_MODULE_CVR_FETUR 0
#endif
/*! Get value of features byte0.*/
#define CTRL_FETUR0 (BTRY_SEL0 + BTRY_SEL1 + BTRY_SEL2 + MGNT_FETUR_FETUR + \
CTRL_ACTIONS_FETUR + CTRL_EVNT_LOG_FETUR + CTRL_CFG_METER_LOG_FETUR + \
  CTRL_MD_SW_FETUR)
/*! Get value of features byte1.*/
#define CTRL_FETUR1 (CTRL_CVR_SW_FETUR + CTRL_TRMNL_SW_FETUR + CTRL_UP_SW_FETUR + \
CTRL_DN_SW_FETUR + CTRL_TMPR_LED_FETUR + CTRL_LOW_CRDT_LED_FETUR + CTRL_RLY_FETUR +\
  CTRL_BZR_FETUR)
/*! Get value of features byte2.*/
#define CTRL_FETUR2 (CTRL_ALRM_ICON_FETUR + CTRL_RTC_FETUR + CTRL_MGNT_SENSOR_FETUR + CTRL_GPRS_MODULE_CVR_FETUR)
/*! Get value of features byte3.*/
#define CTRL_FETUR3 0

#define CTRL_DEFAULT_CONSUMPTION_ALARM_VALUE    0UL
#define CTRL_DEFAULT_GRACE_CONSUMPTION_VALUE    0UL


/*! Tell metering by power down event with saving in non volatile memory.*/
#define LPM_TELL_METERING_PWR_DN_WITH_SAVE() mtr_pwr_dwn(MTR_WITH_SAVE)
#define LPM_TELL_RTC_PWR_DN()          __NOP()

//#ifdef MTR_THREE_PH
//#define DISPLAY_SET_BACK_LIGHT()                 S_GPIO_Set_Bits(LCD_BACK_LIGHT_PORT, BACK_LIGHT_PIN) //Set_PIN(LCD_BL_PORT_OUT,LCD_BL_PIN)           
//#define DISPLAY_RESET_BACK_LIGHT()           S_GPIO_Clear_Bits(LCD_BACK_LIGHT_PORT, BACK_LIGHT_PIN) // Clr_PIN(LCD_BL_PORT_OUT,LCD_BL_PIN)
//#else
#define DISPLAY_SET_BACK_LIGHT()                 S_GPIO_Clear_Bits(LCD_BACK_LIGHT_PORT, BACK_LIGHT_PIN) //Set_PIN(LCD_BL_PORT_OUT,LCD_BL_PIN)
#define DISPLAY_RESET_BACK_LIGHT()           S_GPIO_Set_Bits(LCD_BACK_LIGHT_PORT, BACK_LIGHT_PIN) // Clr_PIN(LCD_BL_PORT_OUT,LCD_BL_PIN)
//#endif
#define CTRL_NO_BTRY_VOLTAGE       10   // 1.0 voltage

#ifdef ANSI_STACK
#ifdef MTR_SINGLE_PH
#define GET_RLY1_STATUS() S_GPIO_Get_Bits(RLY_STATUS_PORT,RLY1_STATUS_PIN)
#else
#define GET_RLY1_STATUS() S_GPIO_Get_Bits(RLY_STATUS_PORT,RLY1_STATUS_PIN)
#define GET_RLY2_STATUS() S_GPIO_Get_Bits(RLY_STATUS_PORT,RLY2_STATUS_PIN)
#define GET_RLY3_STATUS() S_GPIO_Get_Bits(RLY_STATUS_PORT,RLY3_STATUS_PIN)
#endif//MTR_SINGLE_PH
#endif//ANSI_STACK
#undef CTRL_TMPR_LED
#undef CTRL_LOW_CRDT_LED
#if 0 
#undef CTRL_ALRM_ICON
#undef CTRL_RLY  
#undef MTR_ENABLE_LMT
#undef G2_DM
#endif
/*------------------------------------------------------------------------------------------
Data Types
------------------------------------------------------------------------------------------*/
/*!
* @par Description:
*   This enumerator contains results of testin a switch.
*/
typedef enum
{
  TEST_SW_PASS = (uint8_t)0, /*!< Test switch passes.*/
  SW_ALWAYS_PRESSED = (uint8_t)1, /*!< Switch is always pressed.*/
  SW_ALWAYS_RELEASED = (uint8_t)2, /*!< Switch is always released.*/
} sw_test_t;

/*!
* @par Description:
*   This enumerator contains flags masks of action byte corresponding to each event #tmpr_t.
*/
typedef enum
{
  CVR_REC = (uint8_t)0, /*!< Cover open record code in internal memory.*/
  TRMNL_REC = (uint8_t)1, /*!< Terminal open record code in internal memory.*/
  DETACH_REC = (uint8_t)2, /*!< Terminal open record code in internal memory.*/
} int_mem_rec_code_t;



/*!
* @par Description:
*   This enumerator contains flags masks of control system.
*/
typedef enum
{
  NEW_SEC_FLAG = (uint8_t)0x01, /*!< New second flag.*/
  NEW_MIN_FLAG = (uint8_t)0x02, /*!< New minute flag.*/
  NEW_HR_FLAG = (uint8_t)0x04, /*!< New hour flag.*/
  INT_MEM_FLAG = (uint8_t)0x08, /*!< flag of saveing in internal memory.*/
  RELAY_ERR_FLAG = (uint8_t)0x10, /*!< Flag of relay error (should be open and there is current drawn).*/
  SR_SCAN_MASK = (uint8_t)0x20, /*!< Scan shift register flag.*/
} flags_byte2_masks_t;
/*!
* @par Description:
*   This enumerator contains flags masks of control system.
*/
typedef enum
{
  PWR_UP_MASK = (uint8_t)0x02, /*!< Power up flag mask.*/
  BTRY_LOW_STRT_MASK = (uint8_t)0x04, /*!< Battery low flag mask.*/
  BTRY_HIGH_STRT_MASK = (uint8_t)0x08, /*!< Battery high flag mask.*/
  MGNT_LOW_STRT_MASK = (uint8_t)0x10, /*!< Magnetic low flag mask.*/
  MGNT_HIGH_STRT_MASK = (uint8_t)0x20, /*!< Magnetic high flag mask.*/
  CHK_BTRY_MGNT_MASK = (uint8_t)0x40, /*!< Check battery and magnetic flag.*/
  CHK_TELT_SW_MASK = (uint8_t)0x80, /*!< Check battery and magnetic flag.*/
} flags_byte_masks_t;


/*!
* @par Description:
*   This enumerator contains flags masks of halt states.
*/
typedef enum
{
  LCD_STAT_IN_HALT_MASK = (uint8_t)0x01, /*!< State of LCD after halt.*/
  SW_INT_IN_HALT_MASK = (uint8_t)0x02, /*!< State of switches interrupt after halt.*/
} halt_state_masks_t;



/*------------------------------------------------------------------------------------------
External Variables
------------------------------------------------------------------------------------------*/
uint8_t display_relay_disc_flag = 0;
uint16_t opened_relay_code = 0xffff;
uint8_t formed_action_2 = 0;
uint8_t display_control_flag = 0;
extern uint8_t optical_counter_disable;
extern uint8_t communication_error_flag ;
#ifdef BTRY_LOG
ctrl_BTRY_rec_t  Batttery_Saved = {320,0,0,0,0,0,0};
#endif
 #ifdef OPTICAL_CTRL_FEATURE
  static volatile uint8_t state_of_connectionRightOrWrong =0;
#endif
extern uint8_t rtc_flag_counter;
uint8_t Global_switch_Buzzer=0;
#ifdef AC_BUZZER_FEATURE
extern volatile uint8_t xc;
#endif
ctrl_sys_new_var_t ctrl_sys_new_var={0};
#ifdef AC_BUZZER_FEATURE 
volatile uint8_t buzzer_enable = 0 ;
#endif
static uint8_t flags_byte; /*!< Bit 0 flag of scan shift register.
* Bit 1 flag of power up
* Bit 2 Battery low start
* Bit 3 Battery high start
* Bit 4 Magnetic low start
* Bit 5 Magnetic high start
* Bit 6 Check battery and magnetic flag
* Bit 7 We are waiting end hint of relay flag.*/
//static uint8_t ctrl_sys_var_optional_feature.flag1;
/*!< Bit 0 Up switch state
* Bit 1 Down switch state
* Bit 2 MD switch status
* Bit 3 Continous press period elapsed flag
* Bit 4 Display processing flag while power down.
* Bit 5 New hint by relay action flag
* Bit 6 Open relay mode flag.*/
static uint8_t flags_byte2; /*!< Bit 0 New second flag
* Bit 1 New minute flag
* Bit 2 New hour flag
* Bit 3 LCD timeout flag
* Bit 4 Save in events in internal memory flag
* Bit 5 Relay error flag
* Bit 6 Don't enable interrupt with SR scanning flag
* Bit 7 Ignore result of scanning scrolling switches.*/
volatile uint8_t ctrl_flags; /*!< Flags of control system.*/
uint8_t LPM_status = LPM_NONE_STATUS;
ctrl_sys_var_second_optional_feature_t ctrl_sys_var_second_optional_feature;
volatile uint8_t Boot_loader_flag = NO_BOOTLOADER_DATA;
ctrl_cfg_t ctrl_cfg; /*!< Structure containing all configuration data.*/
ctrl_cfg_t temp_ctrl_cfg; /*!< Structure containing all configuration data.*/
ctrl_sys_var_t ctrl_sys_var; /*!< Structure containing all system variables.*/
ctrl_sys_var_optional_feature_t ctrl_sys_var_optional_feature;
//volatile uint8_t ctrl_flags; /*!< Flags of control system.*/
//volatile ctrl_sys_var_optional_feature_t ctrl_sys_var_optional_feature={0};
uint8_t test_flag = TRUE;
uint8_t ctrl_tst_mode = 0; /*!< Flags of control system.*/
/*! Counter of number of times overload tamper has been repeated during one
* detection period.*/
//uint8_t ctrl_sys_new_var.ovr_load_times_cntr;
volatile uint16_t ctrl_10ms_timer = 0;
uint8_t ctrl_power_down_event=0;
ctrl_last_tamper_t last_tampers;
uint32_t ctrl_Event_Notification_flag = 0;
uint8_t channel_cpy = 0x0;
uint8_t check_relay_save_event = 0;
//extern uint8_t first_day;
volatile uint8_t HDLC_Activity_Counter = 0x00;
uint8_t penalty_tmp = 0;
volatile uint8_t scan_result = 0;
uint8_t continuos_time=0;
uint8_t save_ctrl_cfg_flg = 0;
volatile uint32_t ctrl_rtc_reinit_ctr = 0;
volatile uint8_t No_of_reiniting_rtc = 0;
volatile uint16_t UnEntering_ctrl_task_Num = 0 ;
uint8_t uC_speed_high=0;
uint8_t ctrl_high_freq = 0;
uint8_t skip_counter = 0;
uint8_t ctrl_exit = 0;

uint8_t ptr_meter_state[107];
#ifdef G2_DM
uint8_t Remove_tamp_count = 0;
uint8_t ctrl_technician_id[16] = {0};
#endif
#ifdef KEYPAD_FEATURE
uint8_t keypad_reinit_counter=0;
#endif
#ifdef RF_CC1120
uint8_t cc1120_reinit_counter=0;
#endif
#ifdef DOT_MATRIX_LCD_ENABLE
uint8_t DOT_MATRIX_reinit_counter=0;
#ifdef DOT_MATRIX_LOW_PWR_FEATURE
uint8_t DM_low_pwr_mode_entering_flag=0;
#endif
#endif
#ifdef NEW_ASHANTI_PROJECT
uint8_t data_is_loaded=0;
#endif
#ifdef GROUP_METERING
uint8_t GM_readout_data_timing=10;
uint8_t GM_rise_readout_data_flag=0;
#endif
uint8_t save_switches_in_pwrdn = 0;
uint8_t prv_sw_state=0;
uint8_t  CoverClosedFlag = 0;
//=============================================================================
// Name			:	CaptureObjectsLoadProfile
// Description	:	List of Capture Object in Load Profie
//-----------------------------------------------------------------------------
// Author		:	Mohamed H. Elsaidy
// Date			:	04/10/2011
//=============================================================================
void ctrl_set_openCoverFlagPWD()
{
  ctrl_power_down_event |= CVR_OPN_MASK;
}

#if 0
const TCapture_Object CaptureObjectsLoadProfile[LOAD_PROFILE_RECORDS] =
{
  {
    8, //class ID -> clock
    {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, //OBIS -> clock RTC
    2, // attribute index -> time
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01, 0x00, 0x01, 0x08, 0x00, 0xFF}, //OBIS ->Total Active Energy Import
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01, 0x00, 0x03, 0x08, 0x00, 0xFF}, //OBIS -> Total Reactive Energy Import
    2, // attribute index -> value
    0, //data index
  },
  /* Power factor */
  {
    3, //class ID -> register
    {0x01, 0x00, 0x21, 0x07, 0x00, 0xFF},
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01, 0x00, 0x35, 0x07, 0x00, 0xFF},
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01, 0x00, 0x49, 0x07, 0x00, 0xFF},
    2, // attribute index -> value
    0, //data index
  },
  /*Power*/
  {
    3, //class ID -> register
    {0x01, 0x00, 0x1D, 0x07, 0x00, 0xFF}, //OBIS -> apparent Power phase 1
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01, 0x00, 0x31, 0x07, 0x00, 0xFF}, //OBIS -> apparent Power phase 2
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01, 0x00, 0x45, 0x07, 0x00, 0xFF}, //OBIS -> apparent Power phase 3
    2, // attribute index -> value
    0, //data index
  },
};
#endif
#ifdef DLMS_ENABLED

const TCapture_Object CaptureObjectsLoadProfile[LOAD_PROFILE_RECORDS] =
{
  {
    8, //class ID -> clock
    {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, //OBIS -> clock RTC
    2, // attribute index -> time
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x14 ,0xFF ,0xFF ,0xFF},//Total Cons
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x1F ,0x07 ,0x00 ,0xFF},//intstant current
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x1F ,0x07 ,0x00 ,0xFF},//intstant current
    2, // attribute index -> value
    0, //data index
  }, 
  
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x1F ,0x07 ,0x00 ,0xFF},//intstant current
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x20 ,0x07 ,0x00 ,0xFF}, //OBIS -> intstant voltage
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x20 ,0x07 ,0x00 ,0xFF}, //OBIS -> intstant voltage
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x20 ,0x07 ,0x00 ,0xFF}, //OBIS -> intstant voltage
    2, // attribute index -> value
    0, //data index
  },
  /* Power factor */
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x21 ,0x07 ,0x00 ,0xFF},// obis -> instant PowerFactor
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x21 ,0x07 ,0x00 ,0xFF},// obis -> instant PowerFactor
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x21 ,0x07 ,0x00 ,0xFF},// obis -> instant PowerFactor
    2, // attribute index -> value
    0, //data index
  },
};
const TCapture_Object CaptureObjectsMeterCfg[METER_CFG_PROFILE] =
{
  {
    8, //class ID -> clock
    {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, //OBIS -> clock RTC
    2, // attribute index -> time
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x14 ,0xFF ,0xFF ,0xFF},//Total Cons
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x1F ,0x07 ,0x00 ,0xFF},//intstant current
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x1F ,0x07 ,0x00 ,0xFF},//intstant current
    2, // attribute index -> value
    0, //data index
  }, 
  
};
const uint8_t CaptureObjectsMeterCFG_XDR_Size[LOAD_PROFILE_RECORDS] =
{
  14,
  3,
  2,
  3,
};
/*
const uint8_t CaptureObjectsLoadProfile_XDR_Size[LOAD_PROFILE_RECORDS] =
{
14,
9,
9,
3,3,3,
9,9,9,
9,9,9
};
*/
const uint8_t CaptureObjectsLoadProfile_XDR_Size[LOAD_PROFILE_RECORDS] =
{
  14,
  5,
  3,3,3,
  3,3,3,
  2,2,2
};




const TCapture_Object BillingProfile_CaptureObjects[BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS] =
{
  {
    8, //class ID -> clock
    {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, //OBIS -> clock RTC
    2, // attribute index -> time
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x15 ,0x08 ,0x00 ,0xFF},//intstant current
    2, // attribute index -> value
    0, //data index
  },
  {
    3, //class ID -> register
    {0x01 ,0x00 ,0x20 ,0x07 ,0x00 ,0xFF}, //OBIS -> intstant voltage // make the billing object which found in the billing Period Structure 
    2, // attribute index -> value
    0, //data index
  },
  
};




const uint8_t BillingProfile_CaptureObjects_XDR_Size[BILLING_PROFILE_NUMBER_OF_CAPTURE_OBJECTS] = 
{
  14,
  5,
  5,
  
};



//=============================================================================
// Name           : Standard_Log_CaptureObjects
// Description    :  List of Capture Object in Standard Log
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 26/09/2011
//=============================================================================
const TCapture_Object Standard_Log_CaptureObjects[STANDARD_LOG_NUMBER_OF_CAPTURE_OBJECTS] =
{
  {
    8, //class ID -> clock
    {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, //OBIS -> clock RTC
    2, // attribute index -> time
    0, //data index
  },
  {
    3, //class ID -> Register
    {0x00, 0x00, 0x60, 0x0A, 0x07, 0xFF}, //OBIS -> Meter status
    2, // attribute index -> value
    0, //data index
  },
  {
    1, //class ID -> Data
    {0x00, 0x00, 0x60, 0x0B, 0x00, 0xFF}, //OBIS -> Event Code
    2, // attribute index -> value
    0, //data index
  },
};

const uint8_t Standard_Log_CaptureObjects_XDR_Size[STANDARD_LOG_NUMBER_OF_CAPTURE_OBJECTS] =
{
  14,
  9,
  2,
};




const TCapture_Object Money_Trans_CaptureObjects[MONEY_TRANS_LOG_NUMBER_OF_CAPTURE_OBJECTS] =
{
  {
    8, //class ID -> clock
    {0x00, 0x00, 0x01, 0x00, 0x00, 0xFF}, //OBIS -> clock RTC
    2, // attribute index -> time
    0, //data index
  },
  {
    1, //class ID -> Register
    {0x00, 0x00, 0x60, 0x0A, 0x07, 0xFF}, //OBIS -> Meter status
    2, // attribute index -> value
    0, //data index
  },
  {
    1, //class ID -> Register
    {0x00, 0x00, 0x60, 0x0A, 0x07, 0xFF}, //OBIS -> Meter status
    2, // attribute index -> value
    0, //data index
  },
  {
    1, //class ID -> Data
    {0x00, 0x00, 0x60, 0x0B, 0x00, 0xFF}, //OBIS -> Event Code
    2, // attribute index -> value
    0, //data index
  },
  {
    1, //class ID -> Register
    {0x00, 0x00, 0x60, 0x0A, 0x07, 0xFF}, //OBIS -> Meter status
    2, // attribute index -> value
    0, //data index
  },
  {
    1, //class ID -> Data
    {0x00, 0x00, 0x60, 0x0B, 0x00, 0xFF}, //OBIS -> Event Code
    2, // attribute index -> value
    0, //data index
  },
};



const uint8_t Money_Trans_CaptureObjects_XDR_Size[MONEY_TRANS_LOG_NUMBER_OF_CAPTURE_OBJECTS] =
{
  14,
  5,
  5,
  3,
  2,
  2,
};

#endif
uint8_t crl_lpm_enable_tamper_sw;
#ifdef G2_DM
uint32_t tech_id_temp;
#else
uint16_t tech_id_temp;
#endif
uint8_t transaction_id_temp[8];
/*------------------------------------------------------------------------------------------
Local Variables
------------------------------------------------------------------------------------------*/
/*! This array is lookup table used to match each bit in the first 6 bytes in
* meter status (status of tampers and alarms) to the index of this alarm or
* tamper in action and counter arrays.*/
const uint8_t match_array[NO_TMPRS_ALRMS_BITS] = \
  {
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - Tariff index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - File Sys. & EPROM.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - Metering & ADE.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - Display & LCD index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - RF App. & Driver index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - GPRS index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - RFID index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)DATA_CRDT_MTCH, /*!< Data Corruption - Control System index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - EEPROM index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - ADE comm. index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - No Interrupt index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - RF Chip index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - RFID Chip index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - GPRS Chip index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Hardware Error - Relay error(There is current drawn despite the relay should be open).*/
    (uint8_t)HRDWR_ERR_MTCH, /*!< Reserved.*/
    (uint8_t)BTRY_MTCH, /*!< Battery Low index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)LOW_CRDT_1ST_MTCH, /*!< Low Credit First Alarm index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)LOW_CRDT_2ND_MTCH, /*!< Low Credit Second Alarm index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)NO_CRDT_FRNDLY_MTCH, /*!< No Credit with friendly index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)NO_CRDT_MTCH, /*!< No Credit index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)OVR_LOAD_MTCH, /*!< Over Load Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)OVR_LOAD_MTCH, /*!< Over Load Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)OVR_LOAD_MTCH, /*!< Over Load Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)OVR_VOLT_MTCH, /*!< Over Volt Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)OVR_VOLT_MTCH, /*!< Over Volt Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)OVR_VOLT_MTCH, /*!< Over Volt Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)UNDR_VOLT_MTCH, /*!< Under Volt Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)UNDR_VOLT_MTCH, /*!< Under Volt Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)UNDR_VOLT_MTCH, /*!< Under Volt Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)PHS_SEQ_MTCH, /*!< Phase Sequence Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)PHS_SEQ_MTCH, /*!< Phase Sequence Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)PHS_SEQ_MTCH, /*!< Phase Sequence Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)CVR_OPN_MTCH, /*!< Cover Open index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)TRMNL_OPN_MTCH, /*!< Terminal Open index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)MGNT_HIGH_MTCH, /*!< Magnetic High index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)RVRS_CRNT_MTCH, /*!< Reverse Current Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)RVRS_CRNT_MTCH, /*!< Reverse Current Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)RVRS_CRNT_MTCH, /*!< Reverse Current Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)FLT_ENRG_MTCH, /*!< Fault Energy Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)FLT_ENRG_MTCH, /*!< Fault Energy Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)FLT_ENRG_MTCH, /*!< Fault Energy Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)MIS_POT_MTCH, /*!< Missing Potential Channel A index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)MIS_POT_MTCH, /*!< Missing Potential Channel B index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)MIS_POT_MTCH, /*!< Missing Potential Channel C index of this alarm or tamper in action and counter arrays.*/
    (uint8_t)GEN_MTCH,
    (uint8_t)GPRS_MODULE_CVR_OPEN,
  };
/*! Array containing sizes of configuration structures.*/
const uint8_t cfg_struct_size[CTRL_CFG_MAX + 1] =
{
  sizeof(ser_t),
  sizeof(id_t),
#if ((defined(CTRL_ACTIONS)) && (defined(CTRL_BZR)))
  sizeof(tmpr_t), /*!< Size of actions structures.*/
#else
  0,
#endif
#ifdef CTRL_RTC
  sizeof(rtc_time_t), /*!< Size of time structure.*/
  sizeof(rtc_date_t), /*!< Size of date structure.*/
#else
  0,
  0,
#endif
#if (CTRL_BTRY_NON_CHRG == 1)
  sizeof(btry_t), /*!< Size of battery control structure.*/
#else
  0,
#endif
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
  sizeof(mgnt_t), /*!< Size of magnetic detection structure.*/
#else
  0,
#endif
#if (defined(CTRL_RLY) && defined(MTR_ENABLE_LMT))
  sizeof(overload_t),
#else
  0,
#endif
};
static uint8_t power_on_minute_counter;
static uint8_t sr_ptrn; /*!< Shift register pattern
* Bit 0 Terminal Switch
* Bit 1 Cover Switch
* Bit 2 Up Switch
* Bit 3 Down Switch.*/
#ifdef CTRL_ACTIONS
static uint8_t formed_action;
#endif
static uint8_t calb_pin_state; /*! Calibration pin state. Each bit represents a check* result on the pin check is done every timer interrupt period.*/

#if (defined(CTRL_UP_SW) || defined(CTRL_DN_SW))
static uint8_t cont_press_period; /*!< Period to be elapsed while pressing the
* switch to consider continous press.*/
static uint8_t cont_press_counter; /*!< Continous press counter for scrolling switches incremented with timer tik.*/
#endif
uint8_t pwr_up_sec_cntr; /*!< Seconds counter since last power up.*/
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
/*! Seconds counter used for detect and restore times of magnetic interference.*/
static uint8_t mgnt_min_cntr;
#endif
#if (CTRL_BTRY_NON_CHRG == 1)
static uint8_t btry_prst;
#endif
#if (defined(CTRL_BZR) || defined(CTRL_RLY))
static uint8_t blind_hint_period_sec_counter;
#endif
#if (defined(MTR_ENABLE_LMT) && defined(CTRL_RLY))
/*! Second counter used when the configured action with over load tamper is opening relay.*/
//static uint16_t ctrl_sys_new_var.ovr_load_sec_cntr;
#endif
#ifdef CTRL_BZR
static uint8_t beep_counter;
#endif
#if (defined(CTRL_MD_SW) || defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW) || defined (CTRL_GPRS_MODULE_CVR_SW))
static uint8_t test_sw_sec_counter;
#endif
static uint8_t timeout_cntr = 0;
static uint8_t reset_with_corruption_counter = 0;
#ifdef EDIT_TIME_DATE_MANUALLY
static uint8_t up_down_sw_flag=0;
static uint8_t scan_result_cpy;
#endif
/* a timer to count how many seconds the meter is up side down before recording up side down event */
static uint8_t ctrl_upsidedown_counter;


static uint8_t lpm_pwr_up_sec_cntr; /*!< Seconds counter since last power up.*/
RTC_TimeTypeDef DateTimeDefault = 
{ 
  {0,58,5},
  {1,1,19,3}
};
#ifdef KEYPAD_FEATURE
static keypad_sw_t keypad_up_down =(keypad_sw_t) 0; //get Up and Down Switchs state through KeyPad keypad_sw_t
#endif

uint32_t ctrl_test_relay_second = 0;
uint16_t ctrl_test_relay_minutes = 0;
uint8_t ctrl_test_relay_flag = 0;
uint8_t ctrl_test_relay_second_one_shot;
uint8_t battery_percentage = 0;
uint32_t ctrl_start_open_relay_test_sec_counter;
uint16_t battery_percentage_fraction;
/*------------------------------------------------------------------------------------------
Macros
------------------------------------------------------------------------------------------*/
#define CTRL_RTC_INIT_FLAG    0xEE

/*! Check magnetic switch. 0 represents magnetic interference low, other high.*/
#define CTRL_CHECK_MGNT_SWITCH() (!S_GPIO_Get_Bits(SW_PORT, MAG_SW_PIN))
#define CTRL_CHECK_MGNT_SENSOR()   GetMagneticLevel()
/*! Enter penality mode.*/
#define CTRL_ENTER_PENALTY_MODE() mtr_cmd(MTR_STRT_PEN)
/*! Exit penality mode.*/
#define CTRL_EXIT_PENALTY_MODE() mtr_cmd(MTR_END_PEN)
/*! Apply current status of the relay.*/
#define RELAY_INIT() {check_relay_save_event = 3 ;ctrl_sys_var.basic.meter_status[6] ^= CTRL_RLY_MASK;toggle_relay();}
/*! Call display task returns TRUE to go again to power down.*/
#define CTRL_CALL_DISPLAY_TASK()   display_task()
/*! Read from internal memory.*/
#define CTRL_READ_INT(mem_adrs, data_ptr, data_size) file_sys_read_int(mem_adrs, data_ptr, data_size)
/*! Write in internal memory.*/
#define CTRL_WRITE_INT(mem_adrs, data_ptr, data_size) file_sys_write_int(mem_adrs, data_ptr, data_size)
/*! This MACRO is called when up switch is pressed.*/
#define CTRL_LCD_UP_SW_PRESSED() display_event(DISPLAY_EVE_KEY_UP)
/*! This MACRO is called when up switch is pressed.*/
#define CTRL_LCD_DN_SW_PRESSED() display_event(DISPLAY_EVE_KEY_DOWN)
/*! This MACRO is called when up switch is released.*/
#define CTRL_LCD_UP_SW_RELEASED() display_event(DISPLAY_EVE_UP_KEY_RELEASE)
/*! This MACRO is called when DOWN switch is released.*/
#define CTRL_LCD_DOWN_SW_RELEASED() display_event(DISPLAY_EVE_DOWN_KEY_RELEASE)
/*! Read the switches status.*/
#define CTRL_READ_SW_STAT() GPIO_ReadInputDataBit(SW_PORT, SW_STAT_PIN)
/*! Read the cover switch state.*/
#define CTRL_READ_CVR_SW()  S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN)
/*! Read the GPRS module cover switch state.*/
#define CTRL_READ_GPRS_MODULE_CVR_SW()   (S_GPIO_Get_Bits(SW_PORT, M_SW_PIN)? 0 : M_SW_PIN)
#ifdef MTR_INDIRECT
/*! Read the connection switch state.*/
#define CTRL_READ_SWITCHE_CONNECTION_SW()   S_GPIO_Get_Bits(ENCLOSER_SW_PORT, ENCLOSER_SW_PIN)
#endif
/*! Read the terminal switch state.*/
#define CTRL_READ_TRML_SW() S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN)
/*! Read the down switch state.*/
#ifdef EM_DL130_BOARD
#define CTRL_READ_DWN_SW()  1//S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_DOWN_PIN)
#else
#define CTRL_READ_DWN_SW()  S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_DOWN_PIN)
#endif
#ifdef ANSI_STACK
/*! Set Relay Close pin.*/
#define CTRL_WRITE_RELAY_CLOSE()  S_GPIO_Set_Bits(RELAY_PORT, RELAY_CONNECT)
/*! Set Relay Open pin.*/
#define CTRL_WRITE_RELAY_OPEN() S_GPIO_Set_Bits(RELAY_PORT, RELAY_DISCONNECT)

#else//ANSI_STACK

/*! Set Relay open pin.*/
#define CTRL_WRITE_RELAY_CLOSE()  S_GPIO_Set_Bits(RELAY_PORT, RELAY_DISCONNECT)


/*! Set Relay close pin.*/
#define CTRL_WRITE_RELAY_OPEN() S_GPIO_Set_Bits(RELAY_PORT, RELAY_CONNECT)
#endif////ANSI_STACK
/*! Read the external switch state.*/
#ifdef MTR_INDIRECT 
#define CTRL_READ_EXTERNAL_REALY_STATUS_SW()       S_GPIO_Get_Bits(CONTACTOR_PORT, CONTACTOR_PIN)
#endif

/*! Reset Relay open and close pins.*/
#define CTRL_WRITE_STOP_RELAY()   {S_GPIO_Clear_Bits(RELAY_PORT, RELAY_CONNECT); S_GPIO_Clear_Bits(RELAY_PORT, RELAY_DISCONNECT);}


/*! Set tamper led pin.*/
#define CTRL_WRITE_TMP_LED_ON()    S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
/*! Reset tamper led pin.*/
#define CTRL_WRITE_TMP_LED_OFF()  S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
/*! Toggle tamper led pin.*/
#define CTRL_WRITE_TMP_LED_TOGGLE()  S_GPIO_Toggle_Bits(TMPR_LED_PORT, TMPR_LED_PIN)
/*! Set low credit led pin.*/
#define CTRL_WRITE_LOW_CRDT_ON()     S_GPIO_Clear_Bits(CRDT_LED_PORT, CRDT_LED_PIN)
/*! Reset low credit led pin.*/
#define CTRL_WRITE_LOW_CRDT_OFF()    S_GPIO_Set_Bits(CRDT_LED_PORT, CRDT_LED_PIN)
/*! Toggle Low credit led pin.*/
#define CTRL_WRITE_LOW_CRDT_LED_TOGGLE()  S_GPIO_Toggle_Bits(CRDT_LED_PORT,CRDT_LED_PIN)
/*! Read the up switch state.*/
#define CTRL_WRITE_UP_SW()   S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_UP_PIN)
/*! Read magntic switch pin.*/
#define READ_MGNT_PIN() GPIO_ReadInputDataBit(SW_PORT, MAG_SW_PIN)
/*! Send byte by SPI to shift register.*/
#define CTRL_SEND_BYTE(byte) bSPI_InfSyncTxByte(byte)
/*! Enable MCU interrupts.*/
#define CTRL_ENABLE_INT()  __enable_irq();
/*! Disable all interrupts used before sending data to shift register.*/
#define CTRL_DISABLE_INT() __disable_irq() 
/*! Reset tariff MD data.*/
#define CTRL_RST_TRF_MD(adr) trf_pymt_cmd(TRF_PYMT_CMD_RST, adr)
/*! What to do when MD button is pressed.*/
#define CTRL_RST_MTR_MD() mtr_cmd(MTR_RST_MD)
/*! - Read MD switch pin value.*/
#define READ_MD_SW_PIN() S_GPIO_Get_Bits(SW_PORT,MD_SW_PIN)
/*! Power ADC on.*/
#define CTRL_ADC_ON() S_ADC_ENABLE()
/*! Power ADC off.*/
#define CTRL_ADC_OFF() S_ADC_DISABLE()
/*! Measure voltage value at certain ADC channel.*/
#define CTRL_ADC_READ(result_adr)   S_ADC_Get_Vbat(result_adr);


/*! Initialize GPIOs by normal configuration.*/
#define CTRL_INIT_GPIO() vInit_GPIO()
/*! Initialize Drivers of MCU.*/
#define CTRL_INIT_DRIV() vMC_DriversInit()
/*! Go to halt mode.*/
#define CTRL_HALT() LPM3
/*! Used to disable all interrupts and configure all pins to output and output value is zero.*/
#define CTRL_MCU_DISABLE_ALL(flag)  MCU_disable_all(flag)
/*! Disable switch state pin interrupt.*/
/*! Configure power fail pin to be external interrupt pin.*/
#define CTRL_PWR_FAIL_EXT_INT()   {S_GpioInit_PIN(VDCIN_PORT, VDCIN_PIN , S_GPIO_Mode_INPUT );\
      ANA_ClearINTStatus(ANA_INT_VDCINDROP );\
      ANA_INTConfig(ANA_INT_VDCINDROP , ENABLE);}
/*! Initialize Timer 1 to have interrupt each 10msec.*/

#define CTRL_DISABLE_TIMER0()   { S_DISABLE_TIMER_0_INT; S_DISABLE_TIMER(TMR0);}
#define CTRL_DISABLE_TIMER1()   { S_DISABLE_TIMER_1_INT; S_DISABLE_TIMER(TMR1);}
/*! Read power fail pin.*/
// TODO: handle power fail pin



// TODO: check the power failure flag
#define CTRL_READ_PWR_FAIL_PIN()            0   /*  S_GPIO_Get_Bits(VDCIN_PORT, VDCIN_PIN)*/
/*! Configure interrupt sensitivity of switch status pin to falling edges only.*/
#define CTRL_SW_STAT_FALL_EDGE() (EXTI->CR1 |= 0x08)
/*! Tell display by power up event.*/
#define CTRL_TELL_DISPLAY_PWR_UP() display_event(DISPLAY_EVE_PWR_UP)
/*! Tell tariff and payment by power up event.*/
#define CTRL_TELL_TRF_PWR_UP() trf_pymt_event(TRF_PYMT_EVNT_PWR_UP,0)
/*! Tell metering by power up event.*/
#define CTRL_TELL_METERING_PWR_UP() mtr_pwr_up()
/*! Tell display by power down event.*/
#define CTRL_TELL_DISPLAY_PWR_DN() display_event(DISPLAY_EVE_PWR_DWN)
/*! Tell tariff and payment by power down event.*/
#define CTRL_TELL_TRF_PWR_DN() trf_pymt_event(TRF_PYMT_EVNT_PWR_DN,0)
/*! Tell tariff and payment to save its data.*/
#define CTRL_TELL_TRF_SAVE_DATA() trf_pymt_event(TRF_PYMT_EVNT_SAVE_DATA,0)
/*! Tell GPRS by power down event with saving in non volatile memory.*/
#define CTRL_TELL_GPRS_PWR_DN() gprs_pow_down_event()
/*! Tell metering by power down event without saving in non volatile memory.*/
#define CTRL_TELL_METERING_PWR_DN_WITHOUT_SAVE() mtr_pwr_dwn(1)
/*! Tell metering by power down event with saving in non volatile memory.*/
#define CTRL_TELL_METERING_PWR_DN_WITH_SAVE() mtr_pwr_dwn(0)
/*! Tell STS Module that power down.*/
#define CTRL_TELL_STS_PWR_DN() STS_power_down()
/*! Tell metering to save its data.*/
#define CTRL_TELL_METERING_SAVE_DATA() mtr_cmd(MTR_SAVE_DATA)
/* Tell RF link power down.*/
#define CTRL_TELL_RF_LINK_PWR_DN() RF_Link_event(RF_LINK_EVE_PWR_DWN)
/*! Wait for RTC Sync.*/
#define CTRL_WAIT_RTC_SYNC() (RTC_WaitForSynchro())
/*! Copy the date now from RTC.*/
#define CTRL_GET_RTC_DATE() RTC_GetDate( &date);
/*! Copy time now from RTC.*/
#define CTRL_GET_RTC_TIME() RTC_GetTime(&time);
/*! Inform display by new second event.*/
#define CTRL_TELL_DISPLAY_NEW_SEC() display_event(DISPLAY_EVE_SEC)
/*! Tell RF MBUS by new timer tick.*/
#define CTRL_TELL_RF_NEW_TMR_TICK() RF_Link_event(RF_LINK_EVE_NEW_TMR_TICK)
#ifdef ANSI_STACK
#define CTRL_TELL_OPTICAL_NEW_SEC()           optical_slave_event(ANSI_OPTICAL_NEW_SEC)
#define CTRL_TELL_OPTICAL_DISABLE_NEW_SEC()   optical_counter_disable++
#endif
#ifdef IEC_62056_21_SLAVE
/*! Inform IEC 62056-21 protocol by new second event.*/
#define CTRL_TELL_IEC_NEW_SEC() iec_62056_21_slave_event(IEC_62056_21_NEW_SEC)
/*! Inform IEC 62056-21 protocol by new timer tick event.*/
#define CTRL_TELL_IEC_NEW_TIMER_TICK() iec_62056_21_slave_event(IEC_62056_21_NEW_TIMER_TICK)
#endif
#define COMM_BOOTLOADER_NEW_TIMER_TICK() bootloader_newsec_event()
/*! - Inform metering module with new second event.*/
#define CTRL_TELL_MTR_NEW_SEC()  mtr_new_sec()
#ifdef RFID_FEATURE
/*! - Inform RFID module with new second event.*/
#define CTRL_TELL_RFID_NEW_SEC() rfid_sec_event()
#endif
#ifdef MODEM_ENABLE
/*! - Inform MODEM with new second event.*/
#define CTRL_TELL_SIM900_NEW_SEC() MODEM_sec_event()
#endif
#ifdef GPRS_FEATURE
/*! - Inform GPRS module with new second event.*/
#define CTRL_TELL_GPRS_NEW_SEC() gprs_sec_event()
#endif
#ifdef RF_LINK_FEATURE
/*! - Inform RF Link App and module with new second event.*/
#define CTRL_TELL_RF_LINK_NEW_SEC()   RF_Link_event(RF_LINK_EVE_SEC)
#ifdef RF_LINK_TI_CC1120_ENABLE
#define CTRL_TELL_TI_CC1120_NEW_SEC() TI_CC1120_event(TI_CC1120_EVE_SEC)
#endif
#endif
#ifdef TARIFF_SYS
/*! Inform tariff by new minute event.*/
#define CTRL_TELL_TRF_NEW_MIN() trf_pymt_event(TRF_PYMT_EVNT_MIN, 0)
#define CTRL_TELL_TRF_NEW_SEC() trf_pymt_event(TRF_PYMT_EVNT_SEC, 0)
#define CTRL_SET_DATE_TIME_PYMENT()  trf_pymt_event(TRF_PYMT_EVNT_DATE_TIME_CONFIG, 0)

#endif
#ifdef ANSI_STACK
#define CTRL_TELL_ANSI_NEW_MIN
#endif
#define CRTL_NON_VOLTATILE_MEMORY_ERR() Table03_resetFlag_nonVolatileMemoryError()
#define CTRL_TELL_FILE_SYS_NEW_SEC() file_sys_event(FILE_SYS_NEW_SEC)
#define CTRL_TELL_FILE_SYS_NEW_DAY() file_sys_event(FILE_SYS_NEW_DAY)
#define CTRL_TELL_FILE_SYS_POWER_UP() file_sys_event(FILE_SYS_PWR_UP)

#define CTRL_TELL_BT_NEW_SEC()  BlueTooth_Event(NewSec);

/*! Save data in non volatile memory.*/
#define CTRL_SAVE_DATA(adr, data_ptr, size) file_sys_write(data_ptr, adr, size)
/*! Load data from non volatle memory.*/
#define CTRL_LOAD_DATA(adr, data_ptr, size) file_sys_read(data_ptr, adr, size)
/*! Address of configuration data in non volatile memory.*/
#define CTRL_GET_CFG_ADRS() FILE_SYS_GET_CONTROL_CFG_ADRS
#define CTRL_GET_CFG_ADRS_BACKUP() FILE_SYS_GET_CONTROL_CFG_BACKUP_ADRS
#define CTRL_GET_CONTROL_FLGS_ADRS() //FILE_SYS_GET_CONTROL_FLGS_ADRS
#define CTRL_GET_CONTROL_NEW_VAR_ADRS() FILE_SYS_GET_CONTROL_NEW_VAR_ADRS
#define CTRL_CONTROL_NEW_VAR_ADRS_BACKUP() FILE_SYS_CONTROL_NEW_VAR_ADRS_BACKUP
/*! Address of system variables data in non volatile memory.*/
#define CTRL_GET_SYS_VAR_ADRS() FILE_SYS_GET_CONTROL_SYS_VAR_ADRS
#define CTRL_GET_SYS_VAR_ADRS_BACKUP() FILE_SYS_GET_CONTROL_VAR_BACKUP_ADRS
#if (defined(CTRL_MGNT_SENSOR) || (CTRL_BTRY_NON_CHRG == 1))
/*! Initialize ADC.*/
#define CTRL_ADC_INIT()                 S_ADC_Init()
#endif


/*! Initialize RTC.*/
#define CTRL_RTC_INIT(date_time) {S_RTC_init();S_RTC_C_Update(&DateTimeDefault);}
/*! Enable RTC interrupt.*/
#define CTRL_ENABLE_RTC_INT() S_RTC_ENABLE_INT()
/*! Disable RTC interrupt.*/
#define CTRL_DISABLE_RTC_INT() S_RTC_DISABLE_INT()
#ifdef CTRL_CFG_METER_LOG
/*! - Get number of saved configure meter records.*/
#define CTRL_GET_NUM_CFG_REC() file_sys_get_num_record(FILE_SYS_CFG_METER_RECORD)
/*! - Clear configure meter log.*/
#define CTRL_CLEAR_CFG_LOG() file_sys_clear_log(FILE_SYS_CFG_METER_RECORD)
/*! - Save a configure meter log.*/
#define CTRL_SAVE_CFG_REC(data_ptr) file_sys_save_record(FILE_SYS_CFG_METER_RECORD, data_ptr)
/*! - Get a configure meter record.*/
#define CTRL_GET_CFG_REC(index, data_ptr) file_sys_get_record(FILE_SYS_CFG_METER_RECORD, index, data_ptr)
/*! - Get multiple configure meter records.*/
#define CTRL_GET_CFG_MLTI_REC(from, num, data_ptr) file_sys_get_mlti_rec(\
FILE_SYS_CFG_METER_RECORD, from, num, data_ptr)
#endif
/*! Get the mode now. 1 calibration mode, 0 normal mode.*/
#define CTRL_CAL_MODE_STAT() mtr_cal_state()
/*! Tell metering that up switch is pressed in calibration mode.*/
#define CTRL_MTR_UP_SW_PRESSED() mtr_cmd(MTR_NXT_MAN_CAL)
/*! Tell metering that jumper is connected.*/
#define CTRL_TELL_MTR_CALB_JMPR() mtr_cmd(MTR_STRT_MAN_CAL)
/*! Initialize calibration pin as pulled up input.*/
#define CTRL_CALB_PIN_IN()      S_GpioInitCalibPinINPUT() 
/*! Reinitialize calibration pin as output.*/
#define CTRL_CALB_PIN_OUT() S_GpioInit_ALL_Modules(CALIB_JUMBER_PORT, CALIB_JUMBER_PIN, GPIO_Mode_Out_PP_Low_Fast)
/*! Check calibration pin. TRUE if jumper is connected, FALSE if it is not connected.*/
#if !defined (EM110_BOARD) || !defined (EM330_BOARD)|| defined(EM130_BOARD) || defined (EM210_BOARD)
#define CHECK_CALB_PIN() (S_GPIO_Get_Bits(CALIB_JUMBER_PORT,CALIB_JUMBER_PIN)==0)
#else
#define CHECK_CALB_PIN() (S_GPIO_Get_Bits(CALIB_JUMBER_PORT,CALIB_JUMBER_PIN))
#endif
/*! Initialize timer1 counter.*/
#define CTRL_INIT_TIMER1_COUNTER() TIM1_SetCounter(0)
/*! Configure wakeup timer interrupt.*/
#define CTRL_CONFIG_RTC_WAKEUP_TMR(period) rtc_program_wakeup_timer(period)
/*! Calculate day of week.*/
#define CTRL_CALC_DAY_OF_WEEK() bser_calc_day_of_week(ctrl_sys_var_optional_feature.rtc_date.day, \
ctrl_sys_var_optional_feature.rtc_date.month, ctrl_sys_var_optional_feature.rtc_date.year)

/*! Get instantaneous current drawn.*/
#define CTRL_READ_CURRENT() MTR_GET_CUR(0)
#define CTRL_RTC_UPDATE(BUF)  vRTC_Update(BUF)
/*! This function to initialize LCD each minutes.*/
#define CTRL_SPECIAL_DISPLAY_INIT()   Special_Initialize_LCD() 
#define CTRL_WRITE_LAST_TAMPERS(DATA_PTR,SIZE)   file_sys_write(DATA_PTR, FILE_SYS_GET_COM_RFID_DATA_ADRS, SIZE)
#define CTRL_LOAD_LAST_TAMPERS(DATA_PTR,SIZE)    file_sys_read(DATA_PTR, FILE_SYS_GET_COM_RFID_DATA_ADRS, SIZE)
#define CTRL_WRITE_OPTIONAL_FEATURE(DATA_PTR,SIZE)   file_sys_write(DATA_PTR, FILE_SYS_GET_CTRL_VARIABLE_OPTIONAL_DATA_ADRS, SIZE)
#define CTRL_LOAD_OPTIONAL_FEATURE(DATA_PTR,SIZE)    file_sys_read(DATA_PTR, FILE_SYS_GET_CTRL_VARIABLE_OPTIONAL_DATA_ADRS, SIZE)
#define  METER_RESET_OVERLOAD()                   mtr_reset_overload()


/*! Tell file system to save data.*/
#define CTRL_TELL_FILE_SYS_SAVE_DATA() file_sys_event(FILE_SYS_PWR_DN)

#define CTRL_WRITE_BACKUP_OPTIONAL_FEATURE(DATA_PTR,SIZE)   file_sys_write(DATA_PTR, FILE_SYS_GET_BACKUP_CTRL_VARIABLE_OPTIONAL_DATA_ADRS, SIZE)
#define CTRL_LOAD_BACKUP_OPTIONAL_FEATURE(DATA_PTR,SIZE)    file_sys_read(DATA_PTR, FILE_SYS_GET_BACKUP_CTRL_VARIABLE_OPTIONAL_DATA_ADRS, SIZE)

/*! clear of event notification log1.*/
#define CTRL_CLEAR_EVENT_NOTIFICATION_LOG1()  Cal_Evt_gEvtNotiLog1ClearEvent()

#define CTRL_DATA_CRPT_ENA_FLAG   (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_DATA_CRPT_SEND_ENABLE_FLAG )
#define CTRL_HW_ERR_ENA_FLAG        (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_HARDWARE_ERR_SEND_ENABLE_FLAG )
#define CTRL_BTRY_ENA_FLAG           (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_BTRY_LOW_SEND_ENABLE_FLAG )
#define CTRL_CRDT_1ST_ENA_FLAG        (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_LOW_CRDT_1ST_ALRM_SEND_ENABLE_FLAG )            
#define CRTL_CRDT_2ND_ENA_FLAG         (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_LOW_CRDT_2ND_ALRM_SEND_ENABLE_FLAG )
#define CTRL_NO_CRDT_ENA_FLAG         (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_NO_CRDT_SEND_ENABLE_FLAG )           
#define CTRL_NO_FRNDLY_ENA_FLAG       (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_NO_CRDT_FRNDLY_SEND_ENABLE_FLAG )  
#define CTRL_OVER_LOAD_A_ENA_FLAG     (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[0]& CTRL_OVER_LOAD_SEND_ENABLE_FLAG )                
#define CTRL_OVER_VOLT_A_ENA_FLAG     (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_OVER_VOLT_SEND_ENABLE_FLAG )                
#define CTRL_UNDER_VOLT_A_ENA_FLAG    (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_UNDER_VOLT_SEND_ENABLE_FLAG )                
#define CTRL_CVR_OPEN_ENA_FLAG      (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_CVR_OPN_SEND_ENABLE_FLAG )             
#define CTRL_TERMINAL_OPN_ENA_FLAG    (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_TRMNL_OPN_SEND_ENABLE_FLAG )                
#define CTRL_RVR_A_ENA_FLAG           (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_RVRS_CRNT_SEND_ENABLE_FLAG )         
#define FLT_ENRG_ENA_FLAG       (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_FLT_ENRG_SEND_ENABLE_FLAG )              
#define CTRL_MGNT_HI_ENA_FLAG          (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_MGNT_HIGH_SEND_ENABLE_FLAG )
#define CTRL_RELAY_ERR_ENA_FLAG   (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[1]& CTRL_RELAY_ERR_SEND_ENABLE_FLAG )
#define CTRL_GPRS_MODULE_CVR_FLAG   (ctrl_sys_var_second_optional_feature.enable_send_flag_tamper[2]& CTRL_GPRS_MODULE_CVR_SEND_ENABLE_FLAG )


#define CTRL_WRITE_BACKUP_SECOND_OPTIONAL_FEATURE(DATA_PTR,SIZE)   file_sys_write(DATA_PTR, FILE_SYS_GET_BACKUP_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS, SIZE)
#define CTRL_LOAD_BACKUP_SECOND_OPTIONAL_FEATURE(DATA_PTR,SIZE)    file_sys_read(DATA_PTR, FILE_SYS_GET_BACKUP_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS, SIZE)
#define CTRL_WRITE_SECOND_OPTIONAL_FEATURE(DATA_PTR,SIZE)   file_sys_write(DATA_PTR, FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS, SIZE)
#define CTRL_LOAD_SECOND_OPTIONAL_FEATURE(DATA_PTR,SIZE)    file_sys_read(DATA_PTR, FILE_SYS_GET_CTRL_VARIABLE_SECOND_OPTIONAL_DATA_ADRS, SIZE)
#define GPRS_NOTIFY_REPEAT_TIME          1UL //minutes
#define GPRS_SERVER_RESTART_TIMEOUT      5UL //minutes 
#define GPRS_WAIT_ACK_TIMEOUT            1UL //minutes
/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/
#ifdef G2_DM
TBOOL ctrl_check_tampers_flag(uint8_t event)
{
  uint8_t index,shift;
  
  index = (event - CTRL_DATA_CRPT_MIN) / 8;
  shift = (event - CTRL_DATA_CRPT_MIN) % 8;
  
  if(ctrl_sys_new_var.array_tamper_log_flags[index] & (1<<shift))
  {
 
    return TRUE;
    
  }
  else
  {
    return FALSE;     
  }
                   
}
#endif

/* Moved here from meter_init.c (dropped from the DRU build) along with
   vMC_DriversInit. Indicates that data loaded from EEPROM was bad. */
volatile uint16_t eeprom_load_errors_register = 0;

void vMC_DriversInit(void)
{
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('e',5);
#endif

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('t',5);
#endif
#ifdef EM130_BOARD
  S_SPI1_INIT(); //for flash
#endif
#if defined (MTR_THREE_PH) && !defined(MTR_TWO_PH)
  S_SPI1_INIT(); //for flash
#ifndef DRU_PROJECT
  S_SPI2_INIT();  //for metering
#endif
#endif
  S_I2C_INIT(0xa0, EEPROM_I2C);
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('o',5);
#endif

S_ADC_Init();

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('h',5);
#endif
#ifdef KEYPAD_DEBUG
  S_UART_INIT (OPTICAL_UART_PORT_NUMBER ,'5',UART_WORDLEN_8B,UART_PARITY_NONE);
#else
#ifdef GPRS_DEBUG
  S_UART_INIT (OPTICAL_UART_PORT_NUMBER ,'5',UART_WORDLEN_8B,UART_PARITY_NONE);
#else
#ifdef ANSI_STACK
S_UART_INIT (UART4 ,'5',UART_WORDLEN_9B,UART_PARITY_EVEN); 
#else
  S_UART_INIT (OPTICAL_UART_PORT_NUMBER ,'0',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
#if defined RS485_ENABLE
  S_UART_INIT (RS_UART_PORT_NUMBER ,'0',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
#if defined DLMS_UI_RS485 
S_UART_INIT (RS_UART_PORT_NUMBER ,'6',UART_WORDLEN_8B,UART_PARITY_NONE);
#endif
#ifdef HDLC_PROTOCOL
  S_UART_INIT (CONNECTED_METER_MODEM ,'0',UART_WORDLEN_8B,UART_PARITY_EVEN);   //back to RS_UART_PORT_NUMBER
#endif

#ifdef EM210_BOARD                 //two phase board
  S_UART_INIT (V9240_UART_NUMBER ,'6',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
#endif
#endif


#ifdef WIFI_ESP32_ENABLE
S_UART_INIT (S_UART2 ,'7',UART_WORDLEN_8B,UART_PARITY_EVEN);
#endif
  
#ifdef GPRS_FEATURE
  //S_UART_INIT (S_UART0 ,'5',UART_WORDLEN_8B,UART_PARITY_NONE);
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('w',5);
#endif
#ifdef MTR_SINGLE_PH
  if(PMU_GetPowerStatus(PMU_PWRSTS_VDCINDROP) == 0 )
  {
#ifdef V9261f_ENABLE
    #if !defined(EM110_BOARD) && !defined (EM330_BOARD) && !defined(EM130_BOARD) && !defined(EM210_BOARD)
    S_UART_INIT (S_UART3 ,'4',UART_WORDLEN_9B,UART_PARITY_ODD);
    #else
    S_UART_INIT (S_UART2 ,'4',UART_WORDLEN_9B,UART_PARITY_ODD);
    #endif
#else
#if (defined(EM122_BOARD) || defined(EM110_BOARD)||defined (EM330_BOARD)|| defined(EM122U_BOARD))
    S_UART_INIT (S_UART2 ,'4',UART_WORDLEN_9B,UART_PARITY_ODD);
#else
#if (MicroController == Micro_V94XX)
#else
   S_UART_INIT (S_UART3 ,'5',UART_WORDLEN_9B,UART_PARITY_ODD);  
#endif
   
#endif
#endif
  }
#endif
  /* TODO: AHMED HASSAN ... Note that Lcd_init() is called two times ..
  one time in DriverInit and other time in meterinit ... We need to call
  it once */
#ifndef DRU_PROJECT
#ifndef DOT_MATRIX_LCD_ENABLE
  lcd_HQ30774_init( 1, LCD_FRQ_256H , LCD_DRV_150 , LCD_VLCD_0);//lcd_HQ30774_init();
#endif
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('G',0);
#endif
  
}/*end function*/


//=============================================================================
// FUNCTION
//=============================================================================
// Name             : MCU_disable_all
// Description      : This function is used to disable all peripherals in MCU.
// Arguments        : NONE
// Return           : NONE
// Output           : NONE
//-----------------------------------------------------------------------------
void MCU_disable_all(uint8_t I2c_Flag)
{
#ifdef MODEM_ENABLE /*!< Enable using SIM900 GPRS driver.*/
  MODEM_HW_RESET();
#endif
  /*!- Disable the display */
#ifdef DOT_MATRIX_LCD_ENABLE
    Disable_Dot_Matrix_LCD_PWR();
#else
  lcd_HQ30774_cmd(0);
#endif
  /*!- Shutdown buzzer */
  CTRL_BUZZER_OFF();
  DISPLAY_RESET_BACK_LIGHT();

  /* turn off TMPR_LED_PIN and CRDT_LED_PORT  (output pins - logic high) */
  S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
  S_GPIO_Set_Bits(CRDT_LED_PORT, CRDT_LED_PIN);
  S_GpioDisableRfidBoard();
#if (defined(EM122U_BOARD) || defined(EM122_BOARD))
  S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);
#else
  S_GPIO_Clear_Bits(RS485_PORT_OUT, RS485_PIN);
#endif
  /*RmvCodCmntA_K*/
#ifndef MTR_THREE_PH
  S_GPIO_Set_Bits(SW_PORT,SW_SCROLL_UP_PIN|SW_SCROLL_DOWN_PIN);
#endif
 // S_GPIO_Clear_Bits(ENCLOSER_COVER_PORT,ENCLOSER_COVER_PIN);
 // S_GPIO_Clear_Bits(ENCLOSER_SW_PORT,ENCLOSER_SW_PIN);
#if !defined (EM122U_BOARD) && !defined (EM330_BOARD)
  S_GPIO_Set_Bits(CALIB_JUMBER_PORT,CALIB_JUMBER_PIN);
#endif
#ifdef EM122U_BOARD
 // GPIO_InitType GPIO_InitStruct;
  
  S_GpioInit_PIN(Modem_Power_PORT, Modem_Power_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  if(I2c_Flag == 1)
  {
    S_GPIO_Clear_Bits(Modem_Power_PORT,Modem_Power_PIN);
  }
  else
  {
    S_GPIO_Set_Bits(Modem_Power_PORT,Modem_Power_PIN);
  }
  S_GPIO_Clear_Bits(CALIB_JUMBER_PORT,CALIB_JUMBER_PIN);
#endif
  CTRL_WRITE_STOP_RELAY();
  // todo disable gprs and rfmbus pins
  /* force sleep mode for measurement chip (logic high for mre than 4ms*/
#ifdef V9203_ENABLE
  PMU_WakeUpPinConfig(MTR_IRQ_PIN,IOA_DISABLE);
#endif
  /*logic high on WP pin*/
  /*logic low on I2C pins*/
  /* disable peripherals*/
  S_ADC_DISABLE();
  S_UART_DISABLE(S_UART0);
  S_UART_DISABLE(S_UART1);
  S_UART_DISABLE(S_UART2);
  #if (MicroController == Micro_V94XX)            /*Define new Micro*/
  #else
  S_UART_DISABLE(S_UART3);
  #endif
  S_UART_DISABLE(S_UART4);
  S_UART_DISABLE(S_UART5);
  
  if (I2c_Flag == 1)
  {
    S_GPIO_Set_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);    
   //TODOS_I2C_Dis_EN(DISABLE);
   //TODO S_GPIO_Clear_Bits(EEPROM_WP_PORT,EEPROM_WP_PIN);

    #if defined (MTR_THREE_PH) && !defined (EM130_BOARD) && !defined (EM210_BOARD)
    S_GPIO_Set_Bits( MTR_SLEEP_PORT,MTR_SLEEP_PIN);
    #endif
    SPI_Cmd(S_SPI1, DISABLE);

  #if (MicroController == Micro_V94XX)            /*Define new Micro*/
  #else
    SPI_Cmd(S_SPI2, DISABLE);
    S_ISO7816_Disable_ISO78160();     
    S_ISO7816_Disable_ISO78161(); 
  #endif
#if defined(MTR_THREE_PH) && defined(SW_SPI)
    S_GPIO_Clear_Bits(SOFTSPI_WP_PORT,SOFTSPI_WP_PIN);
    S_GPIO_Clear_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN);
    S_GPIO_Clear_Bits(SOFTSPI_MISO_PORT, SOFTSPI_MISO_PIN |SOFTSPI_CS_PIN);  
#else
//#ifdef HW_SPI
  // TODO S_GpioInit_PIN(S_PORT_C,SPI_2_CS |SPI_2_CLK |SPI_2_MISO |SPI_2_MOSI |SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN|SOFTSPI_MISO_PIN|SOFTSPI_CS_PIN ,S_GPIO_Mode_FORBIDDEN );
  //  S_GPIO_Clear_Bits(SOFTSPI_WP_PORT,SOFTSPI_WP_PIN);
   // S_GPIO_Clear_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN|SOFTSPI_MISO_PIN|SOFTSPI_CS_PIN);
   // S_GPIO_Clear_Bits(S_PORT_C, SPI_2_CS |SPI_2_CLK |SPI_2_MISO |SPI_2_MOSI );

//#else

   // S_GPIO_Clear_Bits(SOFTSPI_WP_PORT,SOFTSPI_WP_PIN);
  //  S_GPIO_Set_Bits(SOFTSPI_MOSI_PORT, SOFTSPI_MOSI_PIN|SOFTSPI_CLK_PIN|SOFTSPI_MISO_PIN|SOFTSPI_CS_PIN);
    //S_GPIO_Clear_Bits(S_PORT_C, SPI_2_CS |SPI_2_CLK |SPI_2_MISO |SPI_2_MOSI );

//#endif
    
#endif
    
  }  
}

/*!
* @brief Reset control configuration
*
* @par Description:
* Return the configuration to its defaults and save it in EEPROM.
*
* @return
* - None.
*/
void ctrl_reset_cfg(uint8_t save)
{
  /*!@par Pseudo Code:*/
  /*! - If the configuration can't be loaded or the CRC failed, load initial values.*/
  memset((uint8_t*)&ctrl_cfg.id, 0, sizeof(ctrl_cfg_t) - sizeof(ser_t));
  
#ifndef ANSI_STACK 
#if (defined(PAYMENT_SYS) && defined(CTRL_RLY))
  ctrl_cfg.tmpr.non_latch_action[CTRL_NO_CRDT] = ACT_OPN_RLY_MASK;
#endif
#endif//ANSI_STACK 
  
#ifdef CTRL_RTC
  ctrl_sys_var_optional_feature.rtc_date.day = 1;
  ctrl_sys_var_optional_feature.rtc_date.month = 1;
  ctrl_sys_var_optional_feature.rtc_date.year = 13;
  ctrl_sys_var_optional_feature.rtc_date.day_of_week = 2;
  /* - Set RTC with default date and time.*/
  ctrl_cfg_set(CTRL_CFG_TIME, (uint8_t*)&ctrl_sys_var_optional_feature.rtc_time);//ismail it will get the same value?!
  ctrl_cfg_set(CTRL_CFG_DATE, (uint8_t*)&ctrl_sys_var_optional_feature.rtc_date);
#endif
#if (CTRL_BTRY_NON_CHRG == 1)
  ctrl_cfg.btry.high_lvl = BTRY_HIGH_LVL_INIT;
  ctrl_cfg.btry.low_lvl = BTRY_LOW_LVL_INIT;
#endif
#if (CTRL_MGNT_SENSOR == 1)
  ctrl_cfg.mgnt.high_lvl = MGNT_HIGH_LVL_INIT;
#endif
#if (CTRL_MGNT_SW == 1)
  ctrl_cfg.mgnt.detect_time = 5;
  ctrl_cfg.mgnt.restore_time = 2;
#endif
  
  //if(!(eeprom_load_errors_register&CTRL_ERR_LOAD_FLAG))
  {
    /*! - Calculate new CRC.*/
    eeprom_load_errors_register =0;
    if (save == 1)
    {
    	save_ctrl_cfg_flg = 1;
    }
  }
}


/*!
* @brief Open the relay.
*
* @par Description:
* Open the relay.
*
* @return
* - None.
*/
void lpm_new_sec(void)
{
  /*!@par Pseudo Code:*/
  /*!increment power up timer.*/
  if(~lpm_pwr_up_sec_cntr)
    lpm_pwr_up_sec_cntr++;
}

void open_relay(void)
{
  //#ifdef MTR_THREE_PH
  mtr_rst_mtr_start_accuracy_flag();
  ctrl_sys_var_optional_feature.flag1 &= ~OPEN_RELAY_MODE;                                                                                                               
  customer_flag = 0;
    /*! - Check if the action is to open the relay, clear the hint flag.*/
  if(!(CTRL_RLY_STAT))
  {
    /*! - Open relay.*/
    toggle_relay();
  }
}

/*!
* @brief Close the relay.
*
* @par Description:
* Close the relay.
*
* @return
* - None.
*/
void close_relay(void)
{
  mtr_rst_mtr_start_accuracy_flag();
  ctrl_sys_var_optional_feature.flag1 &= ~OPEN_RELAY_MODE;                                                                                                               
  customer_flag = 0;
  /*! - Check if the action is to open the relay, clear the hint flag.*/
  if((CTRL_RLY_STAT))
  {
    /*! - close relay.*/
    toggle_relay();
  }
}

/*!
* @brief Update system variable and flags.
*
* @par Description:
* This function is used to update system variable and flags for events happened in low power mode.
*
* @return
* - None.
*/
void ctrl_update_sys_var(uint8_t event)
{
  uint8_t element,shift,index;
  element = (event - CTRL_DATA_CRPT_MIN) / 8;
  shift = (event - CTRL_DATA_CRPT_MIN) % 8;
  /*! - Set new tampers flag.*/
  ctrl_sys_var.flags |= CTRL_GPRS_NEW_TMPR_FLAG;
  ctrl_sys_var.flags |= CTRL_RFID_NEW_TMPR_FLAG;
  /*! - Modify meter status.*/
  /*! - If the event is start, set its coreesponding flag in the meter
  * status and in the shadow status.*/
  // ctrl_sys_var.basic.meter_status[element] |= (1 << shift);
  ctrl_sys_var.shadow_status[element] |= (1 << shift);
  /*! - Get the index of alarms and tampers counter array of the new tamper
  * or alarm.*/
  index = match_array[event - CTRL_DATA_CRPT_MIN];
  /*! - Increment counter of the new tamper or alarm.*/
  if((*(ctrl_sys_var.basic.non_latch_evnt_count + index))!= 0xff)
    (*(ctrl_sys_var.basic.non_latch_evnt_count + index))++;
  if(ctrl_sys_var.new_evnt_counter != 0xffff) ctrl_sys_var.new_evnt_counter++;
}
#ifdef CTRL_RLY
void test_toggle_relay()
{
#ifdef MTR_THREE_PH
  uint32 loop_index = 6*14563;//40ms//1*14563===6.75m
#else
  uint32 loop_index = 3*14563;//30 msec
#endif
  
  S_GPIO_Set_Bits(RELAY_PORT, RELAY_DISCONNECT);
  while((loop_index != 0) && (!CTRL_PWR_DN_STAT))
    loop_index--;
  
  {S_GPIO_Clear_Bits(RELAY_PORT, RELAY_CONNECT); S_GPIO_Clear_Bits(RELAY_PORT, RELAY_DISCONNECT);}
  
  S_GPIO_Clear_Bits(TMPR_LED_PORT,TMPR_LED_PIN);
  
#ifdef MTR_THREE_PH
  loop_index = 3*14563;//20ms//1*14563===6.75m
#else
  loop_index = 3*14563;//30 msec
#endif
  
  
  S_GPIO_Set_Bits(RELAY_PORT, RELAY_CONNECT);
  while((loop_index != 0) && (!CTRL_PWR_DN_STAT))
    loop_index--;
  
  {S_GPIO_Clear_Bits(RELAY_PORT, RELAY_CONNECT); S_GPIO_Clear_Bits(RELAY_PORT, RELAY_DISCONNECT);}
  
  S_GPIO_Set_Bits(TMPR_LED_PORT,TMPR_LED_PIN);    
}
/*!
* @brief Toggle the relay.
*
* @par Description:
* Toggle the relay.
*
* @return
* - None.
*/
void toggle_relay(void)
{
#if 0 
  if((mtr_get_mtr_start_accuracy_flag()))
  {
    if(!CTRL_RLY_STAT)
      return;
  }  
#ifndef  MTR_ACCRA_NEW
  /*!@par Pseudo Code:*/
  uint32_t loop_index;
  /*! - Check the status of relay to toggle.*/
  /*! - If the relay is open and it will be closed.*/
  if(CTRL_RLY_STAT)
  {
    /*! - If you are in open ctrl_sys_var_optional_feature.flag1 relay mode, return witout closing the relay.*/
    if((ctrl_sys_var_optional_feature.flag1 & OPEN_RELAY_MODE)||(customer_flag!=0))//||(ctrl_flags & CTRL_RELAY_HINT_MASK)||( ctrl_sys_var_optional_feature.flag1 & NEW_HINT_RLY_ACT_FLAG))
      if(!(mtr_get_mtr_start_accuracy_flag()))
        return;
    CTRL_WRITE_RELAY_CLOSE();
    
    
    if (check_relay_save_event == 0)
      check_relay_save_event = 1; //close relay
  }
  else
  {
    CTRL_WRITE_RELAY_OPEN();
  }
  
  
  if (check_relay_save_event == 0)
    check_relay_save_event = 2; //open relay
  /*! - Output the proper pulse to toggle relay.*/
  /*! - Delay 100msec (pulse time of the relay) but check for power down.*/
#ifdef MTR_THREE_PH
  loop_index = 15*14563;    //    1*14563===6.75m  //15*14563=100ms   6*14563=40ms
#else
  loop_index = 6*14563;//30 msec
#endif
  while((loop_index != 0) && (!CTRL_PWR_DN_STAT))
    loop_index--;
  /*! - Stop the pulse of relay.*/
  CTRL_WRITE_STOP_RELAY();
  
  if (check_relay_save_event == 1)
    ctrl_event(CTRL_RELAY_DISCONNECTED);
  else if (check_relay_save_event == 2)
    ctrl_event(CTRL_RELAY_CONNECTED);
  /*! - Toggle the relay status.*/
  ctrl_sys_var.basic.meter_status[6] ^= CTRL_RLY_MASK;
  check_relay_save_event = 0;  

#endif
#endif
}
#endif
#ifdef CTRL_MD_SW
/*!
* @brief Check the MD switch state.
*
* @par Description:
* Check the MD switch state.
*
* @return
* - TRUE switch is pressed, FALSE switch is not pressed.
*/
uint8_t check_md_switch(void)
{
  /*!@par Pseudo Code:*/
  static uint8_t md_sw_state;
  /*! - Record state of MD switch.*/
  md_sw_state = (md_sw_state << 1);
  if(READ_MD_SW_PIN() == MD_SW_PRESS_STAT)
    md_sw_state++;
  /*! - Mask the bits of concern (last #NUMBER_SWITCH_CHECK readings) of the switch.*/
  md_sw_state &= SWITCH_CHECK_TIMES_FLAG;
  /*! - Check if the last #NUMBER_SWITCH_CHECK reading of MD switch is the pressed
  and it is the first time to detect the press.*/
  if((md_sw_state == SWITCH_CHECK_TIMES_FLAG) && (!( ctrl_sys_var_optional_feature.flag1 & MD_STAT_MASK)))
  {
    /*! - Set the MD switch pressed flag.*/
    ctrl_sys_var_optional_feature.flag1 |= MD_STAT_MASK;
    return TRUE;
  }
  /*! - Check if the switch state is released, clear the flag.*/
  if(md_sw_state == ((~SWITCH_CHECK_TIMES_FLAG) & SWITCH_CHECK_TIMES_FLAG))
    ctrl_sys_var_optional_feature.flag1 &= (~MD_STAT_MASK);
  return FALSE;
}
#endif

#ifdef CTRL_BZR
/*!
* @brief Turn the buzzer ON.
*
* @par Description:
* Turn the buzzer ON despite you are in the silent period, change its state and
* initialize blind period.
*
* @return
* - None.
*/
void buzzer_on(void)
{
#ifdef AC_BUZZER_FEATURE
  /*!@par Pseudo Code:*/
  if(!ctrl_sys_var.bzr_silent_sec_cntr)
  {
    if(!(ctrl_sys_var.basic.meter_status[6] & CTRL_BZR_STAT_MASK))
      /*! - Initialize hint blind period to avoid removing hint despite blind period
      * elapsed.*/
      blind_hint_period_sec_counter = HINT_BLIND_PERIOD;
  
    CTRL_BUZZER_ON();
#if 0 //Beep_Beep_Feature  , make long tone with card issue
    if(beep_counter ==2)         //to solve the silent condtion 
    { beep_counter= 0;}
   if((++Global_switch_Buzzer >= 60))
    {
     beep_counter = 20;
     xc =6;                         //new tone
    CTRL_BUZZER_ON();
    Global_switch_Buzzer =0;
     
    }
    else if(beep_counter ==0)
    {
      Global_switch_Buzzer =0;
      CTRL_BUZZER_OFF();
    }
    else
    {}
#endif
   /*! - Change meter status.*/
    ctrl_sys_var.basic.meter_status[6] |= CTRL_BZR_STAT_MASK;
  }
#endif
}

/*!
* @brief Turn the buzzer OFF.
*
* @par Description:
* Turn the buzzer OFF, change its state and initialize silent period.
*
* @return
* - None.
*/
void buzzer_off(void)
{
  /*!@par Pseudo Code:*/
  /*! - If the buzzer state is ON.*/
  if(ctrl_sys_var.basic.meter_status[6] & CTRL_BZR_STAT_MASK)
  {
    /*! - Initialize buzzer silent period counter.*/
#ifdef G2_DM
    uint8_t temp_hr=S_RTC_C_GetRTCHOUR();
    uint8_t temp_min=S_RTC_C_GetRTCMIN();
    uint8_t bzr_status_flag=0;
    if(ctrl_sys_var_second_optional_feature.start_bzr_silent_period_hr <= ctrl_sys_var_second_optional_feature.end_bzr_silent_period_hr)
    {
      if((temp_hr >= ctrl_sys_var_second_optional_feature.start_bzr_silent_period_hr && temp_hr < ctrl_sys_var_second_optional_feature.end_bzr_silent_period_hr)&&\
        (temp_min >= ctrl_sys_var_second_optional_feature.start_bzr_silent_period_min && temp_min < ctrl_sys_var_second_optional_feature.end_bzr_silent_period_min))
      {
        bzr_status_flag=1;
      }
    }
    else
    {
      if((temp_hr < ctrl_sys_var_second_optional_feature.end_bzr_silent_period_hr || temp_hr >= ctrl_sys_var_second_optional_feature.start_bzr_silent_period_hr)&&\
        (temp_min < ctrl_sys_var_second_optional_feature.end_bzr_silent_period_min || temp_min >= ctrl_sys_var_second_optional_feature.start_bzr_silent_period_min))
        {
          bzr_status_flag=1;
        }
    }
    if(bzr_status_flag)
    {
      CTRL_BUZZER_OFF();
      ctrl_sys_var.basic.meter_status[6] &= (~CTRL_BZR_STAT_MASK);
    }
  }
#else
    if(!(ctrl_sys_var.bzr_silent_sec_cntr = (ctrl_cfg.tmpr.bzr_silent_period * 60)))
      ctrl_sys_var.bzr_silent_sec_cntr = (~0);
  }
  /*! - Turn buzzer off and modify meter status.*/
  CTRL_BUZZER_OFF();
  ctrl_sys_var.basic.meter_status[6] &= (~CTRL_BZR_STAT_MASK);
#endif
}
#endif


/*!
* @brief Forms the action to apply due to tampers and alarms
*
* @par Description:
* Forms the action to apply from the different actions of all latched and non
* latched events and alarms.
*
* @return
* - None.
*/
void form_action(uint8_t action)
{
#if 0 
  /*!@par Pseudo Code:*/
  uint8_t led_flag;
  /*! - OR all actions.*/
  formed_action |= action;
  /*! - For LEDs, get action to be applied on the LED and change the meter status.*/
  led_flag = action & ACT_TMPR_LED_MASK;
  /*! Check tamper LED to be ON or OFF or Flashing.*/
  if(led_flag == ACT_TMPR_LED_ON)
    ctrl_sys_var.basic.meter_status[6] |= CTRL_TMPR_LED_ON;
  if(led_flag == ACT_TMPR_LED_FLASH)
    ctrl_sys_var.basic.meter_status[6] |= CTRL_TMPR_LED_FLASHING;
  led_flag = action & ACT_CRDT_LED_MASK;
  /*! Check tamper LED to be ON or OFF or Flashing.*/
  if(led_flag == ACT_CRDT_LED_ON)
    ctrl_sys_var.basic.meter_status[6] |= CTRL_CRDT_LED_ON;
  if(led_flag == ACT_CRDT_LED_FLASH)
    ctrl_sys_var.basic.meter_status[6] |= CTRL_CRDT_LED_FLASHING;
#endif
}

/*!
* @brief Deduce actions to apply from tampers and alarms.
*
* @par Description:
* Deduce actions to apply from latched and non latched tampers and alarms.
*
* @return
* - None.
*/
void deduce_actions(void)
{
  /*!@par Pseudo Code:*/
  uint8_t index;
  uint8_t tmp_action;
  uint8_t tmp_bat;
  formed_action = 0;
  formed_action_2 = 0;
#ifdef ANSI_STACK
  uint8_t action2_tmp = 0;
  uint8_t action2 = 0;
#endif
     tmp_bat = ctrl_cfg.tmpr.non_latch_action[CTRL_BTRY_LOW];
     if(ctrl_sys_new_var.ctr_no_bat_flage == 0)
     {
       ctrl_cfg.tmpr.non_latch_action[CTRL_BTRY_LOW] = 0;
     }

#if (defined(CTRL_TMPR_LED) || defined(CTRL_LOW_CRDT_LED))
  /*! - Clear LEDs status bits.*/
  ctrl_sys_var.basic.meter_status[6] &= (~CTRL_LEDS_MASK);
#endif
  /*! - Check tampers and alarms.*/
  for(index = 0; index < NO_TMPRS_ALRMS_BITS; index++)
  {
    /*! - Form action due to existing tampers and alarms.*/
      if((ctrl_sys_var.basic.meter_status[index / 8] & (1 << (index % 8)))&& index < 46)   // index 46 for CTRL_MAXIMUM_CONSUMPTION_MASK
      {
#if defined (CTRL_GPRS_MODULE_CVR_SW) && defined (MTR_SINGLE_PH)
        if(index != 45) //for GPRS_SW_action
#endif
        {
          if(ctrl_no_cdt_no_tmpr() && ctrl_sys_new_var.grace_state == 0x03 && index == 20)
            form_action((*(ctrl_cfg.tmpr.non_latch_action + match_array[index]))& ~ACT_OPN_RLY_MASK);
          else
            form_action(*(ctrl_cfg.tmpr.non_latch_action + match_array[index]));
#ifdef ANSI_STACK         
         if((formed_action)&(ACT_OPN_RLY_MASK))
         {
           if(opened_relay_code == 0xffff)
           {
             if(index < 8)
             {
               opened_relay_code = (1 << RLY_REASON_EEPROM_FAIL);
             }
             else if((index < 27)&&(index >= 24))
             {
               opened_relay_code = (1 << RLY_REASON_OVER_VOLTAGE);               
             }
             else if((index < 30)&&(index >= 27))
             {
               opened_relay_code = (1 << RLY_REASON_UNDER_VOLTAGE);
             }
             else if((index < 39)&&(index >= 36))
             {
               opened_relay_code = (1 << RLY_REASON_ALARMS_REVERSE);     
             }
           }
         }
#endif
      }
      }
  }
#ifdef ANSI_STACK
  /*** check actions of remaining ansi alarms */
for(index = 0; index < CTRL_ANSI_ALARMS_NUM; index++)
 { 
    if((ctrl_sys_var_second_optional_feature.ansi_remaining_alarms )&(1 << index))
    {
     if(index == 0 || index > 5)
     {
     action2 = Ansi_get_Action_prepaid(index,ANSI_ALARM);
     form_action(action2);
     }
     else
     {
     action2_tmp = ctrl_sys_var_second_optional_feature.ansi_remaining_actions[index]; 
     formed_action_2 |= check_value(action2_tmp & ALARMS_DISPLAY_CONTROL) << CTRL_DISPLAY_CONTROL;
     formed_action_2 |= check_value(action2_tmp & ALARMS_RELAY_DISCONNECT_DISPLAY_CONTROL) << CTRL_RELAY_DISCONNECT_DISPLAY_CONTROL; 
     }
     
    } 
 }
 /*** check actions of remaining ansi fatal alarms */
 for(index = 0; index < CTRL_NUM_OF_CODE_NEW; index++)
 { 
    if((ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms )&(1 << index))
    {
     action2 = Ansi_get_Action_prepaid(index,FATAL_ALARM);
     if(index != CTRL_EEPROM_FAIL)
     form_action(action2);
    } 
 }
#endif

#ifdef G2_DM
  if (ctrl_sys_var_optional_feature.meter_status_3rd_alarm & (1<<LOW_CRDT_3RD_ALRM_BIT) )
  {form_action(Apply_Action_LOW_CRDT_3RD_ALRM_START);}
#endif
  if (ctrl_sys_var_optional_feature.ctrl_flags & CTRL_MAXIMUM_CONSUMPTION)
  {
    form_action(ctrl_sys_var_optional_feature.ctrl_maximum_consumptions_action);
  }
  if (ctrl_sys_var_optional_feature.ctrl_consumption_alarm_status && ctrl_sys_var_optional_feature.ctrl_consumption_alarm_status < 4)
  {
    tmp_action = ctrl_sys_var_optional_feature.ctrl_consumption_alarm_action;
#if 0
    if (tmp_action == 0)
    {
      tmp_action = ACT_BZR_ON | ACT_TMPR_LED_ON;
    }
#endif
    if (ctrl_sys_var_optional_feature.ctrl_consumption_alarm_status == 3)
      tmp_action &=  ~(ACT_BZR_ON) ;  // hint relay and open relay are disabled by SW
    
    form_action(tmp_action);
    
    ctrl_sys_var_optional_feature.ctrl_consumption_alarm_status = 2;
  }
  /*! - Check latched tampers or alarms.*/
  for(index = 0; index < CTRL_LTCH_ACT_EVNT_NUM; index++)
  {
    tmp_action = ctrl_cfg.tmpr.latch_action[index];
    if (!((tmp_action & CTRL_PENALTY) && ctrl_check_status(index)))
    {
      tmp_action  &= ~CTRL_PENALTY;
    }
    /*! - Form action due to latched tampers or alarms that occurred before.
    @note Exclude hint by relay action due to latched events happened and stopped before.*/
    
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
    if(index != CTRL_MGNT_HIGH)
    {
      if(ctrl_sys_var.basic.latch_evnt_count[index])
        form_action(tmp_action & (~ACT_HINT_BY_RLY));
    }
    else
    {
      if(ctrl_sys_var.basic.meter_status[4] & (0x08))
        form_action(tmp_action & (~ACT_HINT_BY_RLY));
    }
#else
    if(ctrl_sys_var.basic.latch_evnt_count[index])
      form_action(tmp_action & (~ACT_HINT_BY_RLY));
#endif
  }
#if defined (CTRL_GPRS_MODULE_CVR_SW) && defined (MTR_SINGLE_PH)
  if(CTRL_GPRS_MODULE_CVR_OPN_OCCURED)
  {
     form_action(ctrl_cfg.tmpr.latch_action[CTRL_CVR_OPN]);
  }
#endif
  ctrl_cfg.tmpr.non_latch_action[CTRL_BTRY_LOW] = tmp_bat;
}

/*!
* @brief Apply actions deduced.
*
* @par Description:
* Apply the actions deduced from tampers and alarms status. Actions that may be
* applied are (Tamper LED ON or flash, Credit LED on or flash, alarm icon, open or hint by
* relay), Turn buzzer on, go to penalty mode).
*
* @return
* - None.
*/

void apply_actions(void)
{

  //static uint8_t No_wait=0;
#ifdef CTRL_RLY
  volatile uint8_t relay_state;
#endif
  /*!@par Pseudo Code:*/
  sr_ptrn &= 0x0F;
#ifdef CTRL_TMPR_LED
  /*! - Tamper LED Actions.*/
  /*! - If the action formed is turn tamper LED ON.*/
  if(ctrl_sys_var.basic.meter_status[6] & CTRL_TMPR_LED_ON)
  {
    /*! - Clear flash tamper LED bit.*/
    ctrl_sys_var.basic.meter_status[6] &= (~CTRL_TMPR_LED_FLASHING);
    /*! - Modify pattern sent to shift register.*/
    CTRL_WRITE_TMP_LED_ON();
  }
  if(timeout_cntr > 1)
  {
    /*! - If the action formed is flash tamper LED, flash LED every second.*/
    if(ctrl_sys_var.basic.meter_status[6] & CTRL_TMPR_LED_FLASHING)
    {
#ifdef CTRL_TMPR_LED
      /*! - Invert Tamper LED bits as it is connected as sink.*/
      CTRL_WRITE_TMP_LED_TOGGLE();
#endif
    }
    /*! - If the action formed is flash credit LED, flash LED every second.*/
    if(ctrl_sys_var.basic.meter_status[6] & CTRL_CRDT_LED_FLASHING)
    {
#ifdef CTRL_LOW_CRDT_LED
      CTRL_WRITE_LOW_CRDT_LED_TOGGLE();
#endif
    }
    timeout_cntr = 0;
  }
  if(!(ctrl_sys_var.basic.meter_status[6] & (CTRL_TMPR_LED_ON | CTRL_TMPR_LED_FLASHING)))
  {
#ifdef CTRL_TMPR_LED
    /*! - Invert Tamper LED bits as it is connected as sink.*/
    CTRL_WRITE_TMP_LED_OFF();
#endif
  }
#endif
#ifdef CTRL_LOW_CRDT_LED
  /*! - Low credit LED Actions.*/
  /*! - If the action formed is turn credit LED ON.*/
  if(ctrl_sys_var.basic.meter_status[6] & CTRL_CRDT_LED_ON)
  {
    /*! - Clear flash tamper LED bit.*/
    ctrl_sys_var.basic.meter_status[6] &= (~CTRL_CRDT_LED_FLASHING);
    /*! - Modify pattern sent to shift register.*/
    CTRL_WRITE_LOW_CRDT_ON();
  }
  if(!(ctrl_sys_var.basic.meter_status[6] & (CTRL_CRDT_LED_ON | CTRL_CRDT_LED_FLASHING)))
  {
#ifdef CTRL_LOW_CRDT_LED
    CTRL_WRITE_LOW_CRDT_OFF();
#endif
  }
#endif
#ifdef ANSI_STACK 
if(formed_action_2 & (1 << CTRL_DISPLAY_CONTROL))
{
  display_control_flag = 1; //raise flag to display screen of alarm
}
if(formed_action_2 & (1 << CTRL_RELAY_DISCONNECT_DISPLAY_CONTROL))  
{
  display_relay_disc_flag = 1; //raise flag to display screen of relay disc reason
}  
#endif
#ifdef CTRL_ALRM_ICON
  /*! - Check action to be apllied to on LCD alarm icon.*/
  if(formed_action & ACT_ALRM_ICON)
    ctrl_sys_var.basic.meter_status[6] |= CTRL_ALRM_ICN_MASK;
  else
    ctrl_sys_var.basic.meter_status[6] &= (~CTRL_ALRM_ICN_MASK);
#endif
#ifdef CTRL_BZR
  if(beep_counter == 1)
  {
    /*! - Check if the period of turning buzzer on by command has finished, turn the buzzer off.*/
    beep_counter = 0;
    CTRL_BUZZER_OFF();
    ctrl_sys_var.basic.meter_status[6] &= (~CTRL_BZR_STAT_MASK);
  }
   else if (beep_counter && (!(formed_action & ACT_BZR_ON) || (ctrl_sys_var.bzr_silent_sec_cntr == 0xFFFF) ))
  {
    /*! - If the buzzer should be ON due to command BEEP, open it.*/
    CTRL_BUZZER_ON();
    ctrl_sys_var.basic.meter_status[6] |= CTRL_BZR_STAT_MASK;
  }
#if Beep_Beep_Feature
    else if((formed_action & ACT_BZR_ON) && !(beep_beep_Counter))      // beep_beep_Counter  to avoid going in this condtion when bad card occur
#else
/*! - Check formed action to be applied on the buzzer.*/
      else if(formed_action & ACT_BZR_ON)
#endif
  {
    /*! - Initialize the buzzer with its default tone.*/
    CTRL_BUZZER_INIT(DFLT_BUZ_TONE);
    buzzer_on();
  }
  else
    buzzer_off();
#endif
#ifdef CTRL_RLY
#ifndef MTR_ACCRA_NEW  
  /*! - Relay Actions.*/
  relay_state = CTRL_RLY_STAT;
  /*! - Check if the action is to open the relay, clear the hint flag.*/
  if(((formed_action & ACT_OPN_RLY_MASK) || ( ctrl_sys_var_optional_feature.flag1 & OPEN_RELAY_MODE)) && (!relay_state))
  {
    /*! - Open relay.*/
    toggle_relay();
#ifdef ANSI_STACK
    if (ctrl_sys_var_second_optional_feature.relay_disconnect_reason == 0xffff)
    {        
      ctrl_sys_var_second_optional_feature.relay_disconnect_reason = opened_relay_code;
    }  
#endif  
#ifdef MTR_ENABLE_LMT
    if(ctrl_sys_var_optional_feature.flag1 & OVER_LOAD_MODE)
    {
      /*! - Clear over load tamper second counter (used with special handling of
      * overload alarm).*/
      ctrl_sys_new_var.ovr_load_sec_cntr = 0;
      check_and_reset_ov_load_cntr(SEQ_NORMAL);
      ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
    }
#endif
  }
  /*! - Check if there a new action hint by relay and the relay is closed.*/
  else if((!(formed_action & ACT_OPN_RLY_MASK) && ( ctrl_sys_var_optional_feature.flag1 & NEW_HINT_RLY_ACT_FLAG)))
  {
    /*! - Reset new hint by relay flag.*/
    ctrl_sys_var_optional_feature.flag1 &= (~NEW_HINT_RLY_ACT_FLAG);
    /*! - Set existing hint flag and initialize blind period counter.*/
    CTRL_SET_HINT_RLY();
    blind_hint_period_sec_counter = HINT_BLIND_PERIOD;
    if(!(relay_state))//&&(!( ctrl_sys_var_optional_feature.flag1 & OVER_LOAD_MODE)))
      /*! - Open the relay.*/
      toggle_relay();
#ifdef MTR_ENABLE_LMT
    if( ctrl_sys_var_optional_feature.flag1 & OVER_LOAD_MODE)
    {
      /*! - Clear over load tamper second counter (used with special handling of
      * overload alarm).*/
      ctrl_sys_var.basic.meter_status[2]&=~CTRL_OVR_LOAD_CHA_MASK;
      ctrl_sys_var.basic.meter_status[2]&=~CTRL_OVR_LOAD_CHB_MASK;
      ctrl_sys_var.basic.meter_status[2]&=~CTRL_OVR_LOAD_CHC_MASK;
      check_and_reset_ov_load_cntr(SEQ_NORMAL);
      ctrl_sys_new_var.ovr_load_sec_cntr = 0;
      ctrl_sys_var_optional_feature.flag1&=~OVER_LOAD_MODE;
      
    }
#endif
  }
  /*! - If there is no action to open the relay and the relay is open.*/
  //else if((CTRL_OVR_LOAD_CHA_STAT || CTRL_OVR_LOAD_CHB_STAT || CTRL_OVR_LOAD_CHC_STAT) && (ctrl_cfg.tmpr.non_latch_action[CTRL_OVER_LOAD] & ACT_OPN_RLY_MASK))
  else if((ctrl_sys_var_optional_feature.flag1 & OVER_LOAD_MODE) && (ctrl_cfg.tmpr.non_latch_action[CTRL_OVER_LOAD] & ACT_OPN_RLY_MASK))
  {
#ifdef MTR_ENABLE_LMT
    if(((ctrl_sys_new_var.ovr_load_times_cntr < ctrl_cfg.overload.persistence_times) || (!ctrl_cfg.overload.persistence_times)) && \
      (!(ctrl_sys_new_var.ovr_load_times_cntr && ((ctrl_sys_new_var.ovr_load_sec_cntr < (ctrl_cfg.overload.open_relay_time * 60) && relay_state) || (ctrl_sys_new_var.ovr_load_sec_cntr < (ctrl_cfg.overload.wait_time * 60) && !relay_state) ))))
#endif
    {
      if (ctrl_sys_new_var.ovr_load_times_cntr+1 >= ctrl_cfg.overload.persistence_times)
      {
        if(ctrl_cfg.overload.persistence_times  && !relay_state)
        {
          ctrl_sys_new_var.ovr_load_times_cntr++;
          if (channel_cpy == 0 || channel_cpy == 1)
            ctrl_event(CTRL_OVR_LOAD_A_STRT);
          else if (channel_cpy == 2)
            ctrl_event(CTRL_OVR_LOAD_B_STRT);
          else 
            ctrl_event(CTRL_OVR_LOAD_C_STRT);
          
          /* if ((MTR_GET_CUR(channel_cpy)/1000) > ctrl_sys_new_var.overload_current_value) */  /* metering not built for the DRU */
          /* ctrl_sys_new_var.overload_current_value = MTR_GET_CUR(channel_cpy)/1000; */  /* metering not built for the DRU */
          ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);       
        } 
        else
        {
          toggle_relay();
#ifdef MTR_THREE_PH
          /* mtr_cmd(MTR_RST_OVR_LD_COUNTERS); */  /* metering not built for the DRU */
#endif
        }
      }
      else
      {
        toggle_relay();
#ifdef MTR_THREE_PH
        /* mtr_cmd(MTR_RST_OVR_LD_COUNTERS); */  /* metering not built for the DRU */
#endif
        if(!relay_state)
        {
          /* if ((MTR_GET_CUR(channel_cpy)/1000) > ctrl_sys_new_var.overload_current_value) */  /* metering not built for the DRU */
            /* ctrl_sys_new_var.overload_current_value = MTR_GET_CUR(channel_cpy)/1000; */  /* metering not built for the DRU */
          ctrl_sys_new_var.ovr_load_times_cntr++;
        }
      }
#ifdef MTR_ENABLE_LMT
      /*! - Clear over load tamper second counter (used with special handling of
      * overload alarm).*/
      ctrl_sys_new_var.ovr_load_sec_cntr = 0;
#endif
    }
  }
  
#endif
#ifdef G2_DM
  else if (saving_days_status==1) //this is for open relay in saving days.
  {
    if(relay_state==0) //if realy is closed open it.
    {
      /*! - open the relay.*/
      toggle_relay();
    }
  }
#endif
  /*! - If there is no action to open the relay and the relay is open.*/
  /* rfid_check_idle_state() dropped: RFID is not built for the DRU, so the
     reader is permanently idle and this term is always true. */
  else if((!(formed_action & ACT_OPN_RLY_MASK)) && (relay_state) && \
    !(ctrl_sys_var_optional_feature.ctrl_flags & CTRL_RELAY_HINT_MASK)&&\
      !( ctrl_sys_var_optional_feature.flag1 & NEW_HINT_RLY_ACT_FLAG)&&\
        !( ctrl_sys_var_optional_feature.flag1 & OPEN_RELAY_MODE)&&\
          !(ctrl_sys_var.flags & NEW_HINT_RLY_ACT_FLAG))
  {
    /*! - Close the relay.*/
    toggle_relay();
    //Disconnector_Method_LocalReconnect();
#ifdef ANSI_STACK
     ctrl_sys_var_second_optional_feature.relay_disconnect_reason = 0xffff;
     opened_relay_code = 0xffff;
     display_relay_disc_flag = 0;
     display_control_flag = 0;
#endif
  }
  /*! - Check if the formed action due to current tampers and alarms contains go
  to penalty mode.*/
  if(formed_action & ACT_GO_PNLTY_MODE)
  {
    /* CTRL_ENTER_PENALTY_MODE(); */  /* metering not built for the DRU */
    if(penalty_tmp == 0) 
    {
      ctrl_event(CTRL_ENTER_PENALETY_MODE);
      penalty_tmp = 1;
    }
  }
  else
  {
    /* CTRL_EXIT_PENALTY_MODE(); */  /* metering not built for the DRU */
    if(penalty_tmp == 1) 
    {
      ctrl_event(CTRL_EXIT_PENALETY_MODE);
      penalty_tmp = 0;
    }
  }
}
#endif
//#endif
#if (defined(CTRL_BZR) || defined(CTRL_RLY))
/*!
* @brief Remove hint of relay or buzzer or both if any exists.
*
* @par Description:
* Remove hint of relay or buzzer or both if any exists.
*
* @return
* - TRUE there is at least one hint removed, FALSE no hint removed.
*/
uint8_t end_hint(void)
{
  /*!@par Pseudo Code:*/
  uint8_t result = FALSE;
  if (ctrl_sys_var_optional_feature.ctrl_consumption_alarm_status == 2)
    ctrl_sys_var_optional_feature.ctrl_consumption_alarm_status = 3;
  /*! - Check that blind period is elapse.*/
  if(!blind_hint_period_sec_counter)
  {
#ifdef CTRL_BZR
    /*! - Check if buzzer is ON.*/
    if(ctrl_sys_var.basic.meter_status[6] & CTRL_BZR_STAT_MASK)
    {
      /*! - Turn buzzer off.*/
      result = TRUE;
      buzzer_off();
    }
#endif
#ifdef CTRL_RLY
#ifndef MTR_ACCRA_NEW
    /*! - Check if relay is waiting hint end.*/
    if(ctrl_sys_var.flags & NEW_HINT_RLY_ACT_FLAG || ctrl_sys_var_optional_feature.ctrl_flags & CTRL_RELAY_HINT_MASK)
    {
      /*!- Close the relay and clear waiting hint flag.*/
      result = TRUE;
      if(CTRL_RLY_STAT)
        toggle_relay();
      CTRL_CLEAR_HINT_RLY();
    }
#endif
#endif
#ifdef CTRL_ACTIONS
    if(result)
    {
      /*! - Form the action to apply due to all latched and non latched tampers
      * and alarms.*/
      deduce_actions();
      /*! - Apply deduced actions.*/
      apply_actions();
    }
#endif
#ifdef MTR_THREE_PH
    if((CTRL_OVR_LOAD_CHA_STAT || CTRL_OVR_LOAD_CHB_STAT || CTRL_OVR_LOAD_CHC_STAT) && (ctrl_cfg.tmpr.non_latch_action[CTRL_OVER_LOAD] & ACT_HINT_BY_RLY))
    {
      if(CTRL_OVR_LOAD_CHA_STAT)
        ctrl_event(CTRL_OVR_LOAD_A_STOP);
      if(CTRL_OVR_LOAD_CHB_STAT)
        ctrl_event(CTRL_OVR_LOAD_B_STOP);
      if(CTRL_OVR_LOAD_CHC_STAT)
        ctrl_event(CTRL_OVR_LOAD_C_STOP);
    }
#endif
#if 0
    if (ctrl_sys_var_optional_feature.ctrl_flags & CTRL_MAXIMUM_CONSUMPTION && ctrl_sys_var_optional_feature.ctrl_maximum_consumptions_action & ACT_HINT_BY_RLY)     
    {
      ctrl_sys_var_optional_feature.ctrl_flags &= ~CTRL_MAXIMUM_CONSUMPTION;
      memset((uint8_t*)&trf_pymt_var.md_actv_pwr.curr,0,sizeof(trf_pymt_var.md_actv_pwr.curr));
      memset((uint8_t*)&trf_pymt_var.md_current.curr,0,sizeof(trf_pymt_var.md_current.curr));
      ctrl_event(CTRL_TMPR_OVR_LD_STOP);
    }
#endif
  }
  return result;
}
#endif

#ifdef FILE_SYS_USE_INT
/*!
* @brief Save certain record in internal memory.
*
* @par Description:
* Save a record in internal memory and save a flag that memory is written.
*
* @param [in] code Code of the event to be saved #ctrl_evnt_code_t.
*
* @return
* - None.
*/
void save_rec_int(uint8_t code)
{
  /*!@par Pseudo Code:*/
  int_mem_rec_t int_rec;
  /*! - Set write flag byte with a defined value.*/
  int_rec.write_flag = INT_MEM_NEW_DATA_FLAG;
  int_rec.rec.evnt_code = code;
  /*! - Write meter status.*/
  memcpy(&(int_rec.rec.meter_status[0]), ctrl_sys_var.basic.meter_status, 4);
  /*! - Write time now.*/
  memcpy(&(int_rec.rec.minute), &(ctrl_sys_var_optional_feature.rtc_time.minute), 5);
  if(code == CTRL_CVR_OPN_STRT)
    code = CVR_REC;
  else if (code == CTRL_TRMNL_OPN_STRT)
    code = TRMNL_REC;  
  else
    code = DETACH_REC; 
  /*! - Save record in internal EERPOM.*/
  CTRL_WRITE_INT(FILE_SYS_GET_INTRNL_MEM_SEC0_ADRS + (sizeof(int_mem_rec_t) * code), \
    (uint8_t*)&int_rec, sizeof(int_rec));
}
#endif

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW) || defined (CTRL_GPRS_MODULE_CVR_SW))
/*!
* @brief Take actions due to last switches press in power down.
*
* @par Description:
* Take actions due to switch release or press in power down.
*
* @return
* - None.
*/
void apply_sw_actions_pwr_dn(void)
{
  /*!@par Pseudo Code:*/
#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_GPRS_MODULE_CVR_SW))
  uint8_t index;
  uint8_t sr_mask;
  uint8_t sw_stat;
  uint8_t event_code;
  /*! - Update time from RTC as RTC interrupt is disabled power down.*/
  ctrl_rtc_isr();
  /*! - Check cover and terminal switches.*/
  if((~scan_result) & (SR_TRMNL_SW | SR_CVR_SW))
  {
    /*! - Load terminal values in loop variables.*/
    sr_mask = SR_TRMNL_SW;
    sw_stat = CTRL_TRMNL_OPN_STAT;
    event_code = CTRL_TRMNL_OPN_STRT;
    /*! - Loop on the two switches (cover, terminal).*/
    for(index = 0; index < 2; index++)
    {
      /*! - Check if the cover or terminal was just opened.*/
      if((!(scan_result & sr_mask)) && (!sw_stat))
      {
        if (event_code == CTRL_TRMNL_OPN_STRT)
          crl_lpm_enable_tamper_sw &= ~0x02; // Remove Terminal
        else
          crl_lpm_enable_tamper_sw &= ~0x01; // Remove Cover
        
#ifdef FILE_SYS_USE_INT
        /*! - Publish tamper.*/
        ctrl_event(event_code);
#endif
      }
      /*! - Load cover values in loop variables.*/
      sr_mask = SR_CVR_SW;
      sw_stat = CTRL_CVR_OPN_STAT;
      event_code = CTRL_CVR_OPN_STRT;
    }
  }
#endif
}
#endif

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW) || defined (CTRL_GPRS_MODULE_CVR_SW))
/*!
* @brief Take actions due to last switches press in power up.
*
* @par Description:
* Take actions due to switch release or press in power up.
*
* @return
* - None.
*/
void apply_sw_actions_pwr_up(void)
{
  /*!@par Pseudo Code:*/
  uint8_t status ;
#if (defined GPRS_FEATURE && defined CTRL_GPRS_MODULE_CVR_SW)
  status = CTRL_GPRS_MODULE_CVR_OPN_STAT;
  if (!CTRL_READ_GPRS_MODULE_CVR_SW() && status)// module cover released then take action
  {
    ctrl_event(CTRL_GPRS_MODULE_CVR_STOP);
    ctrl_sys_var.basic.meter_status[5] &= (~CTRL_GPRS_MODULE_CVR_MASK);
  }
  if (CTRL_READ_GPRS_MODULE_CVR_SW() && (!status))
  {
    ctrl_event(CTRL_GPRS_MODULE_CVR_STRT);
#ifdef DLMS_ENABLED
    CTRL_SET_EVENT_NOTIFICATION_FLAG(GPRS_COVER_MASK);
#endif
    ctrl_sys_var.basic.meter_status[5] |= (CTRL_GPRS_MODULE_CVR_MASK);
  } 
#endif
#ifdef CTRL_CVR_SW
  status = CTRL_CVR_OPN_STAT;
  /*! - Check if the cover is now closed and last was open.*/
  if((scan_result & SR_CVR_SW) && (status))
  {
    /*! - Publish cover open stop event.*/
    ctrl_event(CTRL_CVR_OPN_STOP);
    ctrl_sys_var.basic.meter_status[4] &= (~CTRL_CVR_OPN_MASK);
    CoverClosedFlag = 1;
  }
  /*! - Check if the cover is now open and last was closed.*/
  if((!(scan_result & SR_CVR_SW)) && (!status))
  {
    /*! - Publish cover open start event.*/
    ctrl_event(CTRL_CVR_OPN_STRT);
    ctrl_sys_var.basic.meter_status[4] |= CTRL_CVR_OPN_MASK;
#ifdef DLMS_ENABLED    
    CTRL_SET_EVENT_NOTIFICATION_FLAG(COVER_OPEN_EVENT_MASK);
#endif
    
  }
#endif
#ifdef CTRL_TRMNL_SW
  status = CTRL_TRMNL_OPN_STAT;
  /*! - Check if the terminal is now closed and last was open.*/
  if((scan_result & SR_TRMNL_SW) && (status))
  {
    /*! - Publish cover open stop event.*/
    ctrl_event(CTRL_TRMNL_OPN_STOP);
    ctrl_sys_var.basic.meter_status[4] &= (~CTRL_TRMNL_OPN_MASK);
//////////TODO///////////////////////////////    
//    S_I2C_Dis_EN(DISABLE);
//    S_GpioInit_PIN(I2C_PORT, (I2C_DTA_PIN|I2C_CLK_PIN) , S_GPIO_Mode_OUTPUT_CMOS);
//    
//    S_GPIO_Set_Bits(I2C_PORT, I2C_DTA_PIN);
//    S_GPIO_Set_Bits(I2C_PORT, I2C_CLK_PIN);
//    Delay_ms(100);
//    S_GPIO_Clear_Bits(I2C_PORT, I2C_DTA_PIN);
//    S_GPIO_Clear_Bits(I2C_PORT, I2C_CLK_PIN);
//    Delay_ms(100);
//    S_GPIO_Set_Bits(I2C_PORT, I2C_DTA_PIN);
//    S_GPIO_Set_Bits(I2C_PORT, I2C_CLK_PIN);
//    S_I2C_Dis_EN(ENABLE);
//////////TODO///////////////////////////////  
  }
  /*! - Check if the cover is now open and last was closed.*/
  if((!(scan_result & SR_TRMNL_SW)) && (!status))
  {
    /*! - Publish cover open start event.*/
    ctrl_event(CTRL_TRMNL_OPN_STRT);
    ctrl_sys_var.basic.meter_status[4] |= CTRL_TRMNL_OPN_MASK;
#ifdef DLMS_ENABLED
    CTRL_SET_EVENT_NOTIFICATION_FLAG(TERMINAL_OPEN_EVENT_MASK);
#endif
  }
#endif
#if ((defined(CTRL_DN_SW) || defined(CTRL_UP_SW)) && defined(CTRL_BZR) && defined(CTRL_RLY))
  status = FALSE;
  /*! - Check if up or down is pressed.*/
  if((scan_result & (SR_UP_SW | SR_DN_SW)) && (!blind_hint_period_sec_counter))
    status = end_hint();
#endif
#ifdef LOCAL_WITH_DLMS
#if 0
  if (skip_counter != 0)
  skip_counter--;
  if ((scan_result_cpy & SR_UP_SW) && (scan_result_cpy & SR_DN_SW)) // new self test
  {
    skip_counter = 20;
#ifdef STS_ENABLE
    if(!get_pass_scr_flag())
#endif
      display_reset_manual();
#ifdef STS_ENABLE
      reset_pass_scr_flag();
#endif
    return;
  }
  if (skip_counter != 0)
    return;
#endif
#endif
#ifdef CTRL_UP_SW
  /*! - If the Up switch is pressed, Tell display with the press in normal mode
  * and tell metering with the press in calibration mode.*/
  status = 0;
//#ifdef KEYPAD_FEATURE
//  if(((((Display_check_edit_mode() && (scan_result_cpy & SR_UP_SW)) || ((!Display_check_edit_mode() &&((keypad_up_down & KEYPAD_UP)|| (scan_result & SR_UP_SW)))) )  && !(up_down_sw_flag&0x01))) && (!status))
//#elif defined (EDIT_TIME_DATE_MANUALLY)
//    if(((((Display_check_edit_mode() && (scan_result_cpy & SR_UP_SW)) || ((!Display_check_edit_mode() && (scan_result & SR_UP_SW))) )  && !(up_down_sw_flag&0x01))) && (!status))
//#else
      if((scan_result & SR_UP_SW) && (!status))
//#endif
      {
    //    if(CTRL_CAL_MODE_STAT())
    //      CTRL_MTR_UP_SW_PRESSED();
   //     else
    //    {
          CommSetSendDataFlag(1);
          /* CTRL_LCD_UP_SW_PRESSED(); */  /* display module not built for the DRU */
   //     }
#ifdef EDIT_TIME_DATE_MANUALLY
        up_down_sw_flag |= 0x01;
#endif
#ifdef KEYPAD_FEATURE
        keypad_up_down =(keypad_sw_t)0;
#endif
      }
/* Up-switch release branch removed: it depended on the display module's edit
   mode, and the display is not built for the DRU. */
#endif
#ifdef CTRL_DN_SW
  /*! - If the Down switch is pressed, Tell display with the press in normal mode.*/
#ifdef KEYPAD_FEATURE
      else if((((Display_check_edit_mode() && (scan_result_cpy & SR_DN_SW)) || ((!Display_check_edit_mode() &&((keypad_up_down & KEYPAD_DOWN)|| (scan_result & SR_DN_SW)))) )  && !(up_down_sw_flag&0x02)) && (!CTRL_CAL_MODE_STAT()))
#elif defined (EDIT_TIME_DATE_MANUALLY)
      else if((((Display_check_edit_mode() && (scan_result_cpy & SR_DN_SW)) || ((!Display_check_edit_mode() && (scan_result & SR_DN_SW))) )  && !(up_down_sw_flag&0x02)) && (!CTRL_CAL_MODE_STAT()))
        
#else
      else if((scan_result & SR_DN_SW) && (!status) && (!CTRL_CAL_MODE_STAT()))
#endif
      {
        CTRL_LCD_DN_SW_PRESSED();
#ifdef EDIT_TIME_DATE_MANUALLY
   up_down_sw_flag |= 0x02;
#endif
#ifdef KEYPAD_FEATURE
        keypad_up_down=(keypad_sw_t)0;
#endif
      }
#ifdef EDIT_TIME_DATE_MANUALLY

      else if (((Display_check_edit_mode() && !(scan_result_cpy & SR_DN_SW)) || ((!Display_check_edit_mode() && !(scan_result & SR_DN_SW))) )  && (up_down_sw_flag&0x02))
      {
        CTRL_LCD_DOWN_SW_RELEASED();
        up_down_sw_flag&= ~0x02;
      }

#endif
#endif
}
#endif

#if (defined(CTRL_DN_SW) || defined(CTRL_UP_SW))
/*!
* @brief Check continous press of scrolling switches.
*
* @par Description:
* Change the status of switches according to their history to handle continous press.
*
* @return
* - None.
*
* @note:
* - If the up and down are pressed simultaneously, down switch is ignored.
* - Cover and terminal switches are normally closed.
* - This function records the state of switches in the variable scan_result.
*/
void check_cont_press(void)
{
  /*!@par Pseudo Code:*/
  uint8_t stat_mask;
  uint8_t mask;
  uint8_t index;
  /*! - Check continous press of the two switches UP and DOWN.*/
  for(index = 0; index < CONT_PRESS_SW_NUM; index++)
  {
    /*! - Check if the checked switch is the UP switch or DOWN switch.*/
    if(index)
    {
      mask = SR_DN_SW;
      stat_mask = DN_SW_STAT;
    }
    else
    {
      mask = SR_UP_SW;
      stat_mask = UP_SW_STAT;
    }
    /*! - Check if the switch was pressed or not.*/
    if(scan_result & mask)
    {
      /*! - If the UP switch is pressed, ignore DOWN switch to avoid the effect
      * of pressing the two switches simultaneously.*/
      if(!index)
        scan_result &= (~SR_DN_SW);
      /*! - Check if the switch was pressed before or it is the first time.*/
      if(!( ctrl_sys_var_optional_feature.flag1 & stat_mask))
      {
        /*! - If the first time initialize the period of continous press to 2s.*/
        ctrl_sys_var_optional_feature.flag1 |= stat_mask;
        cont_press_counter = 0;
        cont_press_period = INIT_SCROLL_PERIOD;
        ctrl_sys_var_optional_feature.flag1 &= (~CONT_PRESS_FLAG);
      }
      /*! - Check continous press.*/
      else if(( ctrl_sys_var_optional_feature.flag1 & stat_mask) && ( ctrl_sys_var_optional_feature.flag1 & CONT_PRESS_FLAG))
      {
        /*capture long press*/
        /* latch_long_press_flag=ctrl_sys_var_optional_feature.flag1; */  /* display module not built for the DRU */
        /*! - If it is not the first time and continous press period elapsed.*/
        ctrl_sys_var_optional_feature.flag1 &= (~CONT_PRESS_FLAG);
        cont_press_counter = 0;
        /*! - Decrement the period of continous press.*/
        if(cont_press_period != MIN_SCROLL_PERIOD)
          cont_press_period -= STEP_DOWN_SCROLL_PERIOD;
      }
      else
        scan_result &= (~mask);
    }
    else
      ctrl_sys_var_optional_feature.flag1 &= (~stat_mask);
  }
}
#endif

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW))
/*!
* @brief Get the switches action state.
*
* @par Description:
* Get the event if there is a change in switch state which needs an action or not.
*
* @return
* - uint8_t which represents the state of the switch.
*/
uint8_t get_sw_state(uint8_t sw_mask)
{
  uint8_t state;
  switch(sw_mask)
  {
  case SR_CVR_SW:
    state = !CTRL_READ_CVR_SW();
    break;
  case SR_TRMNL_SW:
    state = !CTRL_READ_TRML_SW();
    break;
  case SR_DN_SW:
    state = CTRL_READ_DWN_SW();
    break;
  case SR_UP_SW:
    state = CTRL_READ_UP_SW();
    break;
  }
  if(state)
    return TRUE;
  return FALSE;
}

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
void update_sr_switches_state(uint8_t type)
{
  /*!@par Pseudo Code:*/
  uint8_t mask;
  uint8_t index;
  for(index = type; index < 4; index++)
  {
    mask = 1 << index;
    ctrl_sys_var.switch_state[index] = (((ctrl_sys_var.switch_state[index] << 1) + get_sw_state(mask))&SWITCH_CHECK_TIMES_FLAG);
    /*! - if all the checks tells that the switch is released for up and down switch or  pressed for cover and terminal, set the corresponding result bit.*/
    if(ctrl_sys_var.switch_state[index] == SWITCH_CHECK_TIMES_FLAG)
      scan_result &= (~mask);
    /*! - if all the checks tells that the switch is pressed for up and down switch or released for cover and terminal, set the corresponding result bit.*/
    if(ctrl_sys_var.switch_state[index] == 0)
      scan_result |= mask;
  }
#ifdef DLMS_UI
  if(get_UI_up_dn_sw_flag()==1) //check down switch in UI
  {
    scan_result |= SR_DN_SW;
    reset_UI_up_dn_sw_flag();

  }
  else if(get_UI_up_dn_sw_flag()==2) //check up switch in UI
  {
    scan_result |= SR_UP_SW;
    reset_UI_up_dn_sw_flag();
    up_switch_hold_press=1;
  }
#endif
  scan_result &= ~SR_DN_SW;
}


#endif

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW))
/*!
* @brief Test one of the switches connected to the shift register.
*
* @par Description:
* Test one of the switches connected to the shift register.
*
* @param [in] sw_mask Mask to be applied on the result of scanning switches
* connected to the shift register.
*
* @return
* - Result of the test (passed, always pressed, always released).
*/
uint8_t test_sw(uint8_t sw_mask)
{
  /*!@par Pseudo Code:*/
  uint8_t result;
  uint8_t index = 0;
#ifdef DEBUG_HANG_LOOPING
  S_GpioInit_PIN(DEBUG_PORT, DEBUG_PIN , S_GPIO_Mode_OUTPUT_CMOS);
  S_GPIO_Clear_Bits(DEBUG_PORT, DEBUG_PIN);
#endif
  for(index = 0; index < (NUMBER_SWITCH_CHECK + 1); index++)
  {
    while(!(flags_byte2 & SR_SCAN_MASK))//ismail  make with time out
    {
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('s',0);
#endif
    };
    flags_byte2 &= (~SR_SCAN_MASK);
    update_sr_switches_state(0);
  }
  /*! - Switch should be released before test beginning.*/
  /*! - If the switch was pressed, result is switch is always pressed.*/
  if(scan_result & sw_mask)
    result = SW_ALWAYS_PRESSED;
  /*! - If the switch is released, test will be resumed.*/
  else
  {
    /*! - Initializes the counter of test time.*/
    test_sw_sec_counter = TEST_SW_PERIOD;
    while(test_sw_sec_counter)
    {
      /*! - Check if the switch is pressed.*/
      while(!(flags_byte2 & SR_SCAN_MASK))//ismail  make with time out
      {
#ifdef DEBUG_HANG_LOOPING
        uart_debug_byte ('v',0);
#endif
      };
      flags_byte2 &= (~SR_SCAN_MASK);
      update_sr_switches_state(0);
      if(scan_result & sw_mask)
      {
        /* !- Test passes and get out.*/
        result = TEST_SW_PASS;
        break;
      }
      else
        result = SW_ALWAYS_RELEASED;
      
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('x',0);
#endif
    }
  }
#ifdef DEBUG_HANG_LOOPING
  S_GPIO_Set_Bits(DEBUG_PORT, DEBUG_PIN);
#endif
  return result;
}
#endif

#ifdef CTRL_EVNT_LOG
/*!
* @brief This function is used to save an event record in non volatile memory.
*
* @par Description:
* Pass an event record data to file system to be saved in non volatile memory.
*
* @param [in] evnt_code Code of the event to be saved.
*
* @return
* - None.
*/
void ctrl_save_evnt_record(uint8_t evnt_code) 
{
#ifdef G2_DM
  G2_DM_Tampers_Log_type  evnt_tamper_rec={0};
  G2_DM_Removed_Tampers_Log_type  remove_tamper={0};

  if((evnt_code >= CTRL_CVR_OPN_REMOVED)&&(evnt_code <= CTRL_GPRS_MODULE_CVR_REMOVED))
    {
      memcpy((uint8_t*)(remove_tamper.tech_id) , (uint8_t*)ctrl_technician_id , sizeof(ctrl_technician_id));
      remove_tamper.removed_tamper_byte=evnt_code;
      /* CTRL_SAVE_EVNT_REC(FILE_SYS_REMOVED_TAMPER_RECORD,(uint8_t*)&remove_tamper); */  /* file system / tariff not built for the DRU */
    }
    else
    {
      evnt_tamper_rec.Tamper_type = evnt_code;
      evnt_tamper_rec.Tamper_type_count = G2_DM_mapping_set_tampers_events_counters(evnt_code); //change arr with the new one
      /* CTRL_SAVE_EVNT_REC(FILE_SYS_EVENT_RECORD,(uint8_t*)&evnt_tamper_rec); */  /* file system / tariff not built for the DRU */
    }
//we construct the struct without time , because time will be added before saving in ctrl_save_record_DM_G2 function.
 
#elif defined(ANSI_STACK) && defined(ANSI_STD)
  
 EVENT_ENTRY_RCD_t event_rec;
 
 Table76_update(evnt_code,event_argument);
 
 memcpy((uint8_t*)&event_rec,(uint8_t*)&Table76_Data.ENTRIES[Table76_Data.LAST_ENTRY_ELEMENT-1],sizeof(EVENT_ENTRY_RCD_t));
 
 /* CTRL_SAVE_EVNT_REC((uint8_t*)&event_rec); */  /* file system / tariff not built for the DRU */
#else
  /*!@par Pseudo Code:*/
  ctrl_evnt_rec_t evnt_rec;
  /*! - Write event code to a buffer.*/
  evnt_rec.evnt_code = evnt_code;
  /*! - Write meter status to the buffer.*/
  memcpy(evnt_rec.meter_status, ctrl_sys_var.basic.meter_status, sizeof(evnt_rec.meter_status));
  /*! - Write time stamp to the buffer.*/
  memcpy(&(evnt_rec.minute), &(ctrl_sys_var_optional_feature.rtc_time.minute), 5);
  /*! - Save data in the buffer as an event record in the non volatile memory.*/
  /* CTRL_SAVE_EVNT_REC((uint8_t*)&evnt_rec); */  /* file system / tariff not built for the DRU */
#endif
}
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
void update_last_operator_ids(uint16_t operator_name)
{
#if 0
  ctrl_cfg_rec_t cfg_rec;
  uint16_t i,n;
  /*!@par Pseudo Code:*/
  memset(ctrl_sys_var_optional_feature.ctrl_last_operator_id,0,sizeof(ctrl_sys_var_optional_feature.ctrl_last_operator_id));
  n = CTRL_GET_NUM_CFG_REC();
  for (i = n ; !((i==0) || ((i== n-3)&&(n>3)));i--)
  {
    CTRL_GET_CFG_MLTI_REC(i-1, 1, (uint8_t *)&cfg_rec);
#ifdef MTR_SINGLE_PH
    //because this value stored in External Flash or EEPROM as a big Endian value.
  cfg_rec.operator_id = SwapEndian16(cfg_rec.operator_id);
#endif
    ctrl_sys_var_optional_feature.ctrl_last_operator_id[n-i]=cfg_rec.operator_id;
  }
#endif
  ctrl_sys_var_optional_feature.ctrl_last_operator_id[2] = ctrl_sys_var_optional_feature.ctrl_last_operator_id[1];
  ctrl_sys_var_optional_feature.ctrl_last_operator_id[1] = ctrl_sys_var_optional_feature.ctrl_last_operator_id[0];
#ifdef MTR_SINGLE_PH
  ctrl_sys_var_optional_feature.ctrl_last_operator_id[0] = SwapEndian16(operator_name);
#else
  ctrl_sys_var_optional_feature.ctrl_last_operator_id[0] = operator_name;
#endif
}
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
uint8_t ctrl_cfg_set(uint8_t cfg_cmd, uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  uint8_t *cfg_ptr;
  uint8_t index;
  /*! - Check if the required data feature is defined.*/
#if ((!defined(CTRL_ACTIONS)) && (!defined(CTRL_BZR)))
  if(cfg_cmd == CTRL_CFG_ACTION)
    return FALSE;
#endif
#ifndef CTRL_RTC
  if((cfg_cmd == CTRL_CFG_TIME) || (cfg_cmd == CTRL_CFG_DATE))
    return FALSE;
#endif
#if (CTRL_BTRY_NON_CHRG == 0)
  if(cfg_cmd == CTRL_CFG_BTRY)
    return FALSE;
#endif
#if ((CTRL_MGNT_SW == 0) && (CTRL_MGNT_SENSOR == 0))
  if(cfg_cmd == CTRL_CFG_MGNT)
    return FALSE;
#endif
  if(cfg_cmd > CTRL_CFG_MAX)
    return FALSE;
  if(cfg_cmd == CTRL_CFG_SERIAL)
  {
    if(ctrl_cfg.serial && ctrl_cfg.serial != 0xffffffff)
      return cfg_struct_size[cfg_cmd];
  }
  /*! - Calculate configuration structure address.*/
  cfg_ptr = (uint8_t*)(&ctrl_cfg);
  for(index = 0; index < cfg_cmd; index++)
    cfg_ptr += cfg_struct_size[index];
  /*! - Copy data to configuration structure.*/
  memcpy(cfg_ptr, data_ptr, cfg_struct_size[cfg_cmd]);
  /*! - Ensure that the penelty mode is not enabled with a wrong tamper*/
  if(cfg_cmd == CTRL_CFG_ACTION)
  {
    for(index = 0; index < CTRL_NON_LTCH_ACT_EVNT_NUM; index++)
    {
#ifdef MTR_THREE_PH
      if(index != CTRL_PHS_SEQ)
#endif
        ctrl_cfg.tmpr.non_latch_action[index] &= ~ACT_GO_PNLTY_MODE;
    }
#ifdef MTR_MIS_POT_TMPR
    ctrl_cfg.tmpr.latch_action[CTRL_MIS_POT] &= ~ACT_GO_PNLTY_MODE;
#endif
  }
#ifdef G2_DM
  if(ctrl_sys_new_var.trf_not_make_new_billing_with_new_date_cfg & G2_CHECH_NO_TIME_BACK)
#endif
  {
  CTRL_DISABLE_RTC_INT();
  if(cfg_cmd == CTRL_CFG_TIME )
  {
    memcpy ((uint8*)&ctrl_sys_var_optional_feature.rtc_time , (uint8*)((rtc_time_t*)data_ptr), sizeof(rtc_time_t));
    S_RTC_C_UpdateTime((rtc_t_t*)&ctrl_sys_var_optional_feature.rtc_time);
  }
  if(cfg_cmd == CTRL_CFG_DATE )
  {
    memcpy ((uint8*)&ctrl_sys_var_optional_feature.rtc_date , (uint8*)((rtc_date_t*)data_ptr), sizeof(rtc_date_t));
    S_RTC_C_UpdateDate((rtc_d_t*)&ctrl_sys_var_optional_feature.rtc_date);
  }
  }
  if((cfg_cmd != CTRL_CFG_TIME) && (cfg_cmd != CTRL_CFG_DATE))
  {
    /*! - Calculate CRC for configuration data.*/
    save_ctrl_cfg_flg = 1;
  }
#ifdef CTRL_RTC
  else
  {
    ctrl_sys_var_optional_feature.rtc_date.day_of_week = CTRL_CALC_DAY_OF_WEEK();
    if(ctrl_sys_var_optional_feature.rtc_date.day_of_week == 0)
      ctrl_sys_var_optional_feature.rtc_date.day_of_week = 7;
    S_RTC_C_UpdateTime((rtc_t_t*)&ctrl_sys_var_optional_feature.rtc_time);
#ifndef G2_DM
    if (ctrl_sys_new_var.trf_not_make_new_billing_with_new_date_cfg || customer_flag)
#else
    if ((ctrl_sys_new_var.trf_not_make_new_billing_with_new_date_cfg & CHECH_NO_TIME_BACK) || customer_flag)
#endif
      /* CTRL_SET_DATE_TIME_PYMENT(); */  /* file system / tariff not built for the DRU */
    if(ctrl_sys_var_optional_feature.rtc_date.day_of_week == 7)
      ctrl_sys_var_optional_feature.rtc_date.day_of_week = 0;
  }
#endif
  CTRL_ENABLE_RTC_INT(); 
  /*! Return data size.*/
  return cfg_struct_size[cfg_cmd];
}

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
uint8_t ctrl_cfg_get(uint8_t cfg_cmd, uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  uint8_t *cfg_ptr;
  uint8_t index;
  /*! - Check if the required data feature is defined.*/
#if ((!defined(CTRL_ACTIONS)) && (!defined(CTRL_BZR)))
  if(cfg_cmd == CTRL_CFG_ACTION)
    return FALSE;
#endif
#ifndef CTRL_RTC
  if((cfg_cmd == CTRL_CFG_TIME) || (cfg_cmd == CTRL_CFG_DATE))
    return FALSE;
#endif
#if (CTRL_BTRY_NON_CHRG == 0)
  if(cfg_cmd == CTRL_CFG_BTRY)
    return FALSE;
#endif
#if ((CTRL_MGNT_SW == 0) && (CTRL_MGNT_SENSOR == 0))
  if(cfg_cmd == CTRL_CFG_MGNT)
    return FALSE;
#endif
  if(cfg_cmd > CTRL_CFG_MAX)
    return FALSE;
  /*! - Calculate configuration structure address.*/
  cfg_ptr = (uint8_t*)(&ctrl_cfg);
  for(index = 0; index < cfg_cmd; index++)
    cfg_ptr += cfg_struct_size[index];
  /*! - Copy configuration structure to the buffer.*/
  memcpy(data_ptr, cfg_ptr, cfg_struct_size[cfg_cmd]);
  
  if (cfg_cmd == CTRL_CFG_SERIAL)
  {
#ifdef MTR_SINGLE_PH
    var_temp_size =0xc0;
    big_endian_memcpy(data_ptr, data_ptr,&var_temp_size, 4);
#endif
  }
  if (cfg_cmd == CTRL_CFG_ID)
  {
#ifdef MTR_SINGLE_PH
  big_endian_memcpy(data_ptr, data_ptr,id_ar_size,6);
#endif
  }
  
  /*! Return data size.*/
  return cfg_struct_size[cfg_cmd];
}

/*!
* @brief This function is used to get defined features of control system.
*
* @par Description:
* Copy defined features array to the buffer. Check the design document.
*
* @return
* - Number of used control features bytes.
*/
uint8_t ctrl_get_fetur(uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Copy features bytes to the buffer.*/
  *data_ptr = CTRL_FETUR0;
  *(data_ptr + 1) = CTRL_FETUR1;
  *(data_ptr + 2) = CTRL_FETUR2;
  *(data_ptr + 3) = CTRL_FETUR3;
  return 4;
}

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
uint16_t ctrl_read_data(uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Copy system variables to the buffer.*/
  memcpy(data_ptr, &(ctrl_sys_var.basic), sizeof(basic_t));
  /*! - Return data size.*/
  return sizeof(basic_t);
}

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
uint8_t ctrl_read_meter_status(uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Copy meter status to the buffer.*/
  memcpy(data_ptr, ctrl_sys_var.shadow_status, sizeof(ctrl_sys_var.shadow_status));
  /*! - Copy hardware component status.*/
  memcpy(data_ptr + 6, ctrl_sys_var.basic.meter_status + 6, 2);
  /*! - Return data size.*/
  return sizeof(ctrl_sys_var.basic.meter_status);
}

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
uint32_t ctrl_get_evnt_num(void)
{
  /*!@par Pseudo Code:*/
  /*! - No event log without the file system, so no records to count.*/
  return 0;
}
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
void ctrl_get_evnt_rec(uint16_t index, uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Reset the counter of unread records.*/
  ctrl_sys_var.new_evnt_counter = 0;
  /*! - Copy the required record to the buffer.*/
  //CTRL_GET_EVNT_REC(index, data_ptr);
}

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
uint16_t ctrl_get_evnt_mlti_rec(uint16_t from, uint16_t num_rec, uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Reset the counter of unread records.*/
  ctrl_sys_var.new_evnt_counter = 0;
  /*! - No event log without the file system, so no records to copy.*/
  return 0;
}
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
uint32_t ctrl_get_cfg_meter_num(void)
{
  /*!@par Pseudo Code:*/
  /*! - No configure-meter log without the file system, so no records to count.*/
  return 0;
}

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
void ctrl_get_cfg_meter_rec(uint8_t index, uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Copy the required configure meter record to the buffer.*/
  //CTRL_GET_CFG_REC(index, data_ptr);
}

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
uint16_t ctrl_get_cfg_meter_mlti_rec(uint8_t from, uint8_t num_rec, uint8_t *data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - No configure-meter log without the file system, so no records to copy.*/
  return 0;
}
#endif

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
* - None.ctrl_event (CTRL_ADE_COMM_ERR_STRT)
*/
void ctrl_event(uint8_t event)
{
  /*!@par Pseudo Code:*/
  uint8_t index, element, shift;
#if (defined(CTRL_BZR) || defined(CTRL_RLY))
  uint8_t action;
#endif
#if 1 //ndef MTR_THREE_PH
  check_update_times_event(event);
#endif
  switch(event)
  {
#ifdef ANSI_STACK  
  case CTRL_SELF_DIAGNOSTIC_STRT: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms |=  ((uint32_t)0x01 << CTRL_SELF_DIAGNOSTIC_FAIL) ;
    if(ctrl_sys_var_second_optional_feature.self_test_counter < 0xff)
    ctrl_sys_var_second_optional_feature.self_test_counter++;
    return;
  case CTRL_USER_OPEN_RELAY_STRT: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms |=  ((uint32_t)0x01 << CTRL_USER_OPEN_RELAY) ; 
    if(ctrl_sys_new_var.relay_disconnect_counter < 0xffff)
    ctrl_sys_new_var.relay_disconnect_counter++; 
    return;
  case CTRL_CUSTOMER_OPEN_RELAY_STRT: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms |=  ((uint32_t)0x01 << CTRL_CUSTOMER_OPEN_RELAY) ; 
    return;
  case CTRL_INCORRECT_PASSWORD_STRT: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms |=  ((uint32_t)0x01 << CTRL_INCORRECT_PASSWORD) ; 
    if( ctrl_sys_var_second_optional_feature.incorrect_password_counter < 0xff)
    ctrl_sys_var_second_optional_feature.incorrect_password_counter++;
    return;
  case CTRL_MISS_NUETRAL_STRT: 
     ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms |=  ((uint32_t)0x01 << CTRL_MISS_NUETRAL) ; 
     if( ctrl_sys_var_second_optional_feature.miss_neutral_counter < 0xff)
     ctrl_sys_var_second_optional_feature.miss_neutral_counter++;
    return;
  case CTRL_ADE_FAIL_STRT: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms |=  ((uint32_t)0x01 << CTRL_ADE_FAIL) ;
    if( ctrl_sys_var_second_optional_feature.ade_fail_counter < 0xff)
    ctrl_sys_var_second_optional_feature.ade_fail_counter++;
    return;
  case CTRL_RELAY_FAIL_STRT: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms |=  ((uint32_t)0x01 << CTRL_RELAY_FAIL) ; 
    if( ctrl_sys_new_var.error_contactor < 0xffff)
    ctrl_sys_new_var.error_contactor++;
    return;
  case CTRL_SELF_DIAGNOSTIC_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms &=  ~((uint32_t)0x01 << CTRL_SELF_DIAGNOSTIC_FAIL) ; 
    return;
  case CTRL_USER_OPEN_RELAY_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms &=  ~((uint32_t)0x01 << CTRL_USER_OPEN_RELAY) ;  
    return;
  case CTRL_CUSTOMER_OPEN_RELAY_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms &=  ~((uint32_t)0x01 << CTRL_CUSTOMER_OPEN_RELAY) ; 
    return;
  case CTRL_INCORRECT_PASSWORD_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_alarms &=  ~((uint32_t)0x01 << CTRL_INCORRECT_PASSWORD) ; 
    return;
  case CTRL_MISS_NUETRAL_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms &=  ~((uint32_t)0x01 << CTRL_MISS_NUETRAL) ; 
    return;
  case CTRL_ADE_FAIL_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms &=  ~((uint32_t)0x01 << CTRL_ADE_FAIL) ;
    return;
  case CTRL_RELAY_FAIL_STOP: 
    ctrl_sys_var_second_optional_feature.ansi_remaining_fatal_alarms &=  ~((uint32_t)0x01 << CTRL_RELAY_FAIL) ; 
    return;
#endif   
  case CTRL_NEW_SEC:
    /*! - Set the flag of new second event.*/
    /*Reset detach counter for meter to store detach event at least once per second */
    ctrl_upsidedown_counter++;
    flags_byte2 |= NEW_SEC_FLAG;
#if (defined(CTRL_MD_SW) || defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW) || defined (CTRL_GPRS_MODULE_CVR_SW))
    /*! - Decrement counter of testing switches.*/
    if(test_sw_sec_counter)
      test_sw_sec_counter--;
    /* Accuracy-test second counter removed: metering is not built for the DRU. */
#endif
    return;
  case CTRL_NEW_MIN:
    /*! - Set new minute event flag.*/
    flags_byte2 |= NEW_MIN_FLAG;
    return;
  case CTRL_NEW_HR:
    /*! - Set new hour event flag.*/
    flags_byte2 |= NEW_HR_FLAG;
    return;
  case CTRL_MD_RST:
    /*! If event is reset MD, increment reset MD counter.*/
    if(ctrl_sys_var.basic.md_reset_count != 0xffff)
      ctrl_sys_var.basic.md_reset_count++;
    break;
  case CTRL_METER_DATA_READ:
    if(ctrl_sys_var.basic.read_data_count != 0xff)
      ctrl_sys_var.basic.read_data_count++;         
    break;
  case CTRL_PWR_DN: 
#if 0
    CTRL_DISABLE_RTC_INT();
  //  if(!(eeprom_load_errors_register&CTRL_OPTIONAL_ERR_LOAD_FLAG))
    {
      ctrl_sys_var_optional_feature.crc = calc_crc((uint8_t*)&ctrl_sys_var_optional_feature, sizeof(ctrl_sys_var_optional_feature_t) - 1,CRC);
      /* CTRL_WRITE_OPTIONAL_FEATURE((uint8_t*)&ctrl_sys_var_optional_feature,sizeof(ctrl_sys_var_optional_feature_t)); */  /* file system / tariff not built for the DRU */
    }
    CTRL_ENABLE_RTC_INT();
 //   if ( ! ( eeprom_load_errors_register & CTRL_NEW_SYS_ERR_LOAD_FLAG))
    {
      ctrl_sys_new_var.crc = calc_crc((uint8_t*)&ctrl_sys_new_var, sizeof(ctrl_sys_new_var) - 1,CRC); 
      /* CTRL_SAVE_DATA(CTRL_GET_CONTROL_NEW_VAR_ADRS(), (uint8_t*)&ctrl_sys_new_var, sizeof(ctrl_sys_new_var_t)); */  /* file system / tariff not built for the DRU */
    }
    
    if(!(eeprom_load_errors_register&CTRL_VAR_ERR_LOAD_FLAG))
    {
      /*! - Do operations related to control system before power down.*/
      /*! - Calculate control system variables CRC.*/
      ctrl_sys_var.crc = calc_crc((uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t) - 1,CRC);
      /*! - Save control system variables in non volatile memory.*/
      /* CTRL_SAVE_DATA(CTRL_GET_SYS_VAR_ADRS(), (uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t)); */  /* file system / tariff not built for the DRU */
    } 
    if(!(eeprom_load_errors_register&CTRL_SECOND_OPTIONAL_ERR_LOAD_FLAG))
    {
      ctrl_sys_var_second_optional_feature.crc = calc_crc((uint8_t*)&ctrl_sys_var_second_optional_feature, sizeof(ctrl_sys_var_second_optional_feature_t) - 1,CRC);
      /* CTRL_WRITE_SECOND_OPTIONAL_FEATURE((uint8_t*)&ctrl_sys_var_second_optional_feature,sizeof(ctrl_sys_var_second_optional_feature_t)); */  /* file system / tariff not built for the DRU */
    }
#endif
    break;
  case CTRL_SAVE_CRITICAL_DATA :
#if 0
      __disable_irq();	// Todo _ check their effect 
#if (MicroController == Micro_V85XX)
    //S_CLK_Init(1);
#endif
    uC_speed_high = 1;
    /* Tariff/payment is not built for the DRU: no publish flag to set and no
       tariff data to save. */
    if(!CTRL_PWR_DN_STAT)
    {
      /*! - Save metering data.*/
      /* CTRL_TELL_METERING_SAVE_DATA(); */  /* metering not built for the DRU */
    }
    CTRL_DISABLE_RTC_INT(); 
    if(!(eeprom_load_errors_register&CTRL_OPTIONAL_ERR_LOAD_FLAG) && (!CTRL_PWR_DN_STAT))
    {
      ctrl_sys_var_optional_feature.crc = calc_crc((uint8_t*)&ctrl_sys_var_optional_feature, sizeof(ctrl_sys_var_optional_feature_t) - 1,CRC);
      /* CTRL_WRITE_OPTIONAL_FEATURE((uint8_t*)&ctrl_sys_var_optional_feature,sizeof(ctrl_sys_var_optional_feature_t)); */  /* file system / tariff not built for the DRU */
      
      if (ctrl_sys_var_optional_feature.load_profile_configuration_time == 0)
        ctrl_sys_var_optional_feature.load_profile_configuration_time = 15*60;
      
      if(!CTRL_PWR_DN_STAT)
      {    
        ctrl_sys_var_optional_feature.crc = calc_crc((uint8_t*)&ctrl_sys_var_optional_feature, sizeof(ctrl_sys_var_optional_feature_t) - 1,CRC);
        /* CTRL_WRITE_BACKUP_OPTIONAL_FEATURE((uint8_t*)&ctrl_sys_var_optional_feature,sizeof(ctrl_sys_var_optional_feature_t)); */  /* file system / tariff not built for the DRU */
      }
    }
    CTRL_ENABLE_RTC_INT();
    if ( ! ( eeprom_load_errors_register & CTRL_NEW_SYS_ERR_LOAD_FLAG) && (!CTRL_PWR_DN_STAT))
    {
      ctrl_sys_new_var.crc = calc_crc((uint8_t*)&ctrl_sys_new_var, sizeof(ctrl_sys_new_var) - 1,CRC); 
      /* CTRL_SAVE_DATA(CTRL_GET_CONTROL_NEW_VAR_ADRS(), (uint8_t*)&ctrl_sys_new_var, sizeof(ctrl_sys_new_var_t)); */  /* file system / tariff not built for the DRU */
      /* if(!CTRL_PWR_DN_STAT) */  /* body removed with the file system */
        /* CTRL_SAVE_DATA(CTRL_CONTROL_NEW_VAR_ADRS_BACKUP(), (uint8_t*)&ctrl_sys_new_var, sizeof(ctrl_sys_new_var_t)); */  /* file system / tariff not built for the DRU */
    }
    if(!(eeprom_load_errors_register&CTRL_VAR_ERR_LOAD_FLAG) && (!CTRL_PWR_DN_STAT) )
    {
      /*! - Calculate control system variables CRC.*/
      ctrl_sys_var.crc = calc_crc((uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t) - 1,CRC);
      /*! - Save control system variables in non volatile memory.*/
      /* CTRL_SAVE_DATA(CTRL_GET_SYS_VAR_ADRS(), (uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t)); */  /* file system / tariff not built for the DRU */
      /* if(!CTRL_PWR_DN_STAT) */  /* body removed with the file system */
        /* CTRL_SAVE_DATA(CTRL_GET_SYS_VAR_ADRS_BACKUP(), (uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t)); */  /* file system / tariff not built for the DRU */
    }
    if(!(eeprom_load_errors_register&CTRL_ERR_LOAD_FLAG) && (!CTRL_PWR_DN_STAT))
    {
      ctrl_cfg.crc = calc_crc((uint8_t*)&ctrl_cfg, sizeof(ctrl_cfg_t) - 1,CRC);
      /*! - Save configuration data in non volatile memory.*/
      /* CTRL_SAVE_DATA(CTRL_GET_CFG_ADRS(), (uint8_t*)&ctrl_cfg, sizeof(ctrl_cfg_t)); */  /* file system / tariff not built for the DRU */
      /* if(!CTRL_PWR_DN_STAT) */  /* body removed with the file system */
        /* CTRL_SAVE_DATA(CTRL_GET_CFG_ADRS_BACKUP(), (uint8_t*)&ctrl_cfg, sizeof(ctrl_cfg_t)); */  /* file system / tariff not built for the DRU */
    }
    if(!(eeprom_load_errors_register&CTRL_SECOND_OPTIONAL_ERR_LOAD_FLAG) && (!CTRL_PWR_DN_STAT))
    {
      ctrl_sys_var_second_optional_feature.crc = calc_crc((uint8_t*)&ctrl_sys_var_second_optional_feature, sizeof(ctrl_sys_var_second_optional_feature_t) - 1,CRC);
      /* CTRL_WRITE_SECOND_OPTIONAL_FEATURE((uint8_t*)&ctrl_sys_var_second_optional_feature,sizeof(ctrl_sys_var_second_optional_feature_t)); */  /* file system / tariff not built for the DRU */
      /* if(!CTRL_PWR_DN_STAT) */  /* body removed with the file system */
        /* CTRL_WRITE_BACKUP_SECOND_OPTIONAL_FEATURE((uint8_t*)&ctrl_sys_var_second_optional_feature,sizeof(ctrl_sys_var_second_optional_feature_t)); */  /* file system / tariff not built for the DRU */
    } 
#ifdef STS_ENABLE
  if(!(eeprom_load_errors_register&STS_CFG_ERR_LOAD_FLAG) && (!CTRL_PWR_DN_STAT))
  {
    CTRL_TELL_STS_PWR_DN();
    if(!CTRL_PWR_DN_STAT)
      WRITE_STS_CFG_BACKUP_DATA((uint8_t *)&STS_Master);
  }
#endif
    /* Tariff power-down notification removed: tariff/payment is not built. */
  //  Flash_app_critical_data();

    uC_speed_high = 0;
#if (MicroController == Micro_V85XX)
    //S_CLK_Init(0);
#endif
    __enable_irq();	// Todo _ check their effect 
    return;
#endif
  }
#ifdef G2_DM
  if(ctrl_check_tampers_flag(event)) //check first on Cfg flag of tamper before save it 
#endif
  {
#ifdef CTRL_EVNT_LOG
#ifdef FILE_SYS_USE_INT
  if(flags_byte2 & INT_MEM_FLAG)
    /*! - If the tamper happened in power down, save in internal EEPROM. Only cover and terminal.*/
    save_rec_int(event);
#else
  /*! - If the tamper happened in power up, Save event record in external
  * non volatile memory except new second event.*/
  ctrl_save_evnt_record(event);
#endif
#endif
  if (event == CTRL_CVR_OPN_STRT)
    ctrl_set_block_state();
  if(((event >= CTRL_DATA_CRPT_MIN) && (event <= CTRL_LATCH_EVNT_STOP_MAX)) || event == CTRL_WARNING_EXCEED_CONSUMPTION_LIMIT)
  {
    /*! - Check if the event is start.*/
    if(event <= CTRL_LATCH_EVNT_STRT_MAX)
    {
      uint8_t temp;
      element = (event - CTRL_DATA_CRPT_MIN) / 8;
      shift = (event - CTRL_DATA_CRPT_MIN) % 8;
      /*! - Set new tampers flag.*/
#ifdef GPRS_FEATURE
      if ( ( (event >= CTRL_DATA_CRPT_MIN && event <= CTRL_DATA_CRPT_MAX )&&CTRL_DATA_CRPT_ENA_FLAG)|| ( (event >= CTRL_COMM_ERR_STRT_MIN && event <= CTRL_HARDWARE_ERR_STRT_MAX )&&CTRL_HW_ERR_ENA_FLAG)||(event == CTRL_LOW_BTRY_STRT &&CTRL_BTRY_ENA_FLAG)||( event == CTRL_LOW_CRDT_1ST_ALRM_STRT &&CTRL_CRDT_1ST_ENA_FLAG) ||(event == CTRL_LOW_CRDT_2ND_ALRM_STRT &&CRTL_CRDT_2ND_ENA_FLAG)||( event == CTRL_NO_CRDT_STRT && CTRL_NO_CRDT_ENA_FLAG) || 
          ( event == CTRL_NO_CRDT_FRNDLY_STRT&& CTRL_NO_FRNDLY_ENA_FLAG) || ( ( (event == CTRL_OVR_VOLT_A_STRT)||(event == CTRL_OVR_VOLT_B_STRT)||(event == CTRL_OVR_VOLT_C_STRT) ) && CTRL_OVER_VOLT_A_ENA_FLAG )||  
            ( ( (event == CTRL_OVR_LOAD_A_STRT) || (event == CTRL_OVR_LOAD_B_STRT) || (event == CTRL_OVR_LOAD_C_STRT))&& CTRL_OVER_LOAD_A_ENA_FLAG)|| ( ( (event == CTRL_UNDR_VOLT_A_STRT) || (event == CTRL_UNDR_VOLT_B_STRT) || (event == CTRL_UNDR_VOLT_C_STRT))&& CTRL_UNDER_VOLT_A_ENA_FLAG)||(  event == CTRL_CVR_OPN_STRT && CTRL_CVR_OPEN_ENA_FLAG)|| 
              (  event == CTRL_TRMNL_OPN_STRT&& CTRL_TERMINAL_OPN_ENA_FLAG )|| ( ((event==CTRL_RVRS_CRNT_A_STRT)||(event==CTRL_RVRS_CRNT_B_STRT)||(event==CTRL_RVRS_CRNT_C_STRT)) && CTRL_RVR_A_ENA_FLAG )|| ( event==CTRL_FLT_ENRG_STRT&& FLT_ENRG_ENA_FLAG )||( event==CTRL_MGNT_HIGH_STRT&& CTRL_MGNT_HI_ENA_FLAG )||
                ( (event == CTRL_RELAY_ERR_STRT) && CTRL_RELAY_ERR_ENA_FLAG) || ( (event == CTRL_GPRS_MODULE_CVR_STRT) && CTRL_GPRS_MODULE_CVR_FLAG))
      {
        ctrl_sys_var_second_optional_feature.gprs_flag_notify |= 0x01; /* if tamper is set for the first time, put the counter to max to send it instantnious else if it fails to send, don't put the counter to max */ //new
        ctrl_sys_var.flags |= CTRL_GPRS_NEW_TMPR_FLAG;
      }
#endif
      ctrl_sys_var.flags |= CTRL_RFID_NEW_TMPR_FLAG;
      /*! - Modify meter status.*/
      /*! - If the event is start, set its coreesponding flag in the meter
      * status and in the shadow status.*/
      ctrl_sys_var.basic.meter_status[element] |= (1 << shift);
      ctrl_sys_var.shadow_status[element] |= (1 << shift);
      /*! - Get the index of alarms and tampers counter array of the new tamper
      * or alarm.*/
      if (event != CTRL_TMPR_OVR_LD_STRT)
      {
        index = match_array[event - CTRL_DATA_CRPT_MIN];
        
        if (event == CTRL_MIS_POT_B_STRT || event == CTRL_MIS_POT_C_STRT)
        {
          if (event == CTRL_MIS_POT_B_STRT)
          {
            if (ctrl_sys_var_optional_feature.ctrl_missing_potential_tamper_counter_phase_B !=  0xff)
            {
              ctrl_sys_var_optional_feature.ctrl_missing_potential_tamper_counter_phase_B++;
            }
          }
          else if (event == CTRL_MIS_POT_C_STRT)
          {
            if (ctrl_sys_var_optional_feature.ctrl_missing_potential_tamper_counter_phase_C !=  0xff)
            {
              ctrl_sys_var_optional_feature.ctrl_missing_potential_tamper_counter_phase_C++;
            }
          }
        }
        else
        {
          /*! - Increment counter of the new tamper or alarm.*/
#ifndef MTR_SINGLE_PH
          if((*(ctrl_sys_var.basic.non_latch_evnt_count + index)) != 0xff)
#else
          if((*(ctrl_sys_var.basic.non_latch_evnt_count + index)) != 0xff &&  event != CTRL_GPRS_MODULE_CVR_STRT)
#endif
            (*(ctrl_sys_var.basic.non_latch_evnt_count + index))++;
        }
      }
#ifdef MTR_SINGLE_PH
      if (event == CTRL_TRMNL_OPN_STRT || \
        event == CTRL_CVR_OPN_STRT || event == CTRL_RVRS_CRNT_A_STRT ||event ==CTRL_RVRS_CRNT_B_STRT|| \
          event == CTRL_FLT_ENRG_A_STRT || event == CTRL_RELAY_ERR_STRT)
      {
        ctrl_sys_var_optional_feature.Last_tamper_code = event-CTRL_DATA_CRPT_MIN;  
        memcpy(&(ctrl_sys_var_optional_feature.Last_tamper_min), &(ctrl_sys_var_optional_feature.rtc_time.minute), 5);
        if (event == CTRL_TRMNL_OPN_STRT)
        {
          temp = 0;
        }
        else if (event == CTRL_CVR_OPN_STRT)
        {
          temp = 1;
        }
        else if (event == CTRL_RVRS_CRNT_A_STRT )
        {
          temp = 2;
        }
        else if (event == CTRL_FLT_ENRG_A_STRT )
        {
          temp = 3;
        }
        else if (event == CTRL_RELAY_ERR_STRT)
        {
          temp = 4;
        }
        else if (event == CTRL_RVRS_CRNT_B_STRT )
        {
          temp = 9;
        }
       if (ctrl_sys_var_optional_feature.rtc_date.year >= 13 )
        {
        if (last_tampers.Last_tamper_year[temp]<=20)
        {
          last_tampers.Last_tamper_min[temp] = ctrl_sys_var_optional_feature.rtc_time.minute;
          last_tampers.Last_tamper_hour[temp] = ctrl_sys_var_optional_feature.rtc_time.hour;
          last_tampers.Last_tamper_day[temp] = ctrl_sys_var_optional_feature.rtc_date.day;
          last_tampers.Last_tamper_month[temp] = ctrl_sys_var_optional_feature.rtc_date.month;
          last_tampers.Last_tamper_year[temp] = ctrl_sys_var_optional_feature.rtc_date.year;
          last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);          
          
          CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t));
        }
        }
      }
      if (event == CTRL_OVR_LOAD_A_STRT || event == CTRL_OVR_VOLT_A_STRT || event == CTRL_UNDR_VOLT_A_STRT || event == CTRL_LOW_BTRY_STRT )
      {
        if (event == CTRL_OVR_LOAD_A_STRT)
        {
          temp = 5;
        }
        else if (event == CTRL_OVR_VOLT_A_STRT)
        {
          temp = 6;
        }
        else if (event == CTRL_UNDR_VOLT_A_STRT )
        {
          temp = 7;
        }
        else if (event == CTRL_LOW_BTRY_STRT )
        {
          temp = 8;
          memcpy((uint8_t*)&ctrl_sys_new_var.last_battery_low_occurance_date_time.tamper_min,(uint8_t*)&ctrl_sys_var_optional_feature.rtc_time.minute,5);
        }
        if (ctrl_sys_var_optional_feature.rtc_date.year >= 13 )
        {
        if (last_tampers.Last_tamper_year[temp]<=20)
        {
          last_tampers.Last_tamper_min[temp] = ctrl_sys_var_optional_feature.rtc_time.minute;
          last_tampers.Last_tamper_hour[temp] = ctrl_sys_var_optional_feature.rtc_time.hour;
          last_tampers.Last_tamper_day[temp] = ctrl_sys_var_optional_feature.rtc_date.day;
          last_tampers.Last_tamper_month[temp] = ctrl_sys_var_optional_feature.rtc_date.month;
          last_tampers.Last_tamper_year[temp] = ctrl_sys_var_optional_feature.rtc_date.year;
          last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);     
          CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t));    
        }
        }
      }
#else
      if (event == CTRL_TRMNL_OPN_STRT || \
        event == CTRL_CVR_OPN_STRT || event == CTRL_RVRS_CRNT_A_STRT ||event ==CTRL_RVRS_CRNT_B_STRT|| event ==CTRL_RVRS_CRNT_C_STRT|| \
          event == CTRL_FLT_ENRG_STRT || event == CTRL_RELAY_ERR_STRT  || event == CTRL_MIS_POT_A_STRT || event == CTRL_MIS_POT_B_STRT || 
            event == CTRL_MIS_POT_C_STRT || event == CTRL_GPRS_MODULE_CVR_STRT || event == CTRL_PHS_SEQ_STRT || event == CTRL_LOW_BTRY_STRT)
      {
        ctrl_sys_var_optional_feature.Last_tamper_code = event-CTRL_DATA_CRPT_MIN;  
        memcpy(&(ctrl_sys_var_optional_feature.Last_tamper_min), &(ctrl_sys_var_optional_feature.rtc_time.minute), 5);
        if (event == CTRL_TRMNL_OPN_STRT)
        {
          temp = 0;
        }
        else if (event == CTRL_CVR_OPN_STRT)
        {
          temp = 1;
        }
        else if (event == CTRL_RVRS_CRNT_A_STRT )
        {
          temp = 2;
        }
        else if (event == CTRL_FLT_ENRG_STRT )
        {
          temp = 3;
        }
        else if (event == CTRL_RELAY_ERR_STRT)
        {
          temp = 4;
        }
        else if (event == CTRL_RVRS_CRNT_B_STRT)
        {
          temp = 9;
        }
        else if(event == CTRL_RVRS_CRNT_C_STRT)
        {
          temp = 10;
        }
        else if (event == CTRL_MIS_POT_A_STRT)
        {
          temp = 17;
        }
        else if (event == CTRL_MIS_POT_B_STRT)
        {
          temp = 18;
        }
        else if (event == CTRL_MIS_POT_C_STRT)
        {
          temp = 19;
        }
        else if(event == CTRL_GPRS_MODULE_CVR_STRT)
        {
          temp = 20;
        }
        else if(event == CTRL_PHS_SEQ_STRT)
        {
          temp = 21;
        }
        else if(event == CTRL_LOW_BTRY_STRT)
        {
          temp = 22;
          memcpy((uint8_t*)&ctrl_sys_new_var.last_battery_low_occurance_date_time.tamper_min,(uint8_t*)&ctrl_sys_var_optional_feature.rtc_time.minute,5);
          
        }
        if (ctrl_sys_var_optional_feature.rtc_date.year >= 13 )
        {
          if (last_tampers.Last_tamper_year[temp]<=20)
          {
        last_tampers.Last_tamper_min[temp] = ctrl_sys_var_optional_feature.rtc_time.minute;
        last_tampers.Last_tamper_hour[temp] = ctrl_sys_var_optional_feature.rtc_time.hour;
        last_tampers.Last_tamper_day[temp] = ctrl_sys_var_optional_feature.rtc_date.day;
        last_tampers.Last_tamper_month[temp] = ctrl_sys_var_optional_feature.rtc_date.month;
        last_tampers.Last_tamper_year[temp] = ctrl_sys_var_optional_feature.rtc_date.year;
        last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);     
        
        /* CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t)); */  /* file system / tariff not built for the DRU */
          }
        }
      }        
      if (event == CTRL_OVR_LOAD_A_STRT || event == CTRL_OVR_LOAD_B_STRT ||event == CTRL_OVR_LOAD_C_STRT || event == CTRL_OVR_VOLT_A_STRT ||  event == CTRL_OVR_VOLT_B_STRT ||  event == CTRL_OVR_VOLT_C_STRT|| event == CTRL_UNDR_VOLT_A_STRT || event == CTRL_UNDR_VOLT_B_STRT || event == CTRL_UNDR_VOLT_C_STRT || event == CTRL_LOW_BTRY_STRT )
      {
        if (event == CTRL_OVR_LOAD_A_STRT)
        {
          temp = 5;
        }
        else if (event == CTRL_OVR_VOLT_A_STRT)
        {
          temp = 6;
        }
        else if (event == CTRL_UNDR_VOLT_A_STRT )
        {
          temp = 7;
        }
        else if (event == CTRL_LOW_BTRY_STRT )
        {
          temp = 8;
          memcpy((uint8_t*)&ctrl_sys_new_var.last_battery_low_occurance_date_time.tamper_min,(uint8_t*)&ctrl_sys_var_optional_feature.rtc_time.minute,5);          
        }   
        else if (event == CTRL_OVR_LOAD_B_STRT)
        {
          temp = 11;
        } 
        else if (event == CTRL_OVR_LOAD_C_STRT)
        {
          temp = 12;
        }
        else if (event == CTRL_OVR_VOLT_B_STRT)
        {
          temp = 13;
        }
        else if (event == CTRL_OVR_VOLT_C_STRT)
        {
          temp = 14;
        }
        else if (event == CTRL_UNDR_VOLT_B_STRT)
        {
          temp = 15;
        }
        else if (event == CTRL_UNDR_VOLT_C_STRT)
        {
          temp = 16;
        }
        if (ctrl_sys_var_optional_feature.rtc_date.year >= 13 )
        {
                    if (last_tampers.Last_tamper_year[temp]<=20)
                    {
        last_tampers.Last_tamper_min[temp] = ctrl_sys_var_optional_feature.rtc_time.minute;
        last_tampers.Last_tamper_hour[temp] = ctrl_sys_var_optional_feature.rtc_time.hour;
        last_tampers.Last_tamper_day[temp] = ctrl_sys_var_optional_feature.rtc_date.day;
        last_tampers.Last_tamper_month[temp] = ctrl_sys_var_optional_feature.rtc_date.month;
        last_tampers.Last_tamper_year[temp] = ctrl_sys_var_optional_feature.rtc_date.year;
        
        last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);
        /* CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t)); */  /* file system / tariff not built for the DRU */
                    }
        }
      }
#endif
    }
    else
    {
      element = ((event - CTRL_COMM_ERR_STOP_MIN) / 8) + 1;
      shift = (event - CTRL_COMM_ERR_STOP_MIN) % 8;
      /*! - If the event is stop, clear its corresponding flag.*/
      ctrl_sys_var.basic.meter_status[element] &= (~(1 << shift));
      /*! - For the shadow status, if the event is not latched, clear its corresponding bit.*/
      if(event <= CTRL_NON_LATCH_EVNT_STOP_MAX)
        ctrl_sys_var.shadow_status[element] &= (~(1 << shift));
    }
    /*! - If the event action is to hint by buzzer, reset silent period of
    * the buzzer.*/
#if (defined(CTRL_BZR) || defined(CTRL_RLY))
    if(event < CTRL_COMM_ERR_STOP_MIN || event == CTRL_WARNING_EXCEED_CONSUMPTION_LIMIT)
    {
      if (event == CTRL_WARNING_EXCEED_CONSUMPTION_LIMIT)
      {
        action = ctrl_sys_var_optional_feature.ctrl_consumption_alarm_action;
#if 0
        if (action == 0)
        {
          action = ACT_BZR_ON | ACT_TMPR_LED_ON;
        }
#endif
      }
      else
      {
        action = (*(ctrl_cfg.tmpr.non_latch_action + index));
        if (event == CTRL_TMPR_OVR_LD_STRT)
          action = ctrl_sys_var_optional_feature.ctrl_maximum_consumptions_action;
      }
#ifdef CTRL_BZR
      /*! - If there is a coming tamper or alarm and its action is hint by
      * buzzer.*/
      if(action & ACT_BZR_ON)
        /*! - Reset silent period counter.*/
        ctrl_sys_var.bzr_silent_sec_cntr = 0;
#endif
#ifdef CTRL_RLY
      /*! - If there is a coming tamper or alarm and its action is hint by
      * relay.*/
      if(action & ACT_HINT_BY_RLY)
      {
        /*! - Set flag of a new hint by relay action should be taken.*/
        ctrl_sys_var_optional_feature.flag1 |= NEW_HINT_RLY_ACT_FLAG;
        ctrl_sys_var.flags |= NEW_HINT_RLY_ACT_FLAG;
      }
      /*! - If there is a coming tamper or alarm and its action is open relay.*/
      if(action & ACT_OPN_RLY_MASK)
      {
        /*! - Clear the flags of hint by relay.*/
        ctrl_sys_var_optional_feature.flags_byte &= (~NEW_HINT_RLY_ACT_FLAG);
        CTRL_CLEAR_HINT_RLY();
      }
#endif
    }
#endif
  }
#ifdef MTR_ENABLE_LMT
  if(((event == CTRL_OVR_LOAD_A_STOP) || (event == CTRL_OVR_LOAD_B_STOP) || (event == CTRL_OVR_LOAD_C_STOP)) && (CTRL_RLY_STAT == 0))
  {
    check_and_reset_ov_load_cntr(SEQ_NORMAL);
    ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
  }
#if 0
  /*! - Check if the event is overload start and its configured action is
  * opening the relay.*/
  if(((event == CTRL_OVR_LOAD_A_STRT) || (event == CTRL_OVR_LOAD_B_STRT) || \
    (event == CTRL_OVR_LOAD_C_STRT)) &&
     (~ctrl_sys_new_var.ovr_load_times_cntr))
  {
    /*! - Increment counter of overload used for special handling of overload tamper.*/
    ctrl_sys_new_var.ovr_load_times_cntr++;
    if (action & ACT_OPN_RLY_MASK)
      ctrl_sys_var_optional_feature.flag1 |= OVER_LOAD_MODE;
  }
#endif
#endif
  
}
}
/*!
* @brief Power status ISR.
*
* @par Description:
* Interrupt service routine called when power line status is changed to up or down.
*
* @return
* - None.
*/
void ctrl_pwr_isr(void)
{
  /*!@par Pseudo Code:*/
#ifdef ADE7953_ENABLE
  /*! - Read power fail pin and modify power down flag.*/
  if(CTRL_READ_PWR_FAIL_PIN())
    ctrl_flags |= CTRL_PWR_DWN_MASK;
  else
    ctrl_flags &= (~CTRL_PWR_DWN_MASK);
#endif
}

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
void ctrl_tmr_isr()
{
  /*!@par Pseudo Code:*/
  /*! - Set shift register scan flag.*/
  flags_byte2 |= SR_SCAN_MASK;
  ctrl_10ms_timer++;
  ctrl_rtc_reinit_ctr++;
  /* SelfTest_gGetSelfTest()==false term dropped: self-test is not built for the
     DRU, so it is always false. */
  /* cal_state()==false term also dropped: metering (V9203) is not built, so the
     meter is never calibrating and that term is always true. */
  if ( ++UnEntering_ctrl_task_Num > 30*100)//if unentering the ctrl task for 10 seconds, =big issue, so reset after saving critical
  {
#ifdef DEBUG_LOW_POWER
    uart_debug_byte ('<',5);
#endif
    CORTEX_NVIC_SystemReset(11);      /* system soft reset */
  }
#ifdef DLMS_ENABLED
  OS_gTimerManager();
  Dlms_Main_gRecvManager() ;
#endif
}
void ctrl_rtc_monitor_task(void)
{
  if(ctrl_rtc_reinit_ctr > 100*5)//for 10 seconds if rtc interrupt wasn't happened through 10 seconds, reinit rtc.
  {
    ctrl_rtc_reinit_ctr = 0;
    No_of_reiniting_rtc ++;
    S_RTC_init();
    CTRL_ENABLE_RTC_INT();
#ifdef DEBUG_LOW_POWER
    uart_debug_byte ('C',10);
#endif
  }
  if (No_of_reiniting_rtc >= 5)
  {
#ifdef DEBUG_LOW_POWER
    uart_debug_byte ('<',5);
#endif
    CORTEX_NVIC_SystemReset(12);      /* system soft reset */
  }
  
}
#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW) || defined(CTRL_GPRS_MODULE_CVR_SW))
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
void ctrl_switch_isr(void)
{
  /*!@par Pseudo Code:*/
  /*! - This function is used to exit halt mode.*/
  scan_result = 0x03;/*
#ifdef CTRL_TRMNL_SW
  if(SWITCHE_COVER_IFG_REG & SWITCHE_COVER)
  {
  scan_result &= ~SR_CVR_SW;
  SWITCHE_COVER_IFG_REG &= ~SWITCHE_COVER;
}
#endif
  
#ifdef CTRL_TRMNL_SW
  if(SWITCHE_Terminal_IFG_REG & SWITCHE_Terminal)
  {
  scan_result &= ~SR_TRMNL_SW;
  SWITCHE_Terminal_IFG_REG &= ~SWITCHE_Terminal;
}
#endif
  
#ifdef CTRL_GPRS_MODULE_CVR_SW
  if(SWITCHE_MODULE_OPEN_IFG_REG & SWITCHE_MODULE_OPEN)
  {
  SWITCHE_MODULE_OPEN_IFG_REG &= ~SWITCHE_MODULE_OPEN;
}
#endif*///
  //P1IFG = 0;
  ///P2IFG = 0;
}

#endif

/*!
* @brief RTC ISR.
*
* @par Description:
* Interrupt service routine called each second to update meter clock from RTC.
*
* @return
* - None.
*/
void ctrl_rtc_isr(void)
{
  /*!@par Pseudo Code:*/
  /*! - Wait for RTC Sync.*/
  /*! - Get time from RTC.*/
  /* seconds_mdw++; */  /* metering not built for the DRU */
  ctrl_rtc_reinit_ctr = 0;
  No_of_reiniting_rtc = 0;
  {
    if (pwr_up_sec_cntr < PWR_UP_NO_REC_TIME && (flags_byte & PWR_UP_MASK))
    {
      BS_check_rtc_senq_date_time();
    }
  }
  if(ctrl_sys_var_optional_feature.rtc_time.second != S_RTC_C_GetRTCSEC())
  {
    /*! - If new second, copy the new value.*/
    ctrl_sys_var_optional_feature.rtc_time.second = S_RTC_C_GetRTCSEC();
    /*! - Inform control with new second event.*/
    ctrl_event(CTRL_NEW_SEC);
    if(!PWR_DN)
    {
#ifdef MODEM_ENABLE
      CTRL_TELL_SIM900_NEW_SEC();
#endif
      //timeout_bootloader_sec();
    }
    //add by ahmed eltabakh for display testing only
    // Add wheel Function
    // RotateWheel(Right);
    //CTRL_TELL_MTR_NEW_SEC();
    //CTRL_TELL_SEASONAL_TRF_NEW_SEC();
  }
#if 0
  if(ctrl_sys_var_optional_feature.rtc_time.minute != S_RTC_C_GetRTCMIN())
  {
    /*! - If new minute, copy new value and inform other modules by new minute event.*/
    ctrl_sys_var_optional_feature.rtc_time.minute = S_RTC_C_GetRTCMIN();
    ctrl_event(CTRL_NEW_MIN);
    
  }
  if(ctrl_sys_var_optional_feature.rtc_time.hour != S_RTC_C_GetRTCHOUR())
  {
    /*! - If new hour, copy new value and inform other modules by new hour event.*/
    ctrl_sys_var_optional_feature.rtc_time.hour = S_RTC_C_GetRTCHOUR();
    ctrl_event(CTRL_NEW_HR);
  }
  if(ctrl_sys_var_optional_feature.rtc_date.month != S_RTC_C_GetRTCMON())
  {
    /*! - If new month, copy new value and inform other modules by new month event.*/
    ctrl_sys_var_optional_feature.rtc_date.month = S_RTC_C_GetRTCMON();
    mtr_cmd(MTR_NEW_MONTH);
  }
  if(ctrl_sys_var_optional_feature.rtc_date.year != S_RTC_C_GetRTCYEAR())
  {
    /*! - If new year, copy new value and inform other modules by new year event.*/
    ctrl_sys_var_optional_feature.rtc_date.year = S_RTC_C_GetRTCYEAR();
  }
  if(ctrl_sys_var_optional_feature.rtc_date.day != S_RTC_C_GetRTCDAY())
  {
    /*! - If new day, copy new value and inform other modules by new day event.*/
    ctrl_sys_var_optional_feature.rtc_date.day = S_RTC_C_GetRTCDAY();
    // ctrl_sys_var_optional_feature.rtc_date.day_of_week = GetRTCDOW();
    ctrl_sys_var_optional_feature.rtc_date.day_of_week = CTRL_CALC_DAY_OF_WEEK();
    if(ctrl_sys_var_optional_feature.rtc_date.day_of_week == 7)
      ctrl_sys_var_optional_feature.rtc_date.day_of_week = 0;
  }
#endif
}

void rtc_task(void){
  if(rtc_flag_counter){
    ctrl_rtc_isr();
    rtc_flag_counter = 0;
  }
}

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
void ctrl_set_gprs_signal_strength(uint8_t new_value)
{
  /*!@par Pseudo Code:*/
  /*! - Set the variable with the new value.*/
  ctrl_sys_var.basic.gprs_signal_strength = new_value;
}
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
uint8_t ctrl_set_unique_rfid(uint8_t* data_ptr)
{
  /*!@par Pseudo Code:*/
  /*! - Copy the new RFID.*/
  memcpy(ctrl_sys_var.basic.rfid_unique, data_ptr, sizeof(ctrl_sys_var.basic.rfid_unique));
  
   /*! - Calculate control system variables CRC.*/
  ctrl_sys_var.crc = calc_crc((uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t) - 1,CRC);
  /*! - Save control system variables in non volatile memory.*/
  CTRL_SAVE_DATA(CTRL_GET_SYS_VAR_ADRS(), (uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var_t));
  /*! - Return size of the ID.*/
  return sizeof(ctrl_sys_var.basic.rfid_unique);
}
#endif


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
uint8_t ctrl_cmd(uint8_t cmd_code)
{
  /*!@par Pseudo Code:*/
  uint8_t result=0,i=0;
  uint8_t crdt_count,crdt_status ;
  
#ifdef PAYMENT_SYS
  uint8_t status;
#endif
  /*! - Check the coming command code.*/
  switch(cmd_code)
  {
#ifdef CTRL_RLY
  case CTRL_TST_RLY:
    ctrl_event(CTRL_SAVE_CRITICAL_DATA);
    /*! - Case of relay test, toggle the relay twice to return to the same state.*/
    toggle_relay();
    //Disconnector_Method_LocalDisconnect();
    vDelay_HW(2);
    toggle_relay();
    //Disconnector_Method_LocalReconnect();
    break;
#endif
#ifdef SELF_TEST_FEATURE
#if (defined(CTRL_RLY) || defined(CTRL_LOW_CRDT_LED) || defined(CTRL_TMPR_LED) || defined(CTRL_BZR))
  case CTRL_VSUAL_SELF_TEST_START:
    /*! - Case of visual test start.*/
#ifdef CTRL_BZR
    /*! - Change meter status.*/
    ctrl_sys_var.basic.meter_status[6] |= CTRL_BZR_STAT_MASK;
    /*! - Turn buzzer ON.*/
    CTRL_BUZZER_ON();
#endif
    
#if (defined(CTRL_LOW_CRDT_LED) || defined(CTRL_TMPR_LED))
    /*! - Turn LEDs ON.*/
    CTRL_WRITE_LOW_CRDT_ON();
    CTRL_WRITE_TMP_LED_OFF();
#endif
    break;
#endif
#if (defined(CTRL_RLY) || defined(CTRL_LOW_CRDT_LED) || defined(CTRL_TMPR_LED) || defined(CTRL_BZR))
  case CTRL_VSUAL_SELF_TEST_STOP:
    /*! - Case of visual test stop.*/
#ifdef CTRL_BZR
    /*! - Turn buzzer off and modify meter status.*/
    CTRL_BUZZER_OFF();
    ctrl_sys_var.basic.meter_status[6] &= (~CTRL_BZR_STAT_MASK);
#endif
#if (defined(CTRL_LOW_CRDT_LED) || defined(CTRL_TMPR_LED))
    /*! - Turn LEDs OFF.*/
    CTRL_WRITE_LOW_CRDT_OFF();
    CTRL_WRITE_TMP_LED_ON();
#endif
    break;
    
#endif
#ifdef CTRL_MD_SW
  case CTRL_TST_MD_SW:
    /*! - Check if the switch is pressed before test begins, end the test.*/
    if(check_md_switch())
      result = SW_ALWAYS_PRESSED;
    else
    {
      /*! - Initialize test switch period.*/
      test_sw_sec_counter = TEST_SW_PERIOD;
#ifdef DEBUG_HANG_LOOPING
      S_GpioInit_PIN(DEBUG_PORT, DEBUG_PIN , S_GPIO_Mode_OUTPUT_CMOS);
      S_GPIO_Clear_Bits(DEBUG_PORT, DEBUG_PIN);
#endif
      while(test_sw_sec_counter)
      {
#ifdef DEBUG_HANG_LOOPING
        uart_debug_byte ('z',0);
#endif
        if(check_md_switch())
        {
          /* !- Test passes and get out.*/
          result = TEST_SW_PASS;
          break;
        }
        else
          result = SW_ALWAYS_RELEASED;
      }
#ifdef DEBUG_HANG_LOOPING
      S_GPIO_Set_Bits(DEBUG_PORT, DEBUG_PIN);
#endif
    }
    return result;
#endif
#ifdef CTRL_CVR_SW
  case CTRL_TST_CVR_SW:
    return test_sw(SR_CVR_SW);
#endif
#ifdef CTRL_TRMNL_SW
  case CTRL_TST_TRMNL_SW:
    return test_sw(SR_TRMNL_SW);
#endif
#ifdef CTRL_UP_SW
  case CTRL_TST_UP_SW:
    return test_sw(SR_UP_SW);
#endif
#ifdef CTRL_DN_SW
  case CTRL_TST_DN_SW:
    return test_sw(SR_DN_SW);
#endif
#endif
  case CTRL_RST:
    /*! - Reset all system variables except hardware component status and no credit status.*/
    result = ctrl_sys_var.basic.meter_status[6];
#ifdef PAYMENT_SYS
    status = CTRL_NO_CRDT_STAT;
#endif
    memset(&ctrl_sys_var, 0, sizeof(ctrl_sys_var));
    memset(&ctrl_sys_new_var, 0, sizeof(ctrl_sys_new_var_t));
    memset((uint8_t*)&ctrl_sys_var_optional_feature,0,sizeof(ctrl_sys_var_optional_feature_t));
    ctrl_sys_var_optional_feature.tariff_prev_num = 0x01;  //calculations in KWH
    ctrl_sys_var_optional_feature.meter_calcualtion = 0x01;//calculations in KWH
    ctrl_sys_var_optional_feature.tariff_prev_num_2 = 0x00;  //calculations in KWH
    ctrl_sys_var_optional_feature.meter_calcualtion_2 = 0x00;//calculations in KWH
    
    ctrl_sys_var_optional_feature.ctrl_consumption_alarm_action = ACT_BZR_ON | ACT_TMPR_LED_ON;
    ctrl_sys_var_optional_feature.ctrl_consumption_alarm_value = CTRL_DEFAULT_CONSUMPTION_ALARM_VALUE;
    ctrl_sys_var_optional_feature.ctrl_grace_consumption = CTRL_DEFAULT_GRACE_CONSUMPTION_VALUE;
    
    ctrl_default_secondryOption();
    ctrl_sys_var.basic.meter_status[6] = result;
#ifdef PAYMENT_SYS
    ctrl_sys_var.basic.meter_status[2] = status;
#endif
    ctrl_sys_var.crc = calc_crc((uint8_t*)&ctrl_sys_var, sizeof(ctrl_sys_var) - 1,CRC);
#ifdef CTRL_EVNT_LOG
#ifdef ANSI_STD
  Table76_clear_event_log();  
#else
    /*! - Clear event log.*/
    /* CTRL_CLEAR_EVNT_LOG(); */  /* file system / tariff not built for the DRU */
#endif
#ifdef DLMS_ENABLED
    CTRL_CLEAR_EVENT_NOTIFICATION_LOG1();
#endif
#endif
#ifdef CTRL_CFG_METER_LOG
    /*! - Clear configure meter log.*/
    /* CTRL_CLEAR_CFG_LOG(); */  /* file system / tariff not built for the DRU */
#endif
#if (defined(MTR_ENABLE_LMT) && defined(CTRL_RLY))
    /*! - Reset overload counters.*/
    ctrl_sys_new_var.ovr_load_sec_cntr = 0;
    ctrl_sys_new_var.ovr_load_times_cntr = 0;
    /* mtr_cmd(MTR_RST_OVR_LD_COUNTERS); */  /* metering not built for the DRU */
#endif
    memset((uint8_t*)&last_tampers,0,sizeof(ctrl_last_tamper_t));
    last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);          
    /* CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t)); */  /* file system / tariff not built for the DRU */
    power_on_minute_counter = 0;
    flags_byte = 0;
    ctrl_sys_var_optional_feature.flag1 = 0;
    flags_byte2 = 0;
    flags_byte2 = 0;
  //  memset((uint8_t*)&ctrl_sys_new_var.trf_chrg_mny_rec,0,sizeof(trf_pymt_mny_rec_t));
#ifdef RECORD_NEW_CUSTOMER_DATE    
    memset((uint8_t*)&ctrl_sys_new_var.new_customer,0,sizeof(ctrl_sys_new_var.new_customer));
    ctrl_sys_new_var.new_customer.crc = calc_crc((uint8_t*)&ctrl_sys_new_var.new_customer, sizeof(ctrl_sys_new_var.new_customer) - 1,CRC);
#endif    
    break;
  case CTRL_RST_TMPR:
    ctrl_reset_watania_tampers(0,10);
    crdt_count = ctrl_sys_var.basic.non_latch_evnt_count[CTRL_NO_CRDT] ;
    // crdt_shdow = CTRL_NO_CRDT_OCCURED;
    crdt_status = CTRL_NO_CRDT_STAT;
    /*! - Clear tampers and alarms counters. Clear status of tampers and alarms also.*/
#if((defined (MTR_RVS_TMPR)) || (defined (MTR_ERTH_TMPR)) || (defined (MTR_MIS_POT_TMPR)) || (defined (MTR_PH_SEQ_TMPR)))
    /* mtr_cmd(MTR_RST_TMP); */  /* metering not built for the DRU */
#endif   
#if (defined(MTR_ENABLE_LMT) && defined(CTRL_RLY))
    ctrl_sys_new_var.ovr_load_sec_cntr = 0;
    ctrl_sys_new_var.ovr_load_times_cntr = 0;
    ctrl_sys_new_var.overload_current_value = 0;
    ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
    /* mtr_cmd(MTR_RST_OVR_LD_COUNTERS); */  /* metering not built for the DRU */
    
    ctrl_sys_var_optional_feature.ctrl_missing_potential_tamper_counter_phase_B = 0;
    ctrl_sys_var_optional_feature.ctrl_missing_potential_tamper_counter_phase_C = 0;
    
    /* Clearing the tariff MD accumulators is gone with the tariff/payment
       module, which is not built for the DRU. */
    
    ctrl_sys_var_optional_feature.ctrl_flags &= ~(CTRL_MAXIMUM_CONSUMPTION);
#endif
    
    memset(ctrl_sys_var.basic.non_latch_evnt_count, 0, \
      sizeof(ctrl_sys_var.basic.non_latch_evnt_count) + sizeof(ctrl_sys_var.basic.latch_evnt_count) + 6);
    /*! - Clear shadow status.*/
    memset(ctrl_sys_var.shadow_status, 0, sizeof(ctrl_sys_var.shadow_status));
    //ctrl_event(CTRL_TMPR_RST);
    flags_byte = 0;
    ctrl_sys_var_optional_feature.flag1 &= OPEN_RELAY_MODE;
    ctrl_sys_var_optional_feature.flag1 &= ~OVER_LOAD_MODE;
    flags_byte2 = 0;
    flags_byte2 = 0;
    
    ctrl_sys_var_optional_feature.flags_byte = 0;
    ctrl_sys_var_optional_feature.flags_byte1 &= OPEN_RELAY_MODE;
    ctrl_sys_var_optional_feature.flags_byte2 = 0;
    
    ctrl_sys_var_optional_feature.Last_tamper_code = 0;
    memset((uint8_t*)&last_tampers,0,sizeof(ctrl_last_tamper_t));
    last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);          
    /* CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t)); */  /* file system / tariff not built for the DRU */
    ctrl_sys_var.tmpr_count = 0;
    
#ifdef RECORD_NEW_CUSTOMER_DATE    
    if (!customer_flag && (ctrl_sys_new_var.new_customer.new_customer_inserted_flag == 0) && (ctrl_sys_new_var.new_customer.year == 0) && CTRL_CUSTOMER_ID != 0 && CTRL_CUSTOMER_ID != 100)
    {
      ctrl_sys_new_var.new_customer.new_customer_inserted_flag = 1;
      ctrl_sys_new_var.new_customer.sec   = ctrl_sys_var_optional_feature.rtc_time.second;
      ctrl_sys_new_var.new_customer.min   = ctrl_sys_var_optional_feature.rtc_time.minute;
      ctrl_sys_new_var.new_customer.hour   = ctrl_sys_var_optional_feature.rtc_time.hour;
      ctrl_sys_new_var.new_customer.day   = ctrl_sys_var_optional_feature.rtc_date.day;
      ctrl_sys_new_var.new_customer.month = ctrl_sys_var_optional_feature.rtc_date.month;
      ctrl_sys_new_var.new_customer.year  = ctrl_sys_var_optional_feature.rtc_date.year;
      ctrl_sys_new_var.new_customer.day_week = ctrl_sys_var_optional_feature.rtc_date.day_of_week;
      ctrl_sys_new_var.new_customer.crc = calc_crc((uint8_t*)&ctrl_sys_new_var.new_customer, sizeof(ctrl_sys_new_var.new_customer) - 1,CRC);
    }
#endif
    /*time and date of last reset tampers for CTRL_OVER_LOAD ,CTRL_BTRY_LOW,CTRL_RVRS_CRNT ,CTRL_CVR_OPN,CTRL_TRMNL_OPN,CTRL_FLT_ENRG*/
    for(i=0;i<6;i++)
    {
      memcpy((uint8_t*)&ctrl_sys_new_var.last_reset_date_time_tamper[i] ,(uint8_t*)&(ctrl_sys_var_optional_feature.rtc_time)+1 ,sizeof(ctrl_sys_new_var.last_reset_date_time_tamper[i]));
    }
    memset((uint8_t *)ctrl_sys_new_var.tamper_event,0,2*sizeof(start_end_event_tamper_t));
    ctrl_sys_var.basic.meter_status[2] |= crdt_status;
    ctrl_sys_var.shadow_status[2] |= crdt_status;
    ctrl_sys_var.basic.non_latch_evnt_count[CTRL_NO_CRDT] = crdt_count;
    btry_prst = 0;
    ctrl_sys_new_var.ctr_no_bat_flage = 0;
    break;
  case CTRL_CLR_NEW_TMPR_GPRS:
    /*! - Clear new tampers flag.*/
    ctrl_sys_var.flags &= (~CTRL_GPRS_NEW_TMPR_FLAG);
    break;
  case CTRL_CLR_NEW_TMPR_RFID:
    /*! - Clear new tampers flag.*/
    ctrl_sys_var.flags &= (~CTRL_RFID_NEW_TMPR_FLAG);
    break;
#if (defined(CTRL_BZR) || defined(CTRL_RLY))
  case CTRL_END_HINT:
    /*! End hint.*/
    end_hint();
    break;
#endif
#ifdef CTRL_BZR
  case CTRL_BEEP_TONE0:
    /*! - Turn buzzer on with tone 0 for certain period.*/
#ifdef AC_BUZZER_FEATURE 
    if (buzzer_enable == 0)
#endif
    {
    CTRL_BUZZER_INIT(BUZ_TONE0);
    beep_counter = BEEP_PERIOD0;
    xc =6;                                  //normal tone
    }
    break;
  case CTRL_BEEP_TONE1:
#ifdef AC_BUZZER_FEATURE 
    if (buzzer_enable == 0)
#endif
    {
    /*! - Turn buzzer on with tone 1 for certain period.*/
    CTRL_BUZZER_INIT(BUZ_TONE1);
    beep_counter = BEEP_PERIOD1;
   #ifdef Beep_Beep_Feature
    beep_beep_Counter = BEEP_PERIOD1;
    #endif
    xc =6;                         //new tone
    }
    break;
  case CTRL_BEEP_TONE2:
    /*! - Turn buzzer on with tone 2 for certain period.*/
    CTRL_BUZZER_INIT(BUZ_TONE2);
    beep_counter = BEEP_PERIOD2;
    break;
  case CTRL_BEEP_TONE3:
#ifdef AC_BUZZER_FEATURE 
    if (buzzer_enable == 0)
#endif
    {
    CTRL_BUZZER_INIT(BUZ_TONE1);
    beep_counter = BEEP_PERIOD3;
    xc =0;                         
    }
    break;
#endif
#ifdef CTRL_EVNT_LOG
  case CTRL_RST_EVNT_LOG:
#ifdef ANSI_STD
    Table76_clear_event_log();  
#else
    /*! - Clear event log.*/
    /* CTRL_CLEAR_EVNT_LOG(); */  /* file system / tariff not built for the DRU */
#endif
#ifdef DLMS_ENABLED
    
    CTRL_CLEAR_EVENT_NOTIFICATION_LOG1();
#endif
    break;
#endif
#ifdef CTRL_CFG_METER_LOG
  case CTRL_RST_CFG_METER_LOG:
    /*! - Clear configure meter log.*/
    /* CTRL_CLEAR_CFG_LOG(); */  /* file system / tariff not built for the DRU */
    break;
#endif
#ifdef CTRL_RLY
  case CTRL_ENTER_OPEN_RELAY_MODE:
    /*! - Enter open relay mode by setting mode flag.*/
    ctrl_sys_var_optional_feature.flag1 |= OPEN_RELAY_MODE;
    ctrl_sys_var.flags |= OPEN_RELAY_MODE;
    ctrl_event(CTRL_EVNT_ENTER_OPEN_RELAY_MODE);
    break;
  case CTRL_EXIT_OPEN_RELAY_MODE:
#ifndef STS_ENABLE
    if(ctrl_sys_new_var.new_customer.new_customer_inserted_flag == 1)
#endif
    {
    /*! - Exit open relay mode by clearing mode flag.*/
    ctrl_sys_var_optional_feature.flag1 &= (~OPEN_RELAY_MODE);
    ctrl_sys_var.flags &= (~OPEN_RELAY_MODE);
    ctrl_event(CTRL_EVNT_EXIT_OPEN_RELAY_MODE);
    }
    break;
#endif
  }
  return 0;
}

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
uint8_t ctrl_change_tmplt_id(uint8_t *data_ptr)
{
#ifdef MTR_SINGLE_PH
  uint8_t tmp;
#endif
  /*!@par Pseudo Code:*/
  
  if(!(ctrl_sys_var.template_id[0] == data_ptr[0]  &&  ctrl_sys_var.template_id[1] == data_ptr[1]))
  {
    if(!(ctrl_sys_var.template_id[0] == 0  &&  ctrl_sys_var.template_id[1] == 0))
    {
      CTRL_CHANGED_TEMPLATE_ID_START;
      ctrl_sys_var.flags |= CTRL_GPRS_NEW_TMPR_FLAG;
      ctrl_sys_var_second_optional_feature.gprs_flag_notify |= 0x01; /* if tamper is set for the first time, put the counter to max to send it instantnious else if it fails to send, don't put the counter to max */ //new
    }
  }
  /*! - Copy the new ID.*/
  memcpy(ctrl_sys_var.template_id, data_ptr, sizeof(ctrl_sys_var.template_id));
#ifdef MTR_SINGLE_PH
  tmp = ctrl_sys_var.template_id[0];
  ctrl_sys_var.template_id[0] = ctrl_sys_var.template_id[1];
  ctrl_sys_var.template_id[1] =  tmp;
#endif
  /*! - Return the size of ID.*/
  return sizeof(ctrl_sys_var.template_id);
}

#ifdef CTRL_CFG_METER_LOG
/*!
* @brief Save a configure meter record.
*
* @par Description:
* Save a configure meter record in non volatile memory.
*
* @param [in] operator_id ID of the operator who configured meter.
* @param [in] interface interface from which meter was configured.
*
* @return
* - None.
*/
void ctrl_save_cfg_meter_rec(uint16_t operator_id, uint8_t interface)
{
  /*!@par Pseudo Code:*/
  ctrl_cfg_rec_t cfg_rec;
  /*! - Update operator ID displayed on screen.*/
  ctrl_sys_var.operator_id = operator_id;
  cfg_rec.operator_id = operator_id;
  /*! - Copy interface of configureation.*/
  cfg_rec.interface = interface;
  /*! - Copy template ID.*/
  CTRL_READ_TMPLT_ID(cfg_rec.template_id);
  /*! - Put time stamp and save the record.*/
  memcpy(&cfg_rec.minute, &ctrl_sys_var_optional_feature.rtc_time.minute, 5);
#ifdef MTR_SINGLE_PH
  //save in External Flash or EEPROM as big Endian according to Single Phase Meter Requirements.
  cfg_rec.operator_id = SwapEndian16(cfg_rec.operator_id);
#endif
  /* CTRL_SAVE_CFG_REC((uint8_t*)&cfg_rec); */  /* file system / tariff not built for the DRU */
  update_last_operator_ids(operator_id);
}
#endif

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || defined(CTRL_DN_SW))
/*!
* @brief Initialize scanning switches results.
*
* @par Description:
* Initialize scanning switches results.
*
* @return
* - None.
*/
void init_sw_scan_res(void)
{
  /*!@par Pseudo Code:*/
  ctrl_sys_var.switch_state[0] = 0xFF;
  ctrl_sys_var.switch_state[1] = 0xFF;
  ctrl_sys_var.switch_state[2] = 0;
  ctrl_sys_var.switch_state[3] = 0;
  /*! - Reset UP and DOWN switches state.*/
  ctrl_sys_var_optional_feature.flag1 &= (~UP_DN_SW_STAT);
}

/*!
* @brief Send pattern to SR before halt.
*
* @par Description:
* Send pattern to SR before halt.
*
* @return
* - None.
*/
void send_low_pwr_ptrn(void)
{
  /*!@par Pseudo Code:*/
  //  CTRL_WRITE_TMP_LED_ON();
}
#endif


/*!
* @brief Low power mode function.
*
* @par Description:
* Go to halt mode and when up, check if the power is up or switch is pressed or
* LCD timeout and handle switches and display during power down.
*
* @return
* - None.
*/
void low_power_mode(void)
{
#ifndef DRU_PROJECT
#ifdef DEBUG_LOW_POWER
  uart_debug_byte ('l',0);
#endif
  /*! - Disable interrupts.*/
  CTRL_DISABLE_INT();
  /*! - Send low power mode pattern.*/
  CTRL_DISABLE_RTC_INT();
  /*! - Reset metering values in LPM .*/
  /*! - Go to low power mode and halt the processor.*/
  go_to_halt();
  /*! - Enable interrupts.*/
  CTRL_ENABLE_INT();
#endif
}

uint8_t ctrl_is_power_down (void)
{
#if 0
  if (PMU_GetPWRSRCStatus() == 1 || (PMU_GetPowerStatus(ANA_COMPOUT_VDDALARM) == 1 )|| (PMU_GetPowerStatus(PMU_PWRSTS_VDCINDROP) == 1 ) )     /*vdd is < threshold*/
    return TRUE;
  return FALSE;
#endif
   return FALSE;
}


void lpm_first_init(void)
{
  if (ctrl_is_power_down())     /*vdd is < threshold*/
  {
#ifdef DEBUG_LOW_POWER
    uart_debug_byte ('l',10);
#endif
    set_power_down_flag();
    low_power_mode();
    power_is_up();
  }
  else
  {
    set_power_up_flag();
  }
}
void Power_up(void)
{
  vMC_DriversInit();
  //lcd_HQ30774_init();
#ifdef DOT_MATRIX_LCD_ENABLE
  Dot_Matrix_GPIO_INIT(); 
  Dot_LCD_Init(0);
#else
  lcd_HQ30774_init( 1, LCD_FRQ_256H , LCD_DRV_150 , LCD_VLCD_DEC300MV);
#endif
#ifdef MTR_TWO_PH
  Two_Phase_Init(RESET_POWER_UP);
#elif defined (MTR_THREE_PH)
 // V9203_Init(V9203_INIT_RAM);
#elif defined (V9261f_ENABLE)
  V9261f_Init(RESET_POWER_UP);
#elif defined (V9360_ENABLE)
  V9360_Init(RESET_POWER_UP);
#elif defined (V9381_ENABLE)
  V9381_Init(RESET_POWER_UP);
#endif
}

/*!
* @brief Operations after power up.
*
* @par Description:
* Tell other modules that power is up and set internal flag to save power up
* record after certain period to assure that no time consuming operations are
* done before the power line is stable.
*
* @return
* - None.
*/
void power_is_up(void)
{
  /*!@par Pseudo Code:*/
  save_switches_in_pwrdn=0;
  //uint8_t rtc_flag = CTRL_RTC_INIT_FLAG;
  LPM_status = 0;
  pwr_up_sec_cntr = 0;
  
  ctrl_flags &= (~CTRL_PWR_DWN_MASK);
  
  //EEPROM_write(Data_eeprom_address ,&rtc_flag ,  EEPROM_RTC_INIT_FLAG_ADD , 1);
  
  ctrl_rtc_isr();
  
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
  trf_grace_elapsed_time.flag = 1;
  trf_grace_elapsed_time.crc = calc_crc((uint8_t*) &trf_grace_elapsed_time,  sizeof(trf_grace_elapsed_time_t)-1,0x01);
  CTRL_WRITE_INT(INT_ELAPSED_SAVE_TIME_ADDR ,(uint8_t*) &trf_grace_elapsed_time, sizeof(trf_grace_elapsed_time_t));
  vDelay_SW((uint16)2000);
#endif
  
  /* Save events record before reset */
  //file_sys_event(FILE_SYS_PWR_DN);
  
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
  trf_grace_elapsed_time.flag = 1;
  trf_grace_elapsed_time.crc = calc_crc((uint8_t*) &trf_grace_elapsed_time,  sizeof(trf_grace_elapsed_time_t)-1,0x01);
  /* Persisting the grace elapsed time is gone with the file system. */
  vDelay_SW((uint16)2000);
#endif
#if defined (DEBUG_LOW_POWER) || defined (DEBUG_BOOTLOADER)
  uart_debug_byte ('x',0);
#endif
  /* system soft reset */
  CORTEX_NVIC_SystemReset(13);
}



#ifdef CTRL_EVNT_LOG
/*!
* @brief Check last power up.
*
* @par Description:
* Check if the last power up and power line is stable now to save power up record
* and checking internal EEPROM for cover or terminal opening during power down.
*
* @return
* - None.
*/
void check_last_power_up(void)
{
#if 0
  /*!@par Pseudo Code:*/
#ifdef FILE_SYS_USE_INT
  uint8_t buffer[2 * sizeof(int_mem_rec_t)];
#endif
  /*! - Check if power up happened since #PWR_UP_NO_REC_TIME seconds.*/
  if((pwr_up_sec_cntr > PWR_UP_NO_REC_TIME) && (flags_byte & PWR_UP_MASK))
  {
    /*! - Clear power up flag.*/
    flags_byte &= (~PWR_UP_MASK);
/* File-system power-up notification removed: file system is not built. */
#ifdef CTRL_EVNT_LOG
    /*! - Save power up event record.*/
    ctrl_event(CTRL_PWR_UP);
#ifdef DLMS_ENABLED
    CTRL_SET_EVENT_NOTIFICATION_FLAG(POWER_UP_MASK);
#endif
#endif
   // update_last_operator_ids();
#ifdef FILE_SYS_USE_INT
    /*! - Read data in internal EEPROM.*/
    CTRL_READ_INT(FILE_SYS_GET_INTRNL_MEM_SEC0_ADRS, buffer, sizeof(buffer));
    /*! - Check for new data in internal memory and save as record in external
    * non volatile memory.*/
    if(buffer[0] == INT_MEM_NEW_DATA_FLAG)
    {
      /* CTRL_SAVE_EVNT_REC(buffer + 1); */  /* file system not built for the DRU */
    }
    if(buffer[sizeof(int_mem_rec_t)] == INT_MEM_NEW_DATA_FLAG)
    {
      /* CTRL_SAVE_EVNT_REC(buffer + sizeof(int_mem_rec_t) + 1); */  /* file system not built for the DRU */
#ifdef FILE_SYS_USE_INT
      /*! - Publish tamper.*/
      ctrl_event(CTRL_TRMNL_OPN_STRT);
#endif
    }
    /*! - Write zeros in internal memory.*/
    memset(buffer, 0, sizeof(buffer));
    CTRL_WRITE_INT(FILE_SYS_GET_INTRNL_MEM_SEC0_ADRS, buffer, sizeof(buffer));
#else
    /* CTRL_SW_STATE_READ(&ctrl_power_down_event); */  /* file system not built for the DRU */
    
    if (ctrl_power_down_event & CVR_OPN_MASK )
    {
     // ctrl_update_sys_var(CTRL_CVR_OPN_STRT);
      ctrl_event(CTRL_CVR_OPN_STRT);
    }
    if (ctrl_power_down_event & TRMNAL_OPN_MASK)
    {
     // ctrl_update_sys_var(CTRL_TRMNL_OPN_STRT);
      ctrl_event(CTRL_TRMNL_OPN_STRT);
    }
    if (ctrl_power_down_event & GPRS_MODULE_CVR_MASK)
    {
      //ctrl_update_sys_var(CTRL_GPRS_MODULE_CVR_STRT);
      ctrl_event(CTRL_GPRS_MODULE_CVR_STRT);
    }
    ctrl_power_down_event = 0;
    /* CTRL_SW_STATE_RESET(&ctrl_power_down_event); */  /* file system / tariff not built for the DRU */
#endif
#ifdef CTRL_RLY
    /*! - Initialize the relay (Apply its current status).*/
    RELAY_INIT();
#endif
  }
#endif
}
#endif

#if (CTRL_BTRY_NON_CHRG == 1)
/*!
* @brief Check battery output voltage.
*
* @par Description:
* Check the battery ouput voltage level and its state and update percentage and
* publish event if state is changed.
*
* @return
* - None.
*/
void check_btry(void)
{  /*!@par Pseudo Code:*/
  uint16_t adc_result ;
  uint8_t status ;
  uint16_t limit_value ;
  uint32_t level ;
  #ifdef SUPERCAP_FEATURE
  static uint8_t SuperCap_Counter=0;
  #endif
  /*RmvCodCmntA_K*/
  /*! - Read battery voltage.*/
#if  defined (EM330_BOARD) && defined (CONTROL_ADC_READING)
    S_GpioInit_PIN(ENABLE_ADC_BATT_PORT, ENABLE_ADC_BATT_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    S_GpioInit_PIN(ENABLE_ADC_SUPERCAP_PORT, ENABLE_ADC_SUPERCAP_PIN , S_GPIO_Mode_OUTPUT_CMOS);
    
    S_GPIO_Clear_Bits(ENABLE_ADC_SUPERCAP_PORT, ENABLE_ADC_SUPERCAP_PIN);//enable measuring
    S_GPIO_Clear_Bits(ENABLE_ADC_BATT_PORT, ENABLE_ADC_BATT_PIN);   //enable measuring    
#endif

#ifdef EM130_BOARD
     static ADC_CalResType batt_result;
#ifndef EM_DL130_BOARD   
  ADC_GetBAT1Voltage_Normal(ADC_BAT_RESDIV,&batt_result);
   adc_result = (uint16_t) (batt_result.BAT1Voltage*100);
#else
  ADC_GetBAT2Voltage_Normal(ADC_BAT_RESDIV,&batt_result);
  adc_result = (uint16_t) (batt_result.BAT2Voltage*100);
#endif

    battery_percentage_fraction = adc_result;
 if (battery_percentage_fraction < 180 || battery_percentage_fraction > 400)
   battery_percentage_fraction = 0;
 adc_result /= 10;
#else
  CTRL_ADC_READ(&adc_result);
#endif
  /*! - Convert high voltage limit from 10^-1 Volt to digital word like ADC output .*/
  /*! - Full scale voltage is 5 Volt divided by voltage divider by 2 which may be
  considered 16 (10^-1 * Volt).*/
#ifndef ASHNTTI_PROJECT
  limit_value = ctrl_cfg.btry.high_lvl;//(BTRY_LOW_LVL_INIT * ((1024  * 3)/ 250)) ;
#else
  limit_value = (ctrl_cfg.btry.high_lvl*3)/10;//(BTRY_LOW_LVL_INIT * ((1024  * 3)/ 250)) ;
#endif
  // 3 represent max battery value
  // 1024 = 2^ ADC_NUM_OF_BITS(10)
  status = CTRL_BTRY_STAT;
  /*! - Check if the status of battery is low and measured value is high.*/
  if((!status) && (adc_result > limit_value))
  {
    /*! - Check if the first time to detect change in level.*/
    if(ctrl_sys_var_optional_feature.flags_byte & BTRY_HIGH_STRT_MASK)
      btry_prst++;
    else
    {
      btry_prst = 0;
      ctrl_sys_var_optional_feature.flags_byte |= BTRY_HIGH_STRT_MASK;
      ctrl_sys_var_optional_feature.flags_byte &= (~BTRY_LOW_STRT_MASK);
    }
    if(btry_prst == BTRY_DETECT_PRESISIT)
    {
#if (defined ASHNTTI_PROJECT || defined NEW_ASHANTI_PROJECT)
      /*! - Publish battery low stop event.*/
      ctrl_event(CTRL_LOW_BTRY_STOP);
#endif
      /*! - Modify meter status.*/
      ctrl_sys_var.basic.meter_status[2] &= (~CTRL_BTRY_LOW_MASK);
    }
  }
  /*! - Convert high limit from 10^-1 Volt to digital word like ADC output .*/
  /*! - Full scale voltage is 5 Volt divided by voltage divider by 2.*/
#ifndef ASHNTTI_PROJECT
  limit_value = ctrl_cfg.btry.low_lvl;//(BTRY_LOW_LVL_INIT * ((1024  * 3)/ 250)) ;
#else
   limit_value = (ctrl_cfg.btry.low_lvl*3)/10;//(BTRY_LOW_LVL_INIT * ((1024  * 3)/ 250)) ;
#endif
  if(status && (adc_result < limit_value))
  {
    /*! - Check if the first time to detect change in level.*/
    if(ctrl_sys_var_optional_feature.flags_byte & BTRY_LOW_STRT_MASK)
      btry_prst++;
    else
    {
      btry_prst = 0;
      ctrl_sys_var_optional_feature.flags_byte |= BTRY_LOW_STRT_MASK;
      ctrl_sys_var_optional_feature.flags_byte &= (~BTRY_HIGH_STRT_MASK);
    }
    if(btry_prst == BTRY_DETECT_PRESISIT)
    {
      /*! - Publish battery low start event.*/
      ctrl_event(CTRL_LOW_BTRY_STRT);
      /*! - Modify meter status.*/
      ctrl_sys_var.basic.meter_status[2] |= CTRL_BTRY_LOW_MASK;
    }
  }
  /*! - Get the percentage of battery. The battery full scale is 3 Volt. and
  * the ADC full scale is 3.3 Volt.*/
  level = adc_result ;//* 10; //((uint32_t)adc_result  * 250) / (1024*3);
  if((level > 100) && (level < 130))
  {
    level = 100 ;
  }
  ctrl_sys_var.basic.battery_level = (uint8_t)level ;
  if(ctrl_sys_var.basic.battery_level < CTRL_NO_BTRY_VOLTAGE )//1volt
  {
    ctrl_sys_new_var.ctr_no_bat_flage = 1;
  }
  
  battery_percentage = (ctrl_sys_var.basic.battery_level*10)/3;
  if (battery_percentage >100)
    battery_percentage = 100;
  
#ifdef ASHNTTI_PROJECT
  ctrl_sys_var.basic.battery_level = battery_percentage;
#endif
#ifdef SUPERCAP_FEATURE
  
    
    if (SuperCap_Counter++ > 10)
    {
       S_GPIO_Set_Bits(Super_Cap_Enable_PORT,Super_Cap_Enable_PIN);   // to close the charging and start reading
    }
    else
    {
      S_GPIO_Clear_Bits(Super_Cap_Enable_PORT, Super_Cap_Enable_PIN); //to open the charging  
    }
    
    if(SuperCap_Counter > 20)
    {
         SuperCap_Counter=0;
        SuperCap_Monitoring();
    }
    
#endif 

#if  defined (EM330_BOARD) && defined (CONTROL_ADC_READING)
    S_GpioInit_PIN(ENABLE_ADC_BATT_PORT, ENABLE_ADC_BATT_PIN , S_GPIO_Mode_FORBIDDEN);
    S_GpioInit_PIN(ENABLE_ADC_SUPERCAP_PORT, ENABLE_ADC_SUPERCAP_PIN , S_GPIO_Mode_FORBIDDEN);
#endif
    
#ifdef BTRY_LOG
    
if ( (battery_percentage_fraction < Batttery_Saved.Battery )  && (battery_percentage_fraction != 0) )
{
Batttery_Saved.Battery = (uint16_t)battery_percentage_fraction;
Batttery_Saved.Second  = CTRL_READ_SEC();
Batttery_Saved.minute  = CTRL_READ_MIN();
Batttery_Saved.hour    = CTRL_READ_HR();
Batttery_Saved.day     = CTRL_READ_DAY();
Batttery_Saved.month   = CTRL_READ_MONTH();
Batttery_Saved.year    = CTRL_READ_YEAR();

//save data 

}
#endif
}
#if 0
//=============================================================================
// FUNCTION
//=============================================================================
// Name           : CTRL_getBatteryLevel
// Description    : check the level of the battery to take return its status.
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 23/05/2011
//=============================================================================
uint16_t CTRL_getBatteryLevel(void)
{
  static uint32_t BatteryLevel = (uint16_t)0; // assume that battery is full for 3V battery uint8_t BatteryStatus;
  uint8_t i;
  uint32_t timeout = 5000 ;
  uint8_t NewSampleReady = 0 ;
  for(i = 0; i < 15; i++)
  {
    // CTRL_startBatteryRead();
    Auxiliary1_ADC_measuring();
    // With this configuration, The Conversion Time is 0.4 msec.
    // So we make timerout is 1 msec.
    Timer_Is_Expired = 0 ;
    // TimerA1_Init(1);  // Start Timer "1 msec"
    NewSampleReady = ADC_get_NewSampleReady();
    while((NewSampleReady == 0) && (--timeout != 0));
    timeout = 5000 ;
    // TurnOffTimersA1();  // Halt Timer
    NewSampleReady = 0;
    uint16_t x = 0 ;
    x = ADC10MCTL0;
    if((x & Battery_ADC_Channel ) == Battery_ADC_Channel)
    {
      ADC_CLR_IT_FLAG;
      while(((ADC_IT_FLAG)==0)&& (--timeout != 0));
      timeout = 5000 ;
      BatteryLevel = ADC10MEM0;
      BatteryLevel *= 74;
      //BatteryLevel = (BatteryLevel + (uint16_t)test_val) / (uint16_t)2;
      ADC_ENABLE_IT;
      return BatteryLevel/1000;
    }
  }
  return 0;
}

//=============================================================================
// FUNCTION
//=============================================================================
// Name           : CTRL_CTRL_startBatteryRead
// Description    : enable the start of conversion on ADC to get new reading for battery
//-----------------------------------------------------------------------------
// Author         : HB
// Date           : 23/05/2011
//=============================================================================
void CTRL_startBatteryRead(void)
{
  ADCInit_Battery();
  ADC_CLR_ENABLE_CONVERSION_FLAG ;  // Ensure ENC is clear
  ADC10CTL1 = ADC10SHP + ADC10CONSEQ_0 + ADC10SSEL_2  + ADC10DIV_7;  // Single channel, once, ADC10CLK=MCLK, Channel 1
  ADC_START_CONVERSION ;       // Start conversion
}


#endif













#endif

#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
/*!
* @brief Check magnetic interference level.
*
* @par Description:
* Check the magnetic interference level and its state. Publish event in case
* its state is changed.
*
* @return
* - None.
*/
void check_mgnt(void)
{
  /*!@par Pseudo Code:*/
#if (CTRL_MGNT_SW == 1)
  if(CTRL_CHECK_MGNT_SWITCH())
#elif (CTRL_MGNT_SENSOR == 1)
    if(CTRL_CHECK_MGNT_SENSOR())
#endif
    {
      /*! - If there is magnetic interference.*/
      if(flags_byte & MGNT_HIGH_STRT_MASK)
      {
        /*! - If it isn't first time to detect interference existence.*/
        if((!CTRL_MGNT_STAT) && (mgnt_min_cntr > ctrl_cfg.mgnt.detect_time))
          /*! - Publish magnetic high event.*/
          ctrl_event(CTRL_MGNT_HIGH_STRT);
      }
      else
      {
        /*! - If it is first time to detect interference existence.*/
        /*! - Set magnetic high start flag.*/
        flags_byte |= MGNT_HIGH_STRT_MASK;
        /*! - Clear magnetic low start flag.*/
        flags_byte &= (~MGNT_LOW_STRT_MASK);
        /*! - Reset second counter.*/
        mgnt_min_cntr = 0;
      }
    }
    else
    {
      /*! - If there is no magnetic interference.*/
      if(flags_byte & MGNT_LOW_STRT_MASK)
      {
        /*! - If it isn't first time to detect interference existence.*/
        if((CTRL_MGNT_STAT) && (mgnt_min_cntr > ctrl_cfg.mgnt.restore_time))
          /*! - Publish magnetic high event.*/
          ctrl_event(CTRL_MGNT_HIGH_STOP);
      }
      else
      {
        /*! - If it is first time to detect interference existence.*/
        /*! - Set magnetic low start flag.*/
        flags_byte |= MGNT_LOW_STRT_MASK;
        /*! - Clear magnetic high start flag.*/
        flags_byte &= (~MGNT_HIGH_STRT_MASK);
        /*! - Reset second counter.*/
        mgnt_min_cntr = 0;
      }
    }
}
#endif

#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || \
defined(CTRL_DN_SW) || defined(CTRL_MD_SW)|| defined(CTRL_GPRS_MODULE_CVR_SW))
/*!
* @brief Check all the switches on the board.
*
* @par Description:
* Check MD switch and the four switches connected to the shift register (UP, DOWN,
* Cover, Terminal) switches.
*
* @return
* - None.
*/
void switches_task(void)
{
  /*!@par Pseudo Code:*/
#if (defined(TRF_USE_MD_KW) || defined(TRF_USE_MD_KVA) || defined(TRF_USE_MD_A))
#ifdef CTRL_MD_SW
  uint8_t reset_data;
#endif
#endif
#if(defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || \
  defined(CTRL_DN_SW))
/*! - Scan the 4 switches UP, DN, Cover, Terminal to get thier state.*/
update_sr_switches_state(0);
#endif
#ifdef EDIT_TIME_DATE_MANUALLY
scan_result_cpy = scan_result;
#endif
#if (defined(CTRL_DN_SW) || defined(CTRL_UP_SW))
check_cont_press();
#endif
#if(defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || \
defined(CTRL_DN_SW)|| defined(CTRL_GPRS_MODULE_CVR_SW))
/*! - Take actions according to the new switches status.*/
#ifndef DRU_PROJECT
if(pwr_up_sec_cntr > 1)
#endif
apply_sw_actions_pwr_up();
#endif
#ifdef CTRL_MD_SW
if(check_md_switch())
{
#if (defined(TRF_USE_MD_KW) || defined(TRF_USE_MD_KVA) || defined(TRF_USE_MD_A))
  reset_data = 3;
  /*! - Clear MD of tariff.*/
  CTRL_RST_TRF_MD(&reset_data);
#endif
  /*! - Clear MD of metering.*/
  CTRL_RST_MTR_MD();
  /*! - Record an event of clearing MD.*/
  ctrl_event(CTRL_MD_CLR_SW);
  /*! - Increment MD reset count.*/
  if(ctrl_sys_var.basic.md_reset_count != 0xffff)
    ctrl_sys_var.basic.md_reset_count++;
}
#endif
#ifdef MTR_INDIRECT
 if (CTRL_READ_EXTERNAL_REALY_STATUS_SW())
  {
   // ctrl_sys_var.basic.meter_status[5] |= (CTRL_EXTERNAL_RELAY_MASK);    TODO name changed 
  } 
 else
  {
    //ctrl_sys_var.basic.meter_status[5] &= ~(CTRL_EXTERNAL_RELAY_MASK);
  }
  
  if (!CTRL_READ_SWITCHE_CONNECTION_SW())
  {
#if 0
    if (ctrl_sys_new_var.indirect_enclosure_connection_sw_state != CTRL_SWITCHE_CONNECTION_AUTO)
    {
      memcpy((uint8_t*)&ctrl_sys_new_var.indirect_enclosure_connection_sw_date_time,&(ctrl_cfg.rtc_time.minute),sizeof(ctrl_sys_new_var.indirect_enclosure_connection_sw_date_time));
    }
#endif
    ctrl_sys_new_var.indirect_enclosure_connection_sw_state = CTRL_SWITCHE_CONNECTION_AUTO;
  }
  else
  {
    if (ctrl_sys_new_var.indirect_enclosure_connection_sw_state != CTRL_SWITCHE_CONNECTION_MANUAL)
    {
      memcpy((uint8_t*)&ctrl_sys_new_var.indirect_enclosure_connection_sw_date_time,&(ctrl_sys_var_optional_feature.rtc_time.minute),sizeof(ctrl_sys_new_var.indirect_enclosure_connection_sw_date_time));
    }
    ctrl_sys_new_var.indirect_enclosure_connection_sw_state = CTRL_SWITCHE_CONNECTION_MANUAL;
  }

#endif

}
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

void ctrl_Event_Notification_Send()
{
#ifdef DLMS_ENABLED
  
  /*!@par Pseudo Code:*/   
  if (ctrl_Event_Notification_flag & COVER_OPEN_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(COVER_OPEN_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_COVER_OPEN);
  }
  if (ctrl_Event_Notification_flag & TERMINAL_OPEN_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(TERMINAL_OPEN_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_TERMINAL_OPEN);
  }
  if (ctrl_Event_Notification_flag & RELAY_ERR_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(RELAY_ERR_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_RELAY_ERR);
  }
  if (ctrl_Event_Notification_flag & RVRS_CRNT_A_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(RVRS_CRNT_A_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_RVRS_CRNT_A);
  }
  if (ctrl_Event_Notification_flag & RVRS_CRNT_B_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(RVRS_CRNT_B_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_RVRS_CRNT_B);
  }
  if (ctrl_Event_Notification_flag & RVRS_CRNT_C_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(RVRS_CRNT_C_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_RVRS_CRNT_C);
  }
  if (ctrl_Event_Notification_flag & FLT_ENRG_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(FLT_ENRG_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_FLT_ENRG);
  }
  if (ctrl_Event_Notification_flag & OVER_LOAD_A_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(OVER_LOAD_A_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_OVER_LOAD_A);
  }
  if (ctrl_Event_Notification_flag & OVER_LOAD_B_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(OVER_LOAD_B_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_OVER_LOAD_B);
  }
  if (ctrl_Event_Notification_flag & OVER_LOAD_C_EVENT_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(OVER_LOAD_C_EVENT_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_OVER_LOAD_C);
  }
  if (ctrl_Event_Notification_flag & GPRS_COVER_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(GPRS_COVER_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_GPRS_COVER);
  }
  if (ctrl_Event_Notification_flag & MIS_POT_A_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(MIS_POT_A_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_MIS_POT_A);
  }
  if (ctrl_Event_Notification_flag & MIS_POT_B_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(MIS_POT_B_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_MIS_POT_B);
  }
  if (ctrl_Event_Notification_flag & MIS_POT_C_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(MIS_POT_C_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_MIS_POT_C);
  }
  if (ctrl_Event_Notification_flag & OVR_VOLT_A_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(OVR_VOLT_A_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_OVR_VOLT_A);
  }
  if (ctrl_Event_Notification_flag & OVR_VOLT_B_MASK ) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(OVR_VOLT_B_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_OVR_VOLT_B);
  }
  if (ctrl_Event_Notification_flag & OVR_VOLT_C_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(OVR_VOLT_C_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_OVR_VOLT_C);
  }
  if (ctrl_Event_Notification_flag & UNDR_VOLT_A_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(UNDR_VOLT_A_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_UNDR_VOLT_A);
  }
  if (ctrl_Event_Notification_flag & UNDR_VOLT_B_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(UNDR_VOLT_B_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_UNDR_VOLT_B);
  }
  if (ctrl_Event_Notification_flag & UNDR_VOLT_C_MASK) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(UNDR_VOLT_C_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_UNDR_VOLT_C);
  }      
  if (ctrl_Event_Notification_flag & POWER_UP_MASK ) 
  {
    CTRL_CLEAR_EVENT_NOTIFICATION_FLAG(POWER_UP_MASK);
    Cal_Evt_gSendEventNotification(LOG_ID_1,CTRL_LOG1_POWER_UP);
  }
#endif
}
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
void ctrl_first_init(void)
{
  /*!@par Pseudo Code:*/
  /*! - Disable interrupts.*/
  CTRL_DISABLE_INT();
  /*! - Check if power line is high or the meter is working on the battery.*/
  while(CTRL_READ_PWR_FAIL_PIN())
  {
    //low_power_mode();
    CTRL_ENABLE_INT();
    vDelay_SW(1000);
    /*! - Double check the pin after a delay.*/
    if(CTRL_READ_PWR_FAIL_PIN())
    {      /*! - Go to halt mode.*/
      go_to_halt();
    }
  }
  
  /*! - Disable interrupts.*/
  CTRL_DISABLE_INT();
  ctrl_flags &= (~CTRL_PWR_DWN_MASK);
  
  set_power_up_flag();
  
}
/*!
* @brief Do operations done every timer tick.
*
* @par Description:
* Do periodic operations done every timer tick.
*
* @return
* - None.
*/
extern uint8_t Check_var; 
void new_timer_tick(void)
{// TODO: handle all switches needed
  /*!@par Pseudo Code:*/
  CTRL_CALB_PIN_IN();
  
    /*! - Read calibration pin input.*/
  calb_pin_state = calb_pin_state << 1;
  calb_pin_state |= CHECK_CALB_PIN();
  
  /*! - Check if the jumper is put.*/
#if !defined (EM110_BOARD) || !defined (EM330_BOARD) || defined(EM130_BOARD) || defined(EM210_BOARD)
#ifdef ANSI_STACK
  if((calb_pin_state & CALB_PIN_MASK) == CALB_PIN_MASK)
#else
  if(((CTRL_CVR_OPN_STAT &&  CTRL_TRMNL_OPN_STAT) && ((calb_pin_state & CALB_PIN_MASK) == CALB_PIN_MASK)))
#endif
#else
  if(((CTRL_CVR_OPN_STAT && CTRL_TRMNL_OPN_STAT) && (calb_pin_state & CALB_PIN_MASK)))
#endif
    /*! - Tell metering that calibration jumper is put.*/
  {
    /* CTRL_TELL_MTR_CALB_JMPR(); */  /* metering not built for the DRU */
    Check_var = 0;
    calb_pin_state = 0;
  }
#if (defined(CTRL_UP_SW) || defined(CTRL_DN_SW))
  /*! - Increment counter used to detect continous press. The calibration-mode
      guard is gone with metering, which is not built for the DRU -- the meter can
      never be in calibration mode, so the counter always runs.*/
  cont_press_counter++;
  /*! - if half second is elapsed set a flag.*/
  if(cont_press_counter == cont_press_period)
    ctrl_sys_var_optional_feature.flag1 |= CONT_PRESS_FLAG;
#endif
#ifdef IEC_62056_21_SLAVE
  /*! - Tell IEC with new timer tick.*/
  CTRL_TELL_IEC_NEW_TIMER_TICK();
#endif
#ifdef RF_LINK_FEATURE
  CTRL_TELL_RF_NEW_TMR_TICK();
#endif
#ifdef CTRL_BZR
  if(beep_counter)
    beep_counter--;
#ifdef Beep_Beep_Feature
  if( ((beep_beep_Counter == 10) && (beep_counter == 0)) && (++Counter_Buzzer == Loop_Time_Buzzer))
  {
    beep_counter = beep_beep_Counter;
    buzzer_enable =1;
    beep_beep_Counter =0;
    Counter_Buzzer =0;
  }
#endif /*Beep_Beep_Feature*/
#endif
#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_UP_SW) || \
  defined(CTRL_DN_SW) || defined(CTRL_MD_SW)|| defined(CTRL_GPRS_MODULE_CVR_SW))
/*!- Check all switches on the board.*/
#ifndef DRU_PROJECT
if (pwr_up_sec_cntr > 1)
#endif
switches_task();
#endif
#ifdef CTRL_ACTIONS
/*! - Form the action to apply due to all latched and non latched tampers
* and alarms.*/
deduce_actions();
/*! - Apply deduced actions.*/
apply_actions();
#endif
#if (defined(CTRL_CVR_SW) || defined(CTRL_TRMNL_SW) || defined(CTRL_MGNT_SW) ||\
defined(CTRL_MGNT_SENSOR) || defined(MTR_RVS_TMPR) || defined(MTR_ERTH_TMPR)\
  || defined(MTR_MIS_POT_TMPR) || defined(MTR_THREE_PH)|| defined(CTRL_GPRS_MODULE_CVR_SW))
/*! - Update tamper counter.*/
ctrl_sys_var.tmpr_count = 0;
#ifdef CTRL_CVR_SW
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_CVR_OPN];
#endif
#ifdef CTRL_TRMNL_SW
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_TRMNL_OPN];
#endif
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_MGNT_HIGH];
#endif
#ifdef MTR_RVS_TMPR
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_RVRS_CRNT];
#endif
#ifdef MTR_ERTH_TMPR
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_FLT_ENRG];
#endif
#ifdef MTR_MIS_POT_TMPR
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_MIS_POT];
#endif
#ifdef MTR_THREE_PH
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.non_latch_evnt_count[CTRL_PHS_SEQ];
#endif
#if defined CTRL_GPRS_MODULE_CVR_SW && defined MTR_THREE_PH
ctrl_sys_var.tmpr_count += ctrl_sys_var.basic.latch_evnt_count[CTRL_GPRS_MODULE_CVR_OPN];
#endif
#endif
}


/*!
* @brief Do operations done every one hour.
*
* @par Description:
* Do periodic operations done every one hour.
*
* @return
* - None.
*/
void new_hr(void)
{
  /*!@par Pseudo Code:*/
  /*! - Save system variables every 6 hours.*/
  if(((CTRL_READ_HR() % 6 )== 0) && (CTRL_READ_MIN() == 0)&& (CTRL_READ_SEC() == 0) &&(!CTRL_PWR_DN_STAT) && (pwr_up_sec_cntr > PWR_UP_NO_REC_CITICAL_TIME) )
  {
    ctrl_event(CTRL_SAVE_CRITICAL_DATA);
  }
}

/*!
* @brief Do operations done every one minute.
*
* @par Description:
* Do periodic operations done every one minute.
*
* @return
* - None.
*/
void new_min(void)
{
#ifdef RF_CC1120
 cc1120_reinit_counter++; 
  if(cc1120_reinit_counter>=5)
  {
    CC1120_Reinit_Chip();
    cc1120_reinit_counter=0;
  }
#endif
  
#ifdef  KEYPAD_FEATURE
  keypad_reinit_counter++;
  if(keypad_reinit_counter>=3)
  {
    Keypad_init();
    reset_sts_keypad_buf();
    reset_sts_diplay_buf();
    keypad_reinit_counter=0;
  }
#endif
#ifdef  DOT_MATRIX_LCD_ENABLE
/* Calibration-mode guard dropped: metering is not built for the DRU, so the
   meter is never in calibration mode and this is always true. */
if(1)
{
  DOT_MATRIX_reinit_counter++;
  if(DOT_MATRIX_reinit_counter>=5)
  {
    Dot_Matrix_GPIO_INIT(); 
    Dot_LCD_Init(0);
    DOT_MATRIX_reinit_counter=0;
  }
}
#endif
  /*! - this function used to check if eeprom fail or not every 1 min */
  //check_Eeprom_Fail();
  
  /*!@par Pseudo Code:*/
  /*! - Check power on hours variable to increment.*/
  power_on_minute_counter++;
  if(power_on_minute_counter == 60)
  {
    if(ctrl_sys_var.basic.power_on_hour != 0xffffffff)
      ctrl_sys_var.basic.power_on_hour++;
    power_on_minute_counter = 0;
  }
#ifdef CTRL_RLY
  /*! - Initialize the relay (Apply its current status).*/
  if((!(CTRL_READ_MIN() % RELAY_UPDATE_PERIOD)) && (pwr_up_sec_cntr > PWR_UP_NO_REC_TIME))
    RELAY_INIT();
#endif
#ifdef TARIFF_SYS
  /*! Inform tariff by new minute event.*/
  /* CTRL_TELL_TRF_NEW_MIN(); */  /* file system / tariff not built for the DRU */
#endif
#ifdef CTRL_RLY
  /*! - Check if there current drawn and relay should be open.*/
  if (ctrl_test_relay_flag == 0)
  {
#ifdef ANSI_STACK    
    if((alarm_get_Relay_status()== RLY_ERROR)|| (alarm_get_Relay_status()== RLY_CLOSE) && CTRL_RLY_STAT)   
#else//ANSI_STACK  
#ifdef MTR_THREE_PH
  /* Phase-current test dropped: metering is not built for the DRU, so there is
     no current reading and this relay hardware-error check cannot trigger. */
  if(0 && CTRL_RLY_STAT)   //current in milliampere
#else
    if(CTRL_READ_CURRENT()>100 && CTRL_RLY_STAT)   
#endif
#endif//ANSI_STACK       
    {
      /*! - If the error occured before.*/
      if(flags_byte2 & RELAY_ERR_FLAG)
      {
        /*! - If the error not published.*/
        if(!CTRL_HRDWARE_ERR_RELAY_STAT)
        {
#ifdef ANSI_STACK
          ctrl_event(CTRL_RELAY_FAIL_STRT);          
#else
          /*! - Publish relay error start.*/
          ctrl_event(CTRL_RELAY_ERR_STRT);  
          ctrl_sys_new_var.error_contactor++;
#endif

#ifdef DLMS_ENABLED
          CTRL_SET_EVENT_NOTIFICATION_FLAG(RELAY_ERR_EVENT_MASK);
#endif
        }
      } 
      /*! - If it is first time to have relay error.*/
      else
      {
        /*! - Set the flag of relay error.*/
        flags_byte2 |= RELAY_ERR_FLAG;
        /*! - Try to initialize the relay (Apply its current status).*/
        RELAY_INIT();
      }
    }
#ifdef ANSI_STACK
 else if(CTRL_HRDWARE_ERR_RELAY_STAT && !CTRL_RLY_STAT && (alarm_get_Relay_status()== RLY_CLOSE))
#else    
  else if(CTRL_HRDWARE_ERR_RELAY_STAT && !CTRL_RLY_STAT && CTRL_READ_CURRENT() > 100)
#endif    
  {
#ifdef ANSI_STACK
    ctrl_event(CTRL_RELAY_FAIL_STOP); 
#else
    /*! - Publish relay error stop.*/
    ctrl_event(CTRL_RELAY_ERR_STOP);  
#endif
    
      /*! - Clear flag of relay error.*/
      flags_byte2 &= (~RELAY_ERR_FLAG);
    }
    else
    {
      flags_byte2 &= (~RELAY_ERR_FLAG);
    }
  }
#endif 
#ifdef GROUP_METERING    
    GM_readout_data_timing++;
if (GM_readout_data_timing >= 10 )
{
  GM_rise_readout_data_flag =1;
  GM_readout_data_timing=0;
}
#endif
}

/*!
* @brief Do operations done every one second.
*
* @par Description:
* Do periodic operations done every one second.
*
* @return
* - None.
*/
void new_sec(void)
{
CommIncrementDruCounterTimeOut();
#ifdef DLMS_UI
UI_timer_counter();
#endif
#ifdef STS_ENABLE
STS_timer_counter();
#endif
#if defined (HDLC_PROTOCOL) || defined (DLMS_ENABLED) 
  HDLC_timer_counter();
#endif
  /*!@par Pseudo Code:*/
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1) || defined(CTRL_BTRY))
  if(!(CTRL_READ_SEC() % BTRY_MGNT_UPDATE_PERIOD))
    flags_byte |= CHK_BTRY_MGNT_MASK;
#endif
  /*! - Increment power up seconds counter.*/
  if(pwr_up_sec_cntr<=PWR_UP_NO_REC_TIME)
    pwr_up_sec_cntr++;
  
  timeout_cntr++;
  reset_with_corruption_counter++;
  HDLC_Activity_Counter++;
#ifdef CTRL_BZR
  /*! - Decrement silent period counter of the buzzer if the configured value of
  * the silent period don't equal zero.*/
  if(ctrl_sys_var.bzr_silent_sec_cntr && ctrl_cfg.tmpr.bzr_silent_period)
    ctrl_sys_var.bzr_silent_sec_cntr--;
#endif
#if (defined(MTR_ENABLE_LMT) && defined(CTRL_RLY))
  /*! - Increment overload seconds counter.*/
  if(ctrl_sys_new_var.ovr_load_sec_cntr != 0xffff)
    ctrl_sys_new_var.ovr_load_sec_cntr++;
#endif
#if (defined(CTRL_RLY) || defined(CTRL_BZR))
  /*! - Decrement blind period counter of hint.*/
  if(blind_hint_period_sec_counter)
    blind_hint_period_sec_counter--;
#endif
  /*! - Inform other modules by new second event.*/
  /*! - Display new second event*/
  /* CTRL_TELL_DISPLAY_NEW_SEC(); */  /* display module not built for the DRU */
#ifdef IEC_62056_21_SLAVE
  /*! - IEC62056-21 new second event*/
  CTRL_TELL_IEC_NEW_SEC();
#endif
#ifdef ANSI_STACK
 if(cmd_cycles <= MAX_HANDLING_CMD_CYCLES)
  cmd_cycles++;
CTRL_TELL_OPTICAL_NEW_SEC();
if(communication_error_flag == 1)
CTRL_TELL_OPTICAL_DISABLE_NEW_SEC();
#endif
  /* CTRL_TELL_MTR_NEW_SEC(); */  /* metering not built for the DRU */
#ifdef RFID_FEATURE
  CTRL_TELL_RFID_NEW_SEC();
#endif
#ifdef GPRS_FEATURE
  CTRL_TELL_GPRS_NEW_SEC();
#endif
#ifdef RF_LINK_FEATURE
  CTRL_TELL_RF_LINK_NEW_SEC();
#ifdef RF_LINK_TI_CC1120_ENABLE
  CTRL_TELL_TI_CC1120_NEW_SEC();
#endif
#endif
#if ((CTRL_MGNT_SW == 1) || (CTRL_MGNT_SENSOR == 1))
  /*! - Increment magnetic seconds counter.*/
  if(mgnt_min_cntr != 0xff)
    mgnt_min_cntr++;
#endif
/* New-second notifications to tariff and to the file system removed: neither
   module is built for the DRU. */
#ifdef IEC_62056_21_SLAVE
  //optical_sec_counter++;
 // if (optical_sec_counter >= 2)
  {
  //  iec62056_21_lcd_icon_flg &= ~IEC_OPTICAL;
  }
#endif
  if (Boot_loader_flag == RECIEVE_BOOTLOADER_DATA)
    COMM_BOOTLOADER_NEW_TIMER_TICK();
  else
    Boot_loader_timeout = 0;
#ifdef RFID_DEBUG_MEASUR_SPEED
  rfid_speed_sec++;
#endif
  if(ctrl_test_relay_flag)
  {
    ctrl_test_relay_second++;
    ctrl_test_relay_second_one_shot=1; // TODO
  }
  
#ifdef STS_ENABLE
  STS_increment_Timer();
#endif
  if(ctrl_sys_new_var.ctrl_start_open_relay_test == 1)
  {
    uint32_t tmp_cfg=ctrl_sys_new_var.ctrl_start_open_relay_test_min_cfg;
    
    if (tmp_cfg == 0)
      tmp_cfg = 5;
  
    ctrl_start_open_relay_test_sec_counter++;
    if(ctrl_start_open_relay_test_sec_counter >= tmp_cfg*(uint32_t)60)
    {
      ctrl_sys_new_var.ctrl_start_open_relay_test = 0;
      ctrl_cmd(CTRL_EXIT_OPEN_RELAY_MODE);
    }
  }
  else
    ctrl_start_open_relay_test_sec_counter = 0;

}


void Ctrl_Set_Date_Time_Fixed(void)
{
  ctrl_sys_var_optional_feature.rtc_date.year = 26;
  ctrl_sys_var_optional_feature.rtc_date.day = 6;
  ctrl_sys_var_optional_feature.rtc_date.month = 8;
  ctrl_sys_var_optional_feature.rtc_date.day_of_week = 1;
}
/*!
* @brief The basic function of control system
*
* @par Description:
* Do all tasks related to control system according to the new events:
* - Scan switches.
* - Checking new events and take proper actions.
* - Check battery level and magnetic interference level.
* - Check power down or power up and do thier tasks  .
*
* @return
* - None.
*/
void ctrl_task(void)
{
  UnEntering_ctrl_task_Num = 0 ;
  rtc_task();
  /*! - Check if there is new second event.*/
  if(flags_byte2 & NEW_SEC_FLAG)
  {
    flags_byte2 &= (~NEW_SEC_FLAG);
    new_sec();
  }
    /*! - Check for scan switches flag.*/
    if(flags_byte2 & SR_SCAN_MASK)
    {
      /*! - Clear the flag.*/
      flags_byte2 &= (~SR_SCAN_MASK);
      /*! - New timer tick.*/
      new_timer_tick();
    }
}

uint8 get_up_flag(void)
{
  return(ctrl_sys_var_optional_feature.flag1 & UP_SW_STAT);
}
/*!
* @brief used to reset up switch state
*
* @par Description:
*no return
*/
void reset_up_flag(void)
{
  ctrl_sys_var_optional_feature.flag1 &= ~UP_SW_STAT;
}

void set_power_up_flag(void)
{
  flags_byte |= PWR_UP_MASK;
}

void set_power_down_flag(void)
{
  ctrl_flags |= CTRL_PWR_DWN_MASK;
}

uint8_t is_power_down(void)
{
  return ctrl_flags & CTRL_PWR_DWN_MASK;
}
uint8_t check_value(uint8_t value)
{
  if (value > 0)
    return TRUE;
  return FALSE;
}

void ctrl_get_selected_meter_status(uint8_t *ptr)
{  
  ptr[0] = 0;
  ptr[1] = 0;
  ptr[0] = check_value(!CTRL_BTRY_OCCURED) | (check_value(CTRL_LOW_CRDT_1ST_ALRM_STAT))<< 1 | (check_value(CTRL_LOW_CRDT_2ND_ALRM_STAT))<< 2 | (check_value(CTRL_NO_CRDT_STAT))<< 3 | (check_value(CTRL_OVR_LOAD_CHA_STAT)|check_value(CTRL_OVR_LOAD_CHB_STAT)|check_value(CTRL_OVR_LOAD_CHC_STAT))<<4 | (check_value(CTRL_OVR_VOLT_CHA_STAT|CTRL_OVR_VOLT_CHB_STAT|CTRL_OVR_VOLT_CHC_STAT))<<5 | check_value(CTRL_UNDR_VOLT_CHA_STAT|CTRL_UNDR_VOLT_CHB_STAT|CTRL_UNDR_VOLT_CHC_STAT)<<6 /*| (check_value(CTRL_PHASE_SEQ_CHA_STAT|CTRL_PHASE_SEQ_CHB_STAT|CTRL_PHASE_SEQ_CHC_STAT))<<7*/ ; //added by eng esmail
#ifdef GPRS_FEATURE
  ptr[1] = check_value(CTRL_CVR_OPN_OCCURED) | (check_value(CTRL_TRMNL_OPN_OCCURED))<< 1 | (check_value(CTRL_RVRS_CRNT_CHA_OCCURED|CTRL_RVRS_CRNT_CHB_OCCURED|CTRL_RVRS_CRNT_CHC_STAT))<< 2 | (check_value(CTRL_FLT_ENRG_CHA_OCCURED|CTRL_FLT_ENRG_CHB_STAT|CTRL_FLT_ENRG_CHC_OCCURED))<< 3 | (check_value(CTRL_MIS_POT_CHA_STAT|CTRL_MIS_POT_CHB_STAT|CTRL_MIS_POT_CHC_STAT))<<4 | (check_value(CTRL_RLY_STAT))<<5 | check_value(CTRL_GPRS_MODULE_CVR_OPN_OCCURED)<<6 | check_value(CTRL_HRDWARE_ERR_RELAY_STAT)<<7 ;
#else
  ptr[1] = check_value(CTRL_CVR_OPN_OCCURED) | (check_value(CTRL_TRMNL_OPN_OCCURED))<< 1 | (check_value(CTRL_RVRS_CRNT_CHA_OCCURED|CTRL_RVRS_CRNT_CHB_OCCURED|CTRL_RVRS_CRNT_CHC_STAT))<< 2 | (check_value(CTRL_FLT_ENRG_CHA_OCCURED|CTRL_FLT_ENRG_CHB_STAT|CTRL_FLT_ENRG_CHC_OCCURED))<< 3 |  (check_value(CTRL_RLY_STAT))<<5  | check_value(CTRL_HRDWARE_ERR_RELAY_STAT)<<7 ;
#endif
}


void save_rtc_flag(void)
{
  uint8_t rtc_flag = CTRL_RTC_INIT_FLAG;
  ////rtc_flag =1 ;
 // EEPROM_write(Data_eeprom_address ,&rtc_flag ,  EEPROM_RTC_INIT_FLAG_ADD , 1);
}

#ifdef MTR_ENABLE_LMT
#ifdef MTR_THREE_PH
void ctrl_overload_state (uint8_t ol_state,uint8_t channel)
{
  uint8_t action,index;
  
  if(!ol_state)
  {
    if(channel == channel_cpy)
    {
      check_and_reset_ov_load_cntr(SEQ_NORMAL);
      ctrl_sys_new_var.ovr_load_sec_cntr = 0;
      ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
    }
  }
  /*! - Check if the event is overload start and its configured action is
  * opening the relay.*/
  else if((~ctrl_sys_new_var.ovr_load_times_cntr)&&\
          !(CTRL_OVR_LOAD_CHA_STAT)&&\
            !(CTRL_OVR_LOAD_CHB_STAT)&&\
              !(CTRL_OVR_LOAD_CHC_STAT)&& !(ctrl_sys_var_optional_feature.flag1 &OVER_LOAD_MODE))
  {
    if(channel==0 || channel==1 )
    {
      index = match_array[CTRL_OVR_LOAD_A_STRT - CTRL_DATA_CRPT_MIN];
    }
    else if(channel==2)
    {
      index = match_array[CTRL_OVR_LOAD_B_STRT - CTRL_DATA_CRPT_MIN];
    }
    else
    {
      index = match_array[CTRL_OVR_LOAD_C_STRT - CTRL_DATA_CRPT_MIN];
    }
    action = (*(ctrl_cfg.tmpr.non_latch_action + index));
    /*! - Increment counter of overload used for special handling of overload tamper.*/
      ctrl_sys_new_var.ovr_load_times_cntr++;
    if (action & ACT_OPN_RLY_MASK)
    {
      channel_cpy = channel;
      ctrl_sys_var_optional_feature.flag1 |= OVER_LOAD_MODE;
      
      if (ctrl_sys_new_var.ovr_load_times_cntr_flag != 0)
      {
        if (ctrl_sys_new_var.ovr_load_times_cntr >= ctrl_cfg.overload.persistence_times)
        {
          if(ctrl_cfg.overload.persistence_times)
          {
            if (!CTRL_OVR_LOAD_CHA_STAT)
            ctrl_event(CTRL_OVR_LOAD_A_STRT);
             ctrl_sys_var_optional_feature.flags_byte1 &= ~OVER_LOAD_MODE;
          }
        }
      }
      
      if(!CTRL_RLY_STAT)
      {
        toggle_relay();
        ctrl_sys_new_var.ovr_load_sec_cntr = 0;
      }
    }
    else
    {
      if (!CTRL_OVR_LOAD_CHA_STAT)
      ctrl_event(CTRL_OVR_LOAD_A_STRT);
#ifdef DLMS_ENABLED
      CTRL_SET_EVENT_NOTIFICATION_FLAG(OVER_LOAD_A_EVENT_MASK);
#endif
    }
  }
}
#else
void ctrl_overload_state (uint8_t ol_state)
{
  uint8_t action,index;
  
  if(!ol_state)
  {
    check_and_reset_ov_load_cntr(SEQ_NORMAL);
    ctrl_sys_new_var.ovr_load_sec_cntr = 0;
    ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
    if (CTRL_OVR_LOAD_CHA_STAT)
      ctrl_event(CTRL_OVR_LOAD_A_STOP);
  }
  /*! - Check if the event is overload start and its configured action is
  * opening the relay.*/
  else if(~ctrl_sys_new_var.ovr_load_times_cntr)
  {
    index = match_array[CTRL_OVR_LOAD_A_STRT - CTRL_DATA_CRPT_MIN];
    action = (*(ctrl_cfg.tmpr.non_latch_action + index));
    /*! - Increment counter of overload used for special handling of overload tamper.*/
    ctrl_sys_new_var.ovr_load_times_cntr++;
    if (action & ACT_OPN_RLY_MASK)
    {
      ctrl_sys_var_optional_feature.flag1 |= OVER_LOAD_MODE;
      if (ctrl_sys_new_var.ovr_load_times_cntr_flag != 0)
      {
        if (ctrl_sys_new_var.ovr_load_times_cntr >= ctrl_cfg.overload.persistence_times)
        {
          if(ctrl_cfg.overload.persistence_times)
          {
            if (!CTRL_OVR_LOAD_CHA_STAT)
            ctrl_event(CTRL_OVR_LOAD_A_STRT);
            ctrl_sys_var_optional_feature.flag1 &= ~OVER_LOAD_MODE;
          }
        }
      }
      if(!CTRL_RLY_STAT)
      {
        toggle_relay();
        ctrl_sys_new_var.ovr_load_sec_cntr = 0;
      }
      
    }
    else
    {
      ctrl_sys_new_var.overload_current_value = MTR_GET_CUR(0)/1000;
      if (!CTRL_OVR_LOAD_CHA_STAT)
      ctrl_event(CTRL_OVR_LOAD_A_STRT);
    }
  }
}
#endif
uint8_t ctrl_check_overload(void)
{
  return (ctrl_sys_var_optional_feature.flag1 &(OVER_LOAD_MODE));
}

void reset_specific_tamper(uint8_t ptr)
{
  uint8_t element ,shift,event,index;
  uint8_t reset_index = 0 , tamper_event_index =0, reset_last_tmpr,reset_tamper_watania_index;
  reset_last_tmpr = 0xff;
  reset_tamper_watania_index = 0xff;
  
  if (ptr >= 20)
  {
    event = CTRL_LATCH_EVNT_STOP_MIN+ptr-20;
  }
  else
  {
    event = CTRL_NON_LATCH_EVNT_STOP_MIN+ptr;
  }
  
  if (event == CTRL_CVR_OPN_STOP )
  {
    ctrl_sys_new_var.tech_id_cover_terminal[0] = tech_id_temp;
  }
  if (event == CTRL_TRMNL_OPN_STOP)
  {
    ctrl_sys_new_var.tech_id_cover_terminal[1] = tech_id_temp;
  }
  
  if (!(event == CTRL_CVR_OPN_STOP || event == CTRL_TRMNL_OPN_STOP) )
  {
    ctrl_sys_new_var.type_operation_last_tech_id = ptr;
  }
  
  index = match_array[event - CTRL_DATA_CRPT_MIN-100];
//  *(ctrl_sys_var.basic.non_latch_evnt_count + index) = 0;
  
  element = ((event - CTRL_COMM_ERR_STOP_MIN) / 8) + 1;
  shift = (event - CTRL_COMM_ERR_STOP_MIN) % 8;
  
//  ctrl_sys_var.basic.meter_status[element] &= (~(1 << shift));
//  ctrl_sys_var.shadow_status[element] &= (~(1 << shift));   
  if (ptr == CTRL_OVER_LOAD - 2)
  {
#if (defined(MTR_ENABLE_LMT) && defined(CTRL_RLY))
    /*! - Reset overload counters.*/
    ctrl_sys_new_var.ovr_load_sec_cntr = 0;
   // ctrl_sys_new_var.ovr_load_times_cntr = 0;
    ctrl_sys_new_var.overload_current_value = 0;
    ctrl_sys_var_optional_feature.flag1 &= ~(OVER_LOAD_MODE);
    reset_index = 1;
    tamper_event_index = 5;
    reset_last_tmpr = 5;
    reset_tamper_watania_index = 6;
#endif
  }
  else if (ptr == CTRL_BTRY_LOW - 2)
  {
    btry_prst = 0;
    reset_index = 2;
    reset_last_tmpr = 8;
    reset_tamper_watania_index = 2;
    ctrl_sys_new_var.ctr_no_bat_flage = 0;
  }
#ifdef MTR_THREE_PH
  else if ((ptr == (CTRL_RVRS_CRNT + 21))||(ptr == (CTRL_RVRS_CRNT + 22)) )  
#else
  else if ((ptr == (CTRL_RVRS_CRNT + 20))||(ptr == (CTRL_RVRS_CRNT + 21)) )
#endif
  {
    reset_index = 3;
#ifdef MTR_THREE_PH
    if(ptr == (CTRL_RVRS_CRNT + 21))
#else
    if(ptr == (CTRL_RVRS_CRNT + 20))
#endif
    {
      tamper_event_index = 3;
      reset_last_tmpr = 2;
    }
#ifdef MTR_THREE_PH
    else if( ptr == (CTRL_RVRS_CRNT + 22) )
#else
    else if( ptr == (CTRL_RVRS_CRNT + 21) )
#endif
    {
      reset_last_tmpr = 9;
      tamper_event_index = 6;
    }
    reset_tamper_watania_index = 3;
  }
  else if (ptr == (CTRL_CVR_OPN + 20) )
  {
    reset_index = 4;
    tamper_event_index = 2;
    reset_last_tmpr = 1;
    reset_tamper_watania_index = 0;
  }
  else if (ptr == (CTRL_TRMNL_OPN + 20) )
  {
    reset_index = 5;
    tamper_event_index = 1;
    reset_last_tmpr = 0;
    reset_tamper_watania_index = 1;
  }
#ifdef MTR_THREE_PH
  else if (ptr == (CTRL_FLT_ENRG + 23) )
#else
  else if (ptr == (CTRL_FLT_ENRG + 22) )
#endif
  {
    reset_index = 6;
    tamper_event_index = 4;
    reset_last_tmpr = 3;
    reset_tamper_watania_index = 4;
  }
  else if (ptr == 8) //Over_volt A
  {
    reset_last_tmpr = 6;
    reset_tamper_watania_index = 7;
  }
  else if (ptr == 11)
  {
    reset_last_tmpr = 7;
    reset_tamper_watania_index = 4;//under_volt A
  }
  
  if (reset_last_tmpr != 0xff)
    ctrl_reset_last_tamper(reset_last_tmpr);
  
  if( reset_index )
  {
    memcpy((uint8_t*)&ctrl_sys_new_var.last_reset_date_time_tamper[reset_index-1] ,(uint8_t*)&(ctrl_sys_var_optional_feature.rtc_time)+1 ,sizeof(ctrl_sys_new_var.last_reset_date_time_tamper[reset_index-1 ]));
  }
  if(tamper_event_index)
  {
    memset((uint8_t*)&ctrl_sys_new_var.tamper_event[tamper_event_index-1],0,sizeof(ctrl_sys_new_var.tamper_event[tamper_event_index-1]));
  }
  
 ctrl_reset_watania_tampers(reset_tamper_watania_index,1);
 

 *(ctrl_sys_var.basic.non_latch_evnt_count + index) = 0;
 
 element = ((event - CTRL_COMM_ERR_STOP_MIN) / 8) + 1;
 shift = (event - CTRL_COMM_ERR_STOP_MIN) % 8;
 
 ctrl_sys_var.basic.meter_status[element] &= (~(1 << shift));
 ctrl_sys_var.shadow_status[element] &= (~(1 << shift));
   if (ptr == CTRL_OVER_LOAD - 2)
  {
#if (defined(MTR_ENABLE_LMT) && defined(CTRL_RLY))
    /*! - Reset overload counters.*/
    ctrl_sys_new_var.ovr_load_times_cntr = 0;
#endif
  }
}

void ctrl_rset_overload(void)
{
  // ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
  if(ctrl_sys_var.basic.meter_status[2]&CTRL_OVR_LOAD_CHA_MASK||\
    ctrl_sys_var.basic.meter_status[2]&CTRL_OVR_LOAD_CHB_MASK||\
      ctrl_sys_var.basic.meter_status[2]&CTRL_OVR_LOAD_CHC_MASK)
  {
    if(ctrl_sys_var_optional_feature.flag1 &OVER_LOAD_MODE)
    {
      ctrl_sys_var.basic.meter_status[2]&=~CTRL_OVR_LOAD_CHA_MASK;
      ctrl_sys_var.basic.meter_status[2]&=~CTRL_OVR_LOAD_CHB_MASK;
      ctrl_sys_var.basic.meter_status[2]&=~CTRL_OVR_LOAD_CHC_MASK;
      ctrl_sys_new_var.ovr_load_times_cntr = 0;
      ctrl_sys_new_var.ovr_load_sec_cntr = 0;
      ctrl_sys_var_optional_feature.flag1 &=  ~(OVER_LOAD_MODE);
      if(CTRL_RLY_STAT)
        ctrl_sys_var.basic.meter_status[6] ^= CTRL_RLY_MASK;
      
    }
  }
  
}


void Bootloader_Activate(uint8_t  * Ptr)
{
  Boot_loader_flag=RECIEVE_BOOTLOADER_DATA;
}

void DRU_Activate(uint8_t  * Ptr)
{
  //set_connection_reset();
}
#endif
uint8_t ctrl_check_status(uint8_t latched_tamper_index)
{
  if (latched_tamper_index == CTRL_CVR_OPN)
  {
    return CTRL_CVR_OPN_STAT;
  }
  else if(latched_tamper_index == CTRL_TRMNL_OPN)
  {
    return(CTRL_TRMNL_OPN_STAT);
  }
#if ((CTRL_MGNT_SW ==1) || (CTRL_MGNT_SENSOR == 1))
  else if(latched_tamper_index == CTRL_MGNT_HIGH)
  {
    return (CTRL_MGNT_STAT);
  }
#endif
  else if(latched_tamper_index == CTRL_RVRS_CRNT)
  {
    return(CTRL_RVRS_CRNT_CHA_STAT | CTRL_RVRS_CRNT_CHB_STAT | CTRL_RVRS_CRNT_CHC_STAT);
  }
  else if(latched_tamper_index == CTRL_FLT_ENRG)
  {
    return (CTRL_FLT_ENRG_CHA_STAT | CTRL_FLT_ENRG_CHB_STAT | CTRL_FLT_ENRG_CHC_STAT);
  }
#ifdef MTR_MIS_POT_TMPR
  else if(latched_tamper_index == CTRL_MIS_POT)
  {
    return (CTRL_MIS_POT_CHA_STAT | CTRL_MIS_POT_CHB_STAT | CTRL_MIS_POT_CHC_STAT);
  }
#endif
#if 0//def GPRS_FEATURE TODO 
  else if(latched_tamper_index == CTRL_GPRS_MODULE_CVR_OPN)
  {
    return (CTRL_GPRS_MODULE_CVR_OPN_STAT);
  }
#endif
  return FALSE;
}


void ctrl_default_secondryOption ()
{
  memset (&ctrl_sys_var_second_optional_feature , 0 , sizeof(ctrl_sys_var_second_optional_feature));
#ifdef GPRS_FEATURE
  ctrl_sys_var_second_optional_feature.gprs_notifyTamperRepeateTime = GPRS_NOTIFY_REPEAT_TIME;
  ctrl_sys_var_second_optional_feature.gprs_resetTimeOut   = GPRS_SERVER_RESTART_TIMEOUT;
  ctrl_sys_var_second_optional_feature.gprs_waitAckTimeout = GPRS_WAIT_ACK_TIMEOUT;
  gprs_defult_notification ();
#endif
}


/*!
* @brief update times of start&stop events.
*
* @par Description:
* This function is used to update times of start&stop events(cover open,terminal open,reverse current phase A,B,fault energy,over load,).
*
* @param [in] event Code of the event.
* is listed in #ctrl_evnt_code_t.
*
* @return
* - None.
*/
void check_update_times_event(uint8_t event )
{
#ifdef LOCAL_WITH_DLMS
  uint8_t temp =0 ,i=0,general_year=0;
  uint8_t* ptr_time ,*ptr_time_first ;
  
  if (event == CTRL_TRMNL_OPN_STRT ||event == CTRL_TRMNL_OPN_STOP)
  {
    temp = 0;
  }
  else if (event == CTRL_CVR_OPN_STRT||event == CTRL_CVR_OPN_STOP)
  {
    temp = 1;
  }
  else if (event == CTRL_RVRS_CRNT_A_STRT||event == CTRL_RVRS_CRNT_A_STOP )
  {
    temp = 2;
  }
  else if (event == CTRL_FLT_ENRG_A_STRT||event == CTRL_FLT_ENRG_A_STOP )
  {
    temp = 3;
  }
  else if (event == CTRL_OVR_LOAD_A_STRT||event == CTRL_OVR_LOAD_A_STOP)
  {
    temp = 4;
  }
  else if (event == CTRL_RVRS_CRNT_B_STRT||event==CTRL_RVRS_CRNT_B_STOP)
  {
    temp = 5;
  }  
  else
  {
    return ;
  }
  
  for(i=0;i<3;i++)
  {
    if (event == CTRL_TRMNL_OPN_STRT ||event == CTRL_CVR_OPN_STRT || event == CTRL_RVRS_CRNT_A_STRT ||  \
      event ==CTRL_RVRS_CRNT_B_STRT|| event == CTRL_FLT_ENRG_A_STRT ||event == CTRL_OVR_LOAD_A_STRT)        
    {
      general_year=ctrl_sys_new_var.tamper_event[temp][i].start_event_tamper.tamper_year ;
      ptr_time=(uint8_t*)&ctrl_sys_new_var.tamper_event[temp][i].start_event_tamper;
      ptr_time_first=(uint8_t*)&ctrl_sys_new_var.first_tamper_event[temp][i].start_event_tamper;    
    }
    else
    {
      general_year=ctrl_sys_new_var.tamper_event[temp][i].End_event_tamper.tamper_year ;
      ptr_time=(uint8_t*)&ctrl_sys_new_var.tamper_event[temp][i].End_event_tamper;
      ptr_time_first=(uint8_t*)&ctrl_sys_new_var.first_tamper_event[temp][i].End_event_tamper;
    }
    if(general_year==0)
    {
      memcpy(ptr_time,&(ctrl_sys_var_optional_feature.rtc_time.minute),5) ;
      memcpy(ptr_time_first,&(ctrl_sys_var_optional_feature.rtc_time.minute),5) ;               
      break ;
    }
    else if(general_year!=0&&i==2)
    {                
      ptr_time-=i*sizeof(start_end_event_tamper_t);
      memcpy((ptr_time),(ptr_time+sizeof(start_end_event_tamper_t)),sizeof(time_date_t));
      memcpy((ptr_time+sizeof(start_end_event_tamper_t)),(ptr_time+sizeof(start_end_event_tamper_t)*2),sizeof(time_date_t));
      memcpy((ptr_time+sizeof(start_end_event_tamper_t)*2),&(ctrl_sys_var_optional_feature.rtc_time.minute),sizeof(time_date_t));
    }
  }
#endif
}
uint8_t ctrl_no_cdt_no_tmpr (void)
{
  uint8_t ii,no_credit_tamper=0;
  if (CTRL_NO_CRDT_FRNDLY_STAT)
    return FALSE;
  
  for(ii =0 ; ii<((uint8_t)CTRL_NON_LTCH_ACT_EVNT_NUM+(uint8_t)CTRL_LTCH_ACT_EVNT_NUM) ; ii++)
  {
    if ((ctrl_sys_var.basic.non_latch_evnt_count[ii]) && ((ctrl_cfg.tmpr.non_latch_action[ii]) & ACT_OPN_RLY_MASK))
    {
      if (ii == CTRL_NO_CRDT)
        no_credit_tamper |= 0x01;
      else
        no_credit_tamper |= 0x02;
    }
  }
  if (no_credit_tamper == 0x01)
    return TRUE;
  return FALSE;
}


void save_ctrl_cfg (void)
{
  CTRL_DISABLE_RTC_INT();
  /*! - Calculate CRC for configuration data.*/
  ctrl_cfg.crc = calc_crc((uint8_t*)&ctrl_cfg, sizeof(ctrl_cfg_t) - 1,CRC);
  /*! - Save configuration data in non volatile memory.*/
  /* CTRL_SAVE_DATA(CTRL_GET_CFG_ADRS(), (uint8_t*)&ctrl_cfg, sizeof(ctrl_cfg_t)); */  /* file system / tariff not built for the DRU */
  
  if(!CTRL_PWR_DN_STAT)
    /* CTRL_SAVE_DATA(CTRL_GET_CFG_ADRS_BACKUP(), (uint8_t*)&ctrl_cfg, sizeof(ctrl_cfg_t)); */  /* file system / tariff not built for the DRU */
  
  CTRL_ENABLE_RTC_INT();
}
void check_and_reset_ov_load_cntr(uint8_t state)
{
  if(ctrl_sys_new_var.ovr_load_times_cntr_flag == state )
  {
    if (ctrl_sys_new_var.ovr_load_times_cntr < ctrl_cfg.overload.persistence_times)
    {
      ctrl_sys_new_var.ovr_load_times_cntr = 0;
      ctrl_sys_new_var.overload_current_value = 0;
      /* mtr_reset_overload(); */  /* metering not built for the DRU */
      ctrl_sys_var_optional_feature.flag1 &=  (~OVER_LOAD_MODE);
    }
  }
}
#ifdef KEYPAD_FEATURE
void ctrl_keypad_down_up_update_flag(uint8_t sw_type)
{
  keypad_up_down =(keypad_sw_t) sw_type;
}
#endif

/*!
* @brief update techincian activity on meter
*
* @par Description:
* param tech_action_flag activit of techincian on meter as defined tech_action_flag_t .
*
* @return
* - None.
*/
void update_technician_function(uint8_t tech_action_flag)
{
  uint8_t index ,tech_flag = 0;
  uint8_t tech_action_flag_index;
  
  if (tech_id_temp == 0)
    return;
  tech_action_flag_index = tech_action_flag -1;
  for( index = 3*tech_action_flag_index  ; index < (3*tech_action_flag ); index++ )
  {
    if(ctrl_sys_new_var.tech_id_time_reset_all_specific[index] == 0)
    {
      tech_flag = 1;
      break; 
    }
  }
  if(tech_flag)
  {
    ctrl_sys_new_var.tech_id_time_reset_all_specific[index ] = tech_id_temp;
  }
  else
  {
    ctrl_sys_new_var.tech_id_time_reset_all_specific[3*tech_action_flag_index ] = ctrl_sys_new_var.tech_id_time_reset_all_specific[3*tech_action_flag_index + 1];
    ctrl_sys_new_var.tech_id_time_reset_all_specific[3*tech_action_flag_index +1 ] = ctrl_sys_new_var.tech_id_time_reset_all_specific[3*tech_action_flag_index + 2];
    ctrl_sys_new_var.tech_id_time_reset_all_specific[3*tech_action_flag_index +2 ]  = tech_id_temp ;
  }
}


uint16_t get_last_tech_ids_individual(uint8_t tech_action_flag)
{
  uint8_t index ,tech_flag = 0;
  uint8_t tech_action_flag_index;
  uint16_t last_tech_id;
  
  last_tech_id = 0;
  
  tech_action_flag_index = tech_action_flag -1;
  for( index = 3*tech_action_flag_index  ; index < (3*tech_action_flag ); index++ )
  {
    if(ctrl_sys_new_var.tech_id_time_reset_all_specific[index] == 0)
    {
      tech_flag = 1;
      break; 
    }
  }
  
  if(tech_flag)
  {
    if (index != 0)
    {
      last_tech_id = ctrl_sys_new_var.tech_id_time_reset_all_specific[index -1];  
    }
  }
  else
  {
    last_tech_id = ctrl_sys_new_var.tech_id_time_reset_all_specific[3*tech_action_flag_index +2 ] ;
  }
  return last_tech_id;
}


void construct_new_tech_ids (uint8_t * buff)
{
  uint16_t tmp;
#ifdef MTR_SINGLE_PH
  memset (buff,0,11);
#else
  memset (buff,0,8);
#endif
  
  tmp = get_last_tech_ids_individual(CTRL_SET_DATE_TIME);
  memcpy(&buff[0],(uint8_t*)&tmp,2);
  tmp = get_last_tech_ids_individual(CTRL_RESET_ALL_TMPR);
  memcpy(&buff[2],(uint8_t*)&tmp,2);
  
  memcpy(&buff[4],(uint8_t*)&ctrl_sys_new_var.tech_id_cover_terminal[0],2);//cover
  memcpy(&buff[6],(uint8_t*)&ctrl_sys_new_var.tech_id_cover_terminal[1],2); //termi  
    
#ifdef MTR_SINGLE_PH
  tmp = get_last_tech_ids_individual(CTRL_RESET_SPECIFIC_TMPR);
  memcpy(&buff[8],(uint8_t*)&tmp,2);
  
  buff[10] = ctrl_sys_new_var.type_operation_last_tech_id; // reasone
#endif
}
uint8_t get_start_date_tamper(uint8_t type_tamper , time_date_t* tamper_date)
{
  uint8_t tamper_index;
  
  if (type_tamper == CTRL_TRMNL_OPN_STRT )
  {
    tamper_index = 0;
  }
  else if (type_tamper == CTRL_CVR_OPN_STRT)
  {
    tamper_index = 1;
  }
  else if (type_tamper == CTRL_RVRS_CRNT_A_STRT )
  {
    tamper_index = 2;
  }
  else if (type_tamper == CTRL_FLT_ENRG_A_STRT )
  {
    tamper_index = 3;
  }
  else if (type_tamper == CTRL_OVR_LOAD_A_STRT)
  {
    tamper_index = 4;
  }
  else if (type_tamper == CTRL_RVRS_CRNT_B_STRT)
  {
    tamper_index = 5;
  }  
  else
  {
    return 0;
  }
  
  if(ctrl_sys_new_var.first_tamper_event[tamper_index][0].start_event_tamper.tamper_year != 0)
  {
    memcpy((uint8_t*)tamper_date ,(uint8_t*)&(ctrl_sys_new_var.first_tamper_event[tamper_index][0].start_event_tamper ),sizeof(time_date_t));
    return 1 ;
  }
  else
  {
    return 0 ;
  }
}

void ctrl_reset_last_tamper (uint8_t tmpr)
{
  last_tampers.Last_tamper_min[tmpr]   = 0; 
  last_tampers.Last_tamper_hour[tmpr]  = 0;
  last_tampers.Last_tamper_day[tmpr]   = 0;
  last_tampers.Last_tamper_month[tmpr] = 0;
  last_tampers.Last_tamper_year[tmpr]  = 0;
  last_tampers.crc = calc_crc((uint8_t*)&last_tampers, sizeof(ctrl_last_tamper_t) - 1,1);          
  /* CTRL_WRITE_LAST_TAMPERS((uint8_t *)&last_tampers,sizeof(ctrl_last_tamper_t)); */  /* file system / tariff not built for the DRU */
}

void CTRL_set_technicial_id_value (void)
{
  if (tech_id_temp == 0)
    return;
      if (ctrl_sys_new_var.tech_ids_index > 2)
      {
        ctrl_sys_new_var.tech_ids_index = 2;
        ctrl_sys_new_var.tech_ids[0] = ctrl_sys_new_var.tech_ids[1];
        ctrl_sys_new_var.tech_ids[1] = ctrl_sys_new_var.tech_ids[2];
        ctrl_sys_var_second_optional_feature.tech_hour[0] = ctrl_sys_var_second_optional_feature.tech_hour[1];
        ctrl_sys_var_second_optional_feature.tech_hour[1] = ctrl_sys_var_second_optional_feature.tech_hour[2];
        ctrl_sys_var_second_optional_feature.tech_day[0] = ctrl_sys_var_second_optional_feature.tech_day[1];
        ctrl_sys_var_second_optional_feature.tech_day[1] = ctrl_sys_var_second_optional_feature.tech_day[2];
        ctrl_sys_var_second_optional_feature.tech_month[0] = ctrl_sys_var_second_optional_feature.tech_month[1];
        ctrl_sys_var_second_optional_feature.tech_month[1] = ctrl_sys_var_second_optional_feature.tech_month[2];
        ctrl_sys_var_second_optional_feature.tech_year[0] =ctrl_sys_var_second_optional_feature.tech_year[1];
        ctrl_sys_var_second_optional_feature.tech_year[1] =ctrl_sys_var_second_optional_feature.tech_year[2];
      }
      ctrl_sys_new_var.tech_ids[ctrl_sys_new_var.tech_ids_index] = tech_id_temp;
      
      ctrl_sys_var_second_optional_feature.tech_hour[ctrl_sys_new_var.tech_ids_index] = ctrl_sys_var_optional_feature.rtc_time.hour ;
      ctrl_sys_var_second_optional_feature.tech_day[ctrl_sys_new_var.tech_ids_index] = ctrl_sys_var_optional_feature.rtc_date.day;
      ctrl_sys_var_second_optional_feature.tech_month[ctrl_sys_new_var.tech_ids_index] = ctrl_sys_var_optional_feature.rtc_date.month;
      ctrl_sys_var_second_optional_feature.tech_year[ctrl_sys_new_var.tech_ids_index] = ctrl_sys_var_optional_feature.rtc_date.year;
    
      
      ctrl_sys_new_var.tech_ids_index++;
}

uint8_t ctrl_get_block_state (void)
{
#ifndef STS_ENABLE
#ifndef ANSI_FEATURE
#ifndef ASHNTTI_PROJECT
  if ((ctrl_sys_new_var.block_state & CTRL_BLOCK_CFG)) // TOdO to be high
  {
    if (ctrl_sys_new_var.block_state & CTRL_BLOCK_STATE)
    {
      return 2;
    }
    else
      return 1;
  }
#endif
#endif // ANSI_FEATURE
#endif
  return 0;
}


void ctrl_set_block_state (void)
{
#ifndef ANSI_FEATURE
#ifndef ASHNTTI_PROJECT
  if (ctrl_get_block_state())
  {
    ctrl_sys_new_var.block_state |= (CTRL_BLOCK_STATE);
  }
#endif
#endif // ANSI_FEATURE
}

void ctrl_set_ctrl_battery_magnet_flag_check(void)
{
  flags_byte |= CHK_BTRY_MGNT_MASK;
}


void ctrl_save_power_up_cfg_pages (void)
{
  if (save_ctrl_cfg_flg)
  {
    save_ctrl_cfg_flg = 0;
    save_ctrl_cfg();
  }
}

void ctrl_reset_watania_tampers (uint8_t ii,uint8_t num)
{  
  uint8_t status,index;
  if(ii >= 10 || num > 10)
    return;
  for(index = ii;num != 0;index++)
  {
    status = 0;
    if (index == 0)
    {
      status = CTRL_CVR_OPN_OCCURED;
    }
    else if (index == 1)
    {
      status = CTRL_TRMNL_OPN_OCCURED;
    }
    else if (index == 2)
    {
      status = !(CTRL_BTRY_OCCURED);
    }  
    else if (index == 3)
    {
      status = CTRL_RVRS_CRNT_CHA_OCCURED | CTRL_RVRS_CRNT_CHB_OCCURED | CTRL_RVRS_CRNT_CHC_OCCURED;
#ifndef ASHNTTI_PROJECT
      /* MTR_GET_TOT_REV_ACT_EN_HIGH() = 0; */  /* metering not built for the DRU */
      /* MTR_GET_TOT_REV_ACT_EN_LOW() = 0; */  /* metering not built for the DRU */
#ifdef MTR_REACTIVE
      /* MTR_GET_TOT_REV_REACT_EN_HIGH() = 0; */  /* metering not built for the DRU */
      /* MTR_GET_TOT_REV_REACT_EN_LOW() = 0; */  /* metering not built for the DRU */
#endif
#endif
    }
    else if (index == 4)
    {
      status = CTRL_FLT_ENRG_CHA_OCCURED | CTRL_FLT_ENRG_CHB_OCCURED;
#ifndef ASHNTTI_PROJECT
      /* MTR_GET_TOT_FLT_EN_HIGH() = 0; */  /* metering not built for the DRU */
      /* MTR_GET_TOT_FLT_EN_LOW() = 0; */  /* metering not built for the DRU */
#endif
    }
    else if (index == 6)
    {
      status = ctrl_sys_new_var.ovr_load_times_cntr;
    }
    num--;    
    if (status==0)
      continue;
#ifndef G2_DM
    memcpy((uint8_t*)&ctrl_sys_new_var.ctrl_removed_tamper[index].minute ,(uint8_t*)&(ctrl_sys_var_optional_feature.rtc_time)+1 ,5); 
    memcpy(ctrl_sys_new_var.ctrl_removed_tamper[index].transaction_id,transaction_id_temp,sizeof(ctrl_sys_new_var.ctrl_removed_tamper[index].transaction_id));
#ifdef MTR_SINGLE_PH
    ctrl_sys_new_var.ctrl_removed_tamper[index].tech_id = SwapEndian16(tech_id_temp);
#else
    ctrl_sys_new_var.ctrl_removed_tamper[index].tech_id = tech_id_temp;
#endif  
#endif
  }


}

void Force_Open_Relay(void)
{
  if(CTRL_RLY_STAT==0)
  {
    /*! - Open relay.*/
    toggle_relay();
  }
}



void ctrl_down_pressed_isr(void)
{
  scan_result |= SR_DN_SW;
}


void ctrl_up_pressed_isr(void)
{
  scan_result |= SR_UP_SW;
}

void ctrl_exit_lpm (void)
{
  ctrl_exit = 1;
}

extern uint8_t RFID_idle_flag;

void check_switches_state_pwrdwn(void)
{
  uint8_t sw_state = 0;
  
  if (get_sw_state(SR_CVR_SW) && !CTRL_CVR_OPN_OCCURED)
    sw_state |= SR_CVR_SW;
  if (get_sw_state(SR_TRMNL_SW) && !CTRL_TRMNL_OPN_OCCURED)
    sw_state |= SR_TRMNL_SW;
#ifdef CTRL_GPRS_MODULE_CVR_SW
  if (CTRL_READ_GPRS_MODULE_CVR_SW() && !CTRL_GPRS_MODULE_CVR_OPN_OCCURED)
    sw_state |= GPRS_MODULE_CVR_MASK;
#endif

  if (sw_state != save_switches_in_pwrdn) // if there is a data will be saved
  {
    /* Configure EEPORM WP, output high CMOS mode */
    S_GPIO_Set_Bits(EEPROM_WP_PORT, EEPROM_WP_PIN);
    S_GpioInit_PIN(EEPROM_WP_PORT, EEPROM_WP_PIN, GPIO_Mode_OUTPUT_CMOS);

    /* Configure i2c extra, output low  mode */
    S_GPIO_Clear_Bits(I2C_EXTRA_PORT, I2C_EXTRA_PIN);
    S_GpioInit_PIN(I2C_EXTRA_PORT, I2C_EXTRA_PIN, GPIO_Mode_OUTPUT_CMOS);
    Delay_ms(200);

    //S_I2C_INIT(0xa0, EEPROM_I2C);

    ctrl_rtc_isr();

    S_GPIO_Clear_Bits(I2C_EXTRA_PORT, I2C_EXTRA_PIN);

    /* CTRL_SW_STATE_STORE(&sw_state); */  /* file system / tariff not built for the DRU */
    save_switches_in_pwrdn |= sw_state;
  }

}

#ifdef G2_DM
/*****************************************************************
*************************************************************/
/*!
* @brief   this function used to separate events and tampers in the logs  
* @author  Nour Ashraf
* @date    30/1/2024
* @version 1
* @retval  void return No thing
*/
/*****************************************************************
*************************************************************/
void ctrl_save_record_DM_G2(uint8_t record_type, uint8_t *data_ptr)
{
  G2_date_time_t temp_date_time={0};
  G2_DM_Tampers_Log_type tamper={0};
  G2_DM_Events_Log_type event={0};
  G2_DM_Removed_Tampers_Log_type removed_tamper={0};
    
  if(record_type == FILE_SYS_REMOVED_TAMPER_RECORD)
  {
    if((((ctrl_evnt_rec_t*)(data_ptr))->evnt_code >= CTRL_CVR_OPN_REMOVED)&&(((ctrl_evnt_rec_t*)(data_ptr))->evnt_code <= CTRL_GPRS_MODULE_CVR_REMOVED))
    {         
      memcpy((uint8_t*)(removed_tamper.tech_id) , (uint8_t*)((G2_DM_Removed_Tampers_Log_type*)(data_ptr))->tech_id , sizeof(removed_tamper.tech_id));
      removed_tamper.removed_tamper_byte = ((ctrl_evnt_rec_t*)(data_ptr))->evnt_code;
      memcpy(&(temp_date_time.sec), &(ctrl_sys_var_optional_feature.rtc_time.second), 6);    
      removed_tamper.removed_tamper_Date_time=date_time_to_unix(temp_date_time);
      /* Record not persisted: the file system is not built for the DRU. */
    }
  }

  else if(record_type == FILE_SYS_EVENT_RECORD)
  {
    if(((((G2_DM_Tampers_Log_type*)(data_ptr))->Tamper_type >= CTRL_CVR_OPN_STRT)&&(((G2_DM_Tampers_Log_type*)(data_ptr))->Tamper_type <= CTRL_FLT_ENRG_STRT)&& \
        (((G2_DM_Tampers_Log_type*)(data_ptr))->Tamper_type != CTRL_MGNT_HIGH_STRT))||(((G2_DM_Tampers_Log_type*)(data_ptr))->Tamper_type == CTRL_GPRS_MODULE_CVR_STRT))
    {   
      tamper.Tamper_type = ((G2_DM_Tampers_Log_type*)(data_ptr))->Tamper_type;
      tamper.Tamper_type_count = ((G2_DM_Tampers_Log_type*)(data_ptr))->Tamper_type_count;
      memcpy(&(temp_date_time.sec), &(ctrl_sys_var_optional_feature.rtc_time.second), 6);
      tamper.Tamper_Date_time=date_time_to_unix(temp_date_time);
      /* Record not persisted: the file system is not built for the DRU. */
    } 

    else
    {
      event.Event_type = ((G2_DM_Events_Log_type*)(data_ptr))->Event_type;
      event.Event_type_count = ((G2_DM_Events_Log_type*)(data_ptr))->Event_type_count; // TODO NOUR (SIZE of array)
      memcpy(&(temp_date_time.sec), &(ctrl_sys_var_optional_feature.rtc_time.second), 6);  
      event.Event_Date_time=date_time_to_unix(temp_date_time);
      /* Record not persisted: the file system is not built for the DRU. */
    }
    
  }

}
#endif
