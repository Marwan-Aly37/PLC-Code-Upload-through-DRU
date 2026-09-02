/**
 * @author Ahmed Elhossiny
 * @create date 2022-12-1
 * @desc LCD_7567_INTERFACING.c
 */

#include "dependencies_layer.h"
#include "ctrl.h"
#include "Flash.h"
#include "CLK_Interface.h"
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "dependencies_layer.h" -- see dependencies_layer.h */
/* #include "keypad.h" */ /* module removed from the DRU build */
#include "v85xx_flash.h"  
#include "comm.h"
#ifdef DLMS_UI
/* #include "UI.h" */ /* module removed from the DRU build */
#endif
#if defined (DOT_MATRIX_LCD_ENABLE)
static uint8_t screen_value[128] = {0};          // buffer to receive screen data from flash
static uint8_t buffer[256] = {0};                // buffer to receive any item from flash
static uint8_t DataBuffer[PAGESIZE] = {0};
uint8_t value_array[11] = {0};            // array for value
static uint16_t pckt_id_of_template = 0;
uint8_t bitmap_buf[176][8];
extern uint8_t minus_Flag;
extern uint8_t dot_matrix_date_time_flag;
extern uint8_t cont_byte_screen;
extern uint8_t DotMatrixOpticalFlag;
extern uint8_t CoverClosedFlag;
extern uint8_t  LedFlag;
uint8_t DM_integraty_template_crc[20] = {0};
uint8_t disp_msg_buffer[28]; //for display_message
uint8_t r_w_flag=0;
uint32_t tmp_sect=0;
uint8_t modify_shift_byte=0;
uint8_t get_decimal_point_index=0;
// #define CHANGE_LANG_DEBUG
#ifdef CHANGE_LANG_DEBUG
uint8_t debug_change_language=0;
#endif
#ifdef CRC_INTEGRITY_ENABLE    
uint8_t Signature_val = 0x55;
#endif
const uint8_t all_screens[] = {
    Test_Screen,
    Remaining,
    Total_consumption,
    Active_Energy,
    Reverse_Energy,
    Earth_Energy_Fault_Energy,
    Charges,
    Date_Time,
    Alarm_Limit,
    Voltage_Instantaneous,
    Current_Instantaneous,
    DM_SCR_FREQUENCY,
    Power_Factor_Instantaneous,
    Active_Power_Instantaneous,
    ID,
    Tamper_Count,
    Tampering_indication_in_the_register,
    MDA_Billing_Period,
    MDA_Billing_Period_for_tariff,
    MD_KW_Billing_Period,
    MD_KW_Billing_Period_for_tariff,
    MDA_meter_life,
    MD_KW_meter_life,
    MD_reset_count,
    Level,
    Meter_reading,
    Pulse_indication_test,
    Load_Cost_per_Hours,
    Tariff_prices,
    Tariff_steps,
    Taxes,
    MD_KVA_Billing_Period,
    MD_KVA_Billing_Period_for_tariff,
    MD_KVA_meter_life,
    Tariff_Active_Energy_KWh,
    Power_Interrupt,
    Total_Reactive_energy,
    Total_apparent_energy,
    Reactive_power,
    Apparent_power,
    Step_remaining,
    Billing_period_consumption,
    Billing_period_bill,
    Friendly_consumption,
    start_of_time_and_date_tamper_event,
    stop_of_time_and_date_tamper_event,
    Test_Accuracy,
    Last_12Consumption_In_KWH_And_Money,
    Total_Debit,
    Last_12Customer_Services_Taxes,
    State_of_relay,
    Current_Level_KWH_per_Current_Step_Tariff,
    Sum_of_charge_interfaces,
    Tamper_counter,
    Error_of_contactor,
    Date_of_Last_charge,
    Department_ID,
    Overload_Counter,
    Battery_Screen,
    Last_terminal_cover_tamper,
    Selected_screens,
    Reserved1,
    Active_4Q_consumption,
    Reactive_4Q_consumption,
    Indirect_param,
    Reserved2, //65
    BPH_reactive_consumption_in_KVAR_for_each_Billing_Record, //66
    BPH_MDW_consumption_in_KW_for_each_Billing, //67
    STS_TEST0_Screens, //BPH_Power_factor_for_each_Billing_Record, //68
    TOKENS_SCREENS,    //Total_Export_Energy, //69
    Total_import_and_export_Energy, //70
    Total_import_Energy, //71
    Reserved3,//72
    sts_code, //73
    disp_message, //74
    GPRS_Screens //75
};


