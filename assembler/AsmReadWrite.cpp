#include "AsmReadWrite.h"

int ReadAsmCode(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    char cmd[MAX_STRING_LENGTH] = {};

    while (fscanf(ASM->cmd_ptr, "%s", cmd) != EOF) {

        if (strchr(cmd, ';') != NULL) {
            if (strchr(cmd, '\n') != NULL) continue;

            fgets(cmd, MAX_STRING_LENGTH, ASM->cmd_ptr);
            continue;
        }

        if (strchr(cmd, ':') != NULL) {
            if (GetLabel(ASM, cmd) != 0) return -1;
            continue;
        }
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
        if (stricmp(cmd, "sqr") == 0) {
            StackPush(&ASM->code, CMD_SQR);
            continue;
        }
        if (stricmp(cmd, "sqrt") == 0) {
            StackPush(&ASM->code, CMD_SQRT);
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
        if (stricmp(cmd, "call") == 0) {
            if (GetJmp(ASM, CMD_CALL) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "ret") == 0) {
            StackPush(&ASM->code, CMD_RET);
            continue;
        }
        if (stricmp(cmd, "jmp") == 0) {
            if (GetJmp(ASM, CMD_JMP) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "jb") == 0) {
            if (GetJmp(ASM, CMD_JB) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "je") == 0) {
            if (GetJmp(ASM, CMD_JE) != 0) {
                fclose(ASM->cmd_ptr);
                ASM->cmd_ptr = NULL;

                return -1;
            }
            continue;
        }
        if (stricmp(cmd, "vis") == 0) {
            StackPush(&ASM->code, CMD_VIS);
            continue;
        }
        if (stricmp(cmd, "dump") == 0) {
            StackPush(&ASM->code, CMD_DUMP);
            continue;
        }
        if (stricmp(cmd, "hlt") == 0) {
            StackPush(&ASM->code, CMD_HLT);
            continue;
        }
        fprintf(stderr, "Error: command %s does not exist!\n", cmd);

        fclose(ASM->cmd_ptr);
        ASM->cmd_ptr = NULL;

        return -1;
    }

    return 0;
}

int GetLabel (ASM_t* ASM, const char* label_name)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    if (ASM->nLabels >= LABELS_ARR_SIZE) {
        fprintf(stderr, "Error: too many labels (max %d)\n", LABELS_ARR_SIZE);
        return -1;
    }

    (ASM->labels[ASM->nLabels]).name    = strdup(label_name);

    (ASM->labels[ASM->nLabels]).address = (int) (ASM->code).sz;

    (ASM->nLabels)++;

    return 0;
}

registers GetRegister(const char* arg_reg)
{
    if (!arg_reg) return NULL_REG;

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

    int cmd_ret = CMD_PUSH;
    int arg_num = 0;

    if (fscanf(ASM->cmd_ptr, "%d", &arg_num) == 1) {

        StackPush(&ASM->code, cmd_ret | NUMBER_MODE);
        StackPush(&ASM->code, arg_num);

        return 0;
    }

    char arg_str[MAX_STRING_LENGTH] = {};

    if (fscanf(ASM->cmd_ptr, "%s", arg_str) == 0) {
        fprintf(stderr, "Error while reading push() argument!\n");
        return -1;
    }

    if (strchr(arg_str, '[') != NULL && RAMCheck(arg_str, &cmd_ret) != 0){
        return -1;
    }

    char* plus_ptr = strchr(arg_str, '+');

    if (plus_ptr != NULL) {
        cmd_ret = cmd_ret | NUMBER_MODE | REGS_MODE;

        *plus_ptr = '\0';

        char arg_reg[MAX_STRING_LENGTH] = {};

        sscanf(arg_str + ((cmd_ret & RAM_MODE) != 0), "%s", arg_reg);
        sscanf(plus_ptr + 1, "%d", &arg_num);

        StackPush(&ASM->code, cmd_ret);
        StackPush(&ASM->code, arg_num);
        StackPush(&ASM->code, GetRegister(arg_reg));

        return 0;
    }

    if (sscanf(arg_str + ((cmd_ret & RAM_MODE) != 0), "%d", &arg_num) == 1) {

        cmd_ret = cmd_ret | NUMBER_MODE;

        StackPush(&ASM->code, cmd_ret);
        StackPush(&ASM->code, arg_num);

        return 0;
    }

    cmd_ret = cmd_ret | REGS_MODE;

    int reg_num = GetRegister(arg_str + ((cmd_ret & RAM_MODE) != 0));

    if (reg_num == NULL_REG) return -1;

    StackPush(&ASM->code, cmd_ret);
    StackPush(&ASM->code, reg_num);

    return 0;
}

