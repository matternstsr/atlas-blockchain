#include "transaction.h"
#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>
#include "blockchain.h"

/**
* tx_out_create - Allocates and initializes a transaction output structure
* @amount: The amount to be sent
* @pub: The public key of the recipient
* 
* Return: A pointer to the created transaction output, or NULL on failure
*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *out;
	uint8_t data[EC_PUB_LEN + sizeof(amount)];
	unsigned char hash[SHA256_DIGEST_LENGTH];

	/* Allocate memory for the transaction output */
	out = malloc(sizeof(tx_out_t));
	if (!out)
		return (NULL);

	/* Initialize the amount */
	out->amount = amount;

	/* Copy the public key */
	memcpy(out->pub, pub, EC_PUB_LEN);

	/* Prepare data for hashing: concatenate amount and pub */
	memcpy(data, &amount, sizeof(amount));
	memcpy(data + sizeof(amount), pub, EC_PUB_LEN);

	/* Generate a SHA256 hash */
	SHA256(data, sizeof(data), hash);

	/* Store the hash in the transaction output */
	memcpy(out->hash, hash, sizeof(out->hash));

	return (out);
}
