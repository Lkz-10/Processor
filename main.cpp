#include <stdio.h>

#include "ProcHeaders.h"

int ArgcCheck(const int argc);
int ReadCode(SPU_t* SPU, const char* file_name);
int SPU_Ctor (SPU_t* SPU, const char* file_name);

int main(const int argc, const char** argv)
{
    if (ArgcCheck(argc) == -1) return -1;

    SPU_t SPU = {};

    if (SPU_Ctor(&SPU, argv[1]) != 0) return -1;

    printf("%d\n", SPU.code[5]);


    return 0;
}

int ArgcCheck(const int argc)
{
    if (argc < 2) return -1;

    return 0;
}

int SPU_Ctor (SPU_t* SPU, const char* file_name)
{
    SPU->ip = 0;

    if (StackCtor(&SPU->stack) != 0) return -1;

    if (ReadCode(SPU, file_name) != 0) return -1;

    return 0;
}

int ReadCode(SPU_t* SPU, const char* file_name)
{
    FILE* code_ptr = fopen(file_name, "r");
    if (code_ptr == NULL) {
        return -1;
    }

    int size = 0;
    fscanf(code_ptr, "%d", &size);

    SPU->code = (int*) calloc(size, sizeof(int));

    for (int i = 0; i < size; ++i) {
        fscanf(code_ptr, "%x", &SPU->code[i]);
    }

    return 0;
}
