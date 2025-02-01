#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define MAX_TRANSACTIONS 5
#define HASH_SIZE 65
#define MAX_NONCE 1000000000  // Limit nonce attempts to prevent infinite loop

// Adjustable difficulty level
int DIFFICULTY = 4; // Default difficulty (can be changed dynamically)

// Block structure
typedef struct Block {
    int index;
    time_t timestamp;
    char transactions[MAX_TRANSACTIONS][256]; // Simplified transactions
    char previous_hash[HASH_SIZE];
    int nonce;
    char hash[HASH_SIZE];
} Block;

// Function to compute SHA-256 hash
void compute_hash(Block *block, char *output) {
    char input[1024];
    snprintf(input, sizeof(input), "%d%ld%s%d", block->index, block->timestamp, block->previous_hash, block->nonce);
    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)input, strlen(input), hash);
    
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[HASH_SIZE - 1] = '\0';
}

// Function to check if hash meets difficulty criteria
int is_valid_hash(const char *hash) {
    for (int i = 0; i < DIFFICULTY; i++) {
        if (hash[i] != '0') {
            return 0;
        }
    }
    return 1;
}

// Proof of Work function
void mine_block(Block *block) {
    clock_t start_time = clock();
    block->nonce = 0;
    
    while (block->nonce < MAX_NONCE) {
        compute_hash(block, block->hash);
        
        if (is_valid_hash(block->hash)) {
            break;
        }
        
        if (block->nonce % 100000 == 0) {
            printf("Nonce: %d, Hash: %s\n", block->nonce, block->hash);
        }
        
        block->nonce++;
    }
    
    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("Block #%d mined in %.4f seconds with difficulty %d\n", block->index, time_taken, DIFFICULTY);
}

// Function to create a new block
Block create_block(int index, const char *previous_hash, char transactions[MAX_TRANSACTIONS][256]) {
    Block new_block;
    new_block.index = index;
    new_block.timestamp = time(NULL);
    memcpy(new_block.previous_hash, previous_hash, HASH_SIZE);
    new_block.nonce = 0;
    
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
        strncpy(new_block.transactions[i], transactions[i], 256);
    }
    
    mine_block(&new_block);
    return new_block;
}

// Function to print block details
void print_block(Block block) {
    printf("Block #%d\n", block.index);
    printf("Timestamp: %ld\n", block.timestamp);
    printf("Previous Hash: %s\n", block.previous_hash);
    printf("Nonce: %d\n", block.nonce);
    printf("Hash: %s\n", block.hash);
    printf("Transactions:\n");
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
        printf("  - %s\n", block.transactions[i]);
    }
    printf("-----------------------------\n");
}

int main() {
    char genesis_transactions[MAX_TRANSACTIONS][256] = {"Tx1", "Tx2", "Tx3", "Tx4", "Tx5"};
    
    // Test mining with different difficulties
    for (int difficulty = 3; difficulty <= 5; difficulty++) {
        DIFFICULTY = difficulty;
        printf("\nMining with difficulty %d:\n", DIFFICULTY);
        Block block = create_block(difficulty - 2, "0", genesis_transactions);
        print_block(block);
    }
    
    return 0;
}
