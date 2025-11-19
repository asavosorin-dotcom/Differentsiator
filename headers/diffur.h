#ifndef _DIFFUR_H_
#define _DIFFUR_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "diff_struct.h"
#include "work_with_text.h"

#include "colors.h"

DiffNode_t* DiffNodeCtor(Type_t type, Value_t* val);
void        DiffDtor    (DiffNode_t* node);

DiffNode_t* DiffReadNode(int* pos, char* buffer);
DiffNode_t* DiffNodeMake(const char* value_node);
void DiffPrintNode(const DiffNode_t* node, FILE* file_Diff);

double DiffSolveExpresion(DiffNode_t* root);

char* Read_title(int* pos, char* buffer);
int isvariable(const char* string);
int isoperator(const char* string);

#endif