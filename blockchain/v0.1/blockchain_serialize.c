#include "blockchain.h"

/**
 * blockchain_serialize - Serializes a Blockchain to a file
 * @blockchain: pointer to the blockchain structure to be serialized
 * @path: path to the file where the blockchain will be saved
 *
 * Return: 0 on success, -1 on failure
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
    FILE *file;
    block_t *block;
    size_t i;
    uint32_t block_count;
    uint8_t endian = 1;  /* Little-endian
    /* Open the file in write mode (overwrite if exists) */
    file = fopen(path, "wb");
    if (!file)
        return -1;
    /* Write the file header */
    fwrite("HBLK", 1, 4, file);  /* Magic number
    fwrite("0.1", 1, 3, file);  /* Version "0.1"
    fwrite(&endian, 1, 1, file);  /* Endianess (1 for little-endian)
    /* Write the number of blocks (in little-endian format) */
    block_count = llist_size(blockchain->chain);
    fwrite(&block_count, 4, 1, file);
    /* Serialize each block */
    block = llist_get_head(blockchain->chain);
    for (i = 0; i < block_count; i++)
    {
        /* Write block index (little-endian) */
        uint32_t index = block->info.index;
        fwrite(&index, 4, 1, file);
        /* Write block difficulty (little-endian) */
        uint32_t difficulty = block->info.difficulty;
        fwrite(&difficulty, 4, 1, file);
        /* Write timestamp (little-endian) */
        uint64_t timestamp = block->info.timestamp;
        fwrite(&timestamp, 8, 1, file);
        /* Write nonce (little-endian) */
        uint64_t nonce = block->info.nonce;
        fwrite(&nonce, 8, 1, file);
        /* Write prev_hash (32 bytes) */
        fwrite(block->info.prev_hash, 1, 32, file);
        /* Write data length (little-endian) */
        uint32_t data_len = block->data.len;
        fwrite(&data_len, 4, 1, file);
        /* Write block data */
        fwrite(block->data.buffer, 1, block->data.len, file);
        /* Write block hash (32 bytes) */
        fwrite(block->hash, 1, 32, file);
        /* Move to the next block */
        block = block->next;
    }
    /* Close the file */
    fclose(file);
    return 0;
}
