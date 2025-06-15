# Day 25: Code Chronicle

## Problem Understanding

The final day involves a simple pattern matching problem with locks and keys:
- Locks have pins extending downward (filled top row)
- Keys have pins extending upward (filled bottom row)
- Each schematic is a 7×5 grid showing column heights
- A lock and key fit if their combined heights don't exceed the available space

## Approach

### Part 1: Lock/Key Compatibility
1. Parse each schematic and determine if it's a lock or key
2. Convert to column heights (excluding the filled row)
3. Check all lock/key pairs for compatibility
4. Count pairs where no column sum exceeds 5

### Part 2: Free Star!
As is tradition, Day 25 Part 2 is a gift - you get the 50th star automatically after solving Part 1.

## Key Insights

1. **Simple Classification**: Top row determines lock vs key
2. **Height Calculation**: Count filled cells per column, subtract 1
3. **Compatibility Check**: Sum of heights must not exceed 5 (the space between top and bottom)
4. **Brute Force Works**: With ~500 schematics, O(n²) checking is fine

## Implementation Details

- Store schematics as arrays of column heights
- Use a boolean flag to distinguish locks from keys
- Simple nested loops to check all lock/key combinations
- Direct height comparison for compatibility

## Complexity

- Time: O(n²) where n is the number of schematics
- Space: O(n) to store all schematics
- Each compatibility check is O(1) with only 5 columns

A fitting end to Advent of Code 2024 - a straightforward problem that brings the challenge to a festive close!