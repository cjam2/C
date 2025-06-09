#!/bin/sh
PARENT_TICKET=$1
JIRA_USER=your_user
JIRA_PASS=your_token
JIRA_URL=https://your-domain.atlassian.net
SUBTASK_TYPE=10002
DONE_TRANSITION_ID=31

response=$(curl -s -u $JIRA_USER:$JIRA_PASS -X POST \
  -H "Content-Type: application/json" \
  -d '{
    "fields": {
      "project": { "key": "'${PARENT_TICKET%%-*}'" },
      "parent": { "key": "'$PARENT_TICKET'" },
      "summary": "Subtask from Mainframe",
      "issuetype": { "id": "'$SUBTASK_TYPE'" }
    }
  }' $JIRA_URL/rest/api/2/issue/)

SUBTASK_KEY=$(echo $response | grep -o '"key":"[^"]*' | cut -d':' -f2 | tr -d '"')

curl -s -u $JIRA_USER:$JIRA_PASS -X POST \
  -H "Content-Type: application/json" \
  -d "{\"body\": \"Subtask created and auto-resolved from mainframe.\"}" \
  $JIRA_URL/rest/api/2/issue/$SUBTASK_KEY/comment

curl -s -u $JIRA_USER:$JIRA_PASS -X POST \
  -H "Content-Type: application/json" \
  -d "{\"transition\": { \"id\": \"$DONE_TRANSITION_ID\" }}" \
  $JIRA_URL/rest/api/2/issue/$SUBTASK_KEY/transitions
