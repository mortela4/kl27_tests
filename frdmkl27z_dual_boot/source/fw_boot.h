/*
 * fw_boot.h
 *
 *  Created on: 7. jun. 2017
 *      Author: morte
 */

#ifndef FW_BOOT_H_
#define FW_BOOT_H_

void JumpToUserApplication(unsigned long reloadVector);
int CheckApplicationReady(unsigned long reloadVector);

#endif /* FW_BOOT_H_ */
