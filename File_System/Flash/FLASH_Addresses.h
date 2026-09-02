/*
 * flash_intf.h
 *
 * Created: 29/04/2018 01:12:04 م
 *  Author: reda.abdelhameid
 */ 


#ifndef FLASH_ADDRESS_H
#define FLASH_ADDRESS_H
/*
=========================================================================
 *         Include files
 * ===========================================================================================*/
#include "config.h" 
#define FLASH_SECTOR_SIZE       (uint16_t)4096
#define NUM_PAGES_PER_SECTOR    (uint8_t)16
#define MAX_NUM_SECTORS         (uint16_t)1024
#define NUM_BYTES_PER_PAGE      (uint16_t)256
   
/************************************* general addressess *************************************/
#define Optical_Managment_guek_Key_ID_START_PAGE_INDEX           (uint8_t)0 
#define Optical_Managment_guek_Key_ID_END_PAGE_INDEX             (uint8_t)3 
#define Optical_Managment_guek_Key_ID_SECTOR                     297//sector  

#define ASSO_NAME_SECURITY0_ID_START_PAGE_INDEX                  (uint8_t)4
#define ASSO_NAME_SECURITY0_ID_END_PAGE_INDEX                    (uint8_t)4
#define ASSO_NAME_SECURITY0_ID_SECTOR                            297//sector

#define PW_MANAGER_CLIENT_OPTO_ID_START_PAGE_INDEX               (uint8_t)5
#define PW_MANAGER_CLIENT_OPTO_ID_END_PAGE_INDEX                 (uint8_t)5
#define PW_MANAGER_CLIENT_OPTO_ID_SECTOR                         297//sector

#define HDLC_PHYSICAL_ID_ID_BYTE_INDEX                           (uint8_t)0
#define HDLC_PHYSICAL_ID_ID_START_PAGE_INDEX                     6
#define HDLC_PHYSICAL_ID_ID_END_PAGE_INDEX                       6
#define HDLC_PHYSICAL_ID_ID_SECTOR                               297//sector


/******************************** RF FIRMWARE INTERNAL FLASH ADDRESSES ***********************************/   
#define FIRMWARE_FLAG_START_ADDRESS ((uint8_t *)0x3FF00u)
#define PLC_FIRMWARE_FLAG_BYTE        0u
#define RF_FIRMWARE_FLAG_BYTE         1u
/******************************** RF FIRMWARE INTERNAL FLASH ADDRESSES ***********************************/   
#define RF_FIRMWARE_START_ADDRESS   ((uint32_t)0x24000u)
#define RF_FIRMWARE_END_ADDRESS     ((uint32_t)0x3FE00u)
#define RF_FLASH_SECTOR_SIZE        ((uint16_t)512u)

#define RF_FIRMWARE_MAX_SIZE        (RF_FIRMWARE_END_ADDRESS - RF_FIRMWARE_START_ADDRESS) // 114,176 B
#define RF_FIRMWARE_SECTOR_COUNT    (RF_FIRMWARE_MAX_SIZE / RF_FLASH_SECTOR_SIZE) // 223


/******************************** PLC FIRMWARE INTERNAL FLASH ADDRESSES ***********************************/   
#define PLC_FIRMWARE_START_ADDRESS   ((uint32_t)0x7A00u)
#define PLC_FIRMWARE_END_ADDRESS     ((uint32_t)0x23C00u)
#define PLC_FLASH_SECTOR_SIZE        ((uint16_t)512u)

#define PLC_FIRMWARE_MAX_SIZE        (PLC_FIRMWARE_END_ADDRESS - PLC_FIRMWARE_START_ADDRESS) // 115,200 B
#define PLC_FIRMWARE_SECTOR_COUNT    (PLC_FIRMWARE_MAX_SIZE / PLC_FLASH_SECTOR_SIZE) // 225

/******************************** Dot Matrix LCD Table address ***********************************/   
#if defined (DOT_MATRIX_LCD_ENABLE)

/************************************ LANGUAGE 1 ***************************************/
#define LN1_LCD_SECTOR_NUM_SCREEN_0                                         880
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_0                                     0X370000
   
#define LN1_LCD_SECTOR_NUM_SCREEN_1                                         881
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_1                                     0X371000

#define LN1_LCD_SECTOR_NUM_SCREEN_2                                         882
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_2                                     0X372000

#define LN1_LCD_SECTOR_NUM_SCREEN_3                                         883
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_3                                     0X373000

#define LN1_LCD_SECTOR_NUM_SCREEN_4                                         884
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_4                                     0X374000 

#define LN1_LCD_SECTOR_NUM_SCREEN_5                                         885
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_5                                     0X375000

