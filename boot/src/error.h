/*This is the error msg handler header of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/
#ifndef ERROR_H
#define ERROR_H

#include "fireefi.h"
#include "files.h"

#define ERROR_LOG_FILE_PATH L"FBEL.log"

void out_error(const CHAR16* msg, ...);

void out_log(const CHAR16* msg, ...);

EFI_STATUS refresh_error_log_file();

void reboot_system_for_error();

#endif
