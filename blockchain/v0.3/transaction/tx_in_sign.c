#include "transaction.h"

int check_hash(llist_node_t out, void *hash);

/**
* tx_in_sign - Signs a transaction input after verifying key
* @in: Transaction input
* @tx_id: hash of transaction holding tx_input
* @sender: private key of receiver
* @unused_transactions: list of all unspent transactions
* Return: hash holding the signature or NULL
*/
sig_t *tx_in_sign(
	ti_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender,
	llist_t *unused_transactions)
{
	uint8_t pub_key[EC_PUB_LEN] = {0};
	uto_t *trans_out = NULL;

	/* Validate input parameters */
	if (!in || !tx_id || !sender || !unused_transactions)
		return NULL;

	/* Find the matching unspent transaction */
	trans_out = llist_find_node(unused_transactions, check_hash, in->tx_out_hash);
	if (!trans_out)
		return NULL;

	/* Convert the sender's private key to the corresponding public key */
	ec_to_pub(sender, pub_key);

	/* Verify the public key matches the unspent transaction output */
	if (memcmp(pub_key, trans_out->out.pub, EC_PUB_LEN))
		return NULL;

	/* Sign the transaction input */
	ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig);

	/* Return the generated signature */
	return (&in->sig);
}

/**
* check_hash - Function to find trans_out match in unspent list
* @out: trans_out to check
* @hash: hash to compare to
* Return: 1 on match, else 0
*/
int check_hash(llist_node_t out, void *hash)
{
	/* Compare the transaction output hash with the provided hash */
	if (!memcmp(((uto_t *)out)->out.hash, hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}