#define LN1_LCD_SECTOR_NUM_SCREEN_6                                         886
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_6                                     0X376000

#define LN1_LCD_SECTOR_NUM_SCREEN_7                                         887
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_7                                     0X377000

#define LN1_LCD_SECTOR_NUM_SCREEN_8                                         888
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_8                                     0X378000

#define LN1_LCD_SECTOR_NUM_SCREEN_9                                         889
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_9                                     0X379000

#define LN1_LCD_SECTOR_NUM_SCREEN_10                                        890
#define LN1_LCD_SECTOR_ADDRESS_SCREEN_10                                    0X37A000


#define LN1_LCD_SECTOR_NUM_BIG_DIGITS                                      891    //big_digits 0->9 reserved 6 page
#define LN1_LCD_SECTOR_ADDRESS_BIG_DIGITS                                  0x37B000

#define LN1_LCD_SECTOR_NUM_SMALL_DIGITS                                    892    //small_digits 0->9 reserved 6 page
#define LN1_LCD_SECTOR_ADDRESS_SMALL_DIGITS                                0x37C000

#define LN1_LCD_SECTOR_NUM_AR_WORDS1                                       893    //words 0->26 reserved 5 page & 3 sectors
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS1                                   0x37D000
#define LN1_LCD_SECTOR_NUM_AR_WORDS2                                       894    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS2                                   0x37E000
#define LN1_LCD_SECTOR_NUM_AR_WORDS3                                       895    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS3                                   0x37F000
#define LN1_LCD_SECTOR_NUM_AR_WORDS4                                       896    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS4                                   0x380000
#define LN1_LCD_SECTOR_NUM_AR_WORDS5                                       897    
#define LN1_LCD_SECTOR_ADDRESS_AR_WORDS5                                   0x381000

#define LN1_LCD_SECTOR_NUM_SYMBOLS1                                        898    //symbols 0->20 reserved 11 page
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS1                                    0x382000
#define LN1_LCD_SECTOR_NUM_SYMBOLS2                                        899    
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS2                                    0x383000
#define LN1_LCD_SECTOR_NUM_SYMBOLS3                                        900    
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS3                                    0x384000
#define LN1_LCD_SECTOR_NUM_SYMBOLS4                                        901    
#define LN1_LCD_SECTOR_ADDRESS_SYMBOLS4                                    0x385000

#define LN1_LCD_SECTOR_NUM_UNITS                                           902    //units 0->6 reserved 6 page
#define LN1_LCD_SECTOR_ADDRESS_UNITS                                       0x386000

#define LN1_LCD_SECTOR_NUM_ALPHA1                                          903    //A -> P
#define LN1_LCD_SECTOR_ADDRESS_ALPHA1                                      0x387000
#define LN1_LCD_SECTOR_NUM_ALPHA2                                          904    //Q -> Z reserved 7 page
#define LN1_LCD_SECTOR_ADDRESS_ALPHA2                                      0x388000

/************************************ LANGUAGE 2 *****************************************/


#define LN2_LCD_SECTOR_NUM_SCREEN_0                                         905
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_0                                     0X389000

#define LN2_LCD_SECTOR_NUM_SCREEN_1                                         906
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_1                                     0X38A000

#define LN2_LCD_SECTOR_NUM_SCREEN_2                                         907
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_2                                     0X38B000

#define LN2_LCD_SECTOR_NUM_SCREEN_3                                         908
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_3                                     0X38C000

#define LN2_LCD_SECTOR_NUM_SCREEN_4                                         909
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_4                                     0X38D000 

#define LN2_LCD_SECTOR_NUM_SCREEN_5                                         910
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_5                                     0X38E000

#define LN2_LCD_SECTOR_NUM_SCREEN_6                                         911
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_6                                     0X38F000

#define LN2_LCD_SECTOR_NUM_SCREEN_7                                         912
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_7                                     0X390000

#define LN2_LCD_SECTOR_NUM_SCREEN_8                                         913
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_8                                     0X391000

#define LN2_LCD_SECTOR_NUM_SCREEN_9                                         914
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_9                                     0X392000

#define LN2_LCD_SECTOR_NUM_SCREEN_10                                        915
#define LN2_LCD_SECTOR_ADDRESS_SCREEN_10                                    0X393000



#define LN2_LCD_SECTOR_NUM_BIG_DIGITS                                      916    //big_digits 0->9 reserved 6 page
#define LN2_LCD_SECTOR_ADDRESS_BIG_DIGITS                                  0x394000


#define LN2_LCD_SECTOR_NUM_SMALL_DIGITS                                    917    //small_digits 0->9 reserved 6 page
#define LN2_LCD_SECTOR_ADDRESS_SMALL_DIGITS                                0x395000

