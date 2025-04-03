#!/bin/bash

# These come from Jenkins as comma-separated strings
# TEST_SET="A1,b6"
# APP="a,b"

# SSH credentials
USER="your_ssh_user"
WGET_LINK="http://example.com/path/to/file"  # Replace with actual URL

# Declare server map
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

# Output filenames per app
declare -A output_names
output_names[a]="aaa-web"
output_names[b]="bbb-web"
output_names[c]="ccc-web"

# Parse comma-separated inputs
IFS=',' read -ra test_sets <<< "$TEST_SET"
IFS=',' read -ra apps <<< "$APP"

# Loop through combinations
for env in "${test_sets[@]}"; do
  for app in "${apps[@]}"; do
    key="${env}_${app}"
    server="${servers[$key]}"
    output_file="${output_names[$app]}"

    if [[ -n "$server" && -n "$output_file" ]]; then
      echo "Connecting to $server for $env app $app"
      ssh "$USER@$server" "wget $WGET_LINK -O $output_file"
    else
      echo "Error: Missing server or output file for $key"
    fi
  done
done