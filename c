#!/bin/bash

# These should come from Jenkins Choice Parameters
# Example:
# TEST_SET=A1
# APP=a
# Jenkins will pass these as environment variables

# Declare the associative array of hardcoded IPs
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

# Combine the Jenkins parameters
key="${TEST_SET}_${APP}"

# Check and print the result
if [[ -n "${servers[$key]}" ]]; then
  echo "Selected Environment: $TEST_SET"
  echo "Selected Application: $APP"
  echo "Server IP: ${servers[$key]}"
else
  echo "Invalid combination: $TEST_SET and $APP not found."
  exit 1
fi