#define LN2_LCD_SECTOR_NUM_AR_WORDS1                                       918    //words 0->26 reserved 5 page 
#define LN2_LCD_SECTOR_ADDRESS_AR_WORDS1                                   0x396000
#define LN2_LCD_SECTOR_NUM_AR_WORDS2                                       919    
#define LN2_LCD_SECTOR_ADDRESS_AR_WORDS2                                   0x397000
#define LN2_LCD_SECTOR_NUM_AR_RESERVED1                                    920    
#define LN2_LCD_SECTOR_ADDRESS_AR_RESERVED1                                0x398000
#define LN2_LCD_SECTOR_NUM_AR_RESERVED2                                    921    
#define LN2_LCD_SECTOR_ADDRESS_AR_RESERVED2                                0x399000
#define LN2_LCD_SECTOR_NUM_AR_RESERVED3                                    922    
#define LN2_LCD_SECTOR_ADDRESS_AR_RESERVED3                                0x39A000


#define LN2_LCD_SECTOR_NUM_SYMBOLS1                                        923    //symbols 0->20 reserved 11 page
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS1                                    0x39B000
#define LN2_LCD_SECTOR_NUM_SYMBOLS2                                        924    
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS2                                    0x39C000
#define LN2_LCD_SECTOR_NUM_SYMBOLS_RESERVED1                               925    
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED1                           0x39D000
#define LN2_LCD_SECTOR_NUM_SYMBOLS_RESERVED2                               926    
#define LN2_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED2                           0x39E000

#define LN2_LCD_SECTOR_NUM_UNITS                                           927    //units 0->6 reserved 6 page
#define LN2_LCD_SECTOR_ADDRESS_UNITS                                       0x39F000

#define LN2_LCD_SECTOR_NUM_ALPHA1                                          928    //A -> P
#define LN2_LCD_SECTOR_ADDRESS_ALPHA1                                      0x3A0000
#define LN2_LCD_SECTOR_NUM_ALPHA2                                          929    //Q -> Z reserved 7 page
#define LN2_LCD_SECTOR_ADDRESS_ALPHA2                                      0x3A1000


/************************************FRENCH LANGUAGE*****************************************/
#define LN3_LCD_SECTOR_NUM_SCREEN_0                                         930
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_0                                     0x3A2000

#define LN3_LCD_SECTOR_NUM_SCREEN_1                                         931
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_1                                     0x3A3000

#define LN3_LCD_SECTOR_NUM_SCREEN_2                                         932
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_2                                     0x3A4000

#define LN3_LCD_SECTOR_NUM_SCREEN_3                                         933
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_3                                     0x3A5000

#define LN3_LCD_SECTOR_NUM_SCREEN_4                                         934
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_4                                     0x3A6000 

#define LN3_LCD_SECTOR_NUM_SCREEN_5                                         935
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_5                                     0x3A7000

#define LN3_LCD_SECTOR_NUM_SCREEN_6                                         936
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_6                                     0X3A8000

#define LN3_LCD_SECTOR_NUM_SCREEN_7                                         937
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_7                                     0X3A9000

#define LN3_LCD_SECTOR_NUM_SCREEN_8                                         938
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_8                                     0X3AA000

#define LN3_LCD_SECTOR_NUM_SCREEN_9                                         939
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_9                                     0X3AB000

#define LN3_LCD_SECTOR_NUM_SCREEN_10                                        940
#define LN3_LCD_SECTOR_ADDRESS_SCREEN_10                                    0X3AC000



#define LN3_LCD_SECTOR_NUM_BIG_DIGITS                                      941    //big_digits 0->9 reserved 6 page
#define LN3_LCD_SECTOR_ADDRESS_BIG_DIGITS                                  0x3AD000


#define LN3_LCD_SECTOR_NUM_SMALL_DIGITS                                    942    //small_digits 0->9 reserved 6 page
#define LN3_LCD_SECTOR_ADDRESS_SMALL_DIGITS                                0x3AE000

#define LN3_LCD_SECTOR_NUM_AR_WORDS1                                       943    //words 0->26 reserved 5 page 
#define LN3_LCD_SECTOR_ADDRESS_AR_WORDS1                                   0x3AF000
#define LN3_LCD_SECTOR_NUM_AR_WORDS2                                       944    
#define LN3_LCD_SECTOR_ADDRESS_AR_WORDS2                                   0x3B0000
#define LN3_LCD_SECTOR_NUM_AR_RESERVED1                                    945    
#define LN3_LCD_SECTOR_ADDRESS_AR_RESERVED1                                0x3B1000
#define LN3_LCD_SECTOR_NUM_AR_RESERVED2                                    946    
#define LN3_LCD_SECTOR_ADDRESS_AR_RESERVED2                                0x3B2000
#define LN3_LCD_SECTOR_NUM_AR_RESERVED3                                    947    
#define LN3_LCD_SECTOR_ADDRESS_AR_RESERVED3                                0x3B3000


