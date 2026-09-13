#include "config.h"
#if (MicroController == Micro_V94XX)

/**
* @file    v85xx_isr.c
* @author  weaktea
* @version V1.0
* @date    2016/07/05
* @brief   Main Interrupt Service Routines.
******************************************************************************/


#include "dependencies_layer.h"
#include "Dlms_Hdlc.h"

#include "V94XX_WDT.h"
#include "V94XX_ISR.h"
#include "V94XX_TMR.h"
#include "V94XX_DMA.h"
#include "RTC_Interface.h"
#include "UART_Interface.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#include "ctrl.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#include "V94XX_PMU.h"
/* #include "task_priority.h" -- see dependencies_layer.h */
#include "TI_CC1120.h"
#include "modem.h"
#include "iec62056_21_slave.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#include "Timer_Interface.h"

#include "ymodem.h"
#include "V94XX_FLASH.h"  
#include "V94XX_ISO7816.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#include "SystemTick_Interface.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#ifdef EM210_BOARD
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
#endif
#ifdef ANSI_STACK
#include "ANSI_Public.h"
#include "ANSI_APP_RF.h"
#endif
#ifdef PLC_MODEM
#include "r_typedefs.h"
#include "iec62056_21_slave.h"
#include "r_typedefs.h"
#include "r_config.h"
#include "r_bsp_api.h"
#include "r_byte_swap.h"
#include "r_memory_api.h"
#include "r_queue_api.h"

#include "r_app_timer.h"

/* g3 part */
#include "r_c3sap_api.h"

/* app part */
#include "r_demo_app.h"
#include "r_demo_app_eap.h"
#include "r_demo_app_thread.h"
#include "r_demo_api.h"

#include "r_ipv6_headers.h"
#include "r_udp_headers.h"
#include "r_demo_tools.h"
#include "r_demo_api.h"
#include "r_demo_common.h"
#include "r_demo_config.h"
#include "r_demo_statistics.h"
#include "r_demo_ib_access.h"
#include "r_demo_app_ver.h"
#include "r_demo_parameters.h"
#include "r_demo_os_wrapper.h"
#include "app_process_thread.h"
extern void(*TMR0_CallBack)(void);
extern void(*TMR1_CallBack)(void);
extern void(*TMR2_CallBack)(void);
extern void(*TX1_CallBack)(void);
extern void(*RX1_CallBack)(uint8_t);
extern void(*TX3_CallBack)(void);
extern void(*RX3_CallBack)(uint8_t);
extern uint8_t uart_buffer_send_flag;
extern volatile uint8_t tx1;
#endif
#ifdef CONSOL_FEATURE
extern uint8_t second_counter;
#endif
uint8_t rtc_flag_counter = 0;
#if defined (IEC_62056_21_SLAVE) && defined (RS485_NetworkDiscovery_Feature)
extern uint8_t SCAN_MODE_Flag;
#endif
extern uint16_t ANSI_Received_Bytes_No;
uint8_t received_bytes_counter = 0 ;
uint8_t communication_error_flag = 0 ;
uint8_t Scalling_Frame_First_Two_Byte[2];
uint8_t recievedFrame_Scalling_Flag = 0;
volatile uint16_t counter_tmr1 = 0;
volatile uint16_t count1_tmr1 = 0;
volatile uint8_t IRQ0STA = (uint8_t)0;
volatile uint8_t seconds = 0;
uint8_t line_period = LINEPERIOD;
uint8_t Check_var = 0;	//todo_mai checkfor its effect , change its scope
uint16_t rx_index_7816 = 0;
uint8_t rx_buffer_7816[500];
extern uint8_t comm_err_icon ;
extern uint64_t V9340_CounterValueFromTheEndOfInitV9340;
#ifdef PLC_MODEM
uint8_t test = 0;
extern uint8_t flag_framend;
extern uint8_t* remaining_addr;
extern uint32_t remaining;
extern uint8_t bsrc_rcv[1536];
extern uint16_t DMA_last_index;
#endif
#define RTC_TELL_MTR_NEW_ENERGY()  mtr_drv_event(MTR_NEW_ENERGY)/*!< Publish new energy event.*/        
#define CTRL_UP_PRESSED_ISR()  ctrl_up_pressed_isr()/*!< Publish new energy event.*/  
#define CTRL_DOWN_PRESSED_ISR()  ctrl_down_pressed_isr()/*!< Publish new energy event.*/  
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('m',10);
#endif
  CORTEX_NVIC_SystemReset(15);
  while(1)
  {}
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
#ifdef CTRL_GPRS_MODULE_CVR_SW
  if ( !S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN)||!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN)||!S_GPIO_Get_Bits(SW_PORT, M_SW_PIN))
