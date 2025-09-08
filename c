#!/bin/bash

# Output file in the same folder where script runs
OUTPUT_FILE="$(pwd)/cmd_output_$(date +%Y%m%d_%H%M%S).log"

# List of commands to run
COMMANDS=(
    "curl --max-time 5 telnet://host1:1234"
    "curl --max-time 5 telnet://host2:5678"
    "curl --max-time 5 telnet://host3:9101"
)

# Loop through each command
for cmd in "${COMMANDS[@]}"; do
    echo "----------------------------------------" | tee -a "$OUTPUT_FILE"
    echo "Running: $cmd" | tee -a "$OUTPUT_FILE"
    echo "----------------------------------------" | tee -a "$OUTPUT_FILE"
    
    # Run the command and capture output
    eval "$cmd" >> "$OUTPUT_FILE" 2>&1
    
    # Simulate Ctrl+C (kill if needed)
    echo -e "\n[Command finished, simulating exit]\n" | tee -a "$OUTPUT_FILE"
done

echo "All commands finished. Results saved to: $OUTPUT_FILE"