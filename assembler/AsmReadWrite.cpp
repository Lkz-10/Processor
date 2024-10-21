#include "AsmReadWrite.h"

int ReadAsmCode(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    char cmd[MAX_STRING_LENGTH] = {};

    while (fscanf(ASM->cmd_ptr, "%s", cmd) != EOF) {
        //printf("Read command: %s\n", cmd);

        if (stricmp(cmd, "push") == 0) {
            if (GetPush(ASM) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "pop") == 0) {
            if (GetPop(ASM) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "add") == 0) {
            StackPush(&ASM->code, CMD_ADD);
            continue;
        }
        if (stricmp(cmd, "sub") == 0) {
            StackPush(&ASM->code, CMD_SUB);
            continue;
        }
        if (stricmp(cmd, "add") == 0) {
            StackPush(&ASM->code, CMD_ADD);
            continue;
        }
        if (stricmp(cmd, "mul") == 0) {
            StackPush(&ASM->code, CMD_MUL);
            continue;
        }
        if (stricmp(cmd, "div") == 0) {
            StackPush(&ASM->code, CMD_DIV);
            continue;
        }
        if (stricmp(cmd, "out") == 0) {
            StackPush(&ASM->code, CMD_OUT);
            continue;
        }
        if (stricmp(cmd, "in") == 0) {
            StackPush(&ASM->code, CMD_IN);
            continue;
        }
        if (stricmp(cmd, "jmp") == 0) {
            if (GetJmp(ASM) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "jb") == 0) {
            if (GetJb(ASM) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "hlt") == 0) {
            StackPush(&ASM->code, CMD_HLT);
            continue;
        }
        fprintf(stderr, "Error: command %s does not exist!\n", cmd);
        //fprintf(stderr, "code size: %lld\n", ASM->code.sz);

        fclose(ASM->cmd_ptr);
        ASM->cmd_ptr = NULL;

        return -1;
    }

    fclose(ASM->cmd_ptr);
    ASM->cmd_ptr = NULL;

    return 0;
}

registers GetRegister(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return NULL_REG;

    char arg_reg[MAX_STRING_LENGTH] = {};

    fscanf(ASM->cmd_ptr, "%s", arg_reg);
    //printf("Read register: %s\n", arg_reg);

    if (stricmp(arg_reg, "AX") == 0) return AX;
    if (stricmp(arg_reg, "BX") == 0) return BX;
    if (stricmp(arg_reg, "CX") == 0) return CX;
    if (stricmp(arg_reg, "DX") == 0) return DX;

    fprintf(stderr, "Error: register %s does not exist!\n", arg_reg);
    return NULL_REG;
}

int GetPush(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    int arg_num = 0;

    if (fscanf(ASM->cmd_ptr, "%d", &arg_num) == 1) {

        StackPush(&ASM->code, CMD_PUSH | NUMBER_MODE);
        StackPush(&ASM->code, arg_num);

        return 0;
    }

    int reg_num = GetRegister(ASM);

    if (reg_num == -1) return -1;

    StackPush(&ASM->code, CMD_PUSH | REGS_MODE);
    StackPush(&ASM->code, reg_num);

    return 0;
}

int GetPop(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    int reg_num = GetRegister(ASM);
    //printf("reg_num got, = %x\n", reg_num);

    if (reg_num == -1) return -1;

    StackPush(&ASM->code, CMD_POP);
    StackPush(&ASM->code, reg_num);

    return 0;
}

int GetJmp(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    int arg_jmp = 0;
    if (fscanf(ASM->cmd_ptr, "%d", &arg_jmp) != 1) return -1;

    StackPush(&ASM->code, CMD_JMP);
    StackPush(&ASM->code, arg_jmp);

    return 0;
}

int GetJb(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    int arg_jb = 0;
    if (fscanf(ASM->cmd_ptr, "%d", &arg_jb) != 1) return -1;

    StackPush(&ASM->code, CMD_JB);
    StackPush(&ASM->code, arg_jb);

    return 0;
}

int PrintCode(ASM_t* ASM, const char* file_name)
{
    if (ASM_verify(ASM, NO_FILE_VERIFY) != 0) return -1;

    ASM->cmd_ptr = fopen(file_name, "w");

    if (!ASM->cmd_ptr) {
        fprintf(stderr, "Error while opening file: %s\n", file_name);
        return -1;
    }

    fprintf(ASM->cmd_ptr, "LkzX\n1\n%lld\n", ASM->code.sz);

    for (size_t i = 0; i < ASM->code.sz; ++i) {
        fprintf(ASM->cmd_ptr, "%x ", (ASM->code.data)[i]);
    }

    fclose(ASM->cmd_ptr);
    ASM->cmd_ptr = NULL;

    return 0;
}
