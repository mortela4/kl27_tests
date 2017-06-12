/*
 * flash_cmds.h
 *
 *  Created on: 6. jun. 2017
 *      Author: morte
 */

#ifndef FLASH_CMDS_H_
#define FLASH_CMDS_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOOT_SZ			(24*1024)			// 0x6000
#define CONFIG_SZ		0x400				// =SECTOR_SZ=1KB
#define CONFIG_START	(BOOT_SZ-CONFIG_SZ)
//
#define FW_SZ			(20*1024)			// 0x5000
#define FW1_START		BOOT_SZ
#define FW2_START		(FW1_START+FW_SZ)	// 0xB000


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);
void app_finalize(void);
//
void InitFlash(void);
int WriteFlashByte(uint32_t addr, uint8_t inData);
int ReadFlashByte(uint32_t addr, uint8_t *outData);

#endif /* FLASH_CMDS_H_ */
