#include "transaction.h"

int check_hash(llist_node_t out, void *hash);

/**
 * tx_in_sign - Signs a transaction input after verifying key
 * @in: Transaction input
 * @tx_id: hash of transaction holding tx_input
 * @sender: private key of reciever
 * @all_unspent: list of all unspent transactions
 * Return: hash holding the signiture
 */
sig_t *tx_in_sign(
	ti_t *in, uint8_t const tx_id[SHA256_DIGEST_LENGTH], EC_KEY const *sender,
	llist_t *all_unspent)
{
	uint8_t pub_key[EC_PUB_LEN] = {0};
	uto_t *tx_out = NULL;

	if (!in || !tx_id || !sender || !all_unspent)
		return (NULL);
	tx_out = llist_find_node(all_unspent, check_hash, in->tx_out_hash);
	if (!tx_out)
		return (NULL);
	ec_to_pub(sender, pub_key);
	if (memcmp(pub_key, tx_out->out.pub, EC_PUB_LEN))
		return (NULL);
	ec_sign(sender, tx_id, SHA256_DIGEST_LENGTH, &in->sig);
	return (&in->sig);
}

/**
 * check_hash - Function to find tx_out match in unspent list
 * @out: tx_out to check
 * @hash: hash to cmp to
 * Return: 1 on match, else 0
 */
int check_hash(llist_node_t out, void *hash)
{
	if (!memcmp(((uto_t *)out)->out.hash, hash, SHA256_DIGEST_LENGTH))
		return (1);
	return (0);
}
