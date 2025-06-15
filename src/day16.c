/*
 * Advent of Code 2024 - Day 16: Reindeer Maze
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_SIZE 200

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3
} Direction;

typedef struct {
    int row, col;
    Direction dir;
} State;

typedef struct {
    char grid[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    int start_row, start_col;
    int end_row, end_col;
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
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0';
        
        strcpy(data->grid[data->rows], line);
        data->cols = strlen(line);
        
        // Find start and end positions
        for (int c = 0; c < data->cols; c++) {
            if (data->grid[data->rows][c] == 'S') {
                data->start_row = data->rows;
                data->start_col = c;
            } else if (data->grid[data->rows][c] == 'E') {
                data->end_row = data->rows;
                data->end_col = c;
            }
        }
        
        data->rows++;
    }
    
    fclose(file);
    return data;
}

// Dijkstra's algorithm for finding shortest path with rotation costs
long part1(InputData* data) {
    // Cost array: [row][col][direction]
    int cost[MAX_SIZE][MAX_SIZE][4];
    bool visited[MAX_SIZE][MAX_SIZE][4];
    
    // Initialize costs to infinity
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            for (int d = 0; d < 4; d++) {
                cost[r][c][d] = INT_MAX;
                visited[r][c][d] = false;
            }
        }
    }
    
    // Start facing East
    cost[data->start_row][data->start_col][EAST] = 0;
    
    // Direction vectors: North, East, South, West
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};
    
    // Priority queue implemented with simple array (for small grid)
    while (true) {
        // Find minimum cost unvisited state
        int min_cost = INT_MAX;
        int min_r = -1, min_c = -1, min_d = -1;
        
        for (int r = 0; r < data->rows; r++) {
            for (int c = 0; c < data->cols; c++) {
                for (int d = 0; d < 4; d++) {
                    if (!visited[r][c][d] && cost[r][c][d] < min_cost) {
                        min_cost = cost[r][c][d];
                        min_r = r;
                        min_c = c;
                        min_d = d;
                    }
                }
            }
        }
        
        if (min_r == -1) break; // No more reachable states
        
        visited[min_r][min_c][min_d] = true;
        
        // Try moving forward
        int new_r = min_r + dr[min_d];
        int new_c = min_c + dc[min_d];
        if (new_r >= 0 && new_r < data->rows && new_c >= 0 && new_c < data->cols &&
            data->grid[new_r][new_c] != '#') {
            int new_cost = min_cost + 1;
            if (new_cost < cost[new_r][new_c][min_d]) {
                cost[new_r][new_c][min_d] = new_cost;
            }
        }
        
        // Try rotating clockwise
        int cw_dir = (min_d + 1) % 4;
        int cw_cost = min_cost + 1000;
        if (cw_cost < cost[min_r][min_c][cw_dir]) {
            cost[min_r][min_c][cw_dir] = cw_cost;
        }
        
        // Try rotating counter-clockwise
        int ccw_dir = (min_d + 3) % 4;
        int ccw_cost = min_cost + 1000;
        if (ccw_cost < cost[min_r][min_c][ccw_dir]) {
            cost[min_r][min_c][ccw_dir] = ccw_cost;
        }
    }
    
    // Find minimum cost at end position
    int min_end_cost = INT_MAX;
    for (int d = 0; d < 4; d++) {
        if (cost[data->end_row][data->end_col][d] < min_end_cost) {
            min_end_cost = cost[data->end_row][data->end_col][d];
        }
    }
    
    return min_end_cost;
}

// Part 2: Count all tiles that are part of any optimal path
long part2(InputData* data) {
    // First, find the minimum cost using same algorithm as part 1
    int cost[MAX_SIZE][MAX_SIZE][4];
    bool visited[MAX_SIZE][MAX_SIZE][4];
    
    // Initialize costs to infinity
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            for (int d = 0; d < 4; d++) {
                cost[r][c][d] = INT_MAX;
                visited[r][c][d] = false;
            }
        }
    }
    
    // Start facing East
    cost[data->start_row][data->start_col][EAST] = 0;
    
    // Direction vectors: North, East, South, West
    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};
    
    // Run Dijkstra's algorithm
    while (true) {
        int min_cost = INT_MAX;
        int min_r = -1, min_c = -1, min_d = -1;
        
        for (int r = 0; r < data->rows; r++) {
            for (int c = 0; c < data->cols; c++) {
                for (int d = 0; d < 4; d++) {
                    if (!visited[r][c][d] && cost[r][c][d] < min_cost) {
                        min_cost = cost[r][c][d];
                        min_r = r;
                        min_c = c;
                        min_d = d;
                    }
                }
            }
        }
        
        if (min_r == -1) break;
        
        visited[min_r][min_c][min_d] = true;
        
        // Try moving forward
        int new_r = min_r + dr[min_d];
        int new_c = min_c + dc[min_d];
        if (new_r >= 0 && new_r < data->rows && new_c >= 0 && new_c < data->cols &&
            data->grid[new_r][new_c] != '#') {
            int new_cost = min_cost + 1;
            if (new_cost < cost[new_r][new_c][min_d]) {
                cost[new_r][new_c][min_d] = new_cost;
            }
        }
        
        // Try rotating clockwise
        int cw_dir = (min_d + 1) % 4;
        int cw_cost = min_cost + 1000;
        if (cw_cost < cost[min_r][min_c][cw_dir]) {
            cost[min_r][min_c][cw_dir] = cw_cost;
        }
        
        // Try rotating counter-clockwise
        int ccw_dir = (min_d + 3) % 4;
        int ccw_cost = min_cost + 1000;
        if (ccw_cost < cost[min_r][min_c][ccw_dir]) {
            cost[min_r][min_c][ccw_dir] = ccw_cost;
        }
    }
    
    // Find minimum cost at end position
    int min_end_cost = INT_MAX;
    for (int d = 0; d < 4; d++) {
        if (cost[data->end_row][data->end_col][d] < min_end_cost) {
            min_end_cost = cost[data->end_row][data->end_col][d];
        }
    }
    
    // Now backtrack from all end states with minimum cost to find all optimal paths
    bool on_optimal_path[MAX_SIZE][MAX_SIZE];
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            on_optimal_path[r][c] = false;
        }
    }
    
    // Queue for BFS backtracking
    State queue[MAX_SIZE * MAX_SIZE * 4];
    int queue_size = 0;
    
    // Add all end states with minimum cost
    for (int d = 0; d < 4; d++) {
        if (cost[data->end_row][data->end_col][d] == min_end_cost) {
            queue[queue_size].row = data->end_row;
            queue[queue_size].col = data->end_col;
            queue[queue_size].dir = d;
            queue_size++;
            on_optimal_path[data->end_row][data->end_col] = true;
        }
    }
    
    // Backtrack through all optimal paths
    for (int i = 0; i < queue_size; i++) {
        int r = queue[i].row;
        int c = queue[i].col;
        int d = queue[i].dir;
        int current_cost = cost[r][c][d];
        
        // Check if we could have come from moving forward
        int prev_r = r - dr[d];
        int prev_c = c - dc[d];
        if (prev_r >= 0 && prev_r < data->rows && prev_c >= 0 && prev_c < data->cols &&
            data->grid[prev_r][prev_c] != '#' && cost[prev_r][prev_c][d] == current_cost - 1) {
            on_optimal_path[prev_r][prev_c] = true;
            // Add to queue if not already processed
            bool found = false;
            for (int j = 0; j < queue_size; j++) {
                if (queue[j].row == prev_r && queue[j].col == prev_c && queue[j].dir == (Direction)d) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                queue[queue_size].row = prev_r;
                queue[queue_size].col = prev_c;
                queue[queue_size].dir = d;
                queue_size++;
            }
        }
        
        // Check if we could have come from rotating
        // Clockwise rotation
        int from_dir_cw = (d + 3) % 4; // Reverse of clockwise
        if (cost[r][c][from_dir_cw] == current_cost - 1000) {
            on_optimal_path[r][c] = true;
            bool found = false;
            for (int j = 0; j < queue_size; j++) {
                if (queue[j].row == r && queue[j].col == c && queue[j].dir == (Direction)from_dir_cw) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                queue[queue_size].row = r;
                queue[queue_size].col = c;
                queue[queue_size].dir = from_dir_cw;
                queue_size++;
            }
        }
        
        // Counter-clockwise rotation
        int from_dir_ccw = (d + 1) % 4; // Reverse of counter-clockwise
        if (cost[r][c][from_dir_ccw] == current_cost - 1000) {
            on_optimal_path[r][c] = true;
            bool found = false;
            for (int j = 0; j < queue_size; j++) {
                if (queue[j].row == r && queue[j].col == c && queue[j].dir == (Direction)from_dir_ccw) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                queue[queue_size].row = r;
                queue[queue_size].col = c;
                queue[queue_size].dir = from_dir_ccw;
                queue_size++;
            }
        }
    }
    
    // Count tiles on optimal paths
    int count = 0;
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            if (on_optimal_path[r][c]) {
                count++;
            }
        }
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    const char* input_file;
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        input_file = "inputs/day16_test.txt";
    } else if (argc > 1 && strcmp(argv[1], "test2") == 0) {
        input_file = "inputs/day16_test2.txt";
    } else {
        input_file = "inputs/day16.txt";
    }
    
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
