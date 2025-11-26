#include "diffur.h"

DiffNode_t* diff_pow(DiffNode_t* node, const char* d_var);
DiffNode_t* diff_expression(DiffNode_t* node, const char* d_var);

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
    static int counter = 0;
    
    if (node->left != NULL)
        DiffDtor(node->left);

    if (node->right != NULL)
        DiffDtor(node->right);
    
    counter++;
    printf("[%d]\n", counter);
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
        *pos += (int) strlen("nil");
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

    if ((int) (num = strtod(value_node, NULL)) != 0 || (strcmp(value_node, "0") == 0))
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
                {
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
                    return sin(num1);
            
                case COS:
                    return cos(num1);
                    
                case LN:
                    return log(num1);            

                case TAN:
                    return tan(num1);

                case ASIN:
                    return asin(num1);

                case ACOS:
                    return acos(num1);

                case ATAN:
                    return atan(num1);

                default:
                    return 0;
                }
                break;
            }

            default:
                printf(BOLD_RED "Type doesn't exist\n" RESET);
                return 0;
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

DiffNode_t* DiffNewNodeVar(const char* d_var)
{
    size_t hash_var = CountHash(d_var);
    
    for (int i = 0; i < VAR_CAPASITY; i++)
    {
        if (hash_var == arr_variable[i].hash)
        {
            Value_t val = {};
            val.index_var = i;
            DiffNode_t* node = DiffNodeCtor(VAR, &val, NULL);
            return node;
        }
    }

    PRINT_ERR("doesn't exist var %s", d_var);
    return NULL;
}

#define copyLeft   DiffCopyNode(node->left)
#define copyRight  DiffCopyNode(node->right)
#define diffLeft   DifferentExpression(node->left , d_var)
#define diffRight  DifferentExpression(node->right, d_var)

#define DIFF_POW   diff_pow(node, d_var)

#define NUM_(num)                   DiffNewNodeNUM(num)
#define VAR_(d_var)                 DiffNewNodeVar(d_var)
#define ADD_(node_left, node_right) DiffNewNodeOP(ADD, node_left, node_right)
#define SUB_(node_left, node_right) DiffNewNodeOP(SUB, node_left, node_right)
#define MUL_(node_left, node_right) DiffNewNodeOP(MUL, node_left, node_right)
#define DIV_(node_left, node_right) DiffNewNodeOP(DIV, node_left, node_right)
#define COS_(node_left)             DiffNewNodeOP(COS, node_left, NULL)
#define SIN_(node_left)             DiffNewNodeOP(SIN, node_left, NULL)
#define DEG_(node_left, node_right) DiffNewNodeOP(DEG, node_left, node_right)
#define LN_(node_left)              DiffNewNodeOP(LN,  node_left, NULL)
#define SQRT_(node_left)            DiffNewNodeOP(DEG, node_left, NUM_(0.5))
#define SQUAR_(node_left)           DiffNewNodeOP(DEG, node_left, NUM_(2))

DiffNode_t* DiffCopyNode(DiffNode_t* node)
{
    DiffNode_t* new_node = DiffNodeCtor(node->type, &(node->value), node->parent);

    if (node->left != NULL)
        new_node->left = DiffCopyNode(node->left);
    else
        new_node->left = NULL;

    if (node->right != NULL)
        new_node->right = DiffCopyNode(node->right);
    else
        new_node->right = NULL;

    return new_node;
}

// написать функцию для пошагового дампа в дифференцировании 
// объеденить switch case в одну функцию для дампа, чтобы перед общим ретерном поставить дамп в латех

DiffNode_t* DifferentExpression(DiffNode_t* node, const char* d_var)
{
    DiffNode_t* ret_node = diff_expression(node, d_var);
    DiffDumpLatexDDX(node, ret_node, d_var);
    return ret_node;
}

