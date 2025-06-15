#ifndef AOC_H
#define AOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

/* File utilities */
char* read_file(const char* filename);
char** read_lines(const char* filename, int* line_count);
void free_lines(char** lines, int line_count);

/* String parsing utilities */
int* extract_ints(const char* str, int* count);
long* extract_longs(const char* str, int* count);
char** split_string(const char* str, const char* delim, int* count);
void free_split(char** parts, int count);

/* Dynamic array */
typedef struct {
    void** items;
    size_t size;
    size_t capacity;
    size_t item_size;
} DynArray;

DynArray* dyn_array_create(size_t item_size);
void dyn_array_push(DynArray* arr, void* item);
void* dyn_array_get(DynArray* arr, size_t index);
void dyn_array_free(DynArray* arr);

/* Hash map */
typedef struct HashNode {
    char* key;
    void* value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** buckets;
    size_t size;
    size_t capacity;
} HashMap;

HashMap* hashmap_create(size_t capacity);
void hashmap_put(HashMap* map, const char* key, void* value);
void* hashmap_get(HashMap* map, const char* key);
bool hashmap_contains(HashMap* map, const char* key);
void hashmap_free(HashMap* map);

/* Grid utilities */
typedef struct {
    int row;
    int col;
} Point;

typedef struct {
    char** grid;
    int rows;
    int cols;
} Grid;

Grid* grid_create(int rows, int cols);
Grid* grid_from_lines(char** lines, int line_count);
void grid_free(Grid* grid);
bool grid_in_bounds(Grid* grid, int row, int col);
Point* grid_neighbors(Grid* grid, int row, int col, bool diagonal, int* count);

/* Math utilities */
long gcd(long a, long b);
long lcm(long a, long b);
int min(int a, int b);
int max(int a, int b);
long minl(long a, long b);
long maxl(long a, long b);

/* Queue for BFS */
typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* rear;
    size_t item_size;
} Queue;

Queue* queue_create(size_t item_size);
void queue_enqueue(Queue* q, void* data);
void* queue_dequeue(Queue* q);
bool queue_is_empty(Queue* q);
void queue_free(Queue* q);

/* Implementation of utility functions */
#ifdef AOC_IMPLEMENTATION

char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = malloc(size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    
    return content;
}

char** read_lines(const char* filename, int* line_count) {
    char* content = read_file(filename);
    if (!content) return NULL;
    
    *line_count = 0;
    for (char* p = content; *p; p++) {
        if (*p == '\n') (*line_count)++;
    }
    if (content[strlen(content) - 1] != '\n') (*line_count)++;
    
    char** lines = malloc(*line_count * sizeof(char*));
    if (!lines) {
        free(content);
        return NULL;
    }
    
    int i = 0;
    char* line = strtok(content, "\n");
    while (line && i < *line_count) {
        lines[i] = strdup(line);
        line = strtok(NULL, "\n");
        i++;
    }
    
    free(content);
    return lines;
}

void free_lines(char** lines, int line_count) {
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
}

int* extract_ints(const char* str, int* count) {
    DynArray* arr = dyn_array_create(sizeof(int));
    const char* p = str;
    
    while (*p) {
        if (isdigit(*p) || (*p == '-' && isdigit(*(p + 1)))) {
            int num = strtol(p, (char**)&p, 10);
            dyn_array_push(arr, &num);
        } else {
            p++;
        }
    }
    
    *count = arr->size;
    int* result = malloc(*count * sizeof(int));
    for (size_t i = 0; i < arr->size; i++) {
        result[i] = *(int*)dyn_array_get(arr, i);
    }
    
    dyn_array_free(arr);
    return result;
}

DynArray* dyn_array_create(size_t item_size) {
    DynArray* arr = malloc(sizeof(DynArray));
    arr->items = malloc(8 * sizeof(void*));
    arr->size = 0;
    arr->capacity = 8;
    arr->item_size = item_size;
    return arr;
}

void dyn_array_push(DynArray* arr, void* item) {
    if (arr->size >= arr->capacity) {
        arr->capacity *= 2;
        arr->items = realloc(arr->items, arr->capacity * sizeof(void*));
    }
    arr->items[arr->size] = malloc(arr->item_size);
    memcpy(arr->items[arr->size], item, arr->item_size);
    arr->size++;
}

void* dyn_array_get(DynArray* arr, size_t index) {
    if (index >= arr->size) return NULL;
    return arr->items[index];
}

void dyn_array_free(DynArray* arr) {
    for (size_t i = 0; i < arr->size; i++) {
        free(arr->items[i]);
    }
    free(arr->items);
    free(arr);
}

int min(int a, int b) { return a < b ? a : b; }
int max(int a, int b) { return a > b ? a : b; }
long minl(long a, long b) { return a < b ? a : b; }
long maxl(long a, long b) { return a > b ? a : b; }

long gcd(long a, long b) {
    while (b != 0) {
        long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long lcm(long a, long b) {
    return (a / gcd(a, b)) * b;
}

#endif /* AOC_IMPLEMENTATION */

#endif /* AOC_H */