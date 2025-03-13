#include "transaction.h"

/**
 * transaction_is_valid - Checks whether a transaction is valid
 * @transaction: The transaction to be validated
 * @unused_transactions: List of unspent transaction outputs
 * Return: 1 if the transaction is valid, 0 if invalid
 */
int transaction_is_valid(
    const transaction_t *transaction, llist_t *unused_transactions)
{
    uint8_t v_hash[SHA256_DIGEST_LENGTH];
    tv_t context = {0};

    /* Check if either transaction or unspent list is NULL */
    if (!transaction || !unused_transactions)
        return 0;

    /* Copy the transaction ID and unspent list into the context structure */
    memcpy(context.tx_id, transaction->id, SHA256_DIGEST_LENGTH);
    context.unspent = unused_transactions;

    /* Compute the hash of the transaction */
    transaction_hash(transaction, v_hash);

    /* Compare the computed hash with the transaction's ID */
    if (memcmp(v_hash, transaction->id, SHA256_DIGEST_LENGTH))
        return 0;

    /* Verify the validity of each input transaction */
    if (llist_for_each(transaction->inputs, (node_func_t)&validate_input_signature, &context))
        return 0;

    /* Accumulate the total amount from the transaction outputs */
    llist_for_each(transaction->outputs, (node_func_t)&accumulate_output_value, &context);

    /* Ensure that the total inputs equal total outputs */
    if (context.input != context.output)
        return 0;

    /* Return 1 if the transaction is valid */
    return 1;
}

/**
 * validate_input_signature - Verifies the signature and checks for unspent outputs
 * @in: Input transaction to validate
 * @i: Iterator needed for llist functions (unused)
 * @context: Structure holding the transaction and list of unspent outputs
 * Return: 0 if valid, 1 if invalid
 */
int validate_input_signature(tx_in_t *in, uint32_t i, tv_t *context)
{
    (void)i;  /* Suppress unused parameter warning */
    uto_t *match_found = NULL;
    EC_KEY *key = NULL;

    /* Look for a matching unspent transaction output */
    match_found = llist_find_node(context->unspent,
        (node_ident_t)&match_unspent_output, in);
    
    /* Return 1 if no matching output is found */
    if (!match_found)
        return 1;

    /* Add the amount of the matched output to the input total */
    context->input += match_found->out.amount;

    /* Get the public key from the matched unspent output */
    key = ec_from_pub(match_found->out.pub);

    /* Return 1 if no valid public key is found */
    if (!key)
        return 1;

    /* Verify the signature using the public key */
    if (!ec_verify(key, context->tx_id, SHA256_DIGEST_LENGTH, &in->sig))
    {
        EC_KEY_free(key);  /* Free the EC_KEY after use */
        return 1;
    }

    /* Free the EC_KEY and return 0 if verification is successful */
    EC_KEY_free(key);
    return 0;
}

/**
 * accumulate_output_value - Calculates the total value of the transaction outputs
 * @out: Output to check
 * @i: Iterator needed for llist functions (unused)
 * @context: Structure holding the transaction and unspent outputs
 * Return: Always 0
 */
int accumulate_output_value(tx_out_t *out, unsigned int i, tv_t *context)
{
    (void)i;  /* Suppress unused parameter warning */

    /* Accumulate the amount from the output into the total output sum */
    context->output += out->amount;
    return 0;  /* Always return 0 as this function doesn't fail */
}

/**
 * match_unspent_output - Compares the hash of unspent outputs with the input transaction
 * @unspent: Unspent transaction output
 * @in: Input transaction to compare
 * Return: 1 if the hashes match, 0 otherwise
 */
int match_unspent_output(uto_t *unspent, tx_in_t *in)
{
    /* Compare the block hash, transaction ID, and output hash */
    if (!memcmp(unspent->block_hash, in->block_hash, SHA256_DIGEST_LENGTH) &&
        !memcmp(unspent->tx_id, in->tx_id, SHA256_DIGEST_LENGTH) &&
        !memcmp(unspent->out.hash, in->tx_out_hash, SHA256_DIGEST_LENGTH))
    {
        return 1;  /* Return 1 if all the hashes match */
    }
    return 0;  /* Return 0 if any of the hashes don't match */
}
