/*
 * Advent of Code 2024 - Day 13
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int ax, ay;  // Button A movement
    int bx, by;  // Button B movement
    long px, py; // Prize location
} Machine;

typedef struct {
    Machine* machines;
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
    
    data->machines = malloc(sizeof(Machine) * 500); // Allocate enough space
    data->count = 0;
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Button A:", 9) == 0) {
            Machine* m = &data->machines[data->count];
            sscanf(line, "Button A: X+%d, Y+%d", &m->ax, &m->ay);
            
            // Read Button B
            fgets(line, sizeof(line), file);
            sscanf(line, "Button B: X+%d, Y+%d", &m->bx, &m->by);
            
            // Read Prize
            fgets(line, sizeof(line), file);
            sscanf(line, "Prize: X=%ld, Y=%ld", &m->px, &m->py);
            
            data->count++;
        }
    }
    
    fclose(file);
    return data;
}

// Solve for button presses using linear algebra
// ax * a + bx * b = px
// ay * a + by * b = py
int solve_machine(Machine* m, long* a_presses, long* b_presses) {
    // Using Cramer's rule
    long det = (long)m->ax * m->by - (long)m->ay * m->bx;
    
    if (det == 0) {
        return 0; // No unique solution
    }
    
    long det_a = m->px * m->by - m->py * m->bx;
    long det_b = m->ax * m->py - m->ay * m->px;
    
    // Check if solutions are integers
    if (det_a % det != 0 || det_b % det != 0) {
        return 0; // Not integer solutions
    }
    
    *a_presses = det_a / det;
    *b_presses = det_b / det;
    
    // Check if solutions are non-negative and within limit
    if (*a_presses < 0 || *b_presses < 0 || *a_presses > 100 || *b_presses > 100) {
        return 0;
    }
    
    // Verify the solution
    if (m->ax * (*a_presses) + m->bx * (*b_presses) == m->px &&
        m->ay * (*a_presses) + m->by * (*b_presses) == m->py) {
        return 1;
    }
    
    return 0;
}

// Solve for button presses without button press limit (for part 2)
int solve_machine_no_limit(Machine* m, long* a_presses, long* b_presses) {
    // Using Cramer's rule
    long det = (long)m->ax * m->by - (long)m->ay * m->bx;
    
    if (det == 0) {
        return 0; // No unique solution
    }
    
    long det_a = m->px * m->by - m->py * m->bx;
    long det_b = m->ax * m->py - m->ay * m->px;
    
    // Check if solutions are integers
    if (det_a % det != 0 || det_b % det != 0) {
        return 0; // Not integer solutions
    }
    
    *a_presses = det_a / det;
    *b_presses = det_b / det;
    
    // Check if solutions are non-negative (no upper limit in part 2)
    if (*a_presses < 0 || *b_presses < 0) {
        return 0;
    }
    
    // Verify the solution
    if (m->ax * (*a_presses) + m->bx * (*b_presses) == m->px &&
        m->ay * (*a_presses) + m->by * (*b_presses) == m->py) {
        return 1;
    }
    
    return 0;
}

long part1(InputData* data) {
    long total_tokens = 0;
    
    for (int i = 0; i < data->count; i++) {
        long a, b;
        if (solve_machine(&data->machines[i], &a, &b)) {
            total_tokens += 3 * a + b;
        }
    }
    
    return total_tokens;
}

long part2(InputData* data) {
    long total_tokens = 0;
    const long offset = 10000000000000L;
    
    for (int i = 0; i < data->count; i++) {
        Machine m = data->machines[i];
        // Add the large offset to prize coordinates
        m.px += offset;
        m.py += offset;
        
        long a, b;
        if (solve_machine_no_limit(&m, &a, &b)) {
            total_tokens += 3 * a + b;
        }
    }
    
    return total_tokens;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day13_test.txt" 
        : "inputs/day13.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->machines);
    free(data);
    
    return 0;
}
