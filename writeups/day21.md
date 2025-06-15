# Day 21: Keypad Conundrum

## Problem Understanding

This problem involves robots typing codes on keypads through multiple layers of indirection:
- A numeric keypad (like a phone) where the actual codes are typed
- Directional keypads that control the robots
- Multiple robots form a chain, each controlling the next

The key insight is that we need to find the shortest sequence of button presses on the first directional keypad to ultimately type the codes on the numeric keypad.

## Approach

### Part 1: Two Directional Robots
1. **Keypad Modeling**: Represent both numeric and directional keypads as 2D grids
2. **Path Generation**: Generate optimal paths between keys, avoiding gaps (invalid positions)
3. **Dynamic Programming**: Use memoization to cache the cost of moving between keys at each level
4. **Recursive Solution**: Calculate the total cost by recursively determining moves through each robot layer

### Part 2: 25 Directional Robots
The same algorithm works for Part 2, but with 25 directional robots instead of 2. The memoization is crucial here to avoid exponential time complexity.

## Key Insights

1. **Avoiding Gaps**: When generating paths, we must avoid moving through invalid positions (gaps in the keypads)
2. **Optimal Move Ordering**: The order of moves matters - we prioritize certain directions to minimize the total cost
3. **Memoization**: Without caching results, the recursive solution would be too slow for 25 levels

## Implementation Details

- Used a recursive function `min_moves()` that calculates the minimum cost to move from one key to another at a given robot level
- Implemented path generation that avoids gaps by choosing appropriate move orders
- Maintained a global memoization table to cache results across different codes

## Complexity

- Time: O(K × L × N²) where K is the number of codes, L is the number of levels, and N is the number of keys
- Space: O(L × N²) for the memoization table

The problem is essentially about finding optimal paths through multiple layers of indirection, which is elegantly solved using dynamic programming.