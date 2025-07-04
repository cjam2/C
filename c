#!/bin/bash

jira_user="your_user"
jira_pass="your_pass"
main_ticket="JIRA-1234"
single_param="abc def ghi"
excluded_dates=("2025-07-04" "2025-08-05")  # Add custom skip dates here

# Computed national holidays (from above)
holidays=("2025-01-01" "2025-02-17" "2025-05-19" "2025-07-01" "2025-08-04" "2025-09-01" "2025-10-13" "2025-12-25" "2025-12-26")

# Combine all skip dates
skip_dates=("${holidays[@]}" "${excluded_dates[@]}")

should_skip() {
    local date="$1"
    for skip in "${skip_dates[@]}"; do
        if [[ "$skip" == "$date" ]]; then
            return 0
        fi
    done
    return 1
}

# Generate tickets for Mon–Fri this week
for i in {0..4}; do
    day_date=$(date -d "monday +$i days" +%Y-%m-%d)
    if should_skip "$day_date"; then
        echo "Skipping $day_date (holiday or excluded)"
        continue
    fi

    for task in $single_param; do
        summary="$day_date - $task - Daily Automation"
        description="Auto-generated subtask for $day_date."

        echo "Creating subtask for $day_date with summary: $summary"

        curl -u $jira_user:$jira_pass \
        -X POST --data "{
          \"fields\": {
            \"project\": { \"key\": \"PROJ\" },
            \"parent\": { \"key\": \"$main_ticket\" },
            \"summary\": \"$summary\",
            \"description\": \"$description\",
            \"issuetype\": { \"name\": \"Sub-task\" }
          }
        }" -H "Content-Type: application/json" \
        https://your-jira-url/rest/api/2/issue/
    done
done
0 0 * * 1
Holiday	Date
New Year's Day	2026-01-01
Family Day	2026-02-16
Victoria Day	2026-05-18
Canada Day	2026-07-01
Civic Holiday	2026-08-03
Labour Day	2026-09-07
Thanksgiving	2026-10-12
Christmas Day	2026-12-25
Boxing Day	2026-12-26







    name: Run Script with Server List from File

on:
  workflow_dispatch:
    inputs:
      ssh_user:
        description: "SSH Username"
        required: true
        default: "admin"
      ssh_pass:
        description: "SSH Password"
        required: true

jobs:
  run-script:
    runs-on: ubuntu-latest

    steps:
    - name: Checkout repo
      uses: actions/checkout@v3

    - name: Run script with servers from file
      run: |
        # Remove carriage returns and comments from the file
        SERVERS=$(grep -v '^#' params/servers.txt | tr -d '\r')

        echo "Running on the following servers:"
        echo "$SERVERS"

        # Optional: Write cleaned servers to temp file
        echo "$SERVERS" > servers-cleaned.txt

        chmod +x ./scripts/run.sh
        ./scripts/run.sh "$SERVERS" "${{ github.event.inputs.ssh_user }}" "${{ github.event.inputs.ssh_pass }}"

