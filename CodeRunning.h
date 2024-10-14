#ifndef __CODE_RUNNING_H__
#define __CODE_RUNNING_H__

#include "ProcGlobals.h"

int RunCode (SPU_t* SPU);

int DoPush  (SPU_t* SPU);
int DoPop   (SPU_t* SPU);

int DoAdd   (SPU_t* SPU);
int DoSub   (SPU_t* SPU);
int DoMul   (SPU_t* SPU);
int DoDiv   (SPU_t* SPU);

int DoOut   (SPU_t* SPU);

int DoJmp   (SPU_t* SPU);
int DoJb    (SPU_t* SPU);

#endif