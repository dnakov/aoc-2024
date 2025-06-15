/*
 * Advent of Code 2024 - Day 07
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    long target;
    int* numbers;
    int count;
} Equation;

typedef struct {
    Equation* equations;
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
    
    data->equations = NULL;
    data->count = 0;
    int capacity = 100;
    data->equations = malloc(capacity * sizeof(Equation));
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (data->count >= capacity) {
            capacity *= 2;
            data->equations = realloc(data->equations, capacity * sizeof(Equation));
        }
        
        Equation* eq = &data->equations[data->count];
        
        // Parse target value
        char* colon = strchr(line, ':');
        if (!colon) continue;
        
        *colon = '\0';
        eq->target = atol(line);
        
        // Parse numbers
        eq->numbers = malloc(50 * sizeof(int));
        eq->count = 0;
        
        char* token = strtok(colon + 1, " \n");
        while (token != NULL) {
            eq->numbers[eq->count++] = atoi(token);
            token = strtok(NULL, " \n");
        }
        
        data->count++;
    }
    
    fclose(file);
    return data;
}

bool can_reach_target(long current, int* numbers, int index, int count, long target, bool part2) {
    if (index == count) {
        return current == target;
    }
    
    // Try addition
    if (can_reach_target(current + numbers[index], numbers, index + 1, count, target, part2)) {
        return true;
    }
    
    // Try multiplication
    if (can_reach_target(current * numbers[index], numbers, index + 1, count, target, part2)) {
        return true;
    }
    
    // Part 2: Try concatenation
    if (part2) {
        // Calculate concatenated value
        long concat = current;
        int temp = numbers[index];
        int digits = 0;
        
        // Count digits in the number to concatenate
        if (temp == 0) {
            digits = 1;
        } else {
            int t = temp;
            while (t > 0) {
                digits++;
                t /= 10;
            }
        }
        
        // Shift current value and add the new number
        for (int i = 0; i < digits; i++) {
            concat *= 10;
        }
        concat += numbers[index];
        
        if (can_reach_target(concat, numbers, index + 1, count, target, part2)) {
            return true;
        }
    }
    
    return false;
}

long part1(InputData* data) {
    long sum = 0;
    
    for (int i = 0; i < data->count; i++) {
        Equation* eq = &data->equations[i];
        
        if (can_reach_target(eq->numbers[0], eq->numbers, 1, eq->count, eq->target, false)) {
            sum += eq->target;
        }
    }
    
    return sum;
}

long part2(InputData* data) {
    long sum = 0;
    
    for (int i = 0; i < data->count; i++) {
        Equation* eq = &data->equations[i];
        
        if (can_reach_target(eq->numbers[0], eq->numbers, 1, eq->count, eq->target, true)) {
            sum += eq->target;
        }
    }
    
    return sum;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day07_test.txt" 
        : "inputs/day07.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    for (int i = 0; i < data->count; i++) {
        free(data->equations[i].numbers);
    }
    free(data->equations);
    free(data);
    
    return 0;
}