/*
 * Advent of Code 2024 - Day 12
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_SIZE 150

typedef struct {
    char grid[MAX_SIZE][MAX_SIZE];
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
    
    data->rows = 0;
    data->cols = 0;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        if (len > 0) {
            strcpy(data->grid[data->rows], line);
            if (data->cols == 0) data->cols = len;
            data->rows++;
        }
    }
    
    fclose(file);
    return data;
}

// Directions for exploring neighbors
int dr[] = {-1, 0, 1, 0};
int dc[] = {0, 1, 0, -1};

// DFS to find a region and calculate its area and perimeter
void dfs(InputData* data, int visited[MAX_SIZE][MAX_SIZE], int r, int c, char plant, int* area, int* perimeter) {
    if (r < 0 || r >= data->rows || c < 0 || c >= data->cols) {
        (*perimeter)++;
        return;
    }
    
    if (data->grid[r][c] != plant) {
        (*perimeter)++;
        return;
    }
    
    if (visited[r][c]) {
        return;
    }
    
    visited[r][c] = 1;
    (*area)++;
    
    for (int i = 0; i < 4; i++) {
        dfs(data, visited, r + dr[i], c + dc[i], plant, area, perimeter);
    }
}

// DFS to find a region and mark it
void dfs_mark(InputData* data, int visited[MAX_SIZE][MAX_SIZE], int r, int c, char plant, int region_id) {
    if (r < 0 || r >= data->rows || c < 0 || c >= data->cols || visited[r][c] || data->grid[r][c] != plant) {
        return;
    }
    
    visited[r][c] = region_id;
    
    for (int i = 0; i < 4; i++) {
        dfs_mark(data, visited, r + dr[i], c + dc[i], plant, region_id);
    }
}

// Count the number of corners for a region (which equals the number of sides)
int count_corners(InputData* data, int region[MAX_SIZE][MAX_SIZE], int region_id) {
    int corners = 0;
    
    // Check each cell and its neighbors for corner patterns
    for (int r = -1; r <= data->rows; r++) {
        for (int c = -1; c <= data->cols; c++) {
            // Check 2x2 patterns for corners
            // Get the 4 cells in the 2x2 grid
            int tl = (r >= 0 && r < data->rows && c >= 0 && c < data->cols && region[r][c] == region_id) ? 1 : 0;
            int tr = (r >= 0 && r < data->rows && c+1 >= 0 && c+1 < data->cols && region[r][c+1] == region_id) ? 1 : 0;
            int bl = (r+1 >= 0 && r+1 < data->rows && c >= 0 && c < data->cols && region[r+1][c] == region_id) ? 1 : 0;
            int br = (r+1 >= 0 && r+1 < data->rows && c+1 >= 0 && c+1 < data->cols && region[r+1][c+1] == region_id) ? 1 : 0;
            
            int count = tl + tr + bl + br;
            
            // Count corners based on the pattern
            if (count == 1) {
                // Single cell = 1 corner
                corners++;
            } else if (count == 2) {
                // Two cells diagonal = 2 corners
                if ((tl && br && !tr && !bl) || (!tl && !br && tr && bl)) {
                    corners += 2;
                }
            } else if (count == 3) {
                // Three cells = 1 inner corner
                corners++;
            }
        }
    }
    
    return corners;
}

long part1(InputData* data) {
    int visited[MAX_SIZE][MAX_SIZE] = {0};
    long total_price = 0;
    
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            if (!visited[r][c]) {
                int area = 0;
                int perimeter = 0;
                dfs(data, visited, r, c, data->grid[r][c], &area, &perimeter);
                total_price += (long)area * perimeter;
            }
        }
    }
    
    return total_price;
}

long part2(InputData* data) {
    int region[MAX_SIZE][MAX_SIZE] = {0};
    int region_id = 1;
    long total_price = 0;
    
    // Mark all regions with unique IDs
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            if (region[r][c] == 0) {
                dfs_mark(data, region, r, c, data->grid[r][c], region_id);
                region_id++;
            }
        }
    }
    
    // Calculate price for each region
    for (int id = 1; id < region_id; id++) {
        int area = 0;
        
        // Count area for this region
        for (int r = 0; r < data->rows; r++) {
            for (int c = 0; c < data->cols; c++) {
                if (region[r][c] == id) {
                    area++;
                }
            }
        }
        
        // Count corners for this region (equals number of sides)
        int corners = count_corners(data, region, id);
        
        total_price += (long)area * corners;
    }
    
    return total_price;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day12_test.txt" 
        : "inputs/day12.txt";
    
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
