#ifndef BUILTINS_H_
#define BUILTINS_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Structure for echo options.
struct echo_options
{
    int flag_n;
    // Interpretation of '\n', '\t' and '\\' is off by default.
    int flag_e;
};

void my_echo(int argc, char **argv);
void my_cd(char **argv);

#endif /* !BUILTINS_H_ */