/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

M A I N

S O U R C E     F I L E

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*!
* @file main.c
*
* @brief Main file contains the main function.
*
* @details The main file contains meter initialization and the super loop.
*
* <b>References</b>\n
*   All modules design documents
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
*   <td> 05/11/12 </td>
*   <td> wkadry, mohamed.zakaria, aibraheem, mohamed.essa and islam.elshahat </td>
*   <td> Created </td>
* </tr>
* </table>\n
*/
/*!
* @defgroup MAIN Main
* @ingroup METER
* @brief Main task of meter firmware.
*/
/*!
* @defgroup BSW BSW
* @ingroup METER
* Basic Software is run on Microcontroller.\n
* This layer is further divided in the layers.\n
*   -# Driver.
*   -# Interface.
*   -# ISR.
*   -# Services.
*
* \n<b>References</b>\n
* - Ref folder is inside BSW folder.
*
* @brief Basic Software.
*/
/*==========================================================================================

INCLUDE FILES FOR MODULE

==========================================================================================*/

#include "dependencies_layer.h"
#include "dependencies_layer.h"
#include "dependencies_layer.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#if (MicroController == Micro_V94XX)
#include "V94XX_WDT.h"
#else
#include "v85xx_wdt.h"
#endif
#include "config.h"
#include "ctrl.h"
#include "v85xx_flash.h"  
#include "comm.h"
/* #include "keypad.h" */ /* module removed from the DRU build */
#include "Timer_Interface.h"
/* #include "G2_DM.h" */ /* module removed from the DRU build */
#ifdef TESTLCD_SEG
/* #include "Vango85x_lcd.h" */ /* module removed from the DRU build */
/* #include "lcd_HQ30774_cfg.h" */ /* module removed from the DRU build */
#if (MicroController == Micro_V94XX)
#include "V94XX.h"
#else
#include "v85xx.h"
#endif
#endif
/* #include "v9261f.h" -- see dependencies_layer.h */
#include "Flash.h"
//#include "Flash_app.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#ifdef G2_SECURITY
/* #include "G2_Security.h" */ /* module removed from the DRU build */
#endif
#ifdef WIFI_ESP_ENABLE

/* #include "ESP32_Interface.h" */ /* module removed from the DRU build */
#endif
#ifdef PLC_MODEM
/* #include "r_demo_main.h" */ /* module removed from the DRU build */
/* #include "PLC_App.h" */ /* module removed from the DRU build */
#endif
#ifdef ANSI_STACK
/* #include "ANSI_Public.h" */ /* module removed from the DRU build */
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
/*
uint8_t cmd_action=0;
uint16_t seg_nuber=0;
uint8_t arr_crc[8]={42,0,0,0,2,15,2,56};
uint8_t crc_=0;
*/
#ifdef TESTLCD_SEG
uint16_t seg_no=0;
uint8_t enter_test = 0;
#endif
/*------------------------------------------------------------------------------------------
External Variables
------------------------------------------------------------------------------------------*/
uint8_t first_init = 0;
uint8_t first_init_pulses = 0;
extern uint8_t buffer_save_send[5000];
/*------------------------------------------------------------------------------------------
Local Variables
------------------------------------------------------------------------------------------*/
#ifdef DEBUG_BOOTLOADER
  uint32 f = 6000;
#endif
/*------------------------------------------------------------------------------------------
Macros
------------------------------------------------------------------------------------------*/

/*==========================================================================================

FUNCTION DECLARATIONS

==========================================================================================*/
/*!
* @brief Main function of the meter
*
* @par Description:
* Main meter function containing initialization and tasks of all meter modules.
*
* @return
* - int dummy return.
*
* @ingroup MAIN
*/


void main() {
    uint16_t sector;
  uint32_t sector_address;
/*RmvCodCmntA_K*/
  __disable_irq();	// Todo _ check their effect 
  WDT_Disable();
  S_CLK_Init(1);  
  S_RTC_init();
  S_GpioInit_ALL_Modules();  
  vMC_DriversInit();
  S_TMR_INTConfig(S_TMR0, DISABLE);
  S_RTC_DISABLE_INT();
  __enable_irq(); // Todo _ check their effect
  FLASH_gInit();
    /* Enable if you want to test the Flash */
#if (FLASH_UNIT_TEST_ENABLE == 1u)
  (void)flash_unit_test_func(FLASH_UNIT_TEST_SELECTED_CASE);
#endif
  CTRL_INIT_TIMER0();
  S_RTC_ENABLE_INT();
  pwr_up_sec_cntr = 11;
  UART_INTConfig ( UART5 , UART_INT_RX , DISABLE );
  IntegrityCrcCheckState();
  Ctrl_Set_Date_Time_Fixed();

  
  while (1)
  {  
    /*! - Communication task containing all communication interfaces.*/
    comm_task();;  
    /*! - Control task.*/
    ctrl_task();
  }
}


