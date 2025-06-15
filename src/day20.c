/*
 * Advent of Code 2024 - Day 20: Race Condition
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_SIZE 150

typedef struct {
    int x, y;
} Position;

typedef struct {
    char grid[MAX_SIZE][MAX_SIZE];
    int rows, cols;
    Position start, end;
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
    while (fgets(line, sizeof(line), file) && data->rows < MAX_SIZE) {
        line[strcspn(line, "\n")] = '\0';
        data->cols = strlen(line);
        
        for (int c = 0; c < data->cols; c++) {
            data->grid[data->rows][c] = line[c];
            if (line[c] == 'S') {
                data->start.x = c;
                data->start.y = data->rows;
            } else if (line[c] == 'E') {
                data->end.x = c;
                data->end.y = data->rows;
            }
        }
        data->rows++;
    }
    
    fclose(file);
    return data;
}

// BFS to find distances from a starting position
void find_distances(InputData* data, Position start, int dist[MAX_SIZE][MAX_SIZE]) {
    // Initialize distances
    for (int y = 0; y < data->rows; y++) {
        for (int x = 0; x < data->cols; x++) {
            dist[y][x] = INT_MAX;
        }
    }
    
    // Queue for BFS
    Position queue[MAX_SIZE * MAX_SIZE];
    int qhead = 0, qtail = 0;
    
    queue[qtail++] = start;
    dist[start.y][start.x] = 0;
    
    int dx[] = {0, 1, 0, -1};
    int dy[] = {-1, 0, 1, 0};
    
    while (qhead < qtail) {
        Position curr = queue[qhead++];
        
        for (int d = 0; d < 4; d++) {
            int nx = curr.x + dx[d];
            int ny = curr.y + dy[d];
            
            if (nx < 0 || nx >= data->cols || ny < 0 || ny >= data->rows) continue;
            if (data->grid[ny][nx] == '#') continue;
            if (dist[ny][nx] != INT_MAX) continue;
            
            dist[ny][nx] = dist[curr.y][curr.x] + 1;
            queue[qtail].x = nx;
            queue[qtail].y = ny;
            qtail++;
        }
    }
}

long part1(InputData* data) {
    // Find distances from start and end
    int dist_from_start[MAX_SIZE][MAX_SIZE];
    int dist_from_end[MAX_SIZE][MAX_SIZE];
    
    find_distances(data, data->start, dist_from_start);
    find_distances(data, data->end, dist_from_end);
    
    // Normal path length
    int normal_path_length = dist_from_start[data->end.y][data->end.x];
    
    int count = 0;
    int min_savings = (data->rows < 20) ? 1 : 100; // Test vs real based on grid size
    
    // Check all possible cheats (2 picosecond cheats)
    for (int y1 = 0; y1 < data->rows; y1++) {
        for (int x1 = 0; x1 < data->cols; x1++) {
            if (data->grid[y1][x1] == '#') continue;
            
            // Try all positions within Manhattan distance 2
            for (int dy = -2; dy <= 2; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    if (abs(dx) + abs(dy) > 2) continue;
                    if (dx == 0 && dy == 0) continue;
                    
                    int x2 = x1 + dx;
                    int y2 = y1 + dy;
                    
                    if (x2 < 0 || x2 >= data->cols || y2 < 0 || y2 >= data->rows) continue;
                    if (data->grid[y2][x2] == '#') continue;
                    
                    // Calculate path with cheat
                    int cheat_dist = abs(dx) + abs(dy);
                    int path_with_cheat = dist_from_start[y1][x1] + cheat_dist + dist_from_end[y2][x2];
                    
                    int savings = normal_path_length - path_with_cheat;
                    if (savings >= min_savings) {
                        count++;
                    }
                }
            }
        }
    }
    
    return count;
}

long part2(InputData* data) {
    // Find distances from start and end
    int dist_from_start[MAX_SIZE][MAX_SIZE];
    int dist_from_end[MAX_SIZE][MAX_SIZE];
    
    find_distances(data, data->start, dist_from_start);
    find_distances(data, data->end, dist_from_end);
    
    // Normal path length
    int normal_path_length = dist_from_start[data->end.y][data->end.x];
    
    int count = 0;
    int min_savings = (data->rows < 20) ? 50 : 100; // Test vs real based on grid size
    int max_cheat_time = 20; // Part 2: up to 20 picoseconds
    
    // Check all possible cheats (up to 20 picosecond cheats)
    for (int y1 = 0; y1 < data->rows; y1++) {
        for (int x1 = 0; x1 < data->cols; x1++) {
            if (data->grid[y1][x1] == '#') continue;
            if (dist_from_start[y1][x1] == INT_MAX) continue;
            
            // Try all positions within Manhattan distance max_cheat_time
            for (int y2 = 0; y2 < data->rows; y2++) {
                for (int x2 = 0; x2 < data->cols; x2++) {
                    if (data->grid[y2][x2] == '#') continue;
                    if (dist_from_end[y2][x2] == INT_MAX) continue;
                    
                    int cheat_dist = abs(x2 - x1) + abs(y2 - y1);
                    if (cheat_dist == 0 || cheat_dist > max_cheat_time) continue;
                    
                    // Calculate path with cheat
                    int path_with_cheat = dist_from_start[y1][x1] + cheat_dist + dist_from_end[y2][x2];
                    
                    int savings = normal_path_length - path_with_cheat;
                    if (savings >= min_savings) {
                        // Only count if we're actually saving time compared to normal path
                        int normal_dist_between = abs(dist_from_start[y2][x2] - dist_from_start[y1][x1]);
                        if (normal_dist_between > cheat_dist) {
                            count++;
                        }
                    }
                }
            }
        }
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day20_test.txt" 
        : "inputs/day20.txt";
    
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
