#!/bin/bash

input_file="example.json"

# Grep pattern: match the whole string and extract the word using regex
matches=($(grep -oP 'abc tracker - \d{4}-\d{2}-\d{2} - \K\w+(?= dip)' "$input_file"))

# Output the array elements
for word in "${matches[@]}"; do
  echo "Captured word: $word"
done