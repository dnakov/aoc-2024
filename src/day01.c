/*
 * Advent of Code 2024 - Day 01
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int* left_list;
    int* right_list;
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
    
    // Count lines first
    int capacity = 1000;
    data->left_list = malloc(capacity * sizeof(int));
    data->right_list = malloc(capacity * sizeof(int));
    data->count = 0;
    
    if (!data->left_list || !data->right_list) {
        free(data->left_list);
        free(data->right_list);
        free(data);
        fclose(file);
        return NULL;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int left, right;
        if (sscanf(line, "%d %d", &left, &right) == 2) {
            if (data->count >= capacity) {
                capacity *= 2;
                data->left_list = realloc(data->left_list, capacity * sizeof(int));
                data->right_list = realloc(data->right_list, capacity * sizeof(int));
            }
            data->left_list[data->count] = left;
            data->right_list[data->count] = right;
            data->count++;
        }
    }
    
    fclose(file);
    return data;
}

int compare_int(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

long part1(InputData* data) {
    // Create copies to sort
    int* left_sorted = malloc(data->count * sizeof(int));
    int* right_sorted = malloc(data->count * sizeof(int));
    
    memcpy(left_sorted, data->left_list, data->count * sizeof(int));
    memcpy(right_sorted, data->right_list, data->count * sizeof(int));
    
    // Sort both lists
    qsort(left_sorted, data->count, sizeof(int), compare_int);
    qsort(right_sorted, data->count, sizeof(int), compare_int);
    
    // Calculate total distance
    long total_distance = 0;
    for (int i = 0; i < data->count; i++) {
        total_distance += abs(left_sorted[i] - right_sorted[i]);
    }
    
    free(left_sorted);
    free(right_sorted);
    
    return total_distance;
}

long part2(InputData* data) {
    // Calculate similarity score
    // For each number in left list, multiply by count in right list
    long similarity_score = 0;
    
    for (int i = 0; i < data->count; i++) {
        int left_num = data->left_list[i];
        int count = 0;
        
        // Count occurrences in right list
        for (int j = 0; j < data->count; j++) {
            if (data->right_list[j] == left_num) {
                count++;
            }
        }
        
        similarity_score += (long)left_num * count;
    }
    
    return similarity_score;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day01_test.txt" 
        : "inputs/day01.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->left_list);
    free(data->right_list);
    free(data);
    
    return 0;
}
