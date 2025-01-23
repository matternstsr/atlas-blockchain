#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include "hblk_crypto.h"

/**
 * ec_load - loads an EC key pair from disk
 * @folder: path to the folder from which to load the keys
 * 
 * Return: pointer to the created EC key pair on success, or NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
    char path[1024];
    FILE *fp;
    EC_KEY *key = NULL;

    if (!folder)
        return (NULL);

        /* Load the public key */
    snprintf(path, sizeof(path), "%s/key_pub.pem", folder);
    fp = fopen(path, "r");
    if (!fp)
    {
        EC_KEY_free(key);
        return (NULL);
    }
    if (!PEM_read_EC_PUBKEY(fp, &key, NULL, NULL))
    {
        fclose(fp);
        EC_KEY_free(key);
        return (NULL);
    }
    fclose(fp);

    /* Load the private key */
    snprintf(path, sizeof(path), "%s/key.pem", folder);
    fp = fopen(path, "r");
    if (!fp)
        {
        printf("stopped1\n");
        return (NULL);
        }
    key = PEM_read_ECPrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!key)
        {
        printf("stopped2\n");
        return (NULL);
        }

    return (key);
}
