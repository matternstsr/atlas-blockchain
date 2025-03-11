#include "transaction.h"

int find_matches(llist_node_t unspent, unsigned int iter, void *context);
int send_tx(uint32_t amount, tc_t *context, EC_KEY const *receiver);
int sig_ins(llist_node_t tx_in, unsigned int iter, void *context);

/*
 * transaction_create - Creates a new transaction struct
 * @sender: Private key of sender
 * @receiver: Public key of receiver
 * @amount: Amount to send
 * @all_unspent: List of unused transactions
 * Return: NULL on failure, pointer to new transaction otherwise
 */
transaction_t *transaction_create(
    EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount,
    llist_t *all_unspent)
{
    uint8_t pub_key[EC_PUB_LEN];
    transaction_t *tx = NULL;
    tc_t *context = NULL;

    if (!sender || !receiver || !amount || !all_unspent)
        return NULL;

    context = calloc(1, sizeof(tc_t));
    tx = calloc(1, sizeof(transaction_t));
    if (!tx || !context)
        return free(context), NULL;

    context->tx = tx;
    context->all_unspent = all_unspent;

    /* Generate public key for sender */
    ec_to_pub(sender, pub_key);
    if (!pub_key)
        return free(tx), free(context), NULL;

    memcpy(context->pub, pub_key, EC_PUB_LEN);
    context->needed = (int)amount;
    context->sender = sender;

    /* Initialize inputs list */
    tx->inputs = llist_create(MT_SUPPORT_FALSE);

    /* Find matching unspent transactions */
    llist_for_each(all_unspent, find_matches, context);
    if (context->needed > 0)
        return free(tx), free(context), NULL;

    /* Initialize outputs list */
    tx->outputs = llist_create(MT_SUPPORT_FALSE);

    /* Send transaction */
    if (!send_tx(amount, context, receiver))
        return free(tx), free(context), NULL;

    /* Hash the transaction */
    transaction_hash(tx, tx->id);

    /* Sign the inputs */
    llist_for_each(tx->inputs, sig_ins, context);

    /* Cleanup */
    free(context);
    return tx;
}

/*
 * find_matches - Finds matches in the unspent list
 * @unspent: Unspent transaction
 * @iter: Iterator to track index in list
 * @context: Struct holding the needed info
 * Return: 0 on success, 1 on failure
 */
int find_matches(llist_node_t unspent, unsigned int iter, void *context)
{
    (void)iter;
    ti_t *new = NULL;

    if (CONTEXT->needed <= 0 || !unspent)
        return 1;

    if (!memcmp(CONTEXT->pub, UNSPENT->out.pub, EC_PUB_LEN))
    {
        new = tx_in_create(UNSPENT);
        llist_add_node(CONTEXT->tx->inputs, new, ADD_NODE_REAR);
        CONTEXT->balance += (int)UNSPENT->out.amount;
        CONTEXT->needed -= (int)UNSPENT->out.amount;
    }
    return 0;
}

/*
 * send_tx - Creates transaction outputs
 * @amount: Amount to send
 * @context: Struct holding the context
 * @receiver: Public key of the receiver
 * Return: 1 on success, 0 on failure
 */
int send_tx(uint32_t amount, tc_t *context, EC_KEY const *receiver)
{
    to_t *new = NULL, *chg = NULL;
    uint8_t pub_rec[EC_PUB_LEN];

    ec_to_pub(receiver, pub_rec);
    new = tx_out_create(amount, pub_rec);
    if (!new)
        return 0;

    llist_add_node(context->tx->outputs, new, ADD_NODE_REAR);

    if (context->balance > (int)amount)
    {
        chg = tx_out_create((context->balance - amount), context->pub);
        if (!chg)
            return 0;
        llist_add_node(context->tx->outputs, (llist_node_t *)chg, ADD_NODE_REAR);
    }

    return 1;
}

/*
 * sig_ins - Signs the input transactions
 * @tx_in: Transaction inputs list
 * @iter: Iterator to track index in list
 * @context: Struct holding the needed info
 * Return: 0 on success, 1 on failure
 */
int sig_ins(llist_node_t tx_in, unsigned int iter, void *context)
{
    (void)iter;

    if (!tx_in)
        return 1;

    tx_in_sign(((ti_t *)tx_in), CONTEXT->tx->id, CONTEXT->sender, CONTEXT->all_unspent);
    return 0;
}
