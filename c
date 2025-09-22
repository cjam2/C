rem === 3a) List files created in /tmp/outputs on remote ===
set "REMOTE_FILES_LIST="
for /f "usebackq delims=" %%F in (`
  plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
    "find /tmp/outputs -type f 2>/dev/null"
`) do (
  set "REMOTE_FILES_LIST=!REMOTE_FILES_LIST! %%F"
)

if defined REMOTE_FILES_LIST (
  echo [INFO] Files to download from !SERVER!: !REMOTE_FILES_LIST!
  for %%F in (!REMOTE_FILES_LIST!) do (
    echo [STEP] Downloading %%F ...
    pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% %USER%@!SERVER!:"%%F" "%OUT_DIR%\"
    if errorlevel 1 (
      echo [WARN] Failed to download %%F from !SERVER!
    )
  )
) else (
  echo [INFO] No extra files found on !SERVER!.
)

rem === 3b) Optionally clean up remote output folder ===
plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
  "rm -rf /tmp/outputs" || echo [WARN] Could not remove /tmp/outputs on !SERVER!