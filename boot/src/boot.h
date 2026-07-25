/*This is the main header of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#ifndef BOOT_H
#define BOOT_H

//All the libaries/submodules included here
#include "error.h"
#include "memory.h"
#include "files.h"
#include "elf.h"
#include "graphics.h"

//kernel parameters
typedef struct
{
	memory_map mm;
	EFI_RUNTIME_SERVICES* rt;
	EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE gop;
} krnl_params;

//The 'krnl_entry' function is used to jump and pass parameters used by the kernel
typedef void (*krnl_entry)(krnl_params* params);//The declaration of the kernel_entry function

#endif
