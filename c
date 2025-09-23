Option Explicit

' Refreshes the cTasks assigned to the user in Config!B5 (email),
' pulling a CSV export from ServiceNow and loading into Sheet "cTasks".
Public Sub RefreshCTasks()
    Dim inst As String, user As String, pass As String, email As String
    Dim onlyActive As Boolean
    Dim url As String, q As String, csv As String
    Dim http As Object
    Dim ws As Worksheet, cfg As Worksheet
    Dim tmpPath As String, tmpFile As String
    Dim fldList As String
    
    On Error GoTo fail
    
    Set cfg = ThisWorkbook.Worksheets("Config")
    Set ws = ThisWorkbook.Worksheets("cTasks")
    
    inst = Trim(cfg.Range("B2").Value)          ' https://yourinstance.service-now.com
    user = Trim(cfg.Range("B3").Value)
    pass = CStr(cfg.Range("B4").Value)
    email = Trim(cfg.Range("B5").Value)
    onlyActive = UCase$(Trim(cfg.Range("B6").Value)) = "TRUE"
    
    If inst = "" Or user = "" Or pass = "" Or email = "" Then
        MsgBox "Please fill InstanceURL, Username, Password, and YourEmail in Config sheet.", vbExclamation
        Exit Sub
    End If
    
    ' ----- Build encoded query -----
    ' We filter by assigned_to.email plus (optional) active=true.
    ' You can add more filters if you like (e.g., stateNOT IN=Closed, etc.)
    q = "assigned_to.email=" & URLEncode(email)
    If onlyActive Then q = q & "^active=true"
    
    ' Fields to export (display values where meaningful)
    ' Note: list export supports CSV via change_task_list.do?CSV
    ' Add/remove fields as you prefer.
    fldList = "number,short_description,state,priority,assigned_to,change_request,opened_at,due_date,work_notes_list,sys_id"
    
    ' Construct CSV export URL (list export)
    ' sysparm_display_value=true to get readable names for references/enums.
    url = inst & "/change_task_list.do?CSV" & _
          "&sysparm_query=" & q & _
          "&sysparm_display_value=true" & _
          "&sysparm_fields=" & Replace(fldList, " ", "")
    
    ' ----- Make request -----
    Set http = CreateObject("WinHttp.WinHttpRequest.5.1")
    http.Open "GET", url, False
    http.SetCredentials user, pass, 0 ' BASIC auth
    http.SetRequestHeader "Accept", "text/csv"
    http.Send
    
    If http.Status < 200 Or http.Status >= 300 Then
        Err.Raise vbObjectError + 513, , "ServiceNow request failed. HTTP " & http.Status & " - " & http.StatusText & vbCrLf & http.ResponseText
    End If
    
    csv = http.ResponseText
    If Len(csv) = 0 Then
        MsgBox "No data returned. Check filters/credentials.", vbInformation
        Exit Sub
    End If
    
    ' ----- Drop CSV to a temp file and import cleanly -----
    tmpPath = Environ$("TEMP")
    If Right$(tmpPath, 1) <> "\" Then tmpPath = tmpPath & "\"
    tmpFile = tmpPath & "servicenow_ctasks.csv"
    
    With CreateObject("Scripting.FileSystemObject").OpenTextFile(tmpFile, 2, True, -1)
        .Write csv
        .Close
    End With
    
    ' Clear & load into cTasks sheet
    ws.Cells.Clear
    
    With ws.QueryTables.Add(Connection:="TEXT;" & tmpFile, Destination:=ws.Range("A1"))
        .TextFileParseType = xlDelimited
        .TextFileCommaDelimiter = True
        .TextFileUnicode = True
        .TextFileTrailingMinusNumbers = True
        .AdjustColumnWidth = True
        .TextFileColumnDataTypes = Array(2) ' treat as text where needed
        .Refresh BackgroundQuery:=False
        .Delete
    End With
    
    ' Add a direct URL to each record (if sys_id present)
    Dim lastRow As Long, sysIdCol As Long, urlCol As Long, hdr As Range, f As Range
    lastRow = ws.Cells(ws.Rows.Count, "A").End(xlUp).Row
    
    ' Find sys_id column
    Set hdr = ws.Rows(1).Find(What:="sys_id", LookIn:=xlValues, LookAt:=xlWhole)
    If Not hdr Is Nothing Then
        sysIdCol = hdr.Column
        urlCol = ws.Cells(1, ws.Columns.Count).End(xlToLeft).Column + 1
        ws.Cells(1, urlCol).Value = "Record URL"
        
        Dim i As Long
        For i = 2 To lastRow
            If Len(ws.Cells(i, sysIdCol).Value) > 0 Then
                ws.Cells(i, urlCol).Value = inst & "/change_task.do?sys_id=" & ws.Cells(i, sysIdCol).Value
            End If
        Next i
        ws.Hyperlinks.Add Anchor:=ws.Range(ws.Cells(2, urlCol), ws.Cells(lastRow, urlCol)), _
                          Address:="", SubAddress:="", TextToDisplay:=""
    End If
    
    ws.Activate
    ws.Range("A1").Select
    MsgBox "cTasks refreshed.", vbInformation
    Exit Sub

fail:
    MsgBox "Error: " & Err.Description, vbCritical
End Sub

' URL-encodes a string (barebones)
Private Function URLEncode(ByVal s As String) As String
    Dim i As Long, ch As String, code As String
    For i = 1 To Len(s)
        ch = Mid$(s, i, 1)
        Select Case AscW(ch)
            Case 48 To 57, 65 To 90, 97 To 122 ' 0-9 A-Z a-z
                code = ch
            Case 32
                code = "%20"
            Case Else
                code = "%" & Right$("0" & Hex$(AscW(ch)), 2)
        End Select
        URLEncode = URLEncode & code
    Next i
End Function









https://yourinstance.service-now.com/api/now/table/change_task
  ?sysparm_display_value=all
  &sysparm_query=assigned_to.email=you@company.com^active=true
  &sysparm_fields=number,short_description,state,priority,assigned_to,change_request,opened_at,due_date,work_notes_list,sys_id
  &sysparm_limit=200