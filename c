rem 2a) Get remote timestamp (server time)
set "TS="
for /f "usebackq delims=" %%T in (`
  plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
    "date +%%Y%%m%%d_%%H%%M%%S"
`) do set "TS=%%T"

if not defined TS (
  echo [ERROR] Could not read remote timestamp from !SERVER!
  exit /b
)

rem 2b) Build the remote output path locally (uses server IP + server timestamp)
set "REMOTE_OUT_PATH=%REMOTE_DIR%/!SERVER!_!TS!.log"
echo [INFO] Using remote output: !REMOTE_OUT_PATH!

rem 2c) Make the uploaded script executable
plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
  "chmod +x '%REMOTE_DIR%/%REMOTE_SH_NAME%'"
if errorlevel 1 (
  echo [ERROR] chmod failed on !SERVER!
  exit /b
)

rem 2d) Run the script and redirect both stdout+stderr to the remote log
rem     (bash -lc ensures PATH/shell init; careful with quotes)
plink.exe -batch -P %PORT% -pw "%PASS%" %HKFLAG% -ssh %USER%@!SERVER! ^
  bash -lc "'%REMOTE_DIR%/%REMOTE_SH_NAME%' > '!REMOTE_OUT_PATH!' 2>&1"
if errorlevel 1 (
  echo [ERROR] Remote execution failed on !SERVER!
  exit /b
)