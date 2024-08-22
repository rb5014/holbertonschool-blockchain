#include "cli.h"

blockchain_context_t *blockchain_context_create(void)
{
	blockchain_context_t *bchain_ctx = malloc(sizeof(*bchain_ctx));

	if (!bchain_ctx)
	{
		fprintf(stderr, "Couldn't allocate memory for blockchain context\n");
		return (NULL);
	}

	bchain_ctx->blockchain = blockchain_create();
	bchain_ctx->wallet = ec_create();
	bchain_ctx->transaction_pool = llist_create(MT_SUPPORT_FALSE);

	if (!bchain_ctx->blockchain || !bchain_ctx->wallet ||
		!bchain_ctx->transaction_pool)
	{
		fprintf(stderr, "Error during blockchain context initialization\n");
		blockchain_context_destroy(bchain_ctx);
		return (NULL);
	}

	return (bchain_ctx);
}

void blockchain_context_destroy(blockchain_context_t *bchain_ctx)
{
	blockchain_destroy(bchain_ctx->blockchain);
	EC_KEY_free(bchain_ctx->wallet);
	llist_destroy(bchain_ctx->transaction_pool, 1,
				  (node_dtor_t) transaction_destroy);
	free(bchain_ctx);
}
