#include <assert.h>
#include <math.h>
#include <ctype.h>

#include "calculator.h"

#include "src/log_info/log_errors.h"
#include "src/Generals_func/generals.h"


static double My_quick_pow (const double val, int degree);


static double Expr     (const char **expr_ptr);

static double Term     (const char **expr_ptr);

static double Degree   (const char **expr_ptr);

static double Prim     (const char **expr_ptr);

static double Unary_operation       (const char **expr_ptr);

static int    Check_unary_operation (const char **expr_ptr);

static double Get_num  (const char **expr_ptr);

//=================================================================================================

double Parce_math_expression (const char *expression)
{
    assert (expression != nullptr && "expression is nullptr");
    
    const char *expr_ptr = expression;
    double val = Expr (&expr_ptr);

    assert (*expr_ptr == '\0' && "expression ended with a non-null character");

    return val;
}

//=================================================================================================

static double Expr (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    double val1 = Term (expr_ptr);

    while (**expr_ptr == '+' || **expr_ptr == '-')
    {
        const char operation = **expr_ptr;
        (*expr_ptr)++;

        double val2 = Term (expr_ptr);

        if (operation == '+')
            val1 = val1 + val2;
        else
            val1 = val1 - val2;
    }

    return val1;
}

//=================================================================================================

static double Term (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    double val1 = Degree (expr_ptr);
    
    while (**expr_ptr == '*' || **expr_ptr == '/')
    {
        const char operation = **expr_ptr;
        (*expr_ptr)++;

        double val2 = Degree (expr_ptr);

        if (operation == '*')
            val1 = val1 * val2;
        else
        {
            assert (!Is_zero (val2) && "val2 is zero");
            val1 = val1 / val2;
        }
    }

    return val1;   
}

//=================================================================================================

static double Degree (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    double val = Prim (expr_ptr);
    
    while (**expr_ptr == '^')
    {
        (*expr_ptr)++;

        int degree = (int) Prim (expr_ptr);
        
        val = My_quick_pow (val, degree);
    }

    return val;   
}

//=================================================================================================

static double Prim (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    double val = 0;

    if (**expr_ptr == '(')
    {
        (*expr_ptr)++;
        val = Expr (expr_ptr);

        assert (**expr_ptr == ')' && "expression is not closed by \')\'");
        (*expr_ptr)++;
    }
    
    else if (Check_unary_operation (expr_ptr))
        val = Unary_operation (expr_ptr);
    else
        val = Get_num (expr_ptr);


    return val;
}

//=================================================================================================

static double My_quick_pow (const double val, int degree)
{
    if (degree == 0)
        return 1;
    
    if (degree == 1)
        return val;

    if (degree % 2 == 0)
    {
        double val_ = My_quick_pow (val, degree / 2);
        return val_ * val_;
    }

    else
    {
        double val_ = My_quick_pow (val, degree - 1);
        return val_ * val;
    }
}

//=================================================================================================

static double Unary_operation (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    double val = 0;

    if (!strncmp (*expr_ptr, "sin", 3))
    {
        (*expr_ptr) += 3;

        val = Prim (expr_ptr);
        val = (double) sin (val);
    }

    else if (!strncmp (*expr_ptr, "cos", 3))
    {
        (*expr_ptr) += 3;

        val = Prim (expr_ptr);
        val = (double) cos (val);
    }

    else if (!strncmp (*expr_ptr, "log",  2))
    {
        (*expr_ptr) += 3;

        val = Prim (expr_ptr);
        assert (val > 0 && "val2 is positive number");
        
        val = (double) log (val);
    }

    return val;
}

//=================================================================================================

static int Check_unary_operation (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    if (!strncmp (*expr_ptr, "sin", 3)) return 1;

    if (!strncmp (*expr_ptr, "cos", 3)) return 1;

    if (!strncmp (*expr_ptr, "log", 3)) return 1;

    return 0;
}

//=================================================================================================

static double Get_num (const char **expr_ptr)
{
    assert (expr_ptr != nullptr && "expr_ptr is nullptr");

    int shift = 0;

    double val = 0;

    sscanf (*expr_ptr, "%lg %n", &val, &shift);    

    assert (shift != 0 && "the expr_ptr did not move");

    *expr_ptr += shift;

    return val;
}

//=================================================================================================