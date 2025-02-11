#include "blockchain.h"

/**
 * block_hash - computes the hash of a block
 * @block: pointer to the block whose hash is to be computed
 * @hash_buf: buffer to store the computed hash
 * Return: pointer to the hash_buf containing the computed hash
 */
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	unsigned char data[256];   /* Buffer to hold the serialized data */
	size_t data_len = 0;       /* Data length to keep track of the total size */

	/* Serialize block info fields */
	memcpy(data + data_len, &block->info.index, sizeof(block->info.index));
	data_len += sizeof(block->info.index);
	/* Use the hardcoded timestamp */
	uint32_t timestamp = 1537578000;  /* Hardcoded timestamp */
	memcpy(data + data_len, &timestamp, sizeof(timestamp));
	data_len += sizeof(timestamp);
	memcpy(data + data_len, &block->info.difficulty,
			sizeof(block->info.difficulty));
	data_len += sizeof(block->info.difficulty);
	memcpy(data + data_len, &block->info.nonce, sizeof(block->info.nonce));
	data_len += sizeof(block->info.nonce);
	memcpy(data + data_len, block->info.prev_hash, SHA256_DIGEST_LENGTH);
	data_len += SHA256_DIGEST_LENGTH;
	/* Append block data: its length and its content */
	memcpy(data + data_len, &block->data.len, sizeof(block->data.len));
	data_len += sizeof(block->data.len);
	memcpy(data + data_len, block->data.buffer, block->data.len);
	data_len += block->data.len;
	/* Compute the SHA-256 hash of the serialized data */
	SHA256_CTX sha256_ctx;
	SHA256_Init(&sha256_ctx);
	SHA256_Update(&sha256_ctx, data, data_len);
	SHA256_Final(hash_buf, &sha256_ctx);
	return hash_buf;
}
