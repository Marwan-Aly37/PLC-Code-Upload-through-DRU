 /******************************************************************************
 *
 *
 * File Name: Flash_unitTest.c
 *
 * Description: Testing the external flash memory FM25W32
 *
 * Date Created: 22/8/2026
 *
 * Author: Moustafa Ahmed
 *
 *******************************************************************************/

/**-------------------------INCLUDES Section-----------------------------**/
#include "../inc/Flash_unitTest.h"
#include "Flash.h"
#include "basic_srv.h" /* Hnst5dmha 3shan nuse el CRC */
#include "string.h"
/**----------------------END OF INCLUDES SECTION-------------------------**/

/**-------------------------Definitions Section--------------------------**/

/* Max operation times from FM25W32 datasheet */
#define PAGE_PROGRAM_DELAY_MS       ((uint32_t)3u)
#define SECTOR_ERASE_DELAY_MS       ((uint32_t)300u)
#define STATUS_WRITE_DELAY_MS       ((uint32_t)15u)
#define CHIP_ERASE_DELAY_MS         ((uint32_t)100000u)

#define CRC_BYTE_INDEX              ((uint16_t)(NUM_BYTES_PER_PAGE - 1u))
#define CRC_MODE                    ((uint8_t)CRC)
#define WEL_BIT_MASK                ((uint8_t)0x02u)

/* This starts at byte 200 and writes 300 bytes, So it can test page boundries */
#define CROSS_PAGE_START_BYTE       ((uint16_t)200u)
#define CROSS_PAGE_TEST_LENGTH      ((uint16_t)300u)

/* Fixed data to generate same random data every run. */
#define RANDOM_START_DATA           ((uint32_t)0xA55A5AA5u)

/*
 * FM25W32 protection 
 * SR1 SEC + BP0 protects upper 4 Kbytes. SEC | BP0 = 1
 * SR1 BP2 + BP1 + BP0 protects all memory. el OR bt3hom brdo 1
 */
#define PROTECT_UPPER_4KB_SR1       ((uint8_t)(SR_SEC | SR_BP0))
#define PROTECT_ALL_FLASH_SR1       ((uint8_t)(SR_BP2 | SR_BP1 | SR_BP0))
#define PROTECT_SR2_CLEAR           ((uint8_t)0x00u) /* da hnst5dmo 3shan nmake sure l value bta3t SR2 tkon zy mhya*/

/**----------------------END OF Definitions Section----------------------**/

/**-------------------------External Functions Section-------------------**/

/* These functions are inside Flash.c but not all of them are in Flash.h. */
extern uint8_t FLASH_Avaliable(void);
extern uint8_t FLASH_Read_Status_Register(uint8_t Register_Number, uint8_t* status_register_value);
extern uint8_t FLASH_Enable_Write(void);
extern uint8_t FLASH_gWritePage(uint8_t* Flash_Address, uint8_t* Data, uint16_t Data_length); /* nta shayl el check 3la el addresses hena */

/**----------------------END OF External Functions Section---------------**/

/**-------------------------Global Variables Section---------------------**/

volatile uint8_t  g_flash_unit_test_result = FLASH_UNIT_TEST_NOT_RUN;
volatile uint8_t  g_flash_unit_test_last_error = FLASH_UNIT_TEST_ERR_NONE;
volatile uint8_t  g_flash_unit_test_status_reg1 = 0u;
volatile uint8_t  g_flash_unit_test_status_reg2 = 0u;
volatile uint16_t g_flash_unit_test_sector = 0u;
volatile uint8_t  g_flash_unit_test_page = 0u;

/**----------------------END OF Global Variables Section-----------------**/

/**-------------------------Local Variables Section----------------------**/

static uint8_t write_page_buffer[NUM_BYTES_PER_PAGE];
static uint8_t read_page_buffer[NUM_BYTES_PER_PAGE];
static uint8_t write_cross_buffer[CROSS_PAGE_TEST_LENGTH];
static uint8_t read_cross_buffer[CROSS_PAGE_TEST_LENGTH];

