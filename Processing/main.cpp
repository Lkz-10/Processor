#include <stdio.h>

#include "ProcHeaders.h"

int main(const int argc, const char** argv)
{
    if (ArgcCheck(argc) != 0) return -1;

    SPU_t SPU = {};

    if (SPU_Ctor(&SPU, argv[1]) != 0) {
        SPU_Dtor(&SPU);
        return -1;
    }

    if (RunCode(&SPU) != 0) {
        SPU_Dtor(&SPU);
        return -1;
    }

    return 0;
}
