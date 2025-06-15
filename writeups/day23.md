# Day 23: LAN Party

## Problem Understanding

This problem involves graph theory, specifically finding cliques (complete subgraphs) in a network of computer connections.

Part 1: Find all triangles (3-cliques) in the graph where at least one computer name starts with 't'.

Part 2: Find the maximum clique (largest complete subgraph) and output the computer names in alphabetical order.

## Approach

### Part 1: Triangle Detection
- Build an adjacency matrix representation of the graph
- Use triple nested loops to find all triangles (sets of 3 mutually connected nodes)
- Count triangles containing at least one node starting with 't'

### Part 2: Maximum Clique Problem
- Implement the Bron-Kerbosch algorithm, a classic algorithm for finding all maximal cliques
- Track the largest clique found
- Sort the nodes in the maximum clique alphabetically for the password

## Key Insights

1. **Graph Representation**: An adjacency matrix makes checking connections O(1)
2. **Triangle Finding**: Simple O(n³) algorithm is sufficient for Part 1
3. **Bron-Kerbosch Algorithm**: Efficient backtracking algorithm for the NP-complete maximum clique problem
4. **String Handling**: Computer names are exactly 2 characters, simplifying parsing

## Implementation Details

- Used a hash-like approach to map computer names to node IDs
- Implemented the basic Bron-Kerbosch algorithm without pivoting
- Maintained three sets: R (current clique), P (candidates), X (already processed)
- Used array-based set operations for efficiency

## Complexity

- Part 1: O(n³) where n is the number of nodes
- Part 2: O(3^(n/3)) worst case for Bron-Kerbosch, but much better in practice
- Space: O(n²) for the adjacency matrix

The maximum clique problem is NP-complete, but the Bron-Kerbosch algorithm performs well on sparse graphs like computer networks.