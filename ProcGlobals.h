#ifndef __PROC_GLOBALS_H__
#define __PROC_GLOBALS_H__

#include <stdlib.h>

typedef int elem_t;

#define SPU_VERIFY             \
    assert(SPU);               \
    assert(SPU->code);         \
    assert(SPU->stack.data);   \
    assert(SPU->regs);

const int CMD_MASK    = 0x0F;
const int MODE_MASK   = 0xF0;

const int NUMBER_MODE = 0x10;
const int REGS_MODE   = 0x20;

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
    int     ip;
    elem_t* code;
    elem_t  regs[REGS_NUM];
    Stk_t   stack;
};

#endif
