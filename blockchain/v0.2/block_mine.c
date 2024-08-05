#include "blockchain.h"

/**
 *  block_mine - Mines a Block in order to insert it in the Blockchain
 * @block: Pointer to the Block to be mined
 *
 * The function must find a hash for block that matches its difficulty
*/
void block_mine(block_t *block)
{

	if (!block)
		return;

	do {
		block_hash(block, block->hash);
	} while ((hash_matches_difficulty(block->hash, block->info.difficulty) == 0)
			 && ++block->info.nonce);

}
