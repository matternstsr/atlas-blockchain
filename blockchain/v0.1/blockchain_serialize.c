#include "blockchain.h"

/**
 * serialize_block_to_file - func to serialize a block and write it to a file
 * @list: node to perform function on
 * @index: unused
 * @arg: filestream pointer
 * Return: 0
 */
int serialize_block_to_file(llist_node_t list, unsigned int index, void *arg)
{
	block_t *tmp = NULL;
	char block_buf[1116];   /* Buffer to hold the block data */
	uint32_t len = 0;

	(void)index;

	/* Cast the list node to block_t* (block pointer) */
	tmp = (block_t *)list;
	if (!tmp)
		return (0);   /* Handle NULL pointers gracefully */
	/* Handle Endianness (If applicable) */
	if (!strcmp(END, "\x02"))
		SWAPENDIAN(tmp);   /* Apply endian swap if needed */
	/* Get the block data length */
	len = tmp->data.len;
	/* Copy block metadata into the buffer */
	memcpy(&block_buf[0], &tmp->info, sizeof(block_info_t));
	/* Copy block data length into the buffer */
	memcpy(&block_buf[56], &tmp->data.len, 4);
	/* Copy actual block data into the buffer */
	memcpy(&block_buf[60], tmp->data.buffer, len);
	/* Copy the block's hash into the buffer */
	memcpy(&block_buf[60 + len], tmp->hash, 32);
	/* Write the buffer to the file */
	fwrite(block_buf, 1, 92 + len, arg);
	return (0);  /* Return 0 to indicate successful handling of the node */
}

/**
 * blockchain_serialize - serializes a blockchain to file
 * @blockchain: chain to serialize
 * @path: file path to serialize to
 * Return: 1 on success, 0 on fail
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fptr = NULL;
	int blocknums = 0;
	char header[12] = {FHEADER};   /* Header of the serialized file */

	if (!blockchain || !path)
		return (0);   /* Check if input parameters are valid */
	/* Get the size of the blockchain (number of blocks) */
	blocknums = llist_size(blockchain->chain);
	/* Set the endianness flag and number of blocks in the header */
	memcpy(&header[7], END, 1);   /* Set endianness flag */
	memcpy(&header[8], &blocknums, 4);   /* Set the number of blocks */
	/* Open the file for writing */
	fptr = fopen(path, "w");
	if (!fptr)
		return (0);   /* Check if file opened successfully */
	/* Write the header to the file */
	fwrite(header, 1, 12, fptr);
	/* Iterate through the blockchain and write each block */
	llist_for_each(blockchain->chain, serialize_block_to_file, fptr);
	/* Close the file after writing */
	fclose(fptr);
	return (1);   /* Indicate success */
}
