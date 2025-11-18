'===== SecureCRT Script: Open multiple SSH sessions using saved creds =====
'# $language = "VBScript"
'# $interface = "1.0"

Sub Main()

    baseSession = "MySSHProfile"       ' Your saved session with creds
    serverFile  = "C:\servers.txt"     ' Your server list

    Dim fso, file, ip
    Set fso = CreateObject("Scripting.FileSystemObject")

    If Not fso.FileExists(serverFile) Then
        MsgBox "Server list not found: " & serverFile
        Exit Sub
    End If

    Set file = fso.OpenTextFile(serverFile, 1)

    Do While Not file.AtEndOfStream
        ip = Trim(file.ReadLine)

        If ip <> "" Then
            connectString = "/S """ & baseSession & """ " & ip
            
            '--- CONNECT in a new tab ---
            Dim tab
            Set tab = crt.Session.ConnectInTab(connectString, True, True)

            '--- HANDLE NEW HOST KEY PROMPT ---
            ' SecureCRT will pause for a host key dialog if unknown
            If tab.Session.Connected = False Then
                ' Wait for a possible dialog
                crt.Sleep 500
                
                If crt.Dialog.Message <> "" Then
                    ' Automatically choose Accept & Save (button index 1)
                    ' Buttons: 0=Cancel, 1=Accept & Save, 2=Accept
                    crt.Dialog.PressButton 1
                End If
            End If

        End If
    Loop

    file.Close

End Sub