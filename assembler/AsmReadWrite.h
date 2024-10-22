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
int GetJmp(ASM_t* ASM, int jmp_mode);
//int GetJb(ASM_t* ASM);
int PrintCode(ASM_t* ASM, const char* file_name);

#endif
