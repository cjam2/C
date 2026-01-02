let
    BaseUrl = "https://track.td.com/rest/api/2/search",
    JQL = "YOUR_JQL_HERE",
    PageSize = 1000,
    FieldsCsv = "summary,status,assignee,created,updated,customfield_24100,customfield_24101",

    FetchPage = (startAt as number) as record =>
        Json.Document(
            Web.Contents(
                BaseUrl,
                [
                    Query = [
                        jql = JQL,
                        startAt = Text.From(startAt),
                        maxResults = Text.From(PageSize),
                        fields = FieldsCsv
                    ]
                ]
            )
        ),

    // get total once
    First = FetchPage(0),
    Total = First[total],

    Pages =
        List.Generate(
            () => [S = 0, R = First],
            each [S] < Total and List.Count([R][issues]) > 0,
            each [S = [S] + PageSize, R = FetchPage([S] + PageSize)],
            each [R][issues]
        ),

    AllIssues = List.Combine(Pages),

    T0 = Table.FromList(AllIssues, Splitter.SplitByNothing(), {"Issue"}, null, ExtraValues.Error),
    T1 = Table.ExpandRecordColumn(T0, "Issue", {"key","fields"}, {"key","fields"}),
    T2 = Table.ExpandRecordColumn(
        T1, "fields",
        {"summary","status","assignee","created","updated","customfield_24100","customfield_24101"},
        {"summary","status","assignee","created","updated","targetStartDate","targetEndDate"}
    ),
    T3 = Table.ExpandRecordColumn(T2, "status", {"name"}, {"status"}),
    SafeAssignee = Table.TransformColumns(T3, {{"assignee", each if _ = null then [displayName=""] else _, type record}}),
    T4 = Table.ExpandRecordColumn(SafeAssignee, "assignee", {"displayName"}, {"assignee"})
in
    T4