/*This is the error msg handler header of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/
#ifndef ERROR_H
#define ERROR_H

#include "fireefi.h"

void out_error(const CHAR16* msg, ...);

#endif
