let
    BaseUrl = "https://track.td.com/rest/api/2/search",
    JQL = "YOUR_JQL_HERE",
    Source =
        Json.Document(
            Web.Contents(
                BaseUrl,
                [
                    Query = [
                        jql = JQL,
                        startAt = "1000",
                        maxResults = "1",
                        fields = "summary"
                    ]
                ]
            )
        )
in
    Source