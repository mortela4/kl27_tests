/* Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "fsl_shell.h"
#include "fsl_debug_console.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "flash_cmds.h"
#include "fw_boot.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BL_VERSION_STRING	"0.1.1"

#define LED_NUMBERS 3U
#define LED_1_INIT() LED_RED_INIT(LOGIC_LED_OFF)
#define LED_2_INIT() LED_GREEN_INIT(LOGIC_LED_OFF)
#define LED_3_INIT() LED_BLUE_INIT(LOGIC_LED_OFF)
#define LED_1_ON() LED_RED_ON()
#define LED_1_OFF() LED_RED_OFF()
#define LED_2_ON() LED_GREEN_ON()
#define LED_2_OFF() LED_GREEN_OFF()
#define LED_3_ON() LED_BLUE_ON()
#define LED_3_OFF() LED_BLUE_OFF()
#define SHELL_Printf PRINTF
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void Led_Init(void);

/* SHELL user send data callback */
void SHELL_SendDataCallback(uint8_t *buf, uint32_t len);

/* SHELL user receive data callback */
void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len);

static int32_t LedControl(p_shell_context_t context, int32_t argc, char **argv);
static int32_t FlashWriteCmd(p_shell_context_t context, int32_t argc, char **argv);
static int32_t FlashReadCmd(p_shell_context_t context, int32_t argc, char **argv);
static int32_t BootAppCmd(p_shell_context_t context, int32_t argc, char **argv);
static int32_t UpdateCmd(p_shell_context_t context, int32_t argc, char **argv);


/*******************************************************************************
 * Variables
 ******************************************************************************/
static const shell_command_context_t xLedCommand = {"led",
                                                    "\r\n\"led arg1 arg2\":\r\n Usage:\r\n    arg1: 1|2|3|4...         "
                                                    "   Led index\r\n    arg2: on|off                Led status\r\n",
                                                    LedControl, 2};
static const shell_command_context_t xFlashReadCommand = {"fread",
                                                    "\r\nfread <addr>\r\n",
													FlashReadCmd, 1};
static const shell_command_context_t xFlashWriteCommand = {"fwrite",
                                                    "\r\nfwrite <addr> <val>\r\n",
													FlashWriteCmd, 2};
static const shell_command_context_t xAppStartCommand = {"appexec",
                                                    "\r\nappexec <appStartAddr>\r\n",
													BootAppCmd, 1};
static const shell_command_context_t xUpdateCommand = {"update",
                                                    "\r\nupdate <autostart=0|1> <emulatefail=0|1>\r\n",
													UpdateCmd, 2};


/*******************************************************************************
 * Code
 ******************************************************************************/

void Led_Init(void)
{
    LED_1_INIT();
    LED_2_INIT();
    LED_3_INIT();
}

void SHELL_SendDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        PUTCHAR(*(buf++));
    }
}

void SHELL_ReceiveDataCallback(uint8_t *buf, uint32_t len)
{
    while (len--)
    {
        *(buf++) = GETCHAR();
    }
}

static int32_t LedControl(p_shell_context_t context, int32_t argc, char **argv)
{
    int32_t kLedIndex = ((int32_t)atoi(argv[1]));
    char *kLedCommand = argv[2];

    /* Check second argument to control led */
    switch (kLedIndex)
    {
#if defined(LED_NUMBERS) && LED_NUMBERS > 0
        case 1:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_1_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_1_OFF();
            }
            else
            {
                SHELL_Printf("Control cmd is wrong!\r\n");
            }
            break;
#endif
#if defined(LED_NUMBERS) && LED_NUMBERS > 1
        case 2:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_2_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_2_OFF();
            }
            else
            {
                SHELL_Printf("Control cmd is wrong!\r\n");
            }
            break;
#endif
#if defined(LED_NUMBERS) && LED_NUMBERS > 2
        case 3:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_3_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_3_OFF();
            }
            else
            {
                SHELL_Printf("Control cmd is wrong!\r\n");
            }
            break;
#endif
#if defined(LED_NUMBERS) && LED_NUMBERS > 3
        case 4:
            if (strcmp(kLedCommand, "on") == 0)
            {
                LED_4_ON();
            }
            else if (strcmp(kLedCommand, "off") == 0)
            {
                LED_4_OFF();
            }
            else
            {
                SHELL_Printf("Control cmd is wrong!\r\n");
            }
            break;
#endif
        default:
            SHELL_Printf("LED index is wrong\r\n");
            break;
    }
    return 0;
}


static int32_t FlashWriteCmd(p_shell_context_t context, int32_t argc, char **argv)
{
    uint32_t addr = strtoul(argv[1], NULL, 16);
    uint32_t val = strtoul(argv[2], NULL, 16);

    SHELL_Printf("Writing 0x%08X to address 0x%08X\r\n", val, addr);

    return WriteFlashByte(addr, (uint8_t)val);;
}


static int32_t FlashReadCmd(p_shell_context_t context, int32_t argc, char **argv)
{
	uint8_t val = 0;
    uint32_t addr = strtoul(argv[1], NULL, 16);

    int32_t status = ReadFlashByte(addr, &val);

    SHELL_Printf("Value from address 0x%08X = 0x%08X\r\n", addr, val);

    return status;
}


static int32_t BootAppCmd(p_shell_context_t context, int32_t argc, char **argv)
{
	uint32_t appStartAddr = strtoul(argv[1], NULL, 16);

	if ( CheckApplicationReady(appStartAddr) )
	{
		SHELL_Printf("Starting application at addr=0x%08X ...\r\n", appStartAddr);
		JumpToUserApplication(appStartAddr);	// Bye, bye ...
	}
	else
	{
		SHELL_Printf("No valid application at addr=0x%08X\r\nCannot start!\r\n", appStartAddr);
	}

	return 0;	// Pointless ...
}


static int32_t UpdateCmd(p_shell_context_t context, int32_t argc, char **argv)
{
	bool switchImgNum = false;
	int32_t autoStart = 1;
	bool emulateFailure = false;

	if ( argc > 1 )
		autoStart = ((int32_t)atoi(argv[1]));

	if ( argc > 2 )
	{
		emulateFailure = ( atoi(argv[2]) != 0 );
	}

   switchImgNum = RunFwUpdate(emulateFailure);

   // Set ImgNum to current(=false) or new(=true) FW:
   SetBootImage(switchImgNum);

   if ( autoStart )
	   StartFirmware();

   return 0;
}


/*! @brief Main function */
int main(void)
{
    shell_context_struct user_context;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Init Flash - yeah, right ...;-) */
    InitFlash();

    /* Init led (explanatory-comment ...) */
    Led_Init();

    /* Init SHELL */
    SHELL_Init(&user_context, SHELL_SendDataCallback, SHELL_ReceiveDataCallback, SHELL_Printf, "SHELL>> ");

    /* Add 'led' command to commands list */
    SHELL_RegisterCommand(&xLedCommand);
    /* Add 'fread' command to commands list */
    SHELL_RegisterCommand(&xFlashReadCommand);
    /* Add 'fwrite' command to commands list */
    SHELL_RegisterCommand(&xFlashWriteCommand);
    /* Add 'appexec' command */
    SHELL_RegisterCommand(&xAppStartCommand);
    /* Add 'update' command */
    SHELL_RegisterCommand(&xUpdateCommand);


    SHELL_Main(&user_context);

    while (1)
    {
    }
}
