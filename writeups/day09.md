# Day 9: Disk Fragmenter

## Problem Understanding

A disk map encodes file and free space blocks. We need to compact the disk and calculate a checksum.

### Part 1: Block-Level Compaction
Move individual file blocks to the leftmost available free space.

### Part 2: File-Level Compaction
Move entire files to the leftmost free space that can fit them.

## Approach

### Part 1: Individual Blocks
1. Expand disk map to block representation
2. Move blocks from right to left into free spaces
3. Calculate checksum: position × file_id

### Part 2: Whole Files
1. Track files with position and size
2. Process files in decreasing ID order
3. Find leftmost fitting free span
4. Move entire file if space found

## Implementation Details

- Alternating file/free space parsing
- Efficient free space tracking
- In-place array manipulation for part 1
- File metadata structure for part 2

## Complexity Analysis

- **Part 1**: O(n) where n is total blocks
- **Part 2**: O(f²) where f is number of files

## Key Insights

1. The disk map alternates between files and free spaces
2. Part 1 allows splitting files, Part 2 requires contiguous moves
3. Processing files in reverse ID order ensures correct behavior