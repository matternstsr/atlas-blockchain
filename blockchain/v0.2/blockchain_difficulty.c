#include "blockchain.h"

uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
    block_t *block, *adj_block;
    uint32_t exp_time = 0;
    uint64_t act_time = 0;
    int idx = 0;

    /* Check if blockchain is NULL */
    if (!blockchain)
        return (0); /* Return 0 if blockchain is invalid */

    /* Get the last block in the blockchain */
    block = llist_get_tail(blockchain->chain);

    /* Check if the current block's index is divisible by the difficulty adjustment interval */
    if ((block->info.index % DIFF_ADJUSTMENT_INTERVAL == 0) && block->info.index != 0)
    {
        /* Calculate the index of the block where the difficulty was last adjusted */
        idx = (llist_size(blockchain->chain) - DIFF_ADJUSTMENT_INTERVAL);
        
        /* Get the block where difficulty was last adjusted */
        adj_block = llist_get_node_at(blockchain->chain, idx);

        /* Calculate expected time difference between blocks */
        exp_time = (block->info.index - adj_block->info.index) * BLOCK_GENERATION_INTERVAL;

        /* Calculate the actual time difference between the blocks */
        act_time = block->info.timestamp - adj_block->info.timestamp;

        /* If the actual time is much greater than expected, decrease difficulty */
        if (act_time > exp_time << 1)
            return (DIFF - 1); /* Decrease difficulty */

        /* If the actual time is much less than expected, increase difficulty */
        else if (act_time < exp_time >> 1)
            return (DIFF + 1); /* Increase difficulty */

        /* If the actual time is within expected range, keep difficulty the same */
        else
            return (DIFF); /* Keep the same difficulty */
    }
    else
    {
        /* If no adjustment is needed, return the current difficulty */
        return (DIFF);
    }
}
