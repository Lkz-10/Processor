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
registers GetRegister(ASM_t* ASM);
int GetPush(ASM_t* ASM);
int GetPop(ASM_t* ASM);
int GetJmp(ASM_t* ASM);
int GetJb(ASM_t* ASM);
int PrintCode(ASM_t* ASM, const char* file_name);

#endif