/// @brief function to get address from flash for any screen.
/// @param index
/// @param sub_index
/// @return counter
uint32_t Get_Screen_Address(uint8_t index, uint8_t sub_index)
{
    uint8_t offset = 128;
    uint32_t temp_add = 0;
    uint32_t counter = 0;
    uint8_t temp_modify_shift_byte=0;
    tmp_sect=0;
    if (r_w_flag==WRITING)
    {
        temp_modify_shift_byte=modify_shift_byte;
        
    }
    else if(r_w_flag==READING)
    {
        temp_modify_shift_byte=DOT_MATRIX_CTRL_CHANGE_LANG_FLAG;
    }
    tmp_sect=get_sector_offset(temp_modify_shift_byte);
    
    temp_add = (LN1_LCD_SECTOR_ADDRESS_SCREEN_0 +tmp_sect) + ((offset) * (sub_index));
    counter += temp_add;
    for (int16 i = index - 1; i >= INDEX_Test_Screen; i--)
    {
        counter += ((offset) * (all_screens[i]));
    }
    return counter;
}

/// @brief get sector by passing page number
/// @param num 
/// @return 
uint32_t Get_Sym_Sector(uint8_t num)
{
    uint32_t symbol_add = 0;
    tmp_sect=get_sector_offset(modify_shift_byte);
    if (num >= 1 && num <= 15)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_BIG_DIGITS;
    }
    else if (num >= 16 && num <= 31)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_SYMBOLS1;
    }
    else if (num >= 32 && num <= 47)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_SYMBOLS2;
    }
    else if (num >= 48 && num <= 63)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_AR_WORDS1;
    }
    else if (num >= 64 && num <= 79)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_AR_WORDS2;
    }
    else if (num >= 80 && num <= 95)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_UNITS;
    }
    else if (num >= 96 && num <= 111)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_ALPHA1;
    }
    else if (num >= 112 && num <= 127)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_ALPHA2;
    }
    else if (num >= 128 && num <= 143)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_SMALL_DIGITS;
    }
    else if (num >= 144 && num <= 159)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_AR_WORDS3;
    }
    else if (num >= 160 && num <= 175)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_AR_WORDS4;
    }
    else if (num >= 176 && num <= 191)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_AR_WORDS5;
    }
    else if (num >= 192 && num <= 207)
    {
        symbol_add = LN1_LCD_SECTOR_ADDRESS_SYMBOLS3;
    }
    return (symbol_add + tmp_sect);
}

uint32_t Get_Sym_Sector_internal(uint8_t num)
{
  
  /** Sector Size equal to 512 and num is number of page size for external 256***/ 
    return (num/2)*PAGESIZE;
}
/// @brief get sector py passing sector address
/// @param add 
/// @return 
uint32_t Get_Scr_Sector(uint32_t add)
{

    tmp_sect=get_sector_offset(modify_shift_byte);

    if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_0+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_1+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_0;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_1+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_2+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_1;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_2+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_3+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_2;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_3+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_4+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_3;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_4+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_5+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_4;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_5+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_6+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_5;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_6+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_7+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_6;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_7+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_8+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_7;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_8+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_9+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_8;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_9+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_SCREEN_10+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_9;
    }
    else if (add >= LN1_LCD_SECTOR_ADDRESS_SCREEN_10+tmp_sect && add < LN1_LCD_SECTOR_ADDRESS_BIG_DIGITS+tmp_sect)
    {
        add = LN1_LCD_SECTOR_ADDRESS_SCREEN_10;
    }
    return (add + tmp_sect);
}

/// @brief function to get size for buffer to reduce time access in flash 
/// @param address
/// @param page
/// @param data
/// @return size
uint16_t Get_Symbol_Size_R_W(uint32_t address, uint8_t page, uint8_t *data, uint8_t mode)
{
    uint8_t rest_of_bits = 0;
    uint8_t Width = 0;
    uint8_t Height = 0;
    uint16_t size = 0;
    if (mode == DATA_WRITE)
    {
        rest_of_bits = data[7] % 8;
        Width = data[6];
        Height = data[7] / 8;
    }
    else if (mode == DATA_READ)
    {
        Flash_Read_Per_Page(address, page, data, 2);
        rest_of_bits = data[1] % 8;
        Width = data[0];
        Height = data[1] / 8;
    }

    if (rest_of_bits > 0 && rest_of_bits < 8)
    {
        Height++;
    }
    size = (Width * Height) + 2;
    return size;
}


