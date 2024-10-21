#ifndef __PROC_CDTOR_H__
#define __PROC_CDTOR_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../ProcGlobals.h"
#include "../StackGlobals.h"
#include "../StackFunctions.h"

int ArgcCheck(const int argc);
int ReadHeader(FILE* code_ptr);
int ReadCode(SPU_t* SPU, FILE* code_ptr);
int SPU_Ctor (SPU_t* SPU, const char* file_name);
int SPU_Dtor (SPU_t* SPU);

#endif
