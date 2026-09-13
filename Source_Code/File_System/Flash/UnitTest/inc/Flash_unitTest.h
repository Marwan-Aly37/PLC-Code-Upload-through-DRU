 /******************************************************************************
 *
 *
 * File Name: Flash_unitTest.h
 *
 * Description: Testing the external flash memory FM25W32
 *
 * Date Created: 22/8/2026
 *
 * Author: Moustafa Ahmed
 *
 *******************************************************************************/

#ifndef Flash_Unit_Test_H_
#define Flash_Unit_Test_H_

/**-------------------------INCLUDES Section------------------------**/
#include "config.h"
#include "FLASH_Addresses.h"
/**----------------------END OF INCLUDES SECTION--------------------**/

/**-------------------------Definitions Section---------------------**/

/*
 * Change this to 1 only while testing.
 * Most of the tests erase/write external flash.
 */
#ifndef FLASH_UNIT_TEST_ENABLE
#define FLASH_UNIT_TEST_ENABLE                       (0u)
#endif

/*
 * This is the test number that will run from main.
 */
#ifndef FLASH_UNIT_TEST_SELECTED_CASE
#define FLASH_UNIT_TEST_SELECTED_CASE                FLASH_UNIT_TEST_CASE_STATUS
#endif

/*
 * Full flash tests erase/write all 4 Mbytes, so they are disabled by default.
 */
#ifndef FLASH_UNIT_TEST_ENABLE_FULL_CHIP_TESTS
#define FLASH_UNIT_TEST_ENABLE_FULL_CHIP_TESTS       (0u)
#endif

/*
 * Chip erase can take up to 100 seconds according to the datasheet.
 */
#ifndef FLASH_UNIT_TEST_ENABLE_CHIP_ERASE
#define FLASH_UNIT_TEST_ENABLE_CHIP_ERASE            (0u)
#endif

#ifndef TEST_BYTE
#define TEST_BYTE                                    ((uint8_t)0xAAu)
#endif

#define FLASH_UNIT_TEST_PASS                         ((uint8_t)0u)
#define FLASH_UNIT_TEST_FAIL                         ((uint8_t)1u)
#define FLASH_UNIT_TEST_NOT_RUN                      ((uint8_t)0xFFu)

/*
 * Short tests use the last sector as a scratch sector.
 */
#define FLASH_UNIT_TEST_SCRATCH_SECTOR               ((uint16_t)(MAX_NUM_SECTORS - 1u))

/* Test numbers */
#define FLASH_UNIT_TEST_CASE_STATUS                  ((uint8_t)0u)
#define FLASH_UNIT_TEST_CASE_PAGE_WRITE_CRC          ((uint8_t)1u)
#define FLASH_UNIT_TEST_CASE_SECTOR_ERASE            ((uint8_t)2u)
#define FLASH_UNIT_TEST_CASE_CROSS_PAGE_WRITE        ((uint8_t)3u)
#define FLASH_UNIT_TEST_CASE_FULL_WRITE_CRC          ((uint8_t)4u)
#define FLASH_UNIT_TEST_CASE_FULL_ERASE              ((uint8_t)5u)
#define FLASH_UNIT_TEST_CASE_UPPER_SECTOR_PROTECTION ((uint8_t)6u)
#define FLASH_UNIT_TEST_CASE_FULL_CHIP_PROTECTION    ((uint8_t)7u)
#define FLASH_UNIT_TEST_CASE_ALL_SAFE                ((uint8_t)8u)
#define FLASH_UNIT_TEST_CASE_ALL_DESTRUCTIVE         ((uint8_t)9u)

/* Error numbers to watch in debugger */
#define FLASH_UNIT_TEST_ERR_NONE                     ((uint8_t)0u)
#define FLASH_UNIT_TEST_ERR_WRONG_TEST_NUMBER        ((uint8_t)1u)
#define FLASH_UNIT_TEST_ERR_FLASH_BUSY               ((uint8_t)2u)
#define FLASH_UNIT_TEST_ERR_STATUS_READ              ((uint8_t)3u)
#define FLASH_UNIT_TEST_ERR_STATUS_WRITE             ((uint8_t)4u)
#define FLASH_UNIT_TEST_ERR_WRITE_ENABLE             ((uint8_t)5u)
#define FLASH_UNIT_TEST_ERR_ERASE                    ((uint8_t)6u)
#define FLASH_UNIT_TEST_ERR_WRITE                    ((uint8_t)7u)
#define FLASH_UNIT_TEST_ERR_READ                     ((uint8_t)8u)
#define FLASH_UNIT_TEST_ERR_COMPARE                  ((uint8_t)9u)
#define FLASH_UNIT_TEST_ERR_CRC                      ((uint8_t)10u)
#define FLASH_UNIT_TEST_ERR_NOT_ERASED               ((uint8_t)11u)
#define FLASH_UNIT_TEST_ERR_PROTECTION               ((uint8_t)12u)
#define FLASH_UNIT_TEST_ERR_FULL_TEST_DISABLED       ((uint8_t)13u)
#define FLASH_UNIT_TEST_ERR_CHIP_ERASE_DISABLED      ((uint8_t)14u)

/**---------------------END OF Definitions Section------------------**/

/**-------------------------Function Dec. Section-------------------**/

/*
 * Watch these variables in IAR after calling flash_unit_test_func().
 */
extern volatile uint8_t  g_flash_unit_test_result;
extern volatile uint8_t  g_flash_unit_test_last_error;
extern volatile uint8_t  g_flash_unit_test_status_reg1;
extern volatile uint8_t  g_flash_unit_test_status_reg2;
extern volatile uint16_t g_flash_unit_test_sector;
extern volatile uint8_t  g_flash_unit_test_page;

/*
 * Call this function with one of FLASH_UNIT_TEST_CASE_xxx test numbers.
 */
uint8_t flash_unit_test_func(uint8_t test_number);

/**-----------------------------------------------------------------**/

#endif /* Flash_Unit_Test endif */
