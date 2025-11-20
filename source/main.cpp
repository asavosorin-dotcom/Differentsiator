#include "main.h"

int main(int argc, char* argv[])
{
    const char* filenameread = argv[1];
    char* code_tree = CreateBuffer(filenameread).buff + 1;
    
    FILE* file_diff = NULL;
 
    switch (argc) //getopts
    {
        case 2:
        {
            file_diff = fopen(argv[1], "w");
            break;
        }
        // 
        case 3:
        {
            file_diff = fopen(argv[2], "w");
            break;
        }

        default:
            return 0;
    }

    int pos = 0;
    DiffNode_t* root = DiffReadNode(&pos, code_tree);

    if (root == NULL)
    {
        DiffDtor(root);
        free(code_tree - 1);
        fclose(file_diff);
        return 0;
    }
    
    GetVariableValue();

    double x = DiffSolveExpresion(root);
    printf("answer1 = %lf\n", x);

    // DiffNode_t* dif_root = DifferentExpression(root, "x");
    // double answer2 = DiffSolveExpresion(dif_root);

    // printf("answer2 = %lf\n", answer2);
    DiffDump(root, "dif_tree");
    DiffDumpLatex(root, "first expression");

    DiffOptimizConst(root);
    DiffDump(root, "after opt 1");

    DiffOptimizNeytralElem(root);
    DiffDump(root, "after opt 2");


    // Value_t root_val = {};
    // root_val.oper = MUL;
    // DiffNode_t* root = DiffNodeCtor(OP, &root_val);

    // Value_t val1 = {};
    // val1.num = 7;
    // root->left = DiffNodeCtor(NUM, &val1);

    // Value_t val2 = {};
    // val2.num = 13;
    // root->right = DiffNodeCtor(NUM, &val2);

    DiffPrintNode(root, file_diff);


    DiffDump(root, "First dump");
    DiffDtor(root);
    // DiffDtor(dif_root);

    free(code_tree - 1);

    fclose(file_diff);
}

    // free(code_tree - 1);
    // fclose(file_akin);