/*
 * Advent of Code 2024 - Day 09
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

typedef struct {
    int* disk;
    int size;
    char* input;
    int input_len;
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
    
    // Read the input string
    data->input = malloc(20000);
    if (fgets(data->input, 20000, file) == NULL) {
        free(data->input);
        free(data);
        fclose(file);
        return NULL;
    }
    
    // Remove newline
    data->input_len = strlen(data->input);
    if (data->input[data->input_len - 1] == '\n') {
        data->input[data->input_len - 1] = '\0';
        data->input_len--;
    }
    
    // Calculate disk size
    data->size = 0;
    for (int i = 0; i < data->input_len; i++) {
        data->size += data->input[i] - '0';
    }
    
    // Allocate disk (-1 for empty space)
    data->disk = malloc(data->size * sizeof(int));
    
    // Fill disk
    int pos = 0;
    int file_id = 0;
    for (int i = 0; i < data->input_len; i++) {
        int len = data->input[i] - '0';
        
        if (i % 2 == 0) {
            // File
            for (int j = 0; j < len; j++) {
                data->disk[pos++] = file_id;
            }
            file_id++;
        } else {
            // Free space
            for (int j = 0; j < len; j++) {
                data->disk[pos++] = -1;
            }
        }
    }
    
    fclose(file);
    return data;
}

void compact_disk_part1(int* disk, int size) {
    // Move file blocks from right to leftmost free space
    int left = 0;
    int right = size - 1;
    
    while (left < right) {
        // Find next free space from left
        while (left < size && disk[left] != -1) {
            left++;
        }
        
        // Find next file block from right
        while (right >= 0 && disk[right] == -1) {
            right--;
        }
        
        if (left < right) {
            // Move block
            disk[left] = disk[right];
            disk[right] = -1;
            left++;
            right--;
        }
    }
}

long calculate_checksum(int* disk, int size) {
    long checksum = 0;
    for (int i = 0; i < size; i++) {
        if (disk[i] != -1) {
            checksum += (long)i * disk[i];
        }
    }
    return checksum;
}

long part1(InputData* data) {
    // Create a copy of the disk
    int* disk = malloc(data->size * sizeof(int));
    memcpy(disk, data->disk, data->size * sizeof(int));
    
    // Compact the disk
    compact_disk_part1(disk, data->size);
    
    // Calculate checksum
    long checksum = calculate_checksum(disk, data->size);
    
    free(disk);
    return checksum;
}

void compact_disk_part2(int* disk, int size, char* input, int input_len) {
    // Get file count
    int file_count = (input_len + 1) / 2;
    
    // Process files in decreasing order of file ID
    for (int file_id = file_count - 1; file_id >= 0; file_id--) {
        // Find the file's current position and size
        int file_start = -1;
        int file_size = 0;
        
        // Calculate expected file size from input
        int input_index = file_id * 2;
        if (input_index < input_len) {
            file_size = input[input_index] - '0';
        }
        
        // Find where the file currently is
        for (int i = 0; i < size; i++) {
            if (disk[i] == file_id) {
                if (file_start == -1) file_start = i;
            }
        }
        
        if (file_start == -1 || file_size == 0) continue;
        
        // Find leftmost span of free space that can fit the file
        int best_pos = -1;
        for (int i = 0; i < file_start; i++) {
            if (disk[i] == -1) {
                // Check if we have enough contiguous free space
                int free_count = 0;
                for (int j = i; j < size && disk[j] == -1; j++) {
                    free_count++;
                    if (free_count >= file_size) {
                        best_pos = i;
                        break;
                    }
                }
                if (best_pos != -1) break;
            }
        }
        
        // If found a suitable position, move the file
        if (best_pos != -1) {
            // Clear old position
            for (int i = file_start; i < file_start + file_size; i++) {
                disk[i] = -1;
            }
            
            // Place at new position
            for (int i = 0; i < file_size; i++) {
                disk[best_pos + i] = file_id;
            }
        }
    }
}

long part2(InputData* data) {
    // Create a copy of the disk
    int* disk = malloc(data->size * sizeof(int));
    memcpy(disk, data->disk, data->size * sizeof(int));
    
    // Compact the disk (whole files)
    compact_disk_part2(disk, data->size, data->input, data->input_len);
    
    // Calculate checksum
    long checksum = calculate_checksum(disk, data->size);
    
    free(disk);
    return checksum;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day09_test.txt" 
        : "inputs/day09.txt";
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->disk);
    free(data->input);
    free(data);
    
    return 0;
}