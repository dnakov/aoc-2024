/*
 * Advent of Code 2024 - Day 10
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int** grid;
    int rows;
    int cols;
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
    
    // First pass: count rows and cols
    data->rows = 0;
    data->cols = 0;
    char line[256];
    
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len-1] == '\n') len--;
        if (len > data->cols) data->cols = len;
        data->rows++;
    }
    
    // Allocate grid
    data->grid = malloc(data->rows * sizeof(int*));
    for (int i = 0; i < data->rows; i++) {
        data->grid[i] = malloc(data->cols * sizeof(int));
    }
    
    // Second pass: read the grid
    rewind(file);
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        for (int col = 0; col < data->cols && line[col] && line[col] != '\n'; col++) {
            data->grid[row][col] = line[col] - '0';
        }
        row++;
    }
    
    fclose(file);
    return data;
}

// Direction vectors: up, down, left, right
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

void count_reachable_nines(InputData* data, int row, int col, int current_height, 
                          bool** visited, int* count) {
    // Mark as visited
    visited[row][col] = true;
    
    // If we reached height 9, count it
    if (current_height == 9) {
        (*count)++;
        return;
    }
    
    // Try all four directions
    for (int i = 0; i < 4; i++) {
        int new_row = row + dr[i];
        int new_col = col + dc[i];
        
        // Check bounds
        if (new_row >= 0 && new_row < data->rows && 
            new_col >= 0 && new_col < data->cols) {
            
            // Check if it's exactly one higher and not visited
            if (data->grid[new_row][new_col] == current_height + 1 &&
                !visited[new_row][new_col]) {
                count_reachable_nines(data, new_row, new_col, current_height + 1, 
                                    visited, count);
            }
        }
    }
}

int get_trailhead_score(InputData* data, int start_row, int start_col) {
    // Create visited array
    bool** visited = malloc(data->rows * sizeof(bool*));
    for (int i = 0; i < data->rows; i++) {
        visited[i] = calloc(data->cols, sizeof(bool));
    }
    
    int count = 0;
    count_reachable_nines(data, start_row, start_col, 0, visited, &count);
    
    // Free visited array
    for (int i = 0; i < data->rows; i++) {
        free(visited[i]);
    }
    free(visited);
    
    return count;
}

long part1(InputData* data) {
    long total_score = 0;
    
    // Find all trailheads (positions with height 0)
    for (int row = 0; row < data->rows; row++) {
        for (int col = 0; col < data->cols; col++) {
            if (data->grid[row][col] == 0) {
                total_score += get_trailhead_score(data, row, col);
            }
        }
    }
    
    return total_score;
}

void count_distinct_trails(InputData* data, int row, int col, int current_height, 
                          int* count) {
    // If we reached height 9, count this trail
    if (current_height == 9) {
        (*count)++;
        return;
    }
    
    // Try all four directions
    for (int i = 0; i < 4; i++) {
        int new_row = row + dr[i];
        int new_col = col + dc[i];
        
        // Check bounds
        if (new_row >= 0 && new_row < data->rows && 
            new_col >= 0 && new_col < data->cols) {
            
            // Check if it's exactly one higher
            if (data->grid[new_row][new_col] == current_height + 1) {
                count_distinct_trails(data, new_row, new_col, current_height + 1, count);
            }
        }
    }
}

int get_trailhead_rating(InputData* data, int start_row, int start_col) {
    int count = 0;
    count_distinct_trails(data, start_row, start_col, 0, &count);
    return count;
}

long part2(InputData* data) {
    long total_rating = 0;
    
    // Find all trailheads (positions with height 0)
    for (int row = 0; row < data->rows; row++) {
        for (int col = 0; col < data->cols; col++) {
            if (data->grid[row][col] == 0) {
                total_rating += get_trailhead_rating(data, row, col);
            }
        }
    }
    
    return total_rating;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day10_test.txt" 
        : "inputs/day10.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    for (int i = 0; i < data->rows; i++) {
        free(data->grid[i]);
    }
    free(data->grid);
    free(data);
    
    return 0;
}