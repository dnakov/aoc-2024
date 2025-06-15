# Day 7: Bridge Repair

## Problem Understanding

Engineers need to determine which equations can be made true by inserting operators between numbers. Evaluation is strictly left-to-right (no precedence rules).

### Part 1: Basic Operators
Using only addition (+) and multiplication (*).

### Part 2: Concatenation Operator
Adding concatenation (||) which joins numbers: 12 || 34 = 1234.

## Approach

### Recursive Evaluation
1. Try each operator at each position
2. Evaluate left-to-right as we go
3. Prune branches that exceed the target
4. Sum test values of solvable equations

### Operator Implementation
- Addition: a + b
- Multiplication: a * b  
- Concatenation: a * 10^(digits in b) + b

## Implementation Details

- Recursive backtracking to try all operator combinations
- Early termination when intermediate result exceeds target
- Efficient concatenation using logarithms to count digits

## Complexity Analysis

- **Part 1**: O(n×2^m) where n=equations, m=operators per equation
- **Part 2**: O(n×3^m) with three operator choices

## Key Insights

1. Left-to-right evaluation simplifies the problem
2. Early pruning is crucial for performance
3. Concatenation can be computed mathematically without string conversion