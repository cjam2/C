USER="$1"
PASS="$2"
URLS_RAW="$3"

IFS='||' read -r -a CMDS <<< "$URLS_RAW"

for cmd in "${CMDS[@]}"; do
  cmd="${cmd#"${cmd%%[![:space:]]*}"}"  # trim left
  cmd="${cmd%"${cmd##*[![:space:]]}"}"  # trim right
  [[ -z "$cmd" ]] && continue
  echo "Running: $cmd"
  eval "$cmd"
done