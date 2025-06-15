/*
 * Advent of Code 2024 - Day 08
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int row;
    int col;
    char freq;
} Antenna;

typedef struct {
    char** grid;
    int rows;
    int cols;
    Antenna* antennas;
    int antenna_count;
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
    
    // Allocate antennas array
    data->antennas = malloc(data->rows * data->cols * sizeof(Antenna));
    data->antenna_count = 0;
    
    // Second pass: read the grid and find antennas
    rewind(file);
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0';
        
        for (int col = 0; col < strlen(line); col++) {
            data->grid[row][col] = line[col];
            
            // Check if it's an antenna (alphanumeric)
            if ((line[col] >= 'A' && line[col] <= 'Z') ||
                (line[col] >= 'a' && line[col] <= 'z') ||
                (line[col] >= '0' && line[col] <= '9')) {
                data->antennas[data->antenna_count].row = row;
                data->antennas[data->antenna_count].col = col;
                data->antennas[data->antenna_count].freq = line[col];
                data->antenna_count++;
            }
        }
        row++;
    }
    
    fclose(file);
    return data;
}

bool is_in_bounds(InputData* data, int row, int col) {
    return row >= 0 && row < data->rows && col >= 0 && col < data->cols;
}

long part1(InputData* data) {
    // Create antinode grid
    bool** antinode = malloc(data->rows * sizeof(bool*));
    for (int i = 0; i < data->rows; i++) {
        antinode[i] = calloc(data->cols, sizeof(bool));
    }
    
    // Check each pair of antennas
    for (int i = 0; i < data->antenna_count; i++) {
        for (int j = i + 1; j < data->antenna_count; j++) {
            if (data->antennas[i].freq != data->antennas[j].freq) continue;
            
            int r1 = data->antennas[i].row;
            int c1 = data->antennas[i].col;
            int r2 = data->antennas[j].row;
            int c2 = data->antennas[j].col;
            
            // Calculate the two antinode positions
            // Antinode 1: twice the distance from antenna 1
            int ar1 = 2 * r2 - r1;
            int ac1 = 2 * c2 - c1;
            
            // Antinode 2: twice the distance from antenna 2
            int ar2 = 2 * r1 - r2;
            int ac2 = 2 * c1 - c2;
            
            // Mark antinodes if in bounds
            if (is_in_bounds(data, ar1, ac1)) {
                antinode[ar1][ac1] = true;
            }
            if (is_in_bounds(data, ar2, ac2)) {
                antinode[ar2][ac2] = true;
            }
        }
    }
    
    // Count antinodes
    int count = 0;
    for (int row = 0; row < data->rows; row++) {
        for (int col = 0; col < data->cols; col++) {
            if (antinode[row][col]) count++;
        }
    }
    
    // Free antinode grid
    for (int i = 0; i < data->rows; i++) {
        free(antinode[i]);
    }
    free(antinode);
    
    return count;
}

long part2(InputData* data) {
    // Create antinode grid
    bool** antinode = malloc(data->rows * sizeof(bool*));
    for (int i = 0; i < data->rows; i++) {
        antinode[i] = calloc(data->cols, sizeof(bool));
    }
    
    // Check each pair of antennas
    for (int i = 0; i < data->antenna_count; i++) {
        for (int j = i + 1; j < data->antenna_count; j++) {
            if (data->antennas[i].freq != data->antennas[j].freq) continue;
            
            int r1 = data->antennas[i].row;
            int c1 = data->antennas[i].col;
            int r2 = data->antennas[j].row;
            int c2 = data->antennas[j].col;
            
            // Calculate direction vector
            int dr = r2 - r1;
            int dc = c2 - c1;
            
            // Find GCD to reduce the direction vector
            int gcd = 1;
            int a = abs(dr);
            int b = abs(dc);
            while (b != 0) {
                int temp = b;
                b = a % b;
                a = temp;
            }
            if (a > 0) gcd = a;
            
            dr /= gcd;
            dc /= gcd;
            
            // Mark all points along the line in both directions
            // Start from antenna 1 and go backwards
            int r = r1;
            int c = c1;
            while (is_in_bounds(data, r, c)) {
                antinode[r][c] = true;
                r -= dr;
                c -= dc;
            }
            
            // Start from antenna 1 and go forwards
            r = r1;
            c = c1;
            while (is_in_bounds(data, r, c)) {
                antinode[r][c] = true;
                r += dr;
                c += dc;
            }
        }
    }
    
    // Count antinodes
    int count = 0;
    for (int row = 0; row < data->rows; row++) {
        for (int col = 0; col < data->cols; col++) {
            if (antinode[row][col]) count++;
        }
    }
    
    // Free antinode grid
    for (int i = 0; i < data->rows; i++) {
        free(antinode[i]);
    }
    free(antinode);
    
    return count;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day08_test.txt" 
        : "inputs/day08.txt";
    
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
    free(data->antennas);
    free(data);
    
    return 0;
}