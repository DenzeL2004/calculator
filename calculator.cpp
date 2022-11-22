#include <assert.h>
#include <ctype.h>

#include "calculator.h"

#include "src/log_info/log_errors.h"
#include "src/Generals_func/generals.h"


static int My_quick_pow (const int val, int degree);

//=================================================================================================

int Parce_math_expression (const char *expression)
{
    assert (expression != nullptr && "expression is nullptr");
    
    const char *expr_ptr = expression;
    int val = Expr (&expr_ptr);

    assert (*expr_ptr == '\0' && "expression ended with a non-null character");

    return val;
}

//=================================================================================================

int Expr (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    int val1 = Term (expr_ptr);

    while (**expr_ptr == '+' || **expr_ptr == '-')
    {
        const char operation = **expr_ptr;
        (*expr_ptr)++;

        int val2 = Term (expr_ptr);

        if (operation == '+')
            val1 = val1 + val2;
        else
            val1 = val1 - val2;
    }

    return val1;
}

//=================================================================================================

int Term (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    int val1 = Prim (expr_ptr);
    
    while (**expr_ptr == '*' || **expr_ptr == '/')
    {
        const char operation = **expr_ptr;
        (*expr_ptr)++;

        int val2 = Prim (expr_ptr);

        if (operation == '*')
            val1 = val1 * val2;
        else
            val1 = val1 / val2;
    }

    return val1;   
}

//=================================================================================================

int Prim (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    int val = 0;

    if (**expr_ptr == '(')
    {
        (*expr_ptr)++;
        val = Expr (expr_ptr);

        assert (**expr_ptr == ')' && "expression is not closed by \')\'");
        (*expr_ptr)++;
    }
    else
        val = Get_num (expr_ptr);

    if (**expr_ptr == '^')
    {
        (*expr_ptr)++;
        int degree = Prim (expr_ptr);

        val = My_quick_pow (val, degree);
    }

    return val;
}

//=================================================================================================

static int My_quick_pow (const int val, int degree)
{
    if (degree == 0)
        return 1;
    
    if (degree == 1)
        return val;

    if (degree % 2 == 0)
    {
        int val_ = My_quick_pow (val, degree / 2);
        return val_ * val_;
    }

    else
    {
        int val_ = My_quick_pow (val, degree - 1);
        return val_ * val;
    }
}

//=================================================================================================

int Get_num (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    int sign = 1;

    if (**expr_ptr == '-')
    {
        sign = -1;
        (*expr_ptr)++;
    }

    int val = 0;

    const char *ptr_old = *expr_ptr;

    while (isdigit (**expr_ptr))
    {
        val = val * 10 + (**expr_ptr - '0');
        (*expr_ptr)++;
    }

    assert (*expr_ptr > ptr_old && "the expr_ptr did not move");

    return val * sign;
}

//=================================================================================================