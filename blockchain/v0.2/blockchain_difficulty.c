#include "blockchain.h"

#define BLK_GEN_INT 1 /* BLOCK GENERATION INTERVAL 1 sec (adjust as nec) */
#define  DIF_ADJ_INT 5 /* DIFFICULTY ADJUSTMENT INTERVAL 5 Every 5 blocks */

/**
 * blockchain_difficulty - Calculates difficulty of next block
 * @blockchain: Pointer to blockchain
 * Return: difficulty to be set for next block
 */
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *lst_blk, *adj_blk;
	uint32_t expected_time, actual_time, difficulty;

	if (!blockchain || !blockchain->chain || llist_size(blockchain->chain) <= 1)
		return (0);  /* No difficulty to adjust or invalid blockchain */
	/* Get last block in blockchain */
	lst_blk = llist_get_tail(blockchain->chain);
	
	if (lst_blk->info.index % DIF_ADJ_INT == 0 && lst_blk->info.index != 0) 
	{
		idx = (llist_size(blockchain->chain) - DIFF_ADJUSTMENT_INTERVAL);
		/* If last block index is multiple of DIF_ADJ_INT & it's not genesis block, adjust difficulty */
		adj_blk = llist_get_node_at(blockchain->chain, idx);
		/* Calculate expected time between two blocks */
		expected_time = DIF_ADJ_INT * BLK_GEN_INT;
		/* Calculate actual time difference */
		actual_time = lst_blk->info.timestamp - adj_blk->info.timestamp;
		/* Adjust difficulty based on time difference */
		if (actual_time > expected_time << 1)  /* Equivalent to actual_time > expected_time * 2 */
			difficulty = adj_blk->info.difficulty - 1; /* Decrease difficulty */
		else if (actual_time < expected_time >> 1)  /* Equivalent to actual_time < expected_time / 2 */
			difficulty = adj_blk->info.difficulty + 1; /* Increase difficulty */
		else
			difficulty = adj_blk->info.difficulty;  /* Keep same */
	}
	else
		difficulty = lst_blk->info.difficulty; /* No adjustment, return current difficulty */
	return (difficulty);
}
