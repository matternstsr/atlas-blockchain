#include "blockchain.h"

/**
 * blockchain_create - Creates a new blockchain
 * Return: Pointer to new chain or NULL
 */

blockchain_t *blockchain_create(void)
{
	blockchain_t *chain = NULL; /* Changed name from new_chain */
	block_t *genesis_block = NULL; /* Changed name from new_block */
	/* Initialize block_info and block_data structures */
	block_info_t block_info = {0, 0, 1537578000, 0, {0}}; /* Changed from info */
	block_data_t block_data = {"Holberton School", 16}; /* Changed from data */
	/* Allocate memory for blockchain */
	chain = malloc(sizeof(blockchain_t));
	if (!chain)
		return (NULL);
	/* Allocate memory for the genesis block */
	genesis_block = malloc(sizeof(block_t));
	if (!genesis_block)
		return (free(chain), NULL);
	/* Create a new linked list for the blockchain */
	chain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!chain->chain)
	{
		free(genesis_block);
		free(chain);
		return (NULL);
	}
	/* Set the info and data of the genesis block */
	genesis_block->info = block_info;
	genesis_block->data = block_data;
	/* Assign predefined hash for the genesis block */
	memcpy(genesis_block->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH);
	/* Add the genesis block to the chain */
	if (llist_add_node(chain->chain, genesis_block, ADD_NODE_REAR) == -1)
	{
		llist_destroy(chain->chain, 0, NULL);
		free(chain);
		free(genesis_block);
		return (NULL);
	}
	/* Return the created blockchain */
	return (chain);
}
