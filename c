#!/bin/bash

# Jenkins should pass these as comma-separated values
# Example:
# TEST_SET="A1,b6"
# APP="a,b"

# Declare the associative array
declare -A servers

servers[A1_a]="192.168.1.10"
servers[A1_b]="192.168.1.11"
servers[A1_c]="192.168.1.12"

servers[b6_a]="10.0.0.10"
servers[b6_b]="10.0.0.11"
servers[b6_c]="10.0.0.12"

servers[f6_a]="172.16.0.10"
servers[f6_b]="172.16.0.11"
servers[f6_c]="172.16.0.12"

# Split the comma-separated values into arrays
IFS=',' read -ra test_sets <<< "$TEST_SET"
IFS=',' read -ra apps <<< "$APP"

echo "Selected Environments: ${test_sets[*]}"
echo "Selected Applications: ${apps[*]}"
echo

# Loop through all combinations
for env in "${test_sets[@]}"; do
  for app in "${apps[@]}"; do
    key="${env}_${app}"
    if [[ -n "${servers[$key]}" ]]; then
      echo "$env - App $app -> Server: ${servers[$key]}"
    else
      echo "Warning: No server found for $key"
    fi
  done
done