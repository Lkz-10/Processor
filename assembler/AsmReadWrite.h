#ifndef __ASM_READ_WRITE_H__
#define __ASM_READ_WRITE_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../ProcGlobals.h"
#include "../StackFunctions.h"
#include "AsmCDtor.h"

int ReadAsmCode(ASM_t* ASM);
int GetLabel (ASM_t* ASM, const char* label_name);
registers GetRegister(const char* arg_reg);
int GetPush(ASM_t* ASM);
int GetPop(ASM_t* ASM);
int RAMCheck (const char* arg_str, int* cmd_ret_ptr);
int GetJmp(ASM_t* ASM, int jmp_mode);
int PrintCode(ASM_t* ASM, const char* file_name);

#endif
