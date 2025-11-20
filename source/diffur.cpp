#include "diffur.h"

DiffNode_t* DiffNodeCtor(Type_t type, Value_t* val, DiffNode_t* parent)
{
    DiffNode_t* node = (DiffNode_t* ) calloc(1, sizeof(DiffNode_t));
    node->type = type;
    node->value = *val;

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

    node->left  = NULL;
    node->right = NULL;
    node->parent = parent;

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

    *pos += skip_space(buffer + *pos);

    if (buffer[*pos] == '(')
    {
        (*pos)++; // пропуск скобки

        char* value_node = Read_title(pos, buffer);
        DiffNode_t* node = DiffNodeMake(value_node);

        if (node == NULL)
        {
            return NULL;
        }

        *pos += skip_space(buffer + *pos);

        node->left = DiffReadNode(pos, buffer);
        
        if (node->left != NULL)
            node->left->parent = node;

        node->right = DiffReadNode(pos, buffer);
        
        if (node->right != NULL)
            node->right->parent = node;

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
        printf(RED "[%s]\n" RESET, buffer + *pos);
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

    if ((num = strtod(value_node, NULL)) != 0 || (strcmp(value_node, "0") == 0))
    {
        type = NUM;
        printf("%lf\n", num);
        value.num = num;
    }

    else if ((index = isvariable(value_node)) != -1)
    {
        type = VAR;
        value.index_var = index;
        arr_variable[index].flag =  YES;
    }
    else if ((index = isoperator(value_node)) != -1)
    {
        type = OP;
        value.oper = arr_operators[index].code;
    }
    else
    {
        printf(BOLD_RED "Wrong format code_tree %s\n" RESET, value_node);
        fflush(stdout);
        return NULL;
    }

    return DiffNodeCtor(type, &value, NULL);
}

void GetVariableValue(void)
{
    double num = 0;
    for (int i = 0; i < VAR_CAPASITY; i++)
    {
        if (arr_variable[i].flag)
        {
            printf("Enter value %s: ", arr_variable[i].name_var);
            scanf("%lf", &num);
            arr_variable[i].value = num;
        }
    }
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
    if (root == NULL)
    {
        // PRINT_ERR("null pointer node")
        return 0;
    }
    
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
                return num1 / num2; // 0

            case DEG:
                return pow(num1, num2);

            case SIN:
                return sin(num2);
        
            case COS:
                return cos(num2);

            default:
                break;
            }
    }
}

DiffNode_t* DiffNewNodeNUM(double num)
{
    Value_t value = {};
    value.num = num;
    DiffNode_t* node = DiffNodeCtor(NUM, &value, NULL);
    return node;
}

DiffNode_t* DiffNewNodeOP(Operator_val_t val, DiffNode_t* left, DiffNode_t* right)
{
    Value_t value = {};
    value.oper = val;
    DiffNode_t* node = DiffNodeCtor(OP, &value, NULL);

    node->left = left;
    if (node->left != NULL)
        node->left->parent = node;

    node->right = right;
    if (node->right != NULL)
        node->right->parent = node;

    return node;
}

#define copyLeft   DiffCopyNode(node->left)
#define copyRight  DiffCopyNode(node->right)
#define diffLeft   DifferentExpression(node->left , d_var)
#define diffRight  DifferentExpression(node->right, d_var)

#define NUM_(num)                   DiffNewNodeNUM(num)
#define ADD_(node_left, node_right) DiffNewNodeOP(ADD, node_left, node_right)
#define SUB_(node_left, node_right) DiffNewNodeOP(SUB, node_left, node_right)
#define MUL_(node_left, node_right) DiffNewNodeOP(MUL, node_left, node_right)
#define DIV_(node_left, node_right) DiffNewNodeOP(DIV, node_left, node_right)
#define SIN_(node_right)            DiffNewNodeOP(SIN, NULL, node_right)
#define COS_(node_right)            DiffNewNodeOP(COS, NULL, node_right)

DiffNode_t* DiffCopyNode(DiffNode_t* node)
{
    DiffNode_t* new_node = DiffNodeCtor(node->type, &(node->value), node->parent);

    if (node->left != NULL)
        new_node->left = DiffCopyNode(node->left);
    
    if (node->right != NULL)
        new_node->right = DiffCopyNode(node->right);

    return new_node;
}

DiffNode_t* DifferentExpression(DiffNode_t* node, const char* d_var)
{
    Value_t val = {};
    switch (node->type)
    {
        case NUM:
            return NUM_(0);

        case VAR:
        {
            int index_var = isvariable(d_var);
            double num = (index_var == node->value.index_var) ? 1 : 0;
            return NUM_(num);
        }

        case OP:
            switch (node->value.oper)
            {
                case ADD:
                    return ADD_(diffLeft, diffRight);

                case SUB:
                    return SUB_(diffLeft, diffRight);

                case MUL:
                    return ADD_(MUL_(diffLeft, copyRight), MUL_(copyLeft, diffRight));

                case DIV:
                    return DIV_(SUB_(MUL_(diffLeft, copyRight), MUL_(copyRight, diffLeft)), MUL_(copyRight, copyRight));
                
                case SIN:
                    return MUL_(COS_(copyRight), diffRight);
                
                case COS:
                    return MUL_(NUM_(-1), MUL_(SIN_(copyRight), diffRight));;
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
    printf(GREEN "[%s]\n" RESET, buffer + *pos);
    return buffer + *pos - len + 1;
}