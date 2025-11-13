'===== SecureCRT Script: Open multiple SSH sessions using saved creds =====
'# $language = "VBScript"
'# $interface = "1.0"

Sub Main()

    '---- CHANGE THIS to your saved session name ----
    baseSession = "MySSHProfile"   ' A saved session with stored username/password

    '---- CHANGE THIS to your server list file ----
    serverFile = "C:\servers.txt"

    Dim fso, file, ip
    Set fso = CreateObject("Scripting.FileSystemObject")

    If Not fso.FileExists(serverFile) Then
        MsgBox "Server list file not found: " & serverFile
        Exit Sub
    End If

    Set file = fso.OpenTextFile(serverFile, 1)

    Do While Not file.AtEndOfStream
        ip = Trim(file.ReadLine)

        If ip <> "" Then
            ' Create a new tab using the saved session, overriding only the hostname
            crt.Session.ConnectInTab "/S """ & baseSession & """ /HOST " & ip
        End If
    Loop

    file.Close

End Sub