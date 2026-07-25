/*This is the file handler header of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#ifndef FILES_H
#define FILES_H

#include "fireefi.h"
#include "error.h"

EFI_FILE_HANDLE get_volume(EFI_HANDLE img);

EFI_FILE_HANDLE open_file(const CHAR16* filename, EFI_FILE_HANDLE volume);

UINT64 get_file_size(EFI_FILE_HANDLE file_handle);

UINT8* read_file(EFI_FILE_HANDLE file_handle);

void close_file(EFI_FILE_HANDLE file_handle);

#endif
