#ifndef _DIFF_DUMP_
#define _DIFF_DUMP_

#include <stdio.h>
#include <stdlib.h>

#include "diff_struct.h"
#include "colors.h"

#define PRINT_IMAGE(...) fprintf(file_dump, __VA_ARGS__)
#define PRINT_HTM(...)   fprintf(file_htm, __VA_ARGS__)

void DiffDumpNode(DiffNode_t* node, FILE* file_dump);
void DiffDumpImage(DiffNode_t* node);
void DiffDump(DiffNode_t* node, const char* text);

void DiffDumpLatexBegin(void);
void DiffDumpLatexEnd(void);

void DiffDumpNodeLatex(DiffNode_t* node);
void DiffDumpLatex(DiffNode_t* node, const char* name);
#endif