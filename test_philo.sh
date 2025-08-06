#!/bin/bash

# Comprehensive testing script for the Dining Philosophers project
# Tests all edge cases and scenarios to ensure robust implementation

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counters
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Test results log
TEST_LOG="test_results.log"
echo "=== Dining Philosophers Test Results ===" > $TEST_LOG
echo "Test started at: $(date)" >> $TEST_LOG
echo "" >> $TEST_LOG

print_header() {
    echo -e "${BLUE}================================${NC}"
    echo -e "${BLUE}$1${NC}"
    echo -e "${BLUE}================================${NC}"
}

print_test() {
    echo -e "${YELLOW}Testing: $1${NC}"
    echo "Testing: $1" >> $TEST_LOG
}

print_pass() {
    echo -e "${GREEN}✓ PASSED: $1${NC}"
    echo "✓ PASSED: $1" >> $TEST_LOG
    ((PASSED_TESTS++))
}

print_fail() {
    echo -e "${RED}✗ FAILED: $1${NC}"
    echo "✗ FAILED: $1" >> $TEST_LOG
    ((FAILED_TESTS++))
}

run_test() {
    local test_name="$1"
    local args="$2"
    local expected_result="$3"
    local timeout_duration="$4"
    
    ((TOTAL_TESTS++))
    print_test "$test_name"
    echo "Command: ./philo $args" >> $TEST_LOG
    
    if [ "$timeout_duration" -eq 0 ]; then
        # For tests that should exit immediately (error cases)
        output=$(timeout 2s ./philo $args 2>&1)
        exit_code=$?
    else
        # For simulation tests
        output=$(timeout ${timeout_duration}s ./philo $args 2>&1)
        exit_code=$?
    fi
    
    echo "Exit code: $exit_code" >> $TEST_LOG
    echo "Output: $output" >> $TEST_LOG
    echo "" >> $TEST_LOG
    
    case $expected_result in
        "error")
            if [ $exit_code -ne 0 ]; then
                print_pass "$test_name"
            else
                print_fail "$test_name - Expected error but got success"
            fi
            ;;
        "death")
            if echo "$output" | grep -q "died"; then
                print_pass "$test_name"
            else
                print_fail "$test_name - Expected death but didn't find it"
            fi
            ;;
        "no_death")
            if echo "$output" | grep -q "died"; then
                print_fail "$test_name - Unexpected death occurred"
            else
                print_pass "$test_name"
            fi
            ;;
        "completion")
            if echo "$output" | grep -q "died"; then
                print_fail "$test_name - Philosopher died unexpectedly"
            else
                print_pass "$test_name"
            fi
            ;;
    esac
}

# Build the project
print_header "BUILDING PROJECT"
echo "Building project..." >> $TEST_LOG
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    print_pass "Project compilation"
else
    print_fail "Project compilation"
    echo "Build failed. Exiting tests." >> $TEST_LOG
    exit 1
fi

# Test 1: Argument validation
print_header "ARGUMENT VALIDATION TESTS"

run_test "No arguments" "" "error" 0
run_test "Too few arguments" "4 410 200" "error" 0
run_test "Too many arguments" "4 410 200 200 5 extra" "error" 0
run_test "Zero philosophers" "0 410 200 200" "error" 0
run_test "Negative philosophers" "-1 410 200 200" "error" 0
run_test "Too many philosophers" "201 410 200 200" "error" 0
run_test "Negative time_to_die" "4 -410 200 200" "error" 0
run_test "Negative time_to_eat" "4 410 -200 200" "error" 0
run_test "Negative time_to_sleep" "4 410 200 -200" "error" 0
run_test "Time too small (die)" "4 59 200 200" "error" 0
run_test "Time too small (eat)" "4 410 59 200" "error" 0
run_test "Time too small (sleep)" "4 410 200 59" "error" 0
run_test "Negative must_eat" "4 410 200 200 -1" "error" 0
run_test "Non-numeric arguments" "abc 410 200 200" "error" 0
run_test "Mixed invalid arguments" "4 abc 200 200" "error" 0

# Test 2: Edge cases
print_header "EDGE CASE TESTS"

run_test "Single philosopher (should die)" "1 800 200 200" "death" 3
run_test "Two philosophers (basic case)" "2 800 200 200" "no_death" 3
run_test "Minimum valid times" "4 60 60 60" "no_death" 3
run_test "Large number of philosophers" "100 1000 200 200" "no_death" 5
run_test "Maximum philosophers" "200 1000 200 200" "no_death" 5

