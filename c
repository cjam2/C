# Extract Subtask Key safely
SUBTASK_KEY=$(echo "$CREATE_RESPONSE" | grep -o '"key":"[^"]*"' | cut -d':' -f2 | tr -d '"')

if [ -z "$SUBTASK_KEY" ]; then
  echo "❌ Failed to extract subtask key for \"${VAR}\""
  echo "$CREATE_RESPONSE"
  continue
fi