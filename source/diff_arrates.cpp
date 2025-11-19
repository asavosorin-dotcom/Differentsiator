#include "diff_struct.h"

Operator_t arr_operators[OP_CAPASITY] = {
                                         "+", CountHash("+"), "ADD", ADD,
                                         "-", CountHash("-"), "SUB", SUB,
                                         "*", CountHash("*"), "MUL", MUL,
                                         "/", CountHash("/"), "DIV", DIV
                                        };

Variable_t arr_variable[VAR_CAPASITY] = { // узнать у Санька
                        "x", 0, CountHash("x"),
                        "y", 0, CountHash("y"),
                        "z", 0, CountHash("z"),
};