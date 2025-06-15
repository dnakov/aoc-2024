# Day 18: RAM Run

## Problem Overview
Navigate through a 2D memory space while avoiding corrupted bytes:
- Grid is 71x71 (0-70 in both dimensions)
- Bytes fall and corrupt specific coordinates
- Find shortest path from (0,0) to (70,70)

Part 1: Find shortest path after first 1024 bytes fall
Part 2: Find which byte first makes the exit unreachable

## Approach

### Part 1: Pathfinding with Obstacles
Standard BFS (Breadth-First Search) implementation:
- Mark corrupted positions in a 2D grid
- Use BFS to find shortest path from start to end
- Track distances to each position
- Return distance to bottom-right corner

### Part 2: Binary Search for Blocking Byte
Key insight: If N bytes block the path, then N+1 bytes also block it.
This monotonic property allows binary search:
1. Binary search on the number of bytes
2. For each midpoint, check if path exists
3. If blocked, search earlier; if not, search later
4. Find the exact byte that first blocks all paths

## Implementation Details
- Used boolean grid to mark corrupted positions
- Queue-based BFS for pathfinding
- Binary search range: 0 to total number of bytes
- Careful indexing: grid[y][x] where y is row, x is column

## Time Complexity
- Part 1: O(V + E) = O(n²) for BFS on n×n grid
- Part 2: O(log(m) × n²) where m is number of bytes
- Space: O(n²) for the grid and distance arrays

## Key Insights
1. BFS guarantees shortest path in unweighted graphs
2. Binary search works because path blockage is monotonic
3. The grid coordinate system requires careful attention to x/y ordering
4. Part 2's "first blocking byte" suggests binary search approach