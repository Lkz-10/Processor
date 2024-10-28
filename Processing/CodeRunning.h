#ifndef __CODE_RUNNING_H__
#define __CODE_RUNNING_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "../ProcGlobals.h"
#include "../StackFunctions.h"

int RunCode (SPU_t* SPU, const char* file_out_name);

int  DoPush (SPU_t* SPU);
int  DoPop  (SPU_t* SPU);
int* GetArg (SPU_t* SPU);

int DoAdd   (SPU_t* SPU);
int DoSub   (SPU_t* SPU);
int DoMul   (SPU_t* SPU);
int DoDiv   (SPU_t* SPU);
int DoSqr   (SPU_t* SPU);
int DoSqrt  (SPU_t* SPU);

int DoOut   (SPU_t* SPU, FILE* file_ptr);
int DoIn    (SPU_t* SPU);

int DoJmp   (SPU_t* SPU);
int DoJb    (SPU_t* SPU);
int DoJe    (SPU_t* SPU);

int DoVis   (SPU_t* SPU, FILE* file_ptr);
int DoDump  (SPU_t* SPU, FILE* file_ptr);

#endif