#define LN3_LCD_SECTOR_NUM_SYMBOLS1                                        948    //symbols 0->20 reserved 11 page
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS1                                    0x3B4000
#define LN3_LCD_SECTOR_NUM_SYMBOLS2                                        949    
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS2                                    0x3B5000
#define LN3_LCD_SECTOR_NUM_SYMBOLS_RESERVED1                               950    
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED1                           0x3B6000
#define LN3_LCD_SECTOR_NUM_SYMBOLS_RESERVED2                               951    
#define LN3_LCD_SECTOR_ADDRESS_SYMBOLS_RESERVED2                           0x3B7000

#define LN3_LCD_SECTOR_NUM_UNITS                                           952   //units 0->6 reserved 6 page
#define LN3_LCD_SECTOR_ADDRESS_UNITS                                       0x3B8000

#define LN3_LCD_SECTOR_NUM_ALPHA1                                          953    //A -> P
#define LN3_LCD_SECTOR_ADDRESS_ALPHA1                                      0x3B9000
#define LN3_LCD_SECTOR_NUM_ALPHA2                                          954    //Q -> Z reserved 7 page
#define LN3_LCD_SECTOR_ADDRESS_ALPHA2                                      0x3BA000
   
/**************************ANSI SCREEN SCROLL TIME ***********************************/
   
#define ANSI_SCREEN_SCROLL_TIME_SECTOR_NUM                                 955
#define ANSI_SCREEN_SCROLL_TIME_SECTOR_ADDRESS                             0x3BB000   
  
/* some sectors reserved from 3BC000 ----> 3BF000*/
/****************************************BUFFER SECTOR*****************************************/
#define BUFFER_SECTOR_NUM                                                 960    
#define BUFFER_SECTOR_ADDRESS                                             0X3C0000

#define INTEGRITY_CRC_NUM                                                 981
#define INTEGRITY_CRC_ADDRESS                                             0x3D5000
/****************************************END*****************************************/
#endif





/******************************** flash_app defined addressess ***********************************/
#define ID_SECTOR_ADD 	   			                  0//sector
#define FLAG_SECTOR_ADD 	   			          499//sector
#define FLAG_APP_SECTOR_ADD 	   			          500//sector
#define COUNTER_SECTOR_ADD 	   			          430//sector
#define BACKUP_ID_SECTOR_ADD                                      527//sector     
// PageAdress = SectorADD*16 + pageIndex
/*--- Image Data Sectors ---*/
#define PRV_IMAGE_SECTOR_START_ADD		                  431 //sector
#define PRV_IMAGE_SECTOR_END_ADD		                  498//sector

#define CURRENT_IMAGE_SECTOR_START_ADD		                  301 //sector
#define CURRENT_IMAGE_SECTOR_END_ADD		                  370//sector

#define NEW_IMAGE_SECTOR_START_ADD		                  371//sector
#define NEW_IMAGE_SECTOR_END_ADD		                  429//sector

#define BOOT_IMAGE_SECTOR_START_ADD     	                  528//sector
#define BOOT_IMAGE_SECTOR_END_ADD		                  534//sector

#define NEW_BOOT_IMAGE_SECTOR_START_ADD		                  535//sector
#define NEW_BOOT_IMAGE_SECTOR_END_ADD    	                  540//sector


/*--- Id of every record or image saved in the first of each page in this sector and make the remaining reserved  ---*/
#define MONEY_ID_PAGE_INDEX		   	                 (uint8_t)0
#define BILLING_ID_PAGE_INDEX		                         (uint8_t)1
#define CONFIG_METER_ID_PAGE_INDEX		                 (uint8_t)2
#define EVENT_RECORD_ID_PAGE_INDEX	                         (uint8_t)3
#define LOAD_PROFILE_ID_PAGE_INDEX	       	                 (uint8_t)4
#ifdef G2_DM
#define TAMPER_ID_PAGE_INDEX		                           (uint8_t)5
#define REMOVE_TAMPER_ID_PAGE_INDEX		                     (uint8_t)6
#define YEARLY_CURRENT_MD_PAGE_INDEX		                  (uint8_t)7
#define YEARLY_POWER_MD_PAGE_INDEX		                     (uint8_t)8
#endif
#define CONFIG_MD_PAGE_INDEX                                     (uint8_t)11
#define DLMS_RECORDS_PAGE_INDEX                                  (uint8_t)12
/*--- Images Information ID ---*/
#define PRV_IMAGE_INFO_PAGE_INDEX        	                 (uint8_t)5
#define CURRENT_IMAGE_INFO_PAGE_INDEX     	                 (uint8_t)6
#define FUTURE_IMAGE_INFO_PAGE_INDEX	   	   	         (uint8_t)7
#define CURRENT_BOOTLOADER_INFO_PAGE_INDEX                       (uint8_t)8
#define NEW_BOOTLOADER_INFO_PAGE_INDEX	    	                 (uint8_t)9
#define FLAG_IMAGE_INDICATION_PAGE_INDEX  	                 (uint8_t)10

