#include "hblk_crypto.h"

/**
* ec_verify - Verifies the signature of a given message
* @key: Pointer to the EC_KEY structure containing the public key
* @msg: Pointer to the message to verify
* @msglen: Length of the message
* @sig: Pointer to sig_t struct containing the signature
*
* Return: 1 if signature is valid, 0 otherwise
*/
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
				sig_t const *sig);
{
	if (!key || !msg || !sig || !sig->sig || sig->len == 0)
		return (0);

	return ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key) == 1;
}