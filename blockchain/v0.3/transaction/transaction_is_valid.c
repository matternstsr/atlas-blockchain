#include "transaction.h"

/**
 * transaction_is_valid - Checks the validity of a transaction.
 * @transaction: The transaction to validate.
 * @all_unspent: List of all unspent transaction outputs.
 * Return: 1 if valid, 0 if invalid.
 */
int transaction_is_valid(
	transaction_t const *transaction, llist_t *all_unspent)
{
	uint8_t calculated_hash[SHA256_DIGEST_LENGTH];
	tv_t validation_context = {0};

	/* Check for null inputs */
	if (!transaction || !all_unspent)
		return 0;

	/* Initialize the validation context with transaction ID */
	memcpy(validation_context.tx_id, transaction->id, SHA256_DIGEST_LENGTH);
	validation_context.unspent = all_unspent;

	/* Calculate and compare transaction hash */
	transaction_hash(transaction, calculated_hash);
	if (memcmp(calculated_hash, transaction->id, SHA256_DIGEST_LENGTH))
		return 0;

	/* Validate inputs using the 'valid_input' function */
	if (llist_for_each(transaction->inputs, (node_func_t)&validate_input, &validation_context))
		return 0;

	/* Calculate total output amount */
	llist_for_each(transaction->outputs, (node_func_t)&sum_output_amount, &validation_context);

	/* Validate if input amount equals output amount */
	if (validation_context.input != validation_context.output)
		return 0;

	return 1;
}

/**
 * validate_input - Validates a single transaction input.
 * @in: Transaction input to validate.
 * @i: Iterator for list iteration.
 * @context: Context holding the transaction and unspent outputs.
 * Return: 0 on valid input, 1 on invalid input.
 */
int validate_input(tx_in_t *in, uint32_t i, tv_t *context)
{
	(void)i;

	uto_t *unspent_output = NULL;
	EC_KEY *public_key = NULL;

	/* Find matching unspent transaction output */
	unspent_output = llist_find_node(context->unspent, (node_ident_t)&check_unspent_match, in);
	if (!unspent_output)
		return 1;

	/* Accumulate the input amount */
	context->input += unspent_output->out.amount;

	/* Verify the public key for signature validation */
	public_key = ec_from_pub(unspent_output->out.pub);
	if (!public_key)
		return 1;

	/* Verify the input's signature against the transaction ID */
	if (!ec_verify(public_key, context->tx_id, SHA256_DIGEST_LENGTH, &in->sig))
	{
		EC_KEY_free(public_key);
		return 1;
	}

	EC_KEY_free(public_key);
	return 0;
}

/**
 * check_unspent_match - Checks if the unspent transaction matches the input.
 * @unspent: Unspent transaction output.
 * @in: Transaction input to check.
 * Return: 1 if matched, 0 if not.
 */
int check_unspent_match(uto_t *unspent, tx_in_t *in)
{
	/* Compare the hash values for matching unspent transaction output */
	if (!memcmp(unspent->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
		!memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
		return 1;
	return 0;
}

/**
 * sum_output_amount - Accumulates the output amounts of the transaction.
 * @out: Transaction output to check.
 * @i: Iterator for list iteration.
 * @context: Context holding transaction details.
 * Return: Always returns 0.
 */
int sum_output_amount(tx_out_t *out, unsigned int i, tv_t *context)
{
	(void)i;

	/* Add the amount from the output to the total output sum */
	context->output += out->amount;
	return 0;
}
