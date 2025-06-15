# Day 22: Monkey Market

## Problem Understanding

This problem involves simulating a pseudorandom number generator with specific transformation rules:
1. Multiply by 64, XOR with itself, modulo 16777216
2. Divide by 32, XOR with itself, modulo 16777216  
3. Multiply by 2048, XOR with itself, modulo 16777216

Part 1 asks for the sum of the 2000th secret number for each buyer.

Part 2 involves finding patterns in price changes (last digit of each secret) to maximize profits. We need to find the best sequence of 4 consecutive price changes that yields the maximum total across all buyers.

## Approach

### Part 1: Secret Number Generation
- Implement the three-step transformation algorithm
- Iterate 2000 times for each buyer
- Sum the final secret numbers

### Part 2: Price Change Patterns
- Generate all 2001 secret numbers for each buyer
- Extract prices (last digit of each secret)
- Calculate price changes between consecutive prices
- Find all unique sequences of 4 consecutive changes
- For each sequence, sum the prices obtained when first encountering that sequence for each buyer
- Return the maximum sum

## Key Insights

1. **Efficient PRNG**: The transformation rules create a deterministic pseudorandom sequence
2. **Price Patterns**: The last digit creates a bounded sequence (0-9) with patterns
3. **First Occurrence Only**: Each buyer only contributes once per unique 4-change sequence
4. **Hash Table**: Use a hash table to efficiently track sequences and their values

## Implementation Details

- Used a hash table with linear probing for sequence storage
- Tracked which buyers have already contributed to each sequence
- Price changes range from -9 to +9, stored as int8_t
- Sequence comparison done element by element

## Complexity

- Time: O(B × I) for generation, O(B × I × S) for pattern matching
  where B = buyers, I = iterations, S = sequence operations
- Space: O(S) for storing unique sequences, where S ≈ 19^4 in worst case

The challenge combines number theory (PRNG simulation) with pattern recognition (finding optimal trading sequences).