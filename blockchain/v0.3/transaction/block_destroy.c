#include "blockchain.h"

/**
 * block_destroy - Destroys a block and frees its memory
 * @block: The block to destroy
 */
void block_destroy(block_t *block)
{
	if (!block)  /* If the block pointer is NULL, simply return. */
		return;
	/* NOTE TO SELF: Free any dynamic memory associated with the block */
	/* For this, there are no dynam allocated membersin the block_t struct. */
	/* If 'data.buffer' were dynam allocated, it would need freed it here. */
	free(block);  /* Free the block struct itself */
}
