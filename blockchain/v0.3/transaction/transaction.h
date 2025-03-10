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

/* Structs */

/**
* struct tx_out_s - Transaction output structure
*
* @amount: Amount of the transaction output
* @pub:    Public key (recipient's address)
* @hash:   Hash of the output, calculated based on @amount and @pub
*/
typedef struct tx_out_s
{
	uint32_t    amount;      /* Amount to be sent in this output */
	uint8_t     pub[EC_PUB_LEN];  /* Public key of recipient */
	uint8_t     hash[SHA256_DIGEST_LENGTH];/* Hash of output for unique ident */
} tx_out_t;


/* Function Prototypes */
tx_out_t *tx_out_create(uint32_t amount, const uint8_t pub[EC_PUB_LEN]);

#endif  /* _TRANSACTION_H_ */
