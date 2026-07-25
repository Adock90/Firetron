/*This is the graphics header of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "fireefi.h"
#include "error.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL* get_gop();

#endif
