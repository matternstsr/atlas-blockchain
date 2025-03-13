#include "transaction.h"

/**
* tx_out_create - Creates a new transaction output struct.
* @pub: Public key associated with the transaction output.
* @amount: Amount of the transaction output.
* Return: Pointer to the new struct or NULL in case of failure.
*/
to_t *tx_out_create(uint32_t amount, const uint8_t pub[EC_PUB_LEN])
{
	/* Check if public key or amount is invalid */
	if (!pub || amount == 0)
		return (NULL);

	/* Allocate memory for the new transaction output struct */
	to_t *tx_out = calloc(1, sizeof(to_t));

	if (!tx_out)
		return (NULL);

	/* Initialize the transaction output with the provided values */
	tx_out->amount = amount;
	memcpy(tx_out->pub, pub, EC_PUB_LEN);

	/* Calculate and assign the hash for the transaction output */
	SHA256((uint8_t *)tx_out, sizeof(tx_out->amount) + EC_PUB_LEN, tx_out->hash);

	return (tx_out);
}
