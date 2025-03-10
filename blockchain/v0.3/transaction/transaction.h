#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdint.h>
#include <stddef.h>
#include "hblk_crypto.h"

/* Len of the pub key for an elliptic curve (if 33 bytes for copmr format) */
/* #define EC_PUB_LEN 33 */

/**
* struct tx_out_s - Represents a transaction output
*
* @amount: The amount of cryptocurrency being transferred
* @pub: The public key of the recipient
* @hash: The hash of the transaction output
*/
typedef struct tx_out_s
{
	uint32_t amount;           /* Amount being transferred */
	uint8_t pub[EC_PUB_LEN];   /* Public key of the recipient */
	uint8_t hash[SHA256_DIGEST_LENGTH]; /* SHA256 hash of the trans output */
} tx_out_t;

/* Function prototypes */
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif /* TRANSACTION_H */
