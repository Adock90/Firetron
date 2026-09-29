/*This is the main header of Firetron's bootloader
which includes gnu-efi headers so we can access them globally.
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#ifndef EFI_H
#define EFI_H

#include <efi.h>
#include <efilib.h>

#define MAX_PATH 4096

extern EFI_HANDLE loaded_img;


void unicode_str_to_ascii_str(const CHAR16* src, CHAR8* dst);

void ascii_str_to_unicode_str(const CHAR8* src, CHAR16* dst);

#endif
