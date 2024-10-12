#ifndef __PROC_GLOBALS_H__
#define __PROC_GLOBALS_H__

#include <stdlib.h>

enum CMD_CODES
{
    CMD_PUSH = 0x01,
    CMD_POP  = 0x02,
    CMD_ADD  = 0x03,
    CMD_SUB  = 0x04,
    CMD_MUL  = 0x05,
    CMD_DIV  = 0x06,
    CMD_OUT  = 0x07,
    CMD_IN   = 0x08,
    CMD_JMP  = 0x09,
    CMD_JB   = 0x0A,
    CMD_HLT  = 0x0B
};

enum registers
{
    AX = 1,
    BX = 2,
    CX = 3,
    DX = 4
};

const int REGS_NUM = 5;

struct SPU_t
{
    int ip;
    int* code;
    Stk_t stack;
    int regs[REGS_NUM];
};

#endif
