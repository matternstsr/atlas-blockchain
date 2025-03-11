#include "transaction.h"
#include "hblk_crypto.h"
#include <openssl/sha.h>
#include <llist.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

/* Function to create a new transaction */
transaction_t *transaction_create(EC_KEY const *sender, EC_KEY const *receiver, uint32_t amount, llist_t *all_unspent)
{
    tx_context_t context = {0};
    transaction_t *new_tx = NULL;

    context.sender = sender;
    context.balance = 0;
    context.all_unspent = all_unspent;

    if (!create_outputs(amount, &context, receiver))
        return NULL;

    new_tx = malloc(sizeof(transaction_t));
    if (!new_tx)
        return NULL;

    context.tx = new_tx;

    llist_for_each(all_unspent, find_a_match, &context);

    transaction_hash(new_tx, new_tx->id);

    return new_tx;
}

/* Match unspent transactions with necessary criteria */
int find_a_match(llist_node_t unspent, unsigned int iter, void *context)
{
    tx_context_t *tx_ctx = (tx_context_t *)context;
    unspent_tx_out_t *unspent_output = (unspent_tx_out_t *)unspent;

    if (unspent_output->out.amount <= tx_ctx->needed) {
        tx_ctx->balance += unspent_output->out.amount;
        llist_add_node(tx_ctx->tx->inputs, unspent_output, ADD_NODE_REAR);
    }

    return 0;
}

/* Send the transaction to the network (dummy function here) */
int send_tx(uint32_t amount, tx_context_t *context, EC_KEY const *receiver)
{
    to_t *new_txo = tx_out_create(amount, receiver->pub_key);
    if (!new_txo)
        return 0;

    llist_add_node(context->tx->outputs, new_txo, ADD_NODE_REAR);

    if (context->balance > (int)amount) {
        to_t *change = tx_out_create(context->balance - amount, context->pub);
        if (!change)
            return 0;
        llist_add_node(context->tx->outputs, (llist_node_t *)change, ADD_NODE_REAR);
    }

    return 1;
}

/* Sign individual transaction input */
void sign_txi(llist_node_t tx_in, unsigned int iter, void *context)
{
    tx_context_t *tx_ctx = (tx_context_t *)context;
    tx_in_t *input = (tx_in_t *)tx_in;
    input->sig = tx_in_sign(input, tx_ctx->tx->id, tx_ctx->sender, tx_ctx->all_unspent);
}
