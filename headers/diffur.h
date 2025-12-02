#ifndef _DIFFUR_H_
#define _DIFFUR_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "diff_struct.h"
#include "diff_dump.h"
#include "diff_optimiz.h"

#include "work_with_text.h"
#include "colors.h"

DiffNode_t* DiffNodeCtor(Type_t type, Value_t* val, DiffNode_t* parent);
void        DiffDtor    (DiffNode_t* node);

DiffNode_t* DiffReadNode(int* pos, char* buffer);
DiffNode_t* DiffNodeMake(const char* value_node);
void        DiffPrintNode(const DiffNode_t* node, FILE* file_Diff);

double DiffSolveExpresion(DiffNode_t* root);

char* Read_title(int* pos, char* buffer);

int isvariable(const char* string);
int isoperator(const char* string);

void GetVariableValue(void);

DiffNode_t* DiffNewNode(Type_t type, Value_t value, DiffNode_t* left, DiffNode_t* right);
DiffNode_t* DiffNewNodeOP(Operator_val_t val, DiffNode_t* left, DiffNode_t* right);
DiffNode_t* DiffNewNodeVar(const char* d_var);
DiffNode_t* DiffNewNodeNUM(double num);

DiffNode_t* DiffCopyNode(DiffNode_t* node);
DiffNode_t* DifferentExpression(DiffNode_t* node, const char* d_var, int dump_flag);
DiffNode_t* DiffExpressionN(DiffNode_t* root, const char* d_var, int n, int dump_flag);

DiffNode_t* DiffTeylor(DiffNode_t* node, int n, const char* d_var);
DiffNode_t* TeylorCoefCount(DiffNode_t* node, double a, int k, const char* d_var);

#endif