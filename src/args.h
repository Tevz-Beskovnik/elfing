/*
 * ARGUMENT PARSING LIBRARY FOR C++
 *
 * I was originary inspired to make this after watching a video on c X macros
 */

#pragma once

#ifndef ARGS_INIT
#define ARGS_INIT
#define ARGS_PROGRAM_DESCRIPTION "Empty program"
#define ARGS_LIST
#endif // !ARGS_INIT

#define _ARGS_LIST BOOLEAN_ARG(help, "Print this message") \
    ARGS_LIST
#define _ARGS_INTERNAL_FN(fn) _##args##_##fn
#define _ARGS_INTERNAL_CALL(fn) _##args##_##fn

#include <vector>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <stddef.h>

///////////////////////
// DEFS
///////////////////////

struct args;

struct args_t {
#define REQUIRED_ARG(NAME, TYPE, _0) TYPE NAME;
#define OPTIONAL_ARG(NAME, TYPE, _0, _1) TYPE NAME;
#define BOOLEAN_ARG(NAME, _0) bool NAME;
#define LIST_ARG(NAME, TYPE, _0, _1) std::vector<TYPE> NAME;


_ARGS_LIST


#undef REQUIRED_ARG
#undef OPTIONAL_ARG
#undef BOOLEAN_ARG
#undef LIST_ARG
};

bool parse_args(int argc, char *argv[], args_t *out_args);

void _ARGS_INTERNAL_FN(print_help)(char *argv[]);

template<typename T>
int _ARGS_INTERNAL_FN(parse_required_arg)(int argc, char *argv[], T *field, const char *arg_name);

template<typename T>
int _ARGS_INTERNAL_FN(parse_optional_arg)(int argc, char *argv[], T *field, const char *arg_name, char *default_val);

template<typename T>
int _ARGS_INTERNAL_FN(parse_list_arg)(int argc, char *argv[], std::vector<T> *field, const char *arg_name, bool required);

int _ARGS_INTERNAL_FN(parse_boolean_arg)(int argc, char *argv[], bool *field, const char *arg_name);

///////////////////
// IMPLS
///////////////////

bool parse_args(int argc, char *argv[], args_t *out_args)
{
#define CHECK_RET(NAME) if(NAME <= 0)\
{\
    if(NAME == 0) _ARGS_INTERNAL_CALL(print_help)(argv);\
    return false;\
}
#define REQUIRED_ARG(NAME, TYPE, _0) int NAME = _ARGS_INTERNAL_CALL(parse_required_arg)(argc, argv, (TYPE *)((uint8_t *)out_args + offsetof(args_t, NAME)), #NAME); \
    CHECK_RET(NAME)
#define OPTIONAL_ARG(NAME, TYPE, _0, DEFAULT_VAL) int NAME = _ARGS_INTERNAL_CALL(parse_optional_arg)(argc, argv, (TYPE *)((uint8_t *)out_args + offsetof(args_t, NAME)), #NAME, #DEFAULT_VAL);\
    CHECK_RET(NAME)
#define BOOLEAN_ARG(NAME, _0) int NAME = _ARGS_INTERNAL_CALL(parse_boolean_arg)(argc, argv, (bool *)((uint8_t *)out_args + offsetof(args_t, NAME)), #NAME);\
    CHECK_RET(NAME)
#define LIST_ARG(NAME, TYPE, _0, REQ) int NAME = _ARGS_INTERNAL_CALL(parse_list_arg)(argc, argv, (std::vector<TYPE> *)((uint8_t *)out_args + offsetof(args_t, NAME)), #NAME, REQ);\
    CHECK_RET(NAME)

    _ARGS_LIST

    return true;

#undef CHECK_RET
#undef REQUIRED_ARG
#undef OPTIONAL_ARG
#undef BOOLEAN_ARG
#undef LIST_ARG
}

template<typename T>
bool _ARGS_INTERNAL_FN(parse_arg)(T *field, char *value)
{
    std::stringstream ss(value);
    ss >> *field;

    return true;
}

template<>
bool _ARGS_INTERNAL_FN(parse_arg)(std::string *field, char *value)
{
    *field = value;

    return true;
}

