#include "blockchain.h"

/**
 * block_create - Creates a new block
 * @prev: Previous block
 * @data: data to duplicte into block
 * @data_len: Number of bytes to copy
 * Return: Pointer to new block or NULL
 */
block_t *block_create(
	block_t const *prev, int8_t const *data, uint32_t data_len)
{
	block_t *new_block = NULL;
	block_info_t info;
	block_data_t new_data = {{0}, 0};
	uint32_t len = data_len;

	new_block = calloc(1, sizeof(block_t));
	if (!new_block)
		return (NULL);
	if (data_len > BLOCKCHAIN_DATA_MAX)
		len = BLOCKCHAIN_DATA_MAX;
	memcpy(new_data.buffer, data, len);
	new_data.len = len;

	info.index = prev->info.index + 1;
	info.difficulty = 0, info.nonce = 0;
	memcpy(info.prev_hash, prev->hash, 32);
	info.timestamp = time(NULL);

	new_block->data = new_data, new_block->info = info;
	new_block->transactions = llist_create(MT_SUPPORT_FALSE);
	return (new_block);
}
