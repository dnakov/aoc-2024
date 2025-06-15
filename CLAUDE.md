# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview
This is a C-based framework for solving Advent of Code 2024 challenges. The project is structured to:
- Fetch daily puzzle inputs
- Provide solution templates for each day
- Build and run C solutions
- Submit answers to AoC
- Generate write-ups for each solution

## Project Structure
```
aoc2024/
├── src/            # Daily solutions (day01.c through day25.c)
├── include/        # Header files
│   └── aoc.h      # Common utilities and helpers
├── inputs/         # Input files for each day
├── writeups/       # Solution explanations and approaches
├── utils/          # Python utilities for AoC interaction
│   ├── aoc.py     # AoC API interactions (fetch/submit)
│   └── template.py # Solution template generator
├── build/          # Compiled binaries
└── Makefile        # Build configuration
```

## Common Commands

### Building Solutions
```bash
# Build a specific day
make day01

# Build all solutions
make all

# Clean build files
make clean
```

### Running Solutions
```bash
# Run a specific day's solution
./build/day01

# Run with test input
./build/day01 test

# Run all solutions
make run-all
```

### Fetching Inputs
```bash
# Fetch input for a specific day (requires session cookie)
python utils/aoc.py fetch 1

# Fetch all available inputs
python utils/aoc.py fetch-all
```

### Creating New Day
```bash
# Create template for a new day
python utils/template.py create 5
```

### Submitting Solutions
```bash
# Submit answer for day 1 part 1
python utils/aoc.py submit 1 1 <answer>

# Submit answer for day 1 part 2
python utils/aoc.py submit 1 2 <answer>
```

## Key Implementation Details

### Solution Structure
Each day's C solution follows this pattern:
- Input parsing functions to read and process the input file
- `part1()`: Solve part 1 of the puzzle
- `part2()`: Solve part 2 of the puzzle
- `main()`: Handle command line args and display results

### Common Utilities (aoc.h)
The header file provides:
- File reading utilities
- String parsing functions (extract integers, split lines)
- Common data structures (dynamic arrays, hash maps)
- Graph algorithms (BFS, DFS, Dijkstra)
- Grid utilities for 2D problems

### Memory Management
- Always free allocated memory
- Use stack allocation when possible
- Consider memory pools for frequently allocated small objects

### Session Cookie
The AoC session cookie must be stored in `.env` file as `AOC_SESSION=<your-session-cookie>` for fetching inputs and submitting solutions.

### Write-up Format
Each solution should have a corresponding write-up in `writeups/day##.md` explaining:
- Problem understanding
- Approach and algorithm choice
- Time/space complexity
- Key insights or tricks