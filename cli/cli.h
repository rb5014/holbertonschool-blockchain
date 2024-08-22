#ifndef CLI_H
#define CLI_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "hblk_crypto.h"
#include "blockchain.h"
#include "transaction.h"

#define DEFAULT_WALLET_PATH "./wallet"
#define LAST_WALLET_PATH_FILE "./last_wallet_path.txt"

typedef struct blockchain_context_s
{
	blockchain_t *blockchain;
	EC_KEY *wallet;
	llist_t *transaction_pool;
} blockchain_context_t;

typedef struct command_context_s
{
	char *name;
	char **args;
	int argc;
} command_context_t;

typedef struct command_s
{
	char *name;
	int (*func_ptr)(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
} command_t;



/* main.c */
void cli_loop(void);
int cli_split_line(char *line, char ***tokens);
int cli_execute_command(command_context_t *cmd_ctx,
					    blockchain_context_t *bchain_ctx);

/* blockchain_context.c */
blockchain_context_t *blockchain_context_create(void);
void blockchain_context_destroy(blockchain_context_t *bchain_ctx);

/* wallet_commands.c */
int wallet_load(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
int wallet_save(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);

/* blockchain_commands.c */
int send(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
int mine(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
int info(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
int save(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
int load(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);

/* exit_commands.c */
int cli_exit(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);
int cli_quit(command_context_t *cmd_ctx, blockchain_context_t *bchain_ctx);

/* utility_functions.c */
int is_positive_number(const char *string);
uint8_t *hex_str_to_pub(char *string);


/* blockchain provided functions in blockchain/v0.3/provided */
void _blockchain_print(blockchain_t const *blockchain);
void _blockchain_print_brief(blockchain_t const *blockchain);


#endif /* CLI_H */
