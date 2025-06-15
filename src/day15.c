/*
 * Advent of Code 2024 - Day 15 - Correct implementation
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_SIZE 100

typedef struct {
    char grid[MAX_SIZE][MAX_SIZE];
    int rows;
    int cols;
    int robot_r;
    int robot_c;
    char* moves;
    int move_count;
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
    
    data->rows = 0;
    data->cols = 0;
    data->move_count = 0;
    data->moves = malloc(20000);
    
    char line[1024];
    int parsing_grid = 1;
    
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
            len--;
        }
        
        if (len == 0) {
            parsing_grid = 0;
            continue;
        }
        
        if (parsing_grid) {
            strcpy(data->grid[data->rows], line);
            if (data->cols == 0) data->cols = len;
            
            // Find robot position
            for (int c = 0; c < len; c++) {
                if (line[c] == '@') {
                    data->robot_r = data->rows;
                    data->robot_c = c;
                    data->grid[data->rows][c] = '.'; // Replace robot with empty space
                }
            }
            data->rows++;
        } else {
            // Parse moves
            for (int i = 0; i < len; i++) {
                data->moves[data->move_count++] = line[i];
            }
        }
    }
    
    fclose(file);
    return data;
}

// Direction mappings
int dr[] = {-1, 1, 0, 0}; // up, down, left, right
int dc[] = {0, 0, -1, 1};

int get_dir(char move) {
    switch(move) {
        case '^': return 0;
        case 'v': return 1;
        case '<': return 2;
        case '>': return 3;
        default: return -1;
    }
}

long part1(InputData* data) {
    // Make a working copy of the grid
    char grid[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < data->rows; i++) {
        strcpy(grid[i], data->grid[i]);
    }
    
    int robot_r = data->robot_r;
    int robot_c = data->robot_c;
    
    // Process each move
    for (int i = 0; i < data->move_count; i++) {
        int dir = get_dir(data->moves[i]);
        if (dir == -1) continue;
        
        int nr = robot_r + dr[dir];
        int nc = robot_c + dc[dir];
        
        // Check bounds
        if (nr < 0 || nr >= data->rows || nc < 0 || nc >= data->cols) {
            continue;
        }
        
        char target = grid[nr][nc];
        
        if (target == '#') {
            // Wall - can't move
            continue;
        } else if (target == '.') {
            // Empty space - move robot
            robot_r = nr;
            robot_c = nc;
        } else if (target == 'O') {
            // Box - try to push chain of boxes
            int push_r = nr;
            int push_c = nc;
            int box_count = 0;
            
            // Count consecutive boxes
            while (push_r >= 0 && push_r < data->rows && 
                   push_c >= 0 && push_c < data->cols && 
                   grid[push_r][push_c] == 'O') {
                box_count++;
                push_r += dr[dir];
                push_c += dc[dir];
            }
            
            // Check if there's space after all boxes
            if (push_r >= 0 && push_r < data->rows && 
                push_c >= 0 && push_c < data->cols && 
                grid[push_r][push_c] == '.') {
                
                // Can push! Move all boxes one step in direction
                // Work backwards to avoid overwriting
                int cur_r = push_r;
                int cur_c = push_c;
                
                for (int j = 0; j < box_count; j++) {
                    int prev_r = cur_r - dr[dir];
                    int prev_c = cur_c - dc[dir];
                    grid[cur_r][cur_c] = 'O';
                    grid[prev_r][prev_c] = '.';
                    cur_r = prev_r;
                    cur_c = prev_c;
                }
                
                // Move robot
                robot_r = nr;
                robot_c = nc;
            }
        }
    }
    
    // Calculate GPS sum
    long gps_sum = 0;
    for (int r = 0; r < data->rows; r++) {
        for (int c = 0; c < data->cols; c++) {
            if (grid[r][c] == 'O') {
                gps_sum += 100 * r + c;
            }
        }
    }
    
    return gps_sum;
}

// Part 2: Wide warehouse
typedef struct {
    char grid[MAX_SIZE][MAX_SIZE * 2];
    int rows;
    int cols;
    int robot_r;
    int robot_c;
} WideWarehouse;

WideWarehouse* create_wide_warehouse(InputData* data) {
    WideWarehouse* wide = malloc(sizeof(WideWarehouse));
    wide->rows = data->rows;
    wide->cols = data->cols * 2;
    
    for (int r = 0; r < data->rows; r++) {
        int c2 = 0;
        for (int c = 0; c < data->cols; c++) {
            char cell = data->grid[r][c];
            if (r == data->robot_r && c == data->robot_c) {
                cell = '.'; // Robot position becomes empty
            }
            
            if (cell == '#') {
                wide->grid[r][c2++] = '#';
                wide->grid[r][c2++] = '#';
            } else if (cell == 'O') {
                wide->grid[r][c2++] = '[';
                wide->grid[r][c2++] = ']';
            } else { // '.' or robot position
                wide->grid[r][c2++] = '.';
                wide->grid[r][c2++] = '.';
            }
        }
        wide->grid[r][wide->cols] = '\0';
    }
    
    // Set robot position in wide warehouse
    wide->robot_r = data->robot_r;
    wide->robot_c = data->robot_c * 2;
    
    return wide;
}

// Check if we can push a wide box (starting with '[') in given direction
int can_push_wide_box(WideWarehouse* wide, int box_r, int box_c, int dir) {
    if (dir == 2 || dir == 3) { // Left or right
        int nr = box_r + dr[dir];
        int nc = (dir == 2) ? box_c - 1 : box_c + 2; // Position to check
        
        if (nc < 0 || nc >= wide->cols || wide->grid[nr][nc] == '#') {
            return 0;
        }
        if (wide->grid[nr][nc] == '.') {
            return 1;
        }
        if (wide->grid[nr][nc] == '[') {
            return can_push_wide_box(wide, nr, nc, dir);
        }
        if (wide->grid[nr][nc] == ']') {
            return can_push_wide_box(wide, nr, nc - 1, dir);
        }
    } else { // Up or down
        int nr = box_r + dr[dir];
        
        // Check both parts of the box
        if (nr < 0 || nr >= wide->rows || 
            wide->grid[nr][box_c] == '#' || wide->grid[nr][box_c + 1] == '#') {
            return 0;
        }
        
        if (wide->grid[nr][box_c] == '.' && wide->grid[nr][box_c + 1] == '.') {
            return 1;
        }
        
        // Check all boxes that would need to be pushed
        int can_push = 1;
        if (wide->grid[nr][box_c] == '[') {
            can_push &= can_push_wide_box(wide, nr, box_c, dir);
        }
        if (wide->grid[nr][box_c] == ']') {
            can_push &= can_push_wide_box(wide, nr, box_c - 1, dir);
        }
        if (wide->grid[nr][box_c + 1] == '[') {
            can_push &= can_push_wide_box(wide, nr, box_c + 1, dir);
        }
        
        return can_push;
    }
    return 0;
}

// Actually push a wide box
void push_wide_box(WideWarehouse* wide, int box_r, int box_c, int dir) {
    if (dir == 2 || dir == 3) { // Left or right
        int nr = box_r + dr[dir];
        int target_c = (dir == 2) ? box_c - 1 : box_c + 2;
        
        // Push any box that's in the way first
        if (wide->grid[nr][target_c] == '[') {
            push_wide_box(wide, nr, target_c, dir);
        } else if (wide->grid[nr][target_c] == ']') {
            push_wide_box(wide, nr, target_c - 1, dir);
        }
        
        // Move this box
        if (dir == 2) { // Left
            wide->grid[nr][box_c - 1] = '[';
            wide->grid[nr][box_c] = ']';
            wide->grid[box_r][box_c + 1] = '.';
        } else { // Right
            wide->grid[nr][box_c + 2] = ']';
            wide->grid[nr][box_c + 1] = '[';
            wide->grid[box_r][box_c] = '.';
        }
    } else { // Up or down
        int nr = box_r + dr[dir];
        
        // Push any boxes that are in the way first
        if (wide->grid[nr][box_c] == '[') {
            push_wide_box(wide, nr, box_c, dir);
        }
        if (wide->grid[nr][box_c] == ']') {
            push_wide_box(wide, nr, box_c - 1, dir);
        }
        if (wide->grid[nr][box_c + 1] == '[') {
            push_wide_box(wide, nr, box_c + 1, dir);
        }
        
        // Move this box
        wide->grid[nr][box_c] = '[';
        wide->grid[nr][box_c + 1] = ']';
        wide->grid[box_r][box_c] = '.';
        wide->grid[box_r][box_c + 1] = '.';
    }
}

long part2(InputData* data) {
    WideWarehouse* wide = create_wide_warehouse(data);
    
    int robot_r = wide->robot_r;
    int robot_c = wide->robot_c;
    
    // Process each move
    for (int i = 0; i < data->move_count; i++) {
        int dir = get_dir(data->moves[i]);
        if (dir == -1) continue;
        
        int nr = robot_r + dr[dir];
        int nc = robot_c + dc[dir];
        
        // Check bounds
        if (nr < 0 || nr >= wide->rows || nc < 0 || nc >= wide->cols) {
            continue;
        }
        
        char target = wide->grid[nr][nc];
        
        if (target == '#') {
            // Wall - can't move
            continue;
        } else if (target == '.') {
            // Empty space - move robot
            robot_r = nr;
            robot_c = nc;
        } else if (target == '[' || target == ']') {
            // Box - try to push it
            int box_c = (target == '[') ? nc : nc - 1;
            
            if (can_push_wide_box(wide, nr, box_c, dir)) {
                push_wide_box(wide, nr, box_c, dir);
                robot_r = nr;
                robot_c = nc;
            }
        }
    }
    
    // Calculate GPS sum (only count '[' positions)
    long gps_sum = 0;
    for (int r = 0; r < wide->rows; r++) {
        for (int c = 0; c < wide->cols; c++) {
            if (wide->grid[r][c] == '[') {
                gps_sum += 100 * r + c;
            }
        }
    }
    
    free(wide);
    return gps_sum;
}

int main(int argc, char* argv[]) {
    const char* input_file;
    if (argc > 1 && strcmp(argv[1], "test") == 0) {
        input_file = "inputs/day15_test.txt";
    } else {
        input_file = "inputs/day15.txt";
    }
    
    InputData* data = parse_input(input_file);
    if (!data) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(data));
    printf("Part 2: %ld\n", part2(data));
    
    // Free allocated memory
    free(data->moves);
    free(data);
    
    return 0;
}