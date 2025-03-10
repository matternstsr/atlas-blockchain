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

/* Macros */
#define COINBASE_AMOUNT 50
#define BLOCKCHAIN_DATA_MAX 1024
#define BUFF_SIZE ((32 * 3 * ins) + (32 * outs))
#define PTR_MOVE (sizeof(uint32_t) + EC_PUB_LEN)
#define UNSPENT ((uto_t *)unspent)
#define CONTEXT ((tc_t *)context)


/* Structs */

/**
 * struct transaction_s - Transaction structure
 *
 * @id:      Transaction ID. A hash of all the inputs and outputs.
 *           Prevents further alteration of the transaction.
 * @inputs:  List of `tx_in_t *`. Transaction inputs
 * @outputs: List of `tx_out_t *`. Transaction outputs
 */
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;

/**
 * struct tx_out_s - Transaction output
 *
 * @amount: Amount received
 * @pub:    Receiver's public address
 * @hash:   Hash of @amount and @pub. Serves as output ID
 */
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t, to_t;

/**
 * struct tx_in_s - Transaction input
 *
 * Description: A transaction input always refers to a previous
 * transaction output. The only exception is for a Coinbase transaction, that
 * adds new coins to ciruclation.
 *
 * @block_hash:  Hash of the Block containing the transaction @tx_id
 * @tx_id:       ID of the transaction containing @tx_out_hash
 * @tx_out_hash: Hash of the referenced transaction output
 * @sig:         Signature. Prevents anyone from altering the content of the
 *               transaction. The transaction input is signed by the receiver
 *               of the referenced transaction output, using their private key
 */
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t       sig;
} tx_in_t, ti_t;

/**
 * struct unspent_tx_out_s - Unspent transaction output
 *
 * Description: This structure helps identify transaction outputs that were not
 * used in any transaction input yet, making them "available".
 *
 * @block_hash: Hash of the Block containing the transaction @tx_id
 * @tx_id:      ID of the transaction containing @out
 * @out:        Copy of the referenced transaction output
 */
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;
} unspent_tx_out_t, uto_t;

/**
 * struct tx_context_s - Tracks the balance available to a private key
 * @pub: public key used to match to a private
 * @balance: total amount available to a key
 * @needed: amount needed to send
 * @tx: transaction struct
 * @sender: senders private key
 * @all_unspent: list of uto_t
 */
typedef struct tx_context_s
{
	uint8_t       pub[EC_PUB_LEN];
	int           balance;
	int           needed;
	transaction_t *tx;
	EC_KEY const  *sender;
	llist_t       *all_unspent;
} tc_t;

/**
 * struct tx_valid_s - HOlds info for validating a transaction
 * @input: input amounts
 * @output: output amounts
 * @tx_id: transaction id
 * @unspent: list of uto_t
 */
typedef struct tx_valid_s
{
	uint32_t   input;
	uint32_t   output;
	uint8_t    tx_id[SHA256_DIGEST_LENGTH];
	llist_t    *unspent;
} tv_t;

/**
 * struct update_list_s - HOlds info for updating unspent
 * @hash: hash of block holding tx
 * @unspent: list of uto_t
 * @tx_id: transaction ID
 */
typedef struct update_list_s
{
	uint8_t    hash[SHA256_DIGEST_LENGTH];
	llist_t    *unspent;
	uint8_t    tx_id[SHA256_DIGEST_LENGTH];
} ul_t;

/* Prototypes */

to_t *tx_out_create(
	uint32_t amount, uint8_t const pub[EC_PUB_LEN]);
uto_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH],
	to_t const *out);
ti_t *tx_in_create(uto_t const *unspent);
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
sig_t *tx_in_sign(
	ti_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender,
	llist_t *all_unspent);
transaction_t *transaction_create(
	EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
	llist_t *all_unspent);
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent);
transaction_t *coinbase_create(
	EC_KEY const *receiver, uint32_t block_index);
int coinbase_is_valid(
	transaction_t const *coinbase, uint32_t block_index);
void transaction_destroy(transaction_t *transaction);
llist_t *update_unspent(
	llist_t *transactions, uint8_t block_hash[SHA256_DIGEST_LENGTH],
	llist_t *all_unspent);


#endif
