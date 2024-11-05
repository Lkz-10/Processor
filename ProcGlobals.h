#ifndef __PROC_GLOBALS_H__
#define __PROC_GLOBALS_H__

#include <stdlib.h>
#include "StackGlobals.h"

#define DEF_CMD(cmd, num, ...) \
    CMD_##cmd = num,

typedef int elem_t;

struct Code_header
{
    int signature;
    int asm_version;
};

const int SIGNATURE         = *((const int*) "LkzX");
const int ASM_VERSION       = 1;
const int MAX_STRING_LENGTH = 52;

#define SPU_VERIFY               \
    assert(SPU);                 \
    assert(SPU->code);           \
    assert(SPU->stack.data);     \
    assert(SPU->ret_addrs.data); \
    assert(SPU->RAM);            \
    assert(SPU->regs);

const int CMD_MASK    = 0x1F;
const int MODE_MASK   = 0xE0;

enum CMD_MODES
{
    NUMBER_MODE = 0x20,
    REGS_MODE   = 0x40,
    RAM_MODE    = 0x80
};

enum CMD_CODES
{
    CMD_PUSH =  0x01,
    CMD_POP  =  0x02,
    CMD_ADD  =  0x03,
    CMD_SUB  =  0x04,
    CMD_MUL  =  0x05,
    CMD_DIV  =  0x06,
    CMD_SQR  =  0x07,
    CMD_SQRT =  0x08,
    CMD_OUT  =  0x09,
    CMD_IN   =  0x0A,
    CMD_JMP  =  0x0B,
    CMD_JB   =  0x0C,
    CMD_JE   =  0x0D,
    CMD_VIS  =  0x0E,
    CMD_DUMP =  0x0F,
    CMD_CALL =  0x10,
    CMD_RET  =  0x11,
    CMD_HLT  =  0x00
    //#include "Commands_DSL.h"
};

enum registers
{
    NULL_REG = 0x00,

    AX = 0x01,
    BX = 0x02,
    CX = 0x03,
    DX = 0x04
};

const int NREGS           = 5;
const int RAM_SIZE        = 121;
const int RAM_LINE_LENGTH = 11;

struct SPU_t
{
    int     ip;
    int     code_size      = 0;
    elem_t* code;
    elem_t  regs[NREGS]    = {};
    elem_t  RAM [RAM_SIZE] = {};
    Stk_t   stack;
    Stk_t   ret_addrs;
};

enum verify_modes
{
    NO_FILE_VERIFY = 0,
    FILE_VERIFY    = 1
};

struct Label
{
    char* name;
    int   address = -1;
};

const int LABELS_ARR_SIZE = 32;

struct ASM_t
{
    Stk_t   code;
    int     nLabels                 = 0;
    Label   labels[LABELS_ARR_SIZE] = {};
    FILE*   cmd_ptr;
};

#endif
