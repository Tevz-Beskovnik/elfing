#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./elf.h"

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
           "off: %d, size: %d, link %d\n"
           "info: %d, addr_align: %d, entsize: %d\n", 
           elf_section_name32(header, sheader->sh_name), sheader->sh_type, sheader->sh_flags, sheader->sh_addr,
           sheader->sh_offset, sheader->sh_size, sheader->sh_link,
           sheader->sh_info, sheader->sh_addralign, sheader->sh_entsize);
}

void print_shdr64(Elf64_Ehdr *header, Elf64_Shdr *sheader)
{
    printf("name: %s, type: %d, flags: %lld, addr: %lld,\n"
           "off: %lld, size: %lld, link %d\n"
           "info: %d, addr_align: %lld, entsize: %lld\n", 
           elf_section_name64(header, sheader->sh_name), sheader->sh_type, sheader->sh_flags, sheader->sh_addr,
           sheader->sh_offset, sheader->sh_size, sheader->sh_link,
           sheader->sh_info, sheader->sh_addralign, sheader->sh_entsize);
}

int main(int argc, char *argv[])
{
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

    printf("%d%d%d%d\n", buffer[0], buffer[1], buffer[2], buffer[3]);

    if(!elf_check(buffer))
    {
        printf("Invalid elf header\n");
        return EXIT_FAILURE;
    }

    uint8_t class = elf_get_class(buffer);

    if(class == ELFCLASS32)
    {
        Elf32_Ehdr *header = elf_get_header32(buffer);

        printf("Header type ELF_32\n");
        print_ehdr32(header);

        printf("Segments: ");

        Elf32_Shdr *sheader;
        for(int i = 0; i < header->e_shnum; i++)
        {
            sheader = elf_get_section_n_header32(header, i);
            print_shdr32(header, sheader);
            printf("=====\n");
        }
    }
    else if(class == ELFCLASS64)
    {
        Elf64_Ehdr *header = elf_get_header64(buffer);

        printf("Header type ELF_64\n");
        print_ehdr64(header);

        printf("Segments: ");

        Elf64_Shdr *sheader;
        for(int i = 0; i < header->e_shnum; i++)
        {
            sheader = elf_get_section_n_header64(header, i);
            print_shdr64(header, sheader);
            printf("=====\n");
        }
    }

    free(buffer);

    return EXIT_SUCCESS;
}
