/*
 * fw_boot.c
 *
 *  Created on: 7. jun. 2017
 *      Author: morte
 */

//#include "mcu.h"

#include "board.h"
#include "flash_cmds.h"
#include "fsl_debug_console.h"
//
#include "fw_boot.h"

/*
#define FW_IMG_1_ADDR	0x6000
#define FW_IMG_2_ADDR	0xB000
*/

volatile uint8_t gImgNum = 0;


void _JumpToUserApplication(unsigned long userSP, unsigned long userStartup)
{
	// set up stack pointer
	__asm("msr msp, r0");
	__asm("msr psp, r0");

	// Jump to PC (r1)
	__asm("mov pc, r1");

	return;
}

void JumpToUserApplication(unsigned long reloadVector)
{
	unsigned long *p = (unsigned long *)reloadVector;
	//Cpu_SetClockConfiguration(CPU_CLOCK_CONFIG_1);
	SysTick->CTRL = 0;	// Also named SYST->CSR : SysTick Control&Status Reg.
	//SCB->SCR = 0;
	SCB->VTOR = reloadVector;
	_JumpToUserApplication(*p, *(p+1));

	return;	// NOTE: never to be reached ...
}

bool CheckApplicationReady(unsigned long reloadVector)
{
	unsigned long *p = (unsigned long *)reloadVector;
	if(*(p + 8) != 0xffffffff)
	{
		return true;
	}
	return false;
}


// Dummy
bool ResetBySW(void)
{
	return true;
}

// Dummy: must be replaced by XModem-over-UDP updater.
bool RunFwUpdate(bool emulateFailure)
{
	if ( emulateFailure )
		return false;
	else
		return true;
}


void StartFirmware(void)
{
	uint32_t fwStartAddr = 0;

	if ( gImgNum == 1 || gImgNum == 2 )
	{
		fwStartAddr = FW_OFFSET + (gImgNum - 1)*FW_SZ ;
	}
	else	// TODO: rather just set gImgNum=1 if invalid?
	{
		PRINTF("\r\nInvalid image number - cannot start!\r\n");

		return;
	}

	if ( CheckApplicationReady(fwStartAddr) == false )
	{
		PRINTF("\r\nNo valid firmware - cannot start!\r\n");

		return;	// TODO: assess - do action here?
	}

	JumpToUserApplication(fwStartAddr);
}


// Should be called immediately after boot.
void CheckResetCause(void)
{
   bool switchImgNum = false;

   if ( ResetBySW() )
   {
      switchImgNum = RunFwUpdate(false);
   }
   // Set ImgNum to current(=false) or new(=true) FW:
   SetBootImage(switchImgNum);

   StartFirmware();   // Start application.
}


uint32_t GetBootImage(void)
{
	return *(volatile uint32_t *)(CONFIG_START);
}


void SetBootImage(bool switchImage)
{
	gImgNum = (uint8_t)GetBootImage();	// From Flash.

   // Illegal image-number?
   if (gImgNum == 0 || gImgNum > 2)
	   gImgNum = 1;

   // Set StartAddr - TODO: check if ==1||==2 first!
   if ( switchImage )
   {
	   // imgNum = (imgNum & 0x03) ^ 0x03;   // XOR 2LSBs
	   // TODO: simplify!
	   if (gImgNum==1)
		   gImgNum = 2;
	   else
		   gImgNum = 1;
   }

   // Update Flash
   WriteFlashByte(CONFIG_START, gImgNum);

   // Informative only:
   uint32_t gFwStartAddr = FW_OFFSET + (gImgNum - 1)*FW_SZ;

   PRINTF("Image addr set to: 0x%08X", gFwStartAddr);
}





