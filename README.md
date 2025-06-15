# Advent of Code 2024 🎄

**All 50 stars achieved!** 🌟

My complete solutions for [Advent of Code 2024](https://adventofcode.com/2024) written in C.

This repository contains a complete framework for solving Advent of Code challenges, including utilities for fetching inputs, submitting answers, and building C solutions.

## Setup

1. Clone this repository
2. Install Python dependencies for fetching inputs:
   ```bash
   pip install -r requirements.txt
   ```
3. Get your session cookie from adventofcode.com and create a `.env` file:
   ```
   AOC_SESSION=your_session_cookie_here
   ```
4. Make sure you have GCC installed

## Quick Start

### Fetch puzzle input
```bash
python utils/aoc.py fetch 1  # Fetch day 1 input
python utils/aoc.py fetch-all  # Fetch all available inputs
```

### Create a new day's solution
```bash
python utils/template.py create 1  # Creates src/day01.c
```

### Build and run
```bash
make day01         # Build day 1
./build/day01      # Run with real input
./build/day01 test # Run with test input

make all           # Build all solutions
make run-all       # Run all solutions
```

### Submit an answer
```bash
python utils/aoc.py submit 1 1 12345  # Submit 12345 as answer for day 1 part 1
```

## Project Structure

```
aoc2024/
├── src/            # Daily solutions (day01.c through day25.c)
├── include/        # Header files
│   └── aoc.h      # Common utilities and helpers
├── inputs/         # Puzzle inputs (git-ignored)
├── writeups/       # Solution explanations
├── utils/          # Python utilities
│   ├── aoc.py     # AoC API interactions
│   └── template.py # Solution template generator
├── build/          # Compiled binaries
└── Makefile        # Build configuration
```

## Progress

| Day | Part 1 | Part 2 | Name | Notes |
|-----|--------|--------|------|-------|
| 01  | ✅     | ✅     | Historian Hysteria | List distance and similarity calculations |
| 02  | ✅     | ✅     | Red-Nosed Reports | Safe sequence detection with dampener |
| 03  | ✅     | ✅     | Mull It Over | Regex parsing and conditional multiplication |
| 04  | ✅     | ✅     | Ceres Search | Word search and X-MAS patterns |
| 05  | ✅     | ✅     | Print Queue | Topological sorting with page ordering rules |
| 06  | ✅     | ✅     | Guard Gallivant | Path tracing and loop detection |
| 07  | ✅     | ✅     | Bridge Repair | Expression evaluation with operators |
| 08  | ✅     | ✅     | Resonant Collinearity | Antenna antinode calculations |
| 09  | ✅     | ✅     | Disk Fragmenter | File system defragmentation |
| 10  | ✅     | ✅     | Hoof It | Trail rating and path counting |
| 11  | ✅     | ✅     | Plutonian Pebbles | Stone transformation with memoization |
| 12  | ✅     | ✅     | Garden Groups | Region perimeter and side counting |
| 13  | ✅     | ✅     | Claw Contraption | Linear equation solving for prizes |
| 14  | ✅     | ✅     | Restroom Redoubt | Robot movement simulation |
| 15  | ✅     | ✅     | Warehouse Woes | Sokoban-style box pushing puzzle |
| 16  | ✅     | ✅     | Reindeer Maze | Dijkstra's algorithm with rotation costs |
| 17  | ✅     | ✅     | Chronospatial Computer | VM implementation and reverse engineering |
| 18  | ✅     | ✅     | RAM Run | Grid pathfinding with falling bytes |
| 19  | ✅     | ✅     | Linen Layout | Pattern matching with dynamic programming |
| 20  | ✅     | ✅     | Race Condition | Shortest path with cheating mechanics |
| 21  | ✅     | ✅     | Keypad Conundrum | Nested robot control optimization |
| 22  | ✅     | ✅     | Monkey Market | Pseudo-random sequence analysis |
| 23  | ✅     | ✅     | LAN Party | Graph cliques and network analysis |
| 24  | ✅     | ✅     | Crossed Wires | Boolean circuit simulation |
| 25  | ✅     | ✅     | Code Chronicle | Lock and key matching |

**Total Stars:** 50/50 ⭐🌟 **COMPLETE!** 🌟⭐

## Features

### Framework
- **Automatic Input Fetching**: Download puzzle inputs directly from AoC
- **Answer Submission**: Submit solutions without leaving the terminal
- **Template Generation**: Quickly create boilerplate for new days
- **Batch Operations**: Build and run all solutions at once

### Helper Functions

The `include/aoc.h` header provides common utilities:
- File reading and line parsing
- Integer extraction from strings
- Dynamic arrays and hash maps
- Grid utilities for 2D problems
- Queue implementation for BFS
- Math utilities (GCD, LCM, min/max)

## Solution Highlights

### Day 15: Warehouse Woes
A Sokoban-style puzzle where a robot pushes boxes in a warehouse. Implemented with:
- Recursive box chain pushing
- GPS coordinate calculation
- Part 2: Wide warehouse with double-width boxes

### Day 17: Chronospatial Computer
Virtual machine implementation with reverse engineering:
- Custom instruction set simulation
- Part 2: Finding quine programs through analysis

### Day 21: Keypad Conundrum
Multi-level robot control optimization:
- Directional keypad sequence generation
- Nested robot control with memoization
- Optimal path finding for button sequences

### Day 24: Crossed Wires
Boolean circuit simulation and analysis:
- Gate evaluation with topological sorting
- Circuit debugging and wire swapping

## License

This project is open source and available under the MIT License.