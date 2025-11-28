#include "diff_read_inf.h"

DiffNode_t* GetNumber     (const char** s);
DiffNode_t* GetP          (const char** s);
DiffNode_t* GetCommand    (const char** s);
DiffNode_t* GetDeg        (const char** s);
DiffNode_t* GetT          (const char** s);
DiffNode_t* GetExpression (const char** s);
DiffNode_t* GetG          (const char** s);

DiffNode_t* DiffRead(const char* filename)
{
    char* buffer = CreateBuffer(filename).buff + 1;
    const char* buf = buffer;

    DiffNode_t* node = GetG(&buf);

    free(buffer - 1);
    return node;
}

DiffNode_t* GetG(const char** s)
{
    DiffNode_t* node = GetExpression(s);
    DiffDump(node, "reading");

    if (**s != '\0')
    {
        PRINT_ERR("Syntax Error\n");
        return NULL;
    }   

    return node;
}

DiffNode_t* GetNumber(const char** s)
{
    double val = 0;
    
    *s += skip_space(*s);

    if (isalpha(**s))
    {
        #define VAR_LENGTH 10
        char var[VAR_LENGTH] = "";

        for (int i = 0; isalpha(**s); i++, (*s)++)
        {
            var[i] = **s;
        }
        
        return DiffNewNodeVar(var);
        #undef VAR_LENGTH
    }

    while (('0' <= **s) && (**s <= '9'))
    {
        val = val * 10 + (**s - '0');
        (*s)++;
    }

    if (**s != '.') 
    {
        DiffNode_t* node1  = DiffNewNodeNUM(val);
        printf("retnode = [%p]\nval = [%lg]\ns = [%s]\n\n---------------------------------------------\n\n", node1, val, *s);
        return node1;
    }

    (*s)++;

    int counter = 0;
    while (('0' <= **s) && (**s <= '9'))
    {
        val = val * 10 + (**s - '0');
        (*s)++;
        counter++;
    }

    for (;counter != 0; counter--)
        val /= 10;

    return DiffNewNodeNUM(val);
}

DiffNode_t* GetP(const char** s)
{
    *s += skip_space(*s);

    if (**s == '(')
    {
        (*s)++;
        *s += skip_space(*s);
        DiffNode_t* node = GetExpression(s);
        *s += skip_space(*s);

        if (**s != ')')
        {
            PRINT_ERR("syntax error\n");
            return NULL;
        }

        (*s)++; 
        *s += skip_space(*s);

        return node;
    }
    else
    {
        return GetNumber(s);
    }
}

DiffNode_t* GetCommand(const char** s)
{
    #define COM_CAPASITY 30
    char command[COM_CAPASITY] = "";

    DiffNode_t* command_node = NULL;

    s += skip_space(*s);

    if (!isalpha(**s))
    {
        command_node = GetP(s);
        return command_node;
    }

    
    for (int i = 0; isalpha(**s); i++, (*s)++)
    {
        if ((i == COM_CAPASITY))
        {
            PRINT_ERR("Command buffer overflow\n");
            return NULL;
        }
        
        command[i] = **s;
    }
    
    
    size_t command_hash = CountHash(command);
    
    int index = 0;

    for (; command_hash != arr_operators[index].hash ; index++)
    {
        if (index == OP_CAPASITY - 1)
        {
            // PRINT_ERR("Unknown command %s\n", command);
            // return NULL;
            *s -= strlen(command);
            (*s)--;
            printf("s = [%s]\n", *s);
            DiffNode_t* node_var = GetP(s); // возможно лучше написать GetVar
            return node_var;
        }
        
    }    
    
    command_node = GetP(s);
    
    return DiffNewNodeOP(arr_operators[index].code, command_node, NULL);

    #undef COM_CAPASITY
}

DiffNode_t* GetDeg(const char** s)
{
    DiffNode_t* command_node = GetCommand(s);

    *s += skip_space(*s);

    while (**s == '^')
    {
        (*s)++;
        DiffNode_t* deg_node = GetCommand(s);

        command_node = DiffNewNodeOP(DEG, command_node, deg_node);

        s += skip_space(*s);
    }

    return command_node;
}

DiffNode_t* GetT(const char** s)
{
    DiffNode_t* node1 = GetDeg(s);

    *s += skip_space(*s);

    while ((**s == '*') || (**s == '/'))
    {
        int op = **s;
        *s += skip_space(*s);
        (*s)++;
        *s += skip_space(*s);
        DiffNode_t* node2 = GetDeg(s);
        *s += skip_space(*s);

        if (op == '*')
            node1 = DiffNewNodeOP(MUL, node1, node2);
        else 
            node1 = DiffNewNodeOP(DIV, node1, node2);

    }

    return node1;
}

DiffNode_t* GetExpression(const char** s)
{
    DiffNode_t* node1 = GetT(s);

    *s += skip_space(*s);

    DiffDump(node1, "expression");

    while (**s == '+' || **s == '-')
    {
        int op = **s;
        fflush(stdout);
        *s += skip_space(*s);
        (*s)++;
        *s += skip_space(*s);
        DiffNode_t* node2 = GetT(s);
        *s += skip_space(*s);

        if (op == '+')
            node1 = DiffNewNodeOP(ADD, node1, node2);
        else
            node1 = DiffNewNodeOP(SUB, node1, node2);
    }

    DiffDump(node1, "expression after +");
    return node1;
}