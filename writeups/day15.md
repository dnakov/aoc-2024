# Day 15: Warehouse Woes

## Problem Understanding

A robot pushes boxes in a warehouse following movement commands. Walls block movement.

### Part 1: Standard Boxes
Single-cell boxes `O` that can be pushed in chains.

### Part 2: Wide Boxes
Two-cell boxes `[]` that require coordinated movement.

## Approach

### Part 1: Simple Pushing
1. For each move, check if destination is free
2. If box present, recursively check if it can be pushed
3. Move all affected boxes in sequence

### Part 2: Wide Box Mechanics
1. Transform warehouse: each cell becomes two cells wide
2. Wide boxes represented as `[]` pairs
3. Movement rules:
   - Horizontal: Push entire box, check both cells
   - Vertical: Complex - boxes can overlap and push multiple boxes
4. Use BFS to find all affected boxes before moving

## Implementation Details

- Warehouse transformation for Part 2 doubles width
- Careful tracking of box pairs (left/right brackets)
- Multi-box collision detection for vertical moves
- GPS calculation uses left bracket position

## Complexity Analysis

- **Part 1**: O(m × n) where m = moves, n = maximum chain length
- **Part 2**: O(m × b) where b = boxes affected per move

## Key Insights

1. Part 2's wide boxes create complex push chains
2. Vertical movement of wide boxes is the main challenge
3. BFS ensures we find all affected boxes before moving
4. The warehouse doubling transformation simplifies parsing