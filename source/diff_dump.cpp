#include "diff_dump.h"

FILE* file_htm   = fopen("Logfile.htm"   , "w");
FILE* file_latex = fopen("dump_latex.tex", "w");

static int index_png = 0;

void DiffDumpNode(DiffNode_t* node, FILE* file_dump)
{       
    #define PRINT_NODE_IMAGE(print_type, color, ...) PRINT_IMAGE("\tnode%p[label = \"{TYPE: %s |VAL: " print_type " | {%p | %p}}\", shape = Mrecord, style = \"filled\", fillcolor = " color "]\n", node, arr_types[node->type] , __VA_ARGS__, node->left, node->right);

    // добавить разные формы и цвета для разных типов ячеек 

    switch (node->type)
    {
        case OP:
            PRINT_NODE_IMAGE("%s", "\"#5f5fffff\"", arr_operators[node->value.oper].command_name);
            break;
            
        case NUM:
            PRINT_NODE_IMAGE("%lf", "\"#0CFF0C\"", node->value.num);
            break;

        case VAR:
            PRINT_NODE_IMAGE("%lf (\'%s\')", "\"#FF0C0C\"", arr_variable[node->value.index_var].value, arr_variable[node->value.index_var].name_var);
            break;

        default:
            printf(BOLD_RED "This type doesn't exist\n" RESET);
    }

    if (node->left != NULL) 
    {
        DiffDumpNode(node->left, file_dump);
        PRINT_IMAGE("\tnode%p -> node%p [color = \"blue\"]\n ", node, node->left);
    }
    
    if (node->right != NULL)
    {
        DiffDumpNode(node->right, file_dump);
        PRINT_IMAGE("\tnode%p -> node%p [color = \"red\"]\n", node, node->right);
    }
    
    #undef PRINT_NODE_IMAGE
}

void DiffDumpImage(DiffNode_t* node)
{
    
    const char* filename = "Diff_dump.txt";
    FILE* file_dump = fopen(filename, "w");
    
    PRINT_IMAGE("digraph {\n");
    DiffDumpNode(node, file_dump);

    PRINT_IMAGE("}");
    
    char command[100] = "";
    fclose(file_dump);
    
    sprintf(command, "dot \"%s\" -T png -o pictures/graph%d.png", filename, index_png);
    // printf("command = %s\n", command);
    system(command);

    index_png++;
}

void DiffDump(DiffNode_t* node, const char* text)
{
    PRINT_HTM("<pre>\n");
    PRINT_HTM("\t<h3>DUMP %s</h3>\n", text);
    
    DiffDumpImage(node);
    
    PRINT_HTM("Image: \n <img src= \"pictures/graph%d.png\">", index_png - 1);
    PRINT_HTM("</pre>");
    
    fflush(file_htm);
}

#define PRINT_LATEX(...) fprintf(file_latex, __VA_ARGS__)

void DiffDumpNodeLatex(DiffNode_t* node)
{
    if (node->type == OP)
    {
        if (node->value.oper > DEG)
        {
            PRINT_LATEX("%s (", arr_operators[node->value.oper].name);
        }
    }
    
    if (node->value.oper == DIV)
        PRINT_LATEX("\\frac");
    
    if (node->parent != NULL)
    {
        if (node->parent->type == OP && node->type == OP)
        {
            if ((((node->parent->value.oper == MUL) || (node->parent->value.oper == DIV) || (node->parent->value.oper == DEG)) && ((node->value.oper == ADD) || (node->value.oper == SUB))) || ((node->parent->value.oper == SIN) || (node->parent->value.oper == COS)))
            {
                PRINT_LATEX("(");
            }
        }
    }
    
    if (node->left != NULL)
        {
            if ((node->value.oper == DIV) || (node->value.oper == DEG))
                PRINT_LATEX("{");

            DiffDumpNodeLatex(node->left);

            if ((node->value.oper == DIV) || (node->value.oper == DEG))
                PRINT_LATEX("}");
        }
        
    switch (node->type)
    {
    case NUM:
        PRINT_LATEX(" %lg", node->value.num);
        break;
    
    case VAR:
        PRINT_LATEX(" %s", arr_variable[node->value.index_var].name_var);
        break;

    case OP:
        switch (node->value.oper)
        {
        case DIV:
            break;

        case ADD: case SUB: case MUL: case DEG:
            PRINT_LATEX(" %s", arr_operators[node->value.oper].name);
            break;

        default:
            PRINT_LATEX(")");
            return;
        }
    
        break;
    default:
        break;
    }

    if (node->right != NULL)
        {
            if ((node->value.oper == DIV) || (node->value.oper == DEG))
                PRINT_LATEX("{");

            DiffDumpNodeLatex(node->right);

            if ((node->value.oper == DIV) || (node->value.oper == DEG))
                PRINT_LATEX("}");
        }

    if (node->parent != NULL)
    {
        if (node->parent->type == OP && node->type == OP)
        {
            if ((((node->parent->value.oper == MUL) || (node->parent->value.oper == DIV)  || (node->parent->value.oper == DEG)) && (node->value.oper == ADD || node->value.oper == SUB)) || (node->parent->value.oper == SIN || node->parent->value.oper == COS))
            {   
                PRINT_LATEX(")");
            }

        }

    }

}

void DiffDumpLatexAnswer(DiffNode_t* node, double answer)
{
    PRINT_LATEX("\\subsection{}\n");
    PRINT_LATEX("\\Large\n");

    PRINT_LATEX("При ");
    
    for (int i = 0; i < VAR_CAPASITY; i++)
    {
        if (arr_variable[i].flag)
        {
            PRINT_LATEX("%s = %lg", arr_variable[i].name_var, arr_variable[i].value);
        }
    }
    PRINT_LATEX("\n");
    PRINT_LATEX("\\newline");
    
    PRINT_LATEX("$");
    DiffDumpNodeLatex(node);
    PRINT_LATEX("$");
    PRINT_LATEX(" = %lg\n", answer);
}

void DiffDumpLatexDDX(DiffNode_t* node, DiffNode_t* diff_node, const char* d_var)
{
    PRINT_LATEX("\\subsection{}\n");
    PRINT_LATEX("\\Large\n");
    PRINT_LATEX("$");
    PRINT_LATEX("\\frac{d}{d%s}(", d_var);
    DiffDumpNodeLatex(node);
    PRINT_LATEX(") = ");
    DiffDumpNodeLatex(diff_node);
    PRINT_LATEX("$\n");
}

void DiffDumpLatex(DiffNode_t* node, const char* name) // дописать по возможности
{
    PRINT_LATEX("\\section{%s}\n", name);
    PRINT_LATEX("\\Large\n");
    PRINT_LATEX("$");
    DiffDumpNodeLatex(node);
    PRINT_LATEX("$\n");
}

void DiffDumpLatexBegin(void)
{
    PRINT_LATEX("\\documentclass{article}\n");
    PRINT_LATEX("\\title{DEBUG!!!}\n");
    PRINT_LATEX("\\begin{document}\n");
    PRINT_LATEX("\\maketitle\n");
}

void DiffDumpLatexEnd(void)
{
    PRINT_LATEX("\\end{document}");
    fflush(file_latex);
}

#undef PRINT_LATEX