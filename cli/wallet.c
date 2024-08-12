#include "cli.h"
static EC_KEY *current_wallet;

int wallet_load(char **args)
{
	char *path = NULL;

	if (args[1] != NULL)
	{
		fprintf(stderr, " Usage: wallet_load <path>\n");
		return (0);
	}
	path = args[0];

	current_wallet = ec_load(path);

	if (!current_wallet)
	{
		printf("Couldn't load wallet, creating a new one\n");
		current_wallet = ec_create();
		if (!current_wallet)
		{
			fprintf(stderr, "Couldn't create wallet, exit\n");
			return (-1);
		}
	}

	return (1);
}

int wallet_save(char **args)
{
	char *path;

	if (args[1] != NULL)
	{
		fprintf(stderr, " Usage: wallet_save <path>\n");
		return (0);
	}
	path = args[0];

	if (ec_save(current_wallet, path) == 0)
	{
		fprintf(stderr, "Couldn't save current wallet\n");
		return (0);
	}

	printf("Private key saved at \"%s%s\"\n", path, PRI_FILENAME);
	printf("Public key saved at \"%s%s\"\n", path, PUB_FILENAME);
	return (1);
}
