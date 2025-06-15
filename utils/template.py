#!/usr/bin/env python3
import sys
import os
from pathlib import Path

C_TEMPLATE = '''/*
 * Advent of Code 2024 - Day {day:02d}
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {{
    // TODO: Define data structures for the problem
    int placeholder;
}} InputData;

InputData* parse_input(const char* filename) {{
    FILE* file = fopen(filename, "r");
    if (!file) {{
        perror("Error opening file");
        return NULL;
    }}
    
    InputData* data = malloc(sizeof(InputData));
    if (!data) {{
        fclose(file);
        return NULL;
    }}
    
    // TODO: Implement parsing logic
    char line[256];
    while (fgets(line, sizeof(line), file)) {{
        // Parse each line
    }}
    
    fclose(file);
    return data;
}}

long part1(InputData* data) {{
    // TODO: Implement part 1 solution
    (void)data; // Suppress unused parameter warning
    return 0;
}}

long part2(InputData* data) {{
    // TODO: Implement part 2 solution
    (void)data; // Suppress unused parameter warning
    return 0;
}}

int main(int argc, char* argv[]) {{
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day{day:02d}_test.txt" 
        : "inputs/day{day:02d}.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {{
        fprintf(stderr, "Failed to parse input\\n");
        return 1;
    }}
    
    printf("Part 1: %ld\\n", part1(data));
    printf("Part 2: %ld\\n", part2(data));
    
    // TODO: Free allocated memory
    free(data);
    
    return 0;
}}
'''

def create_day(day):
    """Create solution template for a specific day."""
    day = int(day)
    
    # Create solution file
    solution_path = Path(f"src/day{day:02d}.c")
    if solution_path.exists():
        print(f"Solution file already exists: {solution_path}")
    else:
        solution_path.write_text(C_TEMPLATE.format(day=day))
        print(f"Created: {solution_path}")
    
    # Create placeholder for test input
    test_input_path = Path(f"inputs/day{day:02d}_test.txt")
    if not test_input_path.exists():
        test_input_path.touch()
        print(f"Created: {test_input_path}")

if __name__ == "__main__":
    if len(sys.argv) < 3 or sys.argv[1] != "create":
        print("Usage: python utils/template.py create <day>")
        sys.exit(1)
    
    create_day(sys.argv[2])