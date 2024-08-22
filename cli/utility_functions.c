#include "cli.h"

int is_positive_number(const char *string)
{
	size_t i;

	for (i = 0; i < strlen(string); i++)
	{
		if (string[i] < '0' || string[i] > '9')
			return (0);
	}
	return (1);
}

uint8_t *hex_str_to_pub(char *hex_string)
{
	uint8_t *octet_string;
	size_t len_string = strlen(hex_string);

	if (!hex_string)
		return (NULL);

	if (len_string != EC_PUB_LEN * 2)
	{
		fprintf(stderr, "Wrong public key length\n");
		return (NULL);
	}

	octet_string = malloc(EC_PUB_LEN);
	if (!octet_string)
	{
		fprintf(stderr, "Couldn't allocate for octet string\n");
		return (NULL);
	}

	for (size_t i = 0; i < strlen(hex_string); i += 2)
	{
		char hex_byte[3] = {hex_string[i], hex_string[i + 1], '\0'};
		uint8_t byte = (uint8_t)strtol(hex_byte, NULL, 16);

		octet_string[i / 2] = byte;
	}

	return (octet_string);
}
