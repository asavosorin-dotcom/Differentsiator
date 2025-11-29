#include "main.h"

int main(int argc, char* argv[])
{
    DiffDumpLatexBegin();
    
    const char* filename_read = argv[1];
    // char* code_tree = CreateBuffer(filenameread).buff + 1;
    
    // int pos = 0;
    // DiffNode_t* root = DiffReadNode(&pos, code_tree);

    DiffNode_t* root = DiffRead(filename_read);

    if (root == NULL)
    {
        DiffDtor(root);
        return 0;
    }
    
    GetVariableValue();

    DiffDumpLatex(root, "First expression");

    double x = DiffSolveExpresion(root);
    printf("answer1 = %lg\n", x);

    MakeGraphicFunc(root);

    MakeGraphicFullInf(root);

    // написать графики


    DiffNode_t* dif_root = DiffExpressionN(root, "x", 1);
    double answer2 = DiffSolveExpresion(dif_root);

    // DiffDump(dif_root, "before latex");
    DiffDumpLatex(dif_root, "in main");

    DiffDumpLatexAnswer(dif_root, answer2);

    printf("answer2 = %lg\n", answer2);
    // DiffDump(root, "dif_tree");

    // DiffNode_t* teylor_node = DiffTeylor(root, 5, "x");

    // DiffDumpLatex(teylor_node, "Teylor");
    // DiffDump(teylor_node, "Teylor");
    // DiffDumpLatexAnswer(teylor_node, DiffSolveExpresion(teylor_node));  

    // DiffDump(dif_root, "before opt 2");
    // dif_root = DiffOptimiz(dif_root);
    // DiffDumpLatex(dif_root, "first dif expression");
    // DiffDump(dif_root, "123r4");

    DiffDumpLatexEnd();

    DiffDump(root, "First dump");
    DiffDtor(dif_root);
    // DiffDtor(teylor_node);
    printf("-------------------------------------------------------\n");
    DiffDtor(root);
}

    // free(code_tree - 1);
    // fclose(file_akin);
