#include "transaction.h"
#include "hblk_crypto.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <openssl/sha.h>

/**
* tx_out_create - Create a transaction output
*
* @amount: The amount of the transaction
* @pub: The public key of the recipient
*
* Return: Pointer to the created transaction output, or NULL on failure
*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *out = NULL;

	/* Allocate memory for the transaction output */
	out = malloc(sizeof(tx_out_t));
	if (!out)
		return (NULL);

	/* Set the amount */
	out->amount = amount;

	/* Copy the recipient's public key into the structure */
	memcpy(out->pub, pub, EC_PUB_LEN);

	/* Compute the hash of the transaction output */
	SHA256_CTX sha256_ctx;

	SHA256_Init(&sha256_ctx);
	SHA256_Update(&sha256_ctx, &out->amount, sizeof(out->amount));
	SHA256_Update(&sha256_ctx, out->pub, EC_PUB_LEN);
	SHA256_Final(out->hash, &sha256_ctx);

	return (out);
}
