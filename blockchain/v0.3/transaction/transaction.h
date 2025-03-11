#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <llist.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#define COINBASE_REWARD 50
#define MAX_BLOCKCHAIN_SIZE 1024
#define TRANSACTION_BUFFER_SIZE ((32 * 3 * num_inputs) + (32 * num_outputs))
#define POINTER_SHIFT (sizeof(uint32_t) + EC_PUB_LEN)
#define AVAILABLE_FUNDS ((unspent_tx_out_t *)unspent)
#define TX_CONTEXT ((transaction_context_t *)context)

#define SUCCESS_CODE 0
#define MAX_SIGNATURE_SIZE 64  /* For signature length assumption */

/* Structs */

/**
 * struct transaction_s - Defines the structure of a transaction
 *
 * @id:      The hash representing the transaction's uniqueness
 * @inputs:  A list of transaction inputs, each referencing previous unspent outputs
 * @outputs: A list of transaction outputs, each representing funds sent to a recipient
 */
typedef struct transaction_s
{
    uint8_t id[SHA256_DIGEST_LENGTH];  /* Transaction ID (hash of inputs + outputs) */
    llist_t *inputs;                   /* Linked list of transaction inputs */
    llist_t *outputs;                  /* Linked list of transaction outputs */
} transaction_t;

/**
 * struct transaction_output_s - Represents an individual output in a transaction
 *
 * @amount: The value being sent to the recipient
 * @pub_key: The recipient's public key (address)
 * @hash: A hash representing the output, calculated from the amount and public key
 */
typedef struct transaction_output_s
{
    uint32_t amount;
    uint8_t pub_key[EC_PUB_LEN];
    uint8_t hash[SHA256_DIGEST_LENGTH];
} transaction_output_t, tx_out_t;

/**
 * struct transaction_input_s - Represents an individual input in a transaction
 *
 * @block_hash: The hash of the block containing the referenced transaction
 * @tx_id: The ID of the transaction containing the referenced output
 * @output_hash: The hash of the specific output being spent
 * @signature: A digital signature proving ownership of the output
 */
typedef struct transaction_input_s
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t tx_id[SHA256_DIGEST_LENGTH];
    uint8_t output_hash[SHA256_DIGEST_LENGTH];
    sig_t signature;
} transaction_input_t, tx_in_t;

/**
 * struct unspent_output_s - Represents an unspent transaction output
 *
 * @block_hash: Hash of the block containing the referenced transaction
 * @tx_id: Transaction ID that contains the unspent output
 * @output: The actual transaction output
 */
typedef struct unspent_output_s
{
    uint8_t block_hash[SHA256_DIGEST_LENGTH];
    uint8_t tx_id[SHA256_DIGEST_LENGTH];
    transaction_output_t output;
} unspent_output_t, unspent_tx_out_t;

/**
 * struct transaction_context_s - Contains the context for creating a transaction
 * @pub_key: Public key associated with the sender's private key
 * @balance: Total funds available to the sender
 * @required: Funds required to complete the transaction
 * @tx: The transaction object being created
 * @sender_private_key: The sender's private key for signing
 * @all_unspent_outputs: List of all unspent transaction outputs
 */
typedef struct transaction_context_s
{
    uint8_t pub_key[EC_PUB_LEN];        /* Public key */
    int balance;                        /* Total balance available */
    int required;                       /* Funds needed for transaction */
    transaction_t *tx;                  /* The transaction being created */
    EC_KEY const *sender_private_key;   /* Sender's private key */
    llist_t *all_unspent_outputs;       /* List of all unspent transaction outputs */
} transaction_context_t;

/* Function Prototypes */
sig_t *tx_input_sign(transaction_input_t *input, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender, llist_t *all_unspent);
transaction_input_t *tx_input_create(const unspent_tx_out_t *unspent);
unspent_tx_out_t *unspent_output_create(uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH], const transaction_output_t *out);
transaction_output_t *tx_output_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
uint8_t *transaction_hash(transaction_t const *tx, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int input_hash(llist_node_t input, unsigned int idx, void *buff);
int output_hash(llist_node_t output, unsigned int idx, void *buff);
transaction_t *create_transaction(EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent);
int generate_outputs(uint32_t amount, transaction_context_t *context, EC_KEY const *receiver);
int match_unspent_outputs(llist_node_t unspent, unsigned int idx, void *context);
int sign_transaction_inputs(llist_node_t tx_input, unsigned int idx, void *context);
int process_unspent_outputs(llist_node_t unspent, unsigned int idx, void *context);
int send_transaction(uint32_t amount, transaction_context_t *context, EC_KEY const *receiver);
void sign_transaction_input(llist_node_t tx_input, unsigned int idx, void *context);

#endif /* _TRANSACTION_H_ */
