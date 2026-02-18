#include "elf.h"
#include <cstdio>

Elf_Byte elf_check(Elf_Byte *buf)
{
    return buf[EI_MAG0] == ELFMAG0 &&
        buf[EI_MAG1] == ELFMAG1 &&
        buf[EI_MAG2] == ELFMAG2 &&
        buf[EI_MAG3] == ELFMAG3;
}

Elf_Byte elf_get_class(Elf_Byte *buf)
{
    return buf[EI_CLASS];
}

Elf_Byte elf_get_data(Elf_Byte *buf)
{
    return buf[EI_DATA];
}

Elf32_Ehdr *elf_get_header32(Elf_Byte *buf)
{
    return (Elf32_Ehdr *)buf;
}

Elf64_Ehdr *elf_get_header64(Elf_Byte *buf)
{
    return (Elf64_Ehdr *)buf;
}

Elf32_Shdr *elf_get_section_n_header32(Elf32_Ehdr *header, Elf32_Half n)
{
    Elf32_Half size = header->e_shentsize;

    if(n >= header->e_shnum) return NULL;

    return (Elf32_Shdr *)(((Elf_Byte *)header) + header->e_shoff + n * size);
}

Elf64_Shdr *elf_get_section_n_header64(Elf64_Ehdr *header, Elf64_Half n)
{
    Elf64_Half size = header->e_shentsize;

    if(n >= header->e_shnum) return NULL;

    return (Elf64_Shdr *)(((Elf_Byte *)header) + header->e_shoff + n * size);
}

void *elf_get_section32(Elf_Byte *buf, Elf32_Shdr *header)
{
    return (void *)(buf + header->sh_offset);
}

void *elf_get_section64(Elf_Byte *buf, Elf64_Shdr *header)
{
    return (void *)(buf + header->sh_offset);
}

const char *elf_section_name32(Elf32_Ehdr *header, Elf32_Word index)
{
    Elf32_Half sect_idx = header->e_shstrndx;
    
    Elf32_Shdr *sect_hdr = elf_get_section_n_header32(header, sect_idx);

    void *section = elf_get_section32((Elf_Byte *)header, sect_hdr);

    return (((const char *)section) + index);
}

const char *elf_section_name64(Elf64_Ehdr *header, Elf64_Word index)
{
    Elf64_Half sect_idx = header->e_shstrndx;    
    
    Elf64_Shdr *sect_hdr = elf_get_section_n_header64(header, sect_idx);

    void *section = elf_get_section64((Elf_Byte *)header, sect_hdr);

    return (((const char *)section) + index);
}

void elf_print_section_flags(int flags)
{
    unsigned int flag_masks[13] = {
        SHF_WRITE,
        SHF_ALLOC,
        SHF_EXECINSTR,
        SHF_MERGE,
        SHF_STRINGS,
        SHF_INFO_LINK,
        SHF_LINK_ORDER,
        SHF_OS_NONCONFORMING,
        SHF_GROUP,
        SHF_TLS,
        SHF_COMPRESSED,
        SHF_MASKOS,
        SHF_MASKPROC
    };

    const char *flag_str[13] = {
        "WRITE",
        "ALLOC",
        "EXECINSTR",
        "MERGE",
        "STRINGS",
        "INFO_LINK",
        "LINK_ORDER",
        "OS_NONCONFORMING",
        "GROUP",
        "TLS",
        "COMPRESSED",
        "MASKOS",
        "MASKPROC"
    };

    printf("Flags:\n");

    for(int i = 0; i < 13; i++)
    {
        if(flags & flag_masks[i])
        {
            printf("%s\n", flag_str[i]);
        }
    }
}

Elf32_Sym *elf_get_sym_n_table32(Elf32_Ehdr *header, Elf32_Shdr *sheader, Elf32_Word n)
{
    if((sheader->sh_type != SHT_SYMTAB && sheader->sh_type != SHT_DYNSYM) || n >= sheader->sh_size/sheader->sh_entsize) return NULL;

    return (Elf32_Sym *)(((unsigned char *)elf_get_section32((unsigned char *)header, sheader)) + n * sheader->sh_entsize);
};

Elf64_Sym *elf_get_sym_n_table64(Elf64_Ehdr *header, Elf64_Shdr *sheader, Elf64_Word n)
{
    if((sheader->sh_type != SHT_SYMTAB && sheader->sh_type != SHT_DYNSYM) || n >= sheader->sh_size/sheader->sh_entsize) return NULL;

    return (Elf64_Sym *)(((unsigned char *)elf_get_section64((unsigned char *)header, sheader)) + n * sheader->sh_entsize);
}

Elf32_Phdr *elf_get_n_program_header32(Elf32_Ehdr *header, Elf32_Word n)
{
    if(n >= header->e_phnum) return NULL;

    Elf32_Word off = header->e_phoff;

    return ((Elf32_Phdr *)((unsigned char *)header + off)) + n;
}

Elf64_Phdr *elf_get_n_program_header64(Elf64_Ehdr *header, Elf64_Word n)
{
    if(n >= header->e_phnum) return NULL;

    Elf64_Word off = header->e_phoff;

    return ((Elf64_Phdr *)((unsigned char *)header + off)) + n;
}

