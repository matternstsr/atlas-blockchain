#include "hblk_crypto.h"

/**
* ec_sign - Signs the given EC_KEY
* @key: key to be signed
* @msg: characters to be signed
* @msglen: number of characters to sign
* @sig: address to hold signiture
* Return: pointer to sig or NULL
*/
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sig_t *sig)
{
	if (!key || !msg)
		return (NULL);

	if (!ECDSA_sign(0, msg, msglen, sig->sig, (unsigned int *)&sig->len,
					(EC_KEY *)key))
	{
		sig->len = 0;
		return (NULL);
	}

	return (sig->sig);
}
