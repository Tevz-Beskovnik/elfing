#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "./src/elf.h"

#define BUILD_WATCH_LIST "./src/elf.h"

#define ARGS_INIT
#define ARGS_LIST \
    REQUIRED_ARG(input, std::string, "Input file to be parsed") \
    BOOLEAN_ARG(elf-header, "Print out the elf header") \
    BOOLEAN_ARG(section-headers, "Print out the elf sections") \
    BOOLEAN_ARG(program-headers, "Print out the program headers") \
    BOOLEAN_ARG(flags, "Print out the flags (if the section has them)") \
    OPTIONAL_ARG(section-header, int)

#define ARGS_PROGRAM_DESCRIPTION "This is a test program"

#include "./nob.h"
#include "./src/args.h"

void print_ehdr32(Elf32_Ehdr *header)
{
    printf("type: %d, machine: %d, version: %d, entry: %d\n"
           "section header offset: %d, count: %d, size: %d\n"
           "program header offset: %d, count: %d, size: %d\n"
           "flags: %d\n"
           "ehsize: %d\n"
           "secton string index: %d\n",
           header->e_type, header->e_machine, header->e_version, header->e_entry,
           header->e_shoff, header->e_shnum, header->e_shentsize,
           header->e_phoff, header->e_phnum, header->e_phentsize,
           header->e_flags,
           header->e_ehsize,
           header->e_shstrndx);
}

void print_ehdr64(Elf64_Ehdr *header)
{
    printf("type: %d, machine: %d, version: %d, entry: %lld\n"
           "section header offset: %lld, count: %d, size: %d\n"
           "program header offset: %lld, count: %d, size: %d\n"
           "flags: %d\n"
           "ehsize: %d\n"
           "section string index: %d\n",
           header->e_type, header->e_machine, header->e_version, header->e_entry,
           header->e_shoff, header->e_shnum, header->e_shentsize,
           header->e_phoff, header->e_phnum, header->e_phentsize,
           header->e_flags,
           header->e_ehsize,
           header->e_shstrndx);
}

void print_shdr32(Elf32_Ehdr *header, Elf32_Shdr *sheader)
{
    printf("name: %s, type: %d, flags: %d, addr: %d,\n"
           "off: %d, size: %d, link: %d\n"
           "info: %d, addr_align: %d, entsize: %d\n", 
           elf_section_name32(header, sheader->sh_name), sheader->sh_type, sheader->sh_flags, sheader->sh_addr,
           sheader->sh_offset, sheader->sh_size, sheader->sh_link,
           sheader->sh_info, sheader->sh_addralign, sheader->sh_entsize);
}

void print_shdr64(Elf64_Ehdr *header, Elf64_Shdr *sheader)
{
    printf("name: %s, type: %d, flags: %lld, addr: %lld,\n"
           "off: %lld, size: %lld, link: %d\n"
           "info: %d, addr_align: %lld, entsize: %lld\n", 
           elf_section_name64(header, sheader->sh_name), sheader->sh_type, sheader->sh_flags, sheader->sh_addr,
           sheader->sh_offset, sheader->sh_size, sheader->sh_link,
           sheader->sh_info, sheader->sh_addralign, sheader->sh_entsize);
}

void print_sym32(Elf32_Sym *sym, void *sym_sect)
{
    const char *name = ((const char *)sym_sect) + sym->st_name;

    printf("name: %s, value %d, size: %d\n"
           "type: %d, bind: %d, visibility: %d\n"
           "shindx: %d\n", 
           name, sym->st_value, sym->st_size,
           ELF32_ST_TYPE(sym->st_info), ELF32_ST_BIND(sym->st_info), ELF32_ST_VISIBILITY(sym->st_other),
           sym->st_shndx);
}

void print_sym64(Elf64_Sym *sym, void *sym_sect)
{
    const char *name = ((const char *)sym_sect) + sym->st_name;

    printf("name: %s, value %lld, size: %lld\n"
           "type: %d, bind: %d, visibility: %d\n"
           "shindx: %d\n", 
           name, sym->st_value, sym->st_size,
           ELF32_ST_TYPE(sym->st_info), ELF32_ST_BIND(sym->st_info), ELF32_ST_VISIBILITY(sym->st_other),
           sym->st_shndx);
}

