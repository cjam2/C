curl -s \
  -X POST "${JIRA_SEARCH_URL}" \
  -H "Authorization: Bearer ${JIRA_TOKEN}" \
  -H "Content-Type: application/json" \
  -H "Accept: application/json" \
  -d "{
    \"jql\": \"${jql}\",
    \"maxResults\": 100,
    \"startAt\": 0,
    \"fields\": [\"*all\"],
    \"expand\": [\"names\",\"schema\"]
  }" > jira.json