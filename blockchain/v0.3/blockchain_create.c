#include "blockchain.h"

/**
 * blockchain_create - Creates a new blockchain
 * Return: Pointer to new chain or NULL
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *new_chain = NULL;
	block_t *new_block = NULL;
	block_info_t info = {0, 0, 1537578000, 0, {0}};
	block_data_t data = {"Holberton School", 16};

	new_chain = calloc(1, sizeof(blockchain_t));
	if (!new_chain)
		return (NULL);
	new_block = calloc(1, sizeof(block_t));
	if (!new_block)
		return (NULL);
	new_chain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!new_chain->chain)
		return (free(new_chain), NULL);
	new_chain->unspent = llist_create(MT_SUPPORT_FALSE);
	new_block->info = info, new_block->data = data;
	memcpy(new_block->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH);

	if (llist_add_node(new_chain->chain, new_block, ADD_NODE_REAR) == -1)
		return (llist_destroy(new_chain->chain, 0, NULL), free(new_chain), NULL);
	return (new_chain);
}
