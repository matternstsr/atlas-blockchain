#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdio.h>
#include <stdlib.h>

EC_KEY *ec_load(char const *folder)
{
    char priv_key_path[512], pub_key_path[512];
    FILE *priv_key_file = NULL, *pub_key_file = NULL;
    EC_KEY *ec_key = NULL;

    /* Construct the file paths */
    snprintf(priv_key_path, sizeof(priv_key_path), "%s/key.pem", folder);
    snprintf(pub_key_path, sizeof(pub_key_path), "%s/key_pub.pem", folder);

    /* Load the private key */
    priv_key_file = fopen(priv_key_path, "r");
    if (!priv_key_file)
    {
        fprintf(stderr, "Error: Unable to open private key file: %s\n", priv_key_path);
        return NULL;
    }

    ec_key = PEM_read_ECPrivateKey(priv_key_file, NULL, NULL, NULL);
    fclose(priv_key_file);
    if (!ec_key)
    {
        fprintf(stderr, "Error: Unable to load private key from %s\n", priv_key_path);
        return NULL;
    }

    /* Load the public key */
    pub_key_file = fopen(pub_key_path, "r");
    if (!pub_key_file)
    {
        fprintf(stderr, "Error: Unable to open public key file: %s\n", pub_key_path);
        EC_KEY_free(ec_key);
        return NULL;
    }

    if (!PEM_read_ECPublicKey(pub_key_file, &ec_key, NULL, NULL))
    {
        fprintf(stderr, "Error: Unable to load public key from %s\n", pub_key_path);
        fclose(pub_key_file);
        EC_KEY_free(ec_key);
        return NULL;
    }

    fclose(pub_key_file);

    /* Success */
    return ec_key;
}
