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
			  sig_t const *sig)
{
	if (!key || !msg || !sig || !sig->len)
		return (0);

	return (ECDSA_verify(0, msg, msglen, sig->sig, sig->len, (EC_KEY *)key));
}

/**
* ec_sign - Signs a message using a given EC_KEY private key
* @key: Pointer to the EC_KEY structure containing the private key
* @msg: Pointer to the message to be signed
* @msglen: Length of the message
* @sig: Pointer to sig_t struct to store the signature
* Return: Pointer to signature buffer on success, NULL on failure
*/
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
					sig_t *sig)
{
	unsigned int sig_len;

	if (!key || !msg || !sig)
		return (NULL);

	if (!ECDSA_sign(0, msg, msglen, sig->sig, &sig_len, (EC_KEY *)key))
	{
		free(sig->sig);
		sig->sig = NULL; /* Ensure we reset the pointer on failure */
		return (NULL);
	}
	sig->len = sig_len;
	return (sig->sig);
}
