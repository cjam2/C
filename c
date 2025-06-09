curl -H "Authorization: Basic $(echo -n "$JIRA_USER:$JIRA_TOKEN" | base64)" ...
curl -H "Authorization: Bearer $JIRA_TOKEN" \
     -H "Content-Type: application/json" \
     --data '{ ... }' \
     https://your-domain.atlassian.net/rest/api/2/issue/
