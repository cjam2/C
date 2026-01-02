let
    // ====== CONFIG ======
    BaseUrl = "https://track.td.com/rest/api/2/search",
    BaseJQL = "resolution in (Unresolved, Done, Fixed, Duplicate, Cancelled, Completed, Complete) AND assignee in (TA8537)",

    // Date windowing to avoid Jira 1000-cap per query:
    // If a window still returns too many, reduce WindowDays (e.g., 1).
    WindowDays = 7,

    // Set an overall date range you care about (adjust as needed)
    RangeStart = Date.AddMonths(Date.From(DateTime.LocalNow()), -12),
    RangeEnd   = Date.AddDays(Date.From(DateTime.LocalNow()), 1),

    PageSize = 100,

    FieldsCsv = "summary,status,assignee,priority,issuetype,created,updated,customfield_24100,customfield_24101",
    // ====================

    // Build list of [from,to) windows
    Windows =
        List.Generate(
            () => RangeStart,
            each _ < RangeEnd,
            each Date.AddDays(_, WindowDays),
            each [From = _, To = Date.Min(Date.AddDays(_, WindowDays), RangeEnd)]
        ),

    // Function: fetch one page for a given window
    FetchPage = (fromDate as date, toDate as date, startAt as number) as record =>
        let
            // Chunk by UPDATED date (change "updated" to "created" if preferred)
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

    // Function: fetch ALL issues for a given window (paginate until empty)
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

    // Pull issues for all windows and combine
    AllIssues =
        List.Combine(
            List.Transform(Windows, each FetchWindowIssues(_[From], _[To]))
        ),

    // Convert to table + expand like your working output
    IssuesTable = Table.FromList(AllIssues, Splitter.SplitByNothing(), {"Issue"}, null, ExtraValues.Error),

    ExpandTop = Table.ExpandRecordColumn(IssuesTable, "Issue", {"id","key","self","fields"}, {"id","key","self","fields"}),

    ExpandFields = Table.ExpandRecordColumn(
        ExpandTop,
        "fields",
        {"summary","status","assignee","priority","issuetype","created","updated","customfield_24100","customfield_24101"},
        {"summary","status","assignee","priority","issuetype","created","updated","targetStartDate","targetEndDate"}
    ),

    ExpandStatus = Table.ExpandRecordColumn(ExpandFields, "status", {"name"}, {"status"}),

    // Assignee can be null; convert null to empty record before expanding
    SafeAssignee =
        Table.TransformColumns(
            ExpandStatus,
            {{"assignee", each if _ = null then [displayName="", name="", emailAddress=""] else _, type record}}
        ),

    ExpandAssignee = Table.ExpandRecordColumn(SafeAssignee, "assignee", {"displayName","name","emailAddress"}, {"assignee","assigneeUser","assigneeEmail"}),

    ExpandPriority = Table.ExpandRecordColumn(ExpandAssignee, "priority", {"name"}, {"priority"}),

    ExpandType = Table.ExpandRecordColumn(ExpandPriority, "issuetype", {"name"}, {"issueType"}),

    // Optional: remove duplicates in case of edge overlaps (should be minimal with [from,to) windows)
    Dedup = Table.Distinct(ExpandType, {"key"})
in
    Dedup