#include "main.h"

int main(int argc, char* argv[])
{
    DiffDumpLatexBegin();
    
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

    DiffDumpLatex(root, "First expression");

    double x = DiffSolveExpresion(root);
    printf("answer1 = %lf\n", x);

    DiffNode_t* dif_root = DiffExpressionN(root, "x", 2);
    double answer2 = DiffSolveExpresion(dif_root);
    DiffDumpLatexAnswer(dif_root, answer2);

    printf("answer2 = %lf\n", answer2);
    DiffDump(root, "dif_tree");

    DiffNode_t* teylor_node = DiffTeylor(root, 5, "x");

    DiffDumpLatex(teylor_node, "Teylor");
    DiffDump(teylor_node, "Teylor");
    DiffDumpLatexAnswer(teylor_node, DiffSolveExpresion(teylor_node));

    DiffDump(dif_root, "before opt 2");
    dif_root = DiffOptimiz(dif_root);
    DiffDumpLatex(dif_root, "first dif expression");
    DiffDump(dif_root, "123r4");

    DiffPrintNode(dif_root, file_diff);
    DiffDumpLatexEnd();

    DiffDump(root, "First dump");
    DiffDtor(dif_root);
    DiffDtor(teylor_node);
    printf("-------------------------------------------------------\n");
    DiffDtor(root);

    free(code_tree - 1);

    fclose(file_diff);
}

    // free(code_tree - 1);
    // fclose(file_akin);