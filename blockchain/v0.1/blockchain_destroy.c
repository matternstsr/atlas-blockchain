#include "blockchain.h"

/**
 * blockchain_destroy - Frees the memory allocated for a blockchain
 * @blockchain: pointer to the blockchain structure to be freed
 */
void blockchain_destroy(blockchain_t *blockchain)
{
	block_t *block;

	if (blockchain) 
	{
		/* Free each block in the linked list */
		while ((block = llist_pop(blockchain->chain)) != NULL)
			block_destroy(block);
			/* Use the block_destroy function to free each block */
		/* Free the linked list structure */
		llist_destroy(blockchain->chain, 1, NULL);
		/* Finally, free the blockchain structure */
		free(blockchain);
	}
}
