/*
 * fw_boot.h
 *
 *  Created on: 7. jun. 2017
 *      Author: morte
 */

#ifndef FW_BOOT_H_
#define FW_BOOT_H_

void JumpToUserApplication(unsigned long reloadVector);
bool CheckApplicationReady(unsigned long reloadVector);
void SetBootImage(bool switchImage);
void CheckResetCause(void);
bool ResetBySW(void);
bool RunFwUpdate(bool emulateFailure);
void StartFirmware(void);
uint32_t GetBootImage(void);
void SetBootImage(bool switchImage);

#endif /* FW_BOOT_H_ */
