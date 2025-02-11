#include "blockchain.h"

/**
 * read_block_header - Reads the header and metadata from the file
 * @file: pointer to the file to read from
 * @block_count: pointer to the block count variable
 * Return: 0 on success, -1 on failure
 */
int read_block_header(FILE *file, uint32_t *block_count)
{
	char header[7] = {0};
	uint8_t header_check;

	if (CHECK_FREAD(file, header, sizeof(header)) ||
		memcmp(header, FHEADER, sizeof(header)))
		return (-1);
	if (CHECK_FREAD(file, &header_check, 1) ||
		CHECK_FREAD(file, block_count, sizeof(*block_count)))
		return (-1);
	return (0);
}

/**
 * cleanup_blockchain - Helper function to clean up blockchain and free memory
 * @blockchain: the blockchain to free
 * @file: file pointer to close after cleanup
 */
void cleanup_blockchain(blockchain_t *blockchain, FILE *file)
{
	llist_destroy(blockchain->chain, 1, NULL);
	free(blockchain);
	fclose(file);
}

/**
 * deserialize_blockchain - Loads a blockchain from a file
 * @path: file to read from
 * Return: Pointer to the blockchain or NULL if an error occurs
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *file = fopen(path, "r");
	blockchain_t *blockchain = NULL;
	block_t *new_block = NULL;
	uint32_t block_count;

	if (!file)
		return (NULL);
	/* Read header and block count */
	if (read_block_header(file, &block_count) != 0)
	close_file_and_return_null(file);
	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
	close_file_and_return_null(file);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!blockchain->chain)
	{
		close_file_and_return_null(file);
		return (NULL);
	}
	for (uint32_t i = 0; i < block_count; i++)
	{
		new_block = calloc(1, sizeof(block_t));
		if (!new_block ||
			CHECK_FREAD(file, &new_block->info, sizeof(block_info_t)) ||
			CHECK_FREAD(file, &new_block->data.len, sizeof(uint8_t) * 4) ||
			CHECK_FREAD(file, new_block->data.buffer, new_block->data.len) ||
			CHECK_FREAD(file, new_block->hash, SHA256_DIGEST_LENGTH))
		{
			free(new_block);
			cleanup_blockchain(blockchain, file);
			return (NULL);
		}
		llist_add_node(blockchain->chain, new_block, ADD_NODE_REAR);
	}
	fclose(file);
	return (blockchain);
}

/**
 * close_file_and_return_null - Closes the file and returns NULL
 * @file: The file to close
 * Return: NULL (always)
 */
void *close_file_and_return_null(FILE *file)
{
	fclose(file);
	return (NULL);
}
