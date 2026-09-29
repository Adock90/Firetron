/*This is the error output handler of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "error.h"

void out_error(const CHAR16* msg, ...)
{

	EFI_FILE_HANDLE file_volume = get_volume(loaded_img);
	if (file_volume == NULL)
	{
		out_error(L"Failed to get volume");
		goto end_log_fun;
	}

	EFI_FILE_HANDLE log_file_handle = open_file(ERROR_LOG_FILE_PATH, file_volume);
	if (log_file_handle == NULL)
	{
		out_error(L"Failed to get a valid log file handle");
		goto end_log_fun;
	}


	UINT16 log_buffer[MAX_PATH];
	UINT16 time_buffer[44];
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
			SPrint(time_buffer, sizeof(time_buffer), L"[ERR][%02d:%02d:%02d] ", time->Hour, time->Minute, time->Second);
			append_file(log_file_handle, time_buffer);
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
	VSPrint(log_buffer, sizeof(log_buffer), msg, list);
	append_file(log_file_handle, log_buffer);
	va_end(list);
end_log_fun_file:
	close_file(log_file_handle);
end_log_fun:
}

void out_log(const CHAR16* msg, ...)
{

	EFI_FILE_HANDLE file_volume = get_volume(loaded_img);
	if (file_volume == NULL)
	{
		out_error(L"Failed to get volume");
		goto end_log_fun;
	}

	EFI_FILE_HANDLE log_file_handle = open_file(ERROR_LOG_FILE_PATH, file_volume);
	if (log_file_handle == NULL)
	{
		out_error(L"Failed to get a valid log file handle");
		goto end_log_fun;
	}


	UINT16 log_buffer[MAX_PATH];
	UINT16 time_buffer[44];
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
		out_error(L"[Failed to get time. AllocatePool. EFI_STATUS: %d] ", status);
	}
	else
	{
		EFI_RUNTIME_SERVICES* rt = ST->RuntimeServices;
		status = uefi_call_wrapper(rt->GetTime, 2, time, NULL);//gets time
		if (EFI_ERROR(status))
		{
			out_error(L"[Failed to get time. GetTime] ");
		}
		else
		{	
			SPrint(time_buffer, sizeof(time_buffer), L"[LOG][%02d:%02d:%02d] ", time->Hour, time->Minute, time->Second);
			append_file(log_file_handle, time_buffer);
		}

		status = uefi_call_wrapper(BS->FreePool, 1, time);
		if (EFI_ERROR(status))
	    {
        	out_error(L"Failed to free time. EFI_STATUS: %d", status);
        }
	}

	//gets the va args passed in function
	va_list list;
    va_start(list, msg);
	VSPrint(log_buffer, sizeof(log_buffer), msg, list);
	append_file(log_file_handle, log_buffer);
	va_end(list);
end_log_fun_file:
	close_file(log_file_handle);
end_log_fun:
}

void reboot_system_for_error()
{
	UINTN time_to_wait = 10000000;
	Print(L"Your Computer Will reboot shortly (Around 10 seconds)\r");
	
	EFI_STATUS status = uefi_call_wrapper(BS->Stall, 1, time_to_wait);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to stall time. EFI_STATUS: %d", status);
	}

	status = ST->RuntimeServices->ResetSystem(EfiResetCold, EFI_LOAD_ERROR, 0, NULL);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to reboot machine. EFI_STATUS: %d", status);
		Exit(EFI_LOAD_ERROR, 0, NULL);
	}
}

EFI_STATUS refresh_error_log_file()
{
	EFI_HANDLE volume = get_volume(loaded_img);
	if (volume == NULL)
	{
		out_error(L"Failed to get volume");
		return EFI_LOAD_ERROR;
	}

	EFI_FILE_HANDLE del_error_log_file = create_file(ERROR_LOG_FILE_PATH, volume);
	if (del_error_log_file == NULL)
	{
		del_error_log_file = open_file(ERROR_LOG_FILE_PATH, volume);
		if (del_error_log_file == NULL)
		{
			out_error(L"Failed to retrieve a valid handle error log for deletion: %s.", ERROR_LOG_FILE_PATH);
			return EFI_LOAD_ERROR;
		}
	}

	EFI_STATUS status = delete_file(del_error_log_file);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to delete error log file: %s. EFI_STATUS: %d", ERROR_LOG_FILE_PATH, status);
		return status;
	}

	EFI_FILE_HANDLE create_log_file = create_file(ERROR_LOG_FILE_PATH, volume);
	if (create_log_file == NULL)
	{
		out_error(L"Failed to create error log file: %s. EFI_STATUS: %d", ERROR_LOG_FILE_PATH, status);
		return status;
	}

	status = close_file(create_log_file);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to close error log file: %s. EFI_STATUS: %d", ERROR_LOG_FILE_PATH, status);
		return status;
	}

	//Exit(EFI_LOAD_ERROR, 0, NULL);

	return status;
}