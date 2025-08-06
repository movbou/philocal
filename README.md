# Philosophers - Dining Philosophers Problem Implementation

A comprehensive implementation of the classic dining philosophers problem using POSIX threads and mutexes, following 42 School coding standards.

## Table of Contents

1. [Overview](#overview)
2. [The Dining Philosophers Problem](#the-dining-philosophers-problem)
3. [Implementation Details](#implementation-details)
4. [Threading Concepts](#threading-concepts)
5. [Installation and Usage](#installation-and-usage)
6. [Testing](#testing)
7. [Code Documentation](#code-documentation)
8. [Performance Analysis](#performance-analysis)
9. [Learning Objectives](#learning-objectives)

## Overview

This project implements the famous dining philosophers problem, a classic synchronization problem in computer science that illustrates the challenges of resource allocation and deadlock prevention in concurrent programming.

The implementation uses POSIX threads (pthreads) with mutex locks to ensure thread safety while preventing deadlocks through careful resource ordering.

## The Dining Philosophers Problem

### Problem Description

The dining philosophers problem was originally formulated by Edsger Dijkstra in 1965 and later refined by Tony Hoare. It describes a scenario where:

- **N philosophers** sit around a circular table
- Each philosopher has a **plate of spaghetti** 
- There is exactly **one fork between each pair** of adjacent philosophers
- To eat, a philosopher must **hold both forks** (left and right)
- Philosophers alternate between three states: **thinking**, **eating**, and **sleeping**

### The Challenge

The challenge is to design a protocol that prevents:

1. **Deadlock**: All philosophers pick up their left fork simultaneously, leaving no right forks available
2. **Starvation**: Some philosophers never get a chance to eat
3. **Race conditions**: Inconsistent state due to unsynchronized access to shared resources

### Real-World Parallels

This problem models many real-world resource allocation scenarios:
- **Database transactions** competing for locks
- **Process scheduling** in operating systems  
- **Network protocols** managing bandwidth
- **Memory management** in multi-threaded applications

## Implementation Details

### Solution Strategy

Our implementation uses several key strategies to solve the dining philosophers problem:

#### 1. Deadlock Prevention Through Fork Ordering

**Problem**: If all philosophers pick up their left fork first, deadlock occurs.

**Solution**: Alternate the fork acquisition order:
- **Even-numbered philosophers**: Pick right fork first, then left
- **Odd-numbered philosophers**: Pick left fork first, then right

```c
void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->right_fork->fork);
        pthread_mutex_lock(&philo->left_fork->fork);
    }
    else
    {
        pthread_mutex_lock(&philo->left_fork->fork);
        pthread_mutex_lock(&philo->right_fork->fork);
    }
}
```

#### 2. Death Detection and Monitoring

**Problem**: Philosophers can die if they don't eat within a specified time.

**Solution**: Dedicated monitor thread that:
- Checks each philosopher's last meal time
- Terminates simulation when death is detected
- Prevents race conditions with death_mutex

#### 3. Precise Timing

**Problem**: Standard sleep functions are imprecise for millisecond timing.

**Solution**: Custom usleep implementation using busy-waiting:

```c
void ft_usleep(long time)
{
    long start = get_current_time();
    while (get_current_time() - start < time)
        usleep(500);
}
```

### Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        Main Process                         │
├─────────────────────────────────────────────────────────────┤
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐     │
│  │ Philosopher │    │ Philosopher │    │ Philosopher │     │
│  │   Thread    │    │   Thread    │    │   Thread    │ ... │
│  │     #1      │    │     #2      │    │     #N      │     │
│  └─────────────┘    └─────────────┘    └─────────────┘     │
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │               Monitor Thread                        │   │
│  │        (Death Detection & Completion)               │   │
│  └─────────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────────┤
│                     Shared Resources                       │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌────────│
│  │ Fork #1 │ │ Fork #2 │ │ Fork #3 │ │ Fork #4 │ │ Fork #N││
│  │ (Mutex) │ │ (Mutex) │ │ (Mutex) │ │ (Mutex) │ │ (Mutex)││
│  └─────────┘ └─────────┘ └─────────┘ └─────────┘ └────────│
│                                                             │
│  ┌─────────────────┐ ┌─────────────────┐                  │
│  │   Write Mutex   │ │   Death Mutex   │                  │
│  │ (Output Sync)   │ │(Death Detection)│                  │
│  └─────────────────┘ └─────────────────┘                  │
└─────────────────────────────────────────────────────────────┘
```

## Threading Concepts

### Mutexes (Mutual Exclusion)

**Purpose**: Ensure only one thread can access a shared resource at a time.

**Types used in our implementation**:

1. **Fork Mutexes**: One per fork, prevents multiple philosophers from grabbing the same fork
2. **Write Mutex**: Synchronizes console output to prevent garbled messages  
3. **Death Mutex**: Protects last_meal_time updates and death checks

### Race Conditions

**Definition**: When the outcome depends on the unpredictable timing of thread execution.

**Example without protection**:
```c
// DANGEROUS: Race condition
philosopher->meal_count++;  // Thread A
if (philosopher->meal_count >= must_eat) // Thread B might see inconsistent state
```

**Our solution**:
```c
// SAFE: Protected by mutex
pthread_mutex_lock(&data->death_mutex);
philo->last_meal_time = get_current_time();
philo->meal_count++;
pthread_mutex_unlock(&data->death_mutex);
```

### Deadlocks

**Definition**: A situation where threads are blocked forever, each waiting for the other.

**Classic Dining Philosophers Deadlock**:
1. All philosophers pick up left fork
2. All philosophers try to pick up right fork (already taken)
3. Nobody can proceed → **DEADLOCK**

**Our Prevention Strategy**:
- **Resource Ordering**: Always acquire lower-numbered resources first
- **Timeout Prevention**: Monitor thread ensures bounded waiting time
- **Starvation Prevention**: Fair scheduling through fork ordering

### Thread Synchronization Patterns

#### Producer-Consumer Pattern
- **Philosophers**: Consume time and forks, produce eating actions
- **Monitor**: Consumes philosopher states, produces termination signals

#### Observer Pattern  
- **Monitor Thread**: Observes all philosopher states
- **Philosopher Threads**: Notify state changes implicitly through shared memory

## Installation and Usage

### Prerequisites

- GCC compiler with pthread support
- POSIX-compliant system (Linux, macOS)
- Make utility

### Compilation

```bash
make          # Compile the project
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Rebuild everything
```

### Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

**Parameters**:
- `number_of_philosophers`: Number of philosophers (1-200)
- `time_to_die`: Time in ms after which a philosopher dies if not eating
- `time_to_eat`: Time in ms a philosopher spends eating
- `time_to_sleep`: Time in ms a philosopher spends sleeping
- `number_of_times_each_philosopher_must_eat`: Optional simulation termination condition

**Examples**:
```bash
./philo 4 310 200 100           # 4 philosophers, might die
./philo 4 800 200 200           # 4 philosophers, should survive  
./philo 5 800 200 200 7         # 5 philosophers, stop after 7 meals each
./philo 1 800 200 200           # Single philosopher (impossible to eat)
```

## Testing

### Comprehensive Test Suite

Run the complete test suite:
```bash
./test_philo.sh
```

The test suite includes:

#### 1. Argument Validation Tests
- Invalid argument counts
- Negative values
- Non-numeric inputs
- Boundary conditions

#### 2. Edge Case Tests  
- Single philosopher (impossible scenario)
- Two philosophers (minimal viable scenario)
- Maximum philosophers (200)
- Minimum valid timings (60ms)

#### 3. Death Scenario Tests
- Inevitable death conditions
- Fast death timing
- Multiple philosopher death scenarios

#### 4. Survival Scenario Tests
- Safe timing configurations
- Generous timing margins
- Long-running simulations

#### 5. Must-Eat Completion Tests
- Various completion conditions
- Multiple meal requirements
- Early termination scenarios

#### 6. Stress Tests
- High philosopher counts
- High-frequency operations
- Extended simulations

#### 7. Performance Tests
- Timing benchmarks
- Resource utilization
- Memory leak detection (with Valgrind)

### Manual Testing Examples

```bash
# Test basic functionality
./philo 4 410 200 200

# Test death scenario
./philo 4 310 200 100

# Test completion scenario  
./philo 4 800 200 200 5

# Test edge cases
./philo 1 800 200 200    # Should show single fork taken, then death
./philo 2 800 200 200    # Minimal working scenario
./philo 200 1000 200 200 # Stress test
```

## Performance Analysis

### Time Complexity
- **Philosopher threads**: O(1) per action (constant time operations)
- **Monitor thread**: O(n) per check cycle (linear in philosopher count)
- **Overall**: O(n) scaling with philosopher count

### Space Complexity
- **Memory usage**: O(n) where n is number of philosophers
- **Mutex overhead**: O(n) mutexes for forks + 2 global mutexes

### Optimization Considerations

1. **Monitor frequency**: Balance between responsiveness and CPU usage
2. **Timing precision**: Custom usleep provides millisecond accuracy
3. **Memory layout**: Structs designed for cache efficiency
4. **Lock granularity**: Minimal critical sections to reduce contention

## Learning Objectives

### Threading Concepts Mastered

1. **Thread Creation and Management**
   - pthread_create(), pthread_join()
   - Thread lifecycle and cleanup

2. **Mutex Synchronization**
   - pthread_mutex_lock(), pthread_mutex_unlock()
   - Critical section design
   - Avoiding race conditions

3. **Deadlock Prevention**
   - Resource ordering strategies
   - Timeout mechanisms
   - Banker's algorithm concepts

4. **Inter-thread Communication**
   - Shared memory patterns
   - Atomic operations
   - Signal handling

### Algorithm Design Principles

1. **Concurrent Algorithm Design**
   - Decomposing problems into concurrent components
   - Identifying shared resources and dependencies
   - Designing for scalability

2. **Resource Management**
   - RAII (Resource Acquisition Is Initialization) patterns
   - Proper cleanup in error conditions
   - Memory leak prevention

3. **Error Handling**
   - Graceful degradation
   - Resource cleanup on failure
   - Robust error reporting

### Real-World Applications

Understanding gained from this project applies to:

- **Web Server Development**: Handling concurrent requests
- **Database Design**: Transaction management and locking
- **Operating Systems**: Process scheduling and resource allocation  
- **Game Development**: Multi-threaded game loops and state management
- **High-Performance Computing**: Parallel algorithm design

---

For detailed code documentation, see [TECHNICAL_DOCUMENTATION.md](TECHNICAL_DOCUMENTATION.md)
