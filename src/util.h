#pragma once
#include "./elf.h"
#include <fstream>

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

#include "./args.h"

void print_ehdr32(Elf32_Ehdr *header, std::ostream &stream = std::cout);
void print_ehdr64(Elf64_Ehdr *header, std::ostream &stream = std::cout);

void print_shdr32(Elf32_Ehdr *header, Elf32_Shdr *sheader, std::ostream &stream = std::cout);
void print_shdr64(Elf64_Ehdr *header, Elf64_Shdr *sheader, std::ostream &stream = std::cout);

void print_shdrs32(Elf32_Ehdr *header, bool flags, bool syms, std::ostream &stream = std::cout);
void print_shdrs64(Elf64_Ehdr *header, bool flags, bool syms, std::ostream &stream = std::cout);

void print_sym32(Elf32_Sym *sym, void *sym_sect, std::ostream &stream = std::cout);
void print_sym64(Elf64_Sym *sym, void *sym_sect, std::ostream &stream = std::cout);

void print_syms32(Elf32_Ehdr *header, Elf32_Shdr *sheader, std::ostream &stream = std::cout);
void print_syms64(Elf64_Ehdr *header, Elf64_Shdr *sheader, std::ostream &stream = std::cout);

void print_phdr32(Elf32_Phdr *header, std::ostream &stream = std::cout);
void print_phdr64(Elf64_Phdr *header, std::ostream &stream = std::cout);

void print_phdrs32(Elf32_Ehdr *header, std::ostream &stream = std::cout);
void print_phdrs64(Elf64_Ehdr *header, std::ostream &stream = std::cout);

void parse_elf32(args_t *args, uint8_t* buffer);
void parse_elf64(args_t *args, uint8_t* buffer);
