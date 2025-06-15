# Advent of Code 2024 🎄

My solutions for [Advent of Code 2024](https://adventofcode.com/2024) written in C.

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

| Day | Part 1 | Part 2 | Write-up |
|-----|--------|--------|----------|
| 01  | ⬜     | ⬜     | ⬜       |
| 02  | ⬜     | ⬜     | ⬜       |
| 03  | ⬜     | ⬜     | ⬜       |
| 04  | ⬜     | ⬜     | ⬜       |
| 05  | ⬜     | ⬜     | ⬜       |
| ... | ...    | ...    | ...      |
| 25  | ⬜     | ⬜     | ⬜       |

## Helper Functions

The `include/aoc.h` header provides common utilities:
- File reading and line parsing
- Integer extraction from strings
- Dynamic arrays and hash maps
- Grid utilities for 2D problems
- Queue implementation for BFS
- Math utilities (GCD, LCM, min/max)