void print_syms32(Elf32_Ehdr *header, Elf32_Shdr *sheader)
{
    for(int i = 0; i < sheader->sh_size / sheader->sh_entsize; i++)
    {
        if(i == 0) continue;

        Elf32_Sym *sym = elf_get_sym_n_table32(header, sheader, i);
        if(sym == NULL)
        {
            printf("Failed getting symbol from symtab\n");
            return;
        }
                   
        Elf32_Shdr *str_sect = (Elf32_Shdr *)((unsigned char *)sheader + sizeof(Elf32_Shdr));
        print_sym32(sym, elf_get_section32((unsigned char *)header, str_sect));
    }
}

void print_syms64(Elf64_Ehdr *header, Elf64_Shdr *sheader)
{
    for(int i = 0; i < sheader->sh_size / sheader->sh_entsize; i++)
    {
        if(i == 0) continue;

        Elf64_Sym *sym = elf_get_sym_n_table64(header, sheader, i);
        if(sym == NULL)
        {
            printf("Failed getting symbol from symtab\n");
            return;
        }
                   
        Elf64_Shdr *str_sect = (Elf64_Shdr *)((unsigned char *)sheader + sizeof(Elf64_Shdr));
        print_sym64(sym, elf_get_section64((unsigned char *)header, str_sect));
    }
}

void print_phdr32(Elf32_Phdr *header)
{
    printf("type: %d, flags: %d, offset: %d\n"
           "vaddr: %d, paddr: %d\n"
           "file size: %d, memory size: %d, align: %d\n",
           header->p_type, header->p_flags, header->p_offset,
           header->p_vaddr, header->p_paddr,
           header->p_filesz, header->p_memsz, header->p_align);
}

void print_phdr64(Elf64_Phdr *header)
{
    printf("type: %d, flags: %d, offset: %lld\n"
           "vaddr: %lld, paddr: %lld\n"
           "file size: %lld, memory size: %lld, align: %lld\n",
           header->p_type, header->p_flags, header->p_offset,
           header->p_vaddr, header->p_paddr,
           header->p_filesz, header->p_memsz, header->p_align);
}

int main(int argc, char *argv[])
{
    REBUILD_SELF_AND_WATCH(argc, argv, "./elf.c");

    if(argc < 2) return EXIT_FAILURE;

    FILE *fptr;

    fptr = fopen(argv[1], "r");

    if(fptr == NULL)
    {
        printf("Failed to open file\n");
        return EXIT_FAILURE;
    }

    fseek(fptr, 0, SEEK_END);
    int size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    printf("size: %d\n", size);

    uint8_t *buffer = (unsigned char *)malloc(size);

    fread((void *)buffer, 1, size, fptr);

    if(!elf_check(buffer))
    {
        printf("Invalid elf header\n");
        return EXIT_FAILURE;
    }

    uint8_t elf_class = elf_get_class(buffer);

    if(elf_class == ELFCLASS32)
    {
        Elf32_Ehdr *header = elf_get_header32(buffer);

        printf("Header type ELF_32\n");
        print_ehdr32(header);

        printf("Segments: \n");

        Elf32_Shdr *sheader;
        for(int i = 0; i < header->e_shnum; i++)
        {
            sheader = elf_get_section_n_header32(header, i);

            print_shdr32(header, sheader);
            elf_print_section_flags(sheader->sh_flags);
            
            if(sheader->sh_type == SHT_DYNSYM || sheader->sh_type == SHT_SYMTAB)
            {
                printf("Symbol table in entry %d:\n", i);
               
                print_syms32(header, sheader);
            }

            printf("=====\n");
        }
    }
    else if(elf_class == ELFCLASS64)
    {
        Elf64_Ehdr *header = elf_get_header64(buffer);

        printf("Header type ELF_64\n");
        print_ehdr64(header);

        printf("Segments: \n");

        Elf64_Shdr *sheader;
        for(int i = 0; i < header->e_shnum; i++)
        {
            sheader = elf_get_section_n_header64(header, i);

            printf("Symbol table in entry %d:\n", i);
            print_shdr64(header, sheader);
            
            if(sheader->sh_type == SHT_DYNSYM || sheader->sh_type == SHT_SYMTAB)
            {
                elf_print_section_flags(sheader->sh_flags);

                print_syms64(header, sheader);
                
            }

            printf("=====\n");
        }
    }

    free(buffer);

    return EXIT_SUCCESS;
}