/*--- Records Data Sectors ---*/
#define ROFILE_SECTOR_NUMBERS			                  272
#define PROFILE_SECTOR_START_ADD                                  25//sector
#define PROFILE_SECTOR_END_ADD                                    296//sector

#define EVENT_RECORD_SECTOR_NUMBERS	                          8
#define EVENT_RECORD_SECTOR_START_ADD                             16//sector
#define EVENT_RECORD_SECTOR_END_ADD                               23//sector

#define CONFIG_RECORD_SECTOR_NUMBERS	 	                  5
#define CONFIG_RECORD_SECTOR_START_ADD                            10//sector
#define CONFIG_RECORD_SECTOR_END_ADD                              14//sector

#define BILLING_RECORD_SECTOR_NUMBERS		                  3
#define BILLING_RECORD_SECTOR_START_ADD                           6//sector
#define BILLING_RECORD_SECTOR_END_ADD                             8//sector
   
#define MONEY_RECORD_SECTOR_NUMBERS 		                  4
#define MONEY_RECORD_SECTOR_START_ADD                             1//sector
#define MONEY_RECORD_SECTOR_END_ADD                               4//sector

#define MD_RECORD_SECTOR_NUMBERS 		                  2
#define MD_RECORD_SECTOR_START_ADD                                298
#define MD_RECORD_SECTOR_END_ADD                                  299//sector

#ifdef G2_DM
#define TAMPER_RECORD_SECTOR_NUMBERS 		                        2
#define TAMPER_RECORD_SECTOR_START_ADD                            530
#define TAMPER_RECORD_SECTOR_END_ADD                              531//sector


#define REMOVE_TAMPER_RECORD_SECTOR_NUMBERS 		                  2
#define REMOVE_TAMPER_RECORD_SECTOR_START_ADD                     532
#define REMOVE_TAMPER_RECORD_SECTOR_END_ADD                       533//sector


#define YEARLY_CURRENT_MD_RECORD_SECTOR_NUMBERS 		            2
#define YEARLY_CURRENT_MD_PAGE_START_ADD                          536
#define YEARLY_CURRENT_MD_PAGE_END_ADD                            537//sector


#define YEARLY_POWER_MD_RECORD_SECTOR_NUMBERS 		               2
#define YEARLY_POWER_MD_PAGE_START_ADD                            538
#define YEARLY_POWER_MD_PAGE_END_ADD                              539//sector

#endif


#define DLMS_RECORD_SECTOR_1_START_ADD                            503//sector 
#define DLMS_RECORD_SECTOR_1_END_ADD                              504//sector

#define DLMS_RECORD_SECTOR_2_START_ADD                            505//sector 
#define DLMS_RECORD_SECTOR_2_END_ADD                              506//sector 

#define DLMS_RECORD_SECTOR_3_START_ADD                            507//sector 
#define DLMS_RECORD_SECTOR_3_END_ADD                              508//sector 

#define DLMS_RECORD_SECTOR_4_START_ADD                            509//sector 
#define DLMS_RECORD_SECTOR_4_END_ADD                              510//sector 

#define DLMS_RECORD_SECTOR_5_START_ADD                            511//sector 
#define DLMS_RECORD_SECTOR_5_END_ADD                              512//sector 

#define DLMS_RECORD_SECTOR_6_START_ADD                            513//sector 
#define DLMS_RECORD_SECTOR_6_END_ADD                              514//sector 

#define DLMS_RECORD_SECTOR_7_START_ADD                            515//sector 
#define DLMS_RECORD_SECTOR_7_END_ADD                              516//sector 

#define DLMS_RECORD_SECTOR_8_START_ADD                            517//sector 
#define DLMS_RECORD_SECTOR_8_END_ADD                              518//sector 

#define DLMS_RECORD_SECTOR_9_START_ADD                            519//sector 
#define DLMS_RECORD_SECTOR_9_END_ADD                              520//sector 

#define DLMS_RECORD_SECTOR_10_START_ADD                           521//sector 
#define DLMS_RECORD_SECTOR_10_END_ADD                             522//sector 

