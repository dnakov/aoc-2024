/*
 * Advent of Code 2024 - Day 18: RAM Run
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_BYTES 4000
#define GRID_SIZE 71
#define TEST_GRID_SIZE 7

typedef struct {
    int x, y;
} Coord;

typedef struct {
    Coord bytes[MAX_BYTES];
    int num_bytes;
    bool is_test;
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
    
    data->num_bytes = 0;
    data->is_test = strstr(filename, "test") != NULL;
    
    char line[256];
    while (fgets(line, sizeof(line), file) && data->num_bytes < MAX_BYTES) {
        int x, y;
        if (sscanf(line, "%d,%d", &x, &y) == 2) {
            data->bytes[data->num_bytes].x = x;
            data->bytes[data->num_bytes].y = y;
            data->num_bytes++;
        }
    }
    
    fclose(file);
    return data;
}

// BFS to find shortest path
int find_shortest_path(bool grid[GRID_SIZE][GRID_SIZE], int grid_size) {
    // Queue for BFS
    Coord queue[GRID_SIZE * GRID_SIZE];
    int dist[GRID_SIZE][GRID_SIZE];
    
    // Initialize distances
    for (int i = 0; i < grid_size; i++) {
        for (int j = 0; j < grid_size; j++) {
            dist[i][j] = INT_MAX;
        }
    }
    
    // Start from (0,0)
    queue[0].x = 0;
    queue[0].y = 0;
    dist[0][0] = 0;
    int qhead = 0, qtail = 1;
    
    // Directions: up, right, down, left
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    
    while (qhead < qtail) {
        Coord curr = queue[qhead++];
        
        // Try all 4 directions
        for (int d = 0; d < 4; d++) {
            int nx = curr.x + dx[d];
            int ny = curr.y + dy[d];
            
            // Check bounds
            if (nx < 0 || nx >= grid_size || ny < 0 || ny >= grid_size) continue;
            
            // Check if corrupted or already visited
            if (grid[ny][nx] || dist[ny][nx] != INT_MAX) continue;
            
            // Update distance and add to queue
            dist[ny][nx] = dist[curr.y][curr.x] + 1;
            queue[qtail].x = nx;
            queue[qtail].y = ny;
            qtail++;
        }
    }
    
    return dist[grid_size-1][grid_size-1];
}

long part1(InputData* data) {
    int grid_size = data->is_test ? TEST_GRID_SIZE : GRID_SIZE;
    int bytes_to_simulate = data->is_test ? 12 : 1024;
    
    // Create grid
    bool grid[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            grid[i][j] = false;
        }
    }
    
    // Mark corrupted bytes
    for (int i = 0; i < bytes_to_simulate && i < data->num_bytes; i++) {
        int x = data->bytes[i].x;
        int y = data->bytes[i].y;
        if (x < grid_size && y < grid_size) {
            grid[y][x] = true; // Note: y is row, x is column
        }
    }
    
    // Find shortest path
    int result = find_shortest_path(grid, grid_size);
    return result == INT_MAX ? -1 : result;
}

long part2(InputData* data) {
    int grid_size = data->is_test ? TEST_GRID_SIZE : GRID_SIZE;
    
    // Binary search to find the first byte that blocks the path
    int left = 0;
    int right = data->num_bytes - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = (left + right) / 2;
        
        // Create grid with first 'mid' bytes
        bool grid[GRID_SIZE][GRID_SIZE];
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                grid[i][j] = false;
            }
        }
        
        // Mark corrupted bytes up to mid
        for (int i = 0; i <= mid && i < data->num_bytes; i++) {
            int x = data->bytes[i].x;
            int y = data->bytes[i].y;
            if (x < grid_size && y < grid_size) {
                grid[y][x] = true;
            }
        }
        
        // Check if path exists
        int dist = find_shortest_path(grid, grid_size);
        
        if (dist == INT_MAX) {
            // No path exists, the blocking byte is at or before mid
            result = mid;
            right = mid - 1;
        } else {
            // Path exists, the blocking byte is after mid
            left = mid + 1;
        }
    }
    
    // Print the coordinates of the blocking byte
    if (result >= 0 && result < data->num_bytes) {
        printf("%d,%d\n", data->bytes[result].x, data->bytes[result].y);
    }
    
    return 0; // Return 0 since we're printing the coordinates
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day18_test.txt" 
        : "inputs/day18.txt";
    
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
