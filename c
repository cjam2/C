#!/bin/bash

# --------- Input Parameters ---------
PARENT_TICKET=$1       # e.g., ABC-123
ASSIGNEE=$2            # Optional (e.g., john.doe)
VARIABLE=$3            # e.g., "abc bcd jkl"

# --------- Jira Configuration ---------
JIRA_DOMAIN="your-domain.atlassian.net"   # 🔁 Replace this
JIRA_PAT="your_jira_pat_here"             # 🔁 Replace this
JIRA_API_URL="https://${JIRA_DOMAIN}/rest/api/2"
AUTH_HEADER="Authorization: Bearer ${JIRA_PAT}"

SUBTASK_TYPE_ID="10002"    # 🔁 Replace with your sub-task type ID
DONE_TRANSITION_ID="31"    # 🔁 Replace with your 'Done' transition ID

# --------- Helper Function to Escape JSON ---------
json_escape() {
  echo "$1" | sed -e 's/\\/\\\\/g' -e 's/"/\\"/g'
}

# Get today's date in yyyy-mm-dd format
TODAY_DATE=$(date +'%Y-%m-%d')

# --------- Loop Through Variables ---------
for VAR in $VARIABLE; do
  SUMMARY="${TODAY_DATE} - ${VAR} - today is good"
  DESCRIPTION="Automatically created via shell script, for ${VAR}"

  SUMMARY_ESCAPED=$(json_escape "$SUMMARY")
  DESCRIPTION_ESCAPED=$(json_escape "$DESCRIPTION")

  echo "➡️ Creating subtask for \"$VAR\"..."

  # --------- Create Subtask ---------
  CREATE_RESPONSE=$(curl -s -X POST "${JIRA_API_URL}/issue" \
    -H "${AUTH_HEADER}" \
    -H "Content-Type: application/json" \
    -d "{
      \"fields\": {
        \"project\": { \"key\": \"${PARENT_TICKET%%-*}\" },
        \"parent\": { \"key\": \"${PARENT_TICKET}\" },
        \"summary\": \"${SUMMARY_ESCAPED}\",
        \"description\": \"${DESCRIPTION_ESCAPED}\",
        \"issuetype\": { \"id\": \"${SUBTASK_TYPE_ID}\" }$( [ -n "$ASSIGNEE" ] && echo ",\"assignee\": { \"name\": \"${ASSIGNEE}\" }" )
      }
    }")

  # --------- Extract Subtask Key ---------
  SUBTASK_KEY=$(echo "$CREATE_RESPONSE" | sed "s/.*\"key\":\"[^\"]*\".*/\1/")

  if [ -z "$SUBTASK_KEY" ]; then
    echo "❌ Failed to create subtask for \"${VAR}\""
    echo "$CREATE_RESPONSE"
    continue
  fi

  echo "✅ Subtask created: $SUBTASK_KEY"

  # --------- Add Comment ---------
  curl -s -X POST "${JIRA_API_URL}/issue/${SUBTASK_KEY}/comment" \
    -H "${AUTH_HEADER}" \
    -H "Content-Type: application/json" \
    -d "{
      \"body\": \"Subtask created and resolved via shell script for ${VAR}\"
    }"

  echo "✅ Comment added"

  # --------- Transition to Done ---------
  curl -s -X POST "${JIRA_API_URL}/issue/${SUBTASK_KEY}/transitions" \
    -H "${AUTH_HEADER}" \
    -H "Content-Type: application/json" \
    -d "{
      \"transition\": { \"id\": \"${DONE_TRANSITION_ID}\" }
    }"

  echo "✅ Subtask transitioned to Done"
done