# Test 3: Death scenarios
print_header "DEATH SCENARIO TESTS"

run_test "Inevitable death (tight timing)" "4 310 200 100" "death" 5
run_test "Very fast death" "4 100 200 200" "death" 3
run_test "Death with multiple philosophers" "5 400 200 100" "death" 5

# Test 4: Survival scenarios  
print_header "SURVIVAL SCENARIO TESTS"

run_test "Safe timing (4 philos)" "4 800 200 200" "no_death" 5
run_test "Safe timing (5 philos)" "5 800 200 200" "no_death" 5
run_test "Generous timing" "4 2000 300 300" "no_death" 5

# Test 5: Must eat scenarios
print_header "MUST EAT SCENARIO TESTS"

run_test "Must eat 1 time (4 philos)" "4 800 200 200 1" "completion" 5
run_test "Must eat 2 times (4 philos)" "4 800 200 200 2" "completion" 7
run_test "Must eat 3 times (5 philos)" "5 800 200 200 3" "completion" 10
run_test "Must eat 5 times (4 philos)" "4 800 200 200 5" "completion" 15

# Test 6: Stress tests
print_header "STRESS TESTS"

run_test "Many philosophers, many meals" "10 2000 200 200 10" "completion" 30
run_test "High frequency eating" "4 1000 50 50" "no_death" 5
run_test "Long simulation" "4 2000 300 300" "no_death" 10

# Test 7: Timing precision tests
print_header "TIMING PRECISION TESTS"

run_test "Exact death timing test" "2 600 200 200" "death" 5
run_test "Close timing margins" "4 500 100 100" "death" 5
run_test "Borderline survival" "4 600 200 100" "no_death" 5

# Memory leak test (if valgrind is available)
print_header "MEMORY TESTS"

if command -v valgrind &> /dev/null; then
    print_test "Memory leak check"
    echo "Testing: Memory leak check" >> $TEST_LOG
    ((TOTAL_TESTS++))
    
    valgrind_output=$(timeout 10s valgrind --leak-check=full --error-exitcode=1 ./philo 4 800 200 200 2 2>&1)
    valgrind_exit=$?
    
    echo "Valgrind output: $valgrind_output" >> $TEST_LOG
    echo "" >> $TEST_LOG
    
    if [ $valgrind_exit -eq 0 ]; then
        print_pass "No memory leaks detected"
    else
        print_fail "Memory leaks detected"
    fi
else
    echo "Valgrind not available - skipping memory tests"
    echo "Valgrind not available - skipping memory tests" >> $TEST_LOG
fi

# Performance test
print_header "PERFORMANCE TESTS"

print_test "Performance benchmark (100 philosophers)"
echo "Testing: Performance benchmark (100 philosophers)" >> $TEST_LOG
((TOTAL_TESTS++))

start_time=$(date +%s.%N)
timeout 10s ./philo 100 1000 200 200 1 > /dev/null 2>&1
exit_code=$?
end_time=$(date +%s.%N)
execution_time=$(echo "$end_time - $start_time" | bc)

echo "Execution time: ${execution_time}s" >> $TEST_LOG
echo "" >> $TEST_LOG

if (( $(echo "$execution_time < 10.0" | bc -l) )); then
    print_pass "Performance test completed in ${execution_time}s"
else
    print_fail "Performance test took too long: ${execution_time}s"
fi

# Summary
print_header "TEST SUMMARY"

echo "" >> $TEST_LOG
echo "=== FINAL RESULTS ===" >> $TEST_LOG
echo "Total tests: $TOTAL_TESTS" >> $TEST_LOG
echo "Passed: $PASSED_TESTS" >> $TEST_LOG
echo "Failed: $FAILED_TESTS" >> $TEST_LOG
echo "Test completed at: $(date)" >> $TEST_LOG

echo "Total tests: $TOTAL_TESTS"
echo "Passed: $PASSED_TESTS"
echo "Failed: $FAILED_TESTS"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}All tests passed! 🎉${NC}"
    exit 0
else
    echo -e "${RED}$FAILED_TESTS test(s) failed. Check $TEST_LOG for details.${NC}"
    exit 1
fi