#ifndef CLI_H
#define CLI_H
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct cmd_s
{
	char *name;
	int (*func_ptr)(char **args);
} cmd_t;

#endif /* CLI_H */
