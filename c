let
  // =========================
  //  A) SOURCE (your CSV URL)
  // =========================
  SourceCsvUrl = "<<< EDIT ME: paste your CSV export URL here >>>",

  CsvRaw = Csv.Document(
            Web.Contents(SourceCsvUrl),
            [Delimiter=",", Encoding=65001, QuoteStyle=QuoteStyle.Csv]
          ),
  Promote = Table.PromoteHeaders(CsvRaw, [PromoteAllScalars=true]),
  Trimmed = Table.TransformColumns(
              Promote,
              List.Transform(Table.ColumnNames(Promote), each {_, (x)=> if Value.Is(x, type text) then Text.Trim(x) else x, type any})
            ),

  // ===========================================
  //  B) CONFIG — date field + fiscal quarters
  // ===========================================
  // <<< EDIT ME: choose the date column you want to bucket by >>>
  // Common choices: "sys_created_on", "start_date", "planned_start_date", "work_start"
  DateFieldName = "sys_created_on",

  // Parse the chosen date column (handles text or already-date)
  WithDate =
    let
      HasColumn = List.Contains(Table.ColumnNames(Trimmed), DateFieldName),
      Step       = if HasColumn then
                      Table.TransformColumns(
                        Trimmed,
                        {{DateFieldName,
                          each try DateTime.From(_) otherwise try Date.From(_) otherwise null,
                          type datetime}}
                      )
                   else Trimmed
    in
      Step,

  // FY25 boundaries
  FY25_Q1_Start = #datetime(2024,11,1,0,0,0),
  FY25_Q1_End   = #datetime(2025,1,31,23,59,59),
  FY25_Q2_Start = #datetime(2025,2,1,0,0,0),
  FY25_Q2_End   = #datetime(2025,4,30,23,59,59),
  FY25_Q3_Start = #datetime(2025,5,1,0,0,0),
  FY25_Q3_End   = #datetime(2025,7,31,23,59,59),
  FY25_Q4_Start = #datetime(2025,8,1,0,0,0),
  FY25_Q4_End   = #datetime(2025,10,31,23,59,59),

  QuarterFromDate = (d as nullable datetime) as nullable text =>
    if d = null then null
    else if d >= FY25_Q1_Start and d <= FY25_Q1_End then "Q1"
    else if d >= FY25_Q2_Start and d <= FY25_Q2_End then "Q2"
    else if d >= FY25_Q3_Start and d <= FY25_Q3_End then "Q3"
    else if d >= FY25_Q4_Start and d <= FY25_Q4_End then "Q4"
    else null,

  AddQuarter = Table.AddColumn(WithDate, "FY25_QTR", each QuarterFromDate(Record.Field(_, DateFieldName)), type text),
  KeepFY25   = Table.SelectRows(AddQuarter, each [FY25_QTR] <> null),

  // Normalize key columns we’ll filter on
  ToText = (tbl as table, col as text) as table =>
              if List.Contains(Table.ColumnNames(tbl), col)
              then Table.TransformColumns(tbl, {{col, each if _ = null then null else Text.From(_), type text}})
              else tbl,

  Norm1 = ToText(KeepFY25, "u_environment"),
  Norm2 = ToText(Norm1, "assignment_group"),
  Norm3 = ToText(Norm2, "number"),   // ticket id column (optional but helps for distinct counts)

  // ==========================================================
  //  C) CATEGORY MAP — add/extend rows to define your report
  //      Each row = how to count a line in the final report
  // ==========================================================
  // You can duplicate/modify rows below to add more categories.
  CategoryMap = Table.FromRecords({
    // Example 1 (your explicit rule):
    // "ARE Release - PAT COs" = assignment_group exactly "ITS-IDE-EP-ARM-AppDeploy" AND u_environment = "PAT"
    [Category = "ARE Release - PAT COs",  GroupExact = "ITS-IDE-EP-ARM-AppDeploy", EnvList = {"PAT"}],

    // Example 2:
    [Category = "ARE Release - PROD COs", GroupExact = "ITS-IDE-EP-ARM-AppDeploy", EnvList = {"PROD"}],

    // Example 3 (DR or DRP environments treated together):
    [Category = "ARE Release - DR/DRP COs", GroupExact = "ITS-IDE-EP-ARM-AppDeploy", EnvList = {"DR","DRP"}]

    // 👉 Add more rows here (e.g., Automation, Onboarding, NFT, DBA, etc.)
    // [Category = "ARE Automation - PAT COs",  GroupExact = "ITS-IDE-EP-ARM-Automation", EnvList = {"PAT"}],
    // [Category = "DBA - PROD COs",            GroupExact = "ITS-IDE-EP-ARM-DBA",        EnvList = {"PROD"}]
  }),

  // ====================================================
  //  D) FILTER + COUNT per Category x Quarter (FY25)
  // ====================================================
  // For each Category row, filter the data then count tickets by quarter.
  CountForCategory = (cat as text, groupExact as text, envList as list) as table =>
    let
      Filtered =
        Table.SelectRows(
          Norm3,
          (r) =>
            // assignment_group equals (case-insensitive)
            let g = try Text.From(r[assignment_group]) otherwise null in
            (g <> null and Text.Upper(g) = Text.Upper(groupExact))
            and
            // u_environment is one of EnvList (case-insensitive)
            let e = try Text.From(r[u_environment]) otherwise null in
            (e <> null and List.Contains(List.Transform(envList, each Text.Upper(_)), Text.Upper(e)))
        ),

      // Prefer distinct count of "number" if present, else row count
      HasNumber = List.Contains(Table.ColumnNames(Filtered), "number"),
      BaseTable = if HasNumber then Table.SelectColumns(Filtered, {"number", "FY25_QTR"}) else Table.SelectColumns(Filtered, {"FY25_QTR"}),

      DistinctBase = if HasNumber then Table.Distinct(BaseTable) else BaseTable,

      Grouped = Table.Group(DistinctBase, {"FY25_QTR"}, {{"Count", each Table.RowCount(_), Int64.Type}}),

      // Ensure all quarters exist (fill missing with 0)
      AllQtrs = #table({"FY25_QTR"}, {{"Q1"},{"Q2"},{"Q3"},{"Q4"}}),
      JoinQ   = Table.Join(AllQtrs, "FY25_QTR", Grouped, "FY25_QTR", JoinKind.LeftOuter),
      Filled0 = Table.ReplaceValue(JoinQ, null, 0, Replacer.ReplaceValue, {"Count"}),

      Pivoted = Table.Pivot(Filled0, List.Distinct(Filled0[FY25_QTR]), "FY25_QTR", "Count")
    in
      Pivoted,

  // Build a single table by iterating the category map
  WithCounts =
    Table.AddColumn(
      CategoryMap, "Counts",
      each CountForCategory([Category], [GroupExact], [EnvList])
    ),

  Expanded =
    let
      // expand Q1..Q4 (which may or may not exist yet)
      Temp = Table.ExpandTableColumn(WithCounts, "Counts", {"Q1","Q2","Q3","Q4"}, {"Q1","Q2","Q3","Q4"}),
      // Replace nulls with 0s
      Filled = Table.ReplaceValue(Temp, null, 0, Replacer.ReplaceValue, {"Q1","Q2","Q3","Q4"})
    in
      Filled,

  // Row total
  WithTotal = Table.AddColumn(Expanded, "FY25 Total", each [Q1]+[Q2]+[Q3]+[Q4], Int64.Type),

  // Output columns (hide config columns)
  Final = Table.SelectColumns(WithTotal, {"Category","Q1","Q2","Q3","Q4","FY25 Total"})
in
  Final








---------




[Category = "ARE Automation - PAT COs", GroupExact = "ITS-IDE-EP-ARM-Automation", EnvList = {"PAT"}],
[Category = "ARE Automation - PROD COs", GroupExact = "ITS-IDE-EP-ARM-Automation", EnvList = {"PROD"}],
[Category = "ARE Automation / Onboarding - PAT COs", GroupExact = "ITS-IDE-EP-ARM-Onboarding", EnvList = {"PAT"}],
[Category = "NFT - PAT COs", GroupExact = "ITS-IDE-EP-ARM-NFT", EnvList = {"PAT"}],
[Category = "DBA - DR/DRP COs", GroupExact = "ITS-IDE-EP-ARM-DBA", EnvList = {"DR","DRP"}],