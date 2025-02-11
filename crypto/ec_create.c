#include "hblk_crypto.h"

/**
* ec_create - Creates a new EC key pair using secp256k1 curve.
* Return: A pointer to the created EC_KEY structure, or NULL on failure.
*/
EC_KEY *ec_create(void)
{
	EC_KEY *key = NULL;

	/* Create a new EC key pair */
	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	if (!key)
		return (NULL);
	/* Generate the private and public key pair */
	if (EC_KEY_generate_key(key) != 1)
	{
		EC_KEY_free(key);
		return (NULL);
	}
	return (key);
}
