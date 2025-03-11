#include "transaction.h"

/**
 * transaction_create - Creates a new transaction struct
 * @sender: Private key of sender
 * @receiver: Public key of receiver
 * @amount: Amount to send
 * @unused_transactions: List of unused transactions
 * Return: NULL on Fail or pointer to new transaction
 */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver, 
                                  uint32_t amount, llist_t *unused_transactions)
{
    uint8_t pub_key[EC_PUB_LEN];
    transaction_t *this_tx = NULL;
    tc_t *context = NULL;

    /* Validate inputs */
    if (!sender || !receiver || !amount || !unused_transactions)
        return NULL;
    /* Allocate memory for context and transaction */
    context = calloc(1, sizeof(tc_t));
    this_tx = calloc(1, sizeof(transaction_t));
    if (!this_tx || !context)
        return NULL;

    /* Set context fields */
    context->tx = this_tx;
    context->unused_transactions = unused_transactions;
    ec_to_pub(sender, pub_key);

    /* Validate public key conversion */
    memcpy(context->pub, pub_key, EC_PUB_LEN);
    context->needed = (int)amount;
    context->sender = sender;

    /* Initialize inputs list */
    this_tx->inputs = llist_create(MT_SUPPORT_FALSE);

    /* Process unspent transactions */
    llist_for_each(unused_transactions, match_transaction, context);

    /* If balance is insufficient, fail */
    if (context->needed > 0)
    {
        free(this_tx);
        return NULL;
    }

    /* Create outputs list */
    this_tx->outputs = llist_create(MT_SUPPORT_FALSE);

    /* Send the transaction */
    if (!process_transaction_output(amount, context, receiver))
    {
        free(this_tx);
        return NULL;
    }
    /* Generate transaction hash */
    transaction_hash(this_tx, this_tx->id);

    /* Sign the transaction inputs */
    llist_for_each(this_tx->inputs, sign_transaction_input, context);

    /* Clean up context */
    free(context);
    return this_tx;
}

/**
 * match_transaction - Searches unspent list for matches
 * @unspent: Unspent transaction
 * @i: Iterator needed for llist functions
 * @context: Struct holding needed info
 * Return: 0 on success or 1 on fail
 */
int match_transaction(llist_node_t unspent, unsigned int i, void *context)
{
    (void)i;
    ti_t *new_txi;

    if (CONTEXT->needed <= 0 || !unspent)
        return 1;

    /* If public keys match, add input to transaction */
    if (!memcmp(CONTEXT->pub, UNSPENT->out.pub, EC_PUB_LEN))
    {
        new_txi = tx_in_create(UNSPENT);
        llist_add_node(CONTEXT->tx->inputs, new_txi, ADD_NODE_REAR);
        CONTEXT->balance += (int)UNSPENT->out.amount;
        CONTEXT->needed -= (int)UNSPENT->out.amount;
    }
    return 0;
}

/**
 * process_transaction_output - Creates transaction outputs
 * @amount: Amount to send
 * @context: Struct holding info
 * @receiver: Public key of receiver
 * Return: 0 on fail, 1 on success
 */
int process_transaction_output(uint32_t amount, tc_t *context, EC_KEY const *receiver)
{
    to_t *new_txo, *change_txo;
    uint8_t pub_rec[EC_PUB_LEN];

    /* Generate public key for receiver */
    ec_to_pub(receiver, pub_rec);
    new_txo = tx_out_create(amount, pub_rec);
    if (!new_txo)
        return 0;

    /* Add receiver's output to transaction */
    llist_add_node(context->tx->outputs, new_txo, ADD_NODE_REAR);

    /* Handle change if necessary */
    if (context->balance > (int)amount)
    {
        change_txo = tx_out_create((context->balance - amount), context->pub);
        if (!change_txo)
            return 0;
        llist_add_node(context->tx->outputs, (llist_node_t *)change_txo, ADD_NODE_REAR);
    }

    return 1;
}

/**
 * sign_transaction_input - Signs input transaction
 * @tx_in: Transaction input
 * @i: Iterator needed for llist functions
 * @context: Struct holding needed info
 * Return: 0 on success, 1 on fail
 */
int sign_transaction_input(llist_node_t tx_in, unsigned int i, void *context)
{
    (void)i;

    if (!tx_in)
        return 1;

    /* Sign the input transaction */
    tx_in_sign(((ti_t *)tx_in), CONTEXT->tx->id, CONTEXT->sender, CONTEXT->unused_transactions);
    return 0;
}
