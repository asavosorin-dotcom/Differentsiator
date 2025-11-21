#include "diff_optimiz.h"

int nodeisnum(DiffNode_t* node, double num);
void nodenull(DiffNode_t* node);

static bool flag_change = 0;

#define cpy_right      DiffNode_t* node_free = node; \
                       free(node->left);             \
                       node = node->right;           \
                       free(node_free);              \
                       flag_change = 1;              \

#define cpy_left       DiffNode_t* node_free = node; \
                       free(node->right);            \
                       node = node->left;            \
                       free(node_free);              \
                       flag_change = 1;              \

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

DiffNode_t* DiffOptimizNeytralElem(DiffNode_t* node)
{
    if (node->left != NULL)
        node->left = DiffOptimizNeytralElem(node->left);
    
    if (node->right != NULL)
        node->right = DiffOptimizNeytralElem(node->right);

    if (node->left == NULL || node->right == NULL)
        return node;

    if (node->type == OP)
    {
        switch (node->value.oper)
        {
        case MUL:
            if (nodeisnum(node->left, 0) || nodeisnum(node->right, 0))
            {
                nodenull(node);
            }
            
            else if (nodeisnum(node->left, 1))
            {
                cpy_right
            }

            else if (nodeisnum(node->right, 1))
            {
                cpy_left
            }

            return node;
        
        case DIV:
            if (nodeisnum(node->left, 0))
            {
                nodenull(node);
            }

            if (nodeisnum(node->right, 1))
            {
                cpy_left
            }

            return node;
        
        case ADD: case SUB:
            if (nodeisnum(node->left, 0))
            {
                cpy_right
            }

            if (nodeisnum(node->right, 0))
            {                
                cpy_left 
            }
            return node;

        default:
            return node;
        }
    }
}


void DiffOptimiz(DiffNode_t* root)
{
    while (1)
    {
        flag_change = 0;
        DiffOptimizConst(root);
        DiffOptimizNeytralElem(root);

        if (!flag_change)
            break;
    }
}

int nodeisnum(DiffNode_t* node, double num)
{
    if (node->type != NUM)
        return 0;

    if (node->value.num != 0)
        return 0;

    return 1;
}

void nodenull(DiffNode_t* node)
{
    node->type = NUM;
    node->value = {};
    node->value.num = 0;

    free(node->left);
    free(node->right);

    node->left = NULL;
    node->right = NULL;
    
    flag_change = 1;
}