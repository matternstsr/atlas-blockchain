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
    EC_POINT *point = NULL;
    BN_CTX *ctx = NULL;
    const EC_GROUP *group;

    if (!pub || !(key = EC_KEY_new_by_curve_name(EC_CURVE)) ||
			!(group = EC_KEY_get0_group(key)) ||
			!(point = EC_POINT_new(group)) || !(ctx = BN_CTX_new()))
        goto cleanup;

    if (EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, ctx) != 1 ||
							EC_KEY_set_public_key(key, point) != 1)
        goto cleanup;

    BN_CTX_free(ctx);
    EC_POINT_free(point);
    return (key);

cleanup:
    EC_KEY_free(key);
    EC_POINT_free(point);
    BN_CTX_free(ctx);
    return (NULL);
}
