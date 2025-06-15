/*
 * Advent of Code 2024 - Day 17: Chronospatial Computer
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_PROGRAM_SIZE 100

typedef struct {
    long long reg_a, reg_b, reg_c;
    int program[MAX_PROGRAM_SIZE];
    int program_size;
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
    
    char line[256];
    // Parse Register A
    fgets(line, sizeof(line), file);
    sscanf(line, "Register A: %lld", &data->reg_a);
    
    // Parse Register B
    fgets(line, sizeof(line), file);
    sscanf(line, "Register B: %lld", &data->reg_b);
    
    // Parse Register C
    fgets(line, sizeof(line), file);
    sscanf(line, "Register C: %lld", &data->reg_c);
    
    // Skip empty line
    fgets(line, sizeof(line), file);
    
    // Parse Program
    fgets(line, sizeof(line), file);
    char* program_str = strstr(line, "Program: ") + 9;
    
    data->program_size = 0;
    char* token = strtok(program_str, ",");
    while (token != NULL) {
        data->program[data->program_size++] = atoi(token);
        token = strtok(NULL, ",");
    }
    
    fclose(file);
    return data;
}

// Get combo operand value
long long get_combo_value(int operand, long long reg_a, long long reg_b, long long reg_c) {
    if (operand <= 3) return operand;
    if (operand == 4) return reg_a;
    if (operand == 5) return reg_b;
    if (operand == 6) return reg_c;
    return 0; // operand 7 is reserved
}

// Run the computer program and return the output as a string
char* run_program(InputData* data) {
    long long reg_a = data->reg_a;
    long long reg_b = data->reg_b;
    long long reg_c = data->reg_c;
    
    static char output[1000];
    output[0] = '\0';
    int output_len = 0;
    
    int ip = 0; // instruction pointer
    
    while (ip < data->program_size - 1) {
        int opcode = data->program[ip];
        int operand = data->program[ip + 1];
        
        switch (opcode) {
            case 0: // adv - division
                reg_a = reg_a / (1LL << get_combo_value(operand, reg_a, reg_b, reg_c));
                break;
                
            case 1: // bxl - bitwise XOR with literal
                reg_b = reg_b ^ operand;
                break;
                
            case 2: // bst - modulo 8
                reg_b = get_combo_value(operand, reg_a, reg_b, reg_c) % 8;
                break;
                
            case 3: // jnz - jump if not zero
                if (reg_a != 0) {
                    ip = operand;
                    continue; // Don't increment ip by 2
                }
                break;
                
            case 4: // bxc - bitwise XOR B and C
                reg_b = reg_b ^ reg_c;
                break;
                
            case 5: // out - output
                if (output_len > 0) {
                    output[output_len++] = ',';
                }
                output_len += sprintf(output + output_len, "%lld", get_combo_value(operand, reg_a, reg_b, reg_c) % 8);
                break;
                
            case 6: // bdv - division to B
                reg_b = reg_a / (1LL << get_combo_value(operand, reg_a, reg_b, reg_c));
                break;
                
            case 7: // cdv - division to C
                reg_c = reg_a / (1LL << get_combo_value(operand, reg_a, reg_b, reg_c));
                break;
        }
        
        ip += 2;
    }
    
    return output;
}

long part1(InputData* data) {
    printf("%s\n", run_program(data));
    return 0; // Return 0 since we're printing the string output
}

// Check if running with given A value produces the target program
int check_quine(InputData* data, long long test_a) {
    InputData test_data = *data;
    test_data.reg_a = test_a;
    
    char* output = run_program(&test_data);
    
    // Convert program to expected output string
    char expected[1000];
    expected[0] = '\0';
    int len = 0;
    for (int i = 0; i < data->program_size; i++) {
        if (i > 0) {
            expected[len++] = ',';
        }
        len += sprintf(expected + len, "%d", data->program[i]);
    }
    
    return strcmp(output, expected) == 0;
}

// Recursive function to find A that produces the program as output
long long find_a_recursive(InputData* data, int position, long long base_a) {
    if (position < 0) return base_a;
    
    // Try all 8 possible values for the next 3 bits
    for (int digit = 0; digit < 8; digit++) {
        long long test_a = (base_a << 3) | digit;
        
        // Run program with this A value
        InputData test_data = *data;
        test_data.reg_a = test_a;
        char* output = run_program(&test_data);
        
        // Parse output
        int values[MAX_PROGRAM_SIZE];
        int count = 0;
        char* copy = strdup(output);
        char* token = strtok(copy, ",");
        while (token != NULL && count < MAX_PROGRAM_SIZE) {
            values[count++] = atoi(token);
            token = strtok(NULL, ",");
        }
        free(copy);
        
        // Check if we have enough output
        int target_len = data->program_size - position;
        if (count >= target_len) {
            // Check if the last target_len digits match
            int matches = 1;
            for (int i = 0; i < target_len; i++) {
                if (values[count - target_len + i] != data->program[position + i]) {
                    matches = 0;
                    break;
                }
            }
            if (matches) {
                // Try to extend this solution
                long long result = find_a_recursive(data, position - 1, test_a);
                if (result != -1) return result;
            }
        }
    }
    
    return -1;
}

long part2(InputData* data) {
    // Find the lowest value of A that makes the program output itself
    // Start from the end of the program and work backwards
    return find_a_recursive(data, data->program_size - 1, 0);
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day17_test.txt" 
        : "inputs/day17.txt";
    
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
