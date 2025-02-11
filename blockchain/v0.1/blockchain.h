#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <openssl/sha.h>
#include "llist.h"


/* Constants */
#define BLOCKCHAIN_DATA_MAX 1024  /* Maximum allowed data size for a block */
#define HBLK_MAGIC_NUMBER 0x48424c4b  /* Magic number for HBLK format */
#define HBLK_VERSION 0x01           /* Version number */
#define MAGIC_NUMBER "HBLK"
#define VERSION "0.1"

/* Enum for block types */
typedef enum block_status {
	BLOCK_STATUS_OK = 0,
	BLOCK_STATUS_INVALID,
} block_status_t;

/* Structure for storing Block information */
typedef struct block_info {
	uint32_t index;          /* Block index */
	uint32_t difficulty;     /* Difficulty (not used in this iteration) */
	uint64_t timestamp;      /* Timestamp of block creation */
	uint8_t prev_block_hash[32]; /* Previous block hash */
	uint8_t hash[32];        /* Current block hash */
} block_info_t;

/* Structure for the Blockchain */
typedef struct blockchain {
	size_t size;            /* Number of blocks in the blockchain */
	block_t *genesis;       /* Genesis block */
	llist_t *chain;         /* List of blocks in the chain */
} blockchain_t;

/* Structure for storing the Block data */
typedef struct block {
	block_info_t info;      /* Block info */
	uint8_t *data;          /* Block data */
	size_t data_len;        /* Length of data */
	uint8_t hash[32];       /* Hash of the block */
    uint32_t index;          // Block index
    uint32_t difficulty;     // Difficulty of the block
    uint64_t timestamp;      // Timestamp of the block
    uint32_t nonce;          // Nonce for proof of work
    uint8_t prev_hash[HASH_SIZE]; // Previous block hash
    struct {
        uint8_t buffer[BLOCKCHAIN_DATA_MAX]; // Block data
        size_t len;                          // Length of data
    } data;
    uint8_t hash[HASH_SIZE];  // Block hash
} block_t;


/* Functions for blockchain operations */
blockchain_t *blockchain_create(void);           /* Create a new blockchain */
int blockchain_destroy(blockchain_t *blockchain); /* Destroy a blockchain */
int blockchain_serialize(blockchain_t const *blockchain, char const *path); /* Serialize blockchain */
blockchain_t *blockchain_deserialize(char const *path);  /* Deserialize blockchain */

/* Functions for block operations */
block_t *block_create(block_t const *prev_block, uint8_t const *data, size_t data_len);  /* Create a new block */
int block_is_valid(block_t const *block, block_t const *prev_block);  /* Validate a block */
int block_hash(block_t *block, uint8_t *hash_out); /* Compute block hash */
void block_destroy(block_t *block);  /* Destroy a block */

/* Utility function for printing the blockchain (for debugging) */
void _blockchain_print_brief(blockchain_t const *blockchain);

#endif /* BLOCKCHAIN_H */
