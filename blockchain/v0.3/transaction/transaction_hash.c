#include "transaction.h"

int hash_in(llist_node_t input, unsigned int iter, void *buff);
int hash_out(llist_node_t output, unsigned int iter, void *buff);

/**
* transaction_hash - Calculates the hash of a transaction
* @transaction: transaction to hash
* @hash_buf: buffer to hold the hash
* Return: pointer to hash_buff or NULL
*/
uint8_t *transaction_hash(
	transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	int ins, outs;
	uint8_t *buff = NULL;

	/* Validate the input parameters */
	if (!transaction || !hash_buf)
		return NULL;

	/* Get the number of inputs and outputs in the transaction */
	ins = llist_size(transaction->inputs);
	outs = llist_size(transaction->outputs);

	/* Allocate a buffer to hold the combined transaction data */
	buff = calloc(1, BUFF_SIZE);
	if (!buff)
		return NULL;

	/* Hash the transaction inputs and outputs */
	llist_for_each(transaction->inputs, hash_in, buff);
	llist_for_each(transaction->outputs, hash_out, &buff[32 * 3 * ins]);

	/* Compute the final transaction hash */
	SHA256(buff, BUFF_SIZE, hash_buf);

	/* Clean up and return the hash buffer */
	free(buff);
	buff = NULL;
	return (hash_buf);
}

/**
* hash_in - reads inputs into a buffer for hashing
* @input: node in list
* @iter: Iteration index in list
* @buff: Buffer to read into
* Return: returns 0 on success, 1 on fail
*/
int hash_in(llist_node_t input, unsigned int iter, void *buff)
{
	unsigned int pos = iter * (32 * 3);

	/* Copy the input data into the buffer at the correct position */
	if (memcpy(&((uint8_t *)buff)[pos], input, 96))
		return (0);
	return (1);
}

/**
* hash_out - reads outputs into a buffer for hashing
* @output: node in list
* @iter: Iteration index in list
* @buff: Buffer to read into
* Return: returns 0 on success, 1 on fail
*/
int hash_out(llist_node_t output, unsigned int iter, void *buff)
{
	unsigned int pos = iter * 32;

	/* Copy the output data into the buffer at the correct position */
	if (memcpy(&((uint8_t *)buff)[pos], (uint8_t *)output + PTR_MOVE, 32))
		return (0);
	return (1);
}
