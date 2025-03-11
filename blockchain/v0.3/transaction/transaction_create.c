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

/* Create transaction outputs for the given amount */
int create_outputs(uint32_t amount, tx_context_t *context, EC_KEY const *receiver)
{
    tx_out_t *output = NULL;
    uint8_t *receiver_pub_key = NULL;

    receiver_pub_key = EC_KEY_get_pub_key(receiver);

    output = tx_out_create(amount, receiver_pub_key);
    if (!output)
        return 0;

    llist_add_node(context->tx->outputs, output, ADD_NODE_REAR);

    return 1;
}

/* Match unspent transactions with necessary criteria */
int match_unspent_tx(llist_node_t unspent, unsigned int iter, void *context)
{
    tx_context_t *tx_ctx = (tx_context_t *)context;
    unspent_tx_out_t *unspent_output = (unspent_tx_out_t *)unspent;

    if (unspent_output->out.amount <= tx_ctx->needed) {
        tx_ctx->balance += unspent_output->out.amount;
        llist_add_node(tx_ctx->tx->inputs, unspent_output, ADD_NODE_REAR);
    }

    return 0;
}

/* Sign transaction inputs */
int sign_tx_inputs(llist_node_t tx_in, unsigned int iter, void *context)
{
    tx_context_t *tx_ctx = (tx_context_t *)context;
    tx_in_t *input = (tx_in_t *)tx_in;

    input->sig = tx_in_sign(input, tx_ctx->tx->id, tx_ctx->sender, tx_ctx->all_unspent);

    return 0;
}

/* Find matching unspent transaction output */
int find_a_match(llist_node_t unspent, unsigned int iter, void *context)
{
    tx_context_t *tx_ctx = (tx_context_t *)context;
    unspent_tx_out_t *unspent_output = (unspent_tx_out_t *)unspent;
    if (tx_ctx->balance < tx_ctx->needed) {
        tx_ctx->balance += unspent_output->out.amount;
        llist_add_node(tx_ctx->tx->inputs, unspent_output, ADD_NODE_REAR);
    }

    return 0;
}

/* Send the transaction to the network (dummy function here) */
int send_tx(uint32_t amount, tx_context_t *context, EC_KEY const *receiver)
{
    printf("Transaction of %u sent to receiver.\n", amount);

    return 1;
}

/* Sign individual transaction input */
void sign_txi(llist_node_t tx_in, unsigned int iter, void *context)
{
    tx_context_t *tx_ctx = (tx_context_t *)context;
    tx_in_t *input = (tx_in_t *)tx_in;
    input->sig = tx_in_sign(input, tx_ctx->tx->id, tx_ctx->sender, tx_ctx->all_unspent);
}

/* Create the transaction output for a specific amount and public key */
to_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
    to_t *output = malloc(sizeof(to_t));
    if (!output)
        return NULL;

    output->amount = amount;
    memcpy(output->pub, pub, EC_PUB_LEN);
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, &output->amount, sizeof(output->amount));
    SHA256_Update(&sha256_ctx, output->pub, EC_PUB_LEN);
    SHA256_Final(output->hash, &sha256_ctx);

    return output;
}

/* Generate the hash for the transaction */
uint8_t *transaction_hash(transaction_t const *transaction, uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    llist_for_each(transaction->inputs, hash_in, hash_buf);
    llist_for_each(transaction->outputs, hash_out, hash_buf);

    SHA256_Final(hash_buf, &sha256_ctx);

    return hash_buf;
}

/* Add a node to the list of inputs */
int hash_in(llist_node_t input, unsigned int iter, void *buff)
{
    tx_in_t *tx_in = (tx_in_t *)input;
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, tx_in->tx_id, sizeof(tx_in->tx_id));
    SHA256_Update(&sha256_ctx, tx_in->tx_out_hash, sizeof(tx_in->tx_out_hash));
    SHA256_Update(&sha256_ctx, &tx_in->sig, sizeof(tx_in->sig));
    SHA256_Final(buff, &sha256_ctx);

    return 0;
}

/* Add a node to the list of outputs */
int hash_out(llist_node_t output, unsigned int iter, void *buff)
{
    tx_out_t *tx_out = (tx_out_t *)output;
    SHA256_CTX sha256_ctx;
    SHA256_Init(&sha256_ctx);
    SHA256_Update(&sha256_ctx, &tx_out->amount, sizeof(tx_out->amount));
    SHA256_Update(&sha256_ctx, tx_out->pub, EC_PUB_LEN);
    SHA256_Final(buff, &sha256_ctx);

    return 0;
}
