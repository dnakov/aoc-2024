# Day 17: Chronospatial Computer

## Problem Overview
We need to simulate a 3-bit computer with:
- Three registers (A, B, C) that can hold any integer
- 8 instructions identified by 3-bit opcodes (0-7)
- Programs consisting of 3-bit numbers

Part 1: Run the program and output the results
Part 2: Find the lowest value of register A that makes the program output itself (a quine)

## Instruction Set
- `adv` (0): A = A / (2^combo)
- `bxl` (1): B = B XOR literal
- `bst` (2): B = combo % 8
- `jnz` (3): Jump to literal if A ≠ 0
- `bxc` (4): B = B XOR C
- `out` (5): Output combo % 8
- `bdv` (6): B = A / (2^combo)
- `cdv` (7): C = A / (2^combo)

Combo operands: 0-3 are literals, 4=A, 5=B, 6=C

## Approach

### Part 1: Simulation
Straightforward implementation of the instruction set:
- Parse instructions and operands
- Execute each instruction according to its semantics
- Handle jumps by updating the instruction pointer
- Collect outputs and join with commas

### Part 2: Finding the Quine
The key insight is that the program processes A by repeatedly:
1. Using the lowest 3 bits of A for computation
2. Dividing A by 8 (shifting right by 3 bits)

This means we can build A backwards:
- Start from the last output digit
- For each position, try all 8 possible 3-bit values
- Check if the program produces the correct suffix
- Recursively build the rest of A

The recursive approach works because each output digit depends primarily on a specific group of 3 bits in A.

## Implementation Details
- Used recursive backtracking to build A from right to left
- Each recursive call handles one output position
- Try all 8 possible values for the current 3 bits
- Verify by running the program and checking the output suffix

## Time Complexity
- Part 1: O(n) where n is the number of instructions executed
- Part 2: O(8^m) worst case where m is program length, but pruning makes it much faster in practice
- Space: O(1) for simulation, O(m) for recursion depth

## Key Insights
1. The program structure (dividing by 8) naturally suggests building A in base 8
2. Working backwards from the output allows efficient pruning
3. Each output digit constrains only a few bits of A, making the search tractable
4. The quine property means the program encodes how to produce itself