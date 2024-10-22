#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <cstdint>

#include "AsmHeaders.h"

int main(const int argc, const char** argv)
{
    if (ArgcCheck(argc) != 0) return -1;

    ASM_t ASM = {};

    if (ASMCtor(&ASM, argv[1]) != 0) return -1;

    if (ReadAsmCode(&ASM) != 0) return -1;

    rewind(ASM.cmd_ptr);
    //printf("label address: %d\n", (ASM.labels[0]).address);

    ASM.code.sz = 0;

    if (ReadAsmCode(&ASM) != 0) return -1;

    fclose(ASM.cmd_ptr);

    //printf("label address: %d\n", (ASM.labels[0]).address);

    if (PrintCode(&ASM, argv[2]) != 0) return -1;

    ASMDtor(&ASM);

    return 0;
}
