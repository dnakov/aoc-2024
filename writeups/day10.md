# Day 10: Hoof It

## Problem Understanding

Find hiking trails on a topographic map. Trails start at height 0, end at height 9, and each step must increase height by exactly 1.

### Part 1: Reachable Peaks
Count unique height-9 positions reachable from each trailhead.

### Part 2: Distinct Trails
Count the number of distinct paths from each trailhead to any height-9 position.

## Approach

### Part 1: Reachability (DFS with Visited Set)
1. From each height-0 position, perform DFS
2. Track visited height-9 positions
3. Sum counts across all trailheads

### Part 2: Path Counting (DFS without Visited Tracking)
1. Count all valid paths recursively
2. Each branch represents a unique trail
3. Sum path counts across all trailheads

## Implementation Details

- 4-directional movement (up, down, left, right)
- Boundary checking for grid edges
- Height validation (must increase by exactly 1)

## Complexity Analysis

- **Part 1**: O(t×n×m) where t=trailheads, n,m=grid dimensions
- **Part 2**: O(t×p) where p=paths per trailhead (potentially exponential)

## Key Insights

1. Part 1 asks for destinations, Part 2 asks for journeys
2. The constraint of increasing by exactly 1 limits path branching
3. No need for complex pathfinding - simple DFS suffices