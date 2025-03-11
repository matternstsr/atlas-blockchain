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
#define SIG_MAX_LEN 64 
#define PTR_MOVE (sizeof(uint32_t) + EC_PUB_LEN)
#define UNSPENT ((uto_t *)unspent)
#define CONTEXT ((tc_t *)context)


/* Structs */

/**
* struct transaction_s - Represents a transaction
*
* @id:      Transaction identifier, a hash of all the inputs and outputs.
*           This ensures the integrity of the transaction, preventing further modification.
* @inputs:  List of `tx_in_t *` structures. Represents the transaction's inputs.
* @outputs: List of `tx_out_t *` structures. Represents the transaction's outputs.
*/
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;

/**
* struct tx_out_s - Represents a transaction output
*
* @amount: The amount being transferred to the recipient
* @pub:    The public key address of the recipient
* @hash:   A hash of @amount and @pub, used as the unique identifier for the output
*/
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t, to_t;

/**
* struct tx_in_s - Represents a transaction input
*
* Description: A transaction input typically references a previous transaction output,
* unless it is a Coinbase transaction, which introduces new coins into circulation.
*
* @block_hash:  The hash of the block that contains the transaction identified by @tx_id
* @tx_id:       The ID of the transaction containing the referenced output
* @tx_out_hash: The hash of the specific transaction output being referenced
* @sig:         A signature that prevents any alterations to the transaction's content.
*               The input is signed by the recipient of the referenced output using their private key.
*/
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t       sig;
} tx_in_t, ti_t;

/**
* struct unspent_tx_out_s - Represents an unspent transaction output
*
* Description: This structure identifies transaction outputs that have not yet been 
* consumed in a transaction input, meaning they are still available for spending.
*
* @block_hash: Hash of the block containing the transaction identified by @tx_id
* @tx_id:      The ID of the transaction containing the unspent output
* @out:        A copy of the referenced transaction output
*/
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;
} unspent_tx_out_t, uto_t;

/**
* struct tx_context_s - Tracks the balance available for a specific private key
* @pub: Public key used to match with a private key
* @balance: The total available balance for the key
* @needed: The amount required for sending
* @tx: The transaction structure
* @sender: The sender's private key
* @all_unspent: A list of unspent transaction outputs (uto_t)
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
* struct tx_valid_s - Holds information to validate a transaction
* @input: The total amount from transaction inputs
* @output: The total amount from transaction outputs
* @tx_id: The transaction ID
* @unspent: A list of unspent transaction outputs (uto_t)
*/
typedef struct tx_valid_s
{
	uint32_t   input;
	uint32_t   output;
	uint8_t    tx_id[SHA256_DIGEST_LENGTH];
	llist_t    *unspent;
} tv_t;

/**
* struct update_list_s - Holds information for updating the list of unspent outputs
* @hash: The hash of the block containing the transaction
* @unspent: The list of unspent transaction outputs (uto_t)
* @tx_id: The transaction ID
*/
typedef struct update_list_s
{
	uint8_t    hash[SHA256_DIGEST_LENGTH];
	llist_t    *unspent;
	uint8_t    tx_id[SHA256_DIGEST_LENGTH];
} ul_t;

/* Prototypes */

/**
 * tx_in_sign - Signs a transaction input after verifying key
 * @in: Transaction input
 * @tx_id: hash of transaction holding tx_input
 * @sender: private key of receiver
 * @all_unspent: list of all unspent transactions
 * Return: hash holding the signature or NULL
 */
sig_t *tx_in_sign(
	ti_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender,
	llist_t *all_unspent);

/**
* tx_in_create - creates a transaction input struct
* @unspent: pointer to unspent transaction to be used
* Return: NULL or pointer to new transaction input struct
*/
ti_t *tx_in_create(const uto_t *unspent);

/**
* unspent_tx_out_create - creates an unspent transaction struct
* @block_hash: hash of block where transaction is at
* @tx_id: Transaction ID
* @out: Transaction output
* Return: NULL or pointer to new unspent transaction
*/
uto_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH], uint8_t tx_id[SHA256_DIGEST_LENGTH],
	const to_t *out);

/**
* tx_out_create - Creates a new transaction output struct.
* @pub: Public key associated with the transaction output.
* @amount: Amount of the transaction output.
* Return: Pointer to the new struct or NULL in case of failure.
*/
to_t *tx_out_create(
	uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

/**
 * transaction_hash - Calculates the hash of a transaction
 * @transaction: transaction to hash
 * @hash_buf: buffer to hold the hash
 * Return: pointer to hash_buff or NULL
 */
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH]);

/**
 * hash_in - reads inputs into a buffer for hashing
 * @input: node in list
 * @iter: Iteration index in list
 * @buff: Buffer to read into
 * Return: returns 0 on success, 1 on fail
 */
int hash_in(llist_node_t input, unsigned int iter, void *buff);

/**
 * hash_out - reads outputs into a buffer for hashing
 * @output: node in list
 * @iter: Iteration index in list
 * @buff: Buffer to read into
 * Return: returns 0 on success, 1 on fail
 */
int hash_out(llist_node_t output, unsigned int iter, void *buff);

/**
 * transaction_create - Creates and returns a new transaction.
 * @sender: Private key of the sender.
 * @receiver: Public key of the receiver.
 * @amount: Amount to send.
 * @all_unspent: List of unspent transaction outputs.
 * Return: NULL on failure or a pointer to the new transaction on success.
 */
transaction_t *transaction_create(
	EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
	llist_t *all_unspent);

/**
 * find_matching_input - Finds unspent outputs that match the sender's public key.
 * @unspent: Unspent transaction output.
 * @i: Current iterator index.
 * @context: Context containing sender's public key and transaction details.
 * Return: 0 on success, 1 on failure.
 */
int find_matching_input(llist_node_t unspent, unsigned int i, void *context);

/**
 * generate_outputs - Generates transaction outputs.
 * @amount: Amount to send.
 * @context: Context containing transaction and unspent outputs.
 * @receiver: Receiver's public key.
 * Return: 1 on success, 0 on failure.
 */
int generate_outputs(uint32_t amount, tc_t *context, EC_KEY const *receiver);

/**
 * sign_input - Signs a transaction input.
 * @tx_in: Current transaction input node.
 * @i: Current iterator index.
 * @context: Context holding transaction and unspent outputs.
 * Return: 0 on success, 1 on failure.
 */
int sign_input(llist_node_t tx_in, unsigned int i, void *context);

/**
 * transaction_is_valid - Checks the validity of a transaction.
 * @transaction: The transaction to validate.
 * @all_unspent: List of all unspent transaction outputs.
 * Return: 1 if valid, 0 if invalid.
 */
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent);

/**
 * validate_input - Validates a single transaction input.
 * @in: Transaction input to validate.
 * @i: Iterator for list iteration.
 * @context: Context holding the transaction and unspent outputs.
 * Return: 0 on valid input, 1 on invalid input.
 */
int validate_input(tx_in_t *in, uint32_t i, tv_t *context);

/**
 * check_unspent_match - Checks if the unspent transaction matches the input.
 * @unspent: Unspent transaction output.
 * @in: Transaction input to check.
 * Return: 1 if matched, 0 if not.
 */
int check_unspent_match(uto_t *unspent, tx_in_t *in);

/**
 * sum_output_amount - Accumulates the output amounts of the transaction.
 * @out: Transaction output to check.
 * @i: Iterator for list iteration.
 * @context: Context holding transaction details.
 * Return: Always returns 0.
 */
int sum_output_amount(tx_out_t *out, unsigned int i, tv_t *context);













#endif
