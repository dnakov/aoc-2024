# Day 24: Crossed Wires

## Problem Understanding

This problem involves simulating a digital logic circuit and identifying errors in what should be a ripple-carry adder.

Part 1: Simulate the circuit with AND, OR, and XOR gates to find the decimal output on wires starting with 'z'.

Part 2: The circuit is supposed to be a ripple-carry adder for adding two 45-bit numbers. Find 8 wires that have been swapped.

## Approach

### Part 1: Circuit Simulation
- Parse initial wire values and gate definitions
- Build a dependency graph of gates
- Simulate by repeatedly processing gates whose inputs are ready
- Extract the binary number from z-wires and convert to decimal

### Part 2: Finding Swapped Wires
A ripple-carry adder has a specific structure:
- Bit 0: z00 = x00 XOR y00, carry = x00 AND y00
- Other bits: z_i = (x_i XOR y_i) XOR carry_{i-1}
- Carry propagation: carry_i = (x_i AND y_i) OR (carry_{i-1} AND (x_i XOR y_i))

To find swaps, I looked for:
1. Z-outputs not coming from XOR gates (except z45)
2. XOR gates with matching x/y inputs that directly output to z (except z00)
3. AND gates not feeding into OR gates (except x00 AND y00)
4. Mismatched wire types (e.g., XOR output used where AND output expected)

## Key Insights

1. **Ripple-Carry Structure**: Understanding the expected circuit pattern was crucial
2. **Gate Type Rules**: Each gate type has specific roles in the adder
3. **Manual Analysis**: The automated detection wasn't perfect; manual circuit tracing helped find all swaps
4. **Wire Pairs**: Swapped wires come in pairs - finding one helps identify its partner

## Implementation Details

- Used wire name indexing for O(1) lookups
- Implemented topological simulation for Part 1
- For Part 2, traced specific problematic paths manually:
  - z09 ↔ nnf (bit 9 final XOR)
  - z20 ↔ nhs (bit 20 final XOR)
  - z34 ↔ wrc (bit 34 final XOR)
  - ddn ↔ kqh (bit 30 XOR/AND swap)

## Complexity

- Time: O(G) for simulation where G is the number of gates
- Space: O(W + G) for storing wires and gates
- Part 2: Manual analysis time to trace the circuit

The challenge combined circuit simulation with pattern recognition to debug a faulty adder implementation.