/**----------------------END OF Local Variables Section------------------**/

/**-------------------------Local Functions Dec. Section-----------------**/

/* Save the error number and the flash location where the test failed. */
static void set_fail(uint8_t error, uint16_t sector, uint8_t page);

/* Convert sector/page/byte values to the 3-byte address format used by Flash.c. */
static void make_address(uint16_t sector, uint8_t page, uint16_t byte_index, uint8_t* address);

/* Wait the datasheet max time, then check that the flash is not busy. */
static uint8_t wait_flash_ready(uint32_t delay_ms);

/* Read status register 1 and 2, and save them in global watch variables. */
static uint8_t read_status_registers(void);

/* Clear flash protection bits before normal write/erase tests. */
static uint8_t clear_flash_protection(void);

/* Write protection bits to status register 1 for protection tests. */
static uint8_t set_flash_protection(uint8_t status_reg1);

/* Erase one 4 Kbyte sector and wait until erase time is finished. */
static uint8_t erase_one_sector(uint16_t sector);

/* Read one 256-byte page from flash. */
static uint8_t read_one_page(uint16_t sector, uint8_t page, uint8_t* data);

/* Write one 256-byte page to flash. */
static uint8_t write_one_page(uint16_t sector, uint8_t page, uint8_t* data);

/* Check that one page contains only 0xFF after erase. */
static uint8_t check_one_page_erased(uint16_t sector, uint8_t page);

/* Check that all 16 pages inside one sector are erased. */
static uint8_t check_sector_erased(uint16_t sector);

/* Fill one page with TEST_BYTE and put CRC in the last byte. */
static void fill_page_with_test_byte(uint8_t* data);

/* Fill one page with repeatable random data and put CRC in the last byte. */
static void fill_page_with_random_data(uint8_t* data, uint16_t sector, uint8_t page);

/* Fill 300 bytes to test writing across two pages. */
static void fill_cross_page_buffer(uint8_t* data);

/* Compare written data with read data and also check the CRC byte. */
static uint8_t compare_page_with_crc(uint8_t* expected, uint8_t* actual);

/* Regenerate the expected random page, read flash, then compare them. */
static uint8_t check_saved_random_page(uint16_t sector, uint8_t page);

/* Test status registers, flash busy state, and write enable latch. */
static uint8_t test_status_and_write_enable(void);

/* Test one page write, read back, compare, and CRC check. */
static uint8_t test_page_write_crc(void);

/* Test filling a sector, erasing it, then checking that it became 0xFF. */
static uint8_t test_sector_erase(void);

/* Test a write that starts near the end of a page and continues to next page. */
static uint8_t test_cross_page_write(void);

/* Destructive test: write and CRC-check every page in the whole flash. */
static uint8_t test_full_write_crc(void);

/* Destructive test: erase the whole flash and check that all bytes are 0xFF. */
static uint8_t test_full_erase(void);

/* Protect the upper sector, try to erase it, then make sure data is still there. */
static uint8_t test_upper_sector_protection(void);

/* Protect all flash, try chip erase, then make sure data is still there. */
static uint8_t test_full_chip_protection(void);

/* Run the shorter tests that do not scan the whole flash. */
static uint8_t test_all_safe(void);

/* Run all tests, including the full 4 Mbyte destructive tests. */
static uint8_t test_all_destructive(void);

/**----------------------END OF Local Functions Dec. Section-------------**/

/**------------------------Function Definition Section-------------------**/