uint16_t Writing_On_Flash(uint8_t *data_arr)
{
#if 0
#ifdef DOT_MATRIX_LCD_ENABLE
uint16_t size=DOT_MATRIX_FRAME_SIZE;
#ifdef CHANGE_LANG_DEBUG
    if(debug_change_language==1) //write on second language 
    {
    // Set bit 4
    data_arr[1] |= (1 << 4);     
    // Clear erase bit 2
    data_arr[1] &= ~(1 << 1);
    }
    if(debug_change_language==2)  //write on third language
    {
    // Set bit 4
    data_arr[1] |= (1 << 5);     
    // Clear erase bit 2
    data_arr[1] &= ~(1 << 1);
    }
#endif    

    
    r_w_flag = WRITING;
    modify_shift_byte= ((data_arr[1]&0x30)>>4);     //(0x30) bits 4 & 5 controling language    
    if (CHECK_BITT(data_arr[1], ERASE_ALL_SECT)) // erase all sectors
    {
        DOT_MATRIX_SAVE_CRITICAL_DATA();        
#ifdef CRC_INTEGRITY_ENABLE        
        FLASH_SectorErase(dotmatrixcrc);
        Delay_ms(4);
        LedFlag = LED_FLASHING_ACCPT;
#endif
#ifdef SAVING_DIRECT_INTERNAL 
        for(uint16_t i=0 ; i< NUM_SECTORS_DOTMATRIX_INTERNAL;i++)
        {
          FLASH_SectorErase(dotmatrixpage+(i*PAGESIZE));
          Delay_ms(4);
        }      
#else
        uint32_t addr=(modify_shift_byte * 0x20000)+LN1_LCD_SECTOR_ADDRESS_SCREEN_0;
        Flash_erase_block_by_address(addr); // this address is first address in block
        Delay_ms(400);      
        if(modify_shift_byte==0)
        {
          erase_flash_sectors_from_to(LN1_LCD_SECTOR_NUM_AR_WORDS4,LN1_LCD_SECTOR_NUM_ALPHA2);
        }
        else if(modify_shift_byte==1)
        {
          erase_flash_sectors_from_to(LN2_LCD_SECTOR_NUM_SCREEN_0,LN2_LCD_SECTOR_NUM_SCREEN_6);
          erase_flash_sectors_from_to(LN2_LCD_SECTOR_NUM_ALPHA1,LN2_LCD_SECTOR_NUM_ALPHA2);
        }
        else if(modify_shift_byte==2)
        {
          erase_flash_sectors_from_to(LN3_LCD_SECTOR_NUM_SCREEN_0,LN3_LCD_SECTOR_NUM_AR_WORDS1);
        }    
#endif           
        FLASH_SectorErase(dotmatrixdata);
        Delay_ms(4);
        FLASH_ProgramByte(dotmatrixdata,(uint8_t*)&modify_shift_byte,1);
        pckt_id_of_template = 0;
        memset(DM_integraty_template_crc,0,sizeof(DM_integraty_template_crc));
    }
#ifdef CRC_INTEGRITY_ENABLE          
      calc_DM_template_integraity(modify_shift_byte,&data_arr[6]);
#endif  
    /************************************************************************************************************/
    if (CHECK_BITT(data_arr[1], CHANGE_SYM_OR_SCR)) // change one symbol or one screen in sector without change of the rest of sector.
    {
        // 1- check which sector and which page i will change data on it.
        uint32_t sym_add = Get_Sym_Sector(data_arr[2]);
        uint8_t sym_page = page_mapping(data_arr[2]);
        uint32_t scr_add = Get_Screen_Address(data_arr[2], data_arr[3]);
        uint32_t scr_sector = Get_Scr_Sector(scr_add);
        uint8_t scr_page = (uint8_t)((scr_add - scr_sector) / 256);
        uint8_t screen_location = (uint8_t)((scr_add - scr_sector) % 256); // to check i will copy which screen in page
        uint32_t temp_add = 0;
        uint32_t sector = 0xffffffff;

        if (CHECK_BITT(data_arr[1], SYM_OR_SCR) == WRITE_SYMBOL)
        {
            sector = sym_add;
        }
        else if ((CHECK_BITT(data_arr[1], SYM_OR_SCR) == WRITE_SCREEN))
        {
            sector = scr_sector;
        }
        // 2-copy data of sector to buffer sector.
        for (uint8_t i = 0; i < 16; i++)
        {
            if (((uint8_t)Flash_Read_Per_Page(sector, i, buffer, sizeof(buffer))) != 0)
                break;

            Flash_Write_Per_Page(BUFFER_SECTOR_ADDRESS, i, buffer, sizeof(buffer));
            memset(buffer, 0, sizeof(buffer));
        }

        // 3- erase sector.
        Flash_erase_sector_by_address(sector);
        Delay_ms(80);

        // 4-back all data again to the sector without the page which i need to change it.
        for (uint8_t j = 0; j < 16; j++)
        {
            if (j == sym_page)
            {
                continue;
            }
            if (((uint8_t)Flash_Read_Per_Page(BUFFER_SECTOR_ADDRESS, j, buffer, sizeof(buffer))) != 0)
                break;

            Flash_Write_Per_Page(sector, j, buffer, sizeof(buffer));
            memset(buffer, 0, sizeof(buffer));
        }

        if (CHECK_BITT(data_arr[1], SYM_OR_SCR) == WRITE_SYMBOL)
        {

            // 5- receive the new data from CMD and save it.
            uint16_t sym_size = Get_Symbol_Size_R_W(0, 0, data_arr, DATA_WRITE);
            memcpy(&buffer[0], &data_arr[6], sym_size);
            Flash_Write_Per_Page(sym_add, sym_page, buffer, sym_size);
        }

        else if ((CHECK_BITT(data_arr[1], SYM_OR_SCR) == WRITE_SCREEN))
        {

            // 5-in one page i have two screens , so i get the second screen and back it again
            Flash_Read_Per_Page(BUFFER_SECTOR_ADDRESS, scr_page, buffer, sizeof(buffer));
            if (screen_location == 0)
            {
                memcpy(&screen_value[0], &buffer[128], 128);
                temp_add = scr_add + 128;
            }
            else
            {
                memcpy(&screen_value[0], &buffer[0], 128);
                temp_add = scr_add;
            }
            FlashWrite(temp_add, screen_value, sizeof(screen_value));
            memset(screen_value, 0, sizeof(screen_value));

            // 6- receive the new data from CMD and save it.
            memcpy(&screen_value[0], &data_arr[6], 128);
            FlashWrite(scr_add, screen_value, sizeof(screen_value));
        }

        // 6-erase buffer sector
        Flash_erase_sector_by_address(BUFFER_SECTOR_ADDRESS);
        Delay_ms(150);
    }
    /************************************************************************************************************/
    if (CHECK_BITT(data_arr[1], SYM_OR_SCR) == WRITE_SCREEN) // write screen
    {
        //start_systick_time_measure(1, milli_sec);
        uint32_t Scr_Add_1 = 0;
        uint32_t Scr_Add_2 = 0;
        if (CHECK_BITT(data_arr[1], ONE_OR_TWO_SCR)) // option send one screen or two screens , if bit2 =1 use 2 screens mode
        {

            Scr_Add_1 = Get_Screen_Address(data_arr[2], data_arr[3]);
            memcpy(&screen_value[0], &data_arr[6], 128);           
#ifdef SAVING_DIRECT_INTERNAL
            FlashWrite_Internal(Scr_Add_1, screen_value, sizeof(screen_value));            
#else
            FlashWrite(Scr_Add_1, screen_value, sizeof(screen_value));
#endif
            Scr_Add_2 = Get_Screen_Address(data_arr[4], data_arr[5]);
            memcpy(&screen_value[0], &data_arr[134], 128);
#ifdef SAVING_DIRECT_INTERNAL              
            FlashWrite_Internal(Scr_Add_2, screen_value, sizeof(screen_value)); 
#else
            FlashWrite(Scr_Add_2, screen_value, sizeof(screen_value));
#endif

        }
        else // use one screen mode
        {
            Scr_Add_1 = Get_Screen_Address(data_arr[2], data_arr[3]);
            memcpy(&screen_value[0], &data_arr[6], 128);
 #ifdef SAVING_DIRECT_INTERNAL              
            FlashWrite_Internal(Scr_Add_1, screen_value, sizeof(screen_value));
#else           
            FlashWrite(Scr_Add_1, screen_value, sizeof(screen_value));
#endif
        }
        //S_Get_SysTick_Counts();
    }

    else if (CHECK_BITT(data_arr[1], SYM_OR_SCR) == WRITE_SYMBOL) // write symbol
    {
 
        uint16_t sym_size = Get_Symbol_Size_R_W(0, 0, data_arr, DATA_WRITE);
        memcpy(&buffer[0], &data_arr[6], sym_size);
        uint32_t sym_add = Get_Sym_Sector(data_arr[2]);
#ifdef SAVING_DIRECT_INTERNAL 
        Flash_Write_Per_Page_Internal(sym_add, data_arr[2], buffer, sym_size);
#else         
        Flash_Write_Per_Page(sym_add, data_arr[2], buffer, sym_size);
#endif
        sym_size = 0;
    }
    
    memset(buffer, 0, sizeof(buffer));
    memset(screen_value, 0, sizeof(screen_value));

    return size;
#endif
}


