#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "./src/elf.h"

#define BUILD_WATCH_LIST "./src/elf.h", "./src/util.h"

#ifndef ARGS_INIT
#define ARGS_INIT
#define ARGS_LIST \
    REQUIRED_ARG(input, std::string, "Input file to be parsed") \
    BOOLEAN_ARG(elf_header, "Print out the elf header") \
    BOOLEAN_ARG(section_headers, "Print out the elf sections") \
    BOOLEAN_ARG(program_headers, "Print out the program headers") \
    BOOLEAN_ARG(flags, "Print out the flags (if the section has them)") \
    OPTIONAL_ARG(section_header, int, "Section header to print", -1) \
    OPTIONAL_ARG(program_header, int, "Program header to print", -1) \

#define ARGS_PROGRAM_DESCRIPTION "A elf header parser"
#endif

#include "./nob.h"
#include "./src/args.h"
#include "./src/util.h"

int main(int argc, char *argv[])
{
#ifndef NDEBUG
    REBUILD_SELF_AND_WATCH(argc, argv, "./src/elf.cpp", "./src/util.cpp");
#endif
    args_t args;
    if(!parse_args(argc, argv, &args)) return EXIT_FAILURE;

    FILE *fptr;

    // reading file into memory
    fptr = fopen(args.input.c_str(), "r");

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

    if (elf_check(buffer)) {

        uint8_t elf_class = elf_get_class(buffer);

        if(elf_class == ELFCLASS32)
        {
            
        }
        else if(elf_class == ELFCLASS64)
        {
            
        }
        else
        {
            printf("Bad elf class signature\n");
        }
    }
    else
    {
        printf("Cannot detect elf signature!\n");
        return EXIT_FAILURE;
    }
    

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