#else
  if ( !S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN)||!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN))
#endif
  {
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('h',10);
#endif
    Delay_ms(120);
    ctrl_rtc_isr();
    if (!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN))
    {
      ctrl_power_down_event |= CVR_OPN_MASK;
      PMU_WakeUpPinConfig(TELT_SW_PIN , IOA_DISABLE);
    }

    if (!S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN))
    {
      ctrl_power_down_event |= TRMNAL_OPN_MASK;
      PMU_WakeUpPinConfig(TERMINAL_SW_PIN , IOA_DISABLE);
    }
#ifdef CTRL_GPRS_MODULE_CVR_SW
    if (!S_GPIO_Get_Bits(SW_PORT, M_SW_PIN))
    {
      ctrl_power_down_event |= GPRS_MODULE_CVR_MASK;
      PMU_WakeUpPinConfig(M_SW_PIN , IOA_DISABLE);
    }
#endif
    S_GPIO_Clear_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
    CTRL_SW_STATE_STORE(&ctrl_power_down_event);
 
 /*RmvCodCmntA_K*/
 
  }
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('H',10);
#endif
  /* system soft reset */
  CORTEX_NVIC_SystemReset(16);
  while(1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (')',10);
#endif
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('b',10);
#endif
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('~',10);
#endif
}

/**
  * @brief  This function handles PMU interrupt request.
  * @param  None
  * @retval None
  */
void PMU_IRQHandler(void)
{
  uint8_t counter = 0;
#if (defined ASHNTTI_PROJECT || defined NEW_ASHANTI_PROJECT)
  if (PMU_GetIOAINTStatus(SW_SCROLL_UP_PIN)) 
  {  
    PMU_WakeUpPinConfig(SW_SCROLL_UP_PIN , IOA_DISABLE);
    PMU_ClearIOAINTStatus(SW_SCROLL_UP_PIN);
    counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_UP_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      CTRL_UP_PRESSED_ISR();
      PMU_WakeUpPinConfig(SW_SCROLL_UP_PIN , IOA_DISABLE);
      PMU_ClearIOAINTStatus(SW_SCROLL_UP_PIN);
    }
  }
  
  if (PMU_GetIOAINTStatus(SW_SCROLL_DOWN_PIN))  
  { 
    PMU_WakeUpPinConfig(SW_SCROLL_DOWN_PIN , IOA_DISABLE);
    PMU_ClearIOAINTStatus(SW_SCROLL_DOWN_PIN);
        counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, SW_SCROLL_DOWN_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      CTRL_DOWN_PRESSED_ISR(); 
      PMU_WakeUpPinConfig(SW_SCROLL_DOWN_PIN , IOA_DISABLE);
      PMU_ClearIOAINTStatus(SW_SCROLL_DOWN_PIN);
    }
  }
#endif
#if defined (V9203_ENABLE) || defined (ADE7953_ENABLE)
  if (PMU_GetIOAINTStatus(MTR_IRQ_PIN))
  {   
    PMU_ClearIOAINTStatus(MTR_IRQ_PIN);
#ifdef V9203_ENABLE 
    PMU_WakeUpPinConfig(MTR_IRQ_PIN,IOA_DISABLE);
    V9203_IRQ_isr();
#elif defined (ADE7953_ENABLE)
    ade7953_isr();
#endif
  }
#endif
  
#ifdef V9360_ENABLE                                /*check it with ENG.ESmail*/            
   if (PMU_GetIOAINTStatus(GPIO_Pin_7))
  {   
    PMU_ClearIOAINTStatus(GPIO_Pin_7);
    PMU_WakeUpPinConfig(GPIO_Pin_7,IOA_DISABLE);
    PWR_down_detect |= VDCIN_PWR_DOWN;
    task_priority_set(PRIORITY_FOR_METER_TASK);
  }
