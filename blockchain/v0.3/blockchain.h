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

#define DIFF block->info.difficulty
#define BLK_GEN_INT 1 /* BLOCK GENERATION INTERVAL 1 sec (adjust as nec) */
#define DIF_ADJ_INT 5 /* DIFFICULTY ADJUSTMENT INTERVAL 5 Every 5 blocks */
#define CHECK_FREAD(file, buf, size) (fread(buf, 1, size, file) != size)
#define BLOCKCHAIN_MAGIC 0x48424c4b   /* Magic number for blockchain */
#define BLOCKCHAIN_DATA_MAX 1024
#define VERS "\x30\x2e\x31"              /* Version of the blockchain */
#define HASH_SIZE SHA256_DIGEST_LENGTH  /* 32 */
#define FHEADER "\x48\x42\x4c\x4b\x30\x2e\x31"  /* Blockchain header */
#define GENESIS_HASH 00000000000000000000000000000000  /* Genesis block hash */
#define END ((_get_endianness() == 1) ? "\x01" : "\x02")  /* Endianness chck */
#define HOLBERTON_HASH \
"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"

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
    int8_t      buffer[BLOCKCHAIN_DATA_MAX];
    uint32_t    len;
} block_data_t;

/**
 * struct block_info_s - Contains the information about a block
 * @index:      The block's index in the chain
 * @difficulty: Diff of the proof-of-work algorithm (leading zeros in hash)
 * @timestamp:  The time when the block was created (in UNIX timestamp format)
 * @nonce:      A value used to modify the block's hash during mining
 * @prev_hash:  The hash of the previous block in the chain
 */
typedef struct block_info_s
{
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
 * @transactions: A linked list of transactions in this block
 */
typedef struct block_s
{
    block_info_t    info; /* Block's metadata (info) */
    block_data_t    data; /* The block's actual data */
    uint8_t         hash[SHA256_DIGEST_LENGTH]; /* Block's unique hash */
    llist_t         *transactions;  /* List of transactions in the block */
} block_t;

/* Blockchain operations */
blockchain_t *blockchain_create(void);
blockchain_t *blockchain_deserialize(char const *path);
int blockchain_add_block(blockchain_t *blockchain, block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);

/* Block operations */
block_t *block_create(block_t const *prev, int8_t const *data,
                        uint32_t data_len);
block_t *block_deserialize(FILE *file);
void block_destroy(block_t *block);
int block_is_valid(block_t const *block, block_t const *prev_block);
uint8_t *block_hash(block_t const *block,
                    uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/* Serialization functions */
int serialize_block_to_file(llist_node_t node, unsigned int index, void *arg);
int read_block_header(FILE *file, uint32_t *block_count);
void cleanup_blockchain(blockchain_t *blockchain, FILE *file);
void *close_file_and_return_null(FILE *file);


/* Block mining Functions */
int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
    uint32_t difficulty);
void block_mine(block_t *block);
uint32_t blockchain_difficulty(blockchain_t const *blockchain);

#endif /* _BLOCKCHAIN_H_ */
