/*This is the memory handler header of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#ifndef MEMORY_H
#define MEMORY_H

#include "fireefi.h"
#include "error.h"

typedef struct
{
	EFI_MEMORY_DESCRIPTOR* map_descriptor;
	UINTN map_size;
	UINTN map_key;
	UINTN descriptor_size;
	UINT32 descriptor_version;
} memory_map;

EFI_STATUS get_memory_map(memory_map* mm);

#endif
