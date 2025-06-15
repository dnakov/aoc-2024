# Day 11: Plutonian Pebbles

## Problem Understanding

Magical stones transform when you blink according to specific rules:
1. Stone engraved with 0 → becomes stone with 1
2. Stone with even number of digits → splits into two stones (left half, right half)
3. Otherwise → multiply the number by 2024

### Part 1: 25 Blinks
Count the total number of stones after 25 blinks.

### Part 2: 75 Blinks
Count the total number of stones after 75 blinks - requiring optimization due to exponential growth.

## Approach

### Part 1: Direct Simulation
- Maintain array of stones
- Apply transformation rules for each blink
- Track the growing array of stones

### Part 2: Memoization
- Key insight: Many stones repeat the same transformations
- Use recursive counting with memoization
- Cache (stone, blinks_remaining) → stone_count
- Avoid storing actual stones, just count them

## Implementation Details

- Hash table with chaining for memoization cache
- Efficient digit splitting using powers of 10
- Cache size of 100,000 entries handles all unique cases

## Complexity Analysis

- **Part 1**: O(2^25) worst case, manageable with direct simulation
- **Part 2**: O(U × B) where U = unique (stone, blinks) pairs
- Space: O(U) for memoization cache

## Key Insights

1. Direct simulation becomes infeasible around 40-50 blinks
2. Many stones converge to the same values through transformations
3. Memoization dramatically reduces redundant calculations
4. The growth rate is approximately 1.5-2x per blink on average