void DotMatrixSaveInternalFlash(void)
{ 
  uint32_t address = 0;
  
  if((DotMatrixOpticalFlag)&&((CommGetSendDataFlag()) || (CoverClosedFlag)))
  {
    memcpy((uint8_t*)&modify_shift_byte,(uint8_t*)dotmatrixdata,1);
    
    if(modify_shift_byte == 0)   
      address = LN1_LCD_SECTOR_ADDRESS_SCREEN_0;    
    else if(modify_shift_byte == 1) 
      address = LN2_LCD_SECTOR_ADDRESS_SCREEN_0;  
    else    
      address = LN3_LCD_SECTOR_ADDRESS_SCREEN_0;   
    
    for(uint16_t i=0 ; i< NUM_SECTORS_DOTMATRIX_INTERNAL;i++)
    {
      FLASH_SectorErase(dotmatrixpage+(i*PAGESIZE));
      Delay_ms(4);
    } 
    for(uint16_t i=0;i <= NUM_SECTORS_DOTMATRIX_INTERNAL;i++)
    {
      memset(DataBuffer,0,PAGESIZE);
      FlashRead(address+(i*PAGESIZE),DataBuffer,PAGESIZE);
      FLASH_ProgramByte(dotmatrixpage+(i*PAGESIZE),DataBuffer,PAGESIZE);
    } 
    DotMatrixOpticalFlag = 0;
  }
#endif
}

