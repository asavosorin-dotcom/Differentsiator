#include "diff_optimiz.h"

int nodeisnum(DiffNode_t* node, double num);
void nodenull(DiffNode_t* node);

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
                free(node->left);
                free(node);
                node = node->right;
            }

            else if (nodeisnum(node->right, 1))
            {
                free(node->right);
                free(node);
                node = node->left;
            }

            return node;
        
        case DIV:
            if (nodeisnum(node->left, 0))
            {
                nodenull(node);
            }

            if (nodeisnum(node->right, 1))
            {
                free(node->right);
                free(node);
                node = node->left;
            }

            return node;
        
        case ADD: case SUB:
            if (nodeisnum(node->left, 0))
            {
                free(node->right);
                free(node);
                node = node->left;
            }

            if (nodeisnum(node->right, 1))
            {
                free(node->left);
                free(node);
                node = node->right;
            }
            return node;

        default:
            break;
        }
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
}