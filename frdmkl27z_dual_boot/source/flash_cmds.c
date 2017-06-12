/*
 * flash_cmds.c
 *
 *  Created on: 5. jun. 2017
 *      Author: morte
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//
#include "board.h"
//
#include "fsl_shell.h"
#include "fsl_debug_console.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "clock_config.h"
#include "fsl_flash.h"

#include "pin_mux.h"

#include "flash_cmds.h"



/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Flash driver Structure */
static flash_config_t s_flashDriver;
/*! @brief Buffer for program */
//static uint32_t s_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
//static uint32_t s_buffer_rbc[BUFFER_LEN];

flash_security_state_t securityStatus = kFLASH_SecurityStateNotSecure; /* Return protection status */
status_t result;    /* Return code from each flash driver function */
uint32_t destAdrss; /* Address of the target location */
uint32_t i, failAddr, failDat;

uint32_t pflashBlockBase = 0;
uint32_t pflashTotalSize = 0;
uint32_t pflashSectorSize = 0;



/*******************************************************************************
 * Code
 ******************************************************************************/

/*
* @brief Gets called when an error occurs.
*
* @details Print error message and trap forever.
*/
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}

/*
* @brief Gets called when the app is complete.
*
* @details Print finshed message and trap forever.
*/
void app_finalize(void)
{
    /* Print finished message. */
    PRINTF("\r\n Starting firmware ... \r\n");
    while (1)
    {
    }
}


/*!
 * @brief Use Standard Software Drivers (SSD) to modify pflash.
 *
 * @details This function uses SSD.
 */
void InitFlash(void)
{
	/* Clean up Flash driver Structure*/
	memset(&s_flashDriver, 0, sizeof(flash_config_t));

	/* Setup flash driver structure for device and initialize variables. */
	result = FLASH_Init(&s_flashDriver);
	if (kStatus_FLASH_Success != result)
	{
	    error_trap();
	}

    /* Get flash properties*/
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);

    /* Print flash information - PFlash. */
    PRINTF("\r\n PFlash Information: ");
    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);

    /* Check security status. */
    result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Print security status. */
    switch (securityStatus)
    {
        case kFLASH_SecurityStateNotSecure:
            PRINTF("\r\n Flash is UNSECURE!");
            break;
        case kFLASH_SecurityStateBackdoorEnabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
            break;
        case kFLASH_SecurityStateBackdoorDisabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
            break;
        default:
            break;
    }
    PRINTF("\r\n");
}


int WriteFlashByte(uint32_t addr, uint8_t inData)
{
	uint32_t iData = (uint32_t)inData;

	if (addr < 0x5000)
	{
		PRINTF("\r\n Illegal address - must be above 0x5000!");
		return 1;
	}
	if (addr % pflashSectorSize != 0)
	{
		PRINTF("\r\n Illegal address - not aligned to Flash SectorSize!");
		return 1;
	}

    /* Try ERASE only if flash is unsecure. */
    if (kFLASH_SecurityStateNotSecure != securityStatus)
    {
    	PRINTF("\r\n Illegal Flash security status - cannot reprogram address!");
		return 1;
	}

    PRINTF("\r\n Erasing Flash sector");
	//destAdrss = pflashBlockBase + (pflashTotalSize - pflashSectorSize);

	result = FLASH_Erase(&s_flashDriver, addr, pflashSectorSize, kFLASH_ApiEraseKey);
	if (kStatus_FLASH_Success != result)
	{
		PRINTF("\r\n Erasing Flash FAILED!");
		error_trap();
	}

//	/* Verify sector if it's been erased. */
//	result = FLASH_VerifyErase(&s_flashDriver, addr, pflashSectorSize, kFLASH_MarginValueUser);
//	if (kStatus_FLASH_Success != result)
//	{
//		error_trap();
//	}

	PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", addr, (addr + pflashSectorSize));

	//PRINTF("\r\n Program byte at beginning of flash sector ...");
	/* Program user buffer into flash*/
	result = FLASH_Program(&s_flashDriver, addr, &iData, sizeof(iData));
	if (kStatus_FLASH_Success != result)
	{
		PRINTF("\r\n Writing to Flash FAILED!");
		error_trap();
	}

	/* Verify programming by Program Check command with user margin levels */
//	result = FLASH_VerifyProgram(&s_flashDriver, addr, sizeof(inData), s_buffer, kFLASH_MarginValueUser,
//								 &failAddr, &failDat);
//	if (kStatus_FLASH_Success != result)
//	{
//		error_trap();
//	}

	//PRINTF("\r\n WRITE to Flash successful!");

    return 0;
}


int ReadFlashByte(uint32_t addr, uint8_t *outData)
{
	if (addr < 0x5000)
	{
		PRINTF("\r\n Illegal address - must be above 0x5000!");
		return 1;
	}
	if (addr % pflashSectorSize != 0)
	{
		PRINTF("\r\n Illegal address - not aligned to Flash SectorSize!");
		return 1;
	}

	*outData = *(volatile uint32_t *)(addr);

    return 0;
}



