#ifndef __STK_FUNCS_H__
#define __STK_FUNCS_H__

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "StackGlobals.h"
#include "../ColourPrint.h"

int StackCtor(Stk_t* stk);
void StackDtor(Stk_t* stk);
int FillPoison (Stk_t* stk);
void StackPush(Stk_t* stk, stk_el_t el);
void StackPop(Stk_t* stk, stk_el_t* var);
int StackError(Stk_t* stk);
void StackDump(Stk_t* stk ON_DEBUG(, const char* file_name, const int line, const char* func_name));
void PrintError(int error_code);

#endif
