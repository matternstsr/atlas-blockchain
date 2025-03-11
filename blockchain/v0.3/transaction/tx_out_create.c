#include "transaction.h"

/**
* tx_out_create - Creates a new transaction output struct.
* @pub: Public key associated with the transaction output.
* @amount: Amount of the transaction output.
* Return: Pointer to the new struct or NULL in case of failure.
*/
to_t *tx_out_create(uint32_t amount, const uint8_t pub[EC_PUB_LEN])
{
	to_t *new_output = NULL;

	/* Check if public key or amount is invalid */
	if (!pub || amount == 0)
		return NULL;

	/* Allocate memory for the new transaction output struct */
	new_output = calloc(1, sizeof(to_t));
	if (!new_output)
		return NULL;

	/* Assign the transaction output values */
	new_output->amount = amount;
	memcpy(new_output->pub, pub, EC_PUB_LEN);

	/* Compute the hash for the transaction output */
	SHA256((uint8_t *)new_output, sizeof(uint32_t) + EC_PUB_LEN, new_output->hash);

	return new_output;
}
