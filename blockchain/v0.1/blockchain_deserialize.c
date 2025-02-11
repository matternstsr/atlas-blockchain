#include "blockchain.h"

#define CHECK_FREAD(fptr, buf, size) (fread(buf, 1, size, fptr) != size)
#define CHECK_MEMCMP(buf, cmp, size) (memcmp(buf, cmp, size))

/**
 * blockchain_deserialize - Loads a blockchain from file
 * @path: file to read from
 * Return: Pointer to chain or NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fptr = fopen(path, "r");
	char header_buf[7] = {0};
	uint8_t end;
	uint32_t numblocks;
	blockchain_t *blockchain;
	block_t *block;

	if (!fptr || CHECK_FREAD(fptr, header_buf, 7) || CHECK_MEMCMP(header_buf, FHEADER, 7) ||
		CHECK_FREAD(fptr, &end, 1) || CHECK_FREAD(fptr, &numblocks, 4))
		return (NULL);
	blockchain = calloc(1, sizeof(blockchain_t));
	if (!blockchain)
	{
		fclose(fptr);
		return (NULL);
	}
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);
	while (numblocks--)
	{
		block = calloc(1, sizeof(block_t));
		if (!block || CHECK_FREAD(fptr, &block->info, sizeof(block_info_t)) ||
			CHECK_FREAD(fptr, &(block->data.len), 4) || CHECK_FREAD(fptr, &block->data.buffer,
			block->data.len) || CHECK_FREAD(fptr, &block->hash, SHA256_DIGEST_LENGTH))
		{
			free(block);
			llist_destroy(blockchain->chain, 1);
			free(blockchain);
			fclose(fptr);
			return (NULL);
		}
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	fclose(fptr);
	return (blockchain);
}
