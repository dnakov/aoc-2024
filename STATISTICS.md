# Advent of Code 2024 - Solution Statistics

## Summary
- **Days Completed**: 15/25 (Days 1-15)
- **Total Parts Solved**: 30/30 (All parts for completed days)
- **Success Rate**: 100% for attempted days

## Solutions by Day

| Day | Part 1 Answer | Part 2 Answer | Complexity | Key Algorithm |
|-----|---------------|---------------|------------|---------------|
| 1   | 2,430,334 | 28,786,472 | O(n log n) | Sorting + Frequency Count |
| 2   | 230 | 301 | O(n×m²) | Sequence Validation |
| 3   | 166,357,705 | 88,811,886 | O(n) | Regex Pattern Matching |
| 4   | 2,662 | 2,034 | O(n×m) | Grid Pattern Search |
| 5   | 4,662 | 5,900 | O(n×m²) | Topological Sort |
| 6   | 5,404 | 1,984 | O(n×m×k) | Path Simulation + Cycle Detection |
| 7   | 3,598,800,864,292 | 340,362,529,351,427 | O(n×3^m) | Recursive Backtracking |
| 8   | 357 | 1,266 | O(f×n²×g) | Vector Math + Collinearity |
| 9   | 6,446,899,523,367 | 6,478,232,739,671 | O(f²) | Disk Defragmentation |
| 10  | 786 | 1,722 | O(t×n×m) | DFS Path Finding |
| 11  | 185,894 | 221,632,504,974,231 | O(U×B) | Dynamic Programming + Memoization |
| 12  | 1,494,342 | 893,676 | O(n×m) | DFS + Corner Counting |
| 13  | 25,751 | 108,528,956,728,655 | O(n) | Linear Algebra (Cramer's Rule) |
| 14  | 208,437,768 | 7,492 | O(n×t) | Modular Arithmetic + Pattern Detection |
| 15  | 1,558,015 | 1,969,654 | O(m×b) | BFS + State Management |

## Algorithm Categories Used

### Graph/Grid Algorithms
- DFS (Days 10, 12)
- BFS (Day 15)
- Path simulation (Day 6)
- Pattern detection (Days 4, 14)

### Mathematical Approaches
- Linear algebra/Cramer's rule (Day 13)
- Modular arithmetic (Day 14)
- Vector mathematics (Day 8)
- Topological sorting (Day 5)

### Dynamic Programming
- Memoization (Day 11)
- State space exploration (Day 7)

### String/Pattern Processing
- Regex matching (Day 3)
- Pattern recognition (Days 4, 14)

### Data Structure Techniques
- Sorting algorithms (Day 1)
- Hash tables (Days 11, 12)
- Priority queues (implicit in Day 9)

## Performance Optimizations

1. **Day 11**: Transformed from array simulation to memoized counting
2. **Day 7**: Early pruning when result exceeds target
3. **Day 6**: Only check obstacle positions on original path
4. **Day 9**: Efficient free space tracking
5. **Day 12**: Corner counting instead of edge tracing

## Interesting Challenges

- **Most Mathematically Elegant**: Day 13 (Cramer's Rule)
- **Largest Numbers**: Day 11 Part 2 (221+ trillion stones)
- **Most Visual**: Day 14 Part 2 (Christmas tree pattern)
- **Most Complex State**: Day 15 Part 2 (Wide box pushing)
- **Best Optimization**: Day 11 (Exponential to polynomial)