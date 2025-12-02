#include "diff_dump.h"

FILE* file_htm   = fopen("Logfile.htm"   , "w");
FILE* file_latex = fopen("dump_latex.tex", "w");

static int index_png = 0;

void DiffDumpNode(DiffNode_t* node, FILE* file_dump)
{       
    #define PRINT_NODE_IMAGE(print_type, color, ...) PRINT_IMAGE("\tnode%p[label = \"{parent: %p | TYPE: %s |VAL: " print_type " | {%p | %p}}\", shape = Mrecord, style = \"filled\", fillcolor = " color "]\n", node, node->parent, arr_types[node->type] , __VA_ARGS__, node->left, node->right);

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

    fflush(stdout);

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
    
    if (node->type == OP)
    {
        if (node->value.oper == DIV)
            PRINT_LATEX("\\frac");
    }
    
    if (node->parent != NULL)
    {
        if (node->parent->type == OP && node->type == OP)
        {
            if ((((node->parent->value.oper == MUL) 
                || (node->parent->value.oper == DIV) 
                || (node->parent->value.oper == DEG)) 
                && ((node->value.oper == ADD) 
                || (node->value.oper == SUB))) 
                || ((node->parent->value.oper == SIN) || (node->parent->value.oper == COS)))
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

        case ADD: 
        [[fallthrough]];
        case SUB: 
        case MUL: 
        case DEG:
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
    PRINT_LATEX("\\subsubsection{}\n");
    PRINT_LATEX("\\Large\n");
    PRINT_LATEX("$");
    PRINT_LATEX("\\frac{d}{d%s}(", d_var);
    DiffDumpNodeLatex(node);
    PRINT_LATEX(") = ");
    DiffDumpNodeLatex(diff_node);
    PRINT_LATEX("$\n");
}

void DiffDumpLatex(DiffNode_t* node, const char* name) 
{
    PRINT_LATEX("\\subsection{%s}\n", name);
    PRINT_LATEX("\\Large\n");
    if (node != NULL)
    {
        PRINT_LATEX("$");
        DiffDumpNodeLatex(node);
        PRINT_LATEX("$\n");
    }
}

void DiffDumpLatexTitle(const char* title)
{
    PRINT_LATEX("\\section{%s}\n", title);
    PRINT_LATEX("\\Large\n");
    PRINT_LATEX("\n");
}

void DiffDumpLatexBegin(void)
{    
    PRINT_LATEX("\\documentclass{article}\n");
    PRINT_LATEX("\\usepackage[utf8]{inputenc}\n");
    PRINT_LATEX("\\usepackage[T2A]{fontenc}\n");
    PRINT_LATEX("\\usepackage{graphicx}\n");
    PRINT_LATEX("\\title{DEBUG!!!}\n");
    PRINT_LATEX("\\begin{document}\n");
    PRINT_LATEX("\\maketitle\n");
}

void DiffDumpLatexEnd(void)
{
    PRINT_LATEX("\\end{document}");
    fflush(file_latex);
    system("pdflatex dump_latex");

}

#define PRINT_DAT(...) fprintf(file_dat, __VA_ARGS__);

void MakeDots(DiffNode_t* root, FILE* file_dat, const char* var)
{
    int var_index = isvariable(var);
 
    double x = arr_variable[var_index].value;

    for (double val = 0; val < 10; val += 0.001)
    {
        arr_variable[var_index].value = val;
        double y = DiffSolveExpresion(root);
        PRINT_DAT("%lg %lg\n", val, y);
    }

    PRINT_DAT("\n\n\n");

    arr_variable[var_index].value = x;
}

#define PRINT_SCRIPT(...) fprintf(script_file, __VA_ARGS__)

void MakeGraphicFunc(DiffNode_t* root)
{
    const char* script_name = "script.txt";
    FILE* script_file = fopen(script_name, "w");

    PRINT_SCRIPT("set terminal pdf\n");
    PRINT_SCRIPT("set output \"graph.pdf\"\n");
    PRINT_SCRIPT("set title \"f(x)\"\n");
    PRINT_SCRIPT("set xlabel \"X\"\n");
    PRINT_SCRIPT("set ylabel \"Y\"\n");
    PRINT_SCRIPT("set grid\n");
    PRINT_SCRIPT("set xrange [0:10]\n");
    PRINT_SCRIPT("set yrange [-3:3]\n");

    FILE* file_graph = fopen("graphic.txt", "w");
    MakeDots(root, file_graph, "x");

    PRINT_SCRIPT("plot \"graphic.txt\" with linespoints lt rgb \"red\" pt 0 ps 0.5");

    char command[50] = "gnuplot script.txt";
    
    fclose(script_file);
    system(command);
}

void MakeGraphicFullInf(DiffNode_t* root)
{
    const char* script_name = "script.txt";
    FILE* script_file = fopen(script_name, "w");

    PRINT_SCRIPT("set terminal pdf\n");
    PRINT_SCRIPT("set output \"graph_full.pdf\"\n");
    PRINT_SCRIPT("set title \"Full inf graph\"\n");
    PRINT_SCRIPT("set xlabel \"X\"\n");
    PRINT_SCRIPT("set ylabel \"Y\"\n");
    PRINT_SCRIPT("set grid\n");
    PRINT_SCRIPT("set xrange [0:10]\n");
    PRINT_SCRIPT("set yrange [-10:10]\n");

    FILE* file_graph = fopen("graphic.txt", "w");
    MakeDots(root, file_graph, "x");

    DiffDumpLatex(NULL, "Возьмем производную от уравнения");
    DiffNode_t* diff_root = DifferentExpression(root, "x");
    MakeDots(diff_root, file_graph, "x");

    DiffDumpLatex(NULL, "Разложим функцию в ряд Тейлора для графика");
    DiffNode_t* teylor_node = DiffTeylor(root, 5, "x");
    MakeDots(teylor_node, file_graph, "x");

    PRINT_SCRIPT("plot \"graphic.txt\" index 0 with linespoints lt rgb \"red\" pt 0 ps 0.5 title \"function\", \\\n");
    PRINT_SCRIPT("\"graphic.txt\" index 1 with linespoints lt rgb \"green\" pt 0 ps 0.5 title \"derivative\", \\\n");
    PRINT_SCRIPT("\"graphic.txt\" index 2 with linespoints lt rgb \"blue\" pt 0 ps 0.5 title \"Teylor func\"\n");

    char command[50] = "gnuplot script.txt";
    
    DiffDtor(diff_root);
    DiffDtor(teylor_node);

    fclose(script_file);
    system(command);
}

void DiffDumpLatexGraphicFullInf(DiffNode_t* root)
{
    MakeGraphicFullInf(root);

    PRINT_LATEX("\\section{Graphik}");

    PRINT_LATEX("\\begin{figure}[h]\n"
    "\\centering \n"
    "\\includegraphics[width=1\\textwidth]{graph_full.pdf} \n"
    "\\caption{Full inf graphic} \n"
    "\\label{fig:my_image} \n"
    "\\end{figure}");
}

#undef PRINT_DAT
#undef PRINT_LATEX