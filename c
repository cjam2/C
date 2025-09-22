rem --- We are already inside :process_server with setlocal EnableDelayedExpansion ---
rem TMP_LIST contains one absolute path per line (only files starting with 10.*)

for /f "usebackq delims= eol=" %%F in ("%TMP_LIST%") do (
  rem 1) Read the line into a var and strip any trailing CR
  set "REMOTE_FILE=%%F"
  for /f "delims=" %%A in ("!REMOTE_FILE!") do set "REMOTE_FILE=%%~A"

  rem 2) Skip blank lines defensively
  if not "!REMOTE_FILE!"=="" (
    rem 3) Build a unique local name in Downloads (handles spaces)
    set "BASE=%%~nxF"
    set "LOCAL_FILE=%USERPROFILE%\Downloads\!SERVER!_!BASE!"

    echo [STEP] Downloading "!REMOTE_FILE!" -> "!LOCAL_FILE!"
    pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% ^
      %USER%@!SERVER!:"!REMOTE_FILE!" "!LOCAL_FILE!"
    if errorlevel 1 (
      echo [WARN] Download failed: !REMOTE_FILE!
    ) else (
      echo [INFO] Deleting remote file: !REMOTE_FILE!
      plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
        "rm -f -- '!REMOTE_FILE!'"
    )
  )
)