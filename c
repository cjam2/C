let
    // ===== CONFIG (tune these for speed) =====
    BaseUrl = "https://track.td.com/rest/api/2/search",
    BaseJQL = "resolution in (Unresolved, Done, Fixed, Duplicate, Cancelled, Completed, Complete) AND assignee in (TA8537, TA0037, TA0069)",

    // Presentation-friendly scope:
    LookbackDays = 90,        // change to 30/60/180 as needed
    WindowDays   = 30,        // bigger window = fewer API calls (reduce to 14 or 7 if you hit 1000 cap)
    PageSize     = 500,       // try 200/500 (higher = fewer calls)

    FieldsCsv = "summary,status,assignee,created,updated,customfield_24100,customfield_24101",
    // ========================================

    RangeEnd   = Date.AddDays(Date.From(DateTime.LocalNow()), 1),
    RangeStart = Date.AddDays(RangeEnd, -LookbackDays),

    Windows =
        List.Generate(
            () => RangeStart,
            each _ < RangeEnd,
            each Date.AddDays(_, WindowDays),
            each [
                From = _,
                To =
                    if Date.AddDays(_, WindowDays) < RangeEnd
                    then Date.AddDays(_, WindowDays)
                    else RangeEnd
            ]
        ),

    FetchPage = (fromDate as date, toDate as date, startAt as number) as record =>
        let
            ChunkJQL =
                BaseJQL
                & " AND updated >= """ & Date.ToText(fromDate, "yyyy-MM-dd") & """"
                & " AND updated < """  & Date.ToText(toDate,   "yyyy-MM-dd") & """"
                & " ORDER BY updated ASC",

            Response =
                Json.Document(
                    Web.Contents(
                        BaseUrl,
                        [
                            Query = [
                                jql = ChunkJQL,
                                startAt = Text.From(startAt),
                                maxResults = Text.From(PageSize),
                                fields = FieldsCsv
                            ]
                        ]
                    )
                )
        in
            Response,

    FetchWindowIssues = (fromDate as date, toDate as date) as list =>
        let
            Pages =
                List.Generate(
                    () => [StartAt = 0, R = FetchPage(fromDate, toDate, 0)],
                    each List.Count([R][issues]) > 0,
                    each [StartAt = [StartAt] + PageSize, R = FetchPage(fromDate, toDate, [StartAt] + PageSize)],
                    each [R][issues]
                ),
            Combined = List.Combine(Pages)
        in
            Combined,

    AllIssues =
        List.Combine(
            List.Transform(Windows, each FetchWindowIssues(_[From], _[To]))
        ),

    IssuesTable = Table.FromList(AllIssues, Splitter.SplitByNothing(), {"Issue"}, null, ExtraValues.Error),

    ExpandTop = Table.ExpandRecordColumn(IssuesTable, "Issue", {"key","fields"}, {"key","fields"}),

    ExpandFields = Table.ExpandRecordColumn(
        ExpandTop,
        "fields",
        {"summary","status","assignee","created","updated","customfield_24100","customfield_24101"},
        {"summary","status","assignee","created","updated","targetStartDate","targetEndDate"}
    ),

    ExpandStatus = Table.ExpandRecordColumn(ExpandFields, "status", {"name"}, {"status"}),

    SafeAssignee =
        Table.TransformColumns(
            ExpandStatus,
            {{"assignee", each if _ = null then [displayName=""] else _, type record}}
        ),
    ExpandAssignee = Table.ExpandRecordColumn(SafeAssignee, "assignee", {"displayName"}, {"assignee"}),

    // Deduplicate (good for window boundaries)
    Dedup = Table.Distinct(ExpandAssignee, {"key"})
in
    Dedup