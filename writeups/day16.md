# Day 16: Reindeer Maze

## Problem Overview
We need to guide a reindeer through a maze from start (S) to end (E) with the following rules:
- Moving forward one tile: costs 1 point
- Rotating 90 degrees (clockwise or counter-clockwise): costs 1000 points
- Reindeer starts facing East
- Cannot move through walls (#)

Part 1: Find the minimum cost to reach the end
Part 2: Count all tiles that are part of any optimal path

## Approach

### Part 1: Minimum Cost Path
This is a weighted shortest path problem where the cost depends on both movement and direction changes. I used Dijkstra's algorithm with a 3D state space:
- State: (row, column, direction)
- Each state can transition to:
  - Move forward in current direction (cost +1)
  - Rotate clockwise (cost +1000)
  - Rotate counter-clockwise (cost +1000)

The key insight is that direction matters - reaching a position facing different directions has different costs.

### Part 2: Counting Optimal Path Tiles
After finding the minimum cost, we need to find all tiles that are part of any path achieving this cost:
1. Run Dijkstra to find costs to all states
2. Find the minimum cost at the end position (any direction)
3. Backtrack from all end states with minimum cost
4. Mark all tiles visited during backtracking

The backtracking considers all possible previous states that could lead to the current state with the correct cost difference.

## Implementation Details
- Used fixed-size arrays for simplicity given the grid size constraints
- Directions encoded as enum (0=North, 1=East, 2=South, 3=West)
- Simple priority queue implementation using linear search (sufficient for this problem size)
- Careful handling of rotation directions and their reverse operations during backtracking

## Time Complexity
- Part 1: O(N²D log(N²D)) where N is grid size and D=4 directions
- Part 2: Same as Part 1 plus O(N²D) for backtracking
- Space: O(N²D) for the cost and visited arrays

## Key Insights
1. The problem is essentially finding shortest paths in a graph where nodes are (position, direction) tuples
2. The high rotation cost (1000) means optimal paths minimize turns
3. Multiple paths can have the same optimal cost, requiring careful tracking in Part 2
4. Backtracking needs to consider all three ways to reach a state: forward movement and both rotations