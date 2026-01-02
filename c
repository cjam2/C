let
    BaseUrl = "https://track.td.com/rest/api/2/search",
    JQL = "YOUR_JQL_HERE",
    PageSize = 1000,

    // We’ll build the full ticket link from the key
    BrowseBase = "https://track.td.com/browse/",

    // Add more fields here (resolutiondate = resolved date in Jira Server/DC)
    FieldsCsv = "summary,status,assignee,created,updated,customfield_24100,customfield_24101,issuetype,project,resolutiondate,priority",

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

    // Build base table
    T0 = Table.FromList(AllIssues, Splitter.SplitByNothing(), {"Issue"}, null, ExtraValues.Error),

    // Expand key + fields
    T1 = Table.ExpandRecordColumn(T0, "Issue", {"key","fields"}, {"Key","fields"}),

    // Expand fields including project + issue type + resolutiondate + priority
    T2 = Table.ExpandRecordColumn(
        T1, "fields",
        {
            "summary","status","assignee","created","updated",
            "customfield_24100","customfield_24101",
            "issuetype","project","resolutiondate","priority"
        },
        {
            "Summary","status","assignee","Created","Updated",
            "TargetStartDate","TargetEndDate",
            "issuetype","project","ResolvedDate","priority"
        }
    ),

    // Expand status.name
    T3 = Table.ExpandRecordColumn(T2, "status", {"name"}, {"Status"}),

    // Assignee can be null
    SafeAssignee = Table.TransformColumns(T3, {{"assignee", each if _ = null then [displayName=""] else _, type record}}),
    T4 = Table.ExpandRecordColumn(SafeAssignee, "assignee", {"displayName"}, {"Assignee"}),

    // Expand issuetype.name
    T5 = Table.ExpandRecordColumn(T4, "issuetype", {"name"}, {"IssueType"}),

    // Expand project.name (and key if you want it too)
    T6 = Table.ExpandRecordColumn(T5, "project", {"name","key"}, {"ProjectName","ProjectKey"}),

    // Expand priority.name (optional but you had it earlier in examples)
    SafePriority = Table.TransformColumns(T6, {{"priority", each if _ = null then [name=""] else _, type record}}),
    T7 = Table.ExpandRecordColumn(SafePriority, "priority", {"name"}, {"Priority"}),

    // Add full ticket URL
    AddUrl = Table.AddColumn(T7, "TicketUrl", each BrowseBase & [Key], type text),

    // --- Date parsing helper columns (resolved date for quarter calc; fallback to Updated if resolved is null) ---
    // Jira date strings are like "2025-01-15T10:20:30.000-0500"
    AddResolvedDT = Table.AddColumn(
        AddUrl,
        "ResolvedDT",
        each try DateTimeZone.FromText([ResolvedDate]) otherwise null,
        type datetimezone
    ),

    AddUpdatedDT = Table.AddColumn(
        AddResolvedDT,
        "UpdatedDT",
        each try DateTimeZone.FromText([Updated]) otherwise null,
        type datetimezone
    ),

    // Choose date for quarter calc: resolved if exists else updated
    AddQuarterDate = Table.AddColumn(
        AddUpdatedDT,
        "QuarterDate",
        each if [ResolvedDT] <> null then DateTimeZone.RemoveZone([ResolvedDT]) else DateTimeZone.RemoveZone([UpdatedDT]),
        type datetime
    ),

    // Compute fiscal quarter and fiscal year label based on your rules:
    // Q1: Nov-Jan (year = year of Jan, i.e., Nov/Dec belong to next year's Q1)
    // Q2: Feb-Apr
    // Q3: May-Jul
    // Q4: Aug-Oct
    AddQuarter = Table.AddColumn(
        AddQuarterDate,
        "Quarter",
        each
            let
                d = Date.From([QuarterDate]),
                m = Date.Month(d),
                y = Date.Year(d),
                FiscalYear = if m = 11 or m = 12 then y + 1 else y,
                Q =
                    if m = 11 or m = 12 or m = 1 then "Q1"
                    else if m >= 2 and m <= 4 then "Q2"
                    else if m >= 5 and m <= 7 then "Q3"
                    else "Q4"
            in
                Text.From(FiscalYear) & " " & Q,
        type text
    ),

    // Clean up helper columns
    Cleanup = Table.RemoveColumns(AddQuarter, {"ResolvedDT","UpdatedDT","QuarterDate"})
in
    Cleanup