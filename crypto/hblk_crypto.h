#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <openssl/ec.h>
#include <openssl/pem.h>
#include <stdint.h>
#include <stddef.h>

/* Define the length of the public key */
#define EC_PUB_LEN 65
#define PUB_FILENAME "key_pub.pem"
#define PRI_FILENAME "key.pem"

/* Function declarations */

/* EC Key Pair Functions */
EC_KEY *ec_create(void);
int ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);
int ec_save(EC_KEY *key, char const *folder);
EC_KEY *ec_load(char const *folder);

#endif /* HBLK_CRYPTO_H */
