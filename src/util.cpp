#include "util.h"
#include "elf.h"
#include <cstdio>

void print_ehdr32(Elf32_Ehdr *header, std::ostream &stream)
{
    stream << "type: " << elf_get_eh_type_name(header->e_type) << ", machine: " << header->e_machine 
           << ", version: " << header->e_version << ", entry: " << header->e_entry << "\n"
           "section header offset: " << header->e_shoff << ", count: " << header->e_shnum << ", size: " << header->e_shentsize << "\n"
           "program header offset: " << header->e_phoff << ", count: " << header->e_phnum << ", size: " << header->e_phentsize << "\n"
           "flags: " << header->e_flags << "\n"
           "ehsize: " << header->e_ehsize << "\n"
           "secton string index: " << header->e_shstrndx << "\n";
}

void print_ehdr64(Elf64_Ehdr *header, std::ostream &stream)
{
    stream << "type: " << elf_get_eh_type_name(header->e_type) << ", machine: " << header->e_machine 
           << ", version: " << header->e_version << ", entry: " << header->e_entry << "\n"
           "section header offset: " << header->e_shoff << ", count: " << header->e_shnum << ", size: " << header->e_shentsize << "\n"
           "program header offset: " << header->e_phoff << ", count: " << header->e_phnum << ", size: " << header->e_phentsize << "\n"
           "flags: " << header->e_flags << "\n"
           "ehsize: " << header->e_ehsize << "\n"
           "secton string index: " << header->e_shstrndx << "\n";
}

void print_shdr32(Elf32_Ehdr *header, Elf32_Shdr *sheader, std::ostream &stream)
{
    stream << "name: " << elf_section_name32(header, sheader->sh_name) 
           << ", type: " << elf_get_sh_type_name(sheader->sh_type) 
           << ", flags: " << sheader->sh_flags << ", addr: " << sheader->sh_addr << ",\n"
           "off: " << sheader->sh_offset << ", size: " << sheader->sh_size << ", link: " << sheader->sh_link << "\n"
           "info: " << sheader->sh_info << ", addr_align: " << sheader->sh_addralign << ", entsize: " << sheader->sh_entsize << "\n";
}

void print_shdr64(Elf64_Ehdr *header, Elf64_Shdr *sheader, std::ostream &stream)
{
    stream << "name: " << elf_section_name64(header, sheader->sh_name) 
           << ", type: " << elf_get_sh_type_name(sheader->sh_type) 
           << ", flags: " << sheader->sh_flags << ", addr: " << sheader->sh_addr << ",\n"
           "off: " << sheader->sh_offset << ", size: " << sheader->sh_size << ", link: " << sheader->sh_link << "\n"
           "info: " << sheader->sh_info << ", addr_align: " << sheader->sh_addralign << ", entsize: " << sheader->sh_entsize << "\n";
}

void print_shdrs32(Elf32_Ehdr *header, bool flags, bool syms, std::ostream &stream)
{
    Elf32_Shdr *sheader;
    stream << "=========\n";
    for(int i = 0; i < header->e_shnum; i++)
    {
        sheader = elf_get_section_n_header32(header, i);
        print_shdr32(header, sheader, stream);

        if(flags) elf_print_section_flags(sheader->sh_flags);

        if(syms && (sheader->sh_type == SHT_DYNSYM || sheader->sh_type == SHT_SYMTAB)) print_syms32(header, sheader, stream);

        stream << "=========\n";
    }
}

void print_shdrs64(Elf64_Ehdr *header, bool flags, bool syms, std::ostream &stream)
{
    Elf64_Shdr *sheader;
    stream << "=========\n";
    for(int i = 0; i < header->e_shnum; i++)
    {
        sheader = elf_get_section_n_header64(header, i);
        print_shdr64(header, sheader, stream);

        if(flags) elf_print_section_flags(sheader->sh_flags);

        if(syms && (sheader->sh_type == SHT_DYNSYM || sheader->sh_type == SHT_SYMTAB)) print_syms64(header, sheader, stream);

        stream << "=========\n";
    }
}

void print_sym32(Elf32_Sym *sym, void *sym_sect, std::ostream &stream)
{
    const char *name = ((const char *)sym_sect) + sym->st_name;

    stream << "name: " << name
           << ", value " << sym->st_value
           << ", size: " << sym->st_size << "\n"
           << "type: " << ELF32_ST_TYPE(sym->st_info)
           << ", bind: " << ELF32_ST_BIND(sym->st_info)
           << ", visibility: " << ELF32_ST_VISIBILITY(sym->st_other) << "\n"
           << "shindx: " << sym->st_shndx
           << std::endl;
}