void _ARGS_INTERNAL_FN(print_help)(char *argv[])
{
#define REQUIRED_ARG(name, type, desc) if(!first_line) {\
    std::cout << indent; \
} else {\
    first_line = false;\
}\
std::cout << "--" << #name << " value, Type: " << #type << ", Description: " << desc << std::endl;
#define OPTIONAL_ARG(name, type, desc, default_val) if(!first_line) {\
    std::cout << indent;\
} else {\
    first_line = false;\
}\
std::cout << "--" << #name << " [value], Default: " << #default_val << ", Type: " << #type << ", Description: " << desc << std::endl;
#define BOOLEAN_ARG(name, desc) if(!first_line) {\
    std::cout << indent;\
} else {\
    first_line = false;\
}\
std::cout << "--" << #name << ", Description: " << desc << std::endl;
#define LIST_ARG(name, type, desc, required) if(!first_line) {\
    std::cout << indent; \
} else {\
    first_line = false;\
}\
std::cout << "--" << #name << " " << (required ? "{" : "[") << "value.." << (required ? "}" : "]") << ", Type: " << #type << ", Description: " << desc << std::endl;


    std::string output = "usage: ";
    output += argv[0];
    output += " ";
    std::string indent(output.size(), ' ');
    bool first_line = true;

    std::cout << ARGS_PROGRAM_DESCRIPTION << std::endl;
    std::cout << output;

    _ARGS_LIST

#undef REQUIRED_ARG
#undef OPTIONAL_ARG
#undef BOOLEAN_ARG
#undef LIST_ARG
}

bool _ARGS_INTERNAL_FN(check_help)(char *val, char *argv[]) {
    if(std::string(val) == "--help") {
        _ARGS_INTERNAL_CALL(print_help)(argv);
        return true;
    }
    return false;
}

template<typename T>
int _ARGS_INTERNAL_FN(parse_required_arg)(int argc, char *argv[], T *field, const char *arg_name)
{
    for(int i = 1; i < argc; i++)
    {
        if(_ARGS_INTERNAL_CALL(check_help)(argv[i], argv)) return -1;
        if(std::string("--") + arg_name == argv[i])
        {
            if(i + 1 >= argc) return false;
            return _ARGS_INTERNAL_CALL(parse_arg)(field, argv[i + 1]);
        }
    }

    return false;
}

template<typename T>
int _ARGS_INTERNAL_FN(parse_optional_arg)(int argc, char *argv[], T *field, const char *arg_name, char *default_val)
{
    for(int i = 1; i < argc; i++)
    {
        if(_ARGS_INTERNAL_CALL(check_help)(argv[i], argv)) return -1;
        if(std::string("--") + arg_name == argv[i])
        {
            if(i + 1 >= argc) return false;
            return _ARGS_INTERNAL_CALL(parse_arg)(field, argv[i + 1]);
        }
    }

    return _ARGS_INTERNAL_CALL(parse_arg)(field, default_val);
}

template<typename T>
int _ARGS_INTERNAL_FN(parse_list_arg)(int argc, char *argv[], std::vector<T> *field, const char *arg_name, bool required)
{
    for(int i = 1; i < argc; i++) 
    {
        if(_ARGS_INTERNAL_CALL(check_help)(argv[i], argv)) return -1;
        if(std::string("--") + arg_name == argv[i])
        {
            while(++i != argc && std::string(argv[i]).substr(0, 2) != "--")
            {
                T val;
                if(_ARGS_INTERNAL_CALL(parse_arg)(&val, argv[i])) {
                    (*field).push_back(val);
                }
            }

            if(required) return !field->size() == 0;

            return true;
        }
    }

    return !required;
}

int _ARGS_INTERNAL_FN(parse_boolean_arg)(int argc, char *argv[], bool *field, const char *arg_name)
{
    for(int i = 1; i < argc; i++)
    {
        if(_ARGS_INTERNAL_CALL(check_help)(argv[i], argv)) return -1;
        if(std::string("--") + arg_name == argv[i])
        {
            *field = true;
            return true;
        }
    }

    *field = false;
    return true;
}
