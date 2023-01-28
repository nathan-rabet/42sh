#include "evalexpr.h"

// Handles precedence for infix expressions.
// Unary not handled.
void handle_precedence(struct stack **operators, struct list *tokens,
                       struct fifo **output)
{
    // Check if stack is empty.
    if (*operators == NULL)
        return;

    // Create precedence map.
    int precedence[158] = { 0 };
    precedence[ASCII_PLUS] = 1;
    precedence[ASCII_MINUS] = 1;

    precedence[ASCII_MUL] = 2;
    precedence[ASCII_DIV] = 2;
    precedence[ASCII_MOD] = 2;

    precedence[ASCII_POW] = 3;

    precedence[ASCII_PARL] = 5;
    precedence[ASCII_PARR] = 5;

    // Pops operators from stack while precedence bigger than token's.
    int greater = precedence[stack_peek(*operators)] > precedence[tokens->data];
    char oper = stack_peek(*operators);

    while ((*operators != NULL && greater) && is_operator(oper))
    {
        // Push operators of greater precedence to output.
        char token = stack_peek(*operators);
        fifo_push(*output, stack_peek(*operators), token);
        *operators = stack_pop(*operators);

        if (*operators != NULL)
        {
            greater =
                precedence[stack_peek(*operators)] > precedence[tokens->data];
            oper = stack_peek(*operators);
        }
    }
}

// Pops remaining operators.
void pop_remaining(struct stack **operators, struct fifo **output)
{
    // Pop remaining operators (brackets excluded) to queue.
    if (*operators != NULL)
    {
        char token = stack_peek(*operators);
        while (*operators != NULL && is_operator(token))
        {
            fifo_push(*output, stack_peek(*operators), token);
            *operators = stack_pop(*operators);

            if (*operators != NULL)
                token = stack_peek(*operators);
        }
    }
}

// Evaluates (valid) infix expressions.
int evalexpr_infix(struct fifo *expr, int *err)
{
    // Create stack and queue for operations and result.
    struct stack *operators = xcalloc(1, sizeof(struct stack));
    operators = NULL;
    struct fifo *output = fifo_init();

    struct list *tokens = expr->head;

    // Iterate till no tokens in queue.
    while (tokens != NULL)
    {
        // Represents operators, brackets and numbers data.
        int operator= tokens->data;

        // Number case.
        if (tokens->token == '?')
        {
            fifo_push(output, operator, tokens->token);
        }
        else if (is_operator(tokens->token))
        {
            // Operator case.

            handle_precedence(&operators, tokens, &output);

            // Push operator to stack.
            operators = stack_push(operators, operator);
        }
        else if (tokens->token == '(')
        {
            // Left bracket case.
            operators = stack_push(operators, operator);
        }
        else if (tokens->token == ')')
        {
            // Right bracket case.
            while (operators != NULL && (stack_peek(operators) != ASCII_PARL))
            {
                char token = stack_peek(operators);
                fifo_push(output, stack_peek(operators), token);

                operators = stack_pop(operators);
            }

            // Pop left bracket found.
            operators = stack_pop(operators);
        }

        // Move to next token.
        tokens = tokens->next;
    }

    // Pop remaining operators (brackets excluded) to queue.
    pop_remaining(&operators, &output);

    // Run RPN on create output.
    int res = evalexpr_rpn(output, err);

    fifo_destroy(output);

    return res;
}

// Evaluates (valid) rpn expressions.
// Tokens in queue must only be '?' or operators.
// '?; represents a number.
int evalexpr_rpn(struct fifo *expr, int *err)
{
    struct list *tokens = expr->head;
    struct stack *s = xcalloc(1, sizeof(struct stack));
    s = NULL;

    // Iterate expression till operator is found.
    while (tokens != NULL)
    {
        if (tokens->token != '?')
        {
            // Pop operands and push result of operation.
            int rhs = stack_peek(s);
            s = stack_pop(s);
            int lhs = stack_peek(s);
            s = stack_pop(s);

            if ((tokens->token == '/' || tokens->token == '%') && rhs == 0)
            {
                *err = 3;
                return *err;
            }
            s = stack_push(s, calculate(tokens->token, lhs, rhs));
        }
        else
        {
            // Parse number and push to stack.
            s = stack_push(s, tokens->data);
        }

        // Move to next token.
        tokens = tokens->next;
    }

    // Return result.
    int res = stack_peek(s);

    // Free stack.
    s = stack_pop(s);

    // If stack is still not NULL, return error.
    if (s != NULL)
    {
        *err = 2;
        stack_destroy(s);
    }

    return res;
}

// Converts expression into queue.
// - Tokens of value '?' in queue indicate number in current queue element.
// - Token data is the operator if token is an operator or parenthesis.
// - (It can be ignored).
int evalexpr_lexer(char *str, int rpn, struct fifo **expr)
{
    *expr = fifo_init();

    // Error check.
    if (*expr == NULL)
        errx(1, "Malloc failed");

    size_t size = strlen(str);
    size_t i = 0;

    while (i < size)
    {
        // Check if character valid.
        if (!valid(str[i], rpn))
            return 1;

        if (str[i] >= '0' && str[i] <= '9')
        {
            // Parse number and push to queue with.
            int num = parse_number(str, &i);

            fifo_push(*expr, num, '?');
        }
        else if (str[i] == ' ')
        {
            // Skip spaces.
            skip_spaces(str, &i);
        }
        else
        {
            // Save operator token to queue.
            int c = str[i];
            fifo_push(*expr, c, str[i]);
            i++;
        }
    }

    // Parse successful.
    return 0;
}
