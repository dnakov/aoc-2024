/*
 * Advent of Code 2024 - Day 22: Monkey Market
 * 
 * This problem involves simulating a pseudorandom number generator with
 * specific transformation rules and finding patterns in price changes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_BUYERS 2500
#define ITERATIONS 2000
#define PRUNE_VALUE 16777216  // 2^24
#define SEQUENCE_LEN 4
#define MAX_SEQUENCES 200000

typedef struct {
    int64_t initial_secrets[MAX_BUYERS];
    int num_buyers;
} InputData;

typedef struct {
    int8_t changes[SEQUENCE_LEN];
    int buyer_id;
    int value;
} Sequence;

typedef struct {
    int8_t key[SEQUENCE_LEN];
    int total_value;
    bool seen[MAX_BUYERS];
} SequenceMap;

static SequenceMap seq_map[MAX_SEQUENCES];
static int seq_map_size = 0;

// Generate the next secret number
int64_t next_secret(int64_t secret) {
    // Step 1: Multiply by 64, mix, prune
    secret = secret ^ (secret * 64);
    secret = secret % PRUNE_VALUE;
    
    // Step 2: Divide by 32, mix, prune
    secret = secret ^ (secret / 32);
    secret = secret % PRUNE_VALUE;
    
    // Step 3: Multiply by 2048, mix, prune
    secret = secret ^ (secret * 2048);
    secret = secret % PRUNE_VALUE;
    
    return secret;
}

// Hash function for sequence of 4 changes
unsigned int hash_sequence(const int8_t* seq) {
    unsigned int hash = 0;
    for (int i = 0; i < SEQUENCE_LEN; i++) {
        hash = hash * 31 + (seq[i] + 10);  // +10 to handle negative values
    }
    return hash % MAX_SEQUENCES;
}

// Check if two sequences are equal
bool sequences_equal(const int8_t* seq1, const int8_t* seq2) {
    for (int i = 0; i < SEQUENCE_LEN; i++) {
        if (seq1[i] != seq2[i]) return false;
    }
    return true;
}

// Add or update sequence in map
void add_sequence(const int8_t* seq, int buyer_id, int value) {
    unsigned int idx = hash_sequence(seq);
    
    // Linear probing for collision resolution
    while (true) {
        if (seq_map[idx].total_value == 0) {
            // New sequence
            memcpy(seq_map[idx].key, seq, SEQUENCE_LEN * sizeof(int8_t));
            seq_map[idx].total_value = value;
            seq_map[idx].seen[buyer_id] = true;
            seq_map_size++;
            return;
        } else if (sequences_equal(seq_map[idx].key, seq)) {
            // Existing sequence
            if (!seq_map[idx].seen[buyer_id]) {
                seq_map[idx].total_value += value;
                seq_map[idx].seen[buyer_id] = true;
            }
            return;
        }
        idx = (idx + 1) % MAX_SEQUENCES;
    }
}

InputData* parse_input(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    InputData* data = malloc(sizeof(InputData));
    if (!data) {
        fclose(file);
        return NULL;
    }
    
    data->num_buyers = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) && data->num_buyers < MAX_BUYERS) {
        data->initial_secrets[data->num_buyers] = atoll(line);
        data->num_buyers++;
    }
    
    fclose(file);
    return data;
}

long part1(InputData* data) {
    long total = 0;
    
    for (int i = 0; i < data->num_buyers; i++) {
        int64_t secret = data->initial_secrets[i];
        
        // Generate 2000 iterations
        for (int j = 0; j < ITERATIONS; j++) {
            secret = next_secret(secret);
        }
        
        total += secret;
    }
    
    return total;
}

long part2(InputData* data) {
    // Clear sequence map
    memset(seq_map, 0, sizeof(seq_map));
    seq_map_size = 0;
    
    // For each buyer, track all sequences of 4 price changes
    for (int buyer = 0; buyer < data->num_buyers; buyer++) {
        int64_t secret = data->initial_secrets[buyer];
        int prices[ITERATIONS + 1];
        int8_t changes[ITERATIONS];
        
        // Generate all prices
        prices[0] = secret % 10;
        for (int i = 0; i < ITERATIONS; i++) {
            secret = next_secret(secret);
            prices[i + 1] = secret % 10;
            changes[i] = prices[i + 1] - prices[i];
        }
        
        // Look for sequences of 4 changes
        for (int i = 0; i <= ITERATIONS - SEQUENCE_LEN; i++) {
            add_sequence(&changes[i], buyer, prices[i + SEQUENCE_LEN]);
        }
    }
    
    // Find the sequence with maximum total value
    int max_value = 0;
    for (int i = 0; i < MAX_SEQUENCES; i++) {
        if (seq_map[i].total_value > max_value) {
            max_value = seq_map[i].total_value;
        }
    }
    
    return max_value;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day22_test.txt" 
        : "inputs/day22.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    free(data);
    
    return 0;
}