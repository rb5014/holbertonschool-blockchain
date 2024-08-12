#ifndef CLI_H
#define CLI_H
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../crypto/hblk_crypto.h"

#define DEFAULT_WALLET_PATH "./wallet"
#define LAST_WALLET_PATH_FILE "./last_wallet_path.txt"
typedef struct cmd_s
{
	char *name;
	int (*func_ptr)(char **args);
} cmd_t;


void cli_loop(void);
char **cli_split_line(char *line);
int cli_execute_command(char **tokens);

int wallet_load(char **args);
int wallet_save(char **args);
#endif /* CLI_H */
