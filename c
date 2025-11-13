'===== SecureCRT VBScript: Open multiple SSH sessions using stored creds =====
'# $language = "VBScript"
'# $interface = "1.0"

Sub Main()

    '---- CHANGE THIS to your saved session name in SecureCRT ----
    baseSession = "Default"    ' Or "MySSH", or whatever session name stores your username/password

    '---- CHANGE THIS to your servers list file ----
    serverFile = "C:\servers.txt"

    Dim fso, file, line
    Set fso = CreateObject("Scripting.FileSystemObject")

    If Not fso.FileExists(serverFile) Then
        MsgBox "Server file not found: " & serverFile
        Exit Sub
    End If

    Set file = fso.OpenTextFile(serverFile, 1)

    Do While Not file.AtEndOfStream
        line = Trim(file.ReadLine)

        If line <> "" Then
            ' Build an ad-hoc session using existing credentials from saved session
            fullSession = "/S """ & baseSession & """ /SSH2 /P 22 /L " & crt.Session.LoggedOnUser & " " & line

            ' Open in a new tab
            crt.Session.ConnectInTab "/S " & baseSession & " /SSH2 /P 22 /C ""aes256-ctr"" /M ""hmac-sha2-256"" /HOST " & line
        End If
    Loop

    file.Close

End Sub