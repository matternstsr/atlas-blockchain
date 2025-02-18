#include "blockchain.h"

/**
 * block_hash - hashes a block using sha256
 * @block: block to hash
 * @hash_buf: buffer to store computed hash
 * Return: hash buffer or NULL
 */
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	SHA256_CTX sha256_ctx;
	uint8_t data[sizeof(block->info) + sizeof(block->data)];

	/* Copy block info and data into the data buffer */
	memcpy(data, &block->info, sizeof(block->info));
	memcpy(data + sizeof(block->info), &block->data, sizeof(block->data));

	/* Initialize the SHA256 context */
	SHA256_Init(&sha256_ctx);

	/* Update the context with the data buffer */
	SHA256_Update(&sha256_ctx, data, sizeof(data));

	/* Finalize the hash computation */
	SHA256_Final(hash_buf, &sha256_ctx);

	return hash_buf;
}
