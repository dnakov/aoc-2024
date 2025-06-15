#!/usr/bin/env python3
"""
Advent of Code API utilities for fetching inputs and submitting solutions.
"""
import os
import sys
import requests
from pathlib import Path
from datetime import datetime
import time
from dotenv import load_dotenv

load_dotenv()

AOC_URL = "https://adventofcode.com"
YEAR = 2024

def get_session():
    """Get AoC session cookie from environment."""
    session = os.getenv("AOC_SESSION")
    if not session:
        print("Error: AOC_SESSION not found in environment or .env file")
        print("Please add your session cookie to .env file:")
        print("AOC_SESSION=<your-session-cookie>")
        sys.exit(1)
    return session

def fetch_input(day):
    """Fetch input for a specific day."""
    session = get_session()
    day = int(day)
    
    # Check if we're too early
    puzzle_time = datetime(YEAR, 12, day, 5, 0, 0)  # 5 AM UTC
    if datetime.utcnow() < puzzle_time:
        print(f"Day {day} hasn't been released yet!")
        print(f"Puzzle unlocks at: {puzzle_time} UTC")
        return False
    
    url = f"{AOC_URL}/{YEAR}/day/{day}/input"
    headers = {"Cookie": f"session={session}"}
    
    print(f"Fetching input for day {day}...")
    response = requests.get(url, headers=headers)
    
    if response.status_code == 200:
        input_path = Path(f"inputs/day{day:02d}.txt")
        input_path.write_text(response.text)
        print(f"Saved to: {input_path}")
        return True
    else:
        print(f"Failed to fetch input: {response.status_code}")
        print(response.text)
        return False

def fetch_all():
    """Fetch all available inputs."""
    today = datetime.utcnow()
    max_day = min(25, today.day if today.month == 12 and today.year == YEAR else 0)
    
    if max_day == 0:
        print("No puzzles are available yet!")
        return
    
    print(f"Fetching inputs for days 1-{max_day}...")
    for day in range(1, max_day + 1):
        input_path = Path(f"inputs/day{day:02d}.txt")
        if input_path.exists():
            print(f"Day {day}: Already exists, skipping")
        else:
            if fetch_input(day):
                time.sleep(1)  # Be nice to the server

def submit_answer(day, part, answer):
    """Submit an answer for a specific day and part."""
    session = get_session()
    day = int(day)
    part = int(part)
    
    if part not in [1, 2]:
        print("Part must be 1 or 2")
        return
    
    url = f"{AOC_URL}/{YEAR}/day/{day}/answer"
    headers = {"Cookie": f"session={session}"}
    data = {"level": str(part), "answer": str(answer)}
    
    print(f"Submitting answer for day {day} part {part}: {answer}")
    response = requests.post(url, headers=headers, data=data)
    
    if response.status_code == 200:
        # Parse response to check if answer was correct
        text = response.text.lower()
        if "that's the right answer" in text:
            print("✅ Correct answer!")
        elif "that's not the right answer" in text:
            print("❌ Wrong answer!")
            if "too high" in text:
                print("   (Your answer is too high)")
            elif "too low" in text:
                print("   (Your answer is too low)")
        elif "you gave an answer too recently" in text:
            print("⏰ Please wait before submitting again")
        elif "already complete" in text:
            print("✓ You've already solved this!")
        else:
            print("? Unknown response:")
            print(text[:500])
    else:
        print(f"Failed to submit: {response.status_code}")

def main():
    if len(sys.argv) < 2:
        print("Usage:")
        print("  python utils/aoc.py fetch <day>      # Fetch input for a day")
        print("  python utils/aoc.py fetch-all        # Fetch all available inputs")
        print("  python utils/aoc.py submit <day> <part> <answer>  # Submit answer")
        sys.exit(1)
    
    command = sys.argv[1]
    
    if command == "fetch":
        if len(sys.argv) < 3:
            print("Usage: python utils/aoc.py fetch <day>")
            sys.exit(1)
        fetch_input(sys.argv[2])
    
    elif command == "fetch-all":
        fetch_all()
    
    elif command == "submit":
        if len(sys.argv) < 5:
            print("Usage: python utils/aoc.py submit <day> <part> <answer>")
            sys.exit(1)
        submit_answer(sys.argv[2], sys.argv[3], sys.argv[4])
    
    else:
        print(f"Unknown command: {command}")
        sys.exit(1)

if __name__ == "__main__":
    main()