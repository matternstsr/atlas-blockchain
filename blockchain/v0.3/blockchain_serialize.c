#include "blockchain.h"

int write_blocks(block_t *block, unsigned int index, FILE *fptr);
int write_tx(transaction_t *tx, unsigned int index, FILE *fptr);
int write_ins(ti_t *in, unsigned int index, FILE *fptr);
int write_outs(to_t *out, unsigned int index, FILE *fptr);
int write_unspent(uto_t *unspent, unsigned int index, FILE *fptr);


/**
 * blockchain_serialize - serializes a blockchain to file
 * @blockchain: chain to serialize
 * @path: file path to serialize to
 * Return: 1 on succerss, 0 on fail
 */
int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fptr = NULL;
	int blocknums = 0, unspent_nums = 0;
	char header[16] = {FHEADER};

	if (!blockchain || !path)
		return (0);
	blocknums = llist_size(blockchain->chain);
	unspent_nums = llist_size(blockchain->unspent);
	memcpy(&header[7], END, 1);
	memcpy(&header[8], &blocknums, 4);
	memcpy(&header[12], &unspent_nums, 4);
	fptr = fopen(path, "w");
	fwrite(header, 1, 16, fptr);
	llist_for_each(blockchain->chain, (node_func_t)&write_blocks, fptr);
	llist_for_each(blockchain->unspent, (node_func_t)&write_unspent, fptr);
	fclose(fptr);
	return (1);
}

/**
 * write_blocks - function to write blocks to file
 * @block: node to perform function on
 * @index: unused
 * @fptr: filestream pointer
 * Return: 0
 */
int write_blocks(block_t *block, unsigned int index, FILE *fptr)
{
	(void)index;
	char block_buf[1120];
	uint32_t len = 0;
	int tx_size = 0;


	tx_size = llist_size(block->transactions);
	if (!strcmp(END, "\x02"))
		SWAPENDIAN(block);

	len = block->data.len;

	memcpy(&block_buf[0], block, sizeof(block_info_t));
	memcpy(&block_buf[56], &block->data.len, 4);
	memcpy(&block_buf[60], block->data.buffer, len);
	memcpy(&block_buf[60 + len], block->hash, 32);
	memcpy(&block_buf[92 + len], &tx_size, 4);
	fwrite(block_buf, 1, 96 + len, fptr);
	if (tx_size > 0)
		llist_for_each(block->transactions, (node_func_t)*write_tx, fptr);
	return (0);
}

/**
 * write_tx - writes a tx node to file
 * @tx: transaction to write
 * @index: unused
 * @fptr: file pointer
 * Return: 0
 */
int write_tx(transaction_t *tx, unsigned int index, FILE *fptr)
{
	(void)index;
	char tx_buff[40];
	int ins = 0, outs = 0;

	ins = llist_size(tx->inputs);
	outs = llist_size(tx->outputs);

	memcpy(&tx_buff[0], tx->id, 32);
	memcpy(&tx_buff[32], &ins, 4);
	memcpy(&tx_buff[36], &outs, 4);
	fwrite(&tx_buff, 1, 40, fptr);
	llist_for_each(tx->inputs, (node_func_t)&write_ins, fptr);
	llist_for_each(tx->outputs, (node_func_t)&write_outs, fptr);
	return (0);
}

/**
 * write_ins - writes tx inputs to file
 * @in: input to write
 * @index: unused
 * @fptr: file pointer
 * Return: 0
 */
int write_ins(ti_t *in, unsigned int index, FILE *fptr)
{
	(void)index;
	char in_buff[169];

	memcpy(&in_buff[0], in->block_hash, 32);
	memcpy(&in_buff[32], in->tx_id, 32);
	memcpy(&in_buff[64], in->tx_out_hash, 32);
	memcpy(&in_buff[96], in->sig.sig, 72);
	memcpy(&in_buff[168], &in->sig.len, 1);
	fwrite(&in_buff, 1, 169, fptr);
	return (0);
}

/**
 * write_outs - writes tx outputs to file
 * @out: output to write
 * @index: unused
 * @fptr: File pointer
 * Return: 0
 */
int write_outs(to_t *out, unsigned int index, FILE *fptr)
{
	(void)index;
	char out_buff[101];

	memcpy(&out_buff[0], &out->amount, 4);
	memcpy(&out_buff[4], out->pub, 65);
	memcpy(&out_buff[69], out->hash, 32);
	fwrite(&out_buff, 1, 101, fptr);
	return (0);
}

/**
 * write_unspent - writes unspent outputs to file
 * @unspent: unspent to write
 * @index: unsused
 * @fptr: file pointer
 * Return: 0
 */
int write_unspent(uto_t *unspent, unsigned int index, FILE *fptr)
{
	(void)index;
	char unspent_buff[165];

	memcpy(&unspent_buff[0], unspent->block_hash, 32);
	memcpy(&unspent_buff[32], unspent->tx_id, 32);
	memcpy(&unspent_buff[64], &unspent->out.amount, 4);
	memcpy(&unspent_buff[68], unspent->out.pub, 65);
	memcpy(&unspent_buff[133], unspent->out.hash, 32);
	fwrite(&unspent_buff, 1, 165, fptr);
	return (0);
}
