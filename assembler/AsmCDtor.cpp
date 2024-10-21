#include "AsmCDtor.h"

int ASMCtor(ASM_t* ASM, const char* file_name)
{
    assert(ASM);

    if (StackCtor(&ASM->code) != 0) return -1;

    ASM->cmd_ptr = fopen(file_name, "r");

    if (ASM_verify(ASM, FILE_VERIFY) != 0) {
        fclose(ASM->cmd_ptr);
        ASM->cmd_ptr = NULL;

        return -1;
    }

    return 0;
}

int ASMDtor(ASM_t* ASM)
{
    assert(ASM);

    free(ASM->code.data);
    ASM->code.data = NULL;

    return 0;
}

int ASM_verify(ASM_t* ASM, int verify_mode)
{
    if (!ASM) {
        fprintf(stderr, "Error: ASM address is NULL\n");
        return -1;
    }
    if (!ASM->code.data) {
        fprintf(stderr, "Error: code address is NULL\n");
        return -1;
    }
    if (verify_mode == FILE_VERIFY && !ASM->cmd_ptr) {
        fprintf(stderr, "Error: file pointer address is NULL\n");
        return -1;
    }
    return 0;
}

int ArgcCheck(const int argc)
{
    if (argc < 3) {
        fprintf(stderr, "Enter names of the files\n");
        return -1;
    }

    return 0;
}
