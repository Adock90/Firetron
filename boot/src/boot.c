/*This is the main implimention of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "boot.h"

EFI_STATUS load_krnl(const CHAR16* filename, EFI_HANDLE img, void** entry)
{
	EFI_FILE_HANDLE file_system_volume = get_volume(img);
	EFI_FILE_HANDLE file_handle = open_file(filename, file_system_volume);
	UINT64 file_size = get_file_size(file_handle);
	if (file_size < 4096)
	{
		out_error(L"File size is too small. Size: %d", file_size);
		close_file(file_handle);
		return EFI_LOAD_ERROR;
	}

	UINT8* krnl_file_buffer = read_file(file_handle);
	if (krnl_file_buffer == NULL)
	{
		out_error(L"Failed to return file contents. Filename: %s", filename);
		close_file(file_handle);
		return EFI_LOAD_ERROR;
	}
	
	EFI_STATUS status = load_elf_file(krnl_file_buffer, entry);
	if (status != EFI_SUCCESS)
	{
		out_error(L"Failed to load kernel file. Filename: %s. EFI_STATUS: %d", filename, status);
		close_file(file_handle);
		EFI_STATUS new_status = uefi_call_wrapper(BS->FreePool,
				1,
				krnl_file_buffer
				);
		if (EFI_ERROR(new_status))
		{
			out_error(L"Failed to Free kernel file buffer. EFI_STATUS: %d", new_status);
			//Note: Not returning FreePool status (new_status) so it doesn't overwrite original error msg
		}

		return status;
	}
	

	close_file(file_handle);
	status = uefi_call_wrapper(BS->FreePool,
				1,
				krnl_file_buffer
				);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to Free kernel file buffer. EFI_STATUS: %d", status);
		return status;
	}

	return status;
}

//efi main function

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable)
{
	InitializeLib(ImageHandle, SystemTable); //Has to be done in order to call UEFI functions
	
	const CHAR16* krnl_file_path = L"firestarter.elf"; //kernel init filename. (firestarter.elf)
	krnl_params kernel_parameters = {0};
	void* entry = NULL;
	

	EFI_STATUS status = load_krnl(krnl_file_path, ImageHandle, &entry);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to load ELF Kernel. Filename: %s. EFI_STATUS: %d", krnl_file_path, status);
		return status;
	}
	

	EFI_GRAPHICS_OUTPUT_PROTOCOL* graphics_protocol = get_gop();
	if (graphics_protocol == NULL)
	{
		out_error(L"Failed to get graphics output protocol");
		return status;
	}
	

	status = get_memory_map(&kernel_parameters.mm);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to get memory map. EFI_STATUS: %d", status);
		return status;
	}

	kernel_parameters.rt = SystemTable->RuntimeServices;
	kernel_parameters.gop = *graphics_protocol->Mode;


	status = uefi_call_wrapper(BS->ExitBootServices,
			2,
			ImageHandle,
			kernel_parameters.mm.map_key
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to Exit Boot Services. EFI_STATUS: %d", status);
		return status;
	}

	//kernel jump

	((krnl_entry)entry)(&kernel_parameters);
	
	//this shouldn't be executed.
	//Only here in case kernel jump fails.
	return status;
}
