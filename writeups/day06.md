# Day 6: Guard Gallivant

## Problem Understanding

A guard patrols a lab following simple rules:
- Move forward until hitting an obstacle (#)
- Turn right 90° when blocked
- Continue until leaving the map

### Part 1: Coverage
Count distinct positions visited during the patrol.

### Part 2: Loop Creation
Find positions where placing a single obstacle causes the guard to loop forever.

## Approach

### Part 1: Path Simulation
1. Track guard position and direction
2. Move forward, turning right at obstacles
3. Mark all visited positions
4. Stop when leaving the map

### Part 2: Loop Detection
1. For each empty position, temporarily place an obstacle
2. Simulate patrol with cycle detection
3. Count positions that create infinite loops

## Implementation Details

- Direction vectors for movement: up, right, down, left
- State tracking: (x, y, direction) for cycle detection
- Optimization: Only test obstacles on the original path

## Complexity Analysis

- **Part 1**: O(n×m) where n,m are grid dimensions
- **Part 2**: O(n×m×k) where k is average path length

## Key Insights

1. Cycles are detected when the guard revisits a position with the same direction
2. Obstacles only matter if they're on the guard's original path
3. The guard always turns right, creating predictable patterns