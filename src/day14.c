/*
 * Advent of Code 2024 - Day 14
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int px, py;  // Position
    int vx, vy;  // Velocity
} Robot;

typedef struct {
    Robot* robots;
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
    
    data->robots = malloc(sizeof(Robot) * 1000); // Allocate enough space
    data->count = 0;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Robot* r = &data->robots[data->count];
        if (sscanf(line, "p=%d,%d v=%d,%d", &r->px, &r->py, &r->vx, &r->vy) == 4) {
            data->count++;
        }
    }
    
    fclose(file);
    return data;
}

// Positive modulo to handle negative numbers correctly
int mod(int a, int b) {
    return ((a % b) + b) % b;
}

long part1(InputData* data) {
    // Grid dimensions (use test dimensions if test mode)
    int width = 101;
    int height = 103;
    
    // Check if we're in test mode by looking at the number of robots
    if (data->count == 12) {
        width = 11;
        height = 7;
    }
    
    int seconds = 100;
    
    // Calculate final positions after 100 seconds
    int* final_x = malloc(sizeof(int) * data->count);
    int* final_y = malloc(sizeof(int) * data->count);
    
    for (int i = 0; i < data->count; i++) {
        Robot* r = &data->robots[i];
        
        // Calculate position after 100 seconds with wrapping
        final_x[i] = mod(r->px + r->vx * seconds, width);
        final_y[i] = mod(r->py + r->vy * seconds, height);
    }
    
    // Count robots in each quadrant
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    int mid_x = width / 2;
    int mid_y = height / 2;
    
    for (int i = 0; i < data->count; i++) {
        // Skip robots on the middle lines
        if (final_x[i] == mid_x || final_y[i] == mid_y) {
            continue;
        }
        
        if (final_x[i] < mid_x && final_y[i] < mid_y) {
            q1++; // Top-left
        } else if (final_x[i] > mid_x && final_y[i] < mid_y) {
            q2++; // Top-right
        } else if (final_x[i] < mid_x && final_y[i] > mid_y) {
            q3++; // Bottom-left
        } else if (final_x[i] > mid_x && final_y[i] > mid_y) {
            q4++; // Bottom-right
        }
    }
    
    free(final_x);
    free(final_y);
    
    return (long)q1 * q2 * q3 * q4;
}

// Check if robots form a dense pattern (likely a Christmas tree)
int has_dense_pattern(int* x_pos, int* y_pos, int count, int width, int height) {
    // Create a grid to mark robot positions
    int* grid = calloc(width * height, sizeof(int));
    
    for (int i = 0; i < count; i++) {
        grid[y_pos[i] * width + x_pos[i]] = 1;
    }
    
    // Look for a large connected component or dense area
    // Check for horizontal lines (tree branches)
    int max_consecutive = 0;
    
    for (int y = 0; y < height; y++) {
        int consecutive = 0;
        for (int x = 0; x < width; x++) {
            if (grid[y * width + x]) {
                consecutive++;
                if (consecutive > max_consecutive) {
                    max_consecutive = consecutive;
                }
            } else {
                consecutive = 0;
            }
        }
    }
    
    free(grid);
    
    // If we find a long horizontal line (> 10 robots), it might be part of the tree
    return max_consecutive > 10;
}

long part2(InputData* data) {
    // Grid dimensions
    int width = 101;
    int height = 103;
    
    // Skip test mode for part 2
    if (data->count == 12) {
        return 0;
    }
    
    // Allocate arrays for current positions
    int* x_pos = malloc(sizeof(int) * data->count);
    int* y_pos = malloc(sizeof(int) * data->count);
    
    // Initialize positions
    for (int i = 0; i < data->count; i++) {
        x_pos[i] = data->robots[i].px;
        y_pos[i] = data->robots[i].py;
    }
    
    // Simulate up to 10000 seconds
    for (int second = 1; second <= 10000; second++) {
        // Update positions
        for (int i = 0; i < data->count; i++) {
            Robot* r = &data->robots[i];
            x_pos[i] = mod(x_pos[i] + r->vx, width);
            y_pos[i] = mod(y_pos[i] + r->vy, height);
        }
        
        // Check if robots form a dense pattern
        if (has_dense_pattern(x_pos, y_pos, data->count, width, height)) {
            // Optionally print the pattern
            if (second < 8000) { // Only print early patterns
                printf("\nPotential pattern at second %d:\n", second);
                
                // Create and print grid
                char* grid = calloc(width * height, sizeof(char));
                for (int i = 0; i < width * height; i++) grid[i] = '.';
                
                for (int i = 0; i < data->count; i++) {
                    grid[y_pos[i] * width + x_pos[i]] = '#';
                }
                
                for (int y = 0; y < height; y++) {
                    for (int x = 0; x < width; x++) {
                        printf("%c", grid[y * width + x]);
                    }
                    printf("\n");
                }
                
                free(grid);
                free(x_pos);
                free(y_pos);
                return second;
            }
        }
    }
    
    free(x_pos);
    free(y_pos);
    return 0;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day14_test.txt" 
        : "inputs/day14.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->robots);
    free(data);
    
    return 0;
}
