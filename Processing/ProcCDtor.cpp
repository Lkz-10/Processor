#include "ProcCDtor.h"

int ArgcCheck(const int argc)
{
    if (argc < 3) {
        fprintf(stderr, "Enter name of file to read\n");
        return -1;
    }

    return 0;
}

int SPU_Ctor (SPU_t* SPU, const char* file_name)
{
    assert(SPU);

    FILE* code_ptr = fopen(file_name, "r");

    if (!code_ptr) {
        fprintf(stderr, "Error while opening file \"%s\"\n", file_name);
        return -1;
    }

    if (ReadHeader(code_ptr) != 0) return -1;

    SPU->ip = 0;

    if (StackCtor(&SPU->stack) != 0) return -1;

    if (ReadCode(SPU, code_ptr) != 0) return -1;

    return 0;
}

int ReadHeader(FILE* code_ptr)
{
    assert(code_ptr);

    Code_header hdr = {};

    fread(&hdr.signature, sizeof(int), 1, code_ptr);
    if (hdr.signature != SIGNATURE) {
        fprintf(stderr, "Error: wrong signature \"%d\" (required \"%d\")\n", hdr.signature, SIGNATURE);
        return -1;
    }

    fscanf(code_ptr, "%d", &hdr.asm_version);
    if (hdr.asm_version != ASM_VERSION) {
        fprintf(stderr, "Error: wrong asm version: %d (required %d)\n", hdr.asm_version, ASM_VERSION);
        return -1;
    }

    return 0;
}

int ReadCode(SPU_t* SPU, FILE* code_ptr)
{
    fscanf(code_ptr, "%d", &SPU->code_size);

    SPU->code = (int*) calloc(SPU->code_size, sizeof(int));

    for (int i = 0; i < SPU->code_size; ++i) {
        fscanf(code_ptr, "%x", &SPU->code[i]);
    }

    fclose(code_ptr);

    return 0;
}

int SPU_Dtor (SPU_t* SPU)
{
    SPU_VERIFY

    StackDtor(&SPU->stack);

    free(SPU->code);
    SPU->code = NULL;

    SPU->ip = 0;

    return 0;
}