uint8_t flash_unit_test_func(uint8_t test_number)
{
  uint8_t result = FLASH_UNIT_TEST_FAIL;

  g_flash_unit_test_result = FLASH_UNIT_TEST_NOT_RUN;
  g_flash_unit_test_last_error = FLASH_UNIT_TEST_ERR_NONE;
  g_flash_unit_test_sector = 0u;
  g_flash_unit_test_page = 0u;

  switch(test_number)
  {
  case FLASH_UNIT_TEST_CASE_STATUS:
    result = test_status_and_write_enable();
    break;

  case FLASH_UNIT_TEST_CASE_PAGE_WRITE_CRC:
    result = test_page_write_crc();
    break;

  case FLASH_UNIT_TEST_CASE_SECTOR_ERASE:
    result = test_sector_erase();
    break;

  case FLASH_UNIT_TEST_CASE_CROSS_PAGE_WRITE:
    result = test_cross_page_write();
    break;

  case FLASH_UNIT_TEST_CASE_FULL_WRITE_CRC:
    result = test_full_write_crc();
    break;

  case FLASH_UNIT_TEST_CASE_FULL_ERASE:
    result = test_full_erase();
    break;

  case FLASH_UNIT_TEST_CASE_UPPER_SECTOR_PROTECTION:
    result = test_upper_sector_protection();
    break;

  case FLASH_UNIT_TEST_CASE_FULL_CHIP_PROTECTION:
    result = test_full_chip_protection();
    break;

  case FLASH_UNIT_TEST_CASE_ALL_SAFE:
    result = test_all_safe();
    break;

  case FLASH_UNIT_TEST_CASE_ALL_DESTRUCTIVE:
    result = test_all_destructive();
    break;

  default:
    set_fail(FLASH_UNIT_TEST_ERR_WRONG_TEST_NUMBER, 0u, 0u);
    result = FLASH_UNIT_TEST_FAIL;
    break;
  }

  g_flash_unit_test_result = result;
  return result;
}
/*============================================================================*/
static void set_fail(uint8_t error, uint16_t sector, uint8_t page)
{
  g_flash_unit_test_last_error = error;
  g_flash_unit_test_sector = sector;
  g_flash_unit_test_page = page;
}
/*============================================================================*/
/*
 * Flash address format used by the Flash.h driver:
 * address[0] = byte inside page
 * address[1] = page number + low sector bits
 * address[2] = high sector bits
 * sector = 1023 = address[2] = 0x3F
 * page = 15 = address[1] = 0xFF
 * byte_index = 0 = address[0] = 0x00
 */
static void make_address(uint16_t sector, uint8_t page, uint16_t byte_index, uint8_t* address)
{
  address[0] = (uint8_t)(byte_index % NUM_BYTES_PER_PAGE);
  address[1] = (uint8_t)((page & 0x0Fu) | (((uint8_t)sector << 4u) & 0xF0u));
  address[2] = (uint8_t)(sector >> 4u);
}
/*============================================================================*/
/*
 * Description:
 * We need to make sure that we are able to run any operation on the flash memory
 */
