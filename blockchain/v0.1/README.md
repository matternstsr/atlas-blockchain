# Blockchain Project

This project implements a basic blockchain with essential operations such as block creation, serialization/deserialization, validity check, and blockchain management. The code handles creating and verifying blocks and provides functions to serialize and deserialize blockchain data.

## Files

### Source Files:
- **blockchain_serialize.c**: Contains the function to serialize the blockchain to a file.
- **block_create.c**: Creates a new block in the blockchain.
- **block_destroy.c**: Frees the memory allocated for a block.
- **block_hash.c**: Computes the hash of a block.
- **block_is_valid.c**: Verifies the validity of a block in the blockchain.
- **blockchain_create.c**: Creates a new blockchain structure.
- **blockchain_deserialize.c**: Deserializes the blockchain from a file.
- **blockchain_destroy.c**: Destroys the blockchain and frees its memory.

### Header Files:
- **blockchain.h**: Contains the function prototypes and type definitions for blockchain-related functions (block, blockchain, etc.).

### Compilation
Use the provided `Makefile` to compile the project. The default target compiles the project into an executable named `blockchain`.

1. To compile the project, run:
   ```bash
   make
