#ifndef _ELF_H
#define _ELF_H

typedef unsigned char Elf_Byte;

typedef unsigned int Elf32_Addr;
typedef unsigned int Elf32_Off;
typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Word;
typedef int Elf32_SWord;
typedef unsigned char Elf32_Byte;

typedef unsigned long long int Elf64_Addr;
typedef unsigned long long int Elf64_Off;
typedef unsigned short Elf64_Half;
typedef unsigned int Elf64_Word;
typedef int Elf64_SWord;
typedef unsigned long long int Elf64_Xword;
typedef long long int Elf64_Sxword;
typedef unsigned char Elf64_Byte;

#ifndef NULL
#if defined (__null)
#define NULL __null
#else 
#define NULL 0
#endif
#endif

#define EI_NIDENT 16

// type field
#define ET_NONE 0
#define ET_REL 1
#define ET_EXEC 2
#define ET_DYN 3
#define ET_CORE 4
#define ET_LOOS 0xfe00
#define ET_HIOS 0xfeff
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

// machine field
#define EM_386 3
#define EM_860 7
#define EM_ARM 40
#define EM_X86_64 62
#define EM_XTENSA 94
#define EM_AARCH64 183

// versions
#define EV_NONE 0
#define EV_CURRENT 1

// identifier indexes
#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6 // version
#define EI_OSABI 7
#define EI_ABIVERSION 8
#define EI_PAD 9 // start of padding bytes
#define EI_NIDENT 16

#define ELFMAG0 0x7f
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
// EI_CLASS
#define ELFCLASSNONE 0 // invliad
#define ELFCLASS32 1
#define ELFCLASS64 2
// EI_DATA
#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define ELFOSABI_NONE 0
#define ELFOSABI_HPUX 1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_GNU 3
#define ELFOSABI_LINUX 3
#define ELFOSABI_FREEBSD 9
#define ELFOSABI_OPENBSD 12

/////////////////
// ELF headers //
/////////////////
typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        Elf32_Half      e_type;
        Elf32_Half      e_machine;
        Elf32_Word      e_version;
        Elf32_Addr      e_entry;
        Elf32_Off       e_phoff;
        Elf32_Off       e_shoff;
        Elf32_Word      e_flags;
        Elf32_Half      e_ehsize;
        Elf32_Half      e_phentsize;
        Elf32_Half      e_phnum;
        Elf32_Half      e_shentsize;
        Elf32_Half      e_shnum;
        Elf32_Half      e_shstrndx;
} Elf32_Ehdr; 

typedef struct {
        unsigned char   e_ident[EI_NIDENT];
        Elf64_Half      e_type;
        Elf64_Half      e_machine;
        Elf64_Word      e_version;
        Elf64_Addr      e_entry;
        Elf64_Off       e_phoff;
        Elf64_Off       e_shoff;
        Elf64_Word      e_flags;
        Elf64_Half      e_ehsize;
        Elf64_Half      e_phentsize;
        Elf64_Half      e_phnum;
        Elf64_Half      e_shentsize;
        Elf64_Half      e_shnum;
        Elf64_Half      e_shstrndx;
} Elf64_Ehdr; // 64 bit header

// section indexes
#define SHN_UNDEF 0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC 0xff00
#define SHN_HIPROC 0xff1f
#define SHN_LOOS 0xff20
#define SHN_HIOS 0xff3f
#define SHN_ABS 0xfff1
#define SHN_COMMON 0xfff2
#define SHN_XINDEX 0xffff
#define SHN_HIRESERVE 0xffff

// scetion types
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
#define SHT_INIT_ARRAY 14
#define SHT_FINI_ARRAY 15
#define SHT_PREINIT_ARRAY 16
#define SHT_GROUP 17
#define SHT_SYMTAB_SHNDX 18
#define SHT_LOOS 0x60000000
#define SHT_HIOS 0x6fffffff
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7fffffff
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xffffffff

// flag masks
#define SHF_WRITE 0x1
#define SHF_ALLOC 0x2
#define SHF_EXECINSTR 0x4
#define SHF_MERGE 0x10
#define SHF_STRINGS 0x20
#define SHF_INFO_LINK 0x40
#define SHF_LINK_ORDER 0x80
#define SHF_OS_NONCONFORMING 0x100
#define SHF_GROUP 0x200
#define SHF_TLS 0x400
#define SHF_COMPRESSED 0x800
#define SHF_MASKOS 0x0ff00000
#define SHF_MASKPROC 0xf0000000

//////////////////
// ELF sections //
//////////////////
typedef struct {
	Elf32_Word	sh_name;
	Elf32_Word	sh_type;
	Elf32_Word	sh_flags;
	Elf32_Addr	sh_addr;
	Elf32_Off	sh_offset;
	Elf32_Word	sh_size;
	Elf32_Word	sh_link;
	Elf32_Word	sh_info;
	Elf32_Word	sh_addralign;
	Elf32_Word	sh_entsize;
} Elf32_Shdr;

typedef struct {
	Elf64_Word	sh_name;
	Elf64_Word	sh_type;
	Elf64_Xword	sh_flags;
	Elf64_Addr	sh_addr;
	Elf64_Off	sh_offset;
	Elf64_Xword	sh_size;
	Elf64_Word	sh_link;
	Elf64_Word	sh_info;
	Elf64_Xword	sh_addralign;
	Elf64_Xword	sh_entsize;
} Elf64_Shdr; 

Elf_Byte elf_check(Elf_Byte *buf);

Elf_Byte elf_get_class(Elf_Byte *buf);

Elf_Byte elf_get_data(Elf_Byte *buf);

Elf32_Ehdr *elf_get_header32(Elf_Byte *buf);

Elf64_Ehdr *elf_get_header64(Elf_Byte *buf);

Elf32_Shdr *elf_get_section_n_header32(Elf32_Ehdr *header, Elf32_Half n);

Elf64_Shdr *elf_get_section_n_header64(Elf64_Ehdr *header, Elf64_Half n);

void *elf_get_section32(Elf_Byte *buf, Elf32_Shdr *header);

void *elf_get_section64(Elf_Byte *buf, Elf64_Shdr *header);

const char *elf_section_name32(Elf32_Ehdr *header, Elf32_Word index);

const char *elf_section_name64(Elf64_Ehdr *header, Elf64_Word index);

void elf_print_section_flags(int flags);

#endif
