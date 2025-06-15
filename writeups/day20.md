# Day 20: Race Condition

## Problem Overview
Navigate a racetrack maze with a special "cheat" ability:
- Normal movement takes 1 picosecond per step
- Can cheat once per race by passing through walls
- Part 1: Cheats last up to 2 picoseconds
- Part 2: Cheats last up to 20 picoseconds
- Find cheats that save at least 100 picoseconds

## Approach

### Path Analysis
1. Use BFS to find distances from start to all reachable positions
2. Use BFS to find distances from all positions to the end
3. Normal path length = distance from start to end

### Finding Cheats
For each potential cheat:
- Start position (x1, y1) must be on the track
- End position (x2, y2) must be on the track
- Manhattan distance between them ≤ max cheat time
- Path with cheat = dist_to_start[x1,y1] + cheat_dist + dist_from_end[x2,y2]
- Savings = normal_path - path_with_cheat

### Key Insight
A valid cheat must actually provide a shortcut. We verify this by checking:
- The normal distance between positions > cheat distance
- This ensures we're bypassing part of the track, not just following it

## Implementation Details
- BFS computes distances in O(n²) time
- Check all pairs of positions for potential cheats
- Manhattan distance for cheat duration calculation
- Careful to avoid counting the same cheat multiple times

## Time Complexity
- BFS: O(n²) where n is grid size
- Part 1: O(n² × k²) where k is max cheat range (small constant)
- Part 2: O(n⁴) for checking all position pairs
- Space: O(n²) for distance arrays

## Key Insights
1. Pre-computing distances from both ends enables efficient cheat evaluation
2. Manhattan distance perfectly models the cheat mechanics
3. Not all short paths are cheats - must verify we're actually bypassing walls
4. The cheat start/end positions uniquely identify each cheat