#ifndef _BLOCKCHAIN_H_
#define _BLOCKCHAIN_H_

#include "provided/endianness.h"
#include <sys/types.h>
#include <openssl/sha.h>
#include <llist.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>

#define BLOCKCHAIN_DATA_MAX 1024
#define MAGIC_NUMS "\x48\x42\x4c\x4b"
#define VERS "\x30\x2e\x31"
#define SHA256_DIGEST_LENGTH 32
#define HOLBERTON_HASH \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
#define FHEADER "\x48\x42\x4c\x4b\x30\x2e\x31"
#define GENESIS_HASH 00000000000000000000000000000000
#define END ((_get_endianness() == 1) ? "\x01" : "\x02")


/**
 * struct blockchain_s - Represents the blockchain structure
 * @chain: A linked list of pointers to each block in the blockchain
 */
typedef struct blockchain_s
{
	llist_t     *chain;
} blockchain_t;

/**
 * struct block_data_s - Holds the data of a block
 * @buffer: Array to store the block's data
 * @len:    The length of the data in the block
 */
typedef struct block_data_s
{
	/*
	* @buffer must be first, allowing us to treat the structure as an
	* array of characters.
	*/
	int8_t      buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;

/**
 * struct block_info_s - Contains the information about a block
 * @index:      The block's index in the chain
 * @difficulty: Difficulty of the proof-of-work algorithm (leading zeros in hash)
 * @timestamp:  The time when the block was created (in UNIX timestamp format)
 * @nonce:      A value used to modify the block's hash during mining
 * @prev_hash:  The hash of the previous block in the chain
 */
typedef struct block_info_s
{
	/*
		* The order of these fields should remain as is to ensure proper alignment.
		* This structure is designed to be directly used as an array of characters
		* in any architecture.
		*/
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;

/**
 * struct block_s - Represents a block in the blockchain
 * @info: Metadata about the block (block_info_t)
 * @data: The data stored in the block (block_data_t)
 * @hash: A 256-bit hash to ensure the block's integrity and authenticity
 */
typedef struct block_s
{
	block_info_t    info; /* Block's metadata (info) */
	block_data_t    data; /* The block's actual data */
	uint8_t         hash[SHA256_DIGEST_LENGTH]; /* Block's unique hash */
} block_t;



/**
 * blockchain_create - Initializes a new blockchain, starting with the Genesis block
 * Return: A pointer to the newly created blockchain, or NULL on failure
 */
blockchain_t *blockchain_create(void);
/**
 * block_create - Generates a new block, linking it to a previous one
 * @prev: The previous block to link to
 * @data: The data to store in the new block
 * @data_len: The size of the data to copy into the block
 * Return: A pointer to the newly created block, or NULL on failure
 */
block_t *block_create(block_t const *prev,
	int8_t const *data, uint32_t data_len);
/**
 * block_destroy - Frees the memory used by a block
 * @block: The block to be deallocated
 */
void block_destroy(block_t *block);
/**
 * blockchain_destroy - Frees the memory used by the entire blockchain
 * @blockchain: The blockchain structure to be destroyed
 */
void blockchain_destroy(blockchain_t *blockchain);
/**
 * block_hash - Calculates the hash of a given block
 * @block: The block to hash
 * @hash_buf: A buffer to store the resulting hash
 * Return: A pointer to the hash buffer containing the block's hash
 */
uint8_t *block_hash(block_t const *block, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
/**
 * blockchain_serialize - Saves the blockchain to a file
 * @blockchain: The blockchain to serialize
 * @path: The file path where the blockchain should be saved
 * Return: 0 on success, or -1 if an error occurred
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
/**
 * blockchain_deserialize - Loads a blockchain from a file
 * @path: The file path from which to load the blockchain
 * Return: A pointer to the deserialized blockchain, or NULL if an error occurred
 */
blockchain_t *blockchain_deserialize(char const *path);
/**
 * block_is_valid - Checks whether a given block is valid based on its predecessor
 * @block: The block to validate
 * @prev_block: The previous block in the chain, to check the validity of the block
 * Return: 1 if the block is valid, or 0 if it's invalid
 */
int block_is_valid(block_t const *block, block_t const *prev_block);
#endif /* _BLOCKCHAIN_H_ */
