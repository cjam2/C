let
    BaseUrl = "https://track.td.com/rest/api/2/search",
    JQL = "resolution in (Unresolved, Done, Fixed, Duplicate, Cancelled, Completed, Complete) AND assignee in (TA8537, TA0037, TA0069)",

    Source =
        Json.Document(
            Web.Contents(
                BaseUrl,
                [
                    Query = [
                        jql = JQL,
                        maxResults = "100",
                        startAt = "0",
                        fields = "summary,status,assignee,priority,issuetype,created,updated,customfield_24100,customfield_24101"
                    ]
                ]
            )
        ),

    Issues = Source[issues],
    IssuesTable = Table.FromList(Issues, Splitter.SplitByNothing(), {"Issue"}, null, ExtraValues.Error),

    ExpandTop = Table.ExpandRecordColumn(IssuesTable, "Issue", {"id","key","self","fields"}, {"id","key","self","fields"}),

    ExpandFields = Table.ExpandRecordColumn(
        ExpandTop,
        "fields",
        {"summary","status","assignee","priority","issuetype","created","updated","customfield_24100","customfield_24101"},
        {"summary","status","assignee","priority","issuetype","created","updated","targetStartDate","targetEndDate"}
    ),

    ExpandStatus = Table.ExpandRecordColumn(ExpandFields, "status", {"name"}, {"status"}),

    ExpandAssignee = Table.ExpandRecordColumn(ExpandStatus, "assignee", {"displayName","name","emailAddress"}, {"assignee","assigneeUser","assigneeEmail"}),

    ExpandPriority = Table.ExpandRecordColumn(ExpandAssignee, "priority", {"name"}, {"priority"}),

    ExpandType = Table.ExpandRecordColumn(ExpandPriority, "issuetype", {"name"}, {"issueType"})
in
    ExpandType