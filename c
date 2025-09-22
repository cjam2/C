for /f "usebackq delims=" %%S in ("%SERVERS_FILE%") do call :process_server "%%S"
goto :done

:process_server
setlocal EnableExtensions EnableDelayedExpansion
set "SERVER=%~1"
if not defined SERVER exit /b
if "!SERVER:~0,1!"=="#" exit /b

echo --- Processing !SERVER! ---

rem 1) Upload
pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% "%LOCAL_SH%" %USER%@!SERVER!:%REMOTE_DIR%/%REMOTE_SH_NAME% || (
  echo [ERROR] Upload failed for !SERVER!
  exit /b
)

rem 2) Run & capture output path
set "REMOTE_OUT_PATH="
for /f "usebackq delims=" %%O in (`
  plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! %REMOTE_CMD%
`) do set "REMOTE_OUT_PATH=%%O"

if not defined REMOTE_OUT_PATH (
  echo [ERROR] No output path returned from !SERVER!
  exit /b
)

rem 3) Download
pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% %USER%@!SERVER!:"!REMOTE_OUT_PATH!" "%OUT_DIR%\" || (
  echo [ERROR] Download failed for !SERVER!
  exit /b
)

rem 4) Cleanup
plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
  "rm -f '%REMOTE_DIR%/%REMOTE_SH_NAME%' '!REMOTE_OUT_PATH!' 2>/dev/null || true"

echo [DONE] !SERVER!
exit /b

:done