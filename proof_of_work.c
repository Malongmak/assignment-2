#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

#define DIFFICULTY 4  // Number of leading zeros required

// Function to calculate SHA-256 hash
void calculate_hash(char *input, unsigned char output[SHA256_DIGEST_LENGTH]) {
    SHA256((unsigned char*)input, strlen(input), output);
}

// Function to create a block and perform proof of work
void proof_of_work(int index, char *previous_hash, char *data) {
    char input[256];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    int nonce = 0;
    char prefix[DIFFICULTY + 1];
    memset(prefix, '0', DIFFICULTY);
    prefix[DIFFICULTY] = '\0';

    printf("Mining...\n");
    
    while (1) {
        // Create the input string for hashing
        sprintf(input, "%d%s%ld%s%d", index, previous_hash, time(NULL), data, nonce);
        
        // Calculate the hash
        calculate_hash(input, hash);
        
        // Convert hash to hexadecimal string
        char hash_hex[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(&hash_hex[i * 2], "%02x", hash[i]);
        }
        
        // Check for leading zeros
        if (strncmp(hash_hex, prefix, DIFFICULTY) == 0) {
            printf("Hash found: %s with nonce: %d\n", hash_hex, nonce);
            break;
        }

        nonce++;
    }
}

int main() {
    // Sample block data
    int index = 1;
    char previous_hash[] = "0";
    char data[] = "Block data here";

    // Perform proof of work
    proof_of_work(index, previous_hash, data);

    return 0;
}