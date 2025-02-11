#include "blockchain.h"

/**
 * block_hash - hashes a block using sha256
 * @block: block to hash
 * @hash_buf: buffer to store computed hash
 * Return: hash buffer or NULL
 */
uint8_t *block_hash(block_t const *block,
					uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	if (!block || !hash_buf)
		return (NULL);
	/* Serialize the block: info + data */
	unsigned char data[sizeof(block->info) + block->data.len];
	size_t data_len = 0;
	/* Copy block's info */
	memcpy(data + data_len, &block->info, sizeof(block->info));
	data_len += sizeof(block->info);
	/* Copy block's data */
	memcpy(data + data_len, block->data.buffer, block->data.len);
	data_len += block->data.len;
	/* Hash the serialized data */
	SHA256(data, data_len, hash_buf);
	return (hash_buf);
}
