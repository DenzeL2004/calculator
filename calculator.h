#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_


int Parce_math_expression (const char *expression);


int Expr (const char **expr_ptr);

int Term (const char **expr_ptr);

int Prim (const char **expr_ptr);

int Get_num (const char **expr_ptr);


#endif  //_CALCULATOR_H_