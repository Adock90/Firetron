/*This is the ELF loader of Firetron's bootloader
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#include "elf.h"

/*We use this function to make sure we are loading a compatible executable*/
EFI_STATUS verify_elf_file(elf64_header* header)
{
	//This is the magic signature for all ELF files
	unsigned char magic_array[MAGIC_LENGTH] = {0x7f, 'E', 'L', 'F'};

	for (int i = 0; i < MAGIC_LENGTH; i++)
    {
        if (magic_array[i] != header->e_ident[i])
			return EFI_LOAD_ERROR;
    }
	
	//Checking that the ELF's target arch(e_machine) is compatible
	if (header->e_machine != EM_X86_64)
		return EFI_LOAD_ERROR;
	
	//Checking if the ELF is 64 bit
	if (header->e_ident[EI_CLASS] != ELFCLASS64)
		return EFI_LOAD_ERROR;

	return EFI_SUCCESS;
}


EFI_STATUS load_elf_file(UINT8* buffer, void** entry)
{
	elf64_header* header = (elf64_header*)buffer;
	elf64_program_header* program_header = (elf64_program_header*)(buffer + header->e_phoff);
	UINT64 max_align = MAX_ALIGN;
	UINT64 mem_start = UINT64_MAX;
	UINT64 mem_end = MEM_END;
	UINT32 required_mem_size = 0;
	void* program_memory_buffer = NULL;

	if (EFI_ERROR(verify_elf_file(header)))
	{
		out_error(L"Not a Compatible ELF File.");
		return EFI_LOAD_ERROR;
	}

	/*Checking the ELF is PIE (Position Independent Executable).
	This means the kernel code can be placed in different locations.*/
	if (header->e_type != ET_DYN)
        return EFI_LOAD_ERROR;

	
	//Evalutating bounds for all sections
	for (int i = 0; i < header->e_phnum; i++, program_header++)
	{
		if (program_header->p_type == PT_LOAD)
		{
			if (program_header->p_align > max_align)
				max_align = program_header->p_align;
	
			UINT64 seg_mem_begin = program_header->p_vaddr;
			UINT64 seg_mem_end = program_header->p_vaddr + program_header->p_memsz + max_align - 1;

			seg_mem_begin &= ~(max_align - 1);
			seg_mem_end &= ~(max_align - 1);

			if (seg_mem_begin < mem_start)
				mem_start = seg_mem_begin;

			if (seg_mem_end > mem_end)
				mem_end = seg_mem_end;

		}
	}

	required_mem_size = mem_end - mem_start;

	//Allocates the right size for program headers as calculated in 'required_mem_size'
	EFI_STATUS status = uefi_call_wrapper(BS->AllocatePool,
			3,
			EfiLoaderData,
			required_mem_size,
			&program_memory_buffer
			);
	if (EFI_ERROR(status))
	{
		out_error(L"Failed to allocate memory for program sections. EFI_STATUS: %d", status);
		return status;
	}

	//loading sections into RAM
	program_header = (elf64_program_header*)(buffer + header->e_phoff);
	for (int j = 0; j < header->e_phnum; j++, program_header++)
	{
		if (program_header->p_type == PT_LOAD)
		{
			UINT64 relative_offset = program_header->p_vaddr - mem_start;
			UINT8* destination = (UINT8*)program_memory_buffer + relative_offset;
			UINT8* source = (UINT8*)buffer + program_header->p_offset;
			UINT32 length = program_header->p_filesz;

			status = uefi_call_wrapper(BS->CopyMem, 
					3, 
					destination, 
					source, 
					length
					);
			if (EFI_ERROR(status))
			{
				out_error(L"Failed to CopyMemory. EFI_STATUS: %d", status);
				return status;
			}
		}
	}

	//updates entry variable to entry point of ELF.
	*entry = (void*)((UINT8*)program_memory_buffer + (header->e_entry - mem_start));

	return EFI_SUCCESS;
}
