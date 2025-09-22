@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem ================== CONFIG ==================
rem Linux login
set "USER=ubuntu"
set "PASS=MyPass123"
set "PORT=22"

rem Optional strict host key pinning (leave empty to skip)
rem Example: ssh-ed25519 255 SHA256:Zr8uW8Xy2vGAbcDEF12345abcd6789efgHIJKlmnopQ
set "HOSTKEY="

rem Use sudo? (0 = no, 1 = yes). If 1, it runs the .sh with sudo.
set "SUDO_MODE=0"

rem File names relative to this .bat's folder:
set "LOCAL_SH_NAME=my_script.sh"
set "SERVERS_FILE_NAME=servers.txt"
set "OUTPUTS_DIR_NAME=outputs"
rem ============================================

rem --- Paths based on the script folder ---
set "BASE_DIR=%~dp0"
set "LOCAL_SH=%BASE_DIR%%LOCAL_SH_NAME%"
set "SERVERS_FILE=%BASE_DIR%%SERVERS_FILE_NAME%"
set "OUT_DIR=%BASE_DIR%%OUTPUTS_DIR_NAME%"

rem --- Tools check ---
where pscp.exe >nul 2>&1 || (echo [ERROR] pscp.exe not found in PATH & goto _halt)
where plink.exe >nul 2>&1 || (echo [ERROR] plink.exe not found in PATH & goto _halt)

rem --- Inputs check ---
if not exist "%LOCAL_SH%" (echo [ERROR] Missing: %LOCAL_SH% & goto _halt)
if not exist "%SERVERS_FILE%" (echo [ERROR] Missing: %SERVERS_FILE% & goto _halt)
if not exist "%OUT_DIR%" (mkdir "%OUT_DIR%" || (echo [ERROR] Cannot create %OUT_DIR% & goto _halt))

set "REMOTE_DIR=/tmp"
for %%A in ("%LOCAL_SH%") do set "REMOTE_SH_NAME=%%~nxA"

set "HKFLAG="
if defined HOSTKEY set "HKFLAG=-hostkey %HOSTKEY%"

echo.
echo ===== RUN REMOTE SCRIPT (Configured) =====
echo User      : %USER%
echo Port      : %PORT%
echo Script    : %LOCAL_SH%
echo Servers   : %SERVERS_FILE%
echo Outputs   : %OUT_DIR%
if defined HOSTKEY echo HostKey   : %HOSTKEY%
echo Sudo mode : %SUDO_MODE%
echo =========================================

rem ===== MAIN LOOP =====
for /f "usebackq delims=" %%S in ("%SERVERS_FILE%") do (
  set "SERVER=%%S"
  if "!SERVER!"=="" (goto _next)
  if "!SERVER:~0,1!"=="#" (goto _next)

  echo.
  echo --- Processing !SERVER! ---

  rem 1) Upload script
  pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% "%LOCAL_SH%" %USER%@!SERVER!:%REMOTE_DIR%/%REMOTE_SH_NAME%
  if errorlevel 1 (echo [ERROR] Upload failed for !SERVER! & goto _next)

  rem 2) Build remote command
  if "%SUDO_MODE%"=="1" (
    set "RUNNER=sudo '%REMOTE_DIR%/%REMOTE_SH_NAME%'"
    set "RUNNER=bash -lc ""sudo '%REMOTE_DIR%/%REMOTE_SH_NAME%'"""
  ) else (
    set "RUNNER=bash -lc ""'%REMOTE_DIR%/%REMOTE_SH_NAME%'"""
  )

  rem We need one bash -lc to: make OUT path, chmod +x, run, echo OUT
  set "REMOTE_CMD=bash -lc ""TS=$(date +%%Y%%m%%d_%%H%%M%%S); OUT='%REMOTE_DIR%/!SERVER!_'""$TS"".log; chmod +x '%REMOTE_DIR%/%REMOTE_SH_NAME%'; "
  if "%SUDO_MODE%"=="1" (
    rem if sudo needs password, uncomment the next line and remove the line after it
    rem set "REMOTE_CMD=!REMOTE_CMD! echo %PASS% | sudo -S '%REMOTE_DIR%/%REMOTE_SH_NAME%' > ""$OUT"" 2>&1; echo ""$OUT"""
    set "REMOTE_CMD=!REMOTE_CMD! sudo '%REMOTE_DIR%/%REMOTE_SH_NAME%' > ""$OUT"" 2>&1; echo ""$OUT"""
  ) else (
    set "REMOTE_CMD=!REMOTE_CMD! '%REMOTE_DIR%/%REMOTE_SH_NAME%' > ""$OUT"" 2>&1; echo ""$OUT"""
  )

  rem 3) Execute and capture the printed OUT path
  set "REMOTE_OUT_PATH="
  for /f "usebackq tokens=* delims=" %%O in (`
    plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! !REMOTE_CMD!
  `) do (
    set "REMOTE_OUT_PATH=%%O"
  )

  if not defined REMOTE_OUT_PATH (
    echo [ERROR] No output path returned from remote on !SERVER!
    goto _cleanup_try
  )

  echo [INFO] Remote output file: !REMOTE_OUT_PATH!

  rem 4) Download the output file
  pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% %USER%@!SERVER!:"!REMOTE_OUT_PATH!" "%OUT_DIR%\"
  if errorlevel 1 (echo [ERROR] Download failed for !SERVER! & goto _cleanup_try)

  rem 5) Cleanup on remote (uploaded .sh and generated log)
  :_cleanup_try
  plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! "rm -f '%REMOTE_DIR%/%REMOTE_SH_NAME%' '!REMOTE_OUT_PATH!' 2>/dev/null || true"
  if errorlevel 1 (echo [WARN] Cleanup may have failed on !SERVER!)
  echo [DONE] !SERVER!
  :_next
)

goto _done

:_halt
echo.
echo Press any key to close...
pause >nul
exit /b 1

:_done
echo.
echo ===== COMPLETE =====
echo Logs downloaded to: %OUT_DIR%
echo.
echo Press any key to close...
pause >nul
exit /b 0