# Day 3: Mull It Over

## Problem Understanding

The elves' computer memory is corrupted, containing garbled text with valid multiplication instructions scattered throughout. We need to extract and execute these instructions.

### Part 1: Basic Pattern Matching
Find all instances of `mul(X,Y)` where X and Y are 1-3 digit numbers, then sum all the multiplication results.

### Part 2: Conditional Instructions
The memory also contains:
- `do()` - enables subsequent multiplications
- `don't()` - disables subsequent multiplications
- Multiplications start enabled by default

## Approach

### Part 1: Regular Expression Matching
1. Use regex pattern: `mul\(([0-9]{1,3}),([0-9]{1,3})\)`
2. Extract all matches and calculate products
3. Sum all results

### Part 2: Stateful Processing
1. Extended regex to also match `do()` and `don't()`
2. Process matches sequentially, maintaining enabled/disabled state
3. Only count multiplications when enabled

## Implementation Details

- Used POSIX regex library (regex.h) for pattern matching
- Single pass through input for both parts
- Careful handling of regex capture groups

## Complexity Analysis

- **Time**: O(n) where n is the input length
- **Space**: O(m) where m is the number of matches

## Key Insights

1. The order of instructions matters in Part 2
2. Regular expressions are perfect for this type of pattern extraction
3. State management is crucial for conditional processing