DiffNode_t* diff_expression(DiffNode_t* node, const char* d_var)
{
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
                    return DIV_(SUB_(MUL_(diffLeft, copyRight), MUL_(copyLeft, diffRight)), MUL_(copyRight, copyRight));
                
                case SIN:
                    return MUL_(COS_(copyLeft), diffLeft);
                
                case COS:
                    return MUL_(NUM_(-1), MUL_(SIN_(copyLeft), diffLeft));

                case LN:
                    return MUL_(DIV_(NUM_(1), copyLeft), diffLeft);

                case DEG:
                    return DIFF_POW;

                case TAN:
                    return MUL_(DIV_(NUM_(1), MUL_(COS_(copyLeft), COS_(copyLeft))), diffLeft);

                case ASIN:
                    return MUL_(DIV_(NUM_(1), SQRT_(SUB_(NUM_(1), SQUAR_(copyLeft)))), diffLeft);

                case ACOS:
                    return MUL_(MUL_(NUM_(-1), DIV_(NUM_(1), SQRT_(SUB_(NUM_(1), SQUAR_(copyLeft))))), diffLeft);
             
                case ATAN:
                    return MUL_(DIV_(NUM_(1), ADD_(NUM_(1), SQUAR_(copyLeft))), diffLeft);

                default:
                    break;
            }

            default:
                return node;
    }
}

DiffNode_t* diff_pow(DiffNode_t* node, const char* d_var)
{
    if (node->right->type == NUM)
    {
        return MUL_(diffLeft, MUL_(NUM_(node->right->value.num), DEG_(copyLeft, NUM_(node->right->value.num - 1))));
    }

    if (node->left->type == NUM)
    {
        return MUL_(MUL_(LN_(copyLeft), DEG_(copyLeft, copyRight)), diffRight);
    }

    return MUL_(DEG_(copyLeft, copyRight), ADD_(MUL_(MUL_(DIV_(NUM_(1), copyLeft), diffRight), copyRight), MUL_(LN_(copyLeft), diffRight)));
}

char* Read_title(int* pos, char* buffer) // можно считывать double здесь
{
    int len = 0;
    *pos += skip_space(buffer + *pos);

    sscanf(buffer + *pos, " \"%*[^\"]\"%n", &len);
    *(buffer + *pos + len - 1) = '\0';            

    (*pos) += len;
    return buffer + *pos - len + 1;
}

DiffNode_t* DiffExpressionN(DiffNode_t* root, const char* d_var, int n)
{
    DiffNode_t* node0 = root;
    DiffNode_t* node1 = NULL;

    for (int i = 0; i < n; i++)
    {
            DiffDumpLatex(node0, "Differentiation Expression");
            node1 = DifferentExpression(node0, d_var);
            node1 = DiffOptimiz(node1);
            DiffDumpLatexDDX(node0, node1, d_var);

        if (node0 != root)
        {
            printf("node[%p]\n", node0);
            fflush(stdout);
            DiffDtor(node0);
        }

        node0 = node1;
    }

    return node1;
}

DiffNode_t* DiffTeylor(DiffNode_t* node, int n, const char* d_var)
{
    printf(BOLD_BLUE "Enter a in Teylor: \n" RESET);
    double a = 0;
    scanf("%lg", &a);
    
    DiffNode_t* teylor_node = NUM_(0);
    
    for (int i = 0; i <= n; i++)
    {
        teylor_node = ADD_(teylor_node, MUL_(NUM_(TeylorCoefCount(node, i, d_var)), DEG_(SUB_(VAR_(d_var), NUM_(a)), NUM_(i))));
    }

    teylor_node = DiffOptimiz(teylor_node);
    return teylor_node;
}

double TeylorCoefCount(DiffNode_t* node, int k, const char* d_var)
{
    DiffNode_t* diff_node = DiffExpressionN(node, d_var, k);
    double answer = DiffSolveExpresion(diff_node);

    if (diff_node != NULL) DiffDtor(diff_node);

    return answer / tgamma(k+1);
}

#undef NUM_(num)                   DiffNewNodeNUM(num)
#undef VAR_(d_var)                 DiffNewNodeVar(d_var)
#undef ADD_(node_left, node_right) DiffNewNodeOP(ADD, node_left, node_right)
#undef SUB_(node_left, node_right) DiffNewNodeOP(SUB, node_left, node_right)
#undef MUL_(node_left, node_right) DiffNewNodeOP(MUL, node_left, node_right)
#undef DIV_(node_left, node_right) DiffNewNodeOP(DIV, node_left, node_right)
#undef COS_(node_left)             DiffNewNodeOP(COS, node_left, NULL)
#undef SIN_(node_left)             DiffNewNodeOP(SIN, node_left, NULL)
#undef DEG_(node_left, node_right) DiffNewNodeOP(DEG, node_left, node_right)
#undef LN_(node_left)              DiffNewNodeOP(LN,  node_left, NULL)
#undef SQRT_(node_left)            DiffNewNodeOP(DEG, node_left, NUM_(0.5))
#undef SQUAR_(node_left)           DiffNewNodeOP(DEG, node_left, NUM_(2))