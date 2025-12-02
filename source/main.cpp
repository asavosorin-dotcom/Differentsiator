#include "main.h"

int main(int argc, char* argv[])
{
    DiffDumpLatexBegin();
    
    const char* filename_read = argv[1];
    // char* code_tree = CreateBuffer(filenameread).buff + 1;
    
    // int pos = 0;
    // DiffNode_t* root = DiffReadNode(&pos, code_tree);

    DiffNode_t* root = DiffRead(filename_read);
    
    GetVariableValue();

    DiffDumpLatexTitle("Начало");
    DiffDumpLatex(root, "Данная функция");

    double answer1 = DiffSolveExpresion(root);

    DiffDumpLatexAnswer(root, answer1);

    printf("answer1 = %lg\n", answer1);

    // MakeGraphicFunc(root);

    
    DiffDumpLatexTitle("Возьмем 3-ю производную");
    DiffNode_t* dif_root = DiffExpressionN(root, "x", 3);
    double answer2 = DiffSolveExpresion(dif_root);
    
    DiffDumpLatexAnswer(dif_root, answer2);
    
    printf("answer2 = %lg\n", answer2);
    
    DiffDumpLatexTitle("Разложим функцию в ряд Тейлора");
    DiffNode_t* teylor_node = DiffTeylor(root, 5, "x");
    
    DiffDumpLatex(teylor_node, "Окончательно");
    // DiffDump(teylor_node, "Teylor");
    DiffDumpLatexAnswer(teylor_node, DiffSolveExpresion(teylor_node));
    
    DiffDumpLatexTitle("Построим график функции");
    DiffDumpLatexGraphicFullInf(root);
    
    DiffDumpLatexEnd();
    
    DiffDump(root, "First dump");
    DiffDtor(dif_root);
    DiffDtor(teylor_node);
    DiffDtor(root);
}

    // free(code_tree - 1);
    // fclose(file_akin);
