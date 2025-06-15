/*
 * Advent of Code 2024 - Day 19: Linen Layout
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_PATTERNS 500
#define MAX_DESIGNS 500
#define MAX_PATTERN_LEN 20
#define MAX_DESIGN_LEN 100

typedef struct {
    char patterns[MAX_PATTERNS][MAX_PATTERN_LEN];
    int num_patterns;
    char designs[MAX_DESIGNS][MAX_DESIGN_LEN];
    int num_designs;
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
    
    data->num_patterns = 0;
    data->num_designs = 0;
    
    char line[4096];
    // Read patterns (first line)
    if (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, ", \n");
        while (token != NULL && data->num_patterns < MAX_PATTERNS) {
            strcpy(data->patterns[data->num_patterns], token);
            data->num_patterns++;
            token = strtok(NULL, ", \n");
        }
    }
    
    // Skip empty line
    fgets(line, sizeof(line), file);
    
    // Read designs
    while (fgets(line, sizeof(line), file) && data->num_designs < MAX_DESIGNS) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline
        if (strlen(line) > 0) {
            strcpy(data->designs[data->num_designs], line);
            data->num_designs++;
        }
    }
    
    fclose(file);
    return data;
}

// Check if a design can be made from available patterns using DP
bool can_make_design(const char* design, InputData* data) {
    int len = strlen(design);
    bool dp[MAX_DESIGN_LEN + 1];
    memset(dp, false, sizeof(dp));
    dp[0] = true; // Empty string can always be made
    
    for (int i = 1; i <= len; i++) {
        for (int j = 0; j < data->num_patterns; j++) {
            int pattern_len = strlen(data->patterns[j]);
            if (pattern_len <= i && dp[i - pattern_len]) {
                // Check if pattern matches at this position
                if (strncmp(&design[i - pattern_len], data->patterns[j], pattern_len) == 0) {
                    dp[i] = true;
                    break;
                }
            }
        }
    }
    
    return dp[len];
}

long part1(InputData* data) {
    int count = 0;
    
    for (int i = 0; i < data->num_designs; i++) {
        if (can_make_design(data->designs[i], data)) {
            count++;
        }
    }
    
    return count;
}

// Count number of ways to make a design from available patterns
long long count_ways_to_make_design(const char* design, InputData* data) {
    int len = strlen(design);
    long long dp[MAX_DESIGN_LEN + 1];
    memset(dp, 0, sizeof(dp));
    dp[0] = 1; // One way to make empty string
    
    for (int i = 1; i <= len; i++) {
        for (int j = 0; j < data->num_patterns; j++) {
            int pattern_len = strlen(data->patterns[j]);
            if (pattern_len <= i) {
                // Check if pattern matches at this position
                if (strncmp(&design[i - pattern_len], data->patterns[j], pattern_len) == 0) {
                    dp[i] += dp[i - pattern_len];
                }
            }
        }
    }
    
    return dp[len];
}

long part2(InputData* data) {
    long long total_ways = 0;
    
    for (int i = 0; i < data->num_designs; i++) {
        total_ways += count_ways_to_make_design(data->designs[i], data);
    }
    
    return total_ways;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day19_test.txt" 
        : "inputs/day19.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // TODO: Free allocated memory
    free(data);
    
    return 0;
}
