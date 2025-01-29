#include "hblk_crypto.h"

/**
* ec_from_pub - Creates an EC_KEY structure from a public key
* @pub: The public key to convert
*
* Return: A pointer to the created EC_KEY structure, or NULL on failure
*/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_KEY *key = NULL;
	const EC_GROUP *group = NULL;
	EC_POINT *point = NULL;
	BN_CTX *ctx = NULL;

	if (!pub)
		return (NULL); /* Create a new EC_KEY struct using secp256k1 curve */
	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
		return (NULL);
	group = EC_KEY_get0_group(key);
	if (!group)
	{
		EC_KEY_free(key);
		return (NULL);
	}/* Create a new EC_POINT structure from the public key */
	point = EC_POINT_new(group);
	if (!point)
	{
		EC_KEY_free(key);
		return (NULL);
	}
	ctx = BN_CTX_new();
	if (!ctx)
	{
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}/* Convert the public key (uncompressed format) to EC_POINT */
	if (EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, ctx) != 1)
	{
		BN_CTX_free(ctx);
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}/* Set the public key of the EC_KEY structure */
	if (EC_KEY_set_public_key(key, point) != 1)
	{
		BN_CTX_free(ctx);
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}/* Cleanup */
	BN_CTX_free(ctx);
	EC_POINT_free(point);
	return (key);
}
