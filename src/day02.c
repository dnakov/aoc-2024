/*
 * Advent of Code 2024 - Day 02
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int* levels;
    int count;
} Report;

typedef struct {
    Report* reports;
    int report_count;
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
    
    // Count lines first
    int capacity = 1000;
    data->reports = malloc(capacity * sizeof(Report));
    data->report_count = 0;
    
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (data->report_count >= capacity) {
            capacity *= 2;
            data->reports = realloc(data->reports, capacity * sizeof(Report));
        }
        
        // Parse numbers from line
        int level_capacity = 20;
        int* levels = malloc(level_capacity * sizeof(int));
        int count = 0;
        
        char* ptr = line;
        int num;
        int offset;
        while (sscanf(ptr, "%d%n", &num, &offset) == 1) {
            if (count >= level_capacity) {
                level_capacity *= 2;
                levels = realloc(levels, level_capacity * sizeof(int));
            }
            levels[count++] = num;
            ptr += offset;
        }
        
        if (count > 0) {
            data->reports[data->report_count].levels = levels;
            data->reports[data->report_count].count = count;
            data->report_count++;
        } else {
            free(levels);
        }
    }
    
    fclose(file);
    return data;
}

int is_safe_report(int* levels, int count) {
    if (count < 2) return 1;
    
    // Determine if increasing or decreasing
    int increasing = levels[1] > levels[0];
    
    for (int i = 1; i < count; i++) {
        int diff = levels[i] - levels[i-1];
        
        // Check if direction is consistent
        if (increasing && diff <= 0) return 0;
        if (!increasing && diff >= 0) return 0;
        
        // Check if difference is between 1 and 3
        int abs_diff = abs(diff);
        if (abs_diff < 1 || abs_diff > 3) return 0;
    }
    
    return 1;
}

long part1(InputData* data) {
    int safe_count = 0;
    
    for (int i = 0; i < data->report_count; i++) {
        if (is_safe_report(data->reports[i].levels, data->reports[i].count)) {
            safe_count++;
        }
    }
    
    return safe_count;
}

long part2(InputData* data) {
    int safe_count = 0;
    
    for (int i = 0; i < data->report_count; i++) {
        Report* report = &data->reports[i];
        
        // First check if already safe
        if (is_safe_report(report->levels, report->count)) {
            safe_count++;
            continue;
        }
        
        // Try removing each level one at a time
        for (int skip = 0; skip < report->count; skip++) {
            // Create temporary array without the skipped element
            int* temp = malloc((report->count - 1) * sizeof(int));
            int idx = 0;
            for (int j = 0; j < report->count; j++) {
                if (j != skip) {
                    temp[idx++] = report->levels[j];
                }
            }
            
            if (is_safe_report(temp, report->count - 1)) {
                safe_count++;
                free(temp);
                break;
            }
            
            free(temp);
        }
    }
    
    return safe_count;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day02_test.txt" 
        : "inputs/day02.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    for (int i = 0; i < data->report_count; i++) {
        free(data->reports[i].levels);
    }
    free(data->reports);
    free(data);
    
    return 0;
}
