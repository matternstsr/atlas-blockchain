#include "hblk_crypto.h"

/**
* sha256 - Computes the hash of a sequence of bytes using SHA-256.
* @s: The sequence of bytes to hash.
* @len: The number of bytes in @s.
* @digest: Buffer to store the resulting hash.
*
* Return: A pointer to @digest, or NULL if @digest is NULL.
*/
uint8_t *sha256(int8_t const *s, size_t len,
				uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!s || !digest)
		return (NULL);
	/* Compute the SHA-256 hash */
	if (!SHA256((unsigned char const *)s, len, digest))
		return (NULL);
	return (digest);
}
