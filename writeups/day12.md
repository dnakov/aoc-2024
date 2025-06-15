# Day 12: Garden Groups

## Problem Understanding

Garden plots are grouped into regions by plant type. We need to calculate fencing costs.

### Part 1: Perimeter-based Pricing
Cost = Area × Perimeter for each region.

### Part 2: Side-based Pricing (Bulk Discount)
Cost = Area × Number of Sides for each region.

## Approach

### Part 1: Area and Perimeter
1. Use DFS to identify connected regions
2. Count cells (area) and exposed edges (perimeter)
3. An edge is exposed if it borders a different region or the grid boundary

### Part 2: Counting Sides
Key insight: Number of sides = Number of corners in a rectilinear polygon

1. Assign unique IDs to each region
2. For each region, count corners by examining 2×2 patterns:
   - 1 cell of region = 1 outer corner
   - 2 diagonal cells = 2 corners
   - 3 cells = 1 inner corner
   - 0 or 4 cells = 0 corners

## Implementation Details

- Region identification using DFS with visited tracking
- Corner counting by checking all possible 2×2 grid positions
- Careful boundary handling when checking patterns

## Complexity Analysis

- **Time**: O(n×m) for region finding + O(n×m) for corner counting
- **Space**: O(n×m) for region ID storage

## Key Insights

1. The number of sides equals the number of corners in rectilinear shapes
2. Each 2×2 pattern contributes a specific number of corners
3. This approach works for any shape, including those with holes