/// @author Ahmed Elhossiny
/// @brief function to get sector offset
/// @date 3-10-2023
uint32_t get_sector_offset(uint8_t offset_sector)
{
  uint32_t result=0;
  if(offset_sector == LANGUAGE1)
    {
        result=DOT_MATRIX_LANGUAGE1_OFFSET;
    }
    else if(offset_sector == LANGUAGE2)
    {
        result=DOT_MATRIX_LANGUAGE2_OFFSET;   //go to english sectors
    }
    else if(offset_sector == LANGUAGE3)
    {
        result=DOT_MATRIX_LANGUAGE3_OFFSET;      //go to french sectors
    }
    return result;
}

void erase_flash_sectors_from_to(uint16_t start,uint16_t end)
{
    for(uint16_t i=start;i<=end;i++)
    {
        FLASH_gEraseSector_ByIndex(i ,100);
    }
}


void erase_dot_matrix_language_section(uint8_t lang_num)
{
    uint8_t modify_shift_byte = 0;
    uint32_t addr = 0;

    for (uint8_t i = 0; i < 3; i++) // check for 3 languages mask
    {
        if (((lang_num >> i) & 1))
        {
            modify_shift_byte = i;
            addr = (modify_shift_byte * 0x20000) + LN1_LCD_SECTOR_ADDRESS_SCREEN_0;
            Flash_erase_block_by_address(addr); // this address is first address in block
            Delay_ms(400);
            if (modify_shift_byte == 0)
            {
                erase_flash_sectors_from_to(LN1_LCD_SECTOR_NUM_AR_WORDS4, LN1_LCD_SECTOR_NUM_ALPHA2);
            }
            else if (modify_shift_byte == 1)
            {
                erase_flash_sectors_from_to(LN2_LCD_SECTOR_NUM_SCREEN_0, LN2_LCD_SECTOR_NUM_SCREEN_6);
                erase_flash_sectors_from_to(LN2_LCD_SECTOR_NUM_ALPHA1, LN2_LCD_SECTOR_NUM_ALPHA2);
            }
            else if (modify_shift_byte == 2)
            {
                erase_flash_sectors_from_to(LN3_LCD_SECTOR_NUM_SCREEN_0, LN3_LCD_SECTOR_NUM_AR_WORDS1);
            }
        }
    }

}


void calc_DM_template_integraity(uint8_t lang_num, uint8_t *data_ptr)
{
    uint8_t tmp_buf[3] = {0};
    uint8_t current_template_crc = 0;
    
    current_template_crc = calc_crc(data_ptr, (DOT_MATRIX_FRAME_SIZE-6) , CRC);
    tmp_buf[0] = DM_integraty_template_crc[lang_num];
    tmp_buf[1] = current_template_crc;
    tmp_buf[2] = pckt_id_of_template;
    DM_integraty_template_crc[lang_num] = calc_crc(tmp_buf, 3, CRC);
    pckt_id_of_template++;
}


#endif