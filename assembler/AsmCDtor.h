#ifndef __ASM_CDTOR_H__
#define __ASM_CDTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../ProcGlobals.h"
#include "../StackFunctions.h"

int ArgcCheck(const int argc);
int ASMCtor(ASM_t* ASM, const char* file_name);
int ASMDtor(ASM_t* ASM);
int ASM_verify(ASM_t* ASM, int verify_mode);

#endif
