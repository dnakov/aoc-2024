/*
 * Advent of Code 2024 - Day 25: Code Chronicle
 * 
 * This problem involves finding compatible lock/key pairs based on
 * column heights that don't overlap.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_SCHEMATICS 500
#define SCHEMATIC_HEIGHT 7
#define SCHEMATIC_WIDTH 5

typedef struct {
    int heights[SCHEMATIC_WIDTH];
    bool is_lock; // true for lock, false for key
} Schematic;

typedef struct {
    Schematic schematics[MAX_SCHEMATICS];
    int num_schematics;
} InputData;

// Convert a schematic grid to column heights
void parse_schematic(char grid[SCHEMATIC_HEIGHT][SCHEMATIC_WIDTH + 2], Schematic* s) {
    // Check if it's a lock (top row all #) or key (bottom row all #)
    s->is_lock = (grid[0][0] == '#');
    
    // Count heights for each column
    for (int col = 0; col < SCHEMATIC_WIDTH; col++) {
        int height = 0;
        
        if (s->is_lock) {
            // For locks, count from top down
            for (int row = 0; row < SCHEMATIC_HEIGHT; row++) {
                if (grid[row][col] == '#') {
                    height++;
                } else {
                    break;
                }
            }
            s->heights[col] = height - 1; // Subtract 1 as per problem description
        } else {
            // For keys, count from bottom up
            for (int row = SCHEMATIC_HEIGHT - 1; row >= 0; row--) {
                if (grid[row][col] == '#') {
                    height++;
                } else {
                    break;
                }
            }
            s->heights[col] = height - 1; // Subtract 1 as per problem description
        }
    }
}

// Check if a lock and key fit together
bool fits(const Schematic* lock, const Schematic* key) {
    // Check each column - total height must not exceed 5 (7 - 2 for top/bottom rows)
    for (int i = 0; i < SCHEMATIC_WIDTH; i++) {
        if (lock->heights[i] + key->heights[i] > 5) {
            return false;
        }
    }
    return true;
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
    
    data->num_schematics = 0;
    char line[256];
    char grid[SCHEMATIC_HEIGHT][SCHEMATIC_WIDTH + 2];
    int row = 0;
    
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        
        if (strlen(line) == 0) {
            // End of schematic
            if (row == SCHEMATIC_HEIGHT) {
                parse_schematic(grid, &data->schematics[data->num_schematics]);
                data->num_schematics++;
            }
            row = 0;
        } else {
            // Add line to current schematic
            if (row < SCHEMATIC_HEIGHT) {
                strcpy(grid[row], line);
                row++;
            }
        }
    }
    
    // Handle last schematic if file doesn't end with blank line
    if (row == SCHEMATIC_HEIGHT) {
        parse_schematic(grid, &data->schematics[data->num_schematics]);
        data->num_schematics++;
    }
    
    fclose(file);
    return data;
}

long part1(InputData* data) {
    int count = 0;
    
    // Check each lock against each key
    for (int i = 0; i < data->num_schematics; i++) {
        if (!data->schematics[i].is_lock) continue;
        
        for (int j = 0; j < data->num_schematics; j++) {
            if (data->schematics[j].is_lock) continue;
            
            if (fits(&data->schematics[i], &data->schematics[j])) {
                count++;
            }
        }
    }
    
    return count;
}

long part2(InputData* data) {
    // Day 25 traditionally has no Part 2 - it's given for free!
    (void)data;
    return 0;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day25_test.txt" 
        : "inputs/day25.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: Merry Christmas!\n");
    
    free(data);
    
    return 0;
}