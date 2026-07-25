/*This is the file handler of Firetron's bootloader
which allows us to do file operations on files.
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "files.h"

EFI_FILE_HANDLE get_volume(EFI_HANDLE img)
{
	EFI_LOADED_IMAGE* loaded_img;
	EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* io_volume;
	EFI_GUID fs_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;

	EFI_FILE_HANDLE volume;

	EFI_STATUS status = uefi_call_wrapper(BS->HandleProtocol,
			3,
			img,
			&lip_guid,
			(void **)&loaded_img
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to get file system volume at BS->HandleProtocol. EFI_STATUS: %d", status);
		Exit(EFI_LOAD_ERROR, 0, NULL);
	}

	status = uefi_call_wrapper(BS->HandleProtocol,
			3,
			loaded_img->DeviceHandle,
			&fs_guid,
			(void*)&io_volume
			);
	if (EFI_ERROR(status))
        {
                out_error(L"Failed to get file system volume at BS->HandleProtocol. EFI_STATUS: %d", status);
                Exit(EFI_LOAD_ERROR, 0, NULL);
        }

	status = uefi_call_wrapper(io_volume->OpenVolume,
			2,
			io_volume,
			&volume
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to open file system volume at OpenVolume. EFI_STATUS: %d", status);
		Exit(EFI_LOAD_ERROR, 0, NULL);
	}

	return volume;
}	

EFI_FILE_HANDLE open_file(const CHAR16* filename, EFI_FILE_HANDLE volume)
{
	EFI_FILE_HANDLE file_handle;
	EFI_STATUS status = uefi_call_wrapper(volume->Open,
			5,
			volume,
			&file_handle,
			filename,
			EFI_FILE_MODE_READ,
			EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to open file: %s. EFI_STATUS: %d", filename, status);
		Exit(EFI_LOAD_ERROR, 0, NULL);
	}

	return file_handle;
}

UINT64 get_file_size(EFI_FILE_HANDLE file_handle)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);
	UINT64 file_size = file_info->FileSize;
	FreePool(file_info);
	return file_size;
}

UINT8* read_file(EFI_FILE_HANDLE file_handle)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);
	UINT64 file_size = get_file_size(file_handle);
	if (file_size < 1)
	{
		out_error(L"File size is too low: %s", file_info->FileName);
		Exit(EFI_LOAD_ERROR, 0, NULL);
	}
	
	UINT8* buffer = AllocatePool(file_size);

	EFI_STATUS status = uefi_call_wrapper(file_handle->Read,
			3,
			file_handle,
			&file_size,
			buffer
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to read file: %s. EFI_STATUS: %d", file_info->FileName, status);
		Exit(EFI_LOAD_ERROR, 0, NULL);
	}

	return buffer;
}

void close_file(EFI_FILE_HANDLE file_handle)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);
	
	EFI_STATUS status = uefi_call_wrapper(file_handle->Close,
		1,
		file_handle
		);
	if (EFI_ERROR(status))
		out_error(L"Failed to close file: %s. EFI_STATUS: %d", file_info->FileName, status);
}