#endif
#ifdef CTRL_GPRS_MODULE_CVR_SW
  if (PMU_GetIOAINTStatus(TELT_SW_PIN) || PMU_GetIOAINTStatus(TERMINAL_SW_PIN)|| PMU_GetIOAINTStatus(M_SW_PIN))
#else
  if (PMU_GetIOAINTStatus(TELT_SW_PIN) || PMU_GetIOAINTStatus(TERMINAL_SW_PIN))
#endif
  {

    /* Configure EEPORM WP, output high CMOS mode */
    S_GPIO_Set_Bits(EEPROM_WP_PORT,EEPROM_WP_PIN);
    S_GpioInit_PIN(EEPROM_WP_PORT,EEPROM_WP_PIN,GPIO_Mode_OUTPUT_CMOS);
    
    
    /* Configure i2c extra, output low  mode */
    S_GPIO_Clear_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
    S_GpioInit_PIN(I2C_EXTRA_PORT,I2C_EXTRA_PIN,GPIO_Mode_OUTPUT_CMOS);
    Delay_ms(200);
    
   // S_I2C_INIT(EEPROM_AT24C256_I2C_ADDRESS, EEPROM_I2C);
    
    ctrl_rtc_isr();
    counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, TELT_SW_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      ctrl_power_down_event |= CVR_OPN_MASK;
      PMU_WakeUpPinConfig(TELT_SW_PIN , IOA_DISABLE);
      //PMU_ClearIOAINTStatus(TELT_SW_PIN);
    }

    counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, TERMINAL_SW_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      ctrl_power_down_event |= TRMNAL_OPN_MASK;
      PMU_WakeUpPinConfig(TERMINAL_SW_PIN , IOA_DISABLE);
      //PMU_ClearIOAINTStatus(TERMINAL_SW_PIN);
    }
#ifdef CTRL_GPRS_MODULE_CVR_SW
    counter = 0;
    while (!S_GPIO_Get_Bits(SW_PORT, M_SW_PIN) && ++counter < 3 );
    if (counter == 3)
    {
      ctrl_power_down_event |= GPRS_MODULE_CVR_MASK;
      PMU_WakeUpPinConfig(M_SW_PIN , IOA_DISABLE);
      //PMU_ClearIOAINTStatus(M_SW_PIN);
    }
#endif
    S_GPIO_Clear_Bits(I2C_EXTRA_PORT,I2C_EXTRA_PIN);
#ifdef DEBUG_LOW_POWER
  uart_debug_byte ('T',0);
  if(PMU_GetIOAINTStatus(TELT_SW_PIN))
    uart_debug_byte ('1',0);
  if(PMU_GetIOAINTStatus(TERMINAL_SW_PIN))
    uart_debug_byte ('2',0);
  if(PMU_GetIOAINTStatus(M_SW_PIN))
    uart_debug_byte ('3',0);
  if(ctrl_power_down_event)
    uart_debug_byte (ctrl_power_down_event,0);
#endif
    PMU_ClearIOAINTStatus(TELT_SW_PIN);
    PMU_ClearIOAINTStatus(TERMINAL_SW_PIN);
    PMU_ClearIOAINTStatus(M_SW_PIN);
    CTRL_SW_STATE_STORE(&ctrl_power_down_event);

    /*RmvCodCmntA_K*/
  }
#ifdef DEBUG_LOW_POWER
  if (PMU_GetINTStatus(PMU_INTSTS_32K|PMU_INTSTS_6M|PMU_INTSTS_PORST|PMU_INTSTS_EXTRST|PMU_INTSTS_DPORST))
  {
    if ( PMU_GetINTStatus(PMU_INTSTS_32K) )   uart_debug_byte ('y',0);
        if ( PMU_GetINTStatus(PMU_INTSTS_6M) ) uart_debug_byte ('k',0);
            if ( PMU_GetINTStatus(PMU_INTSTS_PORST) ) uart_debug_byte ('f',0);
                if ( PMU_GetINTStatus(PMU_INTSTS_EXTRST) ) uart_debug_byte ('z',0);
                    if ( PMU_GetINTStatus(PMU_INTSTS_DPORST) ) uart_debug_byte ('o',0);
  }
if (PMU_GetIOAINTStatus(GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15))
  {
    uart_debug_byte ('G',0);
  }
