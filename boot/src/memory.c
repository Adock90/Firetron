/*This is the memory handler of Firetron's bootloader
which allows us to get the memory map that we can pass to the
kernel.
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "memory.h"

EFI_STATUS get_memory_map(memory_map* map)
{
	EFI_STATUS status = uefi_call_wrapper(BS->GetMemoryMap,
			5,
			&map->map_size,
			map->map_descriptor,
			&map->map_key,
			&map->descriptor_size,
			&map->descriptor_version
			);
	if (EFI_ERROR(status) && status != EFI_BUFFER_TOO_SMALL)
	{
		out_error(L"Failed on GetMemoryMap. EFI_STATUS: %d", status);
		return status;
	}

	map->map_size += map->descriptor_size * 2;

	status = uefi_call_wrapper(BS->AllocatePool,
			3,
			EfiLoaderData,
			map->map_size,
			&map->map_descriptor
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed on AllocatePool. EFI_STATUS: %d", status);
		return status;
	}

	status = uefi_call_wrapper(BS->GetMemoryMap,
			5,
			&map->map_size,
			map->map_descriptor,
			&map->map_key,
			&map->descriptor_version,
			&map->descriptor_size
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to Get Memory Map. EFI_STATUS: %d", status);
		status = uefi_call_wrapper(BS->FreePool,
				1,
				map->map_descriptor
				);
		if (EFI_ERROR(status))
		{
			out_error(L"Failed to Free Pool. EFI_STATUS: %d", status);
			return status;
		}

		return status;
	}

	return status;
}
