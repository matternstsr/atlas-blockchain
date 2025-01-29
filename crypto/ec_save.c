#include "hblk_crypto.h"

/**
* ec_save - Saves an EC key pair to disk.
* @key: The EC key pair to save.
* @folder: The folder path where the keys will be saved.
*
* Return: 1 on success, 0 on failure.
*/
int ec_save(EC_KEY *key, char const *folder)
{
	FILE *priv_file = NULL, *pub_file = NULL;
	char priv_path[1024], pub_path[1024];
	struct stat st = {0};

	if (!key || !folder)
		return (0);
	/* Check if the folder exists, create it if it doesn't */
	if (stat(folder, &st) == -1)
	{
		if (mkdir(folder, 0700) == -1)
			return (0);  /* Failed to create folder */
	}
	/* Build file paths for private and public keys */
	snprintf(priv_path, sizeof(priv_path), "%s/key.pem", folder);
	snprintf(pub_path, sizeof(pub_path), "%s/key_pub.pem", folder);
	/* Open the private key file for writing */
	priv_file = fopen(priv_path, "w");
	if (!priv_file)
		return (0); /* Write the private key to the file */
	if (!PEM_write_ECPrivateKey(priv_file, key, NULL, NULL, 0, NULL, NULL))
	{
		fclose(priv_file);
		return (0);
	} /* Open the public key file for writing */
	pub_file = fopen(pub_path, "w");
	if (!pub_file)
	{
		fclose(priv_file);
		return (0);
	} /* Write the public key to the file */
	if (!PEM_write_EC_PUBKEY(pub_file, key))
	{
		fclose(priv_file);
		fclose(pub_file);
		return (0);
	} /* Cleanup */
	fclose(priv_file);
	fclose(pub_file);
	return (1);  /* Successfully saved keys */
}
