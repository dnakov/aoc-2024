/*
 * Advent of Code 2024 - Day 21: Keypad Conundrum
 * 
 * This problem involves robots typing codes on keypads through multiple layers
 * of indirection. We need to find the shortest sequence of button presses.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_CODES 10
#define MAX_CODE_LEN 10
#define MEMO_SIZE 1000000

// Position on a keypad
typedef struct {
    int row, col;
} Pos;

// Numeric keypad layout
static const char* NUM_PAD[] = {
    "789",
    "456", 
    "123",
    " 0A"
};

// Directional keypad layout
static const char* DIR_PAD[] = {
    " ^A",
    "<v>"
};

typedef struct {
    char codes[MAX_CODES][MAX_CODE_LEN];
    int num_codes;
} InputData;

// Memoization for dynamic programming
typedef struct {
    char from, to;
    int level;
    long cost;
} MemoEntry;

static MemoEntry memo[MEMO_SIZE];
static int memo_count = 0;

// Find position of a key on the numeric keypad
Pos find_num_pos(char key) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 3; c++) {
            if (NUM_PAD[r][c] == key) {
                return (Pos){r, c};
            }
        }
    }
    return (Pos){-1, -1};
}

// Find position of a key on the directional keypad
Pos find_dir_pos(char key) {
    for (int r = 0; r < 2; r++) {
        for (int c = 0; c < 3; c++) {
            if (DIR_PAD[r][c] == key) {
                return (Pos){r, c};
            }
        }
    }
    return (Pos){-1, -1};
}

// Check if a position is valid on the numeric keypad
bool is_valid_num_pos(int r, int c) {
    if (r < 0 || r >= 4 || c < 0 || c >= 3) return false;
    return NUM_PAD[r][c] != ' ';
}

// Check if a position is valid on the directional keypad
bool is_valid_dir_pos(int r, int c) {
    if (r < 0 || r >= 2 || c < 0 || c >= 3) return false;
    return DIR_PAD[r][c] != ' ';
}

// Generate moves to go from one position to another on numeric keypad
void generate_num_moves(Pos from, Pos to, char* moves) {
    int idx = 0;
    
    // Prioritize moves that avoid the gap (0,0)
    // First do vertical moves if safe
    if (from.col == 0 && to.row == 3) {
        // Moving to bottom row from leftmost column - go right first
        while (from.col < to.col) {
            moves[idx++] = '>';
            from.col++;
        }
        while (from.row < to.row) {
            moves[idx++] = 'v';
            from.row++;
        }
    } else if (from.row == 3 && to.col == 0) {
        // Moving to leftmost column from bottom row - go up first
        while (from.row > to.row) {
            moves[idx++] = '^';
            from.row--;
        }
        while (from.col > to.col) {
            moves[idx++] = '<';
            from.col--;
        }
    } else {
        // Normal case - prefer left/down moves first
        while (from.col > to.col) {
            moves[idx++] = '<';
            from.col--;
        }
        while (from.row < to.row) {
            moves[idx++] = 'v';
            from.row++;
        }
        while (from.row > to.row) {
            moves[idx++] = '^';
            from.row--;
        }
        while (from.col < to.col) {
            moves[idx++] = '>';
            from.col++;
        }
    }
    
    moves[idx++] = 'A';
    moves[idx] = '\0';
}

// Generate moves to go from one position to another on directional keypad
void generate_dir_moves(Pos from, Pos to, char* moves) {
    int idx = 0;
    
    // Prioritize moves that avoid the gap (0,0)
    if (from.col == 0 && to.row == 0) {
        // Moving to top row from leftmost column - go right first
        while (from.col < to.col) {
            moves[idx++] = '>';
            from.col++;
        }
        while (from.row > to.row) {
            moves[idx++] = '^';
            from.row--;
        }
    } else if (from.row == 0 && to.col == 0) {
        // Moving to leftmost column from top row - go down first
        while (from.row < to.row) {
            moves[idx++] = 'v';
            from.row++;
        }
        while (from.col > to.col) {
            moves[idx++] = '<';
            from.col--;
        }
    } else {
        // Normal case - prefer left/down moves first
        while (from.col > to.col) {
            moves[idx++] = '<';
            from.col--;
        }
        while (from.row < to.row) {
            moves[idx++] = 'v';
            from.row++;
        }
        while (from.row > to.row) {
            moves[idx++] = '^';
            from.row--;
        }
        while (from.col < to.col) {
            moves[idx++] = '>';
            from.col++;
        }
    }
    
    moves[idx++] = 'A';
    moves[idx] = '\0';
}

// Find memoized result
long find_memo(char from, char to, int level) {
    for (int i = 0; i < memo_count; i++) {
        if (memo[i].from == from && memo[i].to == to && memo[i].level == level) {
            return memo[i].cost;
        }
    }
    return -1;
}

// Add to memoization
void add_memo(char from, char to, int level, long cost) {
    if (memo_count < MEMO_SIZE) {
        memo[memo_count].from = from;
        memo[memo_count].to = to;
        memo[memo_count].level = level;
        memo[memo_count].cost = cost;
        memo_count++;
    }
}

// Calculate minimum moves needed at a given level
long min_moves(char from, char to, int level, bool is_numeric) {
    if (level == 0) {
        return 1; // Direct press
    }
    
    // Check memo
    long cached = find_memo(from, to, level);
    if (cached != -1) {
        return cached;
    }
    
    char moves[20];
    if (is_numeric) {
        Pos from_pos = find_num_pos(from);
        Pos to_pos = find_num_pos(to);
        generate_num_moves(from_pos, to_pos, moves);
    } else {
        Pos from_pos = find_dir_pos(from);
        Pos to_pos = find_dir_pos(to);
        generate_dir_moves(from_pos, to_pos, moves);
    }
    
    // Calculate cost of this sequence at the next level
    long total = 0;
    char prev = 'A';
    for (int i = 0; moves[i]; i++) {
        total += min_moves(prev, moves[i], level - 1, false);
        prev = moves[i];
    }
    
    add_memo(from, to, level, total);
    return total;
}

// Calculate the cost of typing a code with given number of robot levels
long calculate_code_cost(const char* code, int num_robots) {
    long total = 0;
    char prev = 'A';
    
    for (int i = 0; code[i]; i++) {
        total += min_moves(prev, code[i], num_robots, true);
        prev = code[i];
    }
    
    return total;
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
    
    data->num_codes = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) && data->num_codes < MAX_CODES) {
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) > 0) {
            strcpy(data->codes[data->num_codes], line);
            data->num_codes++;
        }
    }
    
    fclose(file);
    return data;
}

long part1(InputData* data) {
    long total = 0;
    memo_count = 0; // Reset memoization
    
    for (int i = 0; i < data->num_codes; i++) {
        long cost = calculate_code_cost(data->codes[i], 3); // 2 dir robots + 1 human
        
        // Extract numeric part
        int numeric_part = 0;
        for (int j = 0; data->codes[i][j]; j++) {
            if (data->codes[i][j] >= '0' && data->codes[i][j] <= '9') {
                numeric_part = numeric_part * 10 + (data->codes[i][j] - '0');
            }
        }
        
        long complexity = cost * numeric_part;
        total += complexity;
    }
    
    return total;
}

long part2(InputData* data) {
    long total = 0;
    memo_count = 0; // Reset memoization
    
    for (int i = 0; i < data->num_codes; i++) {
        long cost = calculate_code_cost(data->codes[i], 26); // 25 dir robots + 1 human
        
        // Extract numeric part
        int numeric_part = 0;
        for (int j = 0; data->codes[i][j]; j++) {
            if (data->codes[i][j] >= '0' && data->codes[i][j] <= '9') {
                numeric_part = numeric_part * 10 + (data->codes[i][j] - '0');
            }
        }
        
        long complexity = cost * numeric_part;
        total += complexity;
    }
    
    return total;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day21_test.txt" 
        : "inputs/day21.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    free(data);
    
    return 0;
}