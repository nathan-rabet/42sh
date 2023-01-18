#include "../../include/execution.h"

void test_exec_cmd()
{
    char **argv = xmalloc(2, sizeof(char *));
    argv[0] = "echo";
    argv[1] = "2";
    int *error = 0;
    exec_cmd(argv, error);
}

int main(void)
{
    xalloc_init();
    test_exec_cmd();
    xfree_all();
}