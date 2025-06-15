# Day 14: Restroom Redoubt

## Problem Understanding

Robots move in straight lines on a grid with wrapping (toroidal topology). Each robot has a position and velocity.

### Part 1: Safety Factor
After 100 seconds, count robots in each quadrant and multiply the counts.

### Part 2: Easter Egg
Find when the robots form a Christmas tree pattern.

## Approach

### Part 1: Simulation
1. Simulate each robot's movement for 100 seconds
2. Use modulo arithmetic for wrapping: `(pos + vel × time) % size`
3. Count robots in each quadrant (excluding middle lines)
4. Calculate safety factor as product of quadrant counts

### Part 2: Pattern Detection
1. Simulate up to 10,000 seconds
2. Detect Christmas tree by looking for dense horizontal lines
3. Pattern found when multiple long horizontal lines appear
4. The tree appears at second 7492

## Implementation Details

- Grid size: 101×103
- Efficient position updates using modulo arithmetic
- Pattern detection using consecutive robot counting
- Visual verification by printing the grid

## Complexity Analysis

- **Part 1**: O(n) where n = number of robots
- **Part 2**: O(n × t) where t = time steps to find pattern

## Key Insights

1. Wrapping is handled elegantly with modulo arithmetic
2. The Christmas tree is recognizable by its dense horizontal lines
3. Different grid sizes would have different pattern times
4. The pattern is an Easter egg - requires visual recognition