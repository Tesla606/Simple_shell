#include "shell.h"

void prompt(void)
{
    write(STDOUT_FILENO, "#cisfun$ ", sizeof("#cisfun$ ") - 1);
}
