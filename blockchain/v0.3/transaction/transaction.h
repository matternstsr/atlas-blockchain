#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "provided/endianness.h"
#include "transaction/transaction.h"
#include <llist.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <time.h>

/* Structs */

/**
* struct tx_out_s - Transaction output
*
* @amount: Amount received
* @pub:    Receiver's public address
* @hash:   Hash of @amount and @pub. Serves as output ID
*/
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t, to_t;

/* Prototypes */
to_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif
