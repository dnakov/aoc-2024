/*
 * Advent of Code 2024 - Day 03
 * 
 * Problem: Parse corrupted memory for valid mul(X,Y) instructions
 * Part 1: Sum all multiplication results
 * Part 2: Handle do() and don't() instructions that enable/disable mul operations
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    char* content;
    size_t length;
} InputData;

InputData* parse_input(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    InputData* data = malloc(sizeof(InputData));
    if (!data) {
        fclose(file);
        return NULL;
    }
    
    // Read entire file into memory
    data->content = malloc(file_size + 1);
    if (!data->content) {
        free(data);
        fclose(file);
        return NULL;
    }
    
    data->length = fread(data->content, 1, file_size, file);
    data->content[data->length] = '\0';
    
    fclose(file);
    return data;
}

long part1(InputData* data) {
    long sum = 0;
    regex_t regex;
    regmatch_t matches[3];
    
    // Regex pattern for mul(X,Y) where X and Y are 1-3 digit numbers
    const char* pattern = "mul\\(([0-9]{1,3}),([0-9]{1,3})\\)";
    
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Failed to compile regex\n");
        return -1;
    }
    
    char* ptr = data->content;
    while (regexec(&regex, ptr, 3, matches, 0) == 0) {
        // Extract the numbers
        char num1_str[4] = {0};
        char num2_str[4] = {0};
        
        int len1 = matches[1].rm_eo - matches[1].rm_so;
        int len2 = matches[2].rm_eo - matches[2].rm_so;
        
        strncpy(num1_str, ptr + matches[1].rm_so, len1);
        strncpy(num2_str, ptr + matches[2].rm_so, len2);
        
        int num1 = atoi(num1_str);
        int num2 = atoi(num2_str);
        
        sum += num1 * num2;
        
        // Move pointer past the current match
        ptr += matches[0].rm_eo;
    }
    
    regfree(&regex);
    return sum;
}

long part2(InputData* data) {
    long sum = 0;
    regex_t mul_regex, do_regex, dont_regex;
    regmatch_t matches[3];
    int enabled = 1; // mul instructions start enabled
    
    // Compile regex patterns
    const char* mul_pattern = "mul\\(([0-9]{1,3}),([0-9]{1,3})\\)";
    const char* do_pattern = "do\\(\\)";
    const char* dont_pattern = "don't\\(\\)";
    
    if (regcomp(&mul_regex, mul_pattern, REG_EXTENDED) != 0 ||
        regcomp(&do_regex, do_pattern, REG_EXTENDED) != 0 ||
        regcomp(&dont_regex, dont_pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Failed to compile regex\n");
        return -1;
    }
    
    // Process character by character to maintain order
    for (size_t i = 0; i < data->length; ) {
        char* ptr = data->content + i;
        
        // Check for do()
        if (regexec(&do_regex, ptr, 1, matches, 0) == 0 && matches[0].rm_so == 0) {
            enabled = 1;
            i += matches[0].rm_eo;
            continue;
        }
        
        // Check for don't()
        if (regexec(&dont_regex, ptr, 1, matches, 0) == 0 && matches[0].rm_so == 0) {
            enabled = 0;
            i += matches[0].rm_eo;
            continue;
        }
        
        // Check for mul(X,Y)
        if (regexec(&mul_regex, ptr, 3, matches, 0) == 0 && matches[0].rm_so == 0) {
            if (enabled) {
                // Extract the numbers
                char num1_str[4] = {0};
                char num2_str[4] = {0};
                
                int len1 = matches[1].rm_eo - matches[1].rm_so;
                int len2 = matches[2].rm_eo - matches[2].rm_so;
                
                strncpy(num1_str, ptr + matches[1].rm_so, len1);
                strncpy(num2_str, ptr + matches[2].rm_so, len2);
                
                int num1 = atoi(num1_str);
                int num2 = atoi(num2_str);
                
                sum += num1 * num2;
            }
            i += matches[0].rm_eo;
            continue;
        }
        
        // No match, move to next character
        i++;
    }
    
    regfree(&mul_regex);
    regfree(&do_regex);
    regfree(&dont_regex);
    return sum;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day03_test.txt" 
        : "inputs/day03.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->content);
    free(data);
    
    return 0;
}
