# Philosophers - Dining Philosophers Problem Implementation

A solution to the classic dining philosophers concurrency problem using POSIX threads. This implementation demonstrates deadlock prevention, thread synchronization, and resource management in C.

## Overview

The dining philosophers problem is a classic concurrency challenge where N philosophers sit around a table, each needing two forks to eat. The challenge is preventing deadlock (when all philosophers hold one fork and wait for another) while ensuring no philosopher starves.

This implementation solves the problem using:
- **Asymmetric fork ordering** to prevent deadlock
- **Dedicated monitoring** for death detection  
- **Precise timing** for millisecond accuracy
- **Thread-safe operations** throughout

## Installation and Usage

### Prerequisites
- GCC compiler with pthread support
- POSIX-compliant system (Linux/macOS)
- Make utility

### Quick Start
```bash
# Clone and compile
git clone <repository-url>
cd philocal
make

# Run with 4 philosophers
./philo 4 800 200 200
```

### Usage
```bash
./philo <philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [must_eat_count]
```

**Parameters:**
- `philosophers`: Number of philosophers (1-200)
- `time_to_die`: Max time (ms) without eating before death
- `time_to_eat`: Time (ms) spent eating  
- `time_to_sleep`: Time (ms) spent sleeping
- `must_eat_count`: (Optional) Stop when all have eaten this many times

**Examples:**
```bash
./philo 4 410 200 200           # Basic example - should survive
./philo 4 310 200 100           # Tight timing - someone will die
./philo 5 800 200 200 7         # Stop after everyone eats 7 times
## How the Problem is Solved

This implementation prevents deadlock using **asymmetric fork ordering**:
- Even-numbered philosophers pick up right fork first, then left
- Odd-numbered philosophers pick up left fork first, then right

This breaks the circular wait condition that causes deadlock.

Additional features:
- **Death monitoring**: Dedicated thread detects when philosophers starve
- **Precise timing**: Custom sleep functions for millisecond accuracy
- **Clean termination**: Proper cleanup when simulation ends

## Testing

Use the included 42 tester:
```bash
cd 42-philosophers-tester
bash test.sh
```

Or test manually:
```bash
./philo 4 410 200 200           # Basic survival test
./philo 4 310 200 100           # Death scenario test  
./philo 5 800 200 200 7         # Completion test
./philo 1 800 200 200           # Edge case (impossible)
```

## Project Structure
```
├── src/                        # Source code
├── philo.h                     # Header file
├── Makefile                    # Build configuration
├── 42-philosophers-tester/     # Official test suite
└── README.md                   # This file
```

---

**Note**: This is a 42 School project implementing the dining philosophers problem in C using POSIX threads.