#define DLMS_RECORD_SECTOR_11_START_ADD                           523//sector 
#define DLMS_RECORD_SECTOR_11_END_ADD                             524//sector

#define BACK_UP_SECTOR                                            525//sector 

////////////////////////////////Group_Metering////////////////////////////////////

#define GM_Flash_ReadOutData_StartAddress                                0x26F000     //start of (623)sector 623*4096
#define GM_Flash_ReadOutData_LastAddress                                 0x2A1000     //start of (673)sector    
                                                           
                                                           
#define GM_Flash_TampersAndEvents_1_Sector_StartAddress                  0x25F000//0x29F000     //start of (607)sector
#define GM_Flash_TampersAndEvents_2_Sector_StartAddress                  0x260000//0x29F001     //start of (608)sector
//                                                         
//                                                         
#define GM_Flash_TampersAndEventsBackUp_1_Sector_StartAddress            0x261000//0x29F002     //start of (609)sector
#define GM_Flash_TampersAndEventsBackUp_2_Sector_StartAddress            0x262000//0x29F003     //start of (610)sector
                                                                    
#define GM_Flash_Orders_StartAddress                                     0x263000// 0x29F004     //start of (611)sector
#define GM_Flash_OrdersBackUp_StartAddress                               0x264000// 0x29F005     //start of (612)sector
        
#define GM_Flash_CMD_Respons_StartAddress                                0x265000// 0x29F004     //start of (611)sector
#define GM_Flash_CMD_ResponsBackUp_StartAddress                          0x266000// 0x29F005     //start of (612)sector
        
        
#define GM_Flash_SerialNumber_StartAddress                                0x267000// 0x29F005     //start of (612)sector
#define GM_Flash_SerialNumberBackUp_StartAddress                          0x268000// 0x29F005     //start of (612)sector

//////////////////////////////////////////////////////////////////////////////////
   /**************************************** Flash app generic address *********************************************************/
/*----------- tampers store --------*/
#define TAMPERS_COUNTER_ID_BYTE_INDEX                           0
#define TAMPERS_COUNTER_ID_PAGE                                 0//page

#define TAMPERS_COUNTER_REDUNDANT_ID_BYTE_INDEX                 100
#define TAMPERS_COUNTER_REDUNDANT_ID_PAGE                       2//page

#define LAST_TAMPER_ID_BYTE_INDEX                               190
#define LAST_TAMPER_ID_PAGE                                     4//page
   
#define EVENTLOG_STORE_PERMISSION_BYTE_INDEX                    0
#define EVENTLOG_STORE_PERMISSION_PAGE                          5//page -- takes 2 page(5,6)

#define EVENT_LPM_0_ID_BYTE_INDEX                               0
#define EVENT_LPM_1_ID_BYTE_INDEX                               16
#define EVENT_LPM_2_ID_BYTE_INDEX                               32
#define EVENT_LPM_ID_PAGE                                       7//page 

#define INSTANTENOUS_ERROR_STATUS_ID_BYTE_INDEX                 74              
#define ERROR_STATUS_ID_BYTE_INDEX                              70
#define ERROR_STATUS_ID_PAGE                                    7//page

#define LIMITER_LOADLIMIT_CFG_ID_BYTE_INDEX                     80
#define LIMITER_LOADLIMIT_CFG_ID_PAGE                           7//page

#define LIMITER_LOADLIMIT_NUM_OF_PLANS_ID_BYTE                  90
#define LIMITER_LOADLIMIT_NUM_OF_PLANS_ID_PAGE                  7//page


#define LIMITER_THRESH_CUR1_CFG_PARAMS_ID_INDEX                 95//16*6 till 200
#define LIMITER_THRESH_CUR1_CFG_PARAMS_ID_PAGE                  7


#define LIMITER_MON_VAL_CUR1_CFG_PARAMS_ID_BYTE                 195
#define LIMITER_MON_VAL_CUR1_CFG_PARAMS_ID_PAGE                 7

#define EVENTLOGSTOREPERMISSION_POWER_COC_ID_BYTE_INDEX         0
#define EVENTLOGSTOREPERMISSION_POWER_COC_ID_PAGE               8//page take 2 pages (8,9)


#define LIMITER_EMERGENCY_CUR1_CFG_PARAMS_BYTE                  0
#define LIMITER_EMERGENCY_CUR1_CFG_PARAMS_PAGE                  10//2page(10,11)

#define LIMITER_LOADLIMIT_PLAN1_INDEX                           0
#define LIMITER_LOADLIMIT_PLAN1_PAGE                            12//2page(12,13)

