@echo off
setlocal EnableExtensions EnableDelayedExpansion

rem ======== CONFIG / ARG PARSING ========
set "USER="
set "PASS="
set "LOCAL_SH="
set "SERVERS_FILE="
set "OUT_DIR="
set "PORT=22"
set "HOSTKEY="

:parse
if "%~1"=="" goto after_parse
if /I "%~1"=="-u"  (set "USER=%~2" & shift & shift & goto parse)
if /I "%~1"=="-pw" (set "PASS=%~2" & shift & shift & goto parse)
if /I "%~1"=="-s"  (set "LOCAL_SH=%~2" & shift & shift & goto parse)
if /I "%~1"=="-l"  (set "SERVERS_FILE=%~2" & shift & shift & goto parse)
if /I "%~1"=="-o"  (set "OUT_DIR=%~2" & shift & shift & goto parse)
if /I "%~1"=="-p"  (set "PORT=%~2" & shift & shift & goto parse)
if /I "%~1"=="-hk" (set "HOSTKEY=%~2" & shift & shift & goto parse)
echo [ERROR] Unknown option: %~1
goto usage

:after_parse
if not defined USER        (echo [ERROR] -u <user> required & goto usage)
if not defined PASS        (echo [ERROR] -pw <password> required & goto usage)
if not defined LOCAL_SH    (echo [ERROR] -s <local_sh_file> required & goto usage)
if not defined SERVERS_FILE (echo [ERROR] -l <servers.txt> required & goto usage)
if not defined OUT_DIR     (echo [ERROR] -o <local_out_dir> required & goto usage)

if not exist "%LOCAL_SH%" (
  echo [ERROR] Local script not found: "%LOCAL_SH%"
  exit /b 1
)
if not exist "%SERVERS_FILE%" (
  echo [ERROR] Servers file not found: "%SERVERS_FILE%"
  exit /b 1
)
if not exist "%OUT_DIR%" (
  echo [INFO] Creating output directory "%OUT_DIR%"
  mkdir "%OUT_DIR%" || (echo [ERROR] Failed to create output dir & exit /b 1)
)

where pscp.exe >nul 2>&1 || (echo [ERROR] pscp.exe not in PATH & exit /b 1)
where plink.exe >nul 2>&1 || (echo [ERROR] plink.exe not in PATH & exit /b 1)

set "REMOTE_DIR=/tmp"
for %%A in ("%LOCAL_SH%") do set "REMOTE_SH_NAME=%%~nxA"
set "HKFLAG="
if defined HOSTKEY set "HKFLAG=-hostkey %HOSTKEY%"

echo.
echo ======== START ========

for /f "usebackq delims=" %%S in ("%SERVERS_FILE%") do (
  set "SERVER=%%S"
  if "!SERVER!"=="" (goto :continue)
  if "!SERVER:~0,1!"=="#" (goto :continue)

  echo.
  echo --- Processing !SERVER! ---

  rem 1) Upload
  pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% "%LOCAL_SH%" %USER%@!SERVER!:%REMOTE_DIR%/%REMOTE_SH_NAME%
  if errorlevel 1 (echo [ERROR] Upload failed for !SERVER! & goto :continue)

  rem 2) Run and get output file path
  set "REMOTE_CMD=bash -lc ""TS=$(date +%%Y%%m%%d_%%H%%M%%S); OUT='%REMOTE_DIR%/!SERVER!_'""$TS"".log; chmod +x '%REMOTE_DIR%/%REMOTE_SH_NAME%'; '%REMOTE_DIR%/%REMOTE_SH_NAME%' > ""$OUT"" 2>&1; echo ""$OUT"""""
  set "REMOTE_OUT_PATH="
  for /f "usebackq tokens=* delims=" %%O in (`
    plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! !REMOTE_CMD!
  `) do (
    set "REMOTE_OUT_PATH=%%O"
  )
  if not defined REMOTE_OUT_PATH (
    echo [ERROR] Execution failed on !SERVER!
    goto :continue
  )

  echo [INFO] Output file: !REMOTE_OUT_PATH!

  rem 3) Download
  pscp.exe -q -batch -P %PORT% -pw "%PASS%" %HKFLAG% %USER%@!SERVER!:"!REMOTE_OUT_PATH!" "%OUT_DIR%\"
  if errorlevel 1 (echo [ERROR] Download failed for !SERVER! & goto :continue)

  rem 4) Cleanup on server
  echo [STEP] Cleaning up remote files on !SERVER! ...
  plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! "rm -f '%REMOTE_DIR%/%REMOTE_SH_NAME%' '!REMOTE_OUT_PATH!'"
  if errorlevel 1 (echo [WARN] Cleanup failed on !SERVER!)

  echo [DONE] Finished !SERVER!
  :continue
)

echo.
echo ======== COMPLETE ========
exit /b 0

:usage
echo Usage:
echo   %~nx0 -u ^<user^> -pw ^<password^> -s ^<local_sh_file^> -l ^<servers.txt^> -o ^<local_out_dir^> [-p ^<port^>] [-hk ^<hostkey^>]
exit /b 2