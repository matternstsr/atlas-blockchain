#include "blockchain.h"

/**
 * blockchain_create - Creates a new blockchain
 * Return: Pointer to new chain or NULL
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *new_chain = NULL;
	block_t *new_block = NULL;

	/* Initialize block_info and block_data structures */
	block_info_t info = {0, 0, 1537578000, 0, {0}};
	block_data_t data = {"Holberton School", 16};
	/* Allocate memory for blockchain */
	new_chain = malloc(sizeof(blockchain_t));
	if (!new_chain)
		return (NULL);
	/* Allocate memory for the genesis block */
	new_block = malloc(sizeof(block_t));
	if (!new_block)
		return (free(new_chain), NULL);
	/* Create a new linked list for the blockchain */
	new_chain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!new_chain->chain)
	{
		free(new_block);
		free(new_chain);
		return (NULL);
	}
	/* Set the info and data of the genesis block */
	new_block->info = info;
	new_block->data = data;
	/* Assign predefined hash for the genesis block */
	memcpy(new_block->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH);
	/* Add the genesis block to the chain */
	if (llist_add_node(new_chain->chain, new_block, ADD_NODE_REAR) == -1)
	{
		llist_destroy(new_chain->chain, 0, NULL);
		free(new_chain);
		free(new_block);
		return (NULL);
	}
	/* Return the created blockchain */
	return (new_chain);
}
