/*
 * Advent of Code 2024 - Day 23: LAN Party
 * 
 * This problem involves finding cliques in a graph of computer connections.
 * Part 1: Find all triangles (3-cliques) containing at least one computer starting with 't'
 * Part 2: Find the largest clique (maximum complete subgraph)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define AOC_IMPLEMENTATION
#include "aoc.h"

#define MAX_NODES 1000
#define MAX_EDGES 5000
#define MAX_NAME_LEN 3
#define MAX_CLIQUE_SIZE 20

typedef struct {
    char name[MAX_NAME_LEN];
    int id;
} Node;

typedef struct {
    Node nodes[MAX_NODES];
    int num_nodes;
    bool adj[MAX_NODES][MAX_NODES];  // Adjacency matrix
    int node_count;
} Graph;

typedef struct {
    int nodes[MAX_CLIQUE_SIZE];
    int size;
} Clique;

// Find or create a node
int get_node_id(Graph* g, const char* name) {
    for (int i = 0; i < g->num_nodes; i++) {
        if (strcmp(g->nodes[i].name, name) == 0) {
            return i;
        }
    }
    
    // Create new node
    if (g->num_nodes < MAX_NODES) {
        strcpy(g->nodes[g->num_nodes].name, name);
        g->nodes[g->num_nodes].id = g->num_nodes;
        return g->num_nodes++;
    }
    
    return -1;
}

// Add edge to graph
void add_edge(Graph* g, const char* a, const char* b) {
    int id_a = get_node_id(g, a);
    int id_b = get_node_id(g, b);
    
    if (id_a >= 0 && id_b >= 0) {
        g->adj[id_a][id_b] = true;
        g->adj[id_b][id_a] = true;
    }
}

// Check if a set of nodes forms a clique
bool is_clique(Graph* g, int* nodes, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (!g->adj[nodes[i]][nodes[j]]) {
                return false;
            }
        }
    }
    return true;
}

// Compare function for sorting node names
int compare_nodes(const void* a, const void* b) {
    return strcmp(((Node*)a)->name, ((Node*)b)->name);
}

// Bron-Kerbosch algorithm for finding maximum clique
void bron_kerbosch(Graph* g, int* R, int R_size, int* P, int P_size, 
                   int* X, int X_size, Clique* max_clique) {
    if (P_size == 0 && X_size == 0) {
        // Found a maximal clique
        if (R_size > max_clique->size) {
            max_clique->size = R_size;
            memcpy(max_clique->nodes, R, R_size * sizeof(int));
        }
        return;
    }
    
    // Make a copy of P for iteration
    int P_copy[MAX_NODES];
    memcpy(P_copy, P, P_size * sizeof(int));
    
    for (int i = 0; i < P_size; i++) {
        int v = P_copy[i];
        
        // R ∪ {v}
        int new_R[MAX_CLIQUE_SIZE];
        memcpy(new_R, R, R_size * sizeof(int));
        new_R[R_size] = v;
        
        // P ∩ N(v)
        int new_P[MAX_NODES];
        int new_P_size = 0;
        for (int j = 0; j < P_size; j++) {
            if (g->adj[v][P[j]]) {
                new_P[new_P_size++] = P[j];
            }
        }
        
        // X ∩ N(v)
        int new_X[MAX_NODES];
        int new_X_size = 0;
        for (int j = 0; j < X_size; j++) {
            if (g->adj[v][X[j]]) {
                new_X[new_X_size++] = X[j];
            }
        }
        
        bron_kerbosch(g, new_R, R_size + 1, new_P, new_P_size, 
                     new_X, new_X_size, max_clique);
        
        // Remove v from P
        for (int j = 0; j < P_size; j++) {
            if (P[j] == v) {
                for (int k = j; k < P_size - 1; k++) {
                    P[k] = P[k + 1];
                }
                P_size--;
                break;
            }
        }
        
        // Add v to X
        X[X_size++] = v;
    }
}

Graph* parse_input(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    Graph* g = calloc(1, sizeof(Graph));
    if (!g) {
        fclose(file);
        return NULL;
    }
    
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char a[MAX_NAME_LEN], b[MAX_NAME_LEN];
        if (sscanf(line, "%2s-%2s", a, b) == 2) {
            add_edge(g, a, b);
        }
    }
    
    fclose(file);
    return g;
}

long part1(Graph* g) {
    int count = 0;
    
    // Find all triangles
    for (int i = 0; i < g->num_nodes; i++) {
        for (int j = i + 1; j < g->num_nodes; j++) {
            if (!g->adj[i][j]) continue;
            
            for (int k = j + 1; k < g->num_nodes; k++) {
                if (g->adj[i][k] && g->adj[j][k]) {
                    // Found a triangle
                    // Check if any node starts with 't'
                    if (g->nodes[i].name[0] == 't' || 
                        g->nodes[j].name[0] == 't' || 
                        g->nodes[k].name[0] == 't') {
                        count++;
                    }
                }
            }
        }
    }
    
    return count;
}

long part2(Graph* g) {
    // Find maximum clique using Bron-Kerbosch
    int R[MAX_CLIQUE_SIZE] = {0};
    int P[MAX_NODES];
    int X[MAX_NODES] = {0};
    
    // Initialize P with all vertices
    for (int i = 0; i < g->num_nodes; i++) {
        P[i] = i;
    }
    
    Clique max_clique = {0};
    bron_kerbosch(g, R, 0, P, g->num_nodes, X, 0, &max_clique);
    
    // Sort nodes in the maximum clique by name
    Node sorted_nodes[MAX_CLIQUE_SIZE];
    for (int i = 0; i < max_clique.size; i++) {
        sorted_nodes[i] = g->nodes[max_clique.nodes[i]];
    }
    qsort(sorted_nodes, max_clique.size, sizeof(Node), compare_nodes);
    
    // Print the password (comma-separated sorted names)
    printf("Part 2: ");
    for (int i = 0; i < max_clique.size; i++) {
        printf("%s", sorted_nodes[i].name);
        if (i < max_clique.size - 1) {
            printf(",");
        }
    }
    printf("\n");
    
    return max_clique.size;
}

int main(int argc, char* argv[]) {
    const char* input_file = (argc > 1 && strcmp(argv[1], "test") == 0) 
        ? "inputs/day23_test.txt" 
        : "inputs/day23.txt";
    
    Graph* g = parse_input(input_file);
    if (!g) {
        fprintf(stderr, "Failed to parse input\n");
        return 1;
    }
    
    printf("Part 1: %ld\n", part1(g));
    part2(g);  // Part 2 prints directly
    
    free(g);
    
    return 0;
}