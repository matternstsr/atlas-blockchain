#include "blockchain.h"

int read_tx(FILE *fptr, uint32_t tx_num, llist_t *tx_list);
int read_inputs(FILE *fptr, uint32_t num_in, llist_t *tx_in);
int read_outputs(FILE *fptr, uint32_t num_out, llist_t *tx_out);
int read_unspent(FILE *fptr, blockchain_t *chain, uint32_t unspent_num);

/**
 * blockchain_deserialize - Loads a blockchain from file
 * @path: file to read from
 * Return: Pointer to chain or NULL
 */
blockchain_t *blockchain_deserialize(char const *path)
{
	FILE *fptr = NULL;
	char header_buf[7] = {0};
	uint8_t end;
	uint32_t numblocks, unspent_num, tx_num, i = 0;
	blockchain_t *blockchain = calloc(1, sizeof(blockchain_t));
	block_t *block = NULL;

	if (!path)
		return (NULL);
	fptr = fopen(path, "r");
	if (!fptr)
		return (NULL);
	fread(header_buf, 1, 7, fptr);
	if (memcmp(header_buf, FHEADER, 7))
		return (NULL);
	fread(&end, 1, 1, fptr);
	fread(&numblocks, 4, 1, fptr);
	fread(&unspent_num, 4, 1, fptr);
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	for (; i < numblocks; i++)
	{
		block = calloc(1, sizeof(block_t));
		fread(&block->info, 1, sizeof(block_info_t), fptr);
		fread(&(block->data.len), sizeof(uint8_t), 4, fptr);
		fread(block->data.buffer, block->data.len, sizeof(uint8_t), fptr);
		fread(block->hash, sizeof(uint8_t), SHA256_DIGEST_LENGTH, fptr);
		fread(&tx_num, 4, 1, fptr);
		if ((int)tx_num != -1)
		{
			block->transactions = llist_create(MT_SUPPORT_FALSE);
			read_tx(fptr, tx_num, block->transactions);
		}
		llist_add_node(blockchain->chain, block, ADD_NODE_REAR);
	}
	read_unspent(fptr, blockchain, unspent_num);
	fclose(fptr);
	return (blockchain);
}

/**
 * read_tx - Reads the transaction info from file
 * @fptr: file pointer to file to read from
 * @tx_num: number of transactions
 * @tx_list: list of transactions
 * Return: 1 on success
 */
int read_tx(FILE *fptr, uint32_t tx_num, llist_t *tx_list)
{
	uint32_t i = 0, num_in, num_out;
	transaction_t *tx;

	if (!tx_num)
		return (1);

	for (; i < tx_num; i++)
	{
		tx = calloc(1, sizeof(transaction_t));
		fread(tx->id, sizeof(uint8_t), SHA256_DIGEST_LENGTH, fptr);
		fread(&num_in, 4, 1, fptr);
		fread(&num_out, 4, 1, fptr);
		tx->inputs = llist_create(MT_SUPPORT_FALSE);
		tx->outputs = llist_create(MT_SUPPORT_FALSE);
		read_inputs(fptr, num_in, tx->inputs);
		read_outputs(fptr, num_out, tx->outputs);
		llist_add_node(tx_list, tx, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * read_inputs - Reads tx inputs from file
 * @fptr: file pointer
 * @num_in: number of inputs
 * @tx_in: list of inputs to add to
 * Return: 1
 */
int read_inputs(FILE *fptr, uint32_t num_in, llist_t *tx_in)
{
	uint32_t i = 0;
	ti_t *in;

	if (!num_in)
		return (1);

	for (; i < num_in; i++)
	{
		in = calloc(1, sizeof(ti_t));
		fread(in->block_hash, 32, 1, fptr);
		fread(in->tx_id, 32, 1, fptr);
		fread(in->tx_out_hash, 32, 1, fptr);
		fread(in->sig.sig, 72, 1, fptr);
		fread(&in->sig.len, 1, 1, fptr);
		llist_add_node(tx_in, in, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * read_outputs - Reads tx outputs from file
 * @fptr: file pointer
 * @num_out: number of outputs
 * @tx_out: list of outputs to add to
 * Return: 1
 */
int read_outputs(FILE *fptr, uint32_t num_out, llist_t *tx_out)
{
	uint32_t i = 0;
	to_t *out;

	if (!num_out)
		return (1);

	for (; i < num_out; i++)
	{
		out = calloc(1, sizeof(ti_t));
		fread(&out->amount, 4, 1, fptr);
		fread(out->pub, 65, 1, fptr);
		fread(out->hash, 32, 1, fptr);
		llist_add_node(tx_out, out, ADD_NODE_REAR);
	}
	return (1);
}

/**
 * read_unspent - Reads unspent outputs from file
 * @fptr: file pointer
 * @chain: blockchain to add list to
 * @unspent_num: number of unspent tx
 * Return: 1
 */
int read_unspent(FILE *fptr, blockchain_t *chain, uint32_t unspent_num)
{
	uto_t *unspent;
	uint32_t i = 0;

	chain->unspent = llist_create(MT_SUPPORT_FALSE);
	if (!unspent_num)
		return (1);

	for (; i < unspent_num; i++)
	{
		unspent = calloc(1, sizeof(uto_t));
		fread(unspent->block_hash, 32, 1, fptr);
		fread(unspent->tx_id, 32, 1, fptr);
		fread(&unspent->out.amount, 4, 1, fptr);
		fread(unspent->out.pub, 65, 1, fptr);
		fread(unspent->out.hash, 32, 1, fptr);
		llist_add_node(chain->unspent, unspent, ADD_NODE_REAR);
	}

	return (1);
}
