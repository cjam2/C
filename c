#!/bin/sh

# Usage: ./jira_create_subtask.sh ABC-123
PARENT_TICKET=$1

# Set your Jira variables
JIRA_DOMAIN="your-domain.atlassian.net"
JIRA_URL="https://$JIRA_DOMAIN"
JIRA_PAT="your_actual_PAT_here"

# Subtask configuration
SUMMARY="Subtask created from mainframe"
DESCRIPTION="Automatically created via PAT-based script"
SUBTASK_TYPE_ID="10002"     # Replace with actual subtask issue type ID
DONE_TRANSITION_ID="31"     # Replace with actual transition ID for 'Done'

# Authorization header using PAT
AUTH_HEADER="Authorization: Bearer $JIRA_PAT"

# ---- Step 1: Create subtask ----
CREATE_RESPONSE=$(curl -s -X POST "$JIRA_URL/rest/api/2/issue" \
  -H "$AUTH_HEADER" \
  -H "Content-Type: application/json" \
  -d "{
    \"fields\": {
      \"project\": { \"key\": \"${PARENT_TICKET%%-*}\" },
      \"parent\": { \"key\": \"$PARENT_TICKET\" },
      \"summary\": \"$SUMMARY\",
      \"description\": \"$DESCRIPTION\",
      \"issuetype\": { \"id\": \"$SUBTASK_TYPE_ID\" }
    }
  }")

SUBTASK_KEY=$(echo "$CREATE_RESPONSE" | grep -o '"key":"[^"]*' | cut -d':' -f2 | tr -d '"')

if [ -z "$SUBTASK_KEY" ]; then
  echo "❌ Failed to create subtask:"
  echo "$CREATE_RESPONSE"
  exit 1
fi

echo "✅ Subtask created: $SUBTASK_KEY"

# ---- Step 2: Add comment ----
curl -s -X POST "$JIRA_URL/rest/api/2/issue/$SUBTASK_KEY/comment" \
  -H "$AUTH_HEADER" \
  -H "Content-Type: application/json" \
  -d "{
    \"body\": \"Subtask created and resolved using Jira PAT from z/OS.\"
  }"

echo "✅ Comment added"

# ---- Step 3: Transition to Done ----
curl -s -X POST "$JIRA_URL/rest/api/2/issue/$SUBTASK_KEY/transitions" \
  -H "$AUTH_HEADER" \
  -H "Content-Type: application/json" \
  -d "{
    \"transition\": {
      \"id\": \"$DONE_TRANSITION_ID\"
    }
  }"

echo "✅ Subtask transitioned to Done"
