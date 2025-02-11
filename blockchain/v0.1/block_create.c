#include "blockchain.h"

/**
 * block_create - creates a new Block structure and initializes it
 * @prev: The previous block in the blockchain
 * @data: Data to be stored in the block
 * @data_len: The length of the data to duplicate
 * Return: A pointer to the newly created block
 * Definition: The difficulty is a measure of how hard it is to find a
 *  valid block hash that meets certain criteria. In proof-of-work systems,
 *  miners must find a hash that is less than a target value.
 * Definition: The nonce is a number that miners adjust during the mining
 *  process in order to change the resulting hash of a block. The nonce is
 *  part of the block header, and miners increment it in order to find a
 *  valid hash.
 */
block_t *block_create(block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *new_block = NULL;

	/*  memory for the new block */
	new_block = malloc(sizeof(block_t));
	if (!new_block)
		return (NULL);
	/* Set the index: previous block's index + 1 */
	new_block->index = prev->index + 1;
	/* Set difficulty and nonce to 0 */
	new_block->difficulty = 0;
	new_block->nonce = 0;
	/* Set the timestamp using the current time */
	new_block->timestamp = time(NULL);
	/* Copy the previous block's hash */
	memcpy(new_block->prev_hash, prev->hash, sizeof(new_block->prev_hash));
	/* Copy the data, ensuring we do not exceed the maximum allowed length */
	new_block->data.len = (data_len > BLOCKCHAIN_DATA_MAX) ? BLOCKCHAIN_DATA_MAX : data_len;
	memcpy(new_block->data.buffer, data, new_block->data.len);
	/* Set the hash to all zeroes (we will hash it later) */
	memset(new_block->hash, 0, sizeof(new_block->hash));
	return (new_block);
}
