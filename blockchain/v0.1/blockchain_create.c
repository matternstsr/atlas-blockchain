#include "blockchain.h"

/**
 * blockchain_create - creates a new blockchain with the Genesis Block
 * Return: A pointer to the created blockchain structure or NULL on failure
 */
blockchain_t *blockchain_create(void)
{
	blockchain_t *blockchain = NULL;
	block_t *genesis_block = NULL;

	/* Allocate memory for the blockchain */
	blockchain = malloc(sizeof(blockchain_t));
	if (!blockchain)
		return (NULL);

	/* Initialize the blockchain */
	blockchain->chain = llist_create(0); /* Pass flags as needed */
	
	/* Allocate and initialize the Genesis Block */
	genesis_block = malloc(sizeof(block_t));
	if (!genesis_block)
	{
		free(blockchain);
		return (NULL);
	}

	/* Set the Genesis Block data */
	genesis_block->index = 0;
	genesis_block->difficulty = 0;
	genesis_block->timestamp = 1537578000;
	genesis_block->nonce = 0;
	memset(genesis_block->info.prev_block_hash, 0, sizeof(genesis_block->info.prev_block_hash));
	strncpy((char *)genesis_block->data_info.buffer, "Holberton School", sizeof(genesis_block->data_info.buffer));
	genesis_block->data_info.len = 16;

	/* The hash is predefined in the prompt */
	unsigned char predefined_hash[] = {
		0xc5, 0x2c, 0x26, 0xc8, 0xb5, 0x46, 0x16, 0x39,
		0x63, 0x5d, 0x8e, 0xdf, 0x2a, 0x97, 0xd4, 0x8d,
		0x0c, 0x8e, 0x00, 0x09, 0xc8, 0x17, 0xf2, 0xb1,
		0xd3, 0xd7, 0xff, 0x2f, 0x04, 0x51, 0x58, 0x03
	};
	memcpy(genesis_block->hash, predefined_hash, sizeof(genesis_block->hash));

	/* Add the Genesis Block to the blockchain */
	llist_add_node(blockchain->chain, genesis_block, ADD_NODE_FRONT); /* Assuming a linked list add function */

	return (blockchain);
}
