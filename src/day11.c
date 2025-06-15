/*
 * Advent of Code 2024 - Day 11
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    long* stones;
    int count;
} InputData;

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
    
    // Count stones first
    data->count = 0;
    data->stones = malloc(sizeof(long) * 100); // Initial capacity
    
    char line[1024];
    if (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " \n");
        while (token != NULL) {
            data->stones[data->count++] = atol(token);
            token = strtok(NULL, " \n");
        }
    }
    
    fclose(file);
    return data;
}

// Count digits in a number
int count_digits(long num) {
    if (num == 0) return 1;
    int count = 0;
    while (num > 0) {
        count++;
        num /= 10;
    }
    return count;
}

// Split a number with even digits into two parts
void split_number(long num, long* left, long* right) {
    int digits = count_digits(num);
    int half = digits / 2;
    
    long divisor = 1;
    for (int i = 0; i < half; i++) {
        divisor *= 10;
    }
    
    *right = num % divisor;
    *left = num / divisor;
}

// Simulate one blink
long* blink(long* stones, int count, int* new_count) {
    // Allocate more space to be safe
    long* new_stones = malloc(sizeof(long) * count * 3);
    if (!new_stones) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    int idx = 0;
    
    for (int i = 0; i < count; i++) {
        if (stones[i] == 0) {
            new_stones[idx++] = 1;
        } else if (count_digits(stones[i]) % 2 == 0) {
            long left, right;
            split_number(stones[i], &left, &right);
            new_stones[idx++] = left;
            new_stones[idx++] = right;
        } else {
            new_stones[idx++] = stones[i] * 2024;
        }
    }
    
    *new_count = idx;
    return new_stones;
}

long part1(InputData* data) {
    // Make a copy of the stones
    long* stones = malloc(sizeof(long) * data->count);
    memcpy(stones, data->stones, sizeof(long) * data->count);
    int count = data->count;
    
    // Simulate 25 blinks
    for (int i = 0; i < 25; i++) {
        int new_count;
        long* new_stones = blink(stones, count, &new_count);
        free(stones);
        stones = new_stones;
        count = new_count;
        
    }
    
    free(stones);
    return count;
}

// Memoization cache for counting stones
typedef struct CacheEntry {
    long stone;
    int blinks;
    long count;
    struct CacheEntry* next;
} CacheEntry;

#define CACHE_SIZE 100000
CacheEntry* cache[CACHE_SIZE];

// Hash function for cache
unsigned int hash(long stone, int blinks) {
    unsigned long long key = ((unsigned long long)stone << 32) | blinks;
    return (key ^ (key >> 16) ^ (key >> 32)) % CACHE_SIZE;
}

// Get from cache
long cache_get(long stone, int blinks) {
    unsigned int idx = hash(stone, blinks);
    CacheEntry* entry = cache[idx];
    while (entry) {
        if (entry->stone == stone && entry->blinks == blinks) {
            return entry->count;
        }
        entry = entry->next;
    }
    return -1; // Not found
}

// Put in cache
void cache_put(long stone, int blinks, long count) {
    unsigned int idx = hash(stone, blinks);
    CacheEntry* entry = malloc(sizeof(CacheEntry));
    entry->stone = stone;
    entry->blinks = blinks;
    entry->count = count;
    entry->next = cache[idx];
    cache[idx] = entry;
}

// Clear cache
void cache_clear() {
    for (int i = 0; i < CACHE_SIZE; i++) {
        CacheEntry* entry = cache[i];
        while (entry) {
            CacheEntry* next = entry->next;
            free(entry);
            entry = next;
        }
        cache[i] = NULL;
    }
}

// Count stones after n blinks using memoization
long count_stones(long stone, int blinks) {
    if (blinks == 0) {
        return 1;
    }
    
    // Check cache
    long cached = cache_get(stone, blinks);
    if (cached != -1) {
        return cached;
    }
    
    long result;
    if (stone == 0) {
        result = count_stones(1, blinks - 1);
    } else if (count_digits(stone) % 2 == 0) {
        long left, right;
        split_number(stone, &left, &right);
        result = count_stones(left, blinks - 1) + count_stones(right, blinks - 1);
    } else {
        result = count_stones(stone * 2024, blinks - 1);
    }
    
    cache_put(stone, blinks, result);
    return result;
}

long part2(InputData* data) {
    // Clear cache from any previous runs
    memset(cache, 0, sizeof(cache));
    
    long total = 0;
    for (int i = 0; i < data->count; i++) {
        total += count_stones(data->stones[i], 75);
    }
    
    // Clean up cache
    cache_clear();
    
    return total;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day11_test.txt" 
        : "inputs/day11.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->stones);
    free(data);
    
    return 0;
}