#define POWER_FAILURE_DURATION_OFFSET_ID                   0//4bytes
#define POWER_FAILURE_DURATION_OFFSET_ID_PAGE                   14//page                 

#define SHORT_POWER_FAILURE_DURATION_OFFSET_ID             4//2byte
#define SHORT_POWER_FAILURE_DURATION_OFFSET_ID_PAGE             14//page

#define LONG_POWER_FAILURE_DURATION_OFFSET_ID              6//4byte
#define LONG_POWER_FAILURE_DURATION_OFFSET_ID_PAGE              14//page

#define LONG_POWER_FAILURE_THRESHOLD_OFFSET_ID            10//2
#define LONG_POWER_FAILURE_THRESHOLD_OFFSET_ID_PAGE             14//page

#define SAG_STATES_DATA_OFFSET_ID                         15// 75bytes
#define SAG_STATES_DATA_OFFSET_ID_PAGE                          14//page

#define SWELL_STATES_DATA_OFFSET_ID                        100//75bytes
#define SWELL_STATES_DATA_OFFSET_ID_PAGE                        14//page

#define SHORT_POWER_FAILURE_COUNTER_ID                    175//2 bytes
#define SHORT_POWER_FAILURE_COUNTER_ID_PAGE                     14//page

#define LONG_POWER_FAILURE_COUNTER_ID                      177 // 2bytes
#define LONG_POWER_FAILURE_COUNTER_ID_PAGE                      14//page

#define POWER_CUT_START_OFFSET2_ID                         180//25bytes
#define POWER_CUT_START_OFFSET2_ID_PAGE                         14//page

#define POWER_CUT_START_OFFSET2_ID_INDEX                        205//25bytes
#define POWER_CUT_START_OFFSET2_ID_PAGE                         14//page

#define LIMITER_LOADLIMIT_TYPE_ID_BYTE                          0///70byte
#define LIMITER_LOADLIMIT_TYPE_ID_PAGE                          15//page

#define TAMPERS_SECTOR                                          501//sector

/*---- load profile store -------*/
//Profile_ID_Cfg_Idx
#define LP_CONFIG1_ID                                           0
#define LP_CONFIG2_ID                                           15
#define LP_CONFIG4_ID                                           30
#define LP_CONFIG5_ID                                           45
#define LP_CONFIG6_ID                                           60//15bytes

#define LP_1_PAGE                                               0//page

#define Battery_TamperEvent_Timestamp_ID                        80//todo it's size in future if you need to use all tampers ,now i use one only.
#define Battery_TamperEvent_Timestamp_ID_PAGE                   LP_1_PAGE//page

#define Battery_TamperEvent_Duration_ID                         200
#define Battery_TamperEvent_Duration_ID_PAGE                    LP_1_PAGE//page

#define Battery_TamperEvent_Cumulative_ID                       245
#define Battery_TamperEvent_Cumulative_ID_PAGE                  LP_1_PAGE//page

/////////////////
//Capture_Object_list_Idx
#define LP_OBJECTLIST_PAGE                                      1//page

#define LP_OBJECTLIST_IDX1_ID                                   30
#define LP_OBJECTLIST_IDX2_ID                                   60
#define LP_OBJECTLIST_IDX4_ID                                   90
#define LP_OBJECTLIST_IDX5_ID                                   120
#define LP_OBJECTLIST_IDX6_ID                                   150
//Capture_Object_list
#define LP_OBJECTLIST_CFG1_ID                                   180
#define LP_OBJECTLIST_CFG2_ID                                   192
#define LP_OBJECTLIST_CFG3_ID                                   204
#define LP_OBJECTLIST_CFG4_ID                                   216
#define LP_OBJECTLIST_CFG5_ID                                   228//12byte
#define LP_OBJECTLIST_CFG6_ID                                   240//12bytes

////////////
#define LAST_BILLING_MONTH_ID_BYTE                              0//25byte
#define LAST_BILLING_MONTH_ID_PAGE                              2//page

#define Time_Change_Flag_ID_BYTE                                25//1byte
#define Time_Change_Flag_ID_PAGE                                2//page

#define AMR_PROFILE_STATUS_CODE_ID                              26//1byte
#define AMR_PROFILE_STATUS_CODE_ID_PAGE                         2//page
////////////

#define LP_SECTOR                                               502//sector


///////////////////////////
#define EVENT_NOT_SECTOR                                        LP_SECTOR

#define LASTHANDLEEVENT_ID                                      0//64bytes
#define LASTHANDLEEVENT_ID_PAGE                                 3

