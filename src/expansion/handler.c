#include "evalexpr.h"

typedef int (*const operation)(const int, const int);

// Assignment operators.
int add(int lhs, int rhs)
{
    return lhs + rhs;
}

int sub(int lhs, int rhs)
{
    return lhs - rhs;
}

int mul(int lhs, int rhs)
{
    return lhs * rhs;
}

int division(int lhs, int rhs)
{
    return lhs / rhs;
}

int mod(int lhs, int rhs)
{
    return lhs % rhs;
}

int expo(int lhs, int rhs)
{
    int res = 1;

    for (int i = 0; i < rhs; i++)
    {
        res *= lhs;
    }

    return res;
}

// Operations.
static const operation operations[] = {
    ['%' - '%'] = mod, ['*' - '%'] = mul,      ['+' - '%'] = add,
    ['-' - '%'] = sub, ['/' - '%'] = division, ['^' - '%'] = expo,
};

// Launches operation on lhs and rhs.
// Arguments given should be valid.
int calculate(char operator, int lhs, int rhs)
{
    return operations[operator - '%'](lhs, rhs);
}
