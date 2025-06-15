CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -I./include
LDFLAGS = -lm
SRCDIR = src
BUILDDIR = build
INCLUDE = include

# Find all day*.c files
SOURCES = $(wildcard $(SRCDIR)/day*.c)
TARGETS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%,$(SOURCES))

.PHONY: all clean run-all

all: $(BUILDDIR) $(TARGETS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Pattern rule for building individual days
$(BUILDDIR)/day%: $(SRCDIR)/day%.c $(INCLUDE)/aoc.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# Build specific day (e.g., make day01)
day%: $(BUILDDIR)/day%
	@echo "Built $<"

# Run all solutions
run-all: all
	@echo "Running all solutions..."
	@for day in $(TARGETS); do \
		echo "\n========== Running $$day =========="; \
		$$day || true; \
	done

# Run specific day (e.g., make run-day01)
run-day%: $(BUILDDIR)/day%
	@echo "Running $<..."
	@$<

# Clean build directory
clean:
	rm -rf $(BUILDDIR)

# Help target
help:
	@echo "Advent of Code 2024 - C Framework"
	@echo ""
	@echo "Targets:"
	@echo "  make all         - Build all solutions"
	@echo "  make day01       - Build day 1 solution"
	@echo "  make run-day01   - Build and run day 1"
	@echo "  make run-all     - Run all solutions"
	@echo "  make clean       - Remove build files"
	@echo "  make help        - Show this help"