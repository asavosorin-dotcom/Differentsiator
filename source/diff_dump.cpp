#include "diff_dump.h"

FILE* file_htm = fopen("Logfile.htm", "w");

static int index_png = 0;

void DiffDumpNode(DiffNode_t* node, FILE* file_dump)
{       
    #define PRINT_NODE_IMAGE(print_type, ...) PRINT_IMAGE("\tnode%p[label = \"{TYPE: %d |VAL: " print_type " | {%p | %p}}\", shape = Mrecord, style = \"filled\", fillcolor = \"#C0FFC0\"]\n", node, node->type, __VA_ARGS__, node->left, node->right);

    switch (node->type)
    {
        case OP:
            PRINT_NODE_IMAGE("%s", arr_operators[node->value.oper].command_name);
            break;
            
        case NUM:
            PRINT_NODE_IMAGE("%lf", node->value.num);
            break;

        case VAR:
            PRINT_NODE_IMAGE("%lf (\'%s\')", arr_variable[node->value.index_var].value, arr_variable[node->value.index_var].name_var);
            break;

        default:
            printf(BOLD_RED "This type doesn't exist\n" RESET);
    }

    if (node->left != NULL) 
    {
        DiffDumpNode(node->left, file_dump);
        PRINT_IMAGE("\tnode%p -> node%p [color = \"blue\"]\n ", node, node->left);
        // index /= 10;
    }
    
    if (node->right != NULL)
    {
        DiffDumpNode(node->right, file_dump);
        // index = (index - 1) / 10;
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