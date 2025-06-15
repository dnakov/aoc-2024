/*
 * Advent of Code 2024 - Day 06
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    char** grid;
    int rows;
    int cols;
    int guard_row;
    int guard_col;
    int guard_dir; // 0=North, 1=East, 2=South, 3=West
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
    data->grid = malloc(data->rows * sizeof(char*));
    for (int i = 0; i < data->rows; i++) {
        data->grid[i] = malloc(data->cols + 1);
        memset(data->grid[i], '.', data->cols);
        data->grid[i][data->cols] = '\0';
    }
    
    // Second pass: read the grid
    rewind(file);
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0';
        
        for (int col = 0; col < strlen(line); col++) {
            data->grid[row][col] = line[col];
            
            // Find guard position and direction
            if (line[col] == '^') {
                data->guard_row = row;
                data->guard_col = col;
                data->guard_dir = 0; // North
                data->grid[row][col] = '.'; // Clear guard marker
            }
        }
        row++;
    }
    
    fclose(file);
    return data;
}

// Direction vectors: North, East, South, West
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

bool is_in_bounds(InputData* data, int row, int col) {
    return row >= 0 && row < data->rows && col >= 0 && col < data->cols;
}

long part1(InputData* data) {
    // Create visited grid
    bool** visited = malloc(data->rows * sizeof(bool*));
    for (int i = 0; i < data->rows; i++) {
        visited[i] = calloc(data->cols, sizeof(bool));
    }
    
    int row = data->guard_row;
    int col = data->guard_col;
    int dir = data->guard_dir;
    
    // Mark starting position as visited
    visited[row][col] = true;
    int count = 1;
    
    while (true) {
        // Calculate next position
        int next_row = row + dr[dir];
        int next_col = col + dc[dir];
        
        // Check if guard leaves the map
        if (!is_in_bounds(data, next_row, next_col)) {
            break;
        }
        
        // Check for obstacle
        if (data->grid[next_row][next_col] == '#') {
            // Turn right
            dir = (dir + 1) % 4;
        } else {
            // Move forward
            row = next_row;
            col = next_col;
            
            // Mark as visited
            if (!visited[row][col]) {
                visited[row][col] = true;
                count++;
            }
        }
    }
    
    // Free visited grid
    for (int i = 0; i < data->rows; i++) {
        free(visited[i]);
    }
    free(visited);
    
    return count;
}

bool creates_loop(InputData* data, int obstacle_row, int obstacle_col) {
    // Temporarily place obstacle
    char original = data->grid[obstacle_row][obstacle_col];
    if (original == '#') return false; // Already an obstacle
    if (obstacle_row == data->guard_row && obstacle_col == data->guard_col) return false; // Can't place at start
    
    data->grid[obstacle_row][obstacle_col] = '#';
    
    // Track visited states (position + direction)
    int state_capacity = 10000;
    int* visited_states = malloc(state_capacity * sizeof(int));
    int state_count = 0;
    
    int row = data->guard_row;
    int col = data->guard_col;
    int dir = data->guard_dir;
    
    bool is_loop = false;
    
    while (true) {
        // Check if we've been in this state before
        int state = (row * data->cols + col) * 4 + dir;
        
        for (int i = 0; i < state_count; i++) {
            if (visited_states[i] == state) {
                is_loop = true;
                break;
            }
        }
        
        if (is_loop) break;
        
        // Add current state
        if (state_count >= state_capacity) {
            state_capacity *= 2;
            visited_states = realloc(visited_states, state_capacity * sizeof(int));
        }
        visited_states[state_count++] = state;
        
        // Calculate next position
        int next_row = row + dr[dir];
        int next_col = col + dc[dir];
        
        // Check if guard leaves the map
        if (!is_in_bounds(data, next_row, next_col)) {
            break;
        }
        
        // Check for obstacle
        if (data->grid[next_row][next_col] == '#') {
            // Turn right
            dir = (dir + 1) % 4;
        } else {
            // Move forward
            row = next_row;
            col = next_col;
        }
    }
    
    // Restore original
    data->grid[obstacle_row][obstacle_col] = original;
    free(visited_states);
    
    return is_loop;
}

long part2(InputData* data) {
    int count = 0;
    
    // Try placing an obstacle at each position
    for (int row = 0; row < data->rows; row++) {
        for (int col = 0; col < data->cols; col++) {
            if (creates_loop(data, row, col)) {
                count++;
            }
        }
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day06_test.txt" 
        : "inputs/day06.txt";
    
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