#endif
}
//uint64_t seconds_debug = 0;

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
#ifdef DEBUG_LOW_POWER
  uart_debug_byte ('t',0);
#endif
   if (RTC_GetINTStatus(RTC_INTSTS_WKUSEC))
  {
    RTC_ClearINTStatus(RTC_INTSTS_WKUSEC);
    if(!(ctrl_flags & CTRL_PWR_DWN_MASK))
    {
      rtc_flag_counter++;
      //ctrl_rtc_isr();
      //seconds_debug++;
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
      trf_grace_elapsed_time.LPM_elapsed++;
#endif 
      seconds_wasSaved++;
      seconds_wasSaved_2++;
      Tooling_Counter++;
    }
        else
    {
#ifndef TRF_GRACE_48_HOURS_COMPARING_TIME
      trf_grace_elapsed_time.LPM_elapsed += 30;
#endif
      CTRL_SET_RTC_INT_LOW_PWR_FLAG();
    }
  }
  else
  {
#ifdef DEBUG_LOW_POWER 
    uart_debug_byte ('6',10);
#endif
  }
#if (defined ASHNTTI_PROJECT || defined NEW_ASHANTI_PROJECT)
  if((ctrl_flags & CTRL_PWR_DWN_MASK))
  {
    CTRL_SET_RTC_INT_LOW_PWR_FLAG();
  }
#endif
#if defined (IEC_62056_21_SLAVE) && defined (RS485_NetworkDiscovery_Feature)
  if(SCAN_MODE_Flag)
    S_UART_INIT((UART_TypeDef*)RS_UART_PORT_NUMBER,'0',UART_WORDLEN_8B,Dlms_NoParity);
#endif
  RTC_ClearINTStatus(0xffffffff);
}

/**
  * @brief  This function handles U32K0 interrupt request.
  * @param  None
  * @retval None
  */
void U32K0_IRQHandler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('v',0);
#endif
}

/**
  * @brief  This function handles U32K1 interrupt request.
  * @param  None
  * @retval None
  */
void U32K1_IRQHandler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('c',0);
#endif
}

/**
  * @brief  This function handles I2C interrupt request.
  * @param  None
  * @retval None
  */
void I2C_IRQHandler(void)
{  //you must open calling it in bootloader code first,as I removed jumping as it isn't used

#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('i',0);
#endif
}

/**
  * @brief  This function handles SPI1 interrupt request.
  * @param  None
  * @retval None
  */
void SPI1_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('s',0);
#endif
}

/**
  * @brief  This function handles UART0 interrupt request.
  * @param  None
  * @retval None
  */



uint8_t uart_receive_byte(UART_TypeDef *uart_num)
{
  uint8_t data;

  data = UART_ReceiveData(uart_num);

  if (get_even_flag_uart(uart_num))
  {
    data = data & 0x7f;
  }
  return data;
}

void UART0_IRQHandler(void)
{
  UART_ClearINTStatus(UART0, UART_INTSTS_RX);
  UART_ClearINTStatus(UART0, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART0,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART0,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART0,  UART_INTSTS_RX );
 // USCI_A0_ISR();
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('q',0);
#endif
  //OPTRXBUF=uart_receive_byte(UART0);
  uint8 byte = uart_receive_byte(UART0);

#if defined (GPRS_FEATURE) && !(MicroController == Micro_V94XX)
  MODEM_rx_isr(byte);
#endif
  
#ifdef V9340_ENABLE 
   V9340_Rx_ByteChkSum = byte;
   //static uint8_t V9340_RX_Buffer_ISR_Counter=0;
   V9340_RX_Buffer[V9340_RX_Buffer_ISR_Counter] = byte;  
   V9340_RX_Buffer_ISR_Counter++;
  if(V9340_RX_Buffer_ISR_Counter >= 5)
  {
     V9340_Rx_I_RecivedByte=1;
    V9340_RX_Buffer_ISR_Counter=0;
  }
  
  //V9340_Recive_DATA(byte);  ///S_UART_RX_FRAME(UART0 ,V9340_RX_Buffer, 5);
#endif

}

