# Day 19: Linen Layout

## Problem Overview
We need to arrange towels with colored stripes to match desired patterns:
- Given a set of available towel patterns (e.g., "r", "wr", "bwu")
- Check which desired designs can be created by combining towels
- Towels cannot be reversed or flipped

Part 1: Count how many designs are possible
Part 2: Count total number of ways to make all possible designs

## Approach

### Dynamic Programming Solution
Both parts use dynamic programming with different objectives:

**Part 1**: Check if design is possible
- `dp[i]` = true if first i characters can be formed
- For each position, check if any pattern ending there matches

**Part 2**: Count number of ways
- `dp[i]` = number of ways to form first i characters
- Sum contributions from all patterns that match at each position

### Algorithm Steps
1. Initialize dp[0] = true (Part 1) or 1 (Part 2)
2. For each position i in the design:
   - For each available pattern:
     - If pattern fits and matches at position i
     - Update dp[i] based on dp[i - pattern_length]
3. Return dp[design_length]

## Implementation Details
- Used string comparison (strncmp) to check pattern matches
- Separate functions for possibility check vs counting ways
- Careful handling of string lengths and array bounds
- Increased buffer size for long input lines (>2KB)

## Time Complexity
- Part 1: O(n × m × p) where n = design length, m = number of patterns, p = pattern length
- Part 2: Same complexity but tracks counts instead of boolean
- Space: O(n) for the DP array

## Key Insights
1. Classic DP pattern: "can we build string X from pieces Y?"
2. Part 1 asks for existence, Part 2 asks for count - same DP structure
3. The order of checking patterns doesn't matter for correctness
4. Early termination in Part 1 (break on first match) provides minor optimization