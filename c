let
    BaseUrl = "https://YOUR_JIRA_URL/rest/api/3/search",
    JQL = "project = ABC AND status != Done ORDER BY updated DESC",

    Source =
        Json.Document(
            Web.Contents(
                BaseUrl,
                [
                    Query = [
                        jql = JQL,
                        maxResults = "100",
                        startAt = "0",
                        fields = "summary,status,assignee,priority,issuetype,created,updated"
                    ]
                ]
            )
        )
in
    Source