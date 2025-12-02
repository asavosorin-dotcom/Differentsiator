#ifndef _DIFF_DUMP_
#define _DIFF_DUMP_

#include <stdio.h>
#include <stdlib.h>

#include "diff_struct.h"
#include "colors.h"
#include "diffur.h"

#define PRINT_IMAGE(...) fprintf(file_dump, __VA_ARGS__)
#define PRINT_HTM(...)   fprintf(file_htm, __VA_ARGS__)
#define PRINT_LATEX(...) fprintf(file_latex, __VA_ARGS__)

extern FILE* file_latex;

void DiffDumpNode(DiffNode_t* node, FILE* file_dump);
void DiffDumpImage(DiffNode_t* node);
void DiffDump(DiffNode_t* node, const char* text);

void DiffDumpLatexBegin(void);
void DiffDumpLatexEnd(void);

void DiffDumpLatexAnswer(DiffNode_t* node, double answer);
void DiffDumpLatexDDX(DiffNode_t* node, DiffNode_t* diff_node, const char* d_var);

void DiffDumpNodeLatex(DiffNode_t* node);
void DiffDumpLatex(DiffNode_t* node, const char* name);
void DiffDumpLatexTitle(const char* title);

void MakeDots(DiffNode_t* root, FILE* file_dat, const char* var);
void MakeGraphicFunc(DiffNode_t* root);

void MakeGraphicFullInf(DiffNode_t* root);
void DiffDumpLatexGraphicFullInf(DiffNode_t* root);

#endif