/**
  * @brief  This function handles UART1 interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    UART_ClearINTStatus(UART1, UART_INTSTS_RX);

#ifdef DEBUG_HANG_LOOPING
    uart_debug_byte (']',0);
#endif
    
#ifdef RF_LINK_TI_CC1120_ENABLE
    TI_CC1120_rx_isr();
#else
    S_GPIO_Set_Bits(BUZZER_PORT,BUZZER_PIN);
#endif


}

/**
  * @brief  This function handles UART2 interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
    UART_ClearINTStatus(UART2, UART_INTSTS_RX);
    
  UART_ClearINTStatus(UART2, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART2,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART2,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART2,  UART_INTSTS_RX );
  
    uint8 byte = uart_receive_byte(UART2);

#if ( (MicroController == Micro_V94XX) && defined (GPRS_FEATURE) )
  MODEM_rx_isr(byte);
#endif
#ifdef HDLC_PROTOCOL
    S_USCI_A2_ISR(byte);
#endif
#ifdef DEBUG_HANG_LOOPING
    uart_debug_byte ('[',0);
#endif

#if defined (EM_DL130_BOARD) && defined (DLMS_UI_RS485)
   S_USCI_A2_ISR(byte);    
#endif
}

/**
  * @brief  This function handles UART3 interrupt request.
  * @param  None
  * @retval None
  */
#if 0                                                               /*ADDCode_K*/  
volatile uint32_t how_manyBytesRxTouart3 = 0;
uint8_t hh=0,hhh=0;

void UART3_IRQHandler(void)
{
  //static uint8_t ii=0,arr2[12]={0};

  UART_ClearINTStatus(UART3, UART_INTSTS_RX);
    hh=UART_GetFlag(UART3,UART_FLAG_RXFULL );

  uint8 byte = uart_receive_byte(UART3);
      hhh=UART_GetFlag(UART3,UART_FLAG_RXFULL );
/*
  arr2[ii++] = byte;
  if (ii >= 8)
  {
    ii=0;
  }*/
  how_manyBytesRxTouart3++;
  if (how_manyBytesRxTouart3 > 8)
    how_manyBytesRxTouart3 = 1;
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('"',0);
#endif

}
#endif

/**
  * @brief  This function handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
uint8 arr[10] ={0};
void UART4_IRQHandler(void)
{ 
#ifdef ANSI_STACK
  uint8_t rxData=0;
  UART_ClearINTStatus(UART4, UART_INTSTS_RX);
  rxData = OPTICAL_UART_RECEIVE_ANSI();
  /* To avoid effect of sun*/
  Sun_Block_Enable();   
  //close RF port
  getNewByte(rxData);  
  optical_slave_event(ANSI_RESET_COUNTER);
  UART_INTConfig (CONNECTED_METER_MODEM, UART_INT_RX , DISABLE );
  UART_Cmd(CONNECTED_METER_MODEM,UART_MODE_TX|UART_MODE_RX ,DISABLE);
  
#else
  uint8_t x= 0;
  
  UART_ClearINTStatus(UART4, UART_INTSTS_RX);
#if (defined(EM110_BOARD) ||defined (EM330_BOARD)|| defined(EM122U_BOARD) || defined(EM130_BOARD)|| defined(EM210_BOARD))
  
  
  UART_ClearINTStatus(UART4, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
  
#if defined (DEBUG_HANG_LOOPING) && !defined (ADE7953_ENABLE) && !defined (GPRS_FEATURE) && !defined (IEC_62056_21_SLAVE)
  uart_debug_byte('I',1);
#endif
#ifdef ADE7953_ENABLE
    if(mtr_cal_state() == FALSE)
#endif
    {
      //OPTRXBUF=uart_receive_byte(UART4);
      x=uart_receive_byte(UART4); 
      UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
#ifdef TEST_YMODEM
      if (OPTRXBUF == '5')
      {
        UART_INTConfig ( UART4  , UART_INT_RX , DISABLE );
        Ymodem_Main_Menu();
        UART_INTConfig ( UART4  , UART_INT_RX , ENABLE );

      }
#else
          /*RmvCodCmntA_K*/
      S_USCI_A4_ISR(x);
#endif
    }
#ifdef ADE7953_ENABLE
  else
    mtr_cal_rx();
#endif
#ifdef GPRS_FEATURE
 // MODEM_rx_isr();
#endif 
    UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
    x=uart_receive_byte(UART4); 
    UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
    x=uart_receive_byte(UART4); 
    UART_ClearINTStatus(UART4,  UART_INTSTS_RX );
    x=uart_receive_byte(UART4);
  
  
#else

#if defined (BLUETOOTH_FEATURE)
  Bluetooth_rx_isr();
#endif
  #ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('"',0);
#endif
#endif
#endif  //ANSI_STACK
}

