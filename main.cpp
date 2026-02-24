#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include "src/elf.h"

#define BUILD_WATCH_LIST "src/elf.h", "src/util.h"

#ifndef ARGS_INIT
#define ARGS_INIT
#define ARGS_LIST \
    REQUIRED_ARG(input, std::string, "Input file to be parsed") \
    BOOLEAN_ARG(elf_header, "Print out the elf header") \
    BOOLEAN_ARG(section_headers, "Print out the elf sections") \
    BOOLEAN_ARG(program_headers, "Print out the program headers") \
    BOOLEAN_ARG(flags, "Print out the flags (if the section has them)") \
    BOOLEAN_ARG(syms, "Bellow the sections that contain them, print their symbols") \
    OPTIONAL_ARG(section_header, int, "Section header to print", -1) \
    OPTIONAL_ARG(program_header, int, "Program header to print", -1) \

#define ARGS_PROGRAM_DESCRIPTION "A elf header parser"
#endif

#ifndef NDEBUG
#include "nob.h"
#endif

#define ARGS_IMPL
#include "src/args.h"
#include "src/util.h"

int main(int argc, char *argv[])
{
#ifndef NDEBUG
    REBUILD_SELF_AND_WATCH(argc, argv, "src/elf.cpp", "src/util.cpp");
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

    uint8_t *buffer = (unsigned char *)malloc(size);

    fread((void *)buffer, 1, size, fptr);

    if (elf_check(buffer)) {

        uint8_t elf_class = elf_get_class(buffer);

        if(elf_class == ELFCLASS32)
            parse_elf32(&args, buffer);
        else if(elf_class == ELFCLASS64)
            parse_elf64(&args, buffer);
        else
        {
            printf("Bad elf class signature\n");
            return EXIT_FAILURE;
        }
    }
    else
    {
        printf("Cannot detect elf signature!\n");
        return EXIT_FAILURE;
    }
    
    free(buffer);

    return EXIT_SUCCESS;
}
