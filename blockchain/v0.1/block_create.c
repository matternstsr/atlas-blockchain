#include "blockchain.h"

/**
 * block_create - Creates a new block
 * @prev: Previous block
 * @data: data to duplicate into block
 * @data_len: Number of bytes to copy
 * Return: Pointer to new block or NULL
 */
block_t *block_create(block_t const *prev,
						int8_t const *data, uint32_t data_len)
{
	block_t *new_block = NULL;
	block_info_t info;
	block_data_t new_data = {{0}, 0};
	uint32_t len = data_len;

	new_block = malloc(sizeof(block_t));
	if (!new_block)
		return (NULL);

	/* Ensure data length doesn't exceed max */
	if (data_len > BLOCKCHAIN_DATA_MAX)
		len = BLOCKCHAIN_DATA_MAX;

	/* Copy the data into the new block's buffer */
	memcpy(new_data.buffer, data, len);
	new_data.len = len;

	/* Set up the block information (index, timestamp, prev_hash) */
	info.index = prev->info.index + 1; /* Next block index */
	info.difficulty = 0; /* Difficulty (could be used later) */
	info.nonce = 0;/* Nonce (for proof of work) */
	memcpy(info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	/* Copy the previous block's hash */
	info.timestamp = time(NULL);  /* Current timestamp for block creation */

	/* Initialize the hash to zero initially (to be computed later) */
	memset(new_block->hash, 0, SHA256_DIGEST_LENGTH);

	/* Set the new block's data and info */
	new_block->data = new_data;
	new_block->info = info;

	return (new_block);
}
