#include "CodeRunning.h"

int RunCode(SPU_t* SPU)
{
    assert(SPU);

    while (true) {
        //StackDump(&SPU->stack);
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
            case CMD_IN:
            {
                if (DoIn(SPU) != 0) return -1;
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

    StackPush(&SPU->stack, b-a);

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

    StackPush(&SPU->stack, b/a);

    (SPU->ip)++;

    return 0;
}

int DoOut(SPU_t* SPU)
{
    SPU_VERIFY

    printf("Out: %d\n", StackPop(&SPU->stack));

    (SPU->ip)++;

    return 0;
}

int DoIn(SPU_t* SPU)
{
    SPU_VERIFY

    printf("Enter element to push:\n");

    elem_t elem = 0;
    if (scanf("%d", &elem) != 1) {
        fprintf(stderr, "Error: no right type element read\n");
        return -1;
    }

    StackPush(&SPU->stack, elem);

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

    elem_t elem1 = StackPop(&SPU->stack);
    elem_t elem2 = StackPop(&SPU->stack);

    if (elem1 > elem2) DoJmp(SPU);
    else SPU->ip += 2;

    return 0;
}