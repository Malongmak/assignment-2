#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define DIFFICULTY 4  // Number of leading zeros required
#define MAX_BLOCKS 100 // Maximum number of blocks in the blockchain

typedef struct Block {
    int index;
    char previous_hash[65]; // SHA-256 hash in hex
    long timestamp;
    char data[256];
    int nonce;
    char hash[65];
} Block;

Block blockchain[MAX_BLOCKS];
int blockchain_length = 0;

// Simple SHA-256 hash function (placeholder)
void sha256(const char *input, size_t len, unsigned char output[32]) {
    // Placeholder for actual SHA-256 implementation
    // You can replace this with a proper implementation
}

// Function to calculate hash for a block
void calculate_block_hash(Block *block) {
    char input[512];
    sprintf(input, "%d%s%ld%s%d", block->index, block->previous_hash, block->timestamp, block->data, block->nonce);
    sha256(input, strlen(input), (unsigned char *)block->hash);
    for (int i = 0; i < 32; i++) {
        sprintf(&block->hash[i * 2], "%02x", block->hash[i]);
    }
}

// Function to perform proof of work
void proof_of_work(Block *block) {
    char prefix[DIFFICULTY + 1];
    memset(prefix, '0', DIFFICULTY);
    prefix[DIFFICULTY] = '\0';

    printf("Mining block %d...\n", block->index);
    
    while (1) {
        calculate_block_hash(block);
        if (strncmp(block->hash, prefix, DIFFICULTY) == 0) {
            printf("Hash found: %s with nonce: %d\n", block->hash, block->nonce);
            break;
        }
        block->nonce++;
    }
}

// Function to add a new block to the blockchain
void add_block(char *data) {
    Block new_block;
    new_block.index = blockchain_length;
    strcpy(new_block.previous_hash, blockchain_length == 0 ? "0" : blockchain[blockchain_length - 1].hash);
    new_block.timestamp = time(NULL);
    strcpy(new_block.data, data);
    new_block.nonce = 0;

    proof_of_work(&new_block);
    
    // Add the new block to the blockchain
    blockchain[blockchain_length++] = new_block;
}

// Function to verify the integrity of the blockchain
int verify_blockchain() {
    for (int i = 1; i < blockchain_length; i++) {
        Block *current = &blockchain[i];
        Block *previous = &blockchain[i - 1];
        
        if (strcmp(current->previous_hash, previous->hash) != 0) {
            return 0; // Invalid blockchain
        }
        
        char calculated_hash[65];
        calculate_block_hash(current);
        if (strcmp(current->hash, calculated_hash) != 0) {
            return 0; // Invalid hash
        }
    }
    return 1; // Blockchain is valid
}

int main() {
    // Add a few blocks to the blockchain
    add_block("Block 1 Data");
    add_block("Block 2 Data");
    
    // Verify the integrity of the blockchain
    if (verify_blockchain()) {
        printf("Blockchain is valid.\n");
    } else {
        printf("Blockchain is invalid.\n");
    }

    return 0;
}