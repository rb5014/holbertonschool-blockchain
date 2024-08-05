#include "blockchain.h"

/**
 * blockchain_difficulty - computes the difficulty to assign to
 *						   a potential next Block in the Blockchain.
 * @blockchain: Pointer to the Blockchain to analyze
 * Return: Difficulty to be assigned to a potential
 *		   next Block in the Blockchain
 *
 * If the latest Block’s index is a multiple of
 * DIFFICULTY_ADJUSTMENT_INTERVAL, AND that it is not the Genesis Block,
 * the difficulty must be adjusted.
 * Otherwise, the difficulty of the latest Block in blockchain is returned
 * Difficulty adjustment:
 *		.Retrieve the last Block for which an adjustment was made
 *			(the Block with index <blockchain_size> - DIFFICULTY_ADJUSTMENT_INTERVAL)
 *		.Compute the expected elapsed time between the two Blocks
 *		.Compute the actual elapsed time
 *		.The difficulty must be incremented if the elapsed time is
 *			lower than half the expected elapsed time
 *		.The difficulty must be decremented if the elapsed time is
 *			greater than twice the expected elapsed time
 *		.The difficulty should not change otherwise
*/
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *tail, *last_adjust_block;
	uint32_t size, last_index, last_adjust_index;
	uint32_t actual_time, expected_time;
	uint32_t difficulty;

	if (!blockchain)
		return (0);

	tail = llist_get_tail(blockchain->chain);
	if (!tail)
		return (0);

	/* Set difficulty of new block to last difficulty */
	difficulty = tail->info.difficulty;
	last_index = tail->info.index;

    /* if last_index not a multiple of diff interval OR is the genesis block */
	if ((last_index == 0) || last_index % DIFFICULTY_ADJUSTMENT_INTERVAL != 0)
		return (difficulty);

	/* Now difficulty must be adjusted */
	size = llist_size(blockchain->chain);
	last_adjust_index = size - DIFFICULTY_ADJUSTMENT_INTERVAL;
	last_adjust_block = (block_t *) llist_get_node_at(blockchain->chain,
													  last_adjust_index);
	if (!last_adjust_block)
		return (0);

	expected_time = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
	actual_time = tail->info.timestamp - last_adjust_block->info.timestamp;

	if (actual_time < expected_time / 2)
		difficulty++;
	else if ((actual_time > 2 * expected_time) && (difficulty > 0))
		difficulty--;

	return (difficulty);
}
