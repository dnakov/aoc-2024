# Day 5: Print Queue

## Problem Understanding

Safety manual pages must be printed in a specific order based on ordering rules. Each rule specifies that one page must come before another.

### Part 1: Valid Updates
Find updates that already satisfy all ordering rules and sum their middle page numbers.

### Part 2: Fixed Updates
Fix updates that violate ordering rules, then sum their middle page numbers.

## Approach

### Part 1: Rule Validation
1. Parse ordering rules into a precedence structure
2. For each update, verify all applicable rules are satisfied
3. Sum middle elements of valid updates

### Part 2: Topological Sorting
1. Identify invalid updates
2. Build a dependency graph from applicable rules
3. Use topological sort to find correct ordering
4. Sum middle elements of corrected updates

## Implementation Details

- Used adjacency lists to represent ordering constraints
- Implemented Kahn's algorithm for topological sorting
- Efficient rule checking using hash sets for lookups

## Complexity Analysis

- **Part 1**: O(n×m×r) where n=updates, m=pages per update, r=rules
- **Part 2**: O(n×(m²+r)) for building graphs and sorting

## Key Insights

1. Not all rules apply to each update - only those involving pages in the update
2. Topological sorting guarantees a valid ordering exists
3. The middle element is well-defined since updates have odd length