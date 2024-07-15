#include "blockchain.h"

/**
 * block_destroy - Deletes an existing Block
 * @block: Pointer to block to delete
*/
void block_destroy(block_t *block)
{
	free(block);
}
