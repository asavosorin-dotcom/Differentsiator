#include "diff_optimiz.h"

DiffNode_t* DiffOptimizConst(DiffNode_t* node)
{
    
    if (node->left != NULL)
        node->left = DiffOptimizConst(node->left);
    
    if (node->right != NULL)
        node->right = DiffOptimizConst(node->right);
    
    if (node->left == NULL || node->right == NULL)
        return node;

    if (node->left->type == NUM && node->right->type == NUM)
    {
        double num = DiffSolveExpresion(node);
        node->type = NUM;
        node->value = {};
        node->value.num = num;

        free(node->left);
        free(node->right);

        node->left = NULL;
        node->right = NULL;
    }

    return node;
}