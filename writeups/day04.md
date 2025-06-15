# Day 4: Ceres Search

## Problem Understanding

A word search puzzle where we need to find patterns in a grid of letters.

### Part 1: Finding XMAS
Search for all occurrences of "XMAS" in the grid. The word can appear:
- In any of 8 directions (horizontal, vertical, diagonal)
- Forward or backward (XMAS or SAMX)

### Part 2: X-MAS Pattern
Find X-shaped patterns where two "MAS" strings cross at their middle 'A':
```
M.S
.A.
M.S
```
Both diagonals must spell "MAS" (forward or backward).

## Approach

### Part 1: Directional Search
1. For each cell in the grid, try starting "XMAS" in all 8 directions
2. Check bounds and match characters for each direction
3. Count all valid occurrences

### Part 2: Pattern Matching
1. Find all 'A' characters (potential centers)
2. Check if the four corners form valid "MAS" patterns on both diagonals
3. Valid patterns: MAS-MAS, MAS-SAM, SAM-MAS, SAM-SAM

## Implementation Details

- Stored grid as 2D character array
- Used direction vectors for systematic searching
- Boundary checking to prevent out-of-bounds access

## Complexity Analysis

- **Part 1**: O(n×m×8×4) = O(n×m) where n,m are grid dimensions
- **Part 2**: O(n×m) scanning for 'A' centers

## Key Insights

1. Part 1 requires systematic enumeration of all starting positions and directions
2. Part 2's X pattern has exactly 4 valid configurations
3. Efficient boundary checking prevents unnecessary comparisons