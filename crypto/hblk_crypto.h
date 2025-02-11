#ifndef HBLK_CRYPTO_H
#define HBLK_CRYPTO_H

#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/* Define the length of the public key */
#define EC_PUB_LEN 65
#define EC_CURVE NID_secp256k1
#define PUB_FILENAME "key_pub.pem"
#define PRI_FILENAME "key.pem"
#define bzero(ptr, size) memset(ptr, 0, size)


#define MAX_SIG_LEN 72

/**
* struct sig_s - Structure for representing an ECDSA signature
* @sig: Fixed-size array holding the signature data
* @len: Length of the signature (actual length of data in @sig)
*/
typedef struct sig_s
{
	uint8_t sig[MAX_SIG_LEN];
	size_t len;
} sig_t;

/* Function declarations */
EC_KEY *ec_create(void);
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);
int ec_save(EC_KEY *key, char const *folder);
EC_KEY *ec_load(char const *folder);
uint8_t *sha256(int8_t const *s, size_t len,
				uint8_t digest[SHA256_DIGEST_LENGTH]);
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sig_t const *sig);
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
					sig_t *sig);

#endif /* HBLK_CRYPTO_H */