void print_sym64(Elf64_Sym *sym, void *sym_sect, std::ostream &stream)
{
    const char *name = ((const char *)sym_sect) + sym->st_name;

    stream << "name: " << name
           << ", value " << sym->st_value
           << ", size: " << sym->st_size << "\n"
           << "type: " << ELF32_ST_TYPE(sym->st_info)
           << ", bind: " << ELF32_ST_BIND(sym->st_info)
           << ", visibility: " << ELF32_ST_VISIBILITY(sym->st_other) << "\n"
           << "shindx: " << sym->st_shndx
           << std::endl;
}

void print_syms32(Elf32_Ehdr *header, Elf32_Shdr *sheader, std::ostream &stream)
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
        print_sym32(sym, elf_get_section32((unsigned char *)header, str_sect), stream);
    }
}

void print_syms64(Elf64_Ehdr *header, Elf64_Shdr *sheader, std::ostream &stream)
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
        print_sym64(sym, elf_get_section64((unsigned char *)header, str_sect), stream);
    }
}

void print_phdr32(Elf32_Phdr *header, std::ostream &stream)
{
    stream << "type: " << elf_get_ph_type_name(header->p_type)
           << ", flags: " << header->p_flags
           << ", offset: " << header->p_offset << "\n"
           << "vaddr: " << header->p_vaddr
           << ", paddr: " << header->p_paddr << "\n"
           << "file size: " << header->p_filesz
           << ", memory size: " << header->p_memsz
           << ", align: " << header->p_align
           << std::endl;
}

void print_phdr64(Elf64_Phdr *header, std::ostream &stream)
{
    stream << "type: " << elf_get_ph_type_name(header->p_type)
           << ", flags: " << header->p_flags
           << ", offset: " << header->p_offset << "\n"
           << "vaddr: " << header->p_vaddr
           << ", paddr: " << header->p_paddr << "\n"
           << "file size: " << header->p_filesz
           << ", memory size: " << header->p_memsz
           << ", align: " << header->p_align
           << std::endl;
}

void print_phdrs32(Elf32_Ehdr *header, std::ostream &stream)
{
    Elf32_Phdr *sheader;
    stream << "=========\n";
    for(int i = 0; i < header->e_phnum; i++)
    {
        sheader = elf_get_n_program_header32(header, i);
        print_phdr32(sheader, stream);
        stream << "=========\n";
    }
}

void print_phdrs64(Elf64_Ehdr *header, std::ostream &stream)
{
    Elf64_Phdr *sheader;
    stream << "=========\n";
    for(int i = 0; i < header->e_phnum; i++)
    {
        sheader = elf_get_n_program_header64(header, i);
        print_phdr64(sheader, stream);
        stream << "=========\n";
    }
}

void parse_elf32(args_t *args, uint8_t* buffer)
{
    Elf32_Ehdr *header = elf_get_header32(buffer);
    printf("Elf format 32\n");

    if(args->elf_header)
    {
        print_ehdr32(header);
    }
    else if(args->section_headers)
    {
        print_shdrs32(header, args->flags, args->syms);
    }
    else if(args->program_headers)
    {
        print_phdrs32(header);
    }
    else if(args->section_header != -1)
    {
        Elf32_Shdr *section_header = elf_get_section_n_header32(header, args->section_header);

        print_shdr32(header, section_header);

        if(args->flags) elf_print_section_flags(section_header->sh_flags);
        
        if(args->syms && (section_header->sh_type == SHT_DYNSYM || section_header->sh_type == SHT_SYMTAB)) print_syms32(header, section_header);
    }
    else if(args->program_header != -1)
    {
        Elf32_Phdr *phisical_header = elf_get_n_program_header32(header, args->program_header);

        print_phdr32(phisical_header);
    }
}

void parse_elf64(args_t *args, uint8_t* buffer)
{
    Elf64_Ehdr *header = elf_get_header64(buffer);

    printf("Elf format 64\n");

    if(args->elf_header)
    {
        print_ehdr64(header);
    }
    else if(args->section_headers)
    {
        print_shdrs64(header, args->flags, args->syms);
    }
    else if(args->program_headers)
    {
        print_phdrs64(header);
    }
    else if(args->section_header != -1)
    {
        Elf64_Shdr *section_header = elf_get_section_n_header64(header, args->section_header);

        print_shdr64(header, section_header);

        if(args->flags) elf_print_section_flags(section_header->sh_flags);

        if(args->syms && (section_header->sh_type == SHT_DYNSYM || section_header->sh_type == SHT_SYMTAB)) print_syms64(header, section_header);
    }
    else if(args->program_header != -1)
    {
        Elf64_Phdr *phisical_header = elf_get_n_program_header64(header, args->program_header);

        print_phdr64(phisical_header);
    }
}
