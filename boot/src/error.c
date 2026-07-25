/*This is the error output handler of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "error.h"

void out_error(const CHAR16* msg, ...)
{
	EFI_TIME* time;

	//Allocates memory for 'EFI_TIME' struct
	EFI_STATUS status = uefi_call_wrapper(BS->AllocatePool, 
			3, 
			EfiBootServicesData,
			sizeof(EFI_TIME),
			(VOID**)&time
			);
	if (EFI_ERROR(status))
	{
		Print(L"[Failed to get time. AllocatePool. EFI_STATUS: %d] ", status);
	}
	else
	{
		EFI_RUNTIME_SERVICES* rt = ST->RuntimeServices;
		status = uefi_call_wrapper(rt->GetTime, 2, time, NULL);//gets time
		if (EFI_ERROR(status))
		{
			Print(L"[Failed to get time. GetTime] ");
		}
		else
		{	
			Print(L"[%02d:%02d:%02d] ", time->Hour, time->Minute, time->Second);
		}

		status = uefi_call_wrapper(BS->FreePool, 1, time);
		if (EFI_ERROR(status))
	        {
        	        Print(L"Failed to free time. EFI_STATUS: %d", status);
        	}

	}

	//gets the va args passed in function
	va_list list;
    va_start(list, msg);
	VPrint(msg, list);
	Print(L"\r\n");
	va_end(list);
}	
