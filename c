rem === Choose the remote dir whose files you want to pull ===
set "REMOTE_PICK_DIR=%REMOTE_DIR%"   rem e.g. /tmp or /tmp/outputs

rem === Where to save locally ===
set "DL_DIR=%USERPROFILE%\Downloads"

rem === Get a newline-separated list of files (no recursion) to a temp file ===
set "TMP_LIST=%TEMP%\files_!SERVER!_!TS!.txt"
plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
  "find '%REMOTE_PICK_DIR%' -maxdepth 1 -type f -printf '%%p\n'" > "%TMP_LIST%"

rem === Download each file and delete it on remote if success ===
for /f "usebackq delims=" %%F in ("%TMP_LIST%") do (
  set "REMOTE_FILE=%%F"
  set "BASE=%%~nxF"
  set "LOCAL_FILE=%DL_DIR%\!SERVER!_!BASE!"
  echo [STEP] Downloading "%%F" -> "!LOCAL_FILE!"

  pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% ^
    %USER%@!SERVER!:"%%F" "!LOCAL_FILE!"
  if errorlevel 1 (
    echo [WARN] Download failed for %%F (kept on server)
  ) else (
    echo [INFO] Downloaded. Deleting remote file...
    plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
      "rm -f -- '%%F'" || echo [WARN] Could not delete %%F on server
  )
)

del "%TMP_LIST%" 2>nul