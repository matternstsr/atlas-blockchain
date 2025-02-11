/* Assuming _block_print function: */

void _block_print(const block_t *block, const char *indent)
{
    printf("%sBlock %u:\n", indent, block->info.index);
    printf("%s\t\tDifficulty: %u\n", indent, block->info.difficulty);
    printf("%s\t\tTimestamp: %llu\n", indent, block->info.timestamp);
    
    /* Accessing fields from block_t instead of block_info_t */
    printf("%s\t\tnonce: %u,\n", indent, block->nonce);
    printf("%s\t\tprev_block_hash: ", indent);
    _print_hex_buffer(block->info.prev_block_hash, SHA256_DIGEST_LENGTH);
    
    /* Accessing data_info buffer and length */
    printf("%s\t\tbuffer: \"%s\",\n", indent, block->data_info.buffer);
    printf("%s\t\tlen: %u\n", indent, block->data_info.len);
}

/* Assuming _block_print_brief function: */

void _block_print_brief(const block_t *block)
{
    printf("%llu, ", block->info.timestamp);
    printf("%u, ", block->info.difficulty);
    
    /* Accessing fields from block_t instead of block_info_t */
    printf("%u, ", block->nonce);
    _print_hex_buffer(block->info.prev_block_hash, SHA256_DIGEST_LENGTH);
    
    /* Accessing data_info buffer and length */
    printf("\"%s\", ", block->data_info.buffer);
    printf("%u", block->data_info.len);
}
