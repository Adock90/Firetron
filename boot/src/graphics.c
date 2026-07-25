/*This is the graphics handler of Firetron's bootloader
which we pass to the kernel in kernel parameters
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/
#include "graphics.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL* get_gop()
{
	EFI_GUID guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

	EFI_STATUS status = uefi_call_wrapper(BS->LocateProtocol,
			3,
			&guid,
			NULL,
			(void**)&gop
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to get GOP. EFI_STATUS: %d", status);
		return NULL;
	}

	return gop;
}
