# Day 13: Claw Contraption

## Problem Understanding

Claw machines with two buttons that move the claw by fixed amounts. Goal: reach the prize position with minimum tokens.

- Button A: Costs 3 tokens
- Button B: Costs 1 token

### Part 1: Limited Presses
Maximum 100 presses per button. Find minimum tokens to win all possible prizes.

### Part 2: Distant Prizes
Prize positions offset by 10,000,000,000,000. No practical limit on button presses.

## Approach

### Mathematical Solution
System of linear equations:
```
ax × a + bx × b = px
ay × a + by × b = py
```

Where:
- (ax, ay) = Button A movement
- (bx, by) = Button B movement
- (px, py) = Prize position
- a, b = Number of button presses

### Cramer's Rule
1. Calculate determinant: D = ax×by - ay×bx
2. If D = 0, no unique solution
3. Otherwise: a = (px×by - py×bx)/D, b = (py×ax - px×ay)/D
4. Check if a and b are non-negative integers

## Implementation Details

- Parse machine configurations from input
- Apply Cramer's rule to find exact solutions
- Verify integer solutions (no fractional button presses)
- Part 2: Add 10^13 offset to prize coordinates

## Complexity Analysis

- **Time**: O(n) where n = number of machines
- **Space**: O(1) per machine

## Key Insights

1. This is a linear Diophantine equation problem
2. Cramer's rule provides exact solutions efficiently
3. The large offset in Part 2 doesn't affect the algorithm
4. Integer solution check is crucial - fractional presses are impossible