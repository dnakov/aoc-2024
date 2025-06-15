# Day 8: Resonant Collinearity

## Problem Understanding

Antennas create antinodes at specific positions. Each pair of antennas with the same frequency creates antinodes based on their relative positions.

### Part 1: Basic Antinodes
For each antenna pair, antinodes appear at positions where one antenna is twice as far as the other.

### Part 2: Resonant Harmonics
Antinodes appear at all grid positions collinear with any antenna pair.

## Approach

### Part 1: Fixed Positions
1. Group antennas by frequency
2. For each pair, calculate the two antinode positions
3. Count unique positions within bounds

### Part 2: Line Extension
1. For each antenna pair, find all collinear points
2. Use vector math to extend the line in both directions
3. Include antenna positions as antinodes

## Implementation Details

- Vector arithmetic for position calculations
- GCD for reducing direction vectors
- Set data structure for unique position counting

## Complexity Analysis

- **Part 1**: O(f×n²) where f=frequencies, n=antennas per frequency
- **Part 2**: O(f×n²×g) where g=grid positions on each line

## Key Insights

1. Antinode positions follow from vector relationships
2. Part 2 requires finding all integer points on a line
3. GCD reduction ensures we don't miss collinear points