int GetPop(ASM_t* ASM)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    char arg_str[MAX_STRING_LENGTH] = {};
    int cmd_ret = CMD_POP;

    if (fscanf(ASM->cmd_ptr, "%s", arg_str) == 0) {
        fprintf(stderr, "Error while reading pop() argument!\n");
        return -1;
    }

    if (strchr(arg_str, '[') == NULL) {
        cmd_ret = cmd_ret | REGS_MODE;
    } else if (RAMCheck(arg_str, &cmd_ret) != 0) {
        return -1;
    }

    int arg = 0;

    if ((cmd_ret & RAM_MODE) == 0) {
        arg = GetRegister(arg_str);

        if (arg == NULL_REG) return -1;

        StackPush(&ASM->code, cmd_ret);
        StackPush(&ASM->code, arg);

        return 0;
    }

    if (sscanf(arg_str + 1, "%d", &arg) == 1) {
        cmd_ret = cmd_ret | NUMBER_MODE;

        StackPush(&ASM->code, cmd_ret);
        StackPush(&ASM->code, arg);

        return 0;
    }

    cmd_ret = cmd_ret | REGS_MODE;

    char* plus_ptr = strchr(arg_str, '+');

    if (plus_ptr == NULL) {
        arg = GetRegister(arg_str + 1);

        if (arg == NULL_REG) return -1;

        StackPush(&ASM->code, cmd_ret);
        StackPush(&ASM->code, arg);

        return 0;
    }

    cmd_ret = cmd_ret | NUMBER_MODE;

    *plus_ptr = '\0';

    int reg_num = GetRegister(arg_str + 1);
    //printf("reg_num got, = %x\n", reg_num);

    if (reg_num == NULL_REG) return -1;

    sscanf(plus_ptr + 1, "%d", &arg);

    StackPush(&ASM->code, cmd_ret);
    StackPush(&ASM->code, arg);
    StackPush(&ASM->code, reg_num);

    return 0;
}

int RAMCheck (const char* arg_str, int* cmd_ret_ptr)
{
    assert(arg_str);
    assert(cmd_ret_ptr);

    char* closing_bracket_ptr = strchr(arg_str, ']');

    if (closing_bracket_ptr == NULL) {
        fprintf(stderr, "Syntax error: no closing bracket: \"%s\"\n", arg_str);
        return -1;
    }

    *closing_bracket_ptr = '\0';

    *cmd_ret_ptr = (*cmd_ret_ptr) | RAM_MODE;

    return 0;
}

int GetJmp(ASM_t* ASM, int jmp_mode)
{
    if (ASM_verify(ASM, FILE_VERIFY) != 0) return -1;

    char label_name[MAX_STRING_LENGTH] = {};
    fscanf(ASM->cmd_ptr, "%s", label_name);

    int arg_jmp = -1;

    for (int i = 0; i < ASM->nLabels; ++i) {
        if (strcmp((ASM->labels[i]).name, label_name) == 0) {
            arg_jmp = (ASM->labels[i]).address;
            break;
        }
    }

    StackPush(&ASM->code, jmp_mode);
    StackPush(&ASM->code, arg_jmp);

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
