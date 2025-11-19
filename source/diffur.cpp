#include "diffur.h"

DiffNode_t* DiffNodeCtor(Type_t type, Value_t* val)
{
    DiffNode_t* node = (DiffNode_t* ) calloc(1, sizeof(DiffNode_t));
    node->type = type;

    switch (type)
    {
        case OP:
            node->value.oper = val->oper;
            break;
            
        case NUM:
            node->value.num = val->num;
            break;

        case VAR:
            node->value.index_var = val->index_var;
            break;

        default:
            printf(BOLD_RED "This type doesn't exist\n" RESET);
    }

    return node;
}

void DiffDtor(DiffNode_t* node)
{
    if (node->left != NULL)
        DiffDtor(node->left);

    if (node->right != NULL)
        DiffDtor(node->right);
    
    free(node);
}

DiffNode_t* DiffReadNode(int* pos, char* buffer)
{
    // printf(BOLD_BLUE "In beginnig read: [%s]\n" RESET, buffer + *pos);

    static int rank = 0;
    rank++;

    *pos += skip_space(buffer + *pos);

    if (buffer[*pos] == '(')
    {
        // DiffNode_t* node = DiffNodeCtor(); // присобачить в нужное место
        (*pos)++; // пропуск скобки

        char* value_node = Read_title(pos, buffer);
        DiffNode_t* node = DiffNodeMake(value_node);

        *pos += skip_space(buffer + *pos);

        node->left = DiffReadNode(pos, buffer);
        rank--;
        
        node->right = DiffReadNode(pos, buffer);
        rank--;
        
        *pos += skip_space(buffer + *pos);

        if (buffer[*pos] == ')')
            (*pos)++;
            
        // printf("%s\n", buffer + *pos);
        return node;
    }
    else if (buffer[*pos] == 'n' && buffer[*pos + 1] == 'i' && buffer[*pos + 2] == 'l')
    {
        *pos += skip_space(buffer + *pos);
        *pos += strlen("nil");
        // printf("if nil [%s]\n", buffer + *pos);
        return NULL;
    }
    else 
    {
        printf(RED "[%s]\n" RESET, buffer);
        // printf(RED "ERROR format code tree\n" RESET);
        return NULL;
    }
}

DiffNode_t* DiffNodeMake(const char* value_node)
{
    int index = -1;
    Type_t type = NUM;
    Value_t value = {};

    // printf("[%s]", value_node);

    double num = 0;

    char** end_ptr = NULL; //!!!

    if ((num = strtod(value_node, end_ptr)) != 0 || (strcmp(value_node, "0") == 0))
    {
        type = NUM;
        printf("%d\n", num);
        value.num = num;
    }

    else if ((index = isvariable(value_node)) != -1) // дописать функции на проверку является ли строка переменной или оператором
    {
        type = VAR;
        value.index_var = index;
    }
    else if ((index = isoperator(value_node)) != -1)
    {
        type = OP;
        value.oper = arr_operators[index].code;
    }
    else
    {
        printf(BOLD_RED "Wrong format code_tree %s" RESET, value_node);
        return NULL;
    }

    return DiffNodeCtor(type, &value);
}

void DiffPrintNode(const DiffNode_t* node, FILE* file_Diff)
{
    
    fprintf(file_Diff, "(");

    // fprintf(file_Diff, "\"%s\"", );

    switch(node->type)
    {
        case OP:
            fprintf(file_Diff, "\"%s\"", arr_operators[node->value.oper].name);
            break;
        
        case NUM:
            fprintf(file_Diff, "\"%lf\"", node->value.num);
            break;

        case VAR:
            fprintf(file_Diff, "\"%s\"", arr_variable[node->value.index_var].name_var);
            break;

        default:
            printf(BOLD_RED "%s:%d Wrong type %d", __FILE__, __LINE__, node->type);
    }

    if (node->left)
        DiffPrintNode(node->left, file_Diff);
    else
        fprintf(file_Diff, "nil");

    if (node->right)
        DiffPrintNode(node->right, file_Diff);
    else
        fprintf(file_Diff, "nil");

    fprintf(file_Diff, ")");

    fflush(file_Diff);
}

int isvariable(const char* string)
{
    size_t hash_string = CountHash(string);
    
    for (int index = 0; index < VAR_CAPASITY; index++) // бинпоиск на три элемента пока бесполезен 
    {
        if (hash_string == arr_variable[index].hash)
            return index;
    }

    return -1;
}

int isoperator(const char* string)
{
    size_t hash_string = CountHash(string);

    for (int index = 0; index < OP_CAPASITY; index++)
    {
        if (hash_string == arr_operators[index].hash)
            return index;
    }

    return -1;
}

int comporator_var(const void* var1, const void* var2)
{
    if (((const Variable_t*) var1)->hash < ((const Variable_t*) var2)->hash)
        return -1;

    else if (((const Variable_t*) var1)->hash == ((const Variable_t*) var1)->hash)
        return 0;

    else
        return 1;
}

double DiffSolveExpresion(DiffNode_t* root)
{
    switch (root->type)
    {
        case NUM:
            return root->value.num;

        case VAR:
            return arr_variable[root->value.index_var].value;

        case OP:
            double num1 = DiffSolveExpresion(root->left);
            double num2 = DiffSolveExpresion(root->right);

            switch (root->value.oper)
            {
            case ADD:
                return num1 + num2;
            
            case SUB:
                return num1 - num2;

            case MUL:
                return num1 * num2;

            case DIV:
                return num1 / num2;
            default:
                break;
            }
    }
}

char* Read_title(int* pos, char* buffer) // можно считывать double здесь
{
    int len = 0;
    *pos += skip_space(buffer + *pos);

    sscanf(buffer + *pos, " \"%*[^\"]\"%n", &len);
    *(buffer + *pos + len - 1) = '\0';            // меняет вторую кавычку на 0

    (*pos) += len;
    return buffer + *pos - len + 1;
}