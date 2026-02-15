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
           "ehsize: %d\n",
           header->e_type, header->e_machine, header->e_version, header->e_entry,
           header->e_shoff, header->e_shnum, header->e_shentsize,
           header->e_phoff, header->e_phnum, header->e_phentsize,
           header->e_flags,
           header->e_ehsize);
}

void print_ehdr64(Elf64_Ehdr *header)
{
    printf("type: %d, machine: %d, version: %d, entry: %lld\n"
           "section header offset: %lld, count: %d, size: %d\n"
           "program header offset: %lld, count: %d, size: %d\n"
           "flags: %d\n"
           "ehsize: %d\n",
           header->e_type, header->e_machine, header->e_version, header->e_entry,
           header->e_shoff, header->e_shnum, header->e_shentsize,
           header->e_phoff, header->e_phnum, header->e_phentsize,
           header->e_flags,
           header->e_ehsize);
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
        printf("Header type ELF_32\n");
        print_ehdr32(elf_get_header32(buffer));
    }
    else if(class == ELFCLASS64)
    {
        printf("Header type ELF_64\n");
        print_ehdr64(elf_get_header64(buffer));
    }



    free(buffer);

    return EXIT_SUCCESS;
}
