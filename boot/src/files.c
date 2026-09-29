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
		return NULL;
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
        return NULL;
    }

	status = uefi_call_wrapper(io_volume->OpenVolume,
			2,
			io_volume,
			&volume
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to open file system volume at OpenVolume. EFI_STATUS: %d", status);
		return NULL;
	}

	return volume;
}	


EFI_FILE_HANDLE open_file(const CHAR16* filename, EFI_FILE_HANDLE volume)
{
	EFI_FILE_HANDLE file_handle;
	EFI_STATUS status = uefi_call_wrapper(
		volume->Open,
		5,
		volume,
		&file_handle,
		filename,
		EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE,
		EFI_FILE_HIDDEN | EFI_FILE_SYSTEM
	);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to open file: %s. EFI_STATUS: %d", filename, status);
		return NULL;
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
	
	UINT8* buffer = AllocatePool(file_size+1);

	EFI_STATUS status = uefi_call_wrapper(file_handle->Read,
			3,
			file_handle,
			&file_size,
			buffer
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to read file: %s. EFI_STATUS: %d", file_info->FileName, status);
		return NULL;
	}

	buffer[file_size+1] = '\0';

	return buffer;
}

EFI_STATUS write_file(EFI_FILE_HANDLE file_handle, const CHAR16* buffer)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);
	UINTN unicode_buffer_size = StrLen(buffer);
	if (unicode_buffer_size < 1)
	{
		out_error(L"Buffer too small. Buffer size: %d", unicode_buffer_size);
		return EFI_LOAD_ERROR;
	}

	UINT8* ascii_buffer = AllocatePool(unicode_buffer_size+1);
	unicode_str_to_ascii_str(buffer, ascii_buffer);
	UINTN ascii_buffer_size = AsciiStrLen((CHAR8*)ascii_buffer);

	EFI_STATUS status = uefi_call_wrapper(file_handle->Write,
		3,
		file_handle,
		&ascii_buffer_size,
		(void*)ascii_buffer
	);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to write to file: %s. EFI_STATUS: %d", file_info->FileName, status);
		return status;
	}

	FreePool(ascii_buffer);
	FreePool(file_info);

	return status;
}

EFI_STATUS append_file(EFI_FILE_HANDLE file_handle, const CHAR16* string)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);
	UINT8* file_contents = read_file(file_handle);
	if (file_contents == NULL)
		return EFI_OUT_OF_RESOURCES;

	UINTN ascii_len = AsciiStrLen((CHAR8*)file_contents);

	UINTN new_len = ascii_len + StrLen(string) + 1;
	
	UINT16* unicode_file_contents = AllocatePool(new_len *sizeof(CHAR16));
	if (unicode_file_contents == NULL)
	{
		FreePool(file_contents);
		return EFI_OUT_OF_RESOURCES;
	}

	ascii_str_to_unicode_str((CHAR8*)file_contents, unicode_file_contents);

	StrCat(unicode_file_contents, string);
	EFI_STATUS status = write_file(file_handle, string);
	if (EFI_ERROR(status))
	{
		out_error(L"Unable to append data to: %s. EFI_STATUS: %d", file_info->FileName, status);
		FreePool(file_contents);
		return status;
	}

	FreePool(unicode_file_contents);
	FreePool(file_contents);
	return EFI_SUCCESS;
}

EFI_FILE_HANDLE create_file(const CHAR16* filename, EFI_FILE_HANDLE volume)
{
	EFI_FILE_HANDLE file_handle;
	
	EFI_STATUS status = uefi_call_wrapper(volume->Open, 5, volume,
		&file_handle,
		filename,
		EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE,
		EFI_FILE_HIDDEN | EFI_FILE_SYSTEM
	);
	if (EFI_ERROR(status) || file_handle == NULL)
	{
		out_error(L"Failed to create file: %s. EFI_STATUS: %d", filename, status);
		return NULL;
	}
	
	return file_handle;
}

EFI_STATUS delete_file(EFI_FILE_HANDLE file_handle)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);

	EFI_STATUS status = uefi_call_wrapper(file_handle->Delete, 1, file_handle);
	if (EFI_ERROR(status))
		out_error(L"Failed to delete file: %s. EFI_STATUS: %d", file_info->FileName, status);
	
	return status;
}

EFI_STATUS close_file(EFI_FILE_HANDLE file_handle)
{
	EFI_FILE_INFO* file_info = LibFileInfo(file_handle);
	
	EFI_STATUS status = uefi_call_wrapper(file_handle->Close,
		1,
		file_handle
		);
	if (EFI_ERROR(status))
		out_error(L"Failed to close file: %s. EFI_STATUS: %d", file_info->FileName, status);
	
	return status;
}