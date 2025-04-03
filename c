#!/bin/bash

# Input: comma-separated values
ENV_SELECTION="$1"    # e.g., "A1,A2"
MALCODES="$2"         # e.g., "a,c"

# Convert to arrays
IFS=',' read -ra ENVS <<< "$ENV_SELECTION"
IFS=',' read -ra MALCODES <<< "$MALCODES"

# Map of env+malcode to IP
declare -A IP_MAP=(
  ["A1-a"]="10.0.1.1"
  ["A1-b"]="10.0.1.2"
  ["A1-c"]="10.0.1.3"
  ["A2-a"]="10.0.2.1"
  ["A2-b"]="10.0.2.2"
  ["A2-c"]="10.0.2.3"
  ["A3-a"]="10.0.3.1"
  ["A3-b"]="10.0.3.2"
  ["A3-c"]="10.0.3.3"
)

# Loop through all env-malcode combinations
for env in "${ENVS[@]}"; do
  for mal in "${MALCODES[@]}"; do
    key="${env}-${mal}"
    ip="${IP_MAP[$key]}"
    if [ -n "$ip" ]; then
      echo "Deploying malcode '$mal' to environment '$env' at IP: $ip"
      # Your actual command here (example below):
      ssh user@"$ip" "wget http://server/path/to/$mal && sudo systemctl restart app"
    else
      echo "No IP configured for combination $key"
    fi
  done
done