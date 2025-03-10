#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <openssl/ec.h> /* For EC_KEY and EC_PUB_LEN */
#include <stdint.h> /* For uint8_t, uint32_t */

/* Length of an EC public key */
#define EC_PUB_LEN 33

/**
* ec_create - Creates a new EC key pair
*
* Return: A pointer to the EC_KEY, or NULL on failure
*/
EC_KEY *ec_create(void);

/**
* ec_to_pub - Extracts the public key from an EC key pair
* @key: The EC key pair
* @pub: The buffer to store the public key
*
* Return: A pointer to the public key buffer, or NULL on failure
*/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);

#endif /* HBLK_CRYPTO_H */
