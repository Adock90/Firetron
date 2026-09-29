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

EFI_STATUS write_file(EFI_FILE_HANDLE file_handle, const CHAR16* buffer);

EFI_STATUS append_file(EFI_FILE_HANDLE file_handle, const CHAR16* string);

EFI_FILE_HANDLE create_file(const CHAR16* filename, EFI_FILE_HANDLE volume);

EFI_STATUS delete_file(EFI_FILE_HANDLE file_handle);

EFI_STATUS close_file(EFI_FILE_HANDLE file_handle);

#endif
