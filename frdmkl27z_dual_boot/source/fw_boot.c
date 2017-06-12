/*
 * fw_boot.c
 *
 *  Created on: 7. jun. 2017
 *      Author: morte
 */

//#include "mcu.h"

#include "board.h"

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

int CheckApplicationReady(unsigned long reloadVector)
{
	unsigned long *p = (unsigned long *)reloadVector;
	if(*(p + 8) != 0xffffffff)
	{
		return 1;
	}
	return 0;
}


