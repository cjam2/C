@echo off
REM ======================================================
REM download_from_servers_hardcoded.bat
REM Hardcoded username & password (insecure) — use with caution.
REM Usage:
REM   Place ips.txt in same folder (one IP or hostname per line).
REM   Edit the USER and PASSWORD variables below.
REM Example ips.txt:
REM   10.160.163.130
REM   10.160.163.131
REM ======================================================

setlocal

:: ----------------------------
:: CONFIGURE THESE (hardcoded)
:: ----------------------------
set "USER=ubuntu"
set "PASSWORD=MyPlainTextPassword123"
set "REMOTE_PATH=/tmp/results/*"
set "PORT=22"
:: ----------------------------

:: Input file containing IPs (one per line)
set "IPS_FILE=%~dp0ips.txt"

:: Destination base (folder next to script)
set "BASE_DEST=%~dp0downloads"
if "%BASE_DEST:~-1%"=="\" set "BASE_DEST=%BASE_DEST:~0,-1%"

echo ================================================
echo Running downloads with hardcoded credentials
echo   User:      %USER%
echo   Remote:    %REMOTE_PATH%
echo   IPs file:  %IPS_FILE%
echo   Port:      %PORT%
echo   Dest base: %BASE_DEST%
echo ================================================
echo.

if not exist "%IPS_FILE%" (
  echo ERROR: IPs file "%IPS_FILE%" not found. Create ips.txt with one host/IP per line.
  pause
  exit /b 3
)

if not exist "%BASE_DEST%" mkdir "%BASE_DEST%"

REM Loop through each IP/host in ips file
for /f "usebackq tokens=1 delims= " %%I in ("%IPS_FILE%") do (
  set "IP=%%I"
  call :process_host
)

echo.
echo All done.
pause
exit /b 0

:process_host
  echo -----------------------------------------
  echo Processing host: %IP%
  set "DEST=%BASE_DEST%\%IP%"

  if not exist "%DEST%" mkdir "%DEST%"

  REM Build PSCP command with -pw (plaintext)
  REM Note: -batch prevents interactive prompts; if server host key isn't known, pscp will fail with -batch.
  REM We will NOT use -batch so first-time host key prompts can be accepted automatically.
  set "PSCP_EXE=pscp.exe"
  set "PSCP_OPTS=-scp -r -P %PORT% -pw %PASSWORD%"

  REM Surround remote path in quotes for pscp
  setlocal enabledelayedexpansion
  set "REMOTE_SPEC=%USER%@%IP%:""%REMOTE_PATH%"""
  endlocal & set "REMOTE_SPEC=%REMOTE_SPEC%"

  setlocal enabledelayedexpansion
  set "CMDLINE=%PSCP_EXE% %PSCP_OPTS% %REMOTE_SPEC% "%DEST%""
  endlocal & set "CMDLINE=%CMDLINE%"

  echo Running: %CMDLINE%
  REM Execute command
  %CMDLINE%
  if errorlevel 1 (
    echo WARNING: download from %IP% returned non-zero exit code.
  ) else (
    echo OK: files downloaded to %DEST%
  )
  echo.
  goto :eof