/**
  * @brief  This function handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
#pragma optimize=z none no_code_motion no_cse
void UART5_IRQHandler(void)
{
#ifdef ANSI_STACK
  
uint8_t rxData=0;
UART_ClearINTStatus(UART5, UART_INTSTS_RX);    

rxData = BLUTOOTH_UART_RECEIVE_ANSI();

if((rxData == 0 && ANSI_Received_Bytes_No == 0) 
   &&(ansi_get_state() == 0)
     &&(handle_Scalling_Frame_Check() == FALSE)){
       UART_INTConfig (UART5, UART_INT_RX , DISABLE );
       UART_Cmd(UART5,UART_MODE_TX|UART_MODE_RX ,DISABLE);
       optical_slave_event(ANSI_OPTICAL_TO);
     }else{
       set_RF_recieve_flag(SET);
       getNewByte_BLE( rxData );
       /**************************************************/  
       getNewByte(rxData);
       /**************************************************/ 
       /*disable UART 4 (optical) to avoid distortion by sun-light*/
       UART_INTConfig (OPTICAL_UART_PORT_NUMBER, UART_INT_RX , DISABLE );
       UART_Cmd(OPTICAL_UART_PORT_NUMBER,UART_MODE_TX|UART_MODE_RX ,DISABLE);
     }
#else
uint8_t x= 0;

UART_ClearINTStatus(UART5, 0xffffffff /*UART_INTSTS_Msk*/ /*UART_INTSTS_RX*/);
UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
UART_ClearINTStatus(UART5,  UART_INTSTS_RX );

#if defined (DEBUG_HANG_LOOPING) && !defined (ADE7953_ENABLE) && !defined (GPRS_FEATURE) && !defined (IEC_62056_21_SLAVE)
  uart_debug_byte('I',1);
#endif
#ifdef ADE7953_ENABLE
    if(mtr_cal_state() == FALSE)
#endif
    {
      //OPTRXBUF=uart_receive_byte(UART5);
      x=uart_receive_byte(UART5); 
      UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
#ifdef TEST_YMODEM
      if (OPTRXBUF == '5')
      {
        UART_INTConfig ( UART5  , UART_INT_RX , DISABLE );
        Ymodem_Main_Menu();
        UART_INTConfig ( UART5  , UART_INT_RX , ENABLE );

      }
#else
      /*RmvCodCmntA_K*/
#if !defined (EM_DL130_BOARD) && defined (DLMS_UI_RS485)
        S_USCI_A2_ISR(x);
#endif
#ifdef RS485_ENABLE
        S_USCI_A5_ISR(x);
#endif
#endif
    }
#ifdef ADE7953_ENABLE
  else
    mtr_cal_rx();
#endif
#ifdef GPRS_FEATURE
 // MODEM_rx_isr();
#endif 
UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
x=uart_receive_byte(UART5); 
UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
x=uart_receive_byte(UART5); 
UART_ClearINTStatus(UART5,  UART_INTSTS_RX );
x=uart_receive_byte(UART5); 
#endif
}

#if 0
/**
  * @brief  This function handles ISO78160 interrupt request.
  * @param  None
  * @retval None
  */
void ISO78160_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
ARM_ISO78160_IRQHandler();
}
#endif
/**
  * @brief  This function handles ISO78161 interrupt request.
  * @param  None
  * @retval None
  */
void ISO7816_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('0',0);
#endif
  ARM_ISO78160_IRQHandler();
}


/**
  * @brief  This function handles TMR0 interrupt request.
  * @param  None
  * @retval None
  */
//uint64_t val[30]={0},vfg[30]={0};uint16_t bb=0;uint64_t debug_timer0 = 0,temp0=0,temp1=0,temp2=0,energ_acc=0;
#ifdef AC_BUZZER_FEATURE 
    volatile uint8_t xc = 2;
#endif
void TMR0_IRQHandler(void)
{
#ifdef AC_BUZZER_FEATURE 
  static uint8_t counters=0;
  static uint32_t fp = 0;
#endif
    if (TMR_GetINTStatus(TMR0))
  {
    TMR_ClearINTStatus(TMR0);
#ifdef AC_BUZZER_FEATURE 
    if (++counters == 100)
    {
#endif
      ctrl_tmr_isr();
      timer1_tick_ms ++;
#ifdef AC_BUZZER_FEATURE 
      counters = 0 ;
    }
    if (buzzer_enable == 1)
    {
      {
        if (fp > xc)
          S_GPIO_Clear_Bits(BUZZER_PORT, BUZZER_PIN);
        else
          S_GPIO_Set_Bits(BUZZER_PORT, BUZZER_PIN);
        fp++;
        if (fp > 2*xc+1)
          fp = 0 ;
      }
    }
    else
       S_GPIO_Clear_Bits(BUZZER_PORT, BUZZER_PIN);
#endif
#if defined (DEBUG_LOW_POWER) || defined (DEBUG_HANG_LOOPING)
    static uint16 f=0;
      
    if (f>=450)
    {
      uart_debug_byte ('O',0);
      f=0;
    }
    else
      f++;
#endif

 #if defined(EM210_BOARD) || defined (EM_DL130_BOARD)
  if(V9340_CounterValueFromTheEndOfInitV9340)
  {V9340_CounterValueFromTheEndOfInitV9340++;}
#endif
  }
}

/**
  * @brief  This function handles TMR1 interrupt request.
  * @param  None
  * @retval None
  */
void TMR1_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR1))
  {
    TMR_ClearINTStatus(TMR1);
#ifndef PLC_MODEM
    PHDOWN_chk_event = SET;
    counter_tmr1 ++;
    count1_tmr1++;
#else
//    if(test == 1)
//      S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
//    uint32_t size = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1)*256;
//    while(size != DMA_last_index){
//      (*RX1_CallBack)(bsrc_rcv[DMA_last_index]);
//        DMA_last_index++;
//        DMA_last_index = DMA_last_index % 1536;
//    }
//      (*TMR2_CallBack)();
//      app_process_thread();
    
//    if(test == 1)
//      S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
#endif
  }
  #ifdef DEBUG_HANG_LOOPING
    static uint32_t cv=0;
    if (cv++ >= 100)
    uart_debug_byte ('"',0);
#endif
}

/**
  * @brief  This function handles TMR2 interrupt request.
  * @param  None
  * @retval None
  */
   volatile uint32_t counter_1ms=0;
void TMR2_IRQHandler(void)
{
#ifndef PLC_MODEM
  static uint32_t counter_10ms = 0;
  counter_1ms++;
#endif
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (';',0);
#endif
  if (TMR_GetINTStatus(TMR2))
  {
    TMR_ClearINTStatus(TMR2);
#ifndef PLC_MODEM
    //OS_Timer_iHandler();//i think we didn't need it
    if (++counter_10ms >= 100)
    {
#ifdef DLMS_ENABLED
      Dlms_Hdlc_gEndInactivityTimeOut(1);
#endif
      counter_10ms = 0;
	}
#else
//    if(test == 2)
//      S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);

//    uint32_t size = DMA_GetPackLenTransferred(1) + DMA_GetFrameLenTransferred(1)*256;
//    while(size != DMA_last_index){
//      (*RX1_CallBack)(bsrc_rcv[DMA_last_index]);
//        DMA_last_index++;
//        DMA_last_index = DMA_last_index % 1536;
//    }
//      (*TMR2_CallBack)();
    
//    if(test == 2)
//      S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
#endif
  }
#ifdef DEBUG_HANG_LOOPING
    static uint32_t cv=0;

  if (cv++ >= 100)
    uart_debug_byte ('"',0);
#endif
}
/**
  * @brief  This function handles TMR3 interrupt request.
  * @param  None
  * @retval None
  */
void TMR3_IRQHandler(void)
{

    if (TMR_GetINTStatus(TMR3))
  {
    TMR_ClearINTStatus(TMR3);
#ifdef PLC_MODEM
//    if(test == 0)
//      S_GPIO_Set_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
    
    (*TMR0_CallBack)();
    
//    if(test == 0)
//      S_GPIO_Clear_Bits(TMPR_LED_PORT, TMPR_LED_PIN);
#endif
  }
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte (',',0);
#endif
}

