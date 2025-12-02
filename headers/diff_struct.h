#ifndef _DIFF_STRUCT_
#define _DIFF_STRUCT_

#define VAR_CAPASITY  3
#define OP_CAPASITY   15
#define TYPE_CAPASITY 3

#include "work_with_text.h"

enum Type_t         {NUM  = 0, OP = 1, VAR = 2};
enum Operator_val_t {ADD  = 0, 
                     SUB  = 1, 
                     MUL  = 2, 
                     DIV  = 3, 
                     DEG  = 4, 
                     SIN  = 5, 
                     COS  = 6, 
                     LN   = 7, 
                     TAN  = 8, 
                     ASIN = 9, 
                     ACOS = 10, 
                     ATAN = 11, 
                     SINH = 12,
                     COSH = 13, 
                     TANH = 14
                    };
enum Flag_t         {NO = 0, YES = 1};

union Value_t
{
    Operator_val_t oper;
    int            index_var;
    double         num;
};

struct DiffNode_t 
{
    Type_t  type;
    Value_t value;

    DiffNode_t* left;
    DiffNode_t* right;
    DiffNode_t* parent;
};

struct Variable_t
{
    const char* name_var;
    double      value;
    size_t      hash;
    Flag_t      flag; 
};

struct Operator_t
{
    const char* name;
    size_t      hash;
    const char* command_name;
    Operator_val_t code;
};

extern Variable_t  arr_variable [VAR_CAPASITY];
extern Operator_t  arr_operators[OP_CAPASITY];
extern const char* arr_types    [TYPE_CAPASITY];

#define PRINT_ERR(...) printf(RED "%s:%d: ", __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n" RESET);

#endif