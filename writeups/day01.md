# Day 1: Historian Hysteria

## Problem Understanding

The problem involves reconciling two lists of location IDs. We need to:
- Part 1: Calculate the total distance between the lists by pairing sorted elements
- Part 2: Calculate a similarity score based on occurrence counts

## Approach

### Part 1: Total Distance
1. Parse the input to extract two lists of integers
2. Sort both lists independently
3. Pair elements by index (smallest with smallest, etc.)
4. Sum the absolute differences between paired elements

### Part 2: Similarity Score
1. For each number in the left list
2. Count how many times it appears in the right list
3. Multiply the number by its count
4. Sum all these products

## Implementation Details

The solution uses:
- Dynamic arrays to handle variable input sizes
- `qsort()` for efficient sorting in Part 1
- Simple nested loops for counting in Part 2 (O(n²) complexity)

## Complexity Analysis

- **Part 1**: O(n log n) due to sorting
- **Part 2**: O(n²) due to nested loops for counting
  - Could be optimized to O(n) using a hash map for counts

## Key Insights

1. Part 1 requires independent sorting of both lists
2. Part 2 doesn't require sorting - it's about frequency counting
3. The similarity score rewards numbers that appear frequently in both lists