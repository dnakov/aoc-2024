/*
 * Advent of Code 2024 - Day 05
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int before;
    int after;
} Rule;

typedef struct {
    int* pages;
    int count;
} Update;

typedef struct {
    Rule* rules;
    int rule_count;
    Update* updates;
    int update_count;
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
    
    // Initialize
    data->rules = NULL;
    data->rule_count = 0;
    data->updates = NULL;
    data->update_count = 0;
    
    char line[256];
    bool parsing_rules = true;
    int rule_capacity = 100;
    int update_capacity = 100;
    
    data->rules = malloc(rule_capacity * sizeof(Rule));
    data->updates = malloc(update_capacity * sizeof(Update));
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        // Empty line separates rules from updates
        if (strlen(line) == 0) {
            parsing_rules = false;
            continue;
        }
        
        if (parsing_rules) {
            // Parse rule: X|Y
            int before, after;
            if (sscanf(line, "%d|%d", &before, &after) == 2) {
                if (data->rule_count >= rule_capacity) {
                    rule_capacity *= 2;
                    data->rules = realloc(data->rules, rule_capacity * sizeof(Rule));
                }
                data->rules[data->rule_count].before = before;
                data->rules[data->rule_count].after = after;
                data->rule_count++;
            }
        } else {
            // Parse update: comma-separated numbers
            if (data->update_count >= update_capacity) {
                update_capacity *= 2;
                data->updates = realloc(data->updates, update_capacity * sizeof(Update));
            }
            
            Update* update = &data->updates[data->update_count];
            update->pages = malloc(100 * sizeof(int));
            update->count = 0;
            
            char* token = strtok(line, ",");
            while (token != NULL) {
                update->pages[update->count++] = atoi(token);
                token = strtok(NULL, ",");
            }
            
            data->update_count++;
        }
    }
    
    fclose(file);
    return data;
}

bool is_valid_order(InputData* data, Update* update) {
    // For each rule, check if it's violated
    for (int i = 0; i < data->rule_count; i++) {
        Rule* rule = &data->rules[i];
        
        // Find positions of both pages in the update
        int before_pos = -1, after_pos = -1;
        for (int j = 0; j < update->count; j++) {
            if (update->pages[j] == rule->before) before_pos = j;
            if (update->pages[j] == rule->after) after_pos = j;
        }
        
        // If both pages exist and are in wrong order, it's invalid
        if (before_pos != -1 && after_pos != -1 && before_pos > after_pos) {
            return false;
        }
    }
    
    return true;
}

int get_middle_page(Update* update) {
    return update->pages[update->count / 2];
}

long part1(InputData* data) {
    long sum = 0;
    
    for (int i = 0; i < data->update_count; i++) {
        if (is_valid_order(data, &data->updates[i])) {
            sum += get_middle_page(&data->updates[i]);
        }
    }
    
    return sum;
}

// Comparison function for sorting based on rules
int compare_pages(const void* a, const void* b, void* arg) {
    InputData* data = (InputData*)arg;
    int page_a = *(int*)a;
    int page_b = *(int*)b;
    
    // Check if there's a rule that determines order
    for (int i = 0; i < data->rule_count; i++) {
        Rule* rule = &data->rules[i];
        if (rule->before == page_a && rule->after == page_b) return -1;
        if (rule->before == page_b && rule->after == page_a) return 1;
    }
    
    return 0;
}

void fix_order(InputData* data, Update* update) {
    // Simple bubble sort based on rules
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < update->count - 1; i++) {
            for (int j = i + 1; j < update->count; j++) {
                // Check if pages[i] should come after pages[j]
                for (int k = 0; k < data->rule_count; k++) {
                    Rule* rule = &data->rules[k];
                    if (rule->before == update->pages[j] && rule->after == update->pages[i]) {
                        // Swap
                        int temp = update->pages[i];
                        update->pages[i] = update->pages[j];
                        update->pages[j] = temp;
                        changed = true;
                        break;
                    }
                }
            }
        }
    }
}

long part2(InputData* data) {
    long sum = 0;
    
    for (int i = 0; i < data->update_count; i++) {
        if (!is_valid_order(data, &data->updates[i])) {
            fix_order(data, &data->updates[i]);
            sum += get_middle_page(&data->updates[i]);
        }
    }
    
    return sum;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day05_test.txt" 
        : "inputs/day05.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    for (int i = 0; i < data->update_count; i++) {
        free(data->updates[i].pages);
    }
    free(data->rules);
    free(data->updates);
    free(data);
    
    return 0;
}