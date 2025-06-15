#!/usr/bin/env python3
"""
Run all available C solutions and display results.
"""
import subprocess
import time
from pathlib import Path
from datetime import datetime

def run_day(day):
    """Run solution for a specific day."""
    binary_file = Path(f"build/day{day:02d}")
    input_file = Path(f"inputs/day{day:02d}.txt")
    
    if not binary_file.exists():
        return None, None, "Not built"
    
    if not input_file.exists():
        return None, None, "No input"
    
    try:
        start = time.time()
        result = subprocess.run(
            [str(binary_file)],
            capture_output=True,
            text=True,
            timeout=30
        )
        elapsed = time.time() - start
        
        if result.returncode != 0:
            return None, None, f"Error: {result.stderr}"
        
        # Parse output
        lines = result.stdout.strip().split('\n')
        part1 = part2 = None
        
        for line in lines:
            if line.startswith("Part 1:"):
                part1 = line.split(":", 1)[1].strip()
            elif line.startswith("Part 2:"):
                part2 = line.split(":", 1)[1].strip()
        
        return part1, part2, f"{elapsed:.2f}s"
    
    except subprocess.TimeoutExpired:
        return None, None, "Timeout"
    except Exception as e:
        return None, None, f"Error: {str(e)}"

def main():
    print("🎄 Advent of Code 2024 - All Solutions 🎄")
    print("=" * 50)
    
    total_time = 0
    solved_count = 0
    
    # Determine which days to run
    today = datetime.utcnow()
    max_day = 25
    if today.year == 2024 and today.month == 12:
        max_day = min(25, today.day)
    
    results = []
    
    for day in range(1, max_day + 1):
        print(f"Running Day {day:02d}...", end=" ", flush=True)
        part1, part2, status = run_day(day)
        
        if "s" in status and status[0].isdigit():
            time_taken = float(status[:-1])
            total_time += time_taken
            if part1 and part1 != "None":
                solved_count += 1
            if part2 and part2 != "None":
                solved_count += 1
        
        results.append((day, part1, part2, status))
        print(f"[{status}]")
    
    # Display summary
    print("\n" + "=" * 50)
    print("SUMMARY")
    print("=" * 50)
    print(f"{'Day':<5} {'Part 1':<20} {'Part 2':<20} {'Time':<10}")
    print("-" * 50)
    
    for day, part1, part2, status in results:
        p1 = part1 or "—"
        p2 = part2 or "—"
        print(f"{day:02d}    {p1:<20} {p2:<20} {status:<10}")
    
    print("-" * 50)
    print(f"Total solved: {solved_count}/50")
    print(f"Total time: {total_time:.2f}s")

if __name__ == "__main__":
    main()