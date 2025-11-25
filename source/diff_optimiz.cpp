#include "diff_optimiz.h"

static bool flag_change = 0;
int nodeisnum(DiffNode_t* node, double num);
void nodenull(DiffNode_t* node);

DiffNode_t* cpy_left(DiffNode_t* node)
{
    DiffNode_t* node_ret = node->left;
    node->left->parent = node->parent;
    printf("free node[%p]\nfree node[%p]\n", node, node->right);
    free(node->right);             
    free(node);              
    flag_change = 1;      
    
    return node_ret;
}

DiffNode_t* cpy_right(DiffNode_t* node)
{
    DiffNode_t* node_ret = node->right;
    node->right->parent = node->parent;
    printf("free node[%p]\nfree node[%p]\n", node->left, node);
    free(node->left);             
    free(node);              
    flag_change = 1;
    
    return node_ret;
}


DiffNode_t* DiffOptimizConst(DiffNode_t* node)
{
    // printf(":%s:%d node = [%p]\n", __FILE__, __LINE__, node);

    // DiffDump(node, "before");

    fflush(stdout);

    if (node->left != NULL)
        node->left = DiffOptimizConst(node->left);
    
    if (node->right != NULL)
        node->right = DiffOptimizConst(node->right);
    
    if (node->left == NULL || node->right == NULL)
        return node;

    if (node->left->type == NUM && node->right->type == NUM)
    {
        flag_change = 1;
        double num = DiffSolveExpresion(node);

        DiffDump(node, "node opt");

        node->type = NUM;
        node->value = {};
        node->value.num = num;

        // printf(BOLD_RED "SADLVBHLV\n" RESET);
        // fflush(stdout);
        printf("free node[%p]\nfree node[%p]\n", node->left, node->right);
        free(node->left);
        free(node->right);

        node->left = NULL;
        node->right = NULL;
        
        // printf(":%s:%d node->left = [%p]\n", __FILE__, __LINE__, node->left);
        // printf(":%s:%d node->right = [%p]\n", __FILE__, __LINE__, node->right);

        DiffDump(node->parent, "node opt");
    }

    // DiffDump(node, "opt");

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
                // DiffDump(node->parent, "node null");
            }
            
            else if (nodeisnum(node->left, 1))
            {
                node = cpy_right(node);
            }

            else if (nodeisnum(node->right, 1))
            {
                node = cpy_left(node);
            }

            return node;
        
        case DIV:
            if (nodeisnum(node->left, 0))
            {
                nodenull(node);
            }

            else if (nodeisnum(node->right, 1))
            {
                node = cpy_left(node);
            }

            return node;
        
        case ADD: 
            if (nodeisnum(node->left, 0))
            {
                node = cpy_right(node);
                return node;
            }

        case SUB:
            // DiffDump(node, "SUBBBB");
            // printf("%p\n", node->left);
            // fflush(stdout);


            if (nodeisnum(node->right, 0))
            {                
                printf("node[%p] before\n", node);
                node = cpy_left(node);
                printf("node[%p] after\n", node);
            }

            return node;

        default:
            return node;
        }
    }
}


DiffNode_t* DiffOptimiz(DiffNode_t* root)
{
    while (1)
    {
        flag_change = 0;
        root = DiffOptimizConst(root);
        DiffDump(root, "after const");
        root = DiffOptimizNeytralElem(root);
        DiffDump(root, "after neytral");

        if (!flag_change)
            return root;
    }
}

int nodeisnum(DiffNode_t* node, double num)
{
    // printf("%p\n", node);
    
    if (node->type != NUM)
        return 0;

    if (node->value.num != num)
        return 0;

    return 1;
}

void nodenull(DiffNode_t* node)
{
    node->type = NUM;
    node->value = {};
    node->value.num = 0;

    printf("free node[%p]\nfree node[%p]\n", node->left, node->right);

    DiffDtor(node->left);
    DiffDtor(node->right);

    node->left = NULL;
    node->right = NULL;
    
    flag_change = 1;
}
