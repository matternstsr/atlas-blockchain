#include "transaction.h"

/**
* tx_out_create - Create a transaction output (tx_out_t)
* @amount: Amount to send to the recipient
* @pub: Public key of the recipient
* This function creates a new transaction output structure (tx_out_t),
* computes its hash based on the amount and recipient's public key.
* Return: ptr to the new tx_out_t structure or NULL if memory allocation fails.
*/
tx_out_t *tx_out_create(uint32_t amount, const uint8_t pub[EC_PUB_LEN])
{
	tx_out_t *new_out = NULL;

	/* Validate input parameters */
	if (amount == 0 || pub == NULL)
		return (NULL);

	/* Allocate memory for a new transaction output */
	new_out = malloc(sizeof(tx_out_t));
	if (!new_out)
		return (NULL);  /* Memory allocation failed */

	/* Initialize structure fields */
	new_out->amount = amount;
	memcpy(new_out->pub, pub, EC_PUB_LEN);

	/* Calculate the hash for this output */
	SHA256_CTX sha_ctx;

	SHA256_Init(&sha_ctx);
	SHA256_Update(&sha_ctx, &new_out->amount, sizeof(new_out->amount));
	SHA256_Update(&sha_ctx, new_out->pub, EC_PUB_LEN);
	SHA256_Final(new_out->hash, &sha_ctx);

	/* Return the created transaction output */
	return (new_out);
}
