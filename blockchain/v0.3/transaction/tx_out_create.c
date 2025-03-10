#include "transaction.h"

/**
 * tx_out_create - Creates a transaction out struct
 * @pub: public key
 * @amount: amount of transaction
 * Return: pointer to new struct or NULL
 */
to_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	to_t *new_txo;

	if (!pub || !amount)
		return (NULL);

	new_txo = calloc(1, sizeof(to_t));
	if (!new_txo)
		return (NULL);
	new_txo->amount = amount;
	memcpy(new_txo->pub, pub, EC_PUB_LEN);
	SHA256((uint8_t *)new_txo, (EC_PUB_LEN + sizeof(uint32_t)), new_txo->hash);
	return (new_txo);
}
