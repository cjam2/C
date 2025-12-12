#!/bin/bash

URLS="$1"

OUTPUT_FILE="/home/$USER/temporary/$(hostname -i)_cmd_output_$(date +%Y%m%d_%H%M%S).txt"

echo "Output file in the same folder where script runs" | tee -a "$OUTPUT_FILE"

# --------------------------------------------------
# Convert URLS string → COMMANDS array
# Supports:
#   - multiline GitHub input
#   - single line separated by ||
# --------------------------------------------------

URLS="${URLS//$'\r'/}"   # strip Windows CR

declare -a COMMANDS=()

if [[ "$URLS" == *"||"* ]]; then
  # single-line input: cmd1||cmd2||cmd3
  mapfile -t COMMANDS < <(printf '%s' "$URLS" | sed 's/||/\n/g')
else
  # multiline input
  mapfile -t COMMANDS <<< "$URLS"
fi

# Remove empty / whitespace-only commands
CLEAN_COMMANDS=()
for cmd in "${COMMANDS[@]}"; do
  cmd="${cmd#"${cmd%%[![:space:]]*}"}"
  cmd="${cmd%"${cmd##*[![:space:]]}"}"
  [[ -z "$cmd" ]] && continue
  CLEAN_COMMANDS+=("$cmd")
done
COMMANDS=("${CLEAN_COMMANDS[@]}")

# --------------------------------------------------
# Run commands one-by-one (with separation restored)
# --------------------------------------------------

for cmd in "${COMMANDS[@]}"; do
  echo "------------------------------------------" | tee -a "$OUTPUT_FILE"
  echo "Running: $cmd" | tee -a "$OUTPUT_FILE"

  eval "$cmd" >> "$OUTPUT_FILE" 2>&1

  echo "Command finished" | tee -a "$OUTPUT_FILE"
done

echo "All commands finished. Results saved to $OUTPUT_FILE"