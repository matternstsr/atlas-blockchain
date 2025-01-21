#include <stdio.h>
#include <stdlib.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "hblk_crypto.h"

EC_KEY *ec_load(char const *folder)
{
	char priv_key_path[256];
	char pub_key_path[256];
	EC_KEY *key = NULL;
	FILE *priv_file = NULL, *pub_file = NULL;

	snprintf(priv_key_path, sizeof(priv_key_path), "%s/key.pem", folder);
	snprintf(pub_key_path, sizeof(pub_key_path), "%s/key_pub.pem", folder);

	priv_file = fopen(priv_key_path, "r");
	if (!priv_file)
	{
		perror("Failed to open private key file");
		return NULL;
	}
	key = PEM_read_ECPrivateKey(priv_file, NULL, NULL, NULL);
	fclose(priv_file);
	if (!key)
	{
		fprintf(stderr, "Failed to load private key from %s\n", priv_key_path);
		return NULL;
	}
	pub_file = fopen(pub_key_path, "r");
	if (!pub_file)
	{
		fprintf(stderr, "Failed to open public key file\n");
		EC_KEY_free(key);
		return NULL;
	}
	if (!PEM_read_ECPublicKey(pub_file, &key, NULL, NULL))
	{
		fprintf(stderr, "Failed to load public key from %s\n", pub_key_path);
		fclose(pub_file);
		EC_KEY_free(key);
		return NULL;
	}
	fclose(pub_file);
	return key;
}