static uint8_t wait_flash_ready(uint32_t delay_ms)
{
  Delay_ms(delay_ms);

  if(FLASH_Avaliable() != FLASH_NOT_BUSY)
  {
    set_fail(FLASH_UNIT_TEST_ERR_FLASH_BUSY, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
/*
 * Description:
 * hn2ra el status registers bta3t l flash w nmake sure 2nna msh bn receive 2y error
 */
static uint8_t read_status_registers(void)
{
  uint8_t status_reg1 = 0u;
  uint8_t status_reg2 = 0u;

  /* Call mn Flash.c */
  if(FLASH_Read_Status_Register(STATUS_REGISTER_1, &status_reg1) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_STATUS_READ, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(FLASH_Read_Status_Register(STATUS_REGISTER_2, &status_reg2) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_STATUS_READ, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  g_flash_unit_test_status_reg1 = status_reg1;
  g_flash_unit_test_status_reg2 = status_reg2;

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
/*
 * Description:
 * hnt2kd 2n mfysh 2y no3 mn l protection 3la 2y haga
 */
static uint8_t clear_flash_protection(void)
{
  /* hnwrite 3la REG1 and REG2 00 00  */
  if(FLASH_Write_Status_Register(0x00u, 0x00u) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_STATUS_WRITE, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }
  /* 2stna l7d mnt2kd 2n l flash n3ml nktb 3leha tmam */
  if(wait_flash_ready(STATUS_WRITE_DELAY_MS) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }
  
  /*hn2ra el status register tany */
  if(read_status_registers() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  /* hn3ml check tany w nt2kd 3aml remove l 2y protection wla la */
  if((g_flash_unit_test_status_reg1 & (uint8_t)(SR_SEC | SR_BP2 | SR_BP1 | SR_BP0)) != 0u)
  {
    set_fail(FLASH_UNIT_TEST_ERR_PROTECTION, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
/*
 * Description:
 * hn3ml protection
 */
static uint8_t set_flash_protection(uint8_t status_reg1)
{
  if(FLASH_Write_Status_Register(status_reg1, PROTECT_SR2_CLEAR) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_STATUS_WRITE, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(wait_flash_ready(STATUS_WRITE_DELAY_MS) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(read_status_registers() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if((g_flash_unit_test_status_reg1 & status_reg1) != status_reg1)
  {
    set_fail(FLASH_UNIT_TEST_ERR_PROTECTION, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
static uint8_t erase_one_sector(uint16_t sector)
{
  uint8_t address[3] = {0u};

  g_flash_unit_test_sector = sector;
  g_flash_unit_test_page = 0u;

  make_address(sector, 0u, 0u, address);

  if(FLASH_gSectorErase(address) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_ERASE, sector, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  return wait_flash_ready(SECTOR_ERASE_DELAY_MS);
}
/*============================================================================*/
static uint8_t read_one_page(uint16_t sector, uint8_t page, uint8_t* data)
{
  uint8_t address[3] = {0u};

  g_flash_unit_test_sector = sector;
  g_flash_unit_test_page = page;

  make_address(sector, page, 0u, address);

  if(FLASH_gReadPage(address, data, NUM_BYTES_PER_PAGE) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_READ, sector, page);
    return FLASH_UNIT_TEST_FAIL;
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
static uint8_t write_one_page(uint16_t sector, uint8_t page, uint8_t* data)
{
  uint8_t address[3] = {0u};

  g_flash_unit_test_sector = sector;
  g_flash_unit_test_page = page;

  make_address(sector, page, 0u, address);

  if(FLASH_gWritePage(address, data, NUM_BYTES_PER_PAGE) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_WRITE, sector, page);
    return FLASH_UNIT_TEST_FAIL;
  }

  return wait_flash_ready(PAGE_PROGRAM_DELAY_MS);
}
/*============================================================================*/
static uint8_t check_one_page_erased(uint16_t sector, uint8_t page)
{
  uint16_t i = 0u;

  if(read_one_page(sector, page, read_page_buffer) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  for(i = 0u; i < NUM_BYTES_PER_PAGE; i++)
  {
    if(read_page_buffer[i] != 0xFFu)
    {
      set_fail(FLASH_UNIT_TEST_ERR_NOT_ERASED, sector, page);
      return FLASH_UNIT_TEST_FAIL;
    }
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
static uint8_t check_sector_erased(uint16_t sector)
{
  uint8_t page = 0u;

  for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
  {
    if(check_one_page_erased(sector, page) != FLASH_UNIT_TEST_PASS)
    {
      return FLASH_UNIT_TEST_FAIL;
    }
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
static void fill_page_with_test_byte(uint8_t* data)
{
  uint16_t i = 0u;

  for(i = 0u; i < CRC_BYTE_INDEX; i++)
  {
    data[i] = TEST_BYTE;
  }

  data[CRC_BYTE_INDEX] = calc_crc(data, CRC_BYTE_INDEX, CRC_MODE);
}
/*============================================================================*/
static void fill_page_with_random_data(uint8_t* data, uint16_t sector, uint8_t page)
{
  uint16_t i = 0u;
  uint32_t random_value = RANDOM_START_DATA;

  random_value ^= ((uint32_t)sector << 12u);
  random_value ^= ((uint32_t)page << 4u);

  for(i = 0u; i < CRC_BYTE_INDEX; i++)
  {
    random_value = (random_value * 1103515245u) + 12345u;
    data[i] = (uint8_t)(random_value >> 16u);
  }

  data[CRC_BYTE_INDEX] = calc_crc(data, CRC_BYTE_INDEX, CRC_MODE);
}
/*============================================================================*/
static void fill_cross_page_buffer(uint8_t* data)
{
  uint16_t i = 0u;

  for(i = 0u; i < (CROSS_PAGE_TEST_LENGTH - 1u); i++)
  {
    data[i] = (uint8_t)(TEST_BYTE ^ (uint8_t)i);
  }

  data[CROSS_PAGE_TEST_LENGTH - 1u] = calc_crc(data, (CROSS_PAGE_TEST_LENGTH - 1u), CRC_MODE);
}
/*============================================================================*/
static uint8_t compare_page_with_crc(uint8_t* expected, uint8_t* actual)
{
  uint8_t crc = calc_crc(actual, CRC_BYTE_INDEX, CRC_MODE);

  if(actual[CRC_BYTE_INDEX] != crc)
  {
    set_fail(FLASH_UNIT_TEST_ERR_CRC, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(memcmp(expected, actual, NUM_BYTES_PER_PAGE) != 0)
  {
    set_fail(FLASH_UNIT_TEST_ERR_COMPARE, g_flash_unit_test_sector, g_flash_unit_test_page);
    return FLASH_UNIT_TEST_FAIL;
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
static uint8_t check_saved_random_page(uint16_t sector, uint8_t page)
{
  fill_page_with_random_data(write_page_buffer, sector, page);

  if(read_one_page(sector, page, read_page_buffer) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  return compare_page_with_crc(write_page_buffer, read_page_buffer);
}
/*============================================================================*/
static uint8_t test_status_and_write_enable(void)
{
  uint8_t status_reg1 = 0u;

  if(read_status_registers() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(FLASH_Avaliable() != FLASH_NOT_BUSY)
  {
    set_fail(FLASH_UNIT_TEST_ERR_FLASH_BUSY, 0u, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(FLASH_Enable_Write() != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_WRITE_ENABLE, 0u, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(FLASH_Read_Status_Register(STATUS_REGISTER_1, &status_reg1) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_STATUS_READ, 0u, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  g_flash_unit_test_status_reg1 = status_reg1;

  if((status_reg1 & WEL_BIT_MASK) == 0u)
  {
    set_fail(FLASH_UNIT_TEST_ERR_WRITE_ENABLE, 0u, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  return clear_flash_protection();
}
/*============================================================================*/
static uint8_t test_page_write_crc(void)
{
  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  fill_page_with_test_byte(write_page_buffer);

  if(write_one_page(FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u, write_page_buffer) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(read_one_page(FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u, read_page_buffer) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  return compare_page_with_crc(write_page_buffer, read_page_buffer);
}
/*============================================================================*/
static uint8_t test_sector_erase(void)
{
  uint8_t page = 0u;

  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
  {
    fill_page_with_random_data(write_page_buffer, FLASH_UNIT_TEST_SCRATCH_SECTOR, page);

    if(write_one_page(FLASH_UNIT_TEST_SCRATCH_SECTOR, page, write_page_buffer) != FLASH_UNIT_TEST_PASS)
    {
      return FLASH_UNIT_TEST_FAIL;
    }
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  return check_sector_erased(FLASH_UNIT_TEST_SCRATCH_SECTOR);
}
/*============================================================================*/
static uint8_t test_cross_page_write(void)
{
  uint8_t address[3] = {0u};

  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  fill_cross_page_buffer(write_cross_buffer);
  make_address(FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u, CROSS_PAGE_START_BYTE, address);

  if(FLASH_gWrite(address, write_cross_buffer, CROSS_PAGE_TEST_LENGTH) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_WRITE, FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(wait_flash_ready(PAGE_PROGRAM_DELAY_MS) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(FLASH_gRead(address, read_cross_buffer, CROSS_PAGE_TEST_LENGTH) != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_READ, FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(read_cross_buffer[CROSS_PAGE_TEST_LENGTH - 1u] !=
     calc_crc(read_cross_buffer, (CROSS_PAGE_TEST_LENGTH - 1u), CRC_MODE))
  {
    set_fail(FLASH_UNIT_TEST_ERR_CRC, FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(memcmp(write_cross_buffer, read_cross_buffer, CROSS_PAGE_TEST_LENGTH) != 0)
  {
    set_fail(FLASH_UNIT_TEST_ERR_COMPARE, FLASH_UNIT_TEST_SCRATCH_SECTOR, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  return FLASH_UNIT_TEST_PASS;
}
/*============================================================================*/
static uint8_t test_full_write_crc(void)
{
#if (FLASH_UNIT_TEST_ENABLE_FULL_CHIP_TESTS == 1u)
  uint16_t sector = 0u;
  uint8_t page = 0u;

  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  for(sector = 0u; sector < MAX_NUM_SECTORS; sector++)
  {
    if(erase_one_sector(sector) != FLASH_UNIT_TEST_PASS)
    {
      return FLASH_UNIT_TEST_FAIL;
    }

    for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
    {
      fill_page_with_random_data(write_page_buffer, sector, page);

      if(write_one_page(sector, page, write_page_buffer) != FLASH_UNIT_TEST_PASS)
      {
        return FLASH_UNIT_TEST_FAIL;
      }

      if(read_one_page(sector, page, read_page_buffer) != FLASH_UNIT_TEST_PASS)
      {
        return FLASH_UNIT_TEST_FAIL;
      }

      if(compare_page_with_crc(write_page_buffer, read_page_buffer) != FLASH_UNIT_TEST_PASS)
      {
        return FLASH_UNIT_TEST_FAIL;
      }
    }
  }

  return FLASH_UNIT_TEST_PASS;
#else
  set_fail(FLASH_UNIT_TEST_ERR_FULL_TEST_DISABLED, 0u, 0u);
  return FLASH_UNIT_TEST_FAIL;
#endif
}
/*============================================================================*/
static uint8_t test_full_erase(void)
{
#if (FLASH_UNIT_TEST_ENABLE_FULL_CHIP_TESTS == 1u)
  uint16_t sector = 0u;
  uint8_t page = 0u;

  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

#if (FLASH_UNIT_TEST_ENABLE_CHIP_ERASE == 1u)
  if(FLASH_gChipErase() != NO_ERROR)
  {
    set_fail(FLASH_UNIT_TEST_ERR_ERASE, 0u, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(wait_flash_ready(CHIP_ERASE_DELAY_MS) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }
#else
  for(sector = 0u; sector < MAX_NUM_SECTORS; sector++)
  {
    if(erase_one_sector(sector) != FLASH_UNIT_TEST_PASS)
    {
      return FLASH_UNIT_TEST_FAIL;
    }
  }
#endif

  for(sector = 0u; sector < MAX_NUM_SECTORS; sector++)
  {
    for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
    {
      if(check_one_page_erased(sector, page) != FLASH_UNIT_TEST_PASS)
      {
        return FLASH_UNIT_TEST_FAIL;
      }
    }
  }

  return FLASH_UNIT_TEST_PASS;
#else
  set_fail(FLASH_UNIT_TEST_ERR_FULL_TEST_DISABLED, 0u, 0u);
  return FLASH_UNIT_TEST_FAIL;
#endif
}
/*============================================================================*/
static uint8_t test_upper_sector_protection(void)
{
  uint8_t page = 0u;

  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  /* Fill last sector with data first. */
  for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
  {
    fill_page_with_random_data(write_page_buffer, FLASH_UNIT_TEST_SCRATCH_SECTOR, page);

    if(write_one_page(FLASH_UNIT_TEST_SCRATCH_SECTOR, page, write_page_buffer) != FLASH_UNIT_TEST_PASS)
    {
      return FLASH_UNIT_TEST_FAIL;
    }
  }

  /* Protect upper 4 Kbytes, then try to erase it. */
  if(set_flash_protection(PROTECT_UPPER_4KB_SR1) != FLASH_UNIT_TEST_PASS)
  {
    (void)clear_flash_protection();
    return FLASH_UNIT_TEST_FAIL;
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    (void)clear_flash_protection();
    return FLASH_UNIT_TEST_FAIL;
  }

  /* Data should still be there because the sector is protected. */
  for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
  {
    if(check_saved_random_page(FLASH_UNIT_TEST_SCRATCH_SECTOR, page) != FLASH_UNIT_TEST_PASS)
    {
      (void)clear_flash_protection();
      return FLASH_UNIT_TEST_FAIL;
    }
  }

  /* Clear protection and erase the scratch sector again to leave it clean. */
  if(clear_flash_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(erase_one_sector(FLASH_UNIT_TEST_SCRATCH_SECTOR) != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  return check_sector_erased(FLASH_UNIT_TEST_SCRATCH_SECTOR);
}
/*============================================================================*/
static uint8_t test_full_chip_protection(void)
{
#if (FLASH_UNIT_TEST_ENABLE_FULL_CHIP_TESTS == 1u)
#if (FLASH_UNIT_TEST_ENABLE_CHIP_ERASE == 1u)
  uint16_t sector = 0u;
  uint8_t page = 0u;

  if(test_full_write_crc() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(set_flash_protection(PROTECT_ALL_FLASH_SR1) != FLASH_UNIT_TEST_PASS)
  {
    (void)clear_flash_protection();
    return FLASH_UNIT_TEST_FAIL;
  }

  /*
   * Chip erase should not erase data when all flash is protected.
   */
  if(FLASH_gChipErase() != NO_ERROR)
  {
    (void)clear_flash_protection();
    set_fail(FLASH_UNIT_TEST_ERR_ERASE, 0u, 0u);
    return FLASH_UNIT_TEST_FAIL;
  }

  if(wait_flash_ready(CHIP_ERASE_DELAY_MS) != FLASH_UNIT_TEST_PASS)
  {
    (void)clear_flash_protection();
    return FLASH_UNIT_TEST_FAIL;
  }

  for(sector = 0u; sector < MAX_NUM_SECTORS; sector++)
  {
    for(page = 0u; page < NUM_PAGES_PER_SECTOR; page++)
    {
      if(check_saved_random_page(sector, page) != FLASH_UNIT_TEST_PASS)
      {
        (void)clear_flash_protection();
        return FLASH_UNIT_TEST_FAIL;
      }
    }
  }

  return clear_flash_protection();
#else
  set_fail(FLASH_UNIT_TEST_ERR_CHIP_ERASE_DISABLED, 0u, 0u);
  return FLASH_UNIT_TEST_FAIL;
#endif
#else
  set_fail(FLASH_UNIT_TEST_ERR_FULL_TEST_DISABLED, 0u, 0u);
  return FLASH_UNIT_TEST_FAIL;
#endif
}
/*============================================================================*/
static uint8_t test_all_safe(void)
{
  if(test_status_and_write_enable() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(test_page_write_crc() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(test_sector_erase() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(test_cross_page_write() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  return test_upper_sector_protection();
}
/*============================================================================*/
static uint8_t test_all_destructive(void)
{
  if(test_all_safe() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

  if(test_full_write_crc() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }

#if (FLASH_UNIT_TEST_ENABLE_CHIP_ERASE == 1u)
  if(test_full_chip_protection() != FLASH_UNIT_TEST_PASS)
  {
    return FLASH_UNIT_TEST_FAIL;
  }
#endif

  return test_full_erase();
}

/**---------------------------------------------------------------------**/
/**---------------------------END OF FUNC DEF---------------------------**/
