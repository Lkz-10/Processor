#include "CodeRunning.h"

int RunCode(SPU_t* SPU, const char* file_out_name)
{
    assert(SPU);

    FILE* file_out_ptr = fopen(file_out_name, "w");

    while (true)
    {
        switch(SPU->code[SPU->ip] & CMD_MASK)
        {
            case CMD_PUSH:
            {
                if (DoPush(SPU) != 0) {
                    fclose(file_out_ptr);
                    file_out_ptr = NULL;

                    return -1;
                }
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
            case CMD_SQR:
            {
                DoSqr(SPU);
                break;
            }
            case CMD_SQRT:
            {
                if (DoSqrt(SPU) != 0) {
                    fclose(file_out_ptr);
                    file_out_ptr = NULL;

                    return -1;
                }
                break;
            }
            case CMD_OUT:
            {
                DoOut(SPU, file_out_ptr);
                break;
            }
            case CMD_IN:
            {
                if (DoIn(SPU) != 0) {
                    fclose(file_out_ptr);
                    file_out_ptr = NULL;

                    return -1;
                }
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
            case CMD_JE:
            {
                DoJe(SPU);
                break;
            }
            case CMD_VIS:
            {
                DoVis(SPU, file_out_ptr);
                break;
            }
            case CMD_DUMP:
            {
                DoDump(SPU, file_out_ptr);
                break;
            }
            case CMD_CALL:
            {
                DoCall(SPU);
                break;
            }
            case CMD_RET:
            {
                if (DoRet(SPU) != 0) {
                    fclose(file_out_ptr);
                    file_out_ptr = NULL;

                    return -1;
                }
                break;
            }
            case CMD_HLT:
            {
                fclose(file_out_ptr);
                file_out_ptr = NULL;

                return 0;
            }
            default:
                fprintf(stderr, "Unknown command: %d\n", SPU->code[SPU->ip]);
                return -1;
        }
    }
}

int* GetArg(SPU_t* SPU)
{
    SPU_VERIFY

    int opCode = SPU->code[SPU->ip];
    (SPU->ip)++;

    int* argValue = 0;

    if (opCode & NUMBER_MODE) argValue = &SPU->code[(SPU->ip)++];
    if (opCode & REGS_MODE) {
        if (argValue) {
            (SPU->regs)[0] = *argValue + SPU->regs[SPU->code[(SPU->ip)++]];
            argValue = &(SPU->regs)[0];
        } else {
            argValue = &SPU->regs[SPU->code[(SPU->ip)++]];
        }
    }
    if (opCode & RAM_MODE) {
        argValue = &SPU->RAM[*argValue];
    }

    if (!argValue) {
        fprintf(stderr, "Unknown command: %d!\n", SPU->code[SPU->ip - 1]);
    }

    return argValue;
}

int DoPush (SPU_t* SPU)
{
    SPU_VERIFY

    int* arg = GetArg(SPU);

    if (!arg) return -1;

    StackPush(&SPU->stack, *arg);

    return 0;
}

int DoPop (SPU_t* SPU)
{
    SPU_VERIFY

    int* arg = GetArg(SPU);

    if (!arg) return -1;

    *arg = StackPop(&SPU->stack);

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

int DoSqr(SPU_t* SPU)
{
    SPU_VERIFY

    elem_t elem = StackPop(&SPU->stack);

    StackPush(&SPU->stack, elem * elem);

    (SPU->ip)++;

    return 0;
}

int DoSqrt(SPU_t* SPU)
{
    SPU_VERIFY

    double val = (double) StackPop(&SPU->stack);

    if (val < 0)
    {
        fprintf(stderr, "Error: square root of negative number %lg!\n", val);
        return -1;
    }

    val = sqrt(val);

    StackPush(&SPU->stack, (int) val);

    (SPU->ip)++;

    return 0;
}

int DoOut(SPU_t* SPU, FILE* file_ptr)
{
    SPU_VERIFY
    assert(file_ptr);

    fprintf(file_ptr, "%d\n", StackPop(&SPU->stack));

    (SPU->ip)++;

    return 0;
}

int DoIn(SPU_t* SPU)
{
    SPU_VERIFY

    printf("Enter element to push:\n");

    elem_t elem = 0;
    if (scanf("%d", &elem) != 1) {
        fprintf(stderr, "Error: no right type element read, only left one found\n");
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

    if (elem1 > elem2) SPU->ip = SPU->code[SPU->ip + 1];
    else SPU->ip += 2;

    return 0;
}

int DoJe(SPU_t* SPU)
{
    SPU_VERIFY

    if (StackPop(&SPU->stack) == StackPop(&SPU->stack)) SPU->ip = SPU->code[SPU->ip + 1];
    else SPU->ip += 2;

    return 0;
}

int DoCall(SPU_t* SPU)
{
    SPU_VERIFY

    StackPush(&SPU->ret_addrs, SPU->ip + 2);

    SPU->ip = SPU->code[SPU->ip + 1];

    return 0;
}

int DoRet(SPU_t* SPU)
{
    SPU_VERIFY

    int address_to_return_to_right_now = StackPop(&SPU->ret_addrs);

    if (address_to_return_to_right_now == POISON ||
        address_to_return_to_right_now < 0       ||
        address_to_return_to_right_now > SPU->code_size - 1)
    {
        fprintf(stderr, "Error: ret() is called for wrong ip\n");
        return -1;
    }

    SPU->ip = address_to_return_to_right_now;

    return 0;
}

int DoVis(SPU_t* SPU, FILE* file_ptr)
{
    SPU_VERIFY

    for (int i = 0; i < RAM_SIZE / RAM_LINE_LENGTH; ++i)
    {
        for (int j = 0; j < RAM_LINE_LENGTH; ++j)
        {
            if ((SPU->RAM + i * RAM_LINE_LENGTH)[j] == 0)
            {
                fprintf(file_ptr, ".  ");
            }
            else
            {
                fprintf(file_ptr, "*  ");
            }
        }
        fprintf(file_ptr, "\n");
    }

    (SPU->ip)++;

    return 0;
}

int DoDump (SPU_t* SPU, FILE* file_ptr)
{
    SPU_VERIFY

    fprintf(file_ptr, "SPU[%p]:\n{\n", SPU);

    fprintf(file_ptr, "Code size = %d, ip = %d\n\nCode[%p]:\n", SPU->code_size, SPU->ip, SPU->code);
    for (int i = 0; i < SPU->code_size; ++i)
    {
        fprintf(file_ptr, "%4d ", SPU->code[i]);
    }
    fprintf(file_ptr, "\n");
    for (int i = 0; i < SPU->ip; ++i)
    {
        fprintf(file_ptr, "     ");
    }
    fprintf(file_ptr, "  ^\n");

    fprintf(file_ptr, "Stack[%p]:\n{\n", &SPU->stack);
    fprintf(file_ptr, "  capacity: %lld, size: %lld\n\n  Data[%p]:\n  ",
            SPU->stack.capacity, SPU->stack.sz, SPU->stack.data);
    for (size_t i = 0; i < SPU->stack.sz; ++i)
    {
        fprintf(file_ptr, "%4d ", SPU->stack.data[i]);
    }
    fprintf(file_ptr, "\n}\n\n");

    fprintf(file_ptr, "Registers[%p]:\n", SPU->regs);
    fprintf(file_ptr, "  AX: %4d\n", SPU->regs[AX]);
    fprintf(file_ptr, "  BX: %4d\n", SPU->regs[BX]);
    fprintf(file_ptr, "  CX: %4d\n", SPU->regs[CX]);
    fprintf(file_ptr, "  DX: %4d\n", SPU->regs[DX]);
    fprintf(file_ptr, "\n");

    fprintf(file_ptr, "RAM[%p]:\n", SPU->RAM);
    for (int i = 0; i < RAM_SIZE / RAM_LINE_LENGTH; ++i)
    {
        for (int j = 0; j < RAM_LINE_LENGTH; ++j)
        {
            fprintf(file_ptr, "%4d ", (SPU->RAM + i * RAM_LINE_LENGTH)[j]);
        }
        fprintf(file_ptr, "\n");
    }
    fprintf(file_ptr, "}\n");

    (SPU->ip)++;

    return 0;
}
