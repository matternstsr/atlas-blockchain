#include "blockchain.h"

/**
 * block_destroy - frees the memory allocated for a block
 * @block: pointer to the block to be freed
 */
void block_destroy(block_t *block)
{
    if (block) {
        /* Free the memory for the block itself */
        free(block);
    }
}
