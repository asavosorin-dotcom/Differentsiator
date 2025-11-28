#ifndef _DIFF_OPTIMIZ_
#define _DIFF_OPTIMIZ_

#include <stdio.h>

#include "diff_struct.h"
#include "diffur.h"

DiffNode_t* DiffOptimizConst       (DiffNode_t* node);
DiffNode_t* DiffOptimizNeytralElem (DiffNode_t* node);
DiffNode_t* DiffOptimiz            (DiffNode_t* root);

#endif