Option Explicit

Private Sub Workbook_Open()
    On Error GoTo EH
    Jira_RefreshToSheet
    Exit Sub
EH:
    MsgBox "Jira refresh failed: " & Err.Description, vbExclamation
End Sub




Option Explicit

' Requires JsonConverter.bas (VBA-JSON) imported into the project.
' Tools -> References: enable "Microsoft Scripting Runtime" (recommended)

Public Sub Jira_RefreshToSheet()
    Dim baseUrl As String, user As String, secret As String, jql As String
    Dim maxResults As Long
    Dim wsOut As Worksheet
    
    baseUrl = Trim(GetCfg("JiraBaseUrl"))
    user = Trim(GetCfg("Username"))
    secret = Trim(GetCfg("ApiTokenOrPassword"))
    jql = Trim(GetCfg("JQL"))
    maxResults = CLng(Val(GetCfg("MaxResults")))
    If maxResults <= 0 Then maxResults = 100
    
    If Right$(baseUrl, 1) = "/" Then baseUrl = Left$(baseUrl, Len(baseUrl) - 1)
    
    Set wsOut = ThisWorkbook.Worksheets("JiraData")
    wsOut.Cells.Clear
    
    ' Headers
    Dim headers As Variant
    headers = Array("Key", "Summary", "Status", "Assignee", "Priority", "Created", "Updated", "IssueType")
    wsOut.Range("A1").Resize(1, UBound(headers) + 1).Value = headers
    
    Dim startAt As Long: startAt = 0
    Dim rowOut As Long: rowOut = 2
    Dim total As Long: total = -1
    
    Do
        Dim url As String
        url = baseUrl & "/rest/api/3/search" & _
              "?jql=" & UrlEncode(jql) & _
              "&startAt=" & startAt & _
              "&maxResults=" & maxResults & _
              "&fields=" & UrlEncode("summary,status,assignee,priority,created,updated,issuetype")
        
        Dim jsonText As String
        jsonText = HttpGet(url, user, secret)
        
        Dim json As Object
        Set json = JsonConverter.ParseJson(jsonText)
        
        If total = -1 Then total = CLng(json("total"))
        
        Dim issues As Collection
        Set issues = json("issues")
        
        Dim i As Long
        For i = 1 To issues.Count
            Dim issue As Object, fields As Object
            Set issue = issues(i)
            Set fields = issue("fields")
            
            Dim k As String: k = NzStr(issue("key"))
            Dim summary As String: summary = NzStr(fields("summary"))
            Dim status As String: status = NzStr(fields("status")("name"))
            
            Dim assignee As String
            If HasKey(fields, "assignee") And Not IsNull(fields("assignee")) Then
                ' Jira Cloud often provides displayName
                If HasKey(fields("assignee"), "displayName") Then
                    assignee = NzStr(fields("assignee")("displayName"))
                ElseIf HasKey(fields("assignee"), "name") Then
                    assignee = NzStr(fields("assignee")("name"))
                Else
                    assignee = ""
                End If
            Else
                assignee = ""
            End If
            
            Dim priority As String
            If HasKey(fields, "priority") And Not IsNull(fields("priority")) Then
                priority = NzStr(fields("priority")("name"))
            Else
                priority = ""
            End If
            
            Dim created As String: created = NzStr(fields("created"))
            Dim updated As String: updated = NzStr(fields("updated"))
            
            Dim issueType As String
            If HasKey(fields, "issuetype") And Not IsNull(fields("issuetype")) Then
                issueType = NzStr(fields("issuetype")("name"))
            Else
                issueType = ""
            End If
            
            wsOut.Cells(rowOut, 1).Value = k
            wsOut.Cells(rowOut, 2).Value = summary
            wsOut.Cells(rowOut, 3).Value = status
            wsOut.Cells(rowOut, 4).Value = assignee
            wsOut.Cells(rowOut, 5).Value = priority
            wsOut.Cells(rowOut, 6).Value = created
            wsOut.Cells(rowOut, 7).Value = updated
            wsOut.Cells(rowOut, 8).Value = issueType
            
            rowOut = rowOut + 1
        Next i
        
        startAt = startAt + maxResults
        
        If startAt >= total Then Exit Do
        If issues.Count = 0 Then Exit Do
    Loop
    
    ' Make it nicer
    wsOut.Columns.AutoFit
    wsOut.Range("A1").AutoFilter
    
    MsgBox "Jira refresh complete. Issues loaded: " & (rowOut - 2), vbInformation
End Sub

' -----------------------------
' Helpers
' -----------------------------

Private Function GetCfg(ByVal keyName As String) As String
    Dim ws As Worksheet: Set ws = ThisWorkbook.Worksheets("Config")
    Dim rng As Range
    Set rng = ws.Columns(1).Find(What:=keyName, LookAt:=xlWhole, MatchCase:=False)
    If rng Is Nothing Then
        Err.Raise vbObjectError + 1000, , "Missing Config key: " & keyName
    End If
    GetCfg = CStr(rng.Offset(0, 1).Value)
End Function

Private Function HttpGet(ByVal url As String, ByVal user As String, ByVal secret As String) As String
    ' Uses Basic Auth: base64(user:secret)
    ' Jira Cloud: user = email, secret = API token (recommended)
    ' Jira Server/DC: may work with password, or use PAT if available
    
    Dim http As Object
    Set http = CreateObject("WinHttp.WinHttpRequest.5.1")
    
    http.Open "GET", url, False
    http.SetRequestHeader "Accept", "application/json"
    http.SetRequestHeader "Authorization", "Basic " & Base64Encode(user & ":" & secret)
    
    http.Send
    
    If http.Status < 200 Or http.Status >= 300 Then
        Err.Raise vbObjectError + 1001, , "HTTP " & http.Status & " - " & http.StatusText & vbCrLf & http.ResponseText
    End If
    
    HttpGet = http.ResponseText
End Function

Private Function Base64Encode(ByVal plain As String) As String
    Dim bytes() As Byte
    bytes = StrConv(plain, vbFromUnicode)
    
    Dim dom As Object, el As Object
    Set dom = CreateObject("MSXML2.DOMDocument.6.0")
    Set el = dom.createElement("b64")
    el.DataType = "bin.base64"
    el.nodeTypedValue = bytes
    Base64Encode = Replace(el.Text, vbLf, "")
End Function

Private Function UrlEncode(ByVal s As String) As String
    ' Simple URL encoder for query params
    Dim i As Long, ch As String, out As String, code As Long
    For i = 1 To Len(s)
        ch = Mid$(s, i, 1)
        code = AscW(ch)
        Select Case code
            Case 48 To 57, 65 To 90, 97 To 122
                out = out & ch
            Case 45, 46, 95, 126
                out = out & ch
            Case 32
                out = out & "%20"
            Case Else
                out = out & "%" & Right$("0" & Hex(code), 2)
        End Select
    Next i
    UrlEncode = out
End Function

Private Function NzStr(ByVal v As Variant) As String
    If IsNull(v) Or IsEmpty(v) Then
        NzStr = ""
    Else
        NzStr = CStr(v)
    End If
End Function

Private Function HasKey(ByVal dict As Object, ByVal key As String) As Boolean
    On Error GoTo EH
    Dim tmp As Variant
    tmp = dict(key)
    HasKey = True
    Exit Function
EH:
    HasKey = False
End Function