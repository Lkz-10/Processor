#include <stdio.h>

#include "ProcHeaders.h"

int ArgcCheck(const int argc);
int ReadCode(SPU_t* SPU, const char* file_name);
int SPU_Ctor (SPU_t* SPU, const char* file_name);
int SPU_Dtor (SPU_t* SPU);

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

int ArgcCheck(const int argc)
{
    if (argc < 2) {
        fprintf(stderr, "Enter name of file to read\n");
        return -1;
    }

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

int RunCode(SPU_t* SPU)
{
    assert(SPU);

    while (true) {
        switch(SPU->code[SPU->ip] & CMD_MASK)
        {
            case CMD_PUSH:
            {
                if (DoPush(SPU) != 0) return -1;
                break;
            }
            case CMD_POP:
            {
                DoPop(SPU);
                break;
            }
            case CMD_ADD:
            {
                DoAdd(SPU);
                break;
            }
            case CMD_SUB:
            {
                DoSub(SPU);
                break;
            }
            case CMD_MUL:
            {
                DoMul(SPU);
                break;
            }
            case CMD_DIV:
            {
                DoDiv(SPU);
                break;
            }
            case CMD_OUT:
            {
                DoOut(SPU);
                break;
            }
            case CMD_JMP:
            {
                DoJmp(SPU);
                break;
            }
            case CMD_JB:
            {
                DoJb(SPU);
                break;
            }
            case CMD_HLT:
            {
                return 0;
            }
            default:
                fprintf(stderr, "Unknown command: %d\n", SPU->code[SPU->ip]);
                return -1;
        }
    }
}

int DoPush (SPU_t* SPU)
{
    SPU_VERIFY

    switch(SPU->code[SPU->ip] & MODE_MASK)
    {
        case NUMBER_MODE:
        {
            StackPush(&SPU->stack, SPU->code[SPU->ip+1]);
            SPU->ip += 2;
            break;
        }
        case REGS_MODE:
        {
            StackPush(&SPU->stack, SPU->regs[SPU->code[SPU->ip+1]]);
            SPU->ip += 2;
            break;
        }
        default:
        {
            fprintf(stderr, "Unknown mode of \"push\" command: %x\n", SPU->code[SPU->ip]);
            return -1;
        }
    }
    return 0;
}

int DoPop (SPU_t* SPU)
{
    SPU_VERIFY

    SPU->regs[SPU->code[SPU->ip+1]] = StackPop(&SPU->stack);

    SPU->ip += 2;

    return 0;
}

int DoAdd(SPU_t* SPU)
{
    SPU_VERIFY

    StackPush(&SPU->stack, StackPop(&SPU->stack) + StackPop(&SPU->stack));

    (SPU->ip)++;

    return 0;
}

int DoSub (SPU_t* SPU)
{
    SPU_VERIFY

    elem_t a = StackPop(&SPU->stack);
    elem_t b = StackPop(&SPU->stack);

    StackPush(&SPU->stack, a-b);

    (SPU->ip)++;

    return 0;
}

int DoMul (SPU_t* SPU)
{
    SPU_VERIFY

    StackPush(&SPU->stack, StackPop(&SPU->stack) * StackPop(&SPU->stack));

    (SPU->ip)++;

    return 0;
}

int DoDiv(SPU_t* SPU)
{
    SPU_VERIFY

    elem_t a = StackPop(&SPU->stack);
    elem_t b = StackPop(&SPU->stack);

    StackPush(&SPU->stack, a/b);

    (SPU->ip)++;

    return 0;
}

int DoOut(SPU_t* SPU)
{
    SPU_VERIFY

    printf("%d\n", StackPop(&SPU->stack));

    (SPU->ip)++;

    return 0;
}

int DoJmp(SPU_t* SPU)
{
    SPU_VERIFY

    SPU->ip = SPU->code[SPU->ip + 1];

    return 0;
}

int DoJb(SPU_t* SPU)
{
    SPU_VERIFY

    if (SPU->regs[SPU->code[SPU->ip+2]] < SPU->code[SPU->ip+3]) DoJmp(SPU);
    else SPU->ip += 4;

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
