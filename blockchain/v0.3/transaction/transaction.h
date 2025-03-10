#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h> /* for uint32_t, uint8_t */
#include <openssl/ec.h> /* for EC_KEY and EC_PUB_LEN */

/* Length of an EC public key (this is a constant, typically 33 bytes for compressed EC keys) */
#define EC_PUB_LEN 33

/* Length of the transaction output hash (SHA256 length) */
#define HASH_LEN 32

/**
* struct tx_out - Structure for a transaction output
* @amount: The amount of cryptocurrency sent in this output
* @pub: The public key of the recipient
* @hash: A SHA-256 hash derived from the amount and public key
*/
typedef struct tx_out
{
	uint32_t amount;  /* Amount of cryptocurrency being sent */
	uint8_t pub[EC_PUB_LEN]; /* Public key of the recipient */
	uint8_t hash[HASH_LEN];  /* Hash of the transaction output */
} tx_out_t;

/* Function Prototypes */

/**
* tx_out_create - Allocates and initializes a transaction output structure
* @amount: The amount to be sent
* @pub: The public key of the recipient
* 
* Return: A pointer to the created transaction output, or NULL on failure
*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif /* TRANSACTION_H */
