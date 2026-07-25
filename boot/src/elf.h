/*This is the ELF Loader header of Firetron's bootloader 
which defines values and structs relevant to loading of 64 bit ELF files.
	Creator: Adam Croft @adock90
	SPDX-License-Identifier: GPL-2.0
*/

#ifndef ELF_H
#define ELF_H

#include "fireefi.h"
#include "error.h"

#define EI_NIDENT 16

#define MAGIC_LENGTH 4

#define EI_CLASS 4

#define MAX_ALIGN 4096
#define MEM_END 0

#define ELFCLASSNONE 0
#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EV_NONE 0
#define EV_CURRENT 1

#define ELFOSABI_NONE 0
#define ELFOSABI_SYSV 1
#define ELFOSABI_HPUX 2
#define ELFOSABI_NETBSD 3
#define ELFOSABI_LINUX 4
#define ELFOSABI_SOLARIS 5
#define ELFOSABI_IRIX 6
#define ELFOSABI_FREEBSD 7
#define ELFOSABI_TRU64 8
#define ELFOSABI_ARM 9
#define ELFOSABI_STANDALONE 10

#define EI_ABIVERSION 9
#define EI_PAD 10

#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

#define EM_NONE 0
#define EM_M32 1
#define EM_SPARC 2
#define EM_386 3
#define EM_68K 4
#define EM_88K 5
#define EM_860 7
#define EM_MIPS 8
#define EM_PARISC 15
#define EM_SPARC32PLUS 18
#define EM_PPC 20
#define EM_SH 42
#define EM_SPARCV9 43
#define EM_IA_64 50
#define EM_X86_64 62
#define EM_VAX 75

#define PN_XNUM 0xffff

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7fffffff

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_NUM 12
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff


typedef struct 
{
	unsigned char e_ident[EI_NIDENT];
	UINT16 e_type;
	UINT16 e_machine;
	UINT32 e_version;
	UINT64 e_entry;
	UINT64 e_phoff;
	UINT64 e_shoff;
	UINT32 e_flags;
	UINT16 e_ehsize;
	UINT16 e_phentsize;
	UINT16 e_phnum;
	UINT16 e_shentsize;
	UINT16 e_shnum;
	UINT16 e_shstrndx;
} elf64_header;

typedef struct
{
	UINT32 p_type;
	UINT32 p_flags;
	UINT64 p_offset;
	UINT64 p_vaddr;
	UINT64 p_paddr;
	UINT64 p_filesz;
	UINT64 p_memsz;
	UINT64 p_align;
} elf64_program_header;

typedef struct
{
	UINT32 sh_name;
	UINT32 sh_type;
	UINT64 sh_flags;
	UINT64 sh_addr;
	UINT64 sh_offset;
	UINT64 sh_size;
	UINT32 sh_link;
	UINT32 sh_info;
	UINT64 sh_addralign;
	UINT64 sh_entsize;
} elf64_section_header;

EFI_STATUS verify_elf_file(elf64_header* header);

EFI_STATUS load_elf_file(UINT8* buffer, void** entry);

#endif
