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
    FILE *fp = NULL;
    EC_KEY *key = NULL;

    if (!folder)
    {
        fprintf(stderr, "Folder path is NULL\n");
        return (NULL);
    }

    /* Load the private key */
    snprintf(path, sizeof(path), "%s/%s", folder, PRI_FILENAME);
    printf("Attempting to load private key from: %s\n", path);
    fp = fopen(path, "r");
    if (!fp)
    {
        perror("Failed to open private key file");
        return (NULL);
    }
    key = PEM_read_ECPrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!key)
    {
        fprintf(stderr, "Failed to load private key from file\n");
        return (NULL);
    }
    printf("Private key loaded successfully\n");

    /* Load the public key */
    snprintf(path, sizeof(path), "%s/%s", folder, PUB_FILENAME);
    printf("Attempting to load public key from: %s\n", path);
    fp = fopen(path, "r");
    if (!fp)
    {
        perror("Failed to open public key file");
        EC_KEY_free(key);
        return (NULL);
    }
    if (!PEM_read_EC_PUBKEY(fp, &key, NULL, NULL))
    {
        fclose(fp);
        fprintf(stderr, "Failed to load public key from file\n");
        EC_KEY_free(key);
        return (NULL);
    }
    fclose(fp);
/*     printf("Public key loaded successfully\n"); */
    return (key);
}
