# Day 2: Red-Nosed Reports

## Problem Understanding

The problem involves analyzing reactor safety reports. Each report contains a sequence of levels that must meet specific criteria to be considered "safe":

### Safety Rules
1. Levels must be either all increasing OR all decreasing (no mixed directions)
2. Adjacent levels must differ by at least 1 and at most 3

### Part 2: Problem Dampener
The Problem Dampener allows removing a single level from an unsafe report to potentially make it safe.

## Approach

### Part 1: Basic Safety Check
1. Parse each line as a report containing multiple levels
2. For each report:
   - Determine direction from first two levels
   - Verify all subsequent pairs maintain same direction
   - Check that differences are within [1, 3] range

### Part 2: Dampener Logic
1. First check if report is already safe (no removal needed)
2. If unsafe, try removing each level one at a time
3. Check if any single removal makes the report safe

## Implementation Details

- Used dynamic arrays to handle variable-length reports
- Created helper function `is_safe_report()` for reusability
- Part 2 uses brute force approach (O(n²) per report)

## Complexity Analysis

- **Part 1**: O(n×m) where n = number of reports, m = average levels per report
- **Part 2**: O(n×m²) due to trying each possible removal

## Key Insights

1. Direction must be determined from valid adjacent pairs (not just first two)
2. The dampener can fix reports by removing problematic levels at any position
3. Brute force is acceptable given the small report sizes