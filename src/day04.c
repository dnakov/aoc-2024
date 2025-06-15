/*
 * Advent of Code 2024 - Day 04
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    char** grid;
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
    
    // Count rows and get max column width
    char line[256];
    data->rows = 0;
    data->cols = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len-1] == '\n') len--;
        if (len > data->cols) data->cols = len;
        data->rows++;
    }
    
    // Allocate grid
    data->grid = malloc(data->rows * sizeof(char*));
    for (int i = 0; i < data->rows; i++) {
        data->grid[i] = malloc((data->cols + 1) * sizeof(char));
    }
    
    // Read the grid
    rewind(file);
    int row = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (line[len-1] == '\n') line[len-1] = '\0';
        strcpy(data->grid[row], line);
        row++;
    }
    
    fclose(file);
    
    return data;
}

// Check if "XMAS" appears starting at (row, col) in the given direction
int check_xmas(InputData* data, int row, int col, int dr, int dc) {
    const char* target = "XMAS";
    
    for (int i = 0; i < 4; i++) {
        int r = row + i * dr;
        int c = col + i * dc;
        
        // Check bounds
        if (r < 0 || r >= data->rows || c < 0 || c >= data->cols) {
            return 0;
        }
        
        // Check character match
        if (data->grid[r][c] != target[i]) {
            return 0;
        }
    }
    
    return 1;
}

long part1(InputData* data) {
    long count = 0;
    
    // 8 directions: up, up-right, right, down-right, down, down-left, left, up-left
    int directions[8][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1}
    };
    
    // Check every position as a potential starting point
    for (int row = 0; row < data->rows; row++) {
        for (int col = 0; col < data->cols; col++) {
            // Try all 8 directions
            for (int d = 0; d < 8; d++) {
                if (check_xmas(data, row, col, directions[d][0], directions[d][1])) {
                    count++;
                }
            }
        }
    }
    
    return count;
}

// Check if there's an X-MAS pattern centered at (row, col)
int check_xmas_pattern(InputData* data, int row, int col) {
    // Must be centered on 'A'
    if (data->grid[row][col] != 'A') {
        return 0;
    }
    
    // Check bounds for corners
    if (row - 1 < 0 || row + 1 >= data->rows || col - 1 < 0 || col + 1 >= data->cols) {
        return 0;
    }
    
    // Get corner characters
    char nw = data->grid[row - 1][col - 1];  // Northwest
    char ne = data->grid[row - 1][col + 1];  // Northeast
    char sw = data->grid[row + 1][col - 1];  // Southwest
    char se = data->grid[row + 1][col + 1];  // Southeast
    
    // Check if main diagonal (NW to SE) forms MAS or SAM
    int main_diag_mas = (nw == 'M' && se == 'S');
    int main_diag_sam = (nw == 'S' && se == 'M');
    
    // Check if anti-diagonal (NE to SW) forms MAS or SAM
    int anti_diag_mas = (ne == 'M' && sw == 'S');
    int anti_diag_sam = (ne == 'S' && sw == 'M');
    
    // Valid if both diagonals form either MAS or SAM
    return (main_diag_mas || main_diag_sam) && (anti_diag_mas || anti_diag_sam);
}

long part2(InputData* data) {
    long count = 0;
    
    // Check every position as a potential center (skip borders)
    for (int row = 1; row < data->rows - 1; row++) {
        for (int col = 1; col < data->cols - 1; col++) {
            if (check_xmas_pattern(data, row, col)) {
                count++;
            }
        }
    }
    
    return count;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day04_test.txt" 
        : "inputs/day04.txt";
    
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
