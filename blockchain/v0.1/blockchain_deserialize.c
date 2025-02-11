#include "blockchain.h"

/**
 * blockchain_deserialize - Deserializes a blockchain from a file
 * @path: Path to the file containing the serialized blockchain
 * Return: A pointer to the deserialized blockchain, or NULL on failure
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *file;
	blockchain_t *blockchain = NULL;
	uint32_t magic_number;
	uint32_t version;
	block_t *block;
	size_t read_size;

	/* Open the file in binary mode */
	file = fopen(path, "rb");
	if (!file)
		return (NULL);
	/* Read the magic number */
	read_size = fread(&magic_number, sizeof(magic_number), 1, file);
	if (read_size != 1 || memcmp(&magic_number, MAGIC_NUMBER,
		sizeof(MAGIC_NUMBER) - 1) != 0)
	{
		fclose(file);
		return (NULL); /* Incorrect magic number */
	}
	/* Read the version number */
	read_size = fread(&version, sizeof(version), 1, file);
	if (read_size != 1 || version != 0x01)
	{
		fclose(file);
		return (NULL); /* Version mismatch */
	}
	/* Create the blockchain */
	blockchain = blockchain_create();
	if (!blockchain)
	{
		fclose(file);
		return (NULL); /* Memory allocation failure */
	}
	/* Deserialize blocks from file */
	while ((block = block_deserialize(file)) != NULL)
	{
		if (!blockchain_add_block(blockchain, block))
		{
			block_destroy(block);
			blockchain_destroy(blockchain);
			fclose(file);
			return (NULL); /* Failed to add block to the blockchain */
		}
	}
	/* Close the file */
	fclose(file);
	return (blockchain); /* Return the deserialized blockchain */
}