/**
  * @brief  This function handles PWM0 interrupt request.
  * @param  None
  * @retval None
  */
void PWM0_IRQHandler(void)
{//you must open calling it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('^',0);
#endif
}

/**
  * @brief  This function handles PWM1 interrupt request.
  * @param  None
  * @retval None
  */
void PWM1_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('%',0);
#endif
}

/**
  * @brief  This function handles PWM2 interrupt request.
  * @param  None
  * @retval None
  */
void PWM2_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('$',0);
#endif
}

/**
  * @brief  This function handles PWM3 interrupt request.
  * @param  None
  * @retval None
  */
void PWM3_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('#',0);
#endif
}

/**
  * @brief  This function handles DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('@',0);
#endif
#ifdef PLC_MODEM
  if (DMA_GetINTStatus(0x400))
  {
    flag_framend = 1;
    DMA_ClearINTStatus(0x400);
    UART_ClearFlag(UART1, 0x80);
    DMA_DeInit(2);
    NVIC_DisableIRQ(DMA_IRQn);
    if(remaining && remaining_addr){
      Send_DMA_UART(remaining, remaining_addr);
    }
    else{
      (*TX1_CallBack)();
    }
  }
#endif
}

/**
  * @brief  This function handles FLASH interrupt request.
  * @param  None
  * @retval None
  */
void FLASH_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
      uart_debug_byte ('!',0);
#endif
     /* if (FLASH_GetINTStatus(FLASH_INT_CS))
      {
        FLASH_ClearINTStatus(FLASH_INT_CS);
      }*/
}

/**
  * @brief  This function handles ANA interrupt request.
  * @param  None
  * @retval None
  */
void ANA_IRQHandler(void)
{
  if (ANA_GetINTStatus(ANA_INT_VDDALARM))
  {
    ANA_ClearINTStatus(ANA_INT_VDDALARM);
    if(!PMU_GetPowerStatus(PMU_PWRSTS_VDDALARM))
    { 
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('R',0);
#endif
      PWR_down_detect &= ~VDDALARM_PWR_DOWN;
    }
    else
    {
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('Q',0);
#endif
      PWR_down_detect |= VDDALARM_PWR_DOWN;
      task_priority_set(PRIORITY_FOR_METER_TASK);
    }
  }
  if (ANA_GetINTStatus(ANA_INT_VDCINDROP))
  {
    ANA_ClearINTStatus(ANA_INT_VDCINDROP);
    if(!PMU_GetPowerStatus(PMU_PWRSTS_VDCINDROP))
    { 
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('N',0);
#endif
      PWR_down_detect &= ~VDCIN_PWR_DOWN;
    }
    else
    {
#ifdef DEBUG_LOW_POWER
      uart_debug_byte ('V',0);      
#endif
      PWR_down_detect |= VDCIN_PWR_DOWN;
      task_priority_set(PRIORITY_FOR_METER_TASK);
    }
   }
  if(PWR_down_detect & (VDCIN_PWR_DOWN|VDDALARM_PWR_DOWN))
    ctrl_flags |= CTRL_PWR_DWN_MASK;
  else
    ctrl_flags &= ~CTRL_PWR_DWN_MASK;

#if 1
  if (ANA_GetINTStatus(ANA_INT_VDCINDROP))
  {
    /* Power up condition */
    ANA_ClearINTStatus(ANA_INT_VDCINDROP);
    PWR_down_detect &= ~VDCIN_PWR_DOWN;
  }
  else
  {
    /* Power down condition */
    ANA_ClearINTStatus(ANA_INT_VDCINDROP);
    PWR_down_detect |= VDCIN_PWR_DOWN;
    task_priority_set(PRIORITY_FOR_METER_TASK);
  }
#endif
  
}
#if 0
/**
  * @brief  This function handles SPI2 interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{//you must jump to it in bootloader code first,as I removed jumping as it isn't used
#ifdef DEBUG_HANG_LOOPING
  uart_debug_byte ('~',0);
#endif
}
#endif
#endif /* MicroController */
/******************* (C) COPYRIGHT Vango Technologies, Inc *****END OF FILE****/