//note : form 64 till 255 in page 3 is empty
#define EVENTLOGSENDEPERMISSION_POWER_COC_ID                    0//512bytes //not jumping to anther page (checked ok ✔)
#define EVENTLOGSENDEPERMISSION_POWER_COC_ID_PAGE               5 //2pages (5,6)

/////////////
#define CALENDER_SECTOR                                         LP_SECTOR
   
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_PASSIVE_TIME_ID  0//70bytes
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_PASSIVE_TIME_ID_PAGE 7//page

#define PassiveToActiveCoping_Flag_ID                           71 //5 bytes
#define PassiveToActiveCoping_Flag_ID_PAGE                      7//page

#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_ACTIVE_ID 78//36byte 
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_ACTIVE_ID_PAGE 7//page
   
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_PASSIVE_ID 114//36bytes
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_CALENDER_NAME_PASSIVE_ID_PAGE 7//page

//note : use this :: from 150 to 192 in page 7 is empty 

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_ACTIVE_ID         152//520bytes, every one : 104bytes //not jumping to anther page (checked ok ✔)
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_ACTIVE_ID_PAGE    7//page


#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_PASSIVE_ID        160          
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_SEASON_PASSIVE_ID_PAGE   9//page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_ACTIVE_ID           170//56*5=280
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_ACTIVE_ID_PAGE      11//page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_PASSIVE_ID          194//280 till 218
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_WEEK_PASSIVE_ID_PAGE     11//page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_ACTIVE_ID      218// 384bytes 
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_ACTIVE_ID_PAGE 12//  page

#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_PASSIVE_ID     92// 384bytes
#define ACTIVTIY_CALENDAR_ACTIVE_IMPORT_DAY_TABLE_PASSIVE_ID_PAGE 14// page

/////////////
#define CALENDER_2_SECTOR                                         526//sector/****-----** last sector till now ****------*******/

#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_INDEX_ID           0//15bytes
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_ACTIVE_INDEX_ID_PAGE      0//page

#define ACTIVITY_CALENDER_ACTIVE_IMPORT_PASSIVE_INDEX_ID         15//15bytes
#define ACTIVITY_CALENDER_ACTIVE_IMPORT_PASSIVE_INDEX_ID_PAGE    0//page

#define SPECIAL_DAYS_RECORDS_ID                                  30
#define SPECIAL_DAYS_RECORDS_ID_PAGE                             0//page

#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_NUM_ID 40//5bytes
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_NUM_ID_PAGE 0//page 

#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_NUM_ID  45//5bytes
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_NUM_ID_PAGE  0//page
   
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ACTIVE_ID         50//10bytes
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ACTIVE_ID_PAGE    0//page

#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_ID         60//64bytes
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTER_ASSIGMENT_ID_PAGE    0//page

#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_ID         130//80bytes
#define REGISTER_ACTIVATION_ACTIVE_IMPORT_REGISTE_ACTIVATION_ID_PAGE    0//page

#define SPECIAL_DAYS_TABLE_ID                                           210//200bytes
#define SPECIAL_DAYS_TABLE_ID_PAGE                                      0//page

//////
#define TRF_REG_SECTOR                                                  526//sector

#define ACTIVE_TARRIF_BILLING_PERIOD_ID                                 0//10*25=250bytes
#define ACTIVE_TARRIF_BILLING_PERIOD_ID_PAGE                            2//page

#define REACTIVE_TARRIF_BILLING_PERIOD_ID                               250
#define REACTIVE_TARRIF_BILLING_PERIOD_ID_PAGE                          2//page

#define ACTIVE_GEN_TARRIF_BILLING_PERIOD_ID                             244
#define ACTIVE_GEN_TARRIF_BILLING_PERIOD_ID_PAGE                        3//page

#define REACTIVE_GEN_TARRIF_BILLING_PERIOD_ID                           238
#define REACTIVE_GEN_TARRIF_BILLING_PERIOD_ID_PAGE                      4//page

#define PRE_Register_Period1_ID                                         233//64
#define PRE_Register_Period1_ID_PAGE                                    5//page

#define ACTIVE_TARRIF_PERIOD1_ID                                        41//640bytes
#define ACTIVE_TARRIF_PERIOD1_ID_PAGE                                   6//page //6,7,8 [till 170inpage 8]

#define LAST_PERIODIC_MIN_OFFSET_ID                                     170//6*4=24
#define LAST_PERIODIC_MIN_OFFSET_ID_PAGE                                8//page

#define LAST_PERIODIC_MAX_OFFSET_ID                                     200
#define LAST_PERIODIC_MAX_OFFSET_ID_PAGE                                8//page

#define LAST_PERIODIC_AVG_OFFSET_ID                                     224
#define LAST_PERIODIC_AVG_OFFSET_ID_PAGE                                8//page
#endif	// end of file