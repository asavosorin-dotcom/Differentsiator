#ifndef _DIFF_READ_INF
#define _DIFF_READ_INF

#include <stdio.h>
#include <math.h>

#include "diff_struct.h"
#include "diffur.h"
#include "diff_dump.h"

#include "work_with_text.h"

DiffNode_t* DiffRead(const char* filename);

#define PRINT_ERR(...) printf(RED "%